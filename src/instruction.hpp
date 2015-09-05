#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <vector>

#include "address.hpp"
#include "ram.hpp"
#include "specific_instruction.hpp"

using namespace std;

class Instruction {
  public:
    vector<bool> const val;
    Instruction(vector<bool> valIn, const vector<bool> &regIn, const Ram &ramIn) : val(valIn), inst(getInstruction(valIn)), adr(getAddress(valIn, regIn, ramIn)) { }
    void exec(vector<bool> &pc, vector<bool> &reg, Ram &ram);
  private:
    // vector<bool>* const reg;
    // Ram* const ram;
    SpecificInstruction * const inst;
  public:
    Address const adr;
  private:
    Address getAddress(vector<bool> val, const vector<bool> &reg, const Ram &ram);
    static SpecificInstruction * getInstruction(vector<bool> val);
};

#endif