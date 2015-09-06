#include "cursor.hpp"

#include <map>
#include <tuple>
#include <vector>

#include "const.hpp"
#include "drawing.hpp"

///////// CONSTRUCTOR ///////////

/*
 * Constructor is ipmlemented as function, because on some
 * systems 'const string drawing' from drawing.hpp is not yet
 * initialized at this point. So instead this function is called
 * the first time any of this class variables are needed.
 */
void Cursor::initialize() {
  // Coordinates of first ram lightbulb in the ascii drawing. 
  // One per address space.
  tuple<int, int> codeRamPosition = 
      Util::getCoordinatesOfFirstOccurance(&drawing, 'a');
  tuple<int, int> dataRamPosition = 
      Util::getCoordinatesOfFirstOccurance(&drawing, 'b');
  ramPosition[CODE][X] = get<0>(codeRamPosition);
  ramPosition[CODE][Y] = get<1>(codeRamPosition);
  ramPosition[DATA][X] = get<0>(dataRamPosition);
  ramPosition[DATA][Y] = get<1>(dataRamPosition);

  // Initializes selected bit with the cursor. One per address space.
  cursorPosition[CODE][X] = 0;
  cursorPosition[CODE][Y] = 0;
  cursorPosition[DATA][X] = 0;
  cursorPosition[DATA][Y] = 0;
}

//////// COORDINATES API /////////

int Cursor::getX() {
  return getBitIndex() + getRamPositionX();
}

int Cursor::getY() {
  return getAddr() + getRamPositionY();
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

/////// ADDR SPACE API ////////

void Cursor::switchAddressSpace() {
  if (addrSpace == CODE) {
    addrSpace = DATA;
  } else {
    addrSpace = CODE;
  }
}

AddrSpace Cursor::getAddressSpace() {
  return addrSpace;
}

/////////// RAM API ////////////

bool Cursor::getBit() {
  return ram.state.at(addrSpace).at(getAddr()).at(getBitIndex());
}

void Cursor::setBit(bool bit) {
  ram.state[addrSpace].at(getAddr()).at(getBitIndex()) = bit;
}

vector<bool> Cursor::getWord() {
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

/////////// PRIVATE //////////

void Cursor::initCheck() {
  if (notInitialized) {
    initialize();
  }
  notInitialized = false;
}

int Cursor::getRamPositionX() {
  initCheck(); 
  return ramPosition[addrSpace][X];
}

int Cursor::getRamPositionY() {
  initCheck(); 
  return ramPosition[addrSpace][Y];
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

void Cursor::setAddr(int addr) {
  initCheck(); 
  cursorPosition[addrSpace][Y] = addr;
}


