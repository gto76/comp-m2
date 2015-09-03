#include "instruction.hpp"

#include <vector>

#include "address.hpp"
#include "specific_instruction.hpp"
#include "util.hpp"

using namespace std;

Address Instruction::getAddress(vector<bool> val) {
  return inst->getAddress(val, reg, ram);
}

void Instruction::exec(vector<bool>* pc) {
  inst->exec(adr, pc, reg, ram);
}


SpecificInstruction * Instruction::getInstruction(vector<bool> val) {
  int instCode = Util::getInt(val);

  switch (instCode) {
    case 0:
      // static Read read;
      // return read;
      return new Read;
    // case 1:
    //   write(value);
    //   break;
    // case 2:
    //   add(value); 
    //   break;
    // case 3:
    //   sub(value);
    //   break;
    // case 4:
    //   jump(value);
    //   break;
    // case 5:
    //   ifMax(value);
    //   break;
    // case 6:
    //   ifMin(value);
    //   break;
    // case 7:
    //   logic(value);
    //   break;
    // case 8:
    //   readPointer(value);
    //   break;
    // case 9:
    //   writePointer(value);
    //   break;
    // case 10:
    //   incDec(value);
    //   break;
    // case 11:
    //   print(value);
    //   break;
    // case 13:
    //   ifNotMax(value);
    //   break;
    // case 14:
    //   ifNotMin(value);
    //   break;
    default:
      return new Read;
  }
}


