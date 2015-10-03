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

// remove
#include <numeric>
#include <iostream>

using namespace std;

extern "C" {
  extern volatile sig_atomic_t pleaseExit;
  void setEnvironment();
}

// MAIN
void startInteractiveMode(string filename);
void selectView();
void prepareOutput();
void updateBuffer();
// EXECUTION MODE
void run();
void exec();
void sleepAndCheckForKey();
// EDIT MODE
void userInput();
bool switchKey(char c);
void isertCharIntoRam(char c);
bool processInputWithShift(char c);
bool insertNumberIntoRam(char c);
void engageInsertCharMode();
void engageInsertNumberMode();
void switchDrawing(bool direction);
// SAVE
void save();
void saveAs();
string getFreeFileName();
string getGenericFileName(int index);
void saveRamToFile(string filename);
// KEY READER
char readStdin();

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
// volatile sig_atomic_t shiftPressed = 0;
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
    selectedView = &view2d;
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
  initOutput(&updateBuffer, selectedView->width, selectedView->height);
}

void updateBuffer() {
  vector<vector<string>> tmp = Renderer::renderState(printer, computer.ram,
                                                     computer.cpu, cursor,
                                                     *selectedView);
  int i = 0;
  for (vector<string> line : tmp) {
    replaceBufferLine(line, i++);
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
  computer.cpu.switchOn();
  redrawScreen();
  sleepAndCheckForKey();
  printer.run();
  // If 'esc' was pressed then it doesn't wait for keypress at the end.
  if (executionCanceled) {
    executionCanceled = false;
  } else {
    readStdin();
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
  if (screenResized) {
    redrawScreen();
  }
  // Pauses execution if a key was hit, and waits for another key hit.
  if (int keyCode = Util::getKey()) {
    // Cancels execution if escape was pressed.
    if (keyCode == 27) {
      executionCanceled = true;
      return;
    }
    // "Press key to continue."
    keyCode = readStdin();
    // Cancels execution if escape or tab was pressed.
    if (keyCode == 27 || keyCode == 9) {
      executionCanceled = true;
    }
    // If s was pressed - save.
    if (keyCode == 115) {
      save();
      executionCanceled = true;
    }
    // If S was pressed - save as.
    if (keyCode == 83) {
      saveAs();
      executionCanceled = true;
    }
  }
}

////////////////////
/// EDITING MODE ///
////////////////////

void userInput() {
  while(1) {
    char c = readStdin();
    if (insertChar) {
      isertCharIntoRam(c);
      fileSaved = false;
    } else if (shiftPressed) {
      bool shouldContinue = !processInputWithShift(c);
      if (shouldContinue) {
        continue;
      }
    } else {
      if (insertNumber) {
        if (insertNumberIntoRam(c)) {
          fileSaved = false;
          continue;
        }
      }
      bool shouldContinue = switchKey(c);
      if (shouldContinue) {
        continue;
      }
    }
    redrawScreen();
  }
}

bool switchKey(char c) {
  switch (c) {
    // BEGIN EXECUTION
    case 10:    // enter
      run();
      break;
    // MODES
    case 50:    // 2
      shiftPressed = 1;
      return true;
    case 105:   // i
      engageInsertCharMode();
      break;
    case 73:    // I
      engageInsertNumberMode();
      break;
    // VIEWS
    case 44:    // ,
      switchDrawing(true);
      break;
    case 46:    // .
      switchDrawing(false);
      break;
    // SAVE
    case 115:   // s
      save();
      break;
    case 83:    // S
      saveAs();
      break;
    // QUIT
    case 81:    // Q
      exit(0);
      break;
    case 113: { // q
      if (!fileSaved) {
        saveAs();
      }
      exit(0);
      break;
    }
    // BASIC MOVEMENT
    case 107:   // k
    case 65:    // A, part of escape seqence of up arrow
      cursor.decreaseY();
      break;
    case 106:   // j
    case 66:    // B, part of escape seqence of down arrow
      cursor.increaseY();
      break;
    case 108:   // l
    case 67:    // C, part of escape seqence of rigth arrow
      cursor.increaseX();
      break;
    case 104:   // h
    case 68:    // D, part of escape seqence of left arrow
      cursor.decreaseX();
      break;
    case 116:   // t
    case 9:     // tab
      cursor.switchAddressSpace();
      break;
    case 72:    // H (home)
    case 94:    // ^
      cursor.setBitIndex(0);
      break;
    case 70:    // F (end)
    case 36:    // $
      cursor.setBitIndex(WORD_SIZE-1);
      break;
    // VIM MOVEMENT
    case 103:   // g
      cursor.setByteIndex(0);
      break;
    case 71:    // G
      cursor.setByteIndex(RAM_SIZE-1);
      break;
    case 101:   // e
      cursor.goToEndOfWord();
      break;
    case 98:    // b
      cursor.goToBeginningOfWord();
      break;
    case 119:   // w
      cursor.goToBeginningOfNextWord();
      break;
    case 97:    // a
      cursor.setBitIndex(4);
      break;
    case 122:   // z
    case 90:    // shift + tab
    case 84:    // T
      cursor.goToInstructionsAddress();
      break;
    // BASIC MANIPULATION
    case 32:    // space
      cursor.switchBit();
      fileSaved = false;
      break;
    case 51:    // 3, part of escape seqence of delete key
    case 127:   // backspace
    case 120: { // x
      vector<bool> temp = cursor.getWord();
      bool success = cursor.deleteByteAndMoveRestUp();
      if (success) {
        fileSaved = false;
      } else {
        clipboard = temp;
      }
      break;
    }
    case 75:    // K
    case 53:    // 5, part of escape seqence of page up
      cursor.moveByteUp();
      fileSaved = false;
      break;
    case 74:    // J
    case 54:    // 6, part of escape seqence of page down
      cursor.moveByteDown();
      fileSaved = false;
      break;
    // VIM MANIPULATION
    case 102:   // f
      cursor.setBit(true);
      cursor.increaseX();
      fileSaved = false;
      break;
    case 100:   // d
      cursor.setBit(false);
      cursor.increaseX();
      fileSaved = false;
      break;
    case 111: { // o
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
    case 59: {  // ;
      cursor.insertByteAndMoveRestDown();
      fileSaved = false;
      break;
    }
    case 99:    // c
    case 121:   // y
      clipboard = cursor.getWord();
      break;
    case 118:   // v
    case 112:   // p
      cursor.setWord(clipboard);
      fileSaved = false;
      break;
    case 80: {  // P
      bool success = cursor.insertByteAndMoveRestDown();
      if (success) {
        cursor.setWord(clipboard);
      fileSaved = false;
      }
      break;
    }
    default:
      return true;
  }
  return false;
}

void isertCharIntoRam(char c) {
  insertChar = false;
  if (c == 27) {  // Esc
    return;
  }
  cursor.setWord(Util::getBoolByte(c));
  cursor.increaseY();
}

/*
 * Returns whether it found a key.
 */
bool processInputWithShift(char c) {
  shiftPressed = 0;
  if (c == 65) {           // A, part of up arrow
    cursor.moveByteUp();
    fileSaved = false;
    return true;
  } else if (c == 66) {    // B, part of down arrow
    cursor.moveByteDown();
    fileSaved = false;
    return true;
  } else if (c == 126) {   // ~, part of insert key (also is 2)
    cursor.insertByteAndMoveRestDown();
    fileSaved = false;
    return true;
  } else {
    return false;
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

void switchDrawing(bool direction) {
  if (direction) {
    if (*selectedView == view3d) {
      selectedView = &view3db;
    } else if (*selectedView == view3db) {
      selectedView = &view2d;
    } else {
      return;
    }
  } else {
    if (*selectedView == view3db) {
      selectedView = &view3d;
    } else if (*selectedView == view2d) {
      selectedView = &view3db;
    } else {
      return;
    }
  }
  prepareOutput();
  clearScreen();
  redrawScreen();
}

////////////
/// SAVE ///
////////////


void save() {
  if (fileSaved) {
    printer.printString("Saved "+loadedFilename);
    return;
  }
  if (loadedFilename.empty()) {
    saveAs();
  } else {
    saveRamToFile(loadedFilename);
    fileSaved = true;
    printer.printString("Saved "+loadedFilename);
    redrawScreen();
  }
}

void saveAs() {
  string fileName = getFreeFileName();
  saveRamToFile(fileName);
  loadedFilename = fileName;
  fileSaved = true;
  printer.printString("Saved as "+fileName);
  redrawScreen();
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

char readStdin() {
  char c = 0;
  errno = 0;
  ssize_t num = read(0, &c, 1);
  if (num == -1 && errno == EINTR) {
    // Exits if ctrl-c was pressed.
    if (pleaseExit) {
      exit(0);
    }
    if (screenResized) {
      redrawScreen();
    }
    return readStdin();
  }
  return c;
}
