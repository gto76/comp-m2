#ifndef RANDOM_INPUT_H
#define RANDOM_INPUT_H

#include "provides_output.hpp"

using namespace std;

class RandomInput : public virtual ProvidesOutput {
  public:
    virtual string getOutput() const;
};

#endif