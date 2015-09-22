#include "parser.hpp"

#include <string>
#include <vector>

#include "const.hpp"
#include "instruction.hpp"
#include "load.hpp"
#include "ram.hpp"

using namespace std;

string Parser::parse(vector<string> filenamesIn, bool outputChars) {
  vector<Ram> rams = vector<Ram>(filenamesIn.size());
  // Fills rams with contents of files.
  for (size_t i = 0; i < filenamesIn.size(); i++) {
    Load::fillRamWithFile(filenamesIn[i].c_str(), rams[i]);
  }
  string source;
  source = SOURCE_HEADER+ "\n\n";
  for (size_t i = 0; i < filenamesIn.size(); i++) {
    source += getComputerFunction(rams[i], i)+ "\n\n";
  }
  source += SOURCE_FOOTER_1 + to_string(filenamesIn.size()) + 
            SOURCE_FOOTER_2 + "\n";
  return source;
}

string Parser::getComputerFunction(Ram ram, int index) {
  string function;
  function += getFunctionHeader(ram.state[DATA], index)+ "\n";
  for (size_t i = 0; i < RAM_SIZE; i++) {
    function += getLineOfCode(ram.state[CODE][i], i, ram) + "\n";
  }
  function += SOURCE_FUNCTION_FOOTER;
  return function;
}

string Parser::getFunctionHeader(vector<vector<bool>> data, int index) {
  return SOURCE_FUNCTION_HEADER_1 + to_string(index+1) + 
         SOURCE_FUNCTION_HEADER_2 + to_string(index) +
         SOURCE_FUNCTION_HEADER_3 + getData(data) +
         SOURCE_FUNCTION_HEADER_4;
}

string Parser::getData(vector<vector<bool>> data) {
  string out;
  bool first = true;
  for (vector<bool> word : data) {
    if (first) {
      first = false;
      out += to_string(Util::getInt(word));
    } else {
      out += ", " + to_string(Util::getInt(word));
    }
  }
  return out;
}

string Parser::getLineOfCode(vector<bool> word, int index, Ram ram) {
  Instruction inst = Instruction(word, EMPTY_WORD, &ram);
  string strIndex;
  if (index < 10) {
    strIndex = "0";
  }
  strIndex += to_string(index);
  string instCode = inst.getCode(index+1);
  return "  a" + strIndex + ": " + instCode;
}