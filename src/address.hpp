#ifndef ADDRESS_H
#define ADDRESS_H

#include <string>
#include <vector>

#include "addr_space.hpp"

using namespace std;

class Address {
  public:
    Address(AddrSpace spaceIn, vector<bool> valIn)
        : space(spaceIn), 
          val(valIn) { }
    bool operator == (const Address& a) const {
      return space == a.space && val == a.val;
    }
    string getIntStr() const;
    AddrSpace const space;
    vector<bool> const val;
};

#endif