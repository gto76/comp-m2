#ifndef STANDARD_OUTPUT_H
#define STANDARD_OUTPUT_H

#include <string>
#include <vector>

#include "provides_output.hpp"

using namespace std;

class StandardOutput {
  public:
    StandardOutput(bool outputNumbersIn, bool outputCharsIn) 
        : outputNumbers(outputNumbersIn), outputChars(outputCharsIn) { }
    void run();

    ProvidesOutput *input = NULL;
    
  private:
    const bool outputNumbers;
    const bool outputChars;
    void printBool(vector<bool> wordIn);
};

#endif