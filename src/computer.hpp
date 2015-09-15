#ifndef COMPUTER_H
#define COMPUTER_H

#include <vector>

#include "cpu.hpp"
#include "provides_output.hpp"
#include "ram.hpp"

using namespace std;

class Computer : public ProvidesOutput {
  public:
    Computer(int idIn, ProvidesOutput &inputIn, void (*printStateIn)(void),
             void (*sleepAndCheckForKeyIn)(void))
        : id(idIn),
          ram(Ram(inputIn)), 
          cpu(Cpu(&ram)),
          printState(printStateIn),
          sleepAndCheckForKey(sleepAndCheckForKeyIn) { }
    vector<bool> getOutput();

    int id;
    // Main components.
    Ram ram;
    Cpu cpu;
    // Cycle counter.
    bool executionCanceled = false;
    int executionCounter = 0;

  private:
    // Print state function pointer.
    void (*printState)(void);
    void (*sleepAndCheckForKey)(void);
};

#endif