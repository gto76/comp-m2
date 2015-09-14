#include "cpu.hpp"

#include <cmath>
#include <string>
#include <vector>
#include <set>

#include "address.hpp"
#include "instruction.hpp"
#include "util.hpp"

using namespace std;

/////////////////////////
/////// INTERFACE ///////
/////////////////////////

int Cpu::getCycle() {
  return cycle;
}

vector<bool> Cpu::getRegister() {
  return reg;
}

vector<bool> Cpu::getPc() {
  return pc;
}

vector<bool> Cpu::getInstructionWord() {
  Address adr = Address(CODE, pc);
  return ram->get(adr);
}

vector<bool> Cpu::getInstructionCode() {
  return Util::getFirstNibble(getInstructionWord());
}

int Cpu::getInstructionCodeInt() {
  vector<bool> instructionCodeBool = getInstructionCode();
  return Util::getInt(instructionCodeBool);
}

Instruction Cpu::getInstruction() {
  return Instruction(getInstructionWord(), reg, *ram);
}

/*
 * Returns 'false' when last address is reached.
 */
bool Cpu::step() {
  cycle++;
  bool reachedLastAddress = Util::getInt(pc) >= RAM_SIZE;
  if (reachedLastAddress) {
    return false;
  }
  Instruction inst = Instruction(getInstructionWord(), reg, *ram);
  inst.exec(pc, reg, *ram);
  return true;
}

void Cpu::reset() {
  reg = vector<bool>(WORD_SIZE);
  pc = vector<bool>(ADDR_SIZE);
  cycle = 0;
}


