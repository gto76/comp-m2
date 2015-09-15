#include "pipe_input.hpp"

#include "util.hpp"

using namespace std;

vector<bool> PipeInput::getOutput() {
  return Util::readWordFromPipe();
}