#ifndef CONST_H
#define CONST_H

#include <string>
#include <vector>

using namespace std;

const int WORD_SIZE = 8;
const int ADDR_SIZE = 4;
const int RAM_SIZE = 15;

const vector<bool> EMPTY_WORD = { false, false, false, false, false, false, false, false };
const vector<bool> FIRST_ADDRESS = { false, false, false, false };
const vector<bool> LAST_ADDRESS = { true, true, true, true };
const int MAX_VALUE = 255;


// Miliseconds between cycles (when run in interactive mode).
const int FQ = 333;

const string SAVE_FILE_NAME = "saved-ram-";

#endif
