#include "cursor.hpp"

#include <algorithm>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>

#include "const.hpp"
#include "instruction.hpp"

using namespace std;

map<int, Instruction> Cursor::BOUND_DATA_ADDRESSES = { 
    { INIT_OPERAND_INDEX, Instruction(INIT_INSTRUCTION, EMPTY_WORD, NULL) },
    { AND_OPERAND_INDEX, Instruction(AND_INSTRUCTION, EMPTY_WORD, NULL) },
    { OR_OPERAND_INDEX, Instruction(OR_INSTRUCTION, EMPTY_WORD, NULL) },
    { LAST_XOR_OPERAND_INDEX, 
      Instruction(LAST_XOR_INSTRUCTION, EMPTY_WORD, NULL) }
};

///////////////////////////////
/////// ADDR SPACE API ////////
///////////////////////////////

void Cursor::switchAddressSpace() {
  if (addrSpace == CODE) {
    addrSpace = DATA;
  } else {
    addrSpace = CODE;
  }
}

AddrSpace Cursor::getAddressSpace() const {
  return addrSpace;
}

Address Cursor::getAddress() const {
  vector<bool> adrVal = Util::getBoolNibb(cursorPosition.at(addrSpace).at(Y));
  return Address(addrSpace, adrVal);
}

//////////////////////////////////
//////// COORDINATES API /////////
//////////////////////////////////

int Cursor::getAbsoluteBitIndex() const {
  return cursorPosition.at(addrSpace).at(Y) * WORD_SIZE +
         cursorPosition.at(addrSpace).at(X);
}

int Cursor::getX() const {
  return cursorPosition.at(addrSpace).at(X);
}
  
int Cursor::getY() const {
  return cursorPosition.at(addrSpace).at(Y);
}

void Cursor::increaseX() {
  bool atRightmostBit = getBitIndex() >= WORD_SIZE-1;
  if (atRightmostBit) {
    return;
  }
  setBitIndex(getBitIndex() + 1);
}

void Cursor::increaseY() {
  bool atLastAddress = getAddr() >= RAM_SIZE-1;
  if (atLastAddress) {
    return;
  }
  setAddr(getAddr() + 1);
}

void Cursor::decreaseX() {
  bool atLeftmostBit = getBitIndex() <= 0;
  if (atLeftmostBit) {
    return;
  }
  setBitIndex(getBitIndex() - 1);
}

void Cursor::decreaseY() {
  bool atFirstAddress = getAddr() <= 0;
  if (atFirstAddress) {
    return;
  }
  setAddr(getAddr() - 1);
}

void Cursor::setBitIndex(int bitIndex) {
  cursorPosition[addrSpace][X] = bitIndex;
}

void Cursor::setByteIndex(int byteIndex) {
  cursorPosition[addrSpace][Y] = byteIndex;
}

void Cursor::goToAddress(Address adr) {
  addrSpace = adr.space;
  setBitIndex(0);
  setByteIndex(Util::getInt(adr.val));
}

void Cursor::goToEndOfWord() {
  if (getX() == WORD_SIZE-1) {
    increaseY();
  }
  setBitIndex(WORD_SIZE-1);
}

void Cursor::goToBeginningOfWord() {
  if (getX() ==0) {
    decreaseY();
  }
  setBitIndex(0);
}

void Cursor::goToBeginningOfNextWord() {
  if (getY() == RAM_SIZE-1) {
    setBitIndex(WORD_SIZE-1);
  } else {
    increaseY();
    setBitIndex(0);
  }
}

void Cursor::goToInstructionsAddress() {
  if (getAddressSpace() == DATA) {
    return;
  }
  Instruction inst = Instruction(getWord(), EMPTY_WORD, &ram);
  if (inst.adr.space == NONE) {
    return;
  }
  goToAddress(inst.adr);
}

////////////////////////////////
/////////// RAM API ////////////
////////////////////////////////

bool Cursor::getBit() const {
  return ram.state.at(addrSpace).at(getAddr()).at(getBitIndex());
}

void Cursor::setBit(bool bit) {
  ram.state[addrSpace].at(getAddr()).at(getBitIndex()) = bit;
}

void Cursor::switchBit() {
  bool bitValue = getBit(); 
  setBit(!bitValue);
}

void Cursor::eraseByte() {
  setWord(EMPTY_WORD);
}

vector<bool> Cursor::getWord() const {
  Address adr = Address(addrSpace, Util::getBoolNibb(getAddr()));
  return ram.get(adr);
}

void Cursor::setWord(vector<bool> word) {
  Address adr = Address(addrSpace, Util::getBoolNibb(getAddr()));
  ram.set(adr, word);
}

void Cursor::moveByteUp() {
  bool atFirstAddress = getAddr() <= 0;
  if (atFirstAddress) {
    return;
  }
  vector<bool> tmp = getWord();
  decreaseY();
  vector<bool> tmp2 = getWord();
  setWord(tmp);
  increaseY();
  setWord(tmp2);
  decreaseY();
}

void Cursor::moveByteDown() {
  bool atLastAddress = getAddr() >= RAM_SIZE-1;
  if (atLastAddress) {
    return;
  }
  vector<bool> tmp = getWord();
  increaseY();
  vector<bool> tmp2 = getWord();
  setWord(tmp);
  decreaseY();
  setWord(tmp2);
  increaseY();
}

/*
 * Retruns whether the operation was successful.
 */
bool Cursor::insertByteAndMoveRestDown() {
  Address lastAddress = Address(addrSpace, Util::getBoolNibb(RAM_SIZE-1));
  if (isAddressUsed(lastAddress)) {
    return false;
  }
  if (addrSpace == DATA) {
    if (shouldNotModifyData(true)) {
      return false;
    }
  }
  incOrDecAddressesPastTheIndex(addrSpace, getY(), 1);
  actuallyInsert();
  return true;
}

/*
 * Retruns whether the operation was successful.
 */
bool Cursor::deleteByteAndMoveRestUp() {
  if (isAddressUsed(getAddress())) {
    eraseByte();
    return false;
  }
  if (addrSpace == DATA) {
    if (shouldNotModifyData(false)) {
      return false;
    }
  }
  incOrDecAddressesPastTheIndex(addrSpace, getY(), -1);
  actuallyDelete();
  return true;
}

/*
 * Returns whether value at address is either non-empty or referenced.
 */
bool Cursor::isAddressUsed(Address adr) {
  bool valueNonEmpty = ram.get(adr) != EMPTY_WORD;
  if (valueNonEmpty) {
    return true;
  }
  if (addressReferenced(adr)) {
    return true;
  }
  return false;
}

//////////////////////////////
/////////// PRIVATE //////////
//////////////////////////////

bool Cursor::addressReferenced(Address adr) {
  vector<Address> addresses = getAddressesOfEffectiveInstructions();
  return find(addresses.begin(), addresses.end(), adr) != addresses.end();
}

vector<Address> Cursor::getAddressesOfEffectiveInstructions() {
  vector<Address> out;
  vector<Instruction> instructions = 
      Instruction::getEffectiveInstructions(ram, EMPTY_WORD);
  for (Instruction inst : instructions) {
    out.push_back(inst.firstOrderAdr.back()); // USED TO BE [0]!!!
  }
  return out;
}

/*
 * True means insert, false delete.
 */
bool Cursor::shouldNotModifyData(bool insert) {
  vector<Instruction> instructions = 
      Instruction::getEffectiveInstructions(ram, EMPTY_WORD);
  int lastAddressToCheck = LAST_XOR_OPERAND_INDEX;
  // For delete we don't need to check if there exists xor instruction, that
  // has 8th address.
  if (!insert) {
    lastAddressToCheck--;
  }
  for (int i = getY(); i <= lastAddressToCheck; i++) {
    bool addressCouldBeBound = BOUND_DATA_ADDRESSES.find(i) != 
                               BOUND_DATA_ADDRESSES.end();
    if (addressCouldBeBound) {
      Instruction boundingInst = BOUND_DATA_ADDRESSES.at(i);
      bool instructionExists = 
          find(instructions.begin(), instructions.end(), boundingInst) != 
          instructions.end();
      if (instructionExists) {
        return true;
      }
    }
  }
  return false;
}

void Cursor::incOrDecAddressesPastTheIndex(AddrSpace space,
                                           int index, int delta) {
  vector<Instruction> allInstructions = 
      Instruction::getAllInstructions(ram, EMPTY_WORD);
  int indexOfLastInst = 
      Instruction::getIndexOfLastNonEmptyInst(allInstructions);
  for (int i = 0; i <= indexOfLastInst; i++) {
    vector<bool> &word = ram.state[CODE].at(i);
    Instruction inst = Instruction(word, EMPTY_WORD, &ram);
    Address adr = inst.firstOrderAdr[0];
    int adrVal = Util::getInt(adr.val);
    bool instPointingToSpace = adr.space == space;
    bool adrPastTheIndex = adrVal >= index;
    bool notLastAdr = adr.val != LAST_ADDRESS;
    if (instPointingToSpace && adrPastTheIndex && notLastAdr) {
      int newVal = adrVal + delta;
      setAddress(word, newVal, inst.inst->getAdrIndex());
    }
  }
}

void Cursor::setAddress(vector<bool> &word, int newAdrVal, int adrIndex) {
  int adrLength = WORD_SIZE-adrIndex;
  vector<bool> newAdr = Util::getBool(newAdrVal, adrLength);
  word.erase(word.begin() + adrIndex, word.end());
  word.insert(word.end(), newAdr.begin(), newAdr.end());
}

void Cursor::actuallyInsert() {
  for (int i = RAM_SIZE-1; i > getY(); i--) {
    ram.state[addrSpace][i] = ram.state[addrSpace][i-1];
  }
  ram.state[addrSpace][getY()] = EMPTY_WORD;
}

void Cursor::actuallyDelete() {
  for (int i = getY(); i < RAM_SIZE-1; i++) {
    ram.state[addrSpace][i] = ram.state[addrSpace][i+1];
  }
  ram.state[addrSpace][RAM_SIZE-1] = EMPTY_WORD;
}

int Cursor::getBitIndex() const {
  return cursorPosition.at(addrSpace).at(X);
}

int Cursor::getAddr() const { 
  return cursorPosition.at(addrSpace).at(Y);
}

void Cursor::setAddr(int addr) {
  cursorPosition[addrSpace][Y] = addr;
}


