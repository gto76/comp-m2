#include "load.hpp"

using namespace std;

void Load::fillRamWithFile(const char* filename, Ram &ram) {
  ifstream fileStream;    
  fileStream.open(filename);   
  if (fileStream.fail()) {
    fprintf(stderr, "Invalid filename '%s'. Aborting ram load.", filename);
  } else {
    loadedFilename = filename;
    fillRamWithFilestream(&fileStream, ram);
    fileStream.close();  
  }
}

void Load::fillRamWithFilestream(ifstream* fileStream, Ram &ram) {
  int address = 0;
  while (!fileStream->eof()) {
    string line;
    getline(*fileStream, line);
    bool lineEmptyOrAComment = line.empty() || line[0] == '#';
    if (lineEmptyOrAComment) {
      continue;
    }
    writeLineToRam(line, address, ram);
    if (++address >= 2*RAM_SIZE) {
      return;
    }
  } 
}

void Load::writeLineToRam(string line, int address, Ram &ram) {
  int bitIndex = 0;
  for (char c : line) {
    if (address < RAM_SIZE) { 
      writeInstructionBitToRam(address, bitIndex, getBool(c), ram);
    } else {
      writeDataBitToRam(address-RAM_SIZE, bitIndex, getBool(c), ram);
    }
    if (++bitIndex >= WORD_SIZE) {
      return;
    }
  }
}

void Load::writeInstructionBitToRam(int address, int bitIndex, bool bitValue, Ram &ram) {
  ram.state[CODE].at(address).at(bitIndex) = bitValue;
}

void Load::writeDataBitToRam(int address, int bitIndex, bool bitValue, Ram &ram) {
  ram.state[DATA].at(address).at(bitIndex) = bitValue;
}

bool Load::getBool(char c) {
  return c == '*';
}