#ifndef SPECIFIC_INSTRUCTION_H
#define SPECIFIC_INSTRUCTION_H

#include <vector>

#include "address.hpp"
#include "ram.hpp"

using namespace std;

class SpecificInstruction {
  public:
    virtual void exec(const Address &adr, vector<bool> &pc, 
                      vector<bool> &reg, Ram &ram) = 0;
    virtual vector<Address> getFirstOrderAdr(vector<bool> &val) = 0;
    virtual Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                               const Ram &ram) = 0;
    virtual string getLabel() = 0;
    virtual string getCode(const vector<bool> &val) = 0;
};

class Read : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
    string getCode(const vector<bool> &val);
};

class Write : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
    string getCode(const vector<bool> &val);
};

class Add : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
    string getCode(const vector<bool> &val);
};

class Sub : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
    string getCode(const vector<bool> &val);
};

class Jump : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
    string getCode(const vector<bool> &val);
};

class IfMax : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
    string getCode(const vector<bool> &val);
};

class IfMin : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
    string getCode(const vector<bool> &val);
};

class JumpReg : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
    string getCode(const vector<bool> &val);
};

class ReadReg : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
    string getCode(const vector<bool> &val);
};

class InitializeFirstAddress : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
    string getCode(const vector<bool> &val);
};

class Not : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
    string getCode(const vector<bool> &val);
};

class ShiftLeft : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
    string getCode(const vector<bool> &val);
};

class ShiftRight : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
    string getCode(const vector<bool> &val);
};

class And : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
    string getCode(const vector<bool> &val);
};

class Or : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
    string getCode(const vector<bool> &val);
};

class Xor : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
    string getCode(const vector<bool> &val);
};

class ReadPointer : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
    string getCode(const vector<bool> &val);
};

class WritePointer : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
    string getCode(const vector<bool> &val);
};

class Increase : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
    string getCode(const vector<bool> &val);
};

class Decrease : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
    string getCode(const vector<bool> &val);
};

class Print : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
    string getCode(const vector<bool> &val);
};

class IfNotMax : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
    string getCode(const vector<bool> &val);
};

class IfNotMin : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram &ram);
    string getLabel();
    string getCode(const vector<bool> &val);
};

#endif
