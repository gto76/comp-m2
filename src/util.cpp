#include "util.hpp"

#include <dirent.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include "const.hpp"

using namespace std;

int Util::getInt(vector<bool> bbb) {
  int sum = 0;
  int power = 1;
  for (int i = bbb.size()-1; i >= 0; i--, power *= 2) {
    if (bbb[i] == true) {
      sum += power;
    }
  }
  return sum;
}

vector<bool> Util::getBoolByte(int num) {
  return getBool(num, 8);
}

vector<bool> Util::getBoolNibb(int num) {
  return getBool(num, 4);
}

/*
 * Returns binary representation of integer 'num', of length 'length'.
 * If number is too large to fit, then maximum binary number of specified
 * length is returned.
 */
vector<bool> Util::getBool(int num, int length) {
  if (num <= 0) {
    return vector<bool>(length, false);
  }
  if (num >= (int) pow(2, length)-1) {
    return vector<bool>(length, true);
  }
  vector<bool> out(length);
  int j = 0;
  for (int i = length-1; i >= 0; i--) {
    int divider = (int) pow(2, i);
    int res = num / divider;
    if (res > 0) {
      out[j++] = true;
    } else {
      out[j++] = false;
    }
    num = num % divider;
  }
  return out;
}

// New getBool - it doesen't saturate.
// vector<bool> Util::getBool(int num, int length) {
//   vector<bool> out(length);
//   for (c = length; c >= 0; c--) {
//     int k = num >> c;
//     out.push_back(k & 1);
//   }
// }

vector<bool> Util::getBoolByte(string sIn) {
  vector<bool> out(8);
  int i = 0;
  for (char c : sIn) {
    // '*' is interpreted as true, all other characters as false.
    out.at(i++) = c == '*';
    if (i == WORD_SIZE) {
      break;
    }
  }
  return out;
}

vector<bool> Util::getFirstNibble(vector<bool> bbb) {
  return {bbb[0], bbb[1], bbb[2], bbb[3]};
}

vector<bool> Util::getSecondNibble(vector<bool> bbb) {
  return {bbb[4], bbb[5], bbb[6], bbb[7]};
}

string Util::getString(vector<bool> bbb) {
  string out = "";
  for (bool b : bbb) {
    out += Util::getChar(b);
  }
  return out;
}

string Util::getChar(bool b) {
  if (b) {
    return "*";
  } else {
    return "-";
  }
}

string Util::getFormatedInt(vector<bool> wordIn) {
  char formatedInt [4];
  sprintf(formatedInt, "%3d", Util::getInt(wordIn));
  return formatedInt;
}

string Util::getStringWithFormatedInt(vector<bool> wordIn) {
  return Util::getString(wordIn) + " " + Util::getFormatedInt(wordIn) + "\n";
}

vector<bool> Util::parseWord(string word) {
  if (Util::startsWithDigit(word)) {
    int num = Util::extractInteger(word);
    return Util::getBoolByte(num);
  } else {
    return Util::getBoolByte(word);
  }
}

vector<bool> Util::readWordFromPipe() {
  string word;
  // Read until next whitespace.
  cin >> word;
  // Exit when end of pipe input is reached.
  if (!cin) {
    exit(0);
  }
  return Util::parseWord(word);
}

/*
 * GENERAL UTIL
 */

vector<vector<string>> Util::splitIntoLines(vector<string> drawing) {
  vector<vector<string>> out;
  vector<string> line;
  for (string c : drawing) {
    if (c == u8"\u000A" /*NEW_LINE_SECOND*/) {
      continue;
    }
    if (c == u8"\u000D" /*NEW_LINE_FIRST*/) {
      out.push_back(line);
      line = {};
      continue;
    }
    line.push_back(c);
  }
  return out;
} 

vector<string> Util::splitString(string stringIn) {
  vector<string> out;
  stringstream stream (stringIn);
  string line;
  while (getline(stream, line)) {
    out.push_back(line);
  }
  return out;
}

string Util::makeString(vector<string> lines) {
  string out;
  for (string line : lines) {
    out += line;
  }
  return out;
}

int Util::hexToInt(char cIn) {
  unsigned int out;
  stringstream ss;
  ss << cIn;
  ss >> std::hex >> out;
  return out;
}

string Util::getString(char cIn) {
  stringstream ss;
  string s;
  ss << cIn;
  ss >> s;
  return s;
}

vector<vector<bool>> Util::getRamFromString(string ramString) {
  vector<vector<bool>> data = vector<vector<bool>>(
      RAM_SIZE, 
      vector<bool>(WORD_SIZE));
  int i = 0;
  for (string line : splitString(ramString)) {
    data[i++] = getBoolByte(line);
  }
  return data;
}

vector<bool> Util::getRandomWord() {
  vector<bool> wordOut(WORD_SIZE);
  for (int i = 0; i < WORD_SIZE; i++) {
    wordOut[i] = 0 == (rand() % 2);
  }
  return wordOut;
}

tuple<int, int> Util::getCoordinatesOfFirstOccurance(vector<string> text,
                                                     string cIn) {
  int i = 0;
  int j = 0;
  for (string c : text) {
    if (c == NEW_LINE_SECOND) {
      continue;
    }
    if (c == cIn) {
      return tuple<int, int>(i, j);
    }
    if (c == NEW_LINE_FIRST) {
      i = 0;
      j++;
    } else {
      i++;
    }
  }
  printf("Could not find ram in drawing. Aborting");
  exit(1);
}

/*
 * Returns the code of last pressed key, or 0 if none was pressed.
 */
int Util::getKey() {
  int c = 0;
  struct timeval tv = { 0L, 0L };
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds);
  select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
  if (FD_ISSET(STDIN_FILENO, &fds)) {
    c = getchar();
  }
  return c;
}

bool Util::fileExists(string filename) {
    struct stat buf;
    if (stat(filename.c_str(), &buf) != -1) {
        return true;
    }
    return false;
}

bool Util::inputIsPiped() {
  return !isatty(fileno(stdin));
}

bool Util::outputIsPiped() {
  return !isatty(fileno(stdout));
}

bool Util::startsWithDigit(string line) {
  if (line.empty()) {
    return false;
  }
  return isdigit(line[0]);
}

int Util::extractInteger(string line) {
  int i = 0;
  for (char c : line) {
    if (!isdigit(c)) {
      break;
    }
    i++;
  }
  string stringNumber = line.substr(0, i);
  return stoi(stringNumber, NULL);
}

int Util::getSignedIntFromNibble(vector<bool> nibble) {
  bool sign = nibble.at(0);
  vector<bool> number = { nibble.at(1), nibble.at(2), nibble.at(3) };
  int value = Util::getInt(number);
  if (sign) {
    value *= -1;
  }
  return value;
}

void Util::assertEqualLength(vector<bool> value1, vector<bool> value2,
                             int errNumber) {
  if (value1.size() != value2.size()) {
    fprintf(stderr, "Invalid parameters at Util::assertEqualLength. "
            "Vectors not of same size.");
    exit(errNumber);
  }
}

vector<bool> Util::bitwiseAndOrOr(vector<bool> value1, vector<bool> value2,
                                  bool isAnd) {
  assertEqualLength(value1, value2, 6);
  vector<bool> out(value1.size());
  for (size_t i = 0; i < value1.size(); i++) {
    if (isAnd) {
      out.at(i) = value1.at(i) && value2.at(i);
    } else {
      out.at(i) = value1.at(i) || value2.at(i);
    }
  }
  return out;
}

vector<bool> Util::bitwiseXor(vector<bool> value1, vector<bool> value2) {
  assertEqualLength(value1, value2, 7);
  vector<bool> out(value1.size());
  for (size_t i = 0; i < value1.size(); i++) {
    out.at(i) = value1.at(i) != value2.at(i);
  }
  return out;
}

vector<bool> Util::bitwiseNot(vector<bool> val) {
  for (size_t i = 0; i < val.size(); i++) {
    val.at(i) = !val.at(i);
  }
  return val;
}

vector<string> Util::stringToVecOfString(string stringIn) {
  vector<string> out;
  for (char c : stringIn) {
    out.push_back(string(1, c));
  }
  return out;
}

size_t Util::getSizeOfLargestElement(vector<vector<string>> lines) {
  size_t maxSize = 0;
  for (vector<string> line : lines) {
    maxSize = std::max(maxSize, line.size());
  }
  return maxSize;
}

/* 
 * Returns a list of files in a directory (except the ones that begin with a
 * dot)
 */
vector<string> Util::getFilesInDirectory(const string &directory) {
  DIR *dir;
  struct dirent *ent;
  struct stat st;
  vector<string> out;
  dir = opendir(directory.c_str());
  while ((ent = readdir(dir)) != NULL) {
    const string file_name = ent->d_name;
    const string full_file_name = directory + "/" + file_name;
    if (file_name[0] == '.') {
      continue;
    }
    if (stat(full_file_name.c_str(), &st) == -1) {
      continue;
    }
    const bool is_directory = (st.st_mode & S_IFDIR) != 0;
    if (is_directory) {
      continue;
    }
    out.push_back(full_file_name);
  }
  closedir(dir);
  sort(out.begin(), out.end());
  return out;
}

bool Util::endsWith(string const &fullString, string const &ending) {
  if (fullString.length() >= ending.length()) {
    int lengthDelta = fullString.length() - ending.length();
    return fullString.compare(lengthDelta, ending.length(), ending) == 0;
  } else {
    return false;
  }
}

bool Util::isADir(string filename) {
  struct stat s;
  if (stat(filename.c_str(), &s) == 0) {
    return s.st_mode & S_IFDIR;
  } else {
    cout << "Could not open file " << filename << "." << endl;
    exit(1);
  }
}

bool Util::contains(vector<string> options, const char* arg) {
  for (string option : options) {
    if (strcmp(arg, option.c_str()) == 0) {
      return true;
    }
  }
  return false;
}




