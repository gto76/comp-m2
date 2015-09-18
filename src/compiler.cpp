#include "compiler.hpp"

#include <string>
#include <vector>

#include "const.hpp"
#include "instruction.hpp"
#include "load.hpp"
#include "ram.hpp"

using namespace std;

string Compiler::compile(vector<string> filenamesIn) {
  vector<Ram> rams = vector<Ram>(filenamesIn.size());
  // Fills rams with contents of files.
  for (size_t i = 0; i < filenamesIn.size(); i++) {
    Load::fillRamWithFile(filenamesIn[i].c_str(), rams[i]);
  }
  string source;
  source = SOURCE_HEADER+ "\n";
  for (size_t i = 0; i < filenamesIn.size(); i++) {
    source += getComputerFunction(rams[i], i)+ "\n";
  }
  source += SOURCE_FOOTER+ "\n";
  return source;
}

string Compiler::getComputerFunction(Ram ram, int index) {
  string function;
  function += getFunctionHeader(ram.state[DATA], index)+ "\n";
  for (size_t i = 0; i < RAM_SIZE; i++) {
    function += getLineOfCode(ram.state[CODE][i], i, ram) + "\n";
  }
  function += SOURCE_FUNCTION_FOOTER+ "\n";
  return function;
}

string Compiler::getFunctionHeader(vector<vector<bool>> data, int index) {
  string functionHeader = SOURCE_FUNCTION_HEADER+ "\n";
  // TODO
  return functionHeader;
}

string Compiler::getLineOfCode(vector<bool> word, int index, Ram ram) {
  Instruction inst = Instruction(word, EMPTY_WORD, ram); // TODO ram is probably not const?
  return inst.getCode();
}