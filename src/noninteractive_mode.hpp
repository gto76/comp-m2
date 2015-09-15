#ifndef NONINTERACTIVE_MODE_H
#define NONINTERACTIVE_MODE_H

#include <string>
#include <vector>

#include "computer.hpp"
#include "pipe_input.hpp"
#include "standard_output.hpp"

using namespace std;

class NoninteractiveMode {
  public:
    NoninteractiveMode(vector<string> filenamesIn, bool outputCharsIn) 
        : computerChain(getComputerChain(filenamesIn)),
          output(getStandardOutput(outputCharsIn)) { }
    void run();

  private:
    PipeInput input;
    vector<Computer> computerChain;
    StandardOutput output;

    vector<Computer> getComputerChain(vector<string> filenames);
    Computer getComputer(int id);
    StandardOutput getStandardOutput(bool outputCharsIn);
};

#endif