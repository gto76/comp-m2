#include "interactive_mode.hpp"

#include <unistd.h>
#include <cmath>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

#include "comp.hpp"
#include "computer.hpp"
#include "cursor.hpp"
#include "drawing3D.hpp"
#include "drawing3Db.hpp"
#include "drawing2D.hpp"
#include "load.hpp"
#include "output.hpp"
#include "printer.hpp"
#include "ram.hpp"
#include "random_input.hpp"
#include "renderer.hpp"
#include "view.hpp"

using namespace std;

extern "C" {
  extern volatile sig_atomic_t pleaseExit;
  void setEnvironment();
}

// MAIN
void startInteractiveMode(string filename);
void selectView();
void prepareOutput();
void drawScreen();
// EXECUTION MODE
void run();
void exec();
void sleepAndCheckForKey();
// EDIT MODE
void userInput();
void isertCharIntoRam(char c);
void processInputWithShift(char c);
bool insertNumberIntoRam(char c);
void engageInsertCharMode();
void engageInsertNumberMode();
void switchDrawing();
// SAVE
void saveRamToNewFile();
void saveRamToCurrentFile();
string getFreeFileName();
string getGenericFileName(int index);
void saveRamToFile(string filename);
// KEY READER
char readStdin(bool drawCursor);

//////////////////////
//////// VARS ////////
//////////////////////

View view3d = View(drawing3D, LIGHTBULB_ON_3D, LIGHTBULB_OFF_3D);
View view3db = View(drawing3Db, LIGHTBULB_ON_3D_B, LIGHTBULB_OFF_3D_B); 
View view2d = View(drawing2D, LIGHTBULB_ON_2D, LIGHTBULB_OFF_2D);
View *selectedView = &view3d;
RandomInput input;
Computer computer = Computer(redrawScreen, sleepAndCheckForKey);
Printer printer = Printer(computer, redrawScreen, sleepAndCheckForKey);
Cursor cursor = Cursor(computer.ram);

// Whether esc was pressed during execution.
bool executionCanceled = false;
// Filename.
string loadedFilename;
bool fileSaved = true;
// Number of executions.
int executionCounter = 0;
// Saved state of a ram. Loaded after execution ends.
map<AddrSpace, vector<vector<bool>>> savedRamState;
// Whether next key should be read as a char whose value shall thence be
// inserted into ram.
bool insertChar = false;
bool insertNumber = false;
vector<int> digits;
bool shiftPressed = false;
// Copy/paste.
vector<bool> clipboard = EMPTY_WORD;

//////////////////////
//////// MAIN ////////
//////////////////////

void InteractiveMode::startInteractiveMode(string filename) {
  computer.ram.input = &input;
  executionCanceled = false;
  if (filename != "") {
    Load::fillRamWithFile(filename.c_str(), computer.ram);
    loadedFilename = filename;
  }
  selectView();
  setEnvironment();
  prepareOutput();
  clearScreen();
  redrawScreen();
  userInput();
}

void selectView() {
  const char* term = std::getenv("TERM");
  if (strcmp(term, "linux") == 0) {
    selectedView = &view3db;
  } else if (strcmp(term, "rxvt") == 0) {
    selectedView = &view2d;
  }
}

/*
 * Initializes 'output.cpp' by sending dimensions of a 'drawing' and 
 * a 'drawScreen' callback function, that output.c will use on every 
 * screen redraw.
 */
void prepareOutput() {
  setOutput(&drawScreen, selectedView->width, selectedView->height);
}

void drawScreen() {
  vector<vector<string>> buffer = Renderer::renderState(printer, computer.ram, computer.cpu, 
                                                        cursor, *selectedView);
  int i = 0;
  for (vector<string> line : buffer) {
    replaceLine(line, i++);
  }
}

//////////////////////
/// EXECUTION MODE ///
//////////////////////

/*
 * Saves the state of the ram and starts the execution of a program.
 * When execution stops, due to it reaching last address or user pressing 
 * 'esc', it loads back the saved state of the ram, and resets the cpu.
 */
void run() {
  savedRamState = computer.ram.state;
  printer.run();
  // If 'esc' was pressed then it doesn't wait for keypress at the end.
  if (executionCanceled) {
    executionCanceled = false;
  } else {
    readStdin(false);
  }
  computer.ram.state = savedRamState;
  computer.cpu.reset();
  redrawScreen();
  executionCounter++;
}

/*
 * Runs every cycle.
 */
void sleepAndCheckForKey() {
  usleep(FQ*1000);
  // Exits if ctrl-c was pressed.
  if (pleaseExit) {
    exit(0);
  }
  // Pauses execution if a key was hit, and waits for another key hit.
  if (int keyCode = Util::getKey()) {
    // If escape was pressed.
    if (keyCode == 27) {
      executionCanceled = true;
      return;
    }
    // "Press key to continue."
    keyCode = readStdin(false);
    // If esc key was pressed.
    if (keyCode == 27) {
      executionCanceled = true;
    }
    // If s or S was pressed saves to file.
    if (keyCode == 115) {
      saveRamToNewFile();
      executionCanceled = true;
    }
    if (keyCode == 83) {
      saveRamToCurrentFile();
      executionCanceled = true;
    }
  }
}

////////////////////
/// EDITING MODE ///
////////////////////

void userInput() {
  while(1) {
    char c = readStdin(true);
    if (insertChar) {
      isertCharIntoRam(c);
      fileSaved = false;
    } else if (shiftPressed) {
      processInputWithShift(c);
    } else {
      if (insertNumber) {
        if (insertNumberIntoRam(c)) {
          fileSaved = false;
          continue;
        }
      }
      switch (c) {   
        // BEGIN EXECUTION
        case 10:   // enter
          run();
          break;

        // MODES
        case 50:   // 2
          shiftPressed = true;
          break;
        case 105:  // i
          engageInsertCharMode();
          break;
        case 73:   // I
          engageInsertNumberMode();
          break;

        // VIEWS
        case 118:  // v
          switchDrawing();
          break;

        // SAVE
        case 115: { // s
          if (!fileSaved) {
            saveRamToNewFile();
            fileSaved = true;
          }
          break;
        }
        case 83: {  // S
          if (!fileSaved) {
            saveRamToCurrentFile();
            fileSaved = true;
          }
          break;
        }

        // QUIT
        case 81:   // Q
          exit(0);
          break;
        case 113: { // q
          if (!fileSaved) {
            saveRamToNewFile();
          }
          exit(0);
          break;          
        }
        
        // BASIC MOVEMENT
        case 107:  // k
        case 65:   // A, part of escape seqence of up arrow
          cursor.decreaseY();
          break;
        case 106:  // j
        case 66:   // B, part of escape seqence of down arrow
          cursor.increaseY();
          break;
        case 108:  // l
        case 67:   // C, part of escape seqence of rigth arrow
          cursor.increaseX();
          break;
        case 104:  // h
        case 68:   // D, part of escape seqence of left arrow
          cursor.decreaseX();
          break;
        case 116:  // t
        case 9:    // tab
          cursor.switchAddressSpace();
          break;
        case 72:   // H (home)
          cursor.setBitIndex(0);
          break;
        case 70:   // F (end)
          cursor.setBitIndex(WORD_SIZE-1);

        // VIM MOVEMENT
        case 103:  // g
          cursor.setByteIndex(0);
          break;
        case 71:   // G
          cursor.setByteIndex(RAM_SIZE-1);
          break; 
        case 101:  // e
          cursor.goToEndOfWord();
          break;
        case 98:   // b
          cursor.goToBeginningOfWord();
          break;
        case 119:  // w
          cursor.goToBeginningOfNextWord();
          break;
        case 97:   // a
          cursor.setBitIndex(4);
          break;
        case 36:   // $
          cursor.setBitIndex(WORD_SIZE-1);
          break;
        case 94:   // ^
          cursor.setBitIndex(0);
          break;        
        case 122:  // z
        case 90:   // shift + tab
        case 84:   // T
          cursor.goToInstructionsAddress();
          break;

        // BASIC MANIPULATION
        case 32:   // space
          cursor.switchBit();
          fileSaved = false;
          break;
        case 51: { // 3, part of escape seqence of delete key
          vector<bool> temp = cursor.getWord();
          bool success = cursor.deleteByteAndMoveRestUp();
          if (success) {
            clipboard = temp;
            fileSaved = false;
          }
          break;  
        }      
        case 75:   // K
        case 53:   // 5, part of escape seqence of page up
          cursor.moveByteUp();
          fileSaved = false;
          break;
        case 74:   // J
        case 54:   // 6, part of escape seqence of page down
          cursor.moveByteDown();
          fileSaved = false;
          break;

        // VIM MANIPULATION
        case 102:  // f
          cursor.setBit(true);
          cursor.increaseX();
          fileSaved = false;
          break;
        case 100:  // d
          cursor.setBit(false);
          cursor.increaseX();
          fileSaved = false;
          break;
        case 120:  // x
          clipboard = cursor.getWord();
          cursor.eraseByte();
          fileSaved = false;
          // cursor.setBitIndex(0);
          break;
        case 88: { // X
          vector<bool> temp = cursor.getWord();
          bool success = cursor.deleteByteAndMoveRestUp();
          if (success) {
            clipboard = temp;
            fileSaved = false;
          }
          break; 
        } case 111: { // o
          cursor.increaseY();
          bool success = cursor.insertByteAndMoveRestDown();
          if (success) {
            cursor.setBitIndex(0);
            fileSaved = false;
          } else {
            cursor.decreaseY();
          }
          break;
        }
        case 121:  // y
        case 99:   // c
          clipboard = cursor.getWord();
          break;
        case 112:  // p
          cursor.setWord(clipboard);
          fileSaved = false;
          break;
        case 80: { // P
          bool success = cursor.insertByteAndMoveRestDown();
          if (success) {
            cursor.setWord(clipboard);
          fileSaved = false;
          }
          break;
        }
      }
    }
    redrawScreen();
  }
}

void isertCharIntoRam(char c) {
  insertChar = false;
  if (c == 27) {  // Esc
    return;
  }
  cursor.setWord(Util::getBoolByte(c));
  cursor.increaseY();
}

void processInputWithShift(char c) {
  shiftPressed = false;
  if (c == 65) {           // A, part of up arrow
    cursor.moveByteUp();
  } else if (c == 66) {    // B, part of down arrow
    cursor.moveByteDown();
  } else if (c == 126) {   // ~, part of insert key (also is 2)
    cursor.insertByteAndMoveRestDown();
  }
}

// Returns whether the loop should continue.
bool insertNumberIntoRam(char c) {
  if (c < 48 || c > 57) {
    digits = vector<int>();
    insertNumber = false;
    return false;
  }
  digits.insert(digits.begin(), c - 48);
  int numbersValue = 0;
  int i = 0;
  for (int digit : digits) {
    numbersValue += digit * pow(10, i++);
  }
  cursor.setWord(Util::getBoolByte(numbersValue));
  redrawScreen();
  return true;
}

void engageInsertCharMode() {
  if (cursor.getAddressSpace() == DATA) {
    insertChar = true;
  }
}

void engageInsertNumberMode() {
  if (cursor.getAddressSpace() == DATA) {
    insertNumber = true;
  }
}

void switchDrawing() {
  if (*selectedView == view3d) {
    selectedView = &view3db;
  } else if (*selectedView == view3db) {
    selectedView = &view2d;
  } else {
    selectedView = &view3d;
  }
  prepareOutput();
  clearScreen();
  redrawScreen();
}

////////////
/// SAVE ///
////////////

void saveRamToNewFile() {
  string fileName = getFreeFileName();
  saveRamToFile(fileName);
  loadedFilename = fileName;
}

void saveRamToCurrentFile() {
  string fileName;
  if (loadedFilename == "") {
    fileName = getFreeFileName();
  } else {
    fileName = loadedFilename;
  }
  saveRamToFile(fileName);
}

string getFreeFileName() {
  int i = 0;
  while (Util::fileExists(getGenericFileName(++i)));
  return getGenericFileName(i);
}

string getGenericFileName(int index) {
  string fileNumber;
  if (index > 9) {
    fileNumber = to_string(index);
  } else {
    fileNumber = "0" + to_string(index);
  }
  return SAVE_FILE_NAME + fileNumber + '.' + FILE_EXTENSION;
}

void saveRamToFile(string fileName) {
  ofstream fileStream(fileName);
  bool computerRunning = computer.cpu.getCycle() != 0;
  if (computerRunning) {
    fileStream << Ram::stateToString(savedRamState);
  } else {
    fileStream << computer.ram.getString();
  }
  fileStream.close();
}

//////////////////
/// KEY READER ///
//////////////////

char readStdin(bool drawCursor) {
  char c = 0;
  errno = 0;
  ssize_t num = read(0, &c, 1);
  if (num == -1 && errno == EINTR) {
    // Exits if ctrl-c was pressed.
    if (pleaseExit) {
      exit(0);
    }
    redrawScreen();
    return readStdin(drawCursor);
  }
  return c;
}

