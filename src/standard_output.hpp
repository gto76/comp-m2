#ifndef STANDARD_OUTPUT_H
#define STANDARD_OUTPUT_H

#include <string>
#include <vector>

using namespace std;

class StandardOutput {
  public:
    StandardOutput(ProvidesOutput &inputIn, bool outputCharsIn) 
        : input(inputIn),
          outputChars(outputCharsIn) { }
    void run();
  private:
    const ProvidesOutput &input;
    const bool outputChars;
};

#endif