#ifndef RAM_H
#define RAM_H

#include <vector>
#include <map>

#include "addr_space.hpp"
#include "address.hpp"
#include "const.hpp"
#include "printer.hpp"

using namespace std;

class Ram {
  public:
    Ram(Printer &printerIn, bool interactiveModeIn) 
        : printer(printerIn),
          interactiveMode(interactiveModeIn) {
      // Initializes the state, one per address space.
      state[CODE] = vector<vector<bool>>(RAM_SIZE, vector<bool>(WORD_SIZE));
      state[DATA] = vector<vector<bool>>(RAM_SIZE, vector<bool>(WORD_SIZE)); 
      names[CODE] = vector<char>(16, DEFAULT_VAR_NAME);
      names[DATA] = vector<char>(16, DEFAULT_VAR_NAME);
    }
    map<AddrSpace, vector<vector<bool>>> state;
    map<AddrSpace, vector<char>> names; 
    vector<bool> get(Address adr) const;
    void set(Address adr, vector<bool> wordIn);
    string getString() const;

  private:
    Printer &printer;
    const bool interactiveMode;
    vector<bool> getLastAddress(AddrSpace addrSpace) const;
    vector<bool> getInput() const;
    void saveWord(AddrSpace addrSpace, int address, vector<bool> wordIn);
    void assignToLastAddress(AddrSpace addrSpace, vector<bool> wordIn);
    string getString(AddrSpace addrSpace) const;
};

#endif