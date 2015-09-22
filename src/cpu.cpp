#include "cpu.hpp"

#include <cmath>
#include <string>
#include <vector>
#include <set>

#include "address.hpp"
#include "instruction.hpp"
#include "util.hpp"

using namespace std;

/*
 * Returns 'false' when last address is reached.
 */
bool Cpu::step() {
  cycle++;
  bool reachedLastAddress = Util::getInt(pc) >= RAM_SIZE;
  if (reachedLastAddress) {
    return false;
  }
  Instruction inst = getInstruction();
  inst.exec(pc, reg, ram);
  return true;
}

void Cpu::reset() {
  reg = vector<bool>(WORD_SIZE);
  pc = vector<bool>(ADDR_SIZE);
  cycle = 0;
}

Instruction Cpu::getInstruction() const {
  Address adr = Address(CODE, pc);
  vector<bool> instructionWord = ram.get(adr);
  return Instruction(instructionWord, reg, &ram);
}

vector<bool> Cpu::getRegister() const {
  return reg;
}

vector<bool> Cpu::getPc() const {
  return pc;
}

int Cpu::getCycle() const {
  return cycle;
}

void Cpu::switchOn() {
  cycle = 1;
}





