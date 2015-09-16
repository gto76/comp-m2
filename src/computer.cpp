#include "computer.hpp"

#include <stdlib.h>
#include <vector>

#include "comp.hpp"
#include "cpu.hpp"
#include "ram.hpp"

vector<bool> Computer::getOutput() {
  while(!executionCanceled) {
    bool shouldContinue = cpu.step();
    if (ram.outputPending) {
      ram.outputPending = false;
      return ram.output;
    }
    if (printState != NULL) {
      printState();
      if (!shouldContinue) {
        return { };
      }
      sleepAndCheckForKey();
    }
    if (!shouldContinue) {
      exit(0);
    }
  }
  return { };
}
