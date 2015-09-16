#ifndef CONST_H
#define CONST_H

#include <string>
#include <vector>
#include <set>

using namespace std;

// Miliseconds between cycles (when run in interactive mode).
const int FQ = 333;

const string SAVE_FILE_NAME = "saved-ram-";
const string FILE_EXTENSION = "cm2";

const string LIGHTBULB_ON_3D = u8"\u2737";
const string LIGHTBULB_OFF_3D = u8"\u00b7"; 
const string LIGHTBULB_ON_3D_B = u8"\u2666";
const string LIGHTBULB_OFF_3D_B = u8"\u00b7"; 
const string LIGHTBULB_ON_2D = "*";
const string LIGHTBULB_OFF_2D = "-";

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

const string INC_DEC_OPS_INDICATOR = "INC/DEC";
const int INC_DEC_OPS_INDEX = 10;

const string LAST_CODE_ADDR_LABEL = "  STOP  ";
const string LAST_DATA_ADDR_LABEL = " IN/OUT ";

const char DEFAULT_VAR_NAME = '-';

const string HIGHLIGHT_ESC = "\e[7m";
const string HIGHLIGHT_END_ESC = "\e[27m";

const string CODE_INDICATOR = "a";
const string DATA_INDICATOR = "b";
const string CODE_ADR_INDICATOR = "g";
const string DATA_ADR_INDICATOR = "v";
const string REGISTER_INDICATOR = "r";
const string OUTPUT_INDICATOR = "o";
const set<string> ALL_INDICATORS = { CODE_INDICATOR, DATA_INDICATOR, CODE_ADR_INDICATOR,
                             DATA_ADR_INDICATOR, REGISTER_INDICATOR, 
                             OUTPUT_INDICATOR };

const string NEW_LINE_FIRST = u8"\u000D";
const string NEW_LINE_SECOND = u8"\u000A";
const string ESCAPE = u8"\u001B";
const string LOWERCASE_M = u8"\u006D";

#endif
