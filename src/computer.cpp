#include "computer.hpp"

#include <vector>

#include "cpu.hpp"
#include "ram.hpp"

vector<bool> Computer::getOutput() {
  while(!executionCanceled) {
    bool shouldContinue = cpu.step();
    if (printState != NULL) {
      printState();
      if (!shouldContinue) {
        return { };
      }
      sleepAndCheckForKey();
    }
    if (!shouldContinue) {
      exit(id);
    }
    if (ram.outputPending) {
      ram.outputPending = false;
      return ram.output;
    }
  }
  return { };
}