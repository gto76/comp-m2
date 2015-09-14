#ifndef PIPE_INPUT_H
#define PIPE_INPUT_H

#include "provides_output.hpp"

using namespace std;

class PipeInput : public virtual ProvidesOutput {
  public:
    virtual string getOutput() const;
};

#endif