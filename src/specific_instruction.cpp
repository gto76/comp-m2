#include "specific_instruction.hpp"

#include <vector>

#include "address.hpp"
#include "ram.hpp"
#include "util.hpp"

using namespace std;

void increasePc(vector<bool>* pc);

void Read::exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram) {
  *reg = ram->get(adr.space, adr.val);
  increasePc(pc);
}

Address Read::getAddress(vector<bool> val, vector<bool>* reg, Ram* ram) {
  return Address(DATA, Util::getSecondNibble(val));
}

void increasePc(vector<bool>* pc) {
  *pc = Util::getBoolNibb(Util::getInt(*pc) + 1);
}






