
#ifndef ADDRESS_H
#define ADDRESS_H

#include <string>
#include <vector>

#include "addr_space.hpp"

using namespace std;

class Address {
  public:
    Address(const AddrSpace spaceIn, vector<bool> valIn)
        : space(spaceIn), 
          val(valIn) { }
    bool operator == (const Address& other) const {
      return space == other.space && val == other.val;
    }
    string getIntStr() const;
    AddrSpace const space;
    vector<bool> const val;
};

#endif