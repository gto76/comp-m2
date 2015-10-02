#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

using namespace std;

class Ram;

class Parser {
  public:
    static string parse(vector<string> filenamesIn, bool outputChars, 
                        bool inputChars, bool rawInput);
  private:
    static string getComputerFunction(Ram ram, int index);
    static string getFunctionHeader(vector<vector<bool>> data, int index);
    static string getData(vector<vector<bool>> data);
    static string getLineOfCode(vector<bool> word, int index, Ram ram);
};

#endif
