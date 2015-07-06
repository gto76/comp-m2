#include "cpu.hpp"

#include <cmath>
#include <string>
#include <vector>
#include <set>

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
	return Util::getFirstNibble(ram.get(CODE, pc));
}

vector<bool> Cpu::getValue() {
	return Util::getSecondNibble(ram.get(CODE, pc));
}

bool Cpu::hasDataAddress() {
	vector<bool> instructionCodeBool = getInstructionCode();
	int instCode = Util::getInt(instructionCodeBool);
	set<int> instWithoutDataAddress = { 4, 5, 6, 7, 10, 13, 14, 15 };
	return instWithoutDataAddress.count(instCode) == 0;
}

vector<bool> Cpu::getDataAddress() {
	vector<bool> instructionCodeBool = getInstructionCode();
	int instCode = Util::getInt(instructionCodeBool);

	// If instruction has only 3 bits for address.
	set<int> instWith3bitAddress = { 2, 3, 11 };
	if (instWith3bitAddress.count(instCode) == 1) {
		vector<bool> value = getValue();
		return { false, value.at(1), value.at(2), value.at(3) };
	} else { 
		return Util::getSecondNibble(ram.get(CODE, pc));
	}
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
			addSub(value);
			break;
		case 3:
			incDec(value);
			break;
		case 4:
			jump(value);
			break;
		case 5:
			ifMax(value);
			break;
		case 6:
			ifMin(value);
			break;
		case 7:
			addImd(value);
			break;
		case 8:
			readPointer(value);
			break;
		case 9:
			writePointer(value);
			break;
		case 10:
			shiftLR(value);
			break;
		case 11:
			andOr(value);
			break;
		case 12:
			jumpPointer(value);
			break;
		case 13:
			ifNotMax(value);
			break;
		case 14:
			ifNotMin(value);
			break;
		case 15:
			subImd(value);
			break;
		default:
			read(value);
	}

	return true;
}

///////////////////////////////
/////// INSTRUCTION SET ///////
///////////////////////////////

void Cpu::read(vector<bool> adr) {
	reg = ram.get(DATA, adr);
	increasePc();
}

void Cpu::write(vector<bool> adr) {
	ram.set(DATA, adr, reg);
	increasePc();
}

void Cpu::addSub(vector<bool> value) {
	bool isAnd = !value.at(0);
	vector<bool> adr = { false, value.at(1), value.at(2), value.at(3) };
	addOrSubtract(adr, isAnd);
	increasePc();
}

void Cpu::incDec(vector<bool> value) {
	bool isInc = !value.at(0);
	vector<bool> adr = { false, value.at(1), value.at(2), value.at(3) };
	incOrDec(adr, isInc);
	increasePc();
}

void Cpu::jump(vector<bool> adr) {
	pc = adr;
}

void Cpu::ifMax(vector<bool> adr) {
	if (Util::getInt(reg) >= pow(2, WORD_SIZE)-1) {
		pc = adr;
	} else {
		increasePc();
	}
}

void Cpu::ifMin(vector<bool> adr) {
	if (Util::getInt(reg) <= 0) {
		pc = adr;
	} else {
		increasePc();
	}
}

/*
 * Adds passed value to the register.
 */
void Cpu::addImd(vector<bool> value) {
	addOrSubtractImd(value, true);
	increasePc();
}

/*
 * Reads from the address that is stored at passed address.
 */
void Cpu::readPointer(vector<bool> adr) {
	vector<bool> pointer = ram.get(DATA, adr);
	reg = ram.get(DATA, pointer);
	increasePc();
}

/*
 * Writes 'reg' to the address that is stored at passed address.
 */
void Cpu::writePointer(vector<bool> adr) {
	vector<bool> pointer = ram.get(DATA, adr);
	ram.set(DATA, pointer, reg);
	increasePc();
}

/*
 * Splits passed value to first bit, that tells the direction of the shift,
 * and last three bits, that tell the range.
 */
void Cpu::shiftLR(vector<bool> value) {
	int noOfSpots = Util::getSignedIntFromNibble(value) * -1;
	vector<bool> tmp = vector<bool>(WORD_SIZE);
	for(int i = 0; i < WORD_SIZE; i++) {
		tmp[i] = getRegBit(i - noOfSpots);
	}
	reg = tmp;
	increasePc();
}

/*
 * Splits passed value to first bit, that tells wether 'and' or 'or'
 * should be executed, and last three bits, that tell the address.
 * Then executes 'and' or 'or' operation between register value, and
 * value at address and writes the result to register. Since only three
 * bits are used for address, this instruction can only be used with first
 * eight addresses of data ram.
 */
void Cpu::andOr(vector<bool> value) {
	bool isAnd = !value.at(0);
	vector<bool> adr = { false, value.at(1), value.at(2), value.at(3) };
	vector<bool> ramValue = ram.get(DATA, adr);
	reg = Util::bitwiseAndOrOr(reg, ramValue, isAnd);
	increasePc();
}

/*
 * Jumps to address that is stored at passed addres.
 */
void Cpu::jumpPointer(vector<bool> adr) {
	vector<bool> value = ram.get(DATA, adr);
	// Takes the last four bits as a address.
	pc = Util::getSecondNibble(value);
}

void Cpu::ifNotMax(vector<bool> adr) {
	if (Util::getInt(reg) >= pow(2, WORD_SIZE)-1) {
		increasePc();
	} else {
		pc = adr;
	}
}

void Cpu::ifNotMin(vector<bool> adr) {
	if (Util::getInt(reg) <= 0) {
		increasePc();
	} else {
		pc = adr;
	}
}

/*
 * Subtracts passed value from the register.
 */
void Cpu::subImd(vector<bool> value) {
	addOrSubtractImd(value, false);
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
}

void Cpu::incOrDec(vector<bool> adr, bool isInc) {
	vector<bool> value = ram.get(DATA, adr);
	int delta = 0;
	if (isInc) {
		delta = 1;
	} else {
		delta = -1;
	}
	int intValue = Util::getInt(value) + delta;
	ram.set(DATA, adr, Util::getBoolByte(intValue));
}

void Cpu::addOrSubtractImd(vector<bool> value, bool add) {
	int valueIn = Util::getInt(value);
	int regValue = Util::getInt(reg);
	if (add) {
		reg = Util::getBoolByte(regValue + valueIn);
	} else {
		reg = Util::getBoolByte(regValue - valueIn);
	}
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
