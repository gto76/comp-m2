#ifndef STANDARD_OUTPUT_H
#define STANDARD_OUTPUT_H

#include <string>
#include <vector>

#include "provides_output.hpp"

using namespace std;

class StandardOutput {
  public:
    StandardOutput(bool outputCharsIn) 
        : outputChars(outputCharsIn) { }
    void run();

    ProvidesOutput *input = NULL;
    
  private:
    const bool outputChars;
    void printBool(vector<bool> wordIn);
};

#endif