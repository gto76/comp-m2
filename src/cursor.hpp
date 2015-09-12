#ifndef CURSOR_H
#define CURSOR_H

#include <map>
#include <tuple>
#include <vector>

#include "addr_space.hpp"
#include "axis.hpp"
#include "comp.hpp"
#include "ram.hpp"
#include "util.hpp"

using namespace std;

class Cursor {
  public:
    Cursor(Ram &ramIn) : ram(ramIn) { }

    void switchAddressSpace();
    AddrSpace getAddressSpace();
    Address getAddress();

    //////// COORDINATES API /////////

    int getAbsoluteBitIndex();
    void increaseX();
    void increaseY();
    void decreaseX();
    void decreaseY();

    /////////// RAM API ////////////

    bool getBit();
    void setBit(bool bit);
    vector<bool> getWord();
    void setWord(vector<bool> word);
    void moveByteUp();
    void moveByteDown();

  private:
    Ram &ram;
    // Selected address space.
    AddrSpace addrSpace = CODE;
    // Selected bit with the cursor.
    map<AddrSpace, map<Axis, int>> cursorPosition;

    bool notInitialized = true;
    void initCheck();
    void initialize();

    int getBitIndex();
    int getAddr();
    void setBitIndex(int bitIndex);
    void setAddr(int addr);
    void moveByteFor(int delta);
    bool switchWordsInRam(int addr1, int addr2);
};

#endif
