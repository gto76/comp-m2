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
    Address const adr;
    string const label;
    Instruction(vector<bool> valIn, const vector<bool> &regIn, const Ram &ramIn)
        : val(valIn),
          index(Util::getInt(Util::getFirstNibble(valIn))),
          logicIndex(Util::getInt(Util::getSecondNibble(valIn))),
          inst(getInstruction()),
          adr(getAddress(valIn, regIn, ramIn)),
          label(inst->getLabel()) { }
    void exec(vector<bool> &pc, vector<bool> &reg, Ram &ram);
    bool isLogic();
  private:
    Address getAddress(vector<bool> val, const vector<bool> &reg,
                       const Ram &ram);
    SpecificInstruction * getInstruction();
    SpecificInstruction * getLogicInstruction();
};

#endif