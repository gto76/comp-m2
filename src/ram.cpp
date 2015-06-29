#include <string>

#include "ram.hpp"
#include "comp.hpp"
#include "util.hpp"

using namespace std;

vector<bool> Ram::getInstruction(vector<bool> adr) {
	int address = Util::getInt(adr);
	if (address == RAM_SIZE) {
		fprintf(stderr, "Error in function Ram::getInstruction, invalid address");
		exit(4);
	}
	return instructions[address];
}

vector<bool> Ram::getData(vector<bool> adr) {
	int address = Util::getInt(adr);
	// Return random if last address (reserved for output),
	// or read from pipe if input is piped in.
	if (address == RAM_SIZE) {
		if (interactivieMode) {
			return Util::getRandomWord();
		} else {
			return Util::readLineFromPipe();
		}
	}
	return data[address];
}

void Ram::saveWord(int address, vector<bool> wordIn, bool toInstructions) {
	for (int i = 0; i < WORD_SIZE; i++) {
		if (toInstructions) {
			instructions[address][i] = wordIn[i];
		} else {
			data[address][i] = wordIn[i];
		}
	}
}

void Ram::setInstruction(vector<bool> adr, vector<bool> wordIn) {
	int address = Util::getInt(adr);
	if (address < RAM_SIZE) {
		saveWord(address, wordIn, true);
	} else {
		fprintf(stderr, "Error in function Ram::setInstruction, invalid address");
		exit(5);
	}
}

void Ram::setData(vector<bool> adr, vector<bool> wordIn) {
	int address = Util::getInt(adr);
	if (address < RAM_SIZE) {
		saveWord(address, wordIn, false);
	} else {
		printer.print(wordIn);
	}
}

string Ram::getString() {
	string out;
	out += "# Instructions:";
	for (vector<bool> word : instructions) {
		out += Util::getString(word) + '\n';
	}
	out += "\n# Data:";
	for (vector<bool> word : data) {
		out += Util::getString(word) + '\n';
	}
	return out;
}
