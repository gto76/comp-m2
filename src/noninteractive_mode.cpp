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
    ProvidesOutput predecesor;
    // if computer is first
    if (i == 0) {
      predecesor = input;
    } else {
      predecesor = computerChain.back();
    }
    Ram ram = Ram(predecesor);
    Load::fillRamWithFile(filename, ram);  
    Computer computer = Computer(i, ram, NULL, NULL);
    computerChain.push_back(computer);
  }
  return computerChain;
}
