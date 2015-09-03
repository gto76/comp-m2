#ifndef SPECIFIC_INSTRUCTION_H
#define SPECIFIC_INSTRUCTION_H

#include <vector>

#include "address.hpp"
#include "ram.hpp"

using namespace std;

class SpecificInstruction {
  public:
    virtual void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram) = 0;
    virtual Address getAddress(vector<bool> val, const vector<bool> &reg, const Ram &ram) = 0;
};

class Read : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    Address getAddress(vector<bool> val, const vector<bool> &reg, const Ram &ram);
};

class Write : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    Address getAddress(vector<bool> val, const vector<bool> &reg, const Ram &ram);
};

class Add : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    Address getAddress(vector<bool> val, const vector<bool> &reg, const Ram &ram);
};

class Sub : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    Address getAddress(vector<bool> val, const vector<bool> &reg, const Ram &ram);
};

class Jump : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    Address getAddress(vector<bool> val, const vector<bool> &reg, const Ram &ram);
};

class IfMax : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    Address getAddress(vector<bool> val, const vector<bool> &reg, const Ram &ram);
};

class IfMin : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    Address getAddress(vector<bool> val, const vector<bool> &reg, const Ram &ram);
};

class JumpReg : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    Address getAddress(vector<bool> val, const vector<bool> &reg, const Ram &ram);
};

class ReadReg : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    Address getAddress(vector<bool> val, const vector<bool> &reg, const Ram &ram);
};

class InitializeFirstAddress : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    Address getAddress(vector<bool> val, const vector<bool> &reg, const Ram &ram);
};

class Not : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    Address getAddress(vector<bool> val, const vector<bool> &reg, const Ram &ram);
};

class ShiftLeft : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    Address getAddress(vector<bool> val, const vector<bool> &reg, const Ram &ram);
};

class ShiftRight : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    Address getAddress(vector<bool> val, const vector<bool> &reg, const Ram &ram);
};

class And : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    Address getAddress(vector<bool> val, const vector<bool> &reg, const Ram &ram);
};

class Or : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    Address getAddress(vector<bool> val, const vector<bool> &reg, const Ram &ram);
};

class Xor : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    Address getAddress(vector<bool> val, const vector<bool> &reg, const Ram &ram);
};

class ReadPointer : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    Address getAddress(vector<bool> val, const vector<bool> &reg, const Ram &ram);
};

class WritePointer : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    Address getAddress(vector<bool> val, const vector<bool> &reg, const Ram &ram);
};

class Increase : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    Address getAddress(vector<bool> val, const vector<bool> &reg, const Ram &ram);
};

class Decrease : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    Address getAddress(vector<bool> val, const vector<bool> &reg, const Ram &ram);
};

class Print : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    Address getAddress(vector<bool> val, const vector<bool> &reg, const Ram &ram);
};

class IfNotMax : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    Address getAddress(vector<bool> val, const vector<bool> &reg, const Ram &ram);
};

class IfNotMin : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    Address getAddress(vector<bool> val, const vector<bool> &reg, const Ram &ram);
};

#endif
