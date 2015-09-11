#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <streambuf>
#include <string>
#include <tuple>
#include <vector>

#include "const.hpp"
#include "cpu.hpp"
#include "cursor.hpp"
#include "drawing2D.hpp"
#include "drawing3D.hpp"
#include "output.hpp"
#include "printer.hpp"
#include "ram.hpp"
#include "renderer.hpp"
#include "util.hpp"
#include "view.hpp"

using namespace std;

extern "C" {
  extern volatile sig_atomic_t pleaseExit;
  void setEnvironment();
  void resetEnvironment();
}

//////////////////////////
////////// VARS //////////
//////////////////////////

// Two global variables.
bool interactivieMode;
bool executionCanceled = false;

Printer printer;
Ram ram = Ram(printer);
Cpu cpu = Cpu(&ram);

// Graphic representation of the computer state.
vector<vector<string>> buffer;

int executionCounter = 0;

// Saved state of a ram.
map<AddrSpace, vector<vector<bool>>> savedRamState;

// Class for keeping track of and moving around cursor.
Cursor cursor = Cursor(ram);

// Two views.
//View view3D = View({"-","-", "-"}, "*", "-"); // LIGHTBULB_ON_3D, LIGHTBULB_OFF_3D);
// View view3D = View(Drawing2D::getDrawing2D(), "*", "-"); // LIGHTBULB_ON_3D, LIGHTBULB_OFF_3D);
//View view2D; // = View(drawing2D, LIGHTBULB_ON_2D, LIGHTBULB_OFF_2D);
//View &selectedView = view3D;// = view3D;

View view3D = View(drawing3D, LIGHTBULB_ON_3D, LIGHTBULB_OFF_3D);
View view2D = View(drawing2D, LIGHTBULB_ON_2D, LIGHTBULB_OFF_2D);
View &selectedView = view3D;

//////////////////////////
/////// FUNCTIONS ////////
//////////////////////////

///////////////////////
void drawScreen() {
  buffer = Renderer::renderState(printer, ram, cpu, cursor, selectedView);
  int i = 0;
  for (vector<string> line : buffer) {
    replaceLine(line, i++);
  }
}

void switchBitUnderCursor() {
  bool bitValue = cursor.getBit(); 
  cursor.setBit(!bitValue);
}

void eraseByteUnderCursor() {
  cursor.setWord(Util::getBoolByte(0));
  redrawScreen();
}

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
  }
}

void exec() {
  while(!executionCanceled) {
    bool shouldContinue = cpu.step();
    if (interactivieMode) {
      redrawScreen();
    }
    if (!shouldContinue) {
      return;
    }
    if (interactivieMode) {
      sleepAndCheckForKey();
    }
  }
}

/*
 * Saves the state of the ram and starts the execution of a program.
 * When execution stops, due to it reaching last address or user pressing 
 * 'esc', it loads back the saved state of the ram, and resets the cpu.
 */
void run() { 
  if (executionCounter > 0) {
    printer.printEmptyLine();
  }
  savedRamState = ram.state;
  exec();
  // If 'esc' was pressed then it doesn't wait for keypress at the end.
  if (executionCanceled) {
    executionCanceled = false;
  } else {
    readStdin(false);
  }
  ram.state = savedRamState;
  cpu = Cpu(&ram);
  redrawScreen();
  executionCounter++;
}

string getFreeFileName() {
  int i = 0;
  while (Util::fileExists(SAVE_FILE_NAME + to_string(++i)));
  return SAVE_FILE_NAME + to_string(i);
}

void saveRamToFile() {
  string fileName = getFreeFileName();
  ofstream fileStream(fileName);
  fileStream << ram.getString();
  fileStream.close();
}

void userInput() {
  while(1) {
    char c = readStdin(true);
    switch (c) {
      // UP
      case 107: // k
      case 65:  // A, part of escape seqence of up arrow
        cursor.decreaseY();
        break;
      // DOWN
      case 106: // j
      case 66:  // B, part of escape seqence of down arrow
        cursor.increaseY();
        break;
      // RIGHT
      case 108: // l
      case 67:  // C, part of escape seqence of rigth arrow
        cursor.increaseX();
        break;
      // LEFT
      case 104: // h
      case 68:  // D, part of escape seqence of left arrow
        cursor.decreaseX();
        break;
      // SWAP UP
      case 115:  // s
      // case 100: // d
      case 53:  // 5, part of escape seqence of page up
        cursor.moveByteUp();
        break;
      // SWAP DOWN
      // case 102:  // f
      case 103:  // g
      case 54:  // 6, part of escape seqence of page down
        cursor.moveByteDown();
        break;
      // SAVE
      // case 115:  // s
      case 119:  // w
        saveRamToFile();
        break;
      // FLIP
      case 32:  // space
      case 102:  // f
        switchBitUnderCursor();
        break;
      // DELETE
      // case 97:  // a
      case 100:  // d
      case 51:  // 3, part of escape seqence of delete key
        eraseByteUnderCursor();
        break;
      // SWITCH ADR SPACE
      case 97:  // a
      //case 115:  // s
      //case 103:  // g
      case 9:  // tab
        cursor.switchAddressSpace();
        break;
      // RUN
      case 10:  // enter
        run();
        break;
    }
    redrawScreen();
  }
}

/*
 * Initializes 'output.cpp' by sending dimensions of a 'drawing' and 
 * a 'drawScreen' callback function, that output.c will use on every 
 * screen redraw.
 */
void prepareOutput() {
  // size_t drawingWidth = 0;
  // size_t drawingHeight = 0;
  // for (vector<string> line : Util::splitIntoLines(drawing)) {
  //   drawingWidth = std::max(drawingWidth, line.size());
  //   drawingHeight++;
  // }
  setOutput(&drawScreen, selectedView.width, selectedView.height);
}

bool getBool(char c) {
  return c == '*';
}

void writeInstructionBitToRam(int address, int bitIndex, bool bitValue) {
  ram.state[CODE].at(address).at(bitIndex) = bitValue;
}

void writeDataBitToRam(int address, int bitIndex, bool bitValue) {
  ram.state[DATA].at(address).at(bitIndex) = bitValue;
}

void writeLineToRam(string line, int address) {
  int bitIndex = 0;
  for (char c : line) {
    if (address < RAM_SIZE) { 
      writeInstructionBitToRam(address, bitIndex, getBool(c));
    } else {
      writeDataBitToRam(address-RAM_SIZE, bitIndex, getBool(c));
    }
    if (++bitIndex >= WORD_SIZE) {
      return;
    }
  }
}

void loadRamFromFileStream(ifstream* fileStream) {
  int address = 0;
  while (!fileStream->eof()) {
    string line;
    getline(*fileStream, line);
    bool lineEmptyOrAComment = line.empty() || line[0] == '#';
    if (lineEmptyOrAComment) {
      continue;
    }
    writeLineToRam(line, address);
    if (++address >= 2*RAM_SIZE) {
      return;
    }
  } 
}

void checkIfInputIsPiped() {
  interactivieMode = !Util::inputIsPiped();
}

void loadRamIfFileSpecified(int argc, const char* argv[]) {
  if (argc <= 1) {
    return;
  }
  ifstream fileStream;    
  fileStream.open(argv[1]);   
  if (fileStream.fail()) {
    fprintf(stderr, "Invalid filename '%s'. Aborting ram load.", argv[1]);
  } else {
    loadRamFromFileStream(&fileStream);
    fileStream.close();  
  }
}

void setViews() {
  // Two views.
  // View view3D = View(drawing3D, LIGHTBULB_ON_3D, LIGHTBULB_OFF_3D);
  // View view2D = View(drawing2D, LIGHTBULB_ON_2D, LIGHTBULB_OFF_2D);
  // selectedView = view2D;
}

//////////////////////////
////////// MAIN //////////
//////////////////////////

void startInteractiveMode() {
  setViews();
  setEnvironment();
  prepareOutput();
  clearScreen();
  clearScreen();
  redrawScreen();
  userInput();
}

int main(int argc, const char* argv[]) {
  srand(time(NULL));
  checkIfInputIsPiped();
  loadRamIfFileSpecified(argc, argv);
  if (interactivieMode) {
    startInteractiveMode();
  } else {
    exec();
  }
}


