#ifndef PIPE_INPUT_H
#define PIPE_INPUT_H

#include "provides_output.hpp"

using namespace std;

class PipeInput : public ProvidesOutput {
  public:
    vector<bool> getOutput();
};

#endif