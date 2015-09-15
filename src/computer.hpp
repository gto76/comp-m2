#ifndef COMPUTER_H
#define COMPUTER_H

#include <vector>

#include "cpu.hpp"
#include "provides_output.hpp"
#include "ram.hpp"

using namespace std;

class Computer : public ProvidesOutput {
  public:
    Computer()
        : ram(Ram()),
          cpu(Cpu(ram)),
          printState(NULL),
          sleepAndCheckForKey(NULL) { }
    Computer(void (*printStateIn)(void), void (*sleepAndCheckForKeyIn)(void))
        : ram(Ram()), 
          cpu(Cpu(ram)),
          printState(printStateIn),
          sleepAndCheckForKey(sleepAndCheckForKeyIn) { }
    
    vector<bool> getOutput();

    // Main components.
    Ram ram;
    Cpu cpu;
    // Cycle counter.
    int executionCounter = 0;

  private:
    // Print state function pointer.
    void (*printState)(void);
    void (*sleepAndCheckForKey)(void);
};

#endif