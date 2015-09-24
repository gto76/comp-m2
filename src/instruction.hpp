#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <string>
#include <vector>

#include "address.hpp"
#include "specific_instruction.hpp"
#include "util.hpp"

using namespace std;

class Ram;

class Instruction {
  public:
    /*
     * Ram can also be NULL. In this case the pointer instructions that need
     * ram to define address, will asume the ram is empty.
     */
    Instruction(vector<bool> valIn, const vector<bool> &regIn, const Ram *ramIn)
        : val(valIn),
          index(Util::getInt(Util::getFirstNibble(valIn))),
          logicIndex(Util::getInt(Util::getSecondNibble(valIn))),
          inst(getInstruction()),
          firstOrderAdr(getFirstOrderAdr(valIn)),
          adr(getAddress(firstOrderAdr[0], regIn, ramIn)),
          label(inst->getLabel()) { }

    bool operator == (const Instruction &other) const {
      return val == other.val;
    }

    const vector<bool> val;
    const int index;
    const int logicIndex;
    SpecificInstruction * const inst;
    const vector<Address> firstOrderAdr;
    const Address adr;
    const string label;

    void exec(vector<bool> &pc, vector<bool> &reg, Ram &ram);
    bool isLogic();
    string getCode(int pc);
    static vector<Instruction> getEffectiveInstructions(
        const Ram &ram, const vector<bool> &reg);
    static vector<Instruction> getAllInstructions(const Ram &ram, 
                                                  const vector<bool> &reg);
    static int getIndexOfLastNonEmptyInst(
        const vector<Instruction> &allInstructions);
    static vector<Instruction> removeElementsPastIndex(vector<Instruction> &iii,
                                                       int index);

  private:
    vector<Address> getFirstOrderAdr(vector<bool> val);
    Address getAddress(Address firstOrderAdr, const vector<bool> &reg,
                       const Ram *ram);
    SpecificInstruction * getInstruction();
    SpecificInstruction * getLogicInstruction();
};

#endif