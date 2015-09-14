#ifndef COMPUTER_H
#define COMPUTER_H

#include <vector>

#include "cpu.hpp"
#include "provides_output.hpp"
#include "ram.hpp"

using namespace std;

class Computer : public virtual ProvidesOutput {
  public:
    Computer(int idIn, Ram ramIn, void (*printStateIn)(void),
             void (*sleepAndCheckForKeyIn)(void))
        : ram(ramIn), 
          id(idIn),
          cpu(Cpu(&ram))
          printState(printStateIn),
          sleepAndCheckForKey(sleepAndCheckForKeyIn) { }
    virtual vector<bool> getOutput() const;
    const int id;
    // Main components.
    const Ram ram;
    const Cpu cpu;
    // Cycle counter.
    bool executionCanceled = false;
    int executionCounter = 0;

  private:
    // Print state function pointer.
    void (*printState)(void);
    void (*sleepAndCheckForKey)(void);
};

#endif