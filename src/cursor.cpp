#include "cursor.hpp"

#include <algorithm>
#include <map>
#include <tuple>
#include <vector>

#include "const.hpp"
#include "instruction.hpp"

///////// CONSTRUCTOR ///////////

/*
 * Constructor is implemented as function, because on some
 * systems 'const string drawing' from drawing.hpp is not yet
 * initialized at this point. So instead this function is called
 * the first time any of this class variables are needed.
 */
void Cursor::initialize() {
  // Initializes selected bit with the cursor. One per address space.
  cursorPosition[CODE][X] = 0;
  cursorPosition[CODE][Y] = 0;
  cursorPosition[DATA][X] = 0;
  cursorPosition[DATA][Y] = 0;
}

//////// COORDINATES API /////////

int Cursor::getAbsoluteBitIndex() {
  initCheck(); 
  return cursorPosition[addrSpace][Y] * WORD_SIZE +
         cursorPosition[addrSpace][X];
}

int Cursor::getX() {
  initCheck(); 
  return cursorPosition[addrSpace][X];
}
  
int Cursor::getY() {
  initCheck(); 
  return cursorPosition[addrSpace][Y];
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

void Cursor::goToAddress(Address adr) {
  initCheck(); 
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
  Instruction inst = Instruction(getWord(), EMPTY_WORD, ram);
  if (inst.adr.space == NONE) {
    return;
  }
  goToAddress(inst.adr);
}

/////// ADDR SPACE API ////////

void Cursor::switchAddressSpace() {
  initCheck(); 
  if (addrSpace == CODE) {
    addrSpace = DATA;
  } else {
    addrSpace = CODE;
  }
}

AddrSpace Cursor::getAddressSpace() {
  initCheck(); 
  return addrSpace;
}

Address Cursor::getAddress() {
  initCheck(); 
  vector<bool> adrVal = Util::getBoolNibb(cursorPosition.at(addrSpace).at(Y));
  return Address(addrSpace, adrVal);
}

/////////// RAM API ////////////

bool Cursor::getBit() {
  initCheck(); 
  return ram.state.at(addrSpace).at(getAddr()).at(getBitIndex());
}

void Cursor::setBit(bool bit) {
  initCheck(); 
  ram.state[addrSpace].at(getAddr()).at(getBitIndex()) = bit;
}

void Cursor::switchBit() {
  bool bitValue = getBit(); 
  setBit(!bitValue);
}

void Cursor::eraseByte() {
  setWord(EMPTY_WORD);
}

vector<bool> Cursor::getWord() {
  initCheck(); 
  Address adr = Address(addrSpace, Util::getBoolNibb(getAddr()));
  return ram.get(adr);
}

void Cursor::setWord(vector<bool> word) {
  initCheck(); 
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
  initCheck(); 
  if (addrSpace != CODE) {
    return false;
  }
  bool lastWordIsNotEmpty = ram.state[CODE][RAM_SIZE-1] != EMPTY_WORD;
  if (lastWordIsNotEmpty) {
    return false;
  }
  vector<Address> addresses = getAddressesOfAllInstructions();
  Address lastCodeAddress = Address(CODE, Util::getBoolNibb(RAM_SIZE-1));
  bool includesLastAddress = 
      find(addresses.begin(), addresses.end(), lastCodeAddress) != 
      addresses.end();
  if (includesLastAddress) {
    return false;
  }
  incOrDecAddressesPastTheIndex(getY(), 1);
  for (int i = RAM_SIZE-1; i > getY(); i--) {
    ram.state[CODE][i] = ram.state[CODE][i-1];
  }
  ram.state[CODE][getY()] = EMPTY_WORD;
  return true;
}

/*
 * Retruns whether the operation was successful.
 */
bool Cursor::deleteByteAndMoveRestUp() {
  initCheck(); 
  vector<Address> addresses = getAddressesOfAllInstructions();
  bool includesCurrentAddress = 
      find(addresses.begin(), addresses.end(), getAddress()) != 
      addresses.end();
  if (includesCurrentAddress) {
    return false;
  }
  incOrDecAddressesPastTheIndex(getY(), -1);
  for (int i = getY(); i < RAM_SIZE-1; i++) {
    ram.state[CODE][i] = ram.state[CODE][i+1];
  }
  ram.state[CODE][RAM_SIZE-1] = EMPTY_WORD;
  return true;
}

/////////// PRIVATE //////////

vector<Address> Cursor::getAddressesOfAllInstructions() {
  vector<Address> out;
  for (vector<bool> word : ram.state[CODE]) {
    Instruction inst = Instruction(word, EMPTY_WORD, ram);
    out.push_back(inst.firstOrderAdr[0]);
  }
  return out;
}

void Cursor::incOrDecAddressesPastTheIndex(int index, int delta) {
  for (vector<bool> &word : ram.state[CODE]) {
    Instruction inst = Instruction(word, EMPTY_WORD, ram);
    Address adr = inst.firstOrderAdr[0];
    int intVal = Util::getInt(adr.val);
    if (adr.space == CODE && intVal >= index) {
      int newVal = intVal + delta;
      setAddress(word, newVal);
    }
  }
}

void Cursor::setAddress(vector<bool> &word, int val) {
  vector<bool> boolVal = Util::getBoolNibb(val);
  word[4] = boolVal[0];
  word[5] = boolVal[1];
  word[6] = boolVal[2];
  word[7] = boolVal[3];
}

void Cursor::initCheck() {
  if (notInitialized) {
    initialize();
  }
  notInitialized = false;
}

int Cursor::getBitIndex() {
  initCheck(); 
  return cursorPosition[addrSpace][X];
}

int Cursor::getAddr() { 
  initCheck(); 
  return cursorPosition[addrSpace][Y];
}

void Cursor::setBitIndex(int bitIndex) {
  initCheck(); 
  cursorPosition[addrSpace][X] = bitIndex;
}

void Cursor::setByteIndex(int byteIndex) {
  initCheck(); 
  cursorPosition[addrSpace][Y] = byteIndex;
}

void Cursor::setAddr(int addr) {
  initCheck(); 
  cursorPosition[addrSpace][Y] = addr;
}


