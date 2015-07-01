#include "cpu.hpp"

#include <cmath>
#include <string>
#include <vector>

#include "comp.hpp"
#include "util.hpp"

using namespace std;


/////////////////////////
/////// INTERFACE ///////
/////////////////////////

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
	vector<bool> instruction = Util::getFirstNibble(ram.get(CODE, pc));
	// If instruction id is larger than the number of instructions then 
	// the instruction with id 1 (write) gets executed.
	if (Util::getInt(instruction) >= NUM_OF_INSTRUCTIONS) {
		return Util::getBoolNibb(0);
	}
	return instruction;
}

vector<bool> Cpu::getValue() {
	return Util::getSecondNibble(ram.get(CODE, pc));
}

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
	vector<bool> value = getValue();

	switch (instCode) {
		case 0:
			read(value);
			break;
		case 1:
			write(value);
			break;
		case 2:
			add(value);
			break;
		case 3:
			sub(value);
			break;
		case 4:
			jumpImd(value);
			break;
		case 5:
			jumpIfMax(value);
			break;
		case 6:
			jumpIfMin(value);
			break;
		case 7:
			shift(value);
			break;

/* TODO
		case 8:
			readIn(value);
			break;
		case 9:
			writeOut(value);
			break;
		case 10:
			addImd(value);
			break;
		case 11:
			subImd(value);
			break;
		case 12:
			jump(value);
			break;
		case 13:
			jumpIfNotMax(value);
			break;
		case 14:
			jumpIfNotMin(value);
			break;
		case 15:
			andOr();
			break;
*/

		default:
			read(value);
	}

	return true;
}

////////////////////////////
/////// INSTRUCTIONS ///////
////////////////////////////

void Cpu::read(vector<bool> adr) {
	reg = ram.get(DATA, adr);
	increasePc();
}

void Cpu::write(vector<bool> adr) {
	ram.set(DATA, adr, reg);
	increasePc();
}

void Cpu::add(vector<bool> adr) {
	addOrSubtract(adr, true);
}

void Cpu::sub(vector<bool> adr) {
	addOrSubtract(adr, false);
}

void Cpu::jumpImd(vector<bool> adr) {
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

void Cpu::shift(vector<bool> value) {
	int noOfSpots = Util::getSignedIntFromNibble(value);
	vector<bool> tmp = vector<bool>(WORD_SIZE);
	for(int i = 0; i < WORD_SIZE; i++) {
		tmp[i] = getRegBit(i - noOfSpots);
	}
	reg = tmp;
	increasePc();
}

///////////////////
////// UTIL ///////
///////////////////

void Cpu::increasePc() {
	pc = Util::getBoolNibb(Util::getInt(pc) + 1);
}

void Cpu::addOrSubtract(vector<bool> adr, bool add) {
	int regValue = Util::getInt(reg);
	int ramValue = Util::getInt(ram.get(DATA, adr));
	if (add) {
		reg = Util::getBoolByte(regValue + ramValue);
	} else {
		reg = Util::getBoolByte(regValue - ramValue);
	}
	increasePc();
}

/*
 * Returns bit with 'index' from register, or 'false' if index is 
 * out of bounds.
 */
bool Cpu::getRegBit(int index) {
	bool indexOutOfBounds = index < 0 || index >= WORD_SIZE;
	if (indexOutOfBounds) {
		return false;
	}
	return reg.at(index);
}

