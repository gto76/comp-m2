#include "random_input.hpp"

using namespace std;

vector<bool> RandomInput::getOutput() {
  return Util::getRandomWord();  
}