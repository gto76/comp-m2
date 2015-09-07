#ifndef CONST_H
#define CONST_H

#include <string>
#include <vector>

using namespace std;

const int WORD_SIZE = 8;
const int ADDR_SIZE = 4;
const int RAM_SIZE = 15;

const vector<bool> EMPTY_WORD = { false, false, false, false, false, false,
                                  false, false };
const vector<bool> FIRST_ADDRESS = { false, false, false, false };
const vector<bool> LAST_ADDRESS = { true, true, true, true };
const int MAX_VALUE = 255;

const string LOGIC_OPS_INDICATOR = "JRI~<>&VX";
const int LOGIC_OPS_INDEX = 7;

const string  INC_DEC_OPS_INDICATOR = "INC/DEC";
const int INC_DEC_OPS_INDEX = 10;

const string LAST_CODE_ADDR_LABEL = "STOP";
const string LAST_DATA_ADDR_LABEL = "IN/OUT";

// Miliseconds between cycles (when run in interactive mode).
const int FQ = 333;

const string SAVE_FILE_NAME = "saved-ram-";

const string BOLD_ESC = "\033[1m";
const string BOLD_END_ESC = "\033[0;37m";

#endif
