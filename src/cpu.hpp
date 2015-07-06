#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <vector>

#include "const.hpp"

using namespace std;

class Cpu {
	public:
		// INTERFACE
		bool step();
		vector<bool> getRegister();
		vector<bool> getPc();
		vector<bool> getInstructionCode();
		vector<bool> getValue();
		bool hasCodeAddress();
		vector<bool> getCodeAddress();
		bool hasDataAddress();
		vector<bool> getDataAddress();
		int getCycle();
	private:
		// STATE
		vector<bool> reg = vector<bool>(WORD_SIZE);
		vector<bool> pc = vector<bool>(ADDR_SIZE);
		int cycle = 0;
		// INSTRUCTION SET
		void read(vector<bool> adr);
		void write(vector<bool> adr);
		void addSub(vector<bool> adr);
		void incDec(vector<bool> adr);
		void jump(vector<bool> adr);
		void ifMax(vector<bool> adr);
		void ifMin(vector<bool> adr);
		void addImd(vector<bool> value);
		void readPointer(vector<bool> adr);
		void writePointer(vector<bool> adr);
		void shiftLR(vector<bool> value);
		void andOr(vector<bool> value);
		void jumpPointer(vector<bool> adr);
		void ifNotMax(vector<bool> adr);
		void ifNotMin(vector<bool> adr);
		void subImd(vector<bool> value);
		// UTIL
		void increasePc();
		void addOrSubtract(vector<bool> adr, bool add);
		void incOrDec(vector<bool> adr, bool isInc);
		void addOrSubtractImd(vector<bool> value, bool add);
		bool getRegBit(int index);
};

#endif
