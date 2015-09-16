#include "ram.hpp"

#include <stdlib.h>
#include <string>

#include "const.hpp"
#include "util.hpp"

using namespace std;

////// GET //////

vector<bool> Ram::get(Address adr) const {
  AddrSpace space = adr.space;
  vector<bool> value = adr.val;
  // Only looks at last 4 bits of passed address.   
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
    return input->getOutput();
  }
}

////// SET //////

void Ram::set(Address adr, vector<bool> wordIn) {
  AddrSpace space = adr.space;
  vector<bool> value = adr.val;
  if (wordIn.size() != WORD_SIZE) {
    fprintf(stderr, "Error in function Ram::set, invalid length of word");
    exit(6);
  }
  // Only looks at last 4 bits of passed address.   
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
    output = wordIn;
    outputPending = true;
  }
}

//// GET STRING ////

string Ram::getString() const {
  return stateToString(state);
}
//   string out;
//   out += "# Code:\n";
//   out += getString(CODE);
//   out += "\n# Data:\n";
//   out += getString(DATA);
//   return out;
// }

// string Ram::getString(AddrSpace addrSpace) const {
//   string out;
//   for (vector<bool> word : state.at(addrSpace)) {
//     out += Util::getString(word) + '\n';
//   }
//   return out;
// }

string Ram::stateToString(map<AddrSpace, vector<vector<bool>>> state) {
  string out;
  out += "# Code:\n";
  out += spaceToString(state.at(CODE));
  out += "\n# Data:\n";
  out += spaceToString(state.at(DATA));
  return out;
}

string Ram::spaceToString(vector<vector<bool>> space) {
  string out;
  for (vector<bool> word : space) {
    out += Util::getString(word) + '\n';
  }
  return out;
}

