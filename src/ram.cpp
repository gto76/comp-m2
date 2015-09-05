#include "ram.hpp"

#include <string>

#include "comp.hpp"
#include "const.hpp"
#include "util.hpp"

using namespace std;

Ram::Ram() {
	// Initialize the state, one per address space.
	state[CODE] = vector<vector<bool>>(RAM_SIZE, vector<bool>(WORD_SIZE));
	state[DATA] = vector<vector<bool>>(RAM_SIZE, vector<bool>(WORD_SIZE));
}

////// GET //////

// vector<bool> Ram::get(AddrSpace addrSpace, vector<bool> adr) const {
// 	// Only look at last 4 bits of passed address.   
// 	vector<bool> adrShaved(adr.end() - ADDR_SIZE, adr.end());
// 	int address = Util::getInt(adrShaved);
// 	if (address == RAM_SIZE) {
// 		return Ram::getLastAddress(addrSpace);
// 	}
// 	return state.at(addrSpace).at(address);
// }

vector<bool> Ram::get(Address adr) const {
  AddrSpace space = adr.space;
  vector<bool> value = adr.val;
	// Only look at last 4 bits of passed address.   
	vector<bool> adrShaved(value.end() - ADDR_SIZE, value.end());
	int adrIndex = Util::getInt(adrShaved);
	if (adrIndex == RAM_SIZE) {
		return Ram::getLastAddress(space);
	}
	return state.at(space).at(adrIndex);
}

vector<bool> Ram::getLastAddress(AddrSpace addrSpace) const {
	if (addrSpace == CODE) {
		fprintf(stderr, "Error in function Ram::getInstruction, "
			"invalid address");
		exit(4);
	} else {
		return Ram::getInput();
	}
}

/*
 * Returns random value if last address is passed (reserved for output),
 * or reads from pipe if also input is piped in.
 */
vector<bool> Ram::getInput() const {
	if (interactivieMode) {
		return Util::getRandomWord();  
	} else {
		return Util::readWordFromPipe();  // TODO it must be possible to run it in noninteractive withouth pipe
	}
}

////// SET //////

// void Ram::set(AddrSpace addrSpace, vector<bool> adr, vector<bool> wordIn) {
// 	if (wordIn.size() != WORD_SIZE) {
// 		fprintf(stderr, "Error in function Ram::set, invalid length of word");
// 		exit(6);
// 	}
// 	// Only look at last 4 bits of passed address.   
// 	vector<bool> adrShaved(adr.end() - ADDR_SIZE, adr.end());
// 	int address = Util::getInt(adrShaved);
// 	if (address < RAM_SIZE) {
// 		saveWord(addrSpace, address, wordIn);
// 	} else {
// 		assignToLastAddress(addrSpace, wordIn);
// 	}
// }

void Ram::set(Address adr, vector<bool> wordIn) {
  AddrSpace space = adr.space;
  vector<bool> value = adr.val;
  if (wordIn.size() != WORD_SIZE) {
		fprintf(stderr, "Error in function Ram::set, invalid length of word");
		exit(6);
	}
	// Only look at last 4 bits of passed address.   
	vector<bool> adrShaved(value.end() - ADDR_SIZE, value.end());
	int adrIndex = Util::getInt(adrShaved);
	if (adrIndex < RAM_SIZE) {
		saveWord(space, adrIndex, wordIn);
	} else {
		assignToLastAddress(space, wordIn);
	}
}

void Ram::saveWord(AddrSpace addrSpace, int address, vector<bool> wordIn) {
	state[addrSpace][address] = wordIn;
}

void Ram::assignToLastAddress(AddrSpace addrSpace, vector<bool> wordIn) {
	if (addrSpace == CODE) {
		fprintf(stderr, "Error in function Ram::setInstruction, "
				"Trying to write to last address of code address space.");
		exit(5);
	} else {
		printer.print(wordIn);
	}
}

//// GET STRING ////

string Ram::getString() const {
	string out;
	out += "# Code:\n";
	out += getString(CODE);
	out += "\n# Data:\n";
	out += getString(DATA);
	return out;
}

string Ram::getString(AddrSpace addrSpace) const {
	string out;
	for (vector<bool> word : state.at(addrSpace)) {
		out += Util::getString(word) + '\n';
	}
	return out;
}
