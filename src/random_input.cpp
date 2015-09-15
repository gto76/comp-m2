#include "random_input.hpp"

#include "util.hpp"

using namespace std;

vector<bool> RandomInput::getOutput() {
  return Util::getRandomWord();  
}