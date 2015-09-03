#ifndef SPECIFIC_INSTRUCTION_H
#define SPECIFIC_INSTRUCTION_H

#include <vector>

#include "address.hpp"
#include "ram.hpp"

using namespace std;

class SpecificInstruction {
  public:
    virtual void exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram) = 0;
    virtual Address getAddress(vector<bool> val, vector<bool>* reg, Ram* ram) = 0;
};

class Read : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram);
    Address getAddress(vector<bool> val, vector<bool>* reg, Ram* ram);
};

#endif
