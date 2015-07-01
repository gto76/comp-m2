#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <vector>

#include "const.hpp"

using namespace std;

class Cpu {
	public:
		bool step();
		vector<bool> getRegister();
		vector<bool> getPc();
		vector<bool> getInstructionCode();
		vector<bool> getValue();
		int getCycle();
	private:
		vector<bool> reg = vector<bool>(WORD_SIZE);
		vector<bool> pc = vector<bool>(ADDR_SIZE);
		int cycle = 0;
		void read(vector<bool> adr);
		void write(vector<bool> adr);
		void add(vector<bool> adr);
		void sub(vector<bool> adr);
		void jumpImd(vector<bool> adr);
		void jumpIfMax(vector<bool> adr);
		void jumpIfMin(vector<bool> adr);
		void shift(vector<bool> value);
		// UTIL:
		void increasePc();
		void addOrSubtract(vector<bool> adr, bool add);
		bool getRegBit(int index);
};

#endif
