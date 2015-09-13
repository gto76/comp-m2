#ifndef PROVIDES_OUTPUT_H
#define PROVIDES_OUTPUT_H

#include <string>

using namespace std;

class ProvidesOutput {
  virtual ~ProvidesOutput() {}
  virtual string getOutput() const = 0;
};

#endif