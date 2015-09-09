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
    vector<Instruction> allInstructions;
    set<int> pointingInstructions;
    // Counts occurances of a char in a drawing.
    map<char, int> switchIndex;

    // RENDER STATE
    string insertActualValues(string lineIn);
    string insertEscSeqences(string lineWithoutEscapeSeqences,
                             vector<bool> highlightedChars,
                             vector<bool> boldChars);
    // GET HIGHLIGHTED LOCATIONS
    vector<bool> getHighlightedLocations(string lineIn);
    vector<bool> highlightPointingInstructions(
        vector<bool> highlightedLocations, string lineIn);
    vector<bool> highlightOperators(vector<bool> highlightedLocations,
                                    string lineIn, Instruction *inst);
    vector<bool> highlightCodeWord(vector<bool> highlightedLocations,
                                   string lineIn, Instruction *inst);
    vector<bool> highlightDataWord(vector<bool> highlightedLocations,
                                   string lineIn, Instruction *inst);
    vector<bool> highlightWords(vector<bool> highlightedLocations,
                                string lineIn, char indicator,
                                AddrSpace addrSpace);
    vector<bool> highlightLabel(vector<bool> highlightedLocations,
                                string lineIn, string label, string exclude);
    // GET BOLD LOCATIONS
    vector<bool> getBoldLocations(string lineIn);
    vector<bool> enboldenCodeWords(vector<bool> boldLocations, string lineIn);
    vector<bool> enboldenDataWords(vector<bool> boldLocations, string lineIn);
    vector<bool> enboldenWords(vector<bool> boldLocations, string lineIn,
                               char indicator, AddrSpace addrSpace);
    bool isAddressReferenced(Address adr);
    // GET LIGHTBULB
    char getLightbulb(char cIn);
    char getCodeBit(int i);
    char getDataBit(int i);
    char getCharAt(int i, vector<vector<bool>>* matrix);
    bool pcPointingToAddress(int adr);
    char getFormattedOutput(int i);
    // GET INSTRUCTION
    Instruction* getInstruction();
    bool machineActive();
    Instruction initializeInstruction();
    bool instructionHasId(int id);
    bool instructionPointingToAddress(Address adr);
    set<int>* getIndexesOfPointingInstructions();
    set<int> generatePointingInstructions();
    vector<Instruction>* getAllInstructions();
};

#endif