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

class Write : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram);
    Address getAddress(vector<bool> val, vector<bool>* reg, Ram* ram);
};

class Add : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram);
    Address getAddress(vector<bool> val, vector<bool>* reg, Ram* ram);
};

class Sub : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram);
    Address getAddress(vector<bool> val, vector<bool>* reg, Ram* ram);
};

class Jump : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram);
    Address getAddress(vector<bool> val, vector<bool>* reg, Ram* ram);
};

class IfMax : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram);
    Address getAddress(vector<bool> val, vector<bool>* reg, Ram* ram);
};

class IfMin : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram);
    Address getAddress(vector<bool> val, vector<bool>* reg, Ram* ram);
};

class JumpReg : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram);
    Address getAddress(vector<bool> val, vector<bool>* reg, Ram* ram);
};

class ReadReg : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram);
    Address getAddress(vector<bool> val, vector<bool>* reg, Ram* ram);
};

class InitializeFirstAddress : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram);
    Address getAddress(vector<bool> val, vector<bool>* reg, Ram* ram);
};

class Not : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram);
    Address getAddress(vector<bool> val, vector<bool>* reg, Ram* ram);
};

class ShiftLeft : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram);
    Address getAddress(vector<bool> val, vector<bool>* reg, Ram* ram);
};

class ShiftRight : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram);
    Address getAddress(vector<bool> val, vector<bool>* reg, Ram* ram);
};

class And : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram);
    Address getAddress(vector<bool> val, vector<bool>* reg, Ram* ram);
};

class Or : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram);
    Address getAddress(vector<bool> val, vector<bool>* reg, Ram* ram);
};

class Xor : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram);
    Address getAddress(vector<bool> val, vector<bool>* reg, Ram* ram);
};

class ReadPointer : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram);
    Address getAddress(vector<bool> val, vector<bool>* reg, Ram* ram);
};

class WritePointer : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram);
    Address getAddress(vector<bool> val, vector<bool>* reg, Ram* ram);
};

class Increase : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram);
    Address getAddress(vector<bool> val, vector<bool>* reg, Ram* ram);
};

class Decrease : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram);
    Address getAddress(vector<bool> val, vector<bool>* reg, Ram* ram);
};

class Print : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram);
    Address getAddress(vector<bool> val, vector<bool>* reg, Ram* ram);
};

class IfNotMax : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram);
    Address getAddress(vector<bool> val, vector<bool>* reg, Ram* ram);
};

class IfNotMin : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool>* pc, vector<bool>* reg, Ram* ram);
    Address getAddress(vector<bool> val, vector<bool>* reg, Ram* ram);
};

#endif
