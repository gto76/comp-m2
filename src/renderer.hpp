#ifndef RENDERER_H
#define RENDERER_H

#include <map>
#include <set>
#include <string>
#include <vector>

#include "addr_space.hpp"

using namespace std;

class Address;
class Cpu;
class Cursor;
class Instruction;
class Printer;
class Ram;
class SpecificInstruction;
class View;

class Renderer {
  public:
    static vector<vector<string>> renderState(const Printer &printerIn,
                                              const Ram &ramIn, 
                                              const Cpu &cpuIn, 
                                              const Cursor &cursorIn, 
                                              const View &view);

  private:
    const Printer &printer;
    const Ram &ram;
    const Cpu &cpu;
    const Cursor &cursor;
    const View &view;
    Renderer (const Printer &printerIn, const Ram &ramIn, const Cpu &cpuIn,
              const Cursor &cursorIn, const View &viewIn)
        : printer(printerIn),
          ram(ramIn),
          cpu(cpuIn),
          cursor(cursorIn),
          view(viewIn) { }
    // Just a way to leave instruction uninitialized at Renderers creation, and
    // optionaly create it later (intended to hold only zero or one 
    // instruction).
    vector<Instruction> instruction;
    vector<Instruction> allInstructions;
    set<int> pointingInstructions;
    bool effectiveInstructionsInitialized;
    vector<Instruction> effectiveInstructions;
    // Counts occurances of a char in a drawing.
    map<string, int> switchIndex;

    // RENDER STATE
    vector<string> insertActualValues(vector<string> lineIn);
    vector<string> insertEscSeqences(vector<string> &lineWithoutEscapeSeqences,
                                     vector<bool> &highlightedChars,
                                     vector<bool> &brightChars);

    // GET HIGHLIGHTED LOCATIONS
    bool pcHighlighted = false;
    vector<bool> getHighlightedLocations(vector<string> &lineIn);
    vector<bool> getBrightLocations(vector<string> &lineIn);
    void highlightPc(vector<bool> &highlightedLocations,
                     vector<string> &lineIn);
    void highlightCursor(vector<bool> &highlightedLocations,
                         vector<string> &lineIn);
    void findCursor(vector<bool> &highlightedLocations, vector<string> &lineIn,
                    string c);
    void highlightPointingInstructions(vector<bool> &highlightedLocations,
                                       vector<string> &lineIn);
    void highlightOperator(vector<bool> &highlightedLocations,
                           vector<string> &lineIn, Instruction *inst);
    void highlightCodeWord(vector<bool> &highlightedLocations,
                           vector<string> &lineIn, Instruction *inst);
    void highlightDataWord(vector<bool> &highlightedLocations,
                           vector<string> &lineIn, Instruction *inst);
    void highlightWord(vector<bool> &highlightedLocations,
                       vector<string> &lineIn, string indicator,
                       AddrSpace addrSpace);
    void highlightLabel(vector<bool> &highlightedLocations, 
                        vector<string> &lineIn, vector<string> label, 
                        vector<string> exclude);

    // GET LIGHTBULB
    string getLightbulb(string cIn);
    string getCodeBit(int i);
    string getDataBit(int i);
    string getBit(AddrSpace space, int i);
    static pair<int, int> convertIndexToCoordinates(int index);
    bool pcPointingToAddress(int adr);
    string getAdrIndicator(AddrSpace addrSpace, int index);
    string getFormattedOutput(int i);
    
    // GET INSTRUCTION
    Instruction* getInstruction();
    bool machineActive();
    bool executionHasntStarted();
    bool executionEnded();
    Instruction initializeInstruction();
    bool instructionHasId(int id);
    bool instructionPointingToAddress(Address adr);
    set<int>* getIndexesOfPointingInstructions();
    set<int> generatePointingInstructions();
    vector<Instruction>* getEffectiveInstructions();
    vector<Instruction>* getAllInstructions();
    bool isAddressReferencedFirstOrder(Address adr);
};

#endif