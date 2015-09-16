#ifndef STANDARD_OUTPUT_H
#define STANDARD_OUTPUT_H

#include <string>
#include <vector>

#include "provides_output.hpp"

using namespace std;

class StandardOutput {
  public:
    StandardOutput(bool outputCharsIn, bool bufferOutputIn) 
        : outputChars(outputCharsIn),
          bufferOutput(bufferOutputIn) { }
    void run();

    ProvidesOutput *input = NULL;
    
  private:
    vector<char> buffer;
    const bool outputChars;
    const bool bufferOutput;
    void printBool(vector<bool> wordIn);
    void printChar(char c);
    void checkBuffer();
};

#endif