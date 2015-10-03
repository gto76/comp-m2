#include "pipe_input.hpp"

#include <unistd.h>
#include <iostream>
#include <cstdio>

// Not shure if all necesary!
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include "util.hpp"

using namespace std;

extern "C" {
  extern volatile sig_atomic_t pleaseExit;
}

vector<bool> PipeInput::getOutput() {
  if (rawMode) {
    char c = readRawChar();
    return Util::getBoolByte((int)c);
  } else if (inputChars) {
    int c = getchar();
    if (c == EOF) {
      cout << endl;
      exit(0);
    }
    return Util::getBoolByte(c);
  } else {
    return Util::readWordFromPipe();
  }
}

char PipeInput::readRawChar() {
  char c = 0;
  errno = 0;
  ssize_t num = read(0, &c, 1);
  if (num == -1 && errno == EINTR) {
    // Exits if ctrl-c was pressed.
    if (pleaseExit) {
      exit(0);
    }
    return readRawChar();
  }
  return c;
}