#include "pipe_input.hpp"

using namespace std;

string PipeInput::getOutput() {
  return Util::readWordFromPipe();
}