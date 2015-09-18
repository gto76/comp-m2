#ifndef RAM_H
#define RAM_H

#include <vector>
#include <map>

#include "addr_space.hpp"
#include "address.hpp"
#include "const.hpp"
#include "provides_output.hpp"

using namespace std;

class Ram {
  public:
    Ram() {
      // Initializes the state, one per address space.
      state[CODE] = vector<vector<bool>>(RAM_SIZE, vector<bool>(WORD_SIZE));
      state[DATA] = vector<vector<bool>>(RAM_SIZE, vector<bool>(WORD_SIZE)); 
    }

    map<AddrSpace, vector<vector<bool>>> state;
    vector<bool> output;
    bool outputPending = false;
    ProvidesOutput *input = NULL;

    vector<bool> get(const Address &adr) const;
    void set(const Address &adr, const vector<bool> &wordIn);
    string getString() const;
    static string stateToString(map<AddrSpace, vector<vector<bool>>> state);
    static string spaceToString(vector<vector<bool>> space);

  private:
    vector<bool> getLastAddress(AddrSpace addrSpace) const;
    vector<bool> getInput() const;
    void saveWord(AddrSpace addrSpace, int address, const vector<bool> &wordIn);
    void assignToLastAddress(AddrSpace addrSpace, const vector<bool> &wordIn);
};

#endif