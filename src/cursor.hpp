#ifndef CURSOR_H
#define CURSOR_H

#include <map>
#include <tuple>
#include <vector>

#include "addr_space.hpp"
#include "axis.hpp"
#include "comp.hpp"
#include "ram.hpp"
#include "util.hpp"

using namespace std;

class Cursor {
	public:
		void switchAddressSpace();

		//////// COORDINATES API /////////
		int getX();
		int getY();
		void increaseX();
		void increaseY();
		void decreaseX();
		void decreaseY();

		/////////// RAM API ////////////
		bool getBit();
		void setBit(bool bit);
		vector<bool> getWord();
		void setWord(vector<bool> word);
		void moveByteUp();
		void moveByteDown();

	private:
		// Selected address space.
		AddrSpace addrSpace = CODE;
		// Coordinates of first ram lightbulb in the ascii drawing.
		map<AddrSpace, map<Axis, int>> ramPosition;
		// Selected bit with the cursor.
		map<AddrSpace, map<Axis, int>> cursorPosition;

		bool notInitialized = true;
		void initCheck();
		void initialize();

		int getRamPositionX();
		int getRamPositionY();
		int getBitIndex();
		int getAddr();
		void setBitIndex(int bitIndex);
		void setAddr(int addr);
		void moveByteFor(int delta);
		bool switchWordsInRam(int addr1, int addr2);
};

#endif
