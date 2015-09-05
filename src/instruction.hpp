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

    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    
    Address getAddress(vector<bool> val, const vector<bool> &reg, const Ram &ram);

    Instruction(vector<bool> valIn, vector<bool> &regIn, Ram &ramIn) : val(valIn), reg(regIn), ram(ramIn), inst(getInstruction(valIn)), adr(getAddress(valIn)) { }

    void exec(vector<bool>* pc);
  private:

    vector<bool>* const reg;
    Ram* const ram;
    SpecificInstruction * const inst;
  public:
    Address const adr;
  private:
    Address getAddress(vector<bool> val);
    SpecificInstruction * getInstruction(vector<bool> val);
};

#endif