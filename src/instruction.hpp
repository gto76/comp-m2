#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <vector>

#include "address.hpp"
#include "ram.hpp"
#include "specific_instruction.hpp"
#include "util.hpp"

using namespace std;

class Instruction {
  public:
    vector<bool> const val;
    int const index;
    int const logicIndex;
    SpecificInstruction * const inst;
    vector<Address> const firstOrderAdr;
    Address adr;
    string const label;
    Instruction(vector<bool> valIn, const vector<bool> &regIn, const Ram &ramIn)
        : val(valIn),
          index(Util::getInt(Util::getFirstNibble(valIn))),
          logicIndex(Util::getInt(Util::getSecondNibble(valIn))),
          inst(getInstruction()),
          firstOrderAdr(getFirstOrderAdr(valIn)),
          adr(getAddress(firstOrderAdr[0], regIn, ramIn)),
          label(inst->getLabel()) { }
    void exec(vector<bool> &pc, vector<bool> &reg, Ram &ram);
    bool isLogic();
  private:
    vector<Address> getFirstOrderAdr(vector<bool> val);
    Address getAddress(Address firstOrderAdr, const vector<bool> &reg,
                       const Ram &ram);
    SpecificInstruction * getInstruction();
    SpecificInstruction * getLogicInstruction();
};

#endif