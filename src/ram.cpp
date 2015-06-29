#include <string>

#include "ram.hpp"
#include "comp.hpp"
#include "util.hpp"

using namespace std;

vector<bool> Ram::get(vector<bool> adr) {
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
	vector<bool> wordOut(WORD_SIZE);
	for (int i = 0; i < WORD_SIZE; i++) {
		wordOut[i] = state[address][i];
	}
	return wordOut;
}

void Ram::set(vector<bool> adr, vector<bool> wordIn) {
	int address = Util::getInt(adr);
	// Save word
	if (address < RAM_SIZE) {
		for (int i = 0; i < WORD_SIZE; i++) {
			state[address][i] = wordIn[i];
		}
	// Send word to printer
	} else {
		printer.print(wordIn);
	}
}

string Ram::getString() {
	string out;
	for (vector<bool> word : state) {
		out += Util::getString(word) + '\n';
	}
	return out;
}
