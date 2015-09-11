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
    static vector<vector<string>> renderState(const Printer &printerIn, const Ram &ramIn, 
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
    vector<Instruction> allInstructions;
    set<int> pointingInstructions;
    // Counts occurances of a char in a drawing.
    map<string, int> switchIndex;

    // RENDER STATE
    vector<string> insertActualValues(vector<string> lineIn);
    vector<string> insertEscSeqences(vector<string> lineWithoutEscapeSeqences,
                             vector<bool> highlightedChars);

    // GET HIGHLIGHTED LOCATIONS
    vector<bool> getHighlightedLocations(vector<string> lineIn);
    vector<bool> highlightPc(vector<bool> highlightedLocations,
                                       vector<string> lineIn);
    vector<bool> highlightCursor(vector<bool> highlightedLocations,
                                 vector<string> lineIn);
    vector<bool> findCursor(vector<bool> highlightedLocations, vector<string> lineIn,
                            string c);
    vector<bool> highlightPointingInstructions(
        vector<bool> highlightedLocations, vector<string> lineIn);
    vector<bool> highlightOperators(vector<bool> highlightedLocations,
                                    vector<string> lineIn, Instruction *inst);
    vector<bool> highlightCodeWord(vector<bool> highlightedLocations,
                                   vector<string> lineIn, Instruction *inst);
    vector<bool> highlightDataWord(vector<bool> highlightedLocations,
                                   vector<string> lineIn, Instruction *inst);
    vector<bool> highlightWords(vector<bool> highlightedLocations,
                                vector<string> lineIn, string indicator,
                                AddrSpace addrSpace);
    vector<bool> highlightLabel(vector<bool> highlightedLocations,
                                vector<string> lineIn, vector<string> label, 
                                vector<string> exclude);

    // GET LIGHTBULB
    string getLightbulb(string cIn);
    string getCodeBit(int i);
    string getDataBit(int i);
    string getCharAt(int i, vector<vector<bool>>* matrix);
    bool pcPointingToAddress(int adr);
    string getLocationName(AddrSpace addrSpace, int index);
    string getFormattedOutput(int i);
    
    // GET INSTRUCTION
    Instruction* getInstruction();
    bool machineActive();
    Instruction initializeInstruction();
    bool instructionHasId(int id);
    bool instructionPointingToAddress(Address adr);
    set<int>* getIndexesOfPointingInstructions();
    set<int> generatePointingInstructions();
    vector<Instruction>* getAllInstructions();
    bool isAddressReferenced(Address adr);
};

#endif