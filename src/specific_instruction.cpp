#include "specific_instruction.hpp"

#include <vector>

#include "address.hpp"
#include "const.hpp"
#include "ram.hpp"
#include "util.hpp"

using namespace std;

static void increasePc(vector<bool> &pc);
static Address getThreeBitAddress(vector<bool> val);
static void addOrSubtract(Address adr, vector<bool> &reg, Ram &ram, bool add);
static void shift(vector<bool> &pc, vector<bool> &reg, int delta);
static bool getRegBit(vector<bool> &reg, int index);
static void andOrOr(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram,
                    bool isAnd);
static void incOrDec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram,
                     bool isInc);

// READ

/*
 * Copies value at the passed address to the register.
 */
void Read::exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram) {
  reg = ram.get(adr);
  increasePc(pc);
}

Address Read::getAddress(vector<bool> val, const vector<bool> &reg, 
                        const Ram &ram) {
  return Address(DATA, Util::getSecondNibble(val));
}

string Read::getLabel() {
  return "READ  ";
}

// WRITE

/*
 * Copies value of the register to the passed address.
 */
void Write::exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram) {
  ram.set(adr, reg);
  increasePc(pc);
}

Address Write::getAddress(vector<bool> val, const vector<bool> &reg, 
                          const Ram &ram) {
  return Address(DATA, Util::getSecondNibble(val));;
}

string Write::getLabel() {
  return "WRITE  ";
}

// ADD

/*
 * Adds value at the passed address to the register, and
 * stores result in the register.
 */
void Add::exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram) {
  addOrSubtract(adr, reg, ram, true);
  increasePc(pc);
}

Address Add::getAddress(vector<bool> val, const vector<bool> &reg, 
                        const Ram &ram) {
  return Address(DATA, Util::getSecondNibble(val));
}

string Add::getLabel() {
  return "ADD";
}

// SUB

/*
 * Subtracts value at the passed address from the register, and
 * stores result in the register.
 */
void Sub::exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram) {
  addOrSubtract(adr, reg, ram, false);
  increasePc(pc);
}

Address Sub::getAddress(vector<bool> val, const vector<bool> &reg, 
                        const Ram &ram) {
  return Address(DATA, Util::getSecondNibble(val));
}

string Sub::getLabel() {
  return "SUB";
}

// JUMP

/*
 * Jumps to the passed address.
 */
void Jump::exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram) {
  pc = adr.val;
}

Address Jump::getAddress(vector<bool> val, const vector<bool> &reg, 
                         const Ram &ram) {
  return Address(CODE, Util::getSecondNibble(val));
}

string Jump::getLabel() {
  return "JUMP";
}

// IF MAX

/*
 * Jumps to passed address if value of the register is 'max'.
 */
void IfMax::exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram) {
  if (Util::getInt(reg) >= MAX_VALUE) {
    pc = adr.val;
  } else {
    increasePc(pc);
  }
}

Address IfMax::getAddress(vector<bool> val, const vector<bool> &reg,
                          const Ram &ram) {
  return Address(CODE, Util::getSecondNibble(val));
}

string IfMax::getLabel() {
  return "IF MAX";
}

// IF MIN

/*
 * Jumps to passed address if value of the register is 'min'.
 */
void IfMin::exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram) {
  if (Util::getInt(reg) <= 0) {
    pc = adr.val;
  } else {
    increasePc(pc);
  }
}

Address IfMin::getAddress(vector<bool> val, const vector<bool> &reg,
                          const Ram &ram) {
  return Address(CODE, Util::getSecondNibble(val));
}

string IfMin::getLabel() {
  return "IF MIN";
}

// JUMP REG

/*
 * Jumps to the address stored in register.
 */
void JumpReg::exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram) {
  pc = adr.val;
}

Address JumpReg::getAddress(vector<bool> val, const vector<bool> &reg,
                            const Ram &ram) {
  return Address(CODE, Util::getSecondNibble(reg));
}

string JumpReg::getLabel() {
  return LOGIC_OPS_INDICATOR;
}

// READ REG

/*
 * Copies value at the address that is stored in register
 * to the register.
 */
void ReadReg::exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram) {
  reg = ram.get(adr);
  increasePc(pc);
}

Address ReadReg::getAddress(vector<bool> val, const vector<bool> &reg,
                            const Ram &ram) {
  return Address(DATA, Util::getSecondNibble(reg));
}

string ReadReg::getLabel() {
  return LOGIC_OPS_INDICATOR;
}

// INITIALIZE FIRST ADDRESS

/*
 * Copies value at the ningth address to the first address.
 */
void InitializeFirstAddress::exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram) {
  vector<bool> value = ram.get(Address(DATA, { true, false, false, false }));
  ram.set(adr, value);
  increasePc(pc);
}

Address InitializeFirstAddress::getAddress(vector<bool> val,
                                           const vector<bool> &reg,
                                           const Ram &ram) {
  return Address(DATA, FIRST_ADDRESS);
}

string InitializeFirstAddress::getLabel() {
  return LOGIC_OPS_INDICATOR;
}

// NOT

/*
 * Executes 'not' operation on the value of the register.
 */
void Not::exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram) {
  reg = Util::bitwiseNot(reg);
  increasePc(pc);
}

Address Not::getAddress(vector<bool> val, const vector<bool> &reg,
                        const Ram &ram) {
  return Address(NONE, FIRST_ADDRESS);
}

string Not::getLabel() {
  return LOGIC_OPS_INDICATOR;
}

// SHIFT LEFT

/*
 * Shifts bits in the register one spot to the left.
 */
void ShiftLeft::exec(Address adr, vector<bool> &pc, vector<bool> &reg,
                     Ram &ram) {
  shift(pc, reg, 1);
}

Address ShiftLeft::getAddress(vector<bool> val, const vector<bool> &reg,
                              const Ram &ram) {
  return Address(NONE, FIRST_ADDRESS);
}

string ShiftLeft::getLabel() {
  return LOGIC_OPS_INDICATOR;
}

// SHIFT RIGHT

/*
 * Shifts bits in the register one spot to the right.
 */
void ShiftRight::exec(Address adr, vector<bool> &pc, vector<bool> &reg,
                      Ram &ram) {
  shift(pc, reg, -1);
}

Address ShiftRight::getAddress(vector<bool> val, const vector<bool> &reg,
                               const Ram &ram) {
  return Address(NONE, FIRST_ADDRESS);
}

string ShiftRight::getLabel() {
  return LOGIC_OPS_INDICATOR;
}

// AND

/*
 * Executes 'and' operation between register value, and
 * value at first address and writes the result to register.
 */
void And::exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram) {
  andOrOr(adr, pc, reg, ram, true);
}

Address And::getAddress(vector<bool> val, const vector<bool> &reg,
                        const Ram &ram) {
  return Address(DATA, Util::getBoolNibb(6));
}

string And::getLabel() {
  return LOGIC_OPS_INDICATOR;
}

// OR

/*
 * Executes 'or' operation between register value, and
 * value at first address and writes the result to register.
 */
void Or::exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram) {
  andOrOr(adr, pc, reg, ram, false);
}

Address Or::getAddress(vector<bool> val, const vector<bool> &reg,
                       const Ram &ram) {
  return Address(DATA, Util::getBoolNibb(7));
}

string Or::getLabel() {
  return LOGIC_OPS_INDICATOR;
}

// XOR

/*
 * Executes 'xor' operation between register value, and
 * value at the specified address (0-7) and writes the result to register.
 */
void Xor::exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram) {
  vector<bool> ramValue = ram.get(adr);
  reg = Util::bitwiseXor(reg, ramValue);
  increasePc(pc);
}

Address Xor::getAddress(vector<bool> val, const vector<bool> &reg,
                        const Ram &ram) {
  return getThreeBitAddress(val);
}

string Xor::getLabel() {
  return LOGIC_OPS_INDICATOR;
}

// READ POINTER

/*
 * Reads from the address that is stored at passed address.
 */
void ReadPointer::exec(Address adr, vector<bool> &pc, vector<bool> &reg,
                       Ram &ram) {
  vector<bool> pointer = ram.get(adr);
  reg = ram.get(Address(DATA, pointer));
  increasePc(pc);
}

Address ReadPointer::getAddress(vector<bool> val, const vector<bool> &reg,
                                const Ram &ram) {
  vector<bool> pointer = ram.get(Address(DATA, Util::getSecondNibble(val)));
  return Address(DATA, Util::getSecondNibble(pointer));
}

string ReadPointer::getLabel() {
  return "READ *";
}

// WRITE POINTER

/*
 * Writes 'reg' to the address that is stored at passed address.
 */
void WritePointer::exec(Address adr, vector<bool> &pc, vector<bool> &reg,
                        Ram &ram) {
  vector<bool> pointer = ram.get(adr);
  ram.set(Address(DATA, pointer), reg);
  increasePc(pc);
}

Address WritePointer::getAddress(vector<bool> val, const vector<bool> &reg,
                                 const Ram &ram) {
  vector<bool> pointer = ram.get(Address(DATA, Util::getSecondNibble(val)));
  return Address(DATA, Util::getSecondNibble(pointer));
}

string WritePointer::getLabel() {
  return "WRITE *";
}

// INCREASE

/*
 * Increases value at the passed address, and copies
 * it to the register.
 */
void Increase::exec(Address adr, vector<bool> &pc, vector<bool> &reg,
                    Ram &ram) {
  incOrDec(adr, pc, reg, ram, true);
}

Address Increase::getAddress(vector<bool> val, const vector<bool> &reg,
                             const Ram &ram) {
  return getThreeBitAddress(val);
}

string Increase::getLabel() {
  return "INC";
}

// DECREASE

/*
 * Decreases value at the passed address, and copies
 * it to the register.
 */
void Decrease::exec(Address adr, vector<bool> &pc, vector<bool> &reg,
                    Ram &ram) {
  incOrDec(adr, pc, reg, ram, false);
}

Address Decrease::getAddress(vector<bool> val, const vector<bool> &reg,
                             const Ram &ram) {
  return getThreeBitAddress(val);
}

string Decrease::getLabel() {
  return "DEC";
}

// PRINT

/*
 * Copies value at the passed address to the last address and thus
 * sends it to the printer.
 */
void Print::exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram) {
  vector<bool> val = ram.get(adr);
  ram.set(Address(DATA, LAST_ADDRESS), val);
  increasePc(pc);
}

Address Print::getAddress(vector<bool> val, const vector<bool> &reg,
                          const Ram &ram) {
  return Address(DATA, Util::getSecondNibble(val));
}

string Print::getLabel() {
  return "PRINT";
}

// IF NOT MAX

/*
 * Jumps to passed address if value of the register is not 'max'.
 */
void IfNotMax::exec(Address adr, vector<bool> &pc, vector<bool> &reg,
                    Ram &ram) {
  if (Util::getInt(reg) >= MAX_VALUE) {
    increasePc(pc);
  } else {
    pc = adr.val;
  }
}

Address IfNotMax::getAddress(vector<bool> val, const vector<bool> &reg,
                             const Ram &ram) {
  return Address(CODE, Util::getSecondNibble(val));
}

string IfNotMax::getLabel() {
  return "IF NOT MAX";
}

// IF NOT MIN

/*
 * Jumps to passed address if value of the register is not 'min'.
 */
void IfNotMin::exec(Address adr, vector<bool> &pc, vector<bool> &reg,
                    Ram &ram) {
  if (Util::getInt(reg) <= 0) {
    increasePc(pc);
  } else {
    pc = adr.val;
  }
}

Address IfNotMin::getAddress(vector<bool> val, const vector<bool> &reg,
                             const Ram &ram) {
  return Address(CODE, Util::getSecondNibble(val));
}

string IfNotMin::getLabel() {
  return "IF NOT MIN";
}

//////////
// UTIL //
//////////

void increasePc(vector<bool> &pc) {
  pc = Util::getBoolNibb(Util::getInt(pc) + 1);
}

Address getThreeBitAddress(vector<bool> val) {
  vector<bool> adr = Util::getSecondNibble(val);
  adr[0] = false;
  return Address(DATA, adr);
}

/*
 * Adds or subtracts value at passed address from register,
 * and stores result in the register.
 */
void addOrSubtract(Address adr, vector<bool> &reg, Ram &ram, bool add) {
  int regValue = Util::getInt(reg);
  int ramValue = Util::getInt(ram.get(adr));
  if (add) {
    reg = Util::getBoolByte(regValue + ramValue);
  } else {
    reg = Util::getBoolByte(regValue - ramValue);
  }
}

/*
 * Shifts bits in the register for 'delta' spots.
 */
void shift(vector<bool> &pc, vector<bool> &reg, int delta) {
  vector<bool> tmp = vector<bool>(WORD_SIZE);
  for(int i = 0; i < WORD_SIZE; i++) {
    tmp[i] = getRegBit(reg, i + delta);
  }
  reg = tmp;
  increasePc(pc);
}

/*
 * Returns register bit at passed position, or 'false' if position 
 * is out of bounds.
 */
bool getRegBit(vector<bool> &reg, int index) {
  bool indexOutOfBounds = index < 0 || index >= WORD_SIZE;
  if (indexOutOfBounds) {
    return false;
  }
  return reg.at(index);
}

/*
 * Executes 'and' operation between register value, and
 * value at passed address and writes the result to register.
 */
void andOrOr(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram,
             bool isAnd) {
  vector<bool> ramValue = ram.get(adr);
  reg = Util::bitwiseAndOrOr(reg, ramValue, isAnd);
  increasePc(pc);
}

/*
 * Increases or decreases value at the passed address, and copies
 * it to register.
 */
void incOrDec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram,
              bool isInc) {
  vector<bool> value = ram.get(adr);
  int intValue;
  if (isInc) {
    intValue = Util::getInt(value) + 1;
  } else {
    intValue = Util::getInt(value) - 1;
  }
  ram.set(adr, Util::getBoolByte(intValue));
  reg = Util::getBoolByte(intValue);
  increasePc(pc);
}
