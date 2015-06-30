#include <string>
#include <vector>
#include <cmath>

#include "cpu.hpp"
#include "comp.hpp"
#include "util.hpp"

using namespace std;

/*
 * Returns 'false' when reaches last address.
 */
bool Cpu::step() {
	cycle++;
	bool reachedLastAddress = Util::getInt(pc) >= RAM_SIZE;
	if (reachedLastAddress) {
		return false;
	}

	vector<bool> instructionCodeBool = getInstructionCode();
	int instCode = Util::getInt(instructionCodeBool);
	vector<bool> adr = getAddress();

	switch (instCode) {
		case 0:
			read(adr);
			break;
		case 1:
			write(adr);
			break;
		case 2:
			add(adr);
			break;
		case 3:
			sub(adr);
			break;
		case 4:
			jump(adr);
			break;
		case 5:
			jumpIfMax(adr);
			break;
		case 6:
			jumpIfMin(adr);
			break;
		case 7:
			shiftRight();
			break;
		default:
			read(adr);
	}

	return true;
}

int Cpu::getCycle() {
	return cycle;
}

vector<bool> Cpu::getRegister() {
	return reg;
}

vector<bool> Cpu::getPc() {
	return pc;
}

vector<bool> Cpu::getInstructionCode() {
	vector<bool> instruction = Util::getFirstNibble(ram.getInstruction(pc));
	// If instruction id is larger than the number of instructions then 
	// the instruction with id 1 (write) gets executed.
	if (Util::getInt(instruction) >= NUM_OF_INSTRUCTIONS) {
		return Util::getBoolNibb(0);
	}
	return instruction;
}

vector<bool> Cpu::getAddress() {
	return Util::getSecondNibble(ram.getInstruction(pc));
}

void Cpu::increasePc() {
	pc = Util::getBoolNibb(Util::getInt(pc) + 1);
}

void Cpu::read(vector<bool> adr) {
	reg = ram.getData(adr);
	increasePc();
}

void Cpu::write(vector<bool> adr) {
	ram.setData(adr, reg);
	increasePc();
}

void Cpu::add(vector<bool> adr) {
	int regValue = Util::getInt(reg);
	int ramValue = Util::getInt(ram.getData(adr));
	reg = Util::getBoolByte(regValue + ramValue);
	increasePc();
}

void Cpu::sub(vector<bool> adr) {
	int regValue = Util::getInt(reg);
	int ramValue = Util::getInt(ram.getData(adr));
	reg = Util::getBoolByte(regValue - ramValue);
	increasePc();
}

void Cpu::jump(vector<bool> adr) {
	pc = adr;
}

void Cpu::jumpIfMax(vector<bool> adr) {
	if (Util::getInt(reg) >= pow(2, WORD_SIZE)-1) {
		pc = adr;
	} else {
		increasePc();
	}
}

void Cpu::jumpIfMin(vector<bool> adr) {
	if (Util::getInt(reg) <= 0) {
		pc = adr;
	} else {
		increasePc();
	}
}

void Cpu::shiftRight() {
	reg = Util::getBoolByte(Util::getInt(reg) / 2);
	increasePc();
}
