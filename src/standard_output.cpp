#include "standard_output.hpp"

#include <iostream>
#include <vector>

#include "util.hpp"

using namespace std;

void printBool(vecot<bool> word) {
  if (Util::outputIsPiped()) {
    cout << Util::getString(wordIn) + "\n";
  } else {
    if (outputChars) {
      char c = (char)Util::getInt(wordIn);
      cout << c;
    } else {
      cout << Util::getStringWithFormatedInt(wordIn);
    }
  }
}

void StandardOutput::run() {
  while (1) {
    vector<bool> out = input.getOutput();
    if (out == { }) {
      return;
    } else {
      printBool(out);
    }
  }
}