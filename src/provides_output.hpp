#ifndef PROVIDES_OUTPUT_H
#define PROVIDES_OUTPUT_H

#include <string>
#include <vector>

using namespace std;

class ProvidesOutput {
  public:
    virtual ~ProvidesOutput() {}
    virtual vector<bool> getOutput() = 0;  // const = 0;
};

#endif