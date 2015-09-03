#include "specific_instruction.hpp"

#include <vector>

#include "address.hpp"
#include "ram.hpp"
#include "util.hpp"

using namespace std;

void increasePc(vector<bool>* pc);
Address getThreeBitAddress(vector<bool> val);

// READ

void Read::exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram) {
  *reg = ram->get(adr.space, adr.val);
  increasePc(pc);
}

Address Read::getAddress(vector<bool> val, vector<bool>* reg, Ram* ram) {
  return Address(DATA, Util::getSecondNibble(val));
}

// WRITE

void Write::exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram) {
  increasePc(pc);
}

Address Write::getAddress(vector<bool> val, vector<bool>* reg, Ram* ram) {
  return Address(DATA, Util::getSecondNibble(val));;
}

// ADD

void Add::exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram) {
  increasePc(pc);
}

Address Add::getAddress(vector<bool> val, vector<bool>* reg, Ram* ram) {
  return Address(DATA, Util::getSecondNibble(val));
}

// SUB

void Sub::exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram) {
  increasePc(pc);
}

Address Sub::getAddress(vector<bool> val, vector<bool>* reg, Ram* ram) {
  return Address(DATA, Util::getSecondNibble(val));
}

// JUMP

void Jump::exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram) {
  increasePc(pc);
}

Address Jump::getAddress(vector<bool> val, vector<bool>* reg, Ram* ram) {
  return Address(CODE, Util::getSecondNibble(val));
}

// IF MAX

void IfMax::exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram) {
  increasePc(pc);
}

Address IfMax::getAddress(vector<bool> val, vector<bool>* reg, Ram* ram) {
  return Address(CODE, Util::getSecondNibble(val));
}

// IF MIN

void IfMin::exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram) {
  increasePc(pc);
}

Address IfMin::getAddress(vector<bool> val, vector<bool>* reg, Ram* ram) {
  return Address(CODE, Util::getSecondNibble(val));
}

// JUMP REG

void JumpReg::exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram) {
  increasePc(pc);
}

Address JumpReg::getAddress(vector<bool> val, vector<bool>* reg, Ram* ram) {
  return Address(CODE, Util::getSecondNibble(*reg));
}

// READ REG

void ReadReg::exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram) {
  increasePc(pc);
}

Address ReadReg::getAddress(vector<bool> val, vector<bool>* reg, Ram* ram) {
  return Address(DATA, Util::getSecondNibble(*reg));
}

// INITIALIZE FIRST ADDRESS

void InitializeFirstAddress::exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram) {
  increasePc(pc);
}

Address InitializeFirstAddress::getAddress(vector<bool> val, vector<bool>* reg, Ram* ram) {
  return Address(DATA, Util::getFirstAddress());
}

// NOT

void Not::exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram) {
  increasePc(pc);
}

Address Not::getAddress(vector<bool> val, vector<bool>* reg, Ram* ram) {
  return Address(NONE, Util::getFirstAddress());
}

// SHIFT LEFT

void ShiftLeft::exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram) {
  increasePc(pc);
}

Address ShiftLeft::getAddress(vector<bool> val, vector<bool>* reg, Ram* ram) {
  return Address(NONE, Util::getFirstAddress());
}

// SHIFT RIGHT

void ShiftRight::exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram) {
  increasePc(pc);
}

Address ShiftRight::getAddress(vector<bool> val, vector<bool>* reg, Ram* ram) {
  return Address(NONE, Util::getFirstAddress());
}

// AND

void And::exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram) {
  increasePc(pc);
}

Address And::getAddress(vector<bool> val, vector<bool>* reg, Ram* ram) {
  return Address(DATA, Util::getBoolNibb(6));
}

// OR

void Or::exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram) {
  increasePc(pc);
}

Address Or::getAddress(vector<bool> val, vector<bool>* reg, Ram* ram) {
  return Address(DATA, Util::getBoolNibb(7));
}

// XOR

void Xor::exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram) {
  increasePc(pc);
}

Address Xor::getAddress(vector<bool> val, vector<bool>* reg, Ram* ram) {
  return getThreeBitAddress(val);
}

// READ POINTER

void ReadPointer::exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram) {
  increasePc(pc);
}

Address ReadPointer::getAddress(vector<bool> val, vector<bool>* reg, Ram* ram) {
  vector<bool> pointer = ram->get(DATA, Util::getSecondNibble(val));
  return Address(DATA, Util::getSecondNibble(pointer));
}

// WRITE POINTER

void WritePointer::exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram) {
  increasePc(pc);
}

Address WritePointer::getAddress(vector<bool> val, vector<bool>* reg, Ram* ram) {
  vector<bool> pointer = ram->get(DATA, Util::getSecondNibble(val));
  return Address(DATA, Util::getSecondNibble(pointer));
}

// INCREASE

void Increase::exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram) {
  increasePc(pc);
}

Address Increase::getAddress(vector<bool> val, vector<bool>* reg, Ram* ram) {
  return getThreeBitAddress(val);
}

// DECREASE

void Decrease::exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram) {
  increasePc(pc);
}

Address Decrease::getAddress(vector<bool> val, vector<bool>* reg, Ram* ram) {
  return getThreeBitAddress(val);
}

// PRINT

void Print::exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram) {
  increasePc(pc);
}

Address Print::getAddress(vector<bool> val, vector<bool>* reg, Ram* ram) {
  return Address(DATA, Util::getSecondNibble(val));
}

// IF NOT MAX

void IfNotMax::exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram) {
  increasePc(pc);
}

Address IfNotMax::getAddress(vector<bool> val, vector<bool>* reg, Ram* ram) {
  return Address(CODE, Util::getSecondNibble(val));
}

// IF NOT MIN

void IfNotMin::exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram) {
  increasePc(pc);
}

Address IfNotMin::getAddress(vector<bool> val, vector<bool>* reg, Ram* ram) {
  return Address(CODE, Util::getSecondNibble(val));
}

// UTIL

void increasePc(vector<bool>* pc) {
  *pc = Util::getBoolNibb(Util::getInt(*pc) + 1);
}

Address getThreeBitAddress(vector<bool> val) {
  vector<bool> adr = Util::getSecondNibble(val);
  adr[0] = false;
  return Address(DATA, adr);
}






