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
  for (string filename : filenames) {
    Computer computer = getComputer(i);
    Load::fillRamWithFile(filename.c_str(), computer.ram);  
    computerChain.push_back(computer);
  }
  return computerChain;
}

Computer NoninteractiveMode::getComputer(int id) {
  if (id == 0) {
    return Computer(id, input, NULL, NULL);
  } else {
    Computer predecesor = computerChain.back();
    return Computer(id, predecesor, NULL, NULL);
  }
}

StandardOutput NoninteractiveMode::getStandardOutput(bool outputCharsIn) {    
  Computer lastComp = computerChain.back();
  return StandardOutput(computerChain.back(), outputCharsIn);
}
