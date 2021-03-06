#include "instruction.hpp"

#include <vector>

#include "address.hpp"
#include "const.hpp"
#include "ram.hpp"
#include "specific_instruction.hpp"
#include "util.hpp"

using namespace std;

/////////////////
/// INTERFACE ///
/////////////////

void Instruction::exec(vector<bool> &pc, vector<bool> &reg, Ram &ram) {
  inst->exec(adr, pc, reg, ram);
}

bool Instruction::isLogic() {
  return index == LOGIC_OPS_INDEX;
}

string Instruction::getCode(int pc) {
  return inst->getCode(firstOrderAdr[0], pc);
}

/*
 * Doesn't include empty instructions from last non-empty on.
 */
vector<Instruction> Instruction::getEffectiveInstructions(const Ram &ram, 
                                                     const vector<bool> &reg) {
  vector<Instruction> allInstructions = Instruction::getAllInstructions(ram, 
                                                                        reg);
  int lastNonemptyInst = 
      Instruction::getIndexOfLastNonEmptyInst(allInstructions);
  return Instruction::removeElementsPastIndex(allInstructions,
                                              lastNonemptyInst + 1);
}

vector<Instruction> Instruction::getAllInstructions(const Ram &ram, 
                                                    const vector<bool> &reg) {
  vector<Instruction> out;
  for (vector<bool> word : ram.state.at(CODE)) {
    Instruction inst = Instruction(word, EMPTY_WORD, &ram);
    out.push_back(inst);
  }
  return out;
}

int Instruction::getIndexOfLastNonEmptyInst(
    const vector<Instruction> &allInstructions) {
  int lastNonemptyInst = -1;
  int i = 0;
  for (Instruction inst : allInstructions) {
    if (inst.val != EMPTY_WORD) {
      lastNonemptyInst = i;
    }
    i++;
  }
  return lastNonemptyInst;
}

vector<Instruction> Instruction::removeElementsPastIndex(
    vector<Instruction> &iii, int index) {
  return vector<Instruction>(iii.begin(), iii.begin() + index);
}

///////////////
/// PRIVATE ///
///////////////

vector<Address> Instruction::getFirstOrderAdr(vector<bool> val) {
  return inst->getFirstOrderAdr(val);
}

Address Instruction::getAddress(Address firstOrderAdr, const vector<bool> &reg,
                                const Ram *ram) {
  return inst->getAddress(firstOrderAdr, reg, ram);
}

SpecificInstruction * Instruction::getInstruction() {
  switch (index) {
    case 0:
      return new Read;
    case 1:
      return new Write;
    case 2:
      return new Add;
    case 3:
      return new Sub;
    case 4:
      return new Jump;
    case 5:
      return new IfMax;
    case 6:
      return new IfMin;
    case 7:
      return getLogicInstruction();
    case 8:
      return new ReadPointer;
    case 9:
      return new WritePointer;
    case 10: {
      if (val[4] == false) {
        return new Increase;
      } else {
        return new Decrease;
      }
    }
    case 11:
      return new Print;
    case 13:
      return new IfNotMax;
    case 14:
      return new IfNotMin;
    default:
      return new Read;
  }
}

SpecificInstruction * Instruction::getLogicInstruction() {
  switch (logicIndex) {
    case 0:
      return new JumpReg;
    case 1:
      return new ReadReg;
    case 2:
      return new InitializeFirstAddress;
    case 3:
      return new Not;
    case 4:
      return new ShiftLeft;
    case 5:
      return new ShiftRight;
    case 6:
      return new And;
    case 7:
      return new Or;
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15: 
      return new Xor;
    default:
      return new Read;
  }
}
