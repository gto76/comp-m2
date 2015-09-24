#include "renderer.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "addr_space.hpp"
#include "address.hpp"
#include "const.hpp"
#include "cpu.hpp"
#include "cursor.hpp"
#include "instruction.hpp"
#include "printer.hpp"
#include "ram.hpp"
#include "util.hpp"
#include "view.hpp"

using namespace std;

/*
 * Only public method. Also static. It creates new object every time 
 * it gets called.
 */
vector<vector<string>> Renderer::renderState(const Printer &printerIn,
                                             const Ram &ramIn, 
                                             const Cpu &cpuIn, 
                                             const Cursor &cursorIn,
                                             const View &viewIn) {
  Renderer instance(printerIn, ramIn, cpuIn, cursorIn, viewIn);
  vector<vector<string>> out;
  for (vector<string> line : viewIn.lines) {
    out.push_back(instance.insertActualValues(line));
  }
  return out;
}

vector<string> Renderer::insertActualValues(vector<string> lineIn) {
  vector<bool> highlightedChars = getHighlightedLocations(lineIn);
  vector<bool> brightChars = getBrightLocations(lineIn);
  vector<string> lineOut;
  int i = 0;
  for (string cIn : lineIn) {
    string sOut;
    bool charIsALightbulb = LIGHTBULB_INDICATORS.count(cIn);
    if (charIsALightbulb) {
      bool state = getLightbulb(cIn);
      if (BRIGHTEN_LIGHTBULBS) {
        if (state) {
          brightChars[i] = true;
        }
      }
      sOut = view.getLightbulb(state);
    } else if (cIn == OUTPUT_INDICATOR) {
      sOut = getFormattedOutput(switchIndex[cIn]++);
    } else {
      sOut = cIn;
    }
    lineOut.push_back(sOut);
    i++;
  }
  return insertEscSeqences(lineOut, highlightedChars, brightChars);
}

vector<string> Renderer::insertEscSeqences(
    vector<string> &lineWithoutEscapeSeqences, vector<bool> &highlightedChars,
    vector<bool> &brightChars) {
  vector<string> lineOut;
  bool insideHighlightBlock = false;
  bool insideBrightBlock = false;
  for (size_t i = 0; i < lineWithoutEscapeSeqences.size(); i++) {
    bool firstCharInsideBlock = highlightedChars[i] && !insideHighlightBlock;
    if (firstCharInsideBlock) {
      lineOut.insert(lineOut.end(), HIGHLIGHT_ESC_VEC.begin(),   
                     HIGHLIGHT_ESC_VEC.end());
      insideHighlightBlock = true;
    }
    bool firstCharOutsideBlock = !highlightedChars[i] && insideHighlightBlock;
    if (firstCharOutsideBlock) {
      lineOut.insert(lineOut.end(), HIGHLIGHT_END_ESC_VEC.begin(), 
                     HIGHLIGHT_END_ESC_VEC.end());
      insideHighlightBlock = false;
    }
    firstCharInsideBlock = brightChars[i] && !insideBrightBlock;
    if (firstCharInsideBlock) {
      lineOut.insert(lineOut.end(), BRIGHT_ESC_VEC.begin(),   
                     BRIGHT_ESC_VEC.end());
      insideBrightBlock = true;
    }
    firstCharOutsideBlock = !brightChars[i] && insideBrightBlock;
    if (firstCharOutsideBlock) {
      lineOut.insert(lineOut.end(), BRIGHT_END_ESC_VEC.begin(), 
                     BRIGHT_END_ESC_VEC.end());
      insideBrightBlock = false;
    }
    lineOut.push_back(lineWithoutEscapeSeqences[i]);
  }
  return lineOut;
}

////////////////////////////
/// GET BRIGHT LOCATIONS ///
////////////////////////////

vector<bool> Renderer::getBrightLocations(vector<string> &lineIn) {
  vector<bool> brightLocations (lineIn.size(), false);
  if (BRIGHTEN_CURSOR) {
    highlightCursor(brightLocations, lineIn);
  }
  return brightLocations;
}

/////////////////////////////////
/// GET HIGHLIGHTED LOCATIONS ///
/////////////////////////////////

vector<bool> Renderer::getHighlightedLocations(vector<string> &lineIn) {
  vector<bool> highlightedLocations (lineIn.size(), false);
  highlightPc(highlightedLocations, lineIn);
  if (executionEnded()) {
    return highlightedLocations;
  }
  highlightCursor(highlightedLocations, lineIn);
  Instruction *inst = getInstruction();
  bool cursorOnData = inst == NULL;
  if (cursorOnData) {
    highlightPointingInstructions(highlightedLocations, lineIn);
    return highlightedLocations;
  }
  highlightOperator(highlightedLocations, lineIn, inst);
  if (inst->adr.space == CODE) {
    highlightCodeWord(highlightedLocations, lineIn, inst);
  } else if (inst->adr.space == DATA) {
    highlightDataWord(highlightedLocations, lineIn, inst);
  }
  return highlightedLocations;
}

void Renderer::highlightPc(vector<bool> &highlightedLocations,
                                   vector<string> &lineIn) {
  if (executionHasntStarted() || pcHighlighted) {
    return;
  }
  for (size_t i = 0; i < lineIn.size(); i++) {
    if (lineIn[i] == CODE_ADR_INDICATOR) {
      int index = switchIndex[CODE_ADR_INDICATOR];
      if (pcPointingToAddress(index)) {
        highlightedLocations[i] = true;
        pcHighlighted = true;
        return;
      }
    }
  }
}

void Renderer::highlightCursor(vector<bool> &highlightedLocations,
                               vector<string> &lineIn) {
  if (!executionHasntStarted()) {
    return;
  }
  if (cursor.getAddressSpace() == CODE) {
    findCursor(highlightedLocations, lineIn, CODE_INDICATOR);
  } else if (cursor.getAddressSpace() == DATA) {
    findCursor(highlightedLocations, lineIn, DATA_INDICATOR);
  }
}

void Renderer::findCursor(vector<bool> &highlightedLocations,
                                  vector<string> &lineIn, string c) {
  int indexDelta = 0;
  for (size_t i = 0; i < lineIn.size(); i++) {
    if (lineIn[i] == c) {
      int lightbulbIndex = switchIndex[c] + indexDelta++;
      if (cursor.getAbsoluteBitIndex() == lightbulbIndex) {
        highlightedLocations[i] = true;
        return;
      }
    }
  }
}

void Renderer::highlightPointingInstructions(vector<bool> &highlightedLocations,
                                             vector<string> &lineIn) {
  set<int> *pointingInstructions = getIndexesOfPointingInstructions();
  for (size_t i = 0; i < lineIn.size(); i++) {
    if (lineIn[i] == CODE_INDICATOR) {
      int addressValue = switchIndex[CODE_INDICATOR] / WORD_SIZE;
      if (pointingInstructions->count(addressValue)) {
        highlightedLocations[i] = true;
      }
    }
  }
}

void Renderer::highlightOperator(vector<bool> &highlightedLocations,
                                         vector<string> &lineIn,
                                         Instruction *inst) {
  string exclude;
  if (inst->isLogic()) {
    exclude = LOGIC_OPS_INDICATOR[min(inst->logicIndex, 8)];
  }
  if (inst->index == INC_DEC_OPS_INDEX) {
    if (inst->logicIndex <= 7) {
      exclude = "INC";
    } else {
      exclude = "DEC";
    }
  }
  string label = " " + inst->label;
  label.append(11 - inst->label.length(), ' ');  
  highlightLabel(highlightedLocations, lineIn, Util::stringToVecOfString(label), 
                 Util::stringToVecOfString(exclude));
}

void Renderer::highlightCodeWord(vector<bool> &highlightedLocations,
                                        vector<string> &lineIn,
                                        Instruction *inst) {
  if (inst->adr.val == LAST_ADDRESS) {
    vector<string> stopLabel = Util::stringToVecOfString(LAST_CODE_ADDR_LABEL);
    highlightLabel(highlightedLocations, lineIn, stopLabel, {});
    return;
  }
  highlightWord(highlightedLocations, lineIn, CODE_INDICATOR, CODE);
}

void Renderer::highlightDataWord(vector<bool> &highlightedLocations,
                                         vector<string> &lineIn,
                                         Instruction *inst) {
  if (inst->adr.val == LAST_ADDRESS) {
    vector<string> inOutLabel = Util::stringToVecOfString(LAST_DATA_ADDR_LABEL);
    highlightLabel(highlightedLocations, lineIn, inOutLabel, {});
    return;
  }
  highlightWord(highlightedLocations, lineIn, DATA_INDICATOR, DATA);
}

void Renderer::highlightWord(vector<bool> &highlightedLocations,
                                     vector<string> &lineIn, string indicator,
                                     AddrSpace addrSpace) {
  for (size_t i = 0; i < lineIn.size(); i++) {
    if (lineIn[i] == indicator) {
      int addressValue = switchIndex[indicator] / WORD_SIZE;
      Address adr = Address(addrSpace, Util::getBoolNibb(addressValue));
      if (instructionPointingToAddress(adr)) {
        highlightedLocations[i] = !highlightedLocations[i];
      }
    }
  }
}

void Renderer::highlightLabel(vector<bool> &highlightedLocations,
                                     vector<string> &lineIn,
                                     vector<string> label,
                                     vector<string> exclude) {
  auto it = search(begin(lineIn), end(lineIn), begin(label), end(label));
  int labelPosition = it - lineIn.begin();
  bool notFound = it == end(lineIn);
  if (notFound) {
    return;
  }
  size_t excludePosition = numeric_limits<size_t>::max();
  if (!exclude.empty()) {
    it = search(begin(label), end(label), begin(exclude), end(exclude));
    excludePosition = it - label.begin();
  }
  for (size_t i = labelPosition; i < labelPosition + label.size(); i++) {
    bool highlight = (i < labelPosition + excludePosition ||
                      i >= labelPosition + excludePosition + exclude.size());
    if (highlight) {
      highlightedLocations[i] = true;
    }
  }
}

/////////////////////
/// GET LIGHTBULB ///
/////////////////////

bool Renderer::getLightbulb(string cIn) {
  int i = switchIndex[cIn]++;
  if (cIn == CODE_INDICATOR) {
      return getCodeBit(i);
  } else if (cIn == DATA_INDICATOR) {
      return getDataBit(i);
  } else if (cIn == REGISTER_INDICATOR) {
      return cpu.getRegister().at(i);
  } else if (cIn == CODE_ADR_INDICATOR) {
      return getAdrIndicator(CODE, i);
  } else if (cIn == DATA_ADR_INDICATOR) {
      return getAdrIndicator(DATA, i);
  }
  cerr << "There was an error parsing a drawing file.";
  cerr << " Problem with char" << cIn << ". Will ignore it.";
  return false;
}

bool Renderer::getCodeBit(int i) {
  return getBit(CODE, i);
}

bool Renderer::getDataBit(int i) {
  return getBit(DATA, i);
}

bool Renderer::getBit(AddrSpace space, int i) {
  pair<int, int> coord = convertIndexToCoordinates(i);
  return ram.state.at(space).at(coord.second).at(coord.first);
}

pair<int, int> Renderer::convertIndexToCoordinates(int index) {
  int y = index / WORD_SIZE;
  int x = index % WORD_SIZE;
  return pair<int, int>(x, y);
}

bool Renderer::pcPointingToAddress(int adr) {
  if (executionHasntStarted()) {
    return false;
  }
  return Util::getInt(cpu.getPc()) == adr;
}

bool Renderer::getAdrIndicator(AddrSpace addrSpace, int index) {
  Address indicatorsAddress = Address(addrSpace, Util::getBoolNibb(index));
  return isAddressReferencedFirstOrder(indicatorsAddress);
}

//////////////////
/// GET OUTPUT ///
//////////////////

string Renderer::getFormattedOutput(int i) {
  if (printer.getPrinterOutput().length() <= (unsigned) i) {
    return " ";
  } else {
    return string(1, printer.getPrinterOutput().at(i));
  }
}

///////////////////////
/// GET INSTRUCTION ///
///////////////////////

Instruction* Renderer::getInstruction() {
  bool noActiveInstruction = !machineActive() &&
                             cursor.getAddressSpace() == DATA;
  if (noActiveInstruction) {
    return NULL;
  }
  if (instruction.size() == 0) {
    instruction.push_back(initializeInstruction());
  }
  return &instruction[0];
}

bool Renderer::machineActive() {
  return !(executionHasntStarted() || executionEnded());
}

bool Renderer::executionHasntStarted() {
  return cpu.getCycle() == 0;
}

bool Renderer::executionEnded() {
  return Util::getInt(cpu.getPc()) == RAM_SIZE;
}

Instruction Renderer::initializeInstruction() {
  if (machineActive()) {
    return cpu.getInstruction();
  } else {
    return Instruction(cursor.getWord(), EMPTY_WORD, &ram);
  }
}

bool Renderer::instructionHasId(int id) {
  Instruction *inst = getInstruction();
  if (inst == NULL) {
    return false;
  }
  return inst->index == id;
}

/*
 * Is instruction pointing to passed address in passed address space.
 */
bool Renderer::instructionPointingToAddress(Address adr) {
  Instruction *inst = getInstruction();
  if (inst == NULL) {
    return false;
  }
  return inst->adr == adr;
}

/// INSTRUCTION HIGHLIGHT ///

set<int>* Renderer::getIndexesOfPointingInstructions() {
  if (pointingInstructions.empty()) {
    pointingInstructions = generatePointingInstructions();
    if (pointingInstructions.empty()) {
      pointingInstructions.insert(-1);
    }
  }
  return &pointingInstructions;
}

set<int> Renderer::generatePointingInstructions() {
  vector<Instruction> *allInstructions = getEffectiveInstructions();
  set<int> out;
  int i = 0;
  for (Instruction inst : *allInstructions) {
    if (inst.adr == cursor.getAddress()) {
      out.insert(i);
    }
    i++;
  }
  return out;
}

/// ADDRESS INDICATOR ///

bool Renderer::isAddressReferencedFirstOrder(Address adr) {
  // cerr << "Is address referenced " << Util::getString(adr.val);
  if (adr.space == CODE) {
    // cerr << " CODE" << endl;
  } else {
    // cerr << " DATA" << endl;
  };
  // cerr << "======================" << endl;
  vector<Instruction> *instructions = getEffectiveInstructions();
  for (Instruction inst : *instructions) {
    // cerr << "Instruction " << inst.inst->getLabel() << endl;
    vector<Address> aaa = inst.firstOrderAdr;
    // cerr << "has address " << Util::getString(aaa[0].val);
    if (aaa[0].space == CODE) {
      // cerr << " CODE" << endl;
    } else {
      // cerr << " DATA" << endl;
    };
    bool isReferenced = find(aaa.begin(), aaa.end(), adr) != aaa.end();
    // if (adr.space == DATA && adr.val == LAST_ADDRESS) {
    //   cerr << "Last Data adr is referenced " << to_string(isReferenced) << endl;
    // }
    if (isReferenced) {
      return true;
    }
  }
  return false;
}

/// UTIL ///

void printInstructions(vector<Instruction> effectiveInstructions) {
  cerr << "LIST OF EFFECTIVE INST" << endl;
  for (Instruction inst : effectiveInstructions) {
    cerr << inst.inst->getLabel() << " " << Util::getString(inst.adr.val);
    if (inst.adr.space == DATA) {
      cerr << " DATA" << endl;
    } else if (inst.adr.space == CODE) {
      cerr << " CODE" << endl;
    } else {
      cerr << " NONE" << endl;
    }
  }
  cerr << endl;
}

/*
 * Doesn't include empty instructions from last non-empty forward.
 */
vector<Instruction>* Renderer::getEffectiveInstructions() {
  if (effectiveInstructionsInitialized) {
    // cerr << "Effective instructions already initialized" << endl;
  }
  if (!effectiveInstructionsInitialized) {
    effectiveInstructions =
        Instruction::getEffectiveInstructions(ram, cpu.getRegister());
    effectiveInstructionsInitialized = true;
  }
  return &effectiveInstructions;
}

