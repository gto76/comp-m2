#ifndef COMPILER_H
#define COMPILER_H

#include <string>
#include <vector>

#include "ram.hpp"

using namespace std;

class Compiler {
  public:
    static string compile(vector<string> filenamesIn);
  private:
    static string getComputerFunction(Ram ram, int index);
    static string getFunctionHeader(vector<vector<bool>> data, int index);
    static string getData(vector<vector<bool>> data);
    static string getLineOfCode(vector<bool> word, int index, Ram ram);
};

#endif
