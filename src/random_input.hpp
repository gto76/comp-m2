#ifndef RANDOM_INPUT_H
#define RANDOM_INPUT_H

#include <vector>

#include "provides_output.hpp"

using namespace std;

class RandomInput : public ProvidesOutput {
  public:
    vector<bool> getOutput();
};

#endif