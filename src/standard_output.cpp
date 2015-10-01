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
  if (outputChars) {
    cout << (char)Util::getInt(wordIn);
    fflush(stdout);
  } else if (outputNumbers) {
    cout << Util::getStringWithFormatedInt(wordIn);
  } else {
    cout << Util::getString(wordIn) + "\n";
  }
}

