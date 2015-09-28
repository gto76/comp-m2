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
  // Address adr = Address(addrSpace, getAddr());
  return ram.get(getAddress());
}

void Cursor::setWord(vector<bool> word) {
  // Address adr = Address(addrSpace, getAddr());
  ram.set(getAddress(), word);
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
  return insertByteAndMoveRestDown(getAddress());
}

/*
 * Retruns whether the operation was successful.
 */
bool Cursor::deleteByteAndMoveRestUp() {
  return deleteByteAndMoveRestUp(getAddress());
}

//////////////////////////////
/////////// PRIVATE //////////
//////////////////////////////

/*
 * Retruns whether the operation was successful.
 */
bool Cursor::insertByteAndMoveRestDown(Address adr) {
  int modifyTo = canModifyTo(true, adr);
  if (!modifyTo) {
    return false;
  }
  incOrDecAddressesInRange(adr.space, Util::getInt(adr.val), modifyTo, 1);
  actuallyInsert(adr, modifyTo);
  return true;
}

/*
 * Retruns whether the operation was successful.
 */
bool Cursor::deleteByteAndMoveRestUp(Address adr) {
  int modifyTo = canModifyTo(false, adr);
  if (!modifyTo) {
    return false;
  }
  incOrDecAddressesInRange(adr.space, Util::getInt(adr.val), modifyTo, -1);
  actuallyDelete(adr, modifyTo);
  return true;
}

/*
 * Return 0 if it should not modify,
 * or index of the address until which should be modified
 */
int Cursor::canModifyTo(bool insert, Address adr) {
  if (adr.space == DATA) {
    int boundAdr = getFirstBoundDataAdr(insert, Util::getInt(adr.val));
    if (boundAdr) {
      return canModifyDataWithBoundAdrTo(insert, boundAdr, adr);
    }
  }
  if (insert) {
    return canInsertTo(adr);
  } else {
    return canDeleteTo(adr);
  }
}

int Cursor::canModifyDataWithBoundAdrTo(bool insert, int boundAdr, Address adr) {
  if (insert) {
    for (int i = boundAdr; i > Util::getInt(adr.val); i--) {
      if (!addressUsed(Address(DATA, Util::getBoolNibb(i)))) {
        return i;
      }
    }
  } else {
    if (addressUsed(adr)) {
      ram.set(adr, EMPTY_WORD);
      return 0;
    } else {
      return boundAdr-1;
    }
  }
  return 0;
}

int Cursor::canInsertTo(Address adr) {
  Address lastAdr = Address(adr.space, Util::getBoolNibb(RAM_SIZE-1));
  bool adrUsed = addressUsed(lastAdr);
  if (adrUsed) {
    Address redundandAdr = getLastRedundandAdr(adr.space);
    bool redundandAdrBeforAdr = Util::getInt(redundandAdr.val) <=
                                Util::getInt(adr.val) + 1; 
    if (redundandAdr.space == NONE || redundandAdrBeforAdr) {
      return 0;
    } else {
      deleteByteAndMoveRestUp(redundandAdr);
    }
  }
  return RAM_SIZE-1;
}

int Cursor::canDeleteTo(Address adr) {
  if (addressUsed(adr)) {
    ram.set(adr, EMPTY_WORD);
    return 0;
  }
  return RAM_SIZE-1;
}

/*
 * Returns whether value at address is either non-empty or referenced.
 */
bool Cursor::addressUsed(Address adr) {
  bool valueNonEmpty = ram.get(adr) != EMPTY_WORD;
  if (valueNonEmpty) {
    return true;
  }
  if (addressReferenced(adr)) {
    return true;
  }
  return false;
}

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
 * Returns bound address index or 0 if none.
 */
int Cursor::getFirstBoundDataAdr(bool insert, int y) {
  vector<Instruction> instructions = 
      Instruction::getEffectiveInstructions(ram, EMPTY_WORD);
  int lastAddressToCheck = LAST_XOR_OPERAND_INDEX;
  // For delete we don't need to check if there exists xor instruction, that
  // has 8th address.
  if (!insert) {
    lastAddressToCheck--;
  }
  for (int i = y; i <= lastAddressToCheck; i++) {
    bool addressCouldBeBound = BOUND_DATA_ADDRESSES.find(i) != 
                               BOUND_DATA_ADDRESSES.end();
    if (addressCouldBeBound) {
      Instruction boundingInst = BOUND_DATA_ADDRESSES.at(i);
      bool instructionExists = 
          find(instructions.begin(), instructions.end(), boundingInst) != 
          instructions.end();
      if (instructionExists) {
        return i;
      }
    }
  }
  return 0;
}

Address Cursor::getLastRedundandAdr(AddrSpace addrSpaceIn) {
  for (int i = RAM_SIZE-2; i >= 1; i--) {
    Address adr = Address(addrSpaceIn, Util::getBoolNibb(i));
    bool adrNotUsed = !addressUsed(adr);
    if (adrNotUsed) {
      if (addrSpaceIn == CODE) {
        bool valueBeforeNotEmpty = 
            ram.get(Address(addrSpaceIn, Util::getBoolNibb(i-1))) != EMPTY_WORD;
        if (valueBeforeNotEmpty) {
          continue;
        }
      }
      return adr;
    }
  }
  return Address(NONE, FIRST_ADDRESS);
}

void Cursor::incOrDecAddressesInRange(AddrSpace space, int indexStart, 
                                      int indexEnd, int delta) {
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
    bool adrPastTheStart = adrVal >= indexStart;
    bool adrBeforeTheEnd = adrVal < indexEnd;
    bool notLastAdr = adr.val != LAST_ADDRESS;
    if (instPointingToSpace && adrPastTheStart && adrBeforeTheEnd && notLastAdr) {
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

void Cursor::actuallyInsert(Address adr, int until) {
  int adrVal = Util::getInt(adr.val);
  for (int i = until; i > adrVal; i--) {
    ram.state[adr.space][i] = ram.state[adr.space][i-1];
  }
  ram.state[adr.space][adrVal] = EMPTY_WORD;
}

void Cursor::actuallyDelete(Address adr, int until) {
  for (int i = Util::getInt(adr.val); i < until; i++) {
    ram.state[adr.space][i] = ram.state[adr.space][i+1];
  }
  ram.state[adr.space][until] = EMPTY_WORD;
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


