#include "address.hpp"

#include <string>

#include "util.hpp"

string Address::getIntStr() const {
  return to_string(Util::getInt(val));
}