#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <map>
#include <set>
#include <vector>

#include "addr_space.hpp"
#include "address.hpp"
#include "const.hpp"
#include "instruction.hpp"
#include "ram.hpp"

using namespace std;

class Cpu {
  public:
    Cpu(Ram &ramIn) : ram(ramIn) { }
    
    bool step();
    void reset();
    Instruction getInstruction() const;
    vector<bool> getRegister() const;
    vector<bool> getPc() const;
    int getCycle() const;
    void switchOn();

  private:
    Ram &ram;
    vector<bool> reg = vector<bool>(WORD_SIZE);
    vector<bool> pc = vector<bool>(ADDR_SIZE);
    int cycle = 0;
};

#endif
