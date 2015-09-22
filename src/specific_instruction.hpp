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
                               const Ram *ram) = 0;
    virtual string getLabel() = 0;
    virtual int getAdrIndex() = 0;
    virtual string getCode(const Address &adr, int pc) = 0;
};

class Read : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram *ram);
    string getLabel();
    int getAdrIndex();
    string getCode(const Address &adr, int pc);
};

class Write : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram *ram);
    string getLabel();
    int getAdrIndex();
    string getCode(const Address &adr, int pc);
};

class Add : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram *ram);
    string getLabel();
    int getAdrIndex();
    string getCode(const Address &adr, int pc);
};

class Sub : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram *ram);
    string getLabel();
    int getAdrIndex();
    string getCode(const Address &adr, int pc);
};

class Jump : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram *ram);
    string getLabel();
    int getAdrIndex();
    string getCode(const Address &adr, int pc);
};

class IfMax : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram *ram);
    string getLabel();
    int getAdrIndex();
    string getCode(const Address &adr, int pc);
};

class IfMin : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram *ram);
    string getLabel();
    int getAdrIndex();
    string getCode(const Address &adr, int pc);
};

class JumpReg : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram *ram);
    string getLabel();
    int getAdrIndex();
    string getCode(const Address &adr, int pc);
};

class ReadReg : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram *ram);
    string getLabel();
    int getAdrIndex();
    string getCode(const Address &adr, int pc);
};

class InitializeFirstAddress : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram *ram);
    string getLabel();
    int getAdrIndex();
    string getCode(const Address &adr, int pc);
};

class Not : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram *ram);
    string getLabel();
    int getAdrIndex();
    string getCode(const Address &adr, int pc);
};

class ShiftLeft : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram *ram);
    string getLabel();
    int getAdrIndex();
    string getCode(const Address &adr, int pc);
};

class ShiftRight : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram *ram);
    string getLabel();
    int getAdrIndex();
    string getCode(const Address &adr, int pc);
};

class And : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram *ram);
    string getLabel();
    int getAdrIndex();
    string getCode(const Address &adr, int pc);
};

class Or : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram *ram);
    string getLabel();
    int getAdrIndex();
    string getCode(const Address &adr, int pc);
};

class Xor : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram *ram);
    string getLabel();
    int getAdrIndex();
    string getCode(const Address &adr, int pc);
};

class ReadPointer : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram *ram);
    string getLabel();
    int getAdrIndex();
    string getCode(const Address &adr, int pc);
};

class WritePointer : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram *ram);
    string getLabel();
    int getAdrIndex();
    string getCode(const Address &adr, int pc);
};

class Increase : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram *ram);
    string getLabel();
    int getAdrIndex();
    string getCode(const Address &adr, int pc);
};

class Decrease : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram *ram);
    string getLabel();
    int getAdrIndex();
    string getCode(const Address &adr, int pc);
};

class Print : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram *ram);
    string getLabel();
    int getAdrIndex();
    string getCode(const Address &adr, int pc);
};

class IfNotMax : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram *ram);
    string getLabel();
    int getAdrIndex();
    string getCode(const Address &adr, int pc);
};

class IfNotMin : public SpecificInstruction {
  public:
    void exec(const Address &adr, vector<bool> &pc, vector<bool> &reg, Ram &ram);
    vector<Address> getFirstOrderAdr(vector<bool> &val);
    Address getAddress(Address &firstOrderAdr, const vector<bool> &reg, 
                       const Ram *ram);
    string getLabel();
    int getAdrIndex();
    string getCode(const Address &adr, int pc);
};

#endif
