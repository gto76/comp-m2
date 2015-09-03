#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <map>
#include <set>
#include <vector>

#include "addr_space.hpp"
#include "address.hpp"
#include "const.hpp"
#include "ram.hpp"

using namespace std;

class Cpu {
	public:
		// INTERFACE
		bool step();
		vector<bool> getRegister();
		vector<bool> getPc();
		vector<bool> getInstructionCode();
		int getInstructionCodeInt();
		vector<bool> getValue();
		bool hasAddress(AddrSpace addrSpace);
		vector<bool> getAddress();
		int getCycle();

		static bool doesInstructionHaveAddress(vector<bool> instruction);
		static AddrSpace getAddressSpaceOfInstruction(vector<bool> instruction);
		static vector<bool> getAddressOfInstruction(vector<bool> instruction, vector<bool> regIn, Ram ram);

	private:
		// STATIC FIELDS
		static const map<AddrSpace, set<int>> INST_WITH_ADDRESS;
		static const map<AddrSpace, set<int>> LOGIC_INST_WITH_ADDRESS;
		static const int LOGIC_INST_ID;
		static const set<int> INST_WITH_3_BIT_ADDRESS;
		// STATE
		vector<bool> reg = vector<bool>(WORD_SIZE);
		vector<bool> pc = vector<bool>(ADDR_SIZE);
		int cycle = 0;
		// INSTRUCTION SET
		void read(vector<bool> adr);
		void write(vector<bool> adr);
		void add(vector<bool> adr);
		void sub(vector<bool> adr);
		void jump(vector<bool> adr);
		void ifMax(vector<bool> adr);
		void ifMin(vector<bool> adr);
		void logic(vector<bool> value);
		void readPointer(vector<bool> adr);
		void writePointer(vector<bool> adr);
		void incDec(vector<bool> adr);
		void print(vector<bool> adr);
		void ifNotMax(vector<bool> adr);
		void ifNotMin(vector<bool> adr);
		// UTIL
		void increasePc();
		void addOrSubtract(vector<bool> adr, bool add);
		void shift(int delta);
		void andOrOr(bool isAnd);
		void bitwiseXor(int intAdr);
		void bitwiseNot();
		void equals();
		void jumpReg();
		void readReg();
		void initializeFirstAddress();
		void incOrDec(vector<bool> adr, bool isInc);
		bool getRegBit(int index);

		static Address getAddressOfLogicInstruction(vector<bool> value, vector<bool> regIn);
		static int getInstructionCodeOfInstruction(vector<bool> instruction);
		static int getValueCodeOfInstruction(vector<bool> instruction);
};

#endif
