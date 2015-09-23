#ifndef LOAD_H
#define LOAD_H

#include <fstream>
#include <string>

// #include "ram.hpp"

class Ram;

using namespace std;

class Load {
  public:
    static void fillRamWithFile(const char* filename, Ram &ram);

  private:
    static void fillRamWithFilestream(ifstream* fileStream, Ram &ram);
    static void writeLineToRam(string line, int address, Ram &ram);
    static void writeInstructionBitToRam(int address, int bitIndex, bool bitValue, Ram &ram);
    static void writeDataBitToRam(int address, int bitIndex, bool bitValue, Ram &ram);
    static bool getBool(char c);
};

#endif