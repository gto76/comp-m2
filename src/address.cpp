#include "address.hpp"

#include <string>

#include "util.hpp"

string Address::getIntStr() const {
  return to_string(Util::getInt(val));
}

string Address::toString(AddrSpace space) {
  if (space == NONE) {
    return "NONE";
  } else if (space == DATA) {
    return "DATA";
  } else {
    return "CODE";
  }
}
