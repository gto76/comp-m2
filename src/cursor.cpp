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

	// Initialize selected bit with the cursor. One per address space.
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
	if (getBitIndex() >= WORD_SIZE-1) {
		return;
	}
	setBitIndex(getBitIndex() + 1);
}

void Cursor::increaseY() {
	if (getAddr() >= RAM_SIZE-1) {
		return;
	}
	setAddr(getAddr() + 1);
}

void Cursor::decreaseX() {
	if (getBitIndex() <= 0) {
		return;
	}
	setBitIndex(getBitIndex() - 1);
}

void Cursor::decreaseY() {
	if (getAddr() <= 0) {
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
	return ram.state[addrSpace].at(getAddr()).at(getBitIndex());
}

void Cursor::setBit(bool bit) {
	ram.state[addrSpace].at(getAddr()).at(getBitIndex()) = bit;
}

vector<bool> Cursor::getWord() {
	return ram.get(addrSpace, Util::getBoolNibb(getAddr()));
}

void Cursor::setWord(vector<bool> word) {
	ram.set(addrSpace, Util::getBoolNibb(getAddr()), word);
}

void Cursor::moveByteUp() {
 	if (getAddr() <= 0) {
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
 	if (getAddr() >= RAM_SIZE-1) {
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


