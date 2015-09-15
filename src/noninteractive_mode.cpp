#include "noninteractive_mode.hpp"

#include <string>
#include <vector>

#include "computer.hpp"
#include "load.hpp"
#include "ram.hpp"

using namespace std;

void NoninteractiveMode::run() {
  output.run();
}

vector<Computer> NoninteractiveMode::getComputerChain(vector<string> filenames) {
  vector<Computer> computerChain;
  int i = 0;
  // for all filenames
  for (string filename : filenames) {
    Ram ram = getRamFor(i == 0);
    Load::fillRamWithFile(filename.c_str(), ram);  
    Computer computer = Computer(i, ram, NULL, NULL);
    computerChain.push_back(computer);
  }
  return computerChain;
}

Ram NoninteractiveMode::getRamFor(bool firstComputer) {
  if (firstComputer) {
    return Ram(input);
  } else {
    Computer predecesor = computerChain.back();
    return Ram(predecesor);
  }
}

StandardOutput NoninteractiveMode::getStandardOutput(bool outputCharsIn) {    
  Computer lastComp = computerChain.back();
  return StandardOutput(computerChain.back(), outputCharsIn);
}
