#ifndef PIPE_INPUT_H
#define PIPE_INPUT_H

#include "provides_output.hpp"

using namespace std;

extern "C" {
  void setEnvironment();
}

class PipeInput : public ProvidesOutput {
  public:
    PipeInput(bool inputCharsIn, bool rawModeIn) 
        : inputChars(inputCharsIn), rawMode(rawModeIn) {
      if (rawMode) {
        setEnvironment();
      }
    }
    vector<bool> getOutput();

  private:
    bool inputChars;
    bool rawMode;
    char readRawChar();
};

#endif