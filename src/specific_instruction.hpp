#ifndef SPECIFIC_INSTRUCTION_H
#define SPECIFIC_INSTRUCTION_H

#include <vector>

#include "address.hpp"
#include "ram.hpp"

using namespace std;

class SpecificInstruction {
  public:
    virtual void exec(Address adr, vector<bool> &pc, 
                      vector<bool> &reg, Ram &ram) = 0;
    virtual vector<Address> getFirstOrderAdr(vector<bool> val) = 0;
    virtual Address getAddress(Address firstOrderAdr, const vector<bool> &reg, 
                               const Ram &ram) = 0;
    virtual string getLabel() = 0;
};

class Read : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> val);
    Address getAddress(Address firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
};

class Write : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> val);
    Address getAddress(Address firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
};

class Add : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> val);
    Address getAddress(Address firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
};

class Sub : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> val);
    Address getAddress(Address firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
};

class Jump : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> val);
    Address getAddress(Address firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
};

class IfMax : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> val);
    Address getAddress(Address firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
};

class IfMin : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> val);
    Address getAddress(Address firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
};

class JumpReg : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> val);
    Address getAddress(Address firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
};

class ReadReg : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> val);
    Address getAddress(Address firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
};

class InitializeFirstAddress : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> val);
    Address getAddress(Address firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
};

class Not : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> val);
    Address getAddress(Address firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
};

class ShiftLeft : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> val);
    Address getAddress(Address firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
};

class ShiftRight : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> val);
    Address getAddress(Address firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
};

class And : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> val);
    Address getAddress(Address firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
};

class Or : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> val);
    Address getAddress(Address firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
};

class Xor : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> val);
    Address getAddress(Address firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
};

class ReadPointer : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> val);
    Address getAddress(Address firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
};

class WritePointer : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> val);
    Address getAddress(Address firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
};

class Increase : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> val);
    Address getAddress(Address firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
};

class Decrease : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> val);
    Address getAddress(Address firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
};

class Print : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> val);
    Address getAddress(Address firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
};

class IfNotMax : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> val);
    Address getAddress(Address firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
};

class IfNotMin : public SpecificInstruction {
  public:
    void exec(Address adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> val);
    Address getAddress(Address firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
};

#endif
