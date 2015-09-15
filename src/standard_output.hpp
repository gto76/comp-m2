#ifndef STANDARD_OUTPUT_H
#define STANDARD_OUTPUT_H

#include <string>
#include <vector>

#include "provides_output.hpp"

using namespace std;

class StandardOutput {
  public:
    StandardOutput(ProvidesOutput &inputIn, bool outputCharsIn) 
        : input(inputIn),
          outputChars(outputCharsIn) { }
    void run();
  private:
    ProvidesOutput &input;
    const bool outputChars;
    void printBool(vector<bool> wordIn);
};

#endif