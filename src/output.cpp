#include "output.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <string>
#include <vector>

#include "const.hpp"

////// PRIVATE ///////

void registerSigWinChCatcher(void);
void sigWinChCatcher(int signum);
void updateConsoleSize(void);
void refreshScreen();
void updateScreenAndPrintChanges();
void printLine(vector<string> lineVec, int lineNo);
int coordinatesOutOfBounds(int x, int y);
vector<string> resizeLine(vector<string> line, int size);
bool isEscSeqence(vector<string> line, vector<string> seqence, int index);
int insertEscSeqAndReturnLength(vector<string> &lineOut, 
                                vector<string> const &escSeq);
void setLine(vector<string> line, int y);
int getAbsoluteX(int x);
int getAbsoluteY(int y);
int getAbsoluteCoordinate(int value, int console, int track);
void setBuffer(vector<string> s, int x, int y);

////////////////////////////

#define PRINT_IN_CENTER 1
#define DEFAULT_WIDTH 80
#define DEFAULT_HEIGHT 24

int pictureWidth = DEFAULT_WIDTH;
int pictureHeight = DEFAULT_HEIGHT;

int columns = DEFAULT_WIDTH;
int rows = DEFAULT_HEIGHT;
int columnsLast = columns;
int rowsLast = rows;

callback_function updateBuffer;
volatile sig_atomic_t screenResized = 0;

vector<vector<string>> buffer;
vector<vector<string>> screen;

///////////////////////////////
////////// INTERFACE //////////
///////////////////////////////

void initOutput(callback_function updateBufferThat, int width, int height) {
  updateBuffer = updateBufferThat;
  pictureWidth = width;
  pictureHeight = height;
  registerSigWinChCatcher();
  updateConsoleSize();
  // Sets colors.
  printf("\e[37m\e[40m");
}

void redrawScreen() {
  // if (screenResized == 1) {
  //   screenResized = 0;
  //   refreshScreen();
  // } else {
  screenResized = 0;
  updateConsoleSize();
  if (columnsLast != columns || rowsLast != rows) {
    clearScreen();
  }
  updateBuffer();
  updateScreenAndPrintChanges();
  fflush(stdout);
}

  // updateConsoleSize();
  // clearScreen(); // !!!
  // updateBuffer();
  // updateScreenAndPrintChanges();
  // fflush(stdout);

void clearScreen(void) {
  screen = vector<vector<string>>();
  buffer = vector<vector<string>>();
  printf("\e[1;1H\e[2J");
}

void replaceLine(vector<string> line, int y) {
  int x = 0;
  if (coordinatesOutOfBounds(x, y)) {
    return;
  }
  line = resizeLine(line, columns - x - 1);
  setLine(line, y);
}

/////////////////////////////
////////// PRIVATE //////////
/////////////////////////////

void registerSigWinChCatcher() {
  struct sigaction action;
  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;
  action.sa_handler = sigWinChCatcher;
  sigaction(SIGWINCH, &action, NULL);
}

/*
 * Fires when window size changes.
 */
void sigWinChCatcher(int signum) {
  screenResized = 1;
}

/*
 * Asks system about window size.
 */
void updateConsoleSize() {
  struct winsize w;
  ioctl(0, TIOCGWINSZ, &w);
  columnsLast = columns;
  columns = w.ws_col;
  rowsLast = rows;
  rows = w.ws_row;
}

// void refreshScreen() {
//   updateConsoleSize();
//   clearScreen();
//   updateBuffer();
//   updateScreenAndPrintChanges();
//   fflush(stdout);
// }

void updateScreenAndPrintChanges() {
  for (size_t i = 0; i < buffer.size(); i++) {
    if (screen.size() <= i) {
      screen.push_back({});
      printLine(buffer.at(i), i);
    } else if (buffer.at(i) != screen.at(i)) {
      screen.at(i) = buffer.at(i);
      printLine(buffer.at(i), i);
    }
  }
}

void printLine(vector<string> lineVec, int lineNo) {
  string line = accumulate(lineVec.begin(), lineVec.end(), string(""));
  string controlSeq = "\033[" + to_string(getAbsoluteY(lineNo)) + ";" + 
                      to_string(getAbsoluteX(0)) + "H" + line;
  cout << controlSeq;
}

int coordinatesOutOfBounds(int x, int y) {
  return x >= columns || y >= rows || x < 0 || y < 0;
}

vector<string> resizeLine(vector<string> line, int size) {
  if (size < 1) {
    return vector<string>();
  }
  vector<string> lineOut;
  bool insideHighlightedText = false;
  bool insideBrightText = false;
  int counter = 0;
  for (size_t i = 0; i < line.size(); i++) {
    if (line[i] == ESCAPE) {
      if (isEscSeqence(line, HIGHLIGHT_ESC_VEC, i)) {
        i += insertEscSeqAndReturnLength(lineOut, HIGHLIGHT_ESC_VEC);
        insideHighlightedText = true;
        continue;
      } else if (isEscSeqence(line, HIGHLIGHT_END_ESC_VEC, i)) {
        i += insertEscSeqAndReturnLength(lineOut, HIGHLIGHT_END_ESC_VEC);
        insideHighlightedText = false;
        continue;
      } else if (isEscSeqence(line, BRIGHT_ESC_VEC, i)) {
        i += insertEscSeqAndReturnLength(lineOut, BRIGHT_ESC_VEC);
        insideBrightText = true;
        continue;
      } else if (isEscSeqence(line, BRIGHT_END_ESC_VEC, i)) {
        i += insertEscSeqAndReturnLength(lineOut, BRIGHT_END_ESC_VEC);
        insideBrightText = false;
        continue;
      }
    }
    if (counter++ == size+1) {
      if  (insideHighlightedText) {
        lineOut.insert(lineOut.end(), HIGHLIGHT_END_ESC_VEC.begin(), 
                       HIGHLIGHT_END_ESC_VEC.end());
      }
      if  (insideBrightText) {
        lineOut.insert(lineOut.end(), BRIGHT_END_ESC_VEC.begin(), 
                       BRIGHT_END_ESC_VEC.end());
      }
      return lineOut;
    }
    lineOut.push_back(line[i]);
  }
  return lineOut;
}

bool isEscSeqence(vector<string> line, vector<string> seqence, int index) {
  bool lineTooShort = line.size() < index + seqence.size();
  if (lineTooShort) {
    return false;
  }
  return equal(seqence.begin(), seqence.end(), line.begin()+index);
}

int insertEscSeqAndReturnLength(vector<string> &lineOut, 
                                vector<string> const &escSeq) {
  lineOut.insert(lineOut.end(), escSeq.begin(), 
                 escSeq.end());
  return escSeq.size()-1;
}

void setLine(vector<string> line, int y) {
  int size = buffer.size();
  if (size <= y) {
    for (int i = size; i <= y+1; i++) {
      buffer.push_back({});
    }
  }
  buffer.at(y) = line;
}

int getAbsoluteX(int x) {
  int absC = getAbsoluteCoordinate(x, columns, pictureWidth);
  return absC;
}

int getAbsoluteY(int y) {
  return getAbsoluteCoordinate(y, rows, pictureHeight);
}

int getAbsoluteCoordinate(int value, int console, int track) {
  int offset = 0;
  if (PRINT_IN_CENTER) {
    offset = ((console - track) / 2) + ((console - track) % 2);
    if (offset < 0)
      offset = 0;
  }
  return value + 1 + offset;
}

void setBuffer(vector<string> s, int x, int y) {
  int size = buffer.size();
  if (size <= y) {
    for (int i = size; i <= y+1; i++) {
      buffer.push_back({});
    }
  }
  for (size_t j = 0; j < s.size(); j++) {
    buffer[y][j+x] = s[j]; 
  }
}

