#include "specific_instruction.hpp"

#include <string>
#include <vector>

#include "address.hpp"
#include "const.hpp"
#include "ram.hpp"
#include "util.hpp"

using namespace std;

// UTIL FUNCTIONS
static void increasePc(vector<bool> &pc);
static Address getThreeBitAddress(const vector<bool> &val);
static void addOrSubtract(const Address &adr, vector<bool> &reg, Ram &ram, bool add);
static void shift(vector<bool> &pc, vector<bool> &reg, int delta);
static bool getRegBit(vector<bool> &reg, int index);
static void andOrOr(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram,
                    bool isAnd);
static void incOrDec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram,
                     bool isInc);
static string getOperand(const Address &adr);

// READ

/*
 * Copies value at the passed address to the register.
 */
void Read::exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram) {
  reg = ram.get(adr);
  increasePc(pc);
}

vector<Address> Read::getFirstOrderAdr(vector<bool> &val) {
  return { Address(DATA, Util::getSecondNibble(val)) };
}

Address Read::getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                        const Ram *ram) {
  return firstOrderAdr;
}

string Read::getLabel() {
  return "READ  ";
}

int Read::getAdrIndex() {
  return 4;
}

string Read::getCode(const Address &adr, int pc) {
  return "reg = "+getOperand(adr)+";";
}

// WRITE

/*
 * Copies value of the register to the passed address.
 */
void Write::exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram) {
  ram.set(adr, reg);
  increasePc(pc);
}

vector<Address> Write::getFirstOrderAdr(vector<bool> &val) {
  return { Address(DATA, Util::getSecondNibble(val)) };
}

Address Write::getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                          const Ram *ram) {
  return firstOrderAdr;  
}

string Write::getLabel() {
  return "WRITE  ";
}

int Write::getAdrIndex() {
  return 4;
}

string Write::getCode(const Address &adr, int pc) {
  if (adr.val == LAST_ADDRESS) {
    return "pc = "+to_string(pc)+"; return reg;";
  } else {
    return "data["+adr.getIntStr()+"] = reg;";
  }
}

// ADD

/*
 * Adds value at the passed address to the register, and
 * stores result in the register.
 */
void Add::exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram) {
  addOrSubtract(adr, reg, ram, true);
  increasePc(pc);
}

vector<Address> Add::getFirstOrderAdr(vector<bool> &val) {
  return { Address(DATA, Util::getSecondNibble(val)) };
}

Address Add::getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                        const Ram *ram) {
  return firstOrderAdr;  
}

string Add::getLabel() {
  return "ADD";
}

int Add::getAdrIndex() {
  return 4;
}

string Add::getCode(const Address &adr, int pc) {
  return "reg = sadd(reg, "+getOperand(adr)+");";
}


// SUB

/*
 * Subtracts value at the passed address from the register, and
 * stores result in the register.
 */
void Sub::exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram) {
  addOrSubtract(adr, reg, ram, false);
  increasePc(pc);
}

vector<Address> Sub::getFirstOrderAdr(vector<bool> &val) {
  return { Address(DATA, Util::getSecondNibble(val)) };
}

Address Sub::getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                        const Ram *ram) {
  return firstOrderAdr;  
}

string Sub::getLabel() {
  return "SUB";
}

int Sub::getAdrIndex() {
  return 4;
}

string Sub::getCode(const Address &adr, int pc) {
  return "reg = ssub(reg, "+getOperand(adr)+");";
}

// JUMP

/*
 * Jumps to the passed address.
 */
void Jump::exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram) {
  pc = adr.val;
}

vector<Address> Jump::getFirstOrderAdr(vector<bool> &val) {
  return { Address(CODE, Util::getSecondNibble(val)) };
}

Address Jump::getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                         const Ram *ram) {
  return firstOrderAdr;  
}

string Jump::getLabel() {
  return "JUMP";
}

int Jump::getAdrIndex() {
  return 4;
}

string Jump::getCode(const Address &adr, int pc) {
  return "goto *a["+adr.getIntStr()+"];";
}

// IF MAX

/*
 * Jumps to passed address if value of the register is 'max'.
 */
void IfMax::exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram) {
  if (Util::getInt(reg) >= MAX_VALUE) {
    pc = adr.val;
  } else {
    increasePc(pc);
  }
}

vector<Address> IfMax::getFirstOrderAdr(vector<bool> &val) {
  return { Address(CODE, Util::getSecondNibble(val)) };
}

Address IfMax::getAddress(Address &firstOrderAdr, const vector<bool> &reg,
                          const Ram *ram) {
  return firstOrderAdr;  
}

string IfMax::getLabel() {
  return "IF MAX";
}

int IfMax::getAdrIndex() {
  return 4;
}

string IfMax::getCode(const Address &adr, int pc) {
  return "if (reg == "+to_string(MAX_VALUE)+") goto *a["+adr.getIntStr()+"];";
}

// IF MIN

/*
 * Jumps to passed address if value of the register is 'min'.
 */
void IfMin::exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram) {
  if (Util::getInt(reg) <= 0) {
    pc = adr.val;
  } else {
    increasePc(pc);
  }
}

vector<Address> IfMin::getFirstOrderAdr(vector<bool> &val) {
  return { Address(CODE, Util::getSecondNibble(val)) };
}

Address IfMin::getAddress(Address &firstOrderAdr, const vector<bool> &reg,
                          const Ram *ram) {
  return firstOrderAdr;  
}

string IfMin::getLabel() {
  return "IF MIN";
}

int IfMin::getAdrIndex() {
  return 4;
}

string IfMin::getCode(const Address &adr, int pc) {
  return "if (reg == 0) goto *a["+adr.getIntStr()+"];";
}

// JUMP REG

/*
 * Jumps to the address stored in register.
 */
void JumpReg::exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram) {
  pc = adr.val;
}

vector<Address> JumpReg::getFirstOrderAdr(vector<bool> &val) {
  return { Address(NONE, FIRST_ADDRESS) };
}

Address JumpReg::getAddress(Address &firstOrderAdr, const vector<bool> &reg,
                            const Ram *ram) {
  return Address(CODE, Util::getSecondNibble(reg));
}

string JumpReg::getLabel() {
  return LOGIC_OPS_INDICATOR;
}

int JumpReg::getAdrIndex() {
  return -1;
}

string JumpReg::getCode(const Address &adr, int pc) {
  return "goto *a[reg&"+to_string(RAM_SIZE)+"];";
}

// READ REG

/*
 * Copies value at the address that is stored in register
 * to the register.
 */
void ReadReg::exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram) {
  reg = ram.get(adr);
  increasePc(pc);
}

vector<Address> ReadReg::getFirstOrderAdr(vector<bool> &val) {
  return { Address(NONE, FIRST_ADDRESS) };
}

Address ReadReg::getAddress(Address &firstOrderAdr, const vector<bool> &reg,
                            const Ram *ram) {
  return Address(DATA, Util::getSecondNibble(reg));
}

string ReadReg::getLabel() {
  return LOGIC_OPS_INDICATOR;
}

int ReadReg::getAdrIndex() {
  return -1;
}

string ReadReg::getCode(const Address &adr, int pc) {
  return "reg = data[reg&"+to_string(RAM_SIZE)+"];";
}

// INITIALIZE FIRST ADDRESS

/*
 * Copies value at the ningth address to the first address.
 */
void InitializeFirstAddress::exec(const Address &adr, vector<bool> &pc,
                                  vector<bool> &reg, Ram &ram) {
  vector<bool> value = ram.get(Address(DATA, 
                               Util::getBoolNibb(INIT_OPERAND_INDEX)));
  ram.set(adr, value);
  reg = value;
  increasePc(pc);
}

vector<Address> InitializeFirstAddress::getFirstOrderAdr(vector<bool> &val) {
  return { Address(DATA, FIRST_ADDRESS), 
           Address(DATA, Util::getBoolNibb(INIT_OPERAND_INDEX)) };
}

Address InitializeFirstAddress::getAddress(Address &firstOrderAdr,
                                           const vector<bool> &reg,
                                           const Ram *ram) {
  return firstOrderAdr;  
}

string InitializeFirstAddress::getLabel() {
  return LOGIC_OPS_INDICATOR;
}

int InitializeFirstAddress::getAdrIndex() {
  return -1;
}

string InitializeFirstAddress::getCode(const Address &adr, int pc) {
  return "data[0] = data["+to_string(INIT_OPERAND_INDEX)+"]; reg = data[0];";
}

// NOT

/*
 * Executes 'not' operation on the value of the register.
 */
void Not::exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram) {
  reg = Util::bitwiseNot(reg);
  increasePc(pc);
}

vector<Address> Not::getFirstOrderAdr(vector<bool> &val) {
  return { Address(NONE, FIRST_ADDRESS) };
}

Address Not::getAddress(Address &firstOrderAdr, const vector<bool> &reg,
                        const Ram *ram) {
  return firstOrderAdr;  
}

string Not::getLabel() {
  return LOGIC_OPS_INDICATOR;
}

int Not::getAdrIndex() {
  return -1;
}

string Not::getCode(const Address &adr, int pc) {
  return "reg = ~reg;";
}

// SHIFT LEFT

/*
 * Shifts bits in the register one spot to the left.
 */
void ShiftLeft::exec(const Address &adr, vector<bool> &pc, vector<bool> &reg,
                     Ram &ram) {
  shift(pc, reg, 1);
}

vector<Address> ShiftLeft::getFirstOrderAdr(vector<bool> &val) {
  return { Address(NONE, FIRST_ADDRESS) };
}

Address ShiftLeft::getAddress(Address &firstOrderAdr, const vector<bool> &reg,
                              const Ram *ram) {
  return firstOrderAdr;  
}

string ShiftLeft::getLabel() {
  return LOGIC_OPS_INDICATOR;
}

int ShiftLeft::getAdrIndex() {
  return -1;
}

string ShiftLeft::getCode(const Address &adr, int pc) {
  return "reg <<= 1;";
}

// SHIFT RIGHT

/*
 * Shifts bits in the register one spot to the right.
 */
void ShiftRight::exec(const Address &adr, vector<bool> &pc, vector<bool> &reg,
                      Ram &ram) {
  shift(pc, reg, -1);
}

vector<Address> ShiftRight::getFirstOrderAdr(vector<bool> &val) {
  return { Address(NONE, FIRST_ADDRESS) };
}

Address ShiftRight::getAddress(Address &firstOrderAdr, const vector<bool> &reg,
                               const Ram *ram) {
  return firstOrderAdr;  
}

string ShiftRight::getLabel() {
  return LOGIC_OPS_INDICATOR;
}

int ShiftRight::getAdrIndex() {
  return -1;
}

string ShiftRight::getCode(const Address &adr, int pc) {
  return "reg >>= 1;";
}

// AND

/*
 * Executes 'and' operation between register value, and
 * value at first address and writes the result to register.
 */
void And::exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram) {
  andOrOr(adr, pc, reg, ram, true);
}

vector<Address> And::getFirstOrderAdr(vector<bool> &val) {
  return { Address(DATA, Util::getBoolNibb(AND_OPERAND_INDEX)) };
}

Address And::getAddress(Address &firstOrderAdr, const vector<bool> &reg,
                        const Ram *ram) {
  return firstOrderAdr;  
}

string And::getLabel() {
  return LOGIC_OPS_INDICATOR;
}

int And::getAdrIndex() {
  return -1;
}

string And::getCode(const Address &adr, int pc) {
  return "reg &= data["+to_string(AND_OPERAND_INDEX)+"];";
}

// OR

/*
 * Executes 'or' operation between register value, and
 * value at first address and writes the result to register.
 */
void Or::exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram) {
  andOrOr(adr, pc, reg, ram, false);
}

vector<Address> Or::getFirstOrderAdr(vector<bool> &val) {
  return { Address(DATA, Util::getBoolNibb(OR_OPERAND_INDEX)) };
}

Address Or::getAddress(Address &firstOrderAdr, const vector<bool> &reg,
                       const Ram *ram) {
  return firstOrderAdr;  
}

string Or::getLabel() {
  return LOGIC_OPS_INDICATOR;
}

int Or::getAdrIndex() {
  return -1;
}

string Or::getCode(const Address &adr, int pc) {
  return "reg |= data["+to_string(OR_OPERAND_INDEX)+"];";
}

// XOR

/*
 * Executes 'xor' operation between register value, and
 * value at the specified address (0-7) and writes the result to register.
 */
void Xor::exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram) {
  vector<bool> ramValue = ram.get(adr);
  reg = Util::bitwiseXor(reg, ramValue);
  increasePc(pc);
}

vector<Address> Xor::getFirstOrderAdr(vector<bool> &val) {
  return { getThreeBitAddress(val) };
}

Address Xor::getAddress(Address &firstOrderAdr, const vector<bool> &reg,
                        const Ram *ram) {
  return firstOrderAdr;  
}

string Xor::getLabel() {
  return LOGIC_OPS_INDICATOR;
}

int Xor::getAdrIndex() {
  return 5;
}

string Xor::getCode(const Address &adr, int pc) {
  return "reg ^= "+getOperand(adr)+";";
}

// READ POINTER

/*
 * Reads from the address that is stored at passed address.
 */
void ReadPointer::exec(const Address &adr, vector<bool> &pc, vector<bool> &reg,
                       Ram &ram) {
  reg = ram.get(adr);
  increasePc(pc);
}

vector<Address> ReadPointer::getFirstOrderAdr(vector<bool> &val) {
  return { Address(DATA, Util::getSecondNibble(val)) };
}

Address ReadPointer::getAddress(Address &firstOrderAdr, const vector<bool> &reg,
                                const Ram *ram) {
  // If ram is NULL, then treat it as a empty ram.
  if (ram == NULL) {
    Address(DATA, FIRST_ADDRESS);
  }
  vector<bool> pointer = ram->get(firstOrderAdr);
  return Address(DATA, Util::getSecondNibble(pointer));
}

string ReadPointer::getLabel() {
  return "READ *";
}

int ReadPointer::getAdrIndex() {
  return 4;
}

string ReadPointer::getCode(const Address &adr, int pc) {
  return "adr = "+getOperand(adr)+"&"+to_string(RAM_SIZE)+"; "
         "if (adr == "+to_string(RAM_SIZE)+") reg = predecesor(); "
         "else reg = data[adr];";
}

// WRITE POINTER

/*
 * Writes 'reg' to the address that is stored at passed address.
 */
void WritePointer::exec(const Address &adr, vector<bool> &pc, vector<bool> &reg,
                        Ram &ram) {
  ram.set(adr, reg);
  increasePc(pc);
}

vector<Address> WritePointer::getFirstOrderAdr(vector<bool> &val) {
  return { Address(DATA, Util::getSecondNibble(val)) };
}

Address WritePointer::getAddress(Address &firstOrderAdr, const vector<bool> &reg,
                                 const Ram *ram) {
  // If ram is NULL, then treat it as a empty ram.
  if (ram == NULL) {
    Address(DATA, FIRST_ADDRESS);
  }
  vector<bool> pointer = ram->get(firstOrderAdr);
  return Address(DATA, Util::getSecondNibble(pointer));
}

string WritePointer::getLabel() {
  return "WRITE *";
}

int WritePointer::getAdrIndex() {
  return 4;
}

string WritePointer::getCode(const Address &adr, int pc) {
  return "pc = "+to_string(pc)+"; "
         "adr = "+getOperand(adr)+"&"+to_string(RAM_SIZE)+"; "
         "if (adr == "+to_string(RAM_SIZE)+") return reg; "
         "else data[adr] = reg;";
}

// INCREASE

/*
 * Increases value at the passed address, and copies
 * it to the register.
 */
void Increase::exec(const Address &adr, vector<bool> &pc, vector<bool> &reg,
                    Ram &ram) {
  incOrDec(adr, pc, reg, ram, true);
}

vector<Address> Increase::getFirstOrderAdr(vector<bool> &val) {
  return { getThreeBitAddress(val) };
}

Address Increase::getAddress(Address &firstOrderAdr, const vector<bool> &reg,
                             const Ram *ram) {
  return firstOrderAdr;  
}

string Increase::getLabel() {
  return "INC/DEC";
}

int Increase::getAdrIndex() {
  return 5;
}

string Increase::getCode(const Address &adr, int pc) {
  return "data["+adr.getIntStr()+"]++; "
         "reg = data["+adr.getIntStr()+"];";
}

// DECREASE

/*
 * Decreases value at the passed address, and copies
 * it to the register.
 */
void Decrease::exec(const Address &adr, vector<bool> &pc, vector<bool> &reg,
                    Ram &ram) {
  incOrDec(adr, pc, reg, ram, false);
}

vector<Address> Decrease::getFirstOrderAdr(vector<bool> &val) {
  return { getThreeBitAddress(val) };
}

Address Decrease::getAddress(Address &firstOrderAdr, const vector<bool> &reg,
                             const Ram *ram) {
  return firstOrderAdr;  
}

string Decrease::getLabel() {
  return "INC/DEC";
}

int Decrease::getAdrIndex() {
  return 5;
}

string Decrease::getCode(const Address &adr, int pc) {
  return "data["+adr.getIntStr()+"]--; "
         "reg = data["+adr.getIntStr()+"];";
}

// PRINT

/*
 * Copies value at the passed address to the last address and thus
 * sends it to the printer.
 */
void Print::exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram) {
  vector<bool> val = ram.get(adr);
  ram.set(Address(DATA, LAST_ADDRESS), val);
  increasePc(pc);
}

vector<Address> Print::getFirstOrderAdr(vector<bool> &val) {
  return { Address(DATA, Util::getSecondNibble(val)) };
}

Address Print::getAddress(Address &firstOrderAdr, const vector<bool> &reg,
                          const Ram *ram) {
  return firstOrderAdr;  
}

string Print::getLabel() {
  return "PRINT";
}

int Print::getAdrIndex() {
  return 4;
}

string Print::getCode(const Address &adr, int pc) {
  return "pc = "+to_string(pc)+"; "
         "return "+getOperand(adr)+";";
}

// IF NOT MAX

/*
 * Jumps to passed address if value of the register is not 'max'.
 */
void IfNotMax::exec(const Address &adr, vector<bool> &pc, vector<bool> &reg,
                    Ram &ram) {
  if (Util::getInt(reg) >= MAX_VALUE) {
    increasePc(pc);
  } else {
    pc = adr.val;
  }
}

vector<Address> IfNotMax::getFirstOrderAdr(vector<bool> &val) {
  return { Address(CODE, Util::getSecondNibble(val)) };
}

Address IfNotMax::getAddress(Address &firstOrderAdr, const vector<bool> &reg,
                             const Ram *ram) {
  return firstOrderAdr;  
}

string IfNotMax::getLabel() {
  return "IF NOT MAX";
}

int IfNotMax::getAdrIndex() {
  return 4;
}

string IfNotMax::getCode(const Address &adr, int pc) {
  return "if (reg != "+to_string(MAX_VALUE)+") goto *a["+adr.getIntStr()+"];";
}

// IF NOT MIN

/*
 * Jumps to passed address if value of the register is not 'min'.
 */
void IfNotMin::exec(const Address &adr, vector<bool> &pc, vector<bool> &reg,
                    Ram &ram) {
  if (Util::getInt(reg) <= 0) {
    increasePc(pc);
  } else {
    pc = adr.val;
  }
}

vector<Address> IfNotMin::getFirstOrderAdr(vector<bool> &val) {
  return { Address(CODE, Util::getSecondNibble(val)) };
}

Address IfNotMin::getAddress(Address &firstOrderAdr, const vector<bool> &reg,
                             const Ram *ram) {
  return firstOrderAdr;  
}

string IfNotMin::getLabel() {
  return "IF NOT MIN";
}

int IfNotMin::getAdrIndex() {
  return 4;
}

string IfNotMin::getCode(const Address &adr, int pc) {
  return "if (reg != 0) goto *a["+adr.getIntStr()+"];";
}

//////////
// UTIL //
//////////

void increasePc(vector<bool> &pc) {
  pc = Util::getBoolNibb(Util::getInt(pc) + 1);
}

Address getThreeBitAddress(const vector<bool> &val) {
  vector<bool> adr = Util::getSecondNibble(val);
  adr[0] = false;
  return Address(DATA, adr);
}

/*
 * Adds or subtracts value at passed address from register,
 * and stores result in the register.
 */
void addOrSubtract(const Address &adr, vector<bool> &reg, Ram &ram, bool add) {
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
void andOrOr(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram,
             bool isAnd) {
  vector<bool> ramValue = ram.get(adr);
  reg = Util::bitwiseAndOrOr(reg, ramValue, isAnd);
  increasePc(pc);
}

/*
 * Increases or decreases value at the passed address, and copies
 * it to register.
 */
void incOrDec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram,
              bool isInc) {
  vector<bool> value = ram.get(adr);
  int intValue = Util::getInt(value);
  if (isInc) {
    if (intValue == MAX_VALUE) {
      intValue = 0;
    } else {
      intValue++;
    }
  } else {
    if (intValue == 0) {
      intValue = MAX_VALUE;
    } else {
      intValue--;
    }
  }
  ram.set(adr, Util::getBoolByte(intValue));
  reg = Util::getBoolByte(intValue);
  increasePc(pc);
}

string getOperand(const Address &adr) {
  if (adr.val == LAST_ADDRESS) {
    return "predecesor()";
  } else {
    return "data["+adr.getIntStr()+"]";
  }
}
