#include "standard_output.hpp"

#include <iostream>
#include <vector>

#include "util.hpp"

using namespace std;

void StandardOutput::run() {
  while (1) {
    vector<bool> out = input->getOutput();
    if (out.empty()) {
      return;
    } else {
      printBool(out);
    }
  }
}

void StandardOutput::printBool(vector<bool> wordIn) {
  if (Util::outputIsPiped()) {
    cout << Util::getString(wordIn) + "\n";
    return;
  } 
  if (!outputChars) {
    cout << Util::getStringWithFormatedInt(wordIn);
    return;
  }
  char c = (char)Util::getInt(wordIn);
  printChar(c);
}

void StandardOutput::printChar(char c) {
  if (bufferOutput) {
    buffer.push_back(c);
    checkBuffer();
  } else {
    cout << c;
  }
}

void StandardOutput::checkBuffer() {
  if (buffer.back() == '\n') {
    string line = string(buffer.begin(), buffer.end());
    cout << line;
    buffer = { };
  }
}