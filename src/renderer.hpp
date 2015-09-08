#ifndef RENDERER_H
#define RENDERER_H

#include <map>
#include <string>

#include "address.hpp"
#include "cpu.hpp"
#include "cursor.hpp"
#include "instruction.hpp"
#include "printer.hpp"
#include "ram.hpp"
#include "specific_instruction.hpp"

using namespace std;

class Renderer {
  public:
    static string renderState(const Printer &printerIn, const Ram &ramIn, 
                              const Cpu &cpuIn, const Cursor &cursorIn);

  private:
    Printer printer;
    Ram ram;
    Cpu cpu;
    Cursor cursor;
    Renderer (const Printer &printerIn, const Ram &ramIn, const Cpu &cpuIn,
              const Cursor &cursorIn)
        : printer(printerIn),
          ram(ramIn),
          cpu(cpuIn),
          cursor(cursorIn) { }
    // Just a way to leave instruction uninitialized at Renderers creation, and
    // optionaly create it later (intended to hold only zero or one 
    // instruction).
    vector<Instruction> instruction;
    // Counts occurances of a char in a drawing.
    map<char, int> switchIndex;
    string insertActualValues(string lineIn);
    string setCharToBoldIfLogicOp(char cIn);
    string highlightLogicIndicator(char cIn, Instruction inst,
                                   size_t positionOfCharInLogicLabel);
    string setCharToBoldIfIncDecOp(char cIn);
    string highlightIncOrDec(char cIn, Instruction inst);
    char getLightbulb(char cIn);
    bool getRegisterAt(int i);
    bool pcPointingToAddress(int adr);
    bool instructionPointingToAddress(Address adr);
    bool instructionHasId(int id);
    char getFormattedOutput(int i);
    char getCodeBit(int i);
    char getDataBit(int i);
    bool machineActive();
    Instruction* getInstruction();
    Instruction initializeInstruction();
    string insertBoldEscSeqences(string lineWithoutEscapeSeqences,
                                 vector<bool> characterBoldOrNot);
    vector<bool> getBoldLocations(string lineIn);
    vector<bool> enboldenOperators(vector<bool> boldLocations, string lineIn,
                                   Instruction *inst);
    vector<bool> enboldenWords(vector<bool> boldLocations, string lineIn,
                               Instruction *inst, char indicator,
                               AddrSpace addrSpace);
    vector<bool> enboldenLabel(vector<bool> boldLocations, string lineIn,
                               string label, string exclude);
    vector<bool> enboldenCodeWord(vector<bool> boldLocations, string lineIn,
                                  Instruction *inst);
    vector<bool> enboldenDataWord(vector<bool> boldLocations, string lineIn,
                                  Instruction *inst);
};

#endif