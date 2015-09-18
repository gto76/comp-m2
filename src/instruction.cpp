#include "instruction.hpp"

#include <vector>

#include "address.hpp"
#include "specific_instruction.hpp"
#include "util.hpp"

using namespace std;

vector<Address> Instruction::getFirstOrderAdr(vector<bool> val) {
  return inst->getFirstOrderAdr(val);
}

Address Instruction::getAddress(Address firstOrderAdr, const vector<bool> &reg,
                                const Ram &ram) {
  return inst->getAddress(firstOrderAdr, reg, ram);
}

void Instruction::exec(vector<bool> &pc, vector<bool> &reg, Ram &ram) {
  inst->exec(adr, pc, reg, ram);
}

bool Instruction::isLogic() {
  return index == LOGIC_OPS_INDEX;
}

string Instruction::getCode() {
  return inst->getCode(val);
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
