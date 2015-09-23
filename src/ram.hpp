#ifndef RAM_H
#define RAM_H

#include <vector>
#include <map>

#include "addr_space.hpp"
#include "const.hpp"

using namespace std;

class Address;
class ProvidesOutput;

class Ram {
  public:
    // Initializes the state, one per address space.
    Ram() {
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

  private:
    vector<bool> getLastAddress(AddrSpace addrSpace) const;
    // vector<bool> getInput() const;
    void saveWord(AddrSpace addrSpace, int address, const vector<bool> &wordIn);
    void assignToLastAddress(AddrSpace addrSpace, const vector<bool> &wordIn);
    static string spaceToString(vector<vector<bool>> space);
};

#endif