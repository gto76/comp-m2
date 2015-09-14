#ifndef NONINTERACTIVE_MODE_H
#define NONINTERACTIVE_MODE_H

#include <string>
#include <vector>

using namespace std;

class NoninteractiveMode {
  public:
    NoninteractiveMode(vector<string> filenamesIn, bool outputCharsIn) 
        : computerChain(getComputerChain(filenamesIn),
          output(computerChain.back(), outputCharsIn) { }
    void run();

  private:
    const PipeInput input;
    const vector<computer> computerChain;
    const StandardOutput output;

    vector<Computer> getComputerChain(vector<string> filenames);
};

#endif