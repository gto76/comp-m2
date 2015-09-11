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
#include "drawing.hpp"
#include "instruction.hpp"
#include "printer.hpp"
#include "ram.hpp"
#include "util.hpp"

using namespace std;

/*
 * Only public method. Also static. It creates new object every time 
 * it gets called.
 */
vector<vector<string>> Renderer::renderState(const Printer &printerIn,
                                             const Ram &ramIn, 
                                             const Cpu &cpuIn, 
                                             const Cursor &cursorIn) {
  Renderer instance(printerIn, ramIn, cpuIn, cursorIn);
  vector<vector<string>> out;
  for (vector<string> line : Util::splitIntoLines(drawing)) {
    out.push_back(instance.insertActualValues(line));
  }
  return out;
}

vector<string> Renderer::insertActualValues(vector<string> lineIn) {
  vector<bool> highlightedChars = getHighlightedLocations(lineIn);
  vector<string> lineOut;
  for (string cIn : lineIn) {
    string sOut = "";
    // Regex: [0-9a-z]
    bool charIsALightbulb = ALL_INDICATORS.count(cIn);
    if (charIsALightbulb) {
      sOut += getLightbulb(cIn);
    } else {
      sOut += cIn;
    }
    lineOut.push_back(sOut);
  }
  return insertEscSeqences(lineOut, highlightedChars);
}

vector<string> Renderer::insertEscSeqences(
    vector<string> lineWithoutEscapeSeqences, vector<bool> highlightedChars) {
  vector<string> lineOut;
  bool insideHighlightBlock = false;
  for (size_t i = 0; i < lineWithoutEscapeSeqences.size(); i++) {
    bool firstCharInsideBlock = highlightedChars[i] && !insideHighlightBlock;
    if (firstCharInsideBlock) {
      vector<string> highlightEsc = Util::stringToVecOfString(HIGHLIGHT_ESC);
      lineOut.insert(lineOut.end(), highlightEsc.begin(), highlightEsc.end());
      insideHighlightBlock = true;
    }
    bool firstCharOutsideBlock = !highlightedChars[i] && insideHighlightBlock;
    if (firstCharOutsideBlock) {
      vector<string> highlightEscEnd = 
          Util::stringToVecOfString(HIGHLIGHT_END_ESC);
      lineOut.insert(lineOut.end(), highlightEscEnd.begin(), 
                     highlightEscEnd.end());
      insideHighlightBlock = false;
    }
    lineOut.push_back(lineWithoutEscapeSeqences[i]);
  }
  return lineOut;
}

/////////////////////////////////
/// GET HIGHLIGHTED LOCATIONS ///
/////////////////////////////////

vector<bool> Renderer::getHighlightedLocations(vector<string> lineIn) {
  vector<bool> highlightedLocations (lineIn.size(), false);
  highlightedLocations = highlightPc(highlightedLocations, lineIn);
  highlightedLocations = highlightCursor(highlightedLocations, lineIn);
  Instruction *inst = getInstruction();
  if (inst == NULL) {
    return highlightPointingInstructions(highlightedLocations, lineIn);
  }
  highlightedLocations = highlightOperators(highlightedLocations, lineIn, inst);
  if (inst->adr.space == CODE) {
    highlightedLocations = highlightCodeWord(highlightedLocations, lineIn, 
                                            inst);
  } else if (inst->adr.space == DATA) {
    highlightedLocations = highlightDataWord(highlightedLocations, lineIn,
                                             inst);
  }
  return highlightedLocations;
}

vector<bool> Renderer::highlightPc(vector<bool> highlightedLocations,
                                       vector<string> lineIn) {
  if (!machineActive()) {
    return highlightedLocations;
  }
  for (size_t i = 0; i < lineIn.size(); i++) {
    if (lineIn[i] == CODE_ADR_INDICATOR) {
      int index = switchIndex[CODE_ADR_INDICATOR];
      highlightedLocations[i] = pcPointingToAddress(index);;
    }
  }
  return highlightedLocations;
}

/////////

vector<bool> Renderer::highlightCursor(vector<bool> highlightedLocations,
                                       vector<string> lineIn) {
  if (machineActive()) {
    return highlightedLocations;
  }
  if (cursor.getAddressSpace() == CODE) {
    highlightedLocations = findCursor(highlightedLocations, lineIn,
                                      CODE_INDICATOR);
  } else if (cursor.getAddressSpace() == DATA) {
    highlightedLocations = findCursor(highlightedLocations, lineIn,
                                      DATA_INDICATOR);
  }
  return highlightedLocations;
}

vector<bool> Renderer::findCursor(vector<bool> highlightedLocations,
                                  vector<string> lineIn, string c) {
  int indexDelta = 0;
  for (size_t i = 0; i < lineIn.size(); i++) {
    if (lineIn[i] == c) {
      int lightbulbIndex = switchIndex[c] + indexDelta++;
      if (cursor.getAbsoluteBitIndex() == lightbulbIndex) {
        highlightedLocations[i] = true;
      }
    }
  }
  return highlightedLocations;
}

////////////

vector<bool> Renderer::highlightPointingInstructions(
    vector<bool> highlightedLocations, vector<string> lineIn) {
  set<int> *pointingInstructions = getIndexesOfPointingInstructions();
  for (size_t i = 0; i < lineIn.size(); i++) {
    if (lineIn[i] == CODE_INDICATOR) {
      int addressValue = switchIndex[CODE_INDICATOR] / WORD_SIZE;
      if (pointingInstructions->count(addressValue)) {
        highlightedLocations[i] = true;
      }
    }
  }
  return highlightedLocations;
}

///////////////////


vector<bool> Renderer::highlightOperators(vector<bool> highlightedLocations,
                                         vector<string> lineIn,
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
  return highlightLabel(highlightedLocations, lineIn,
                        Util::stringToVecOfString(label), 
                        Util::stringToVecOfString(exclude));
}


/////////////

vector<bool> Renderer::highlightCodeWord(vector<bool> highlightedLocations,
                                        vector<string> lineIn,
                                        Instruction *inst) {
  if (inst->adr.val == LAST_ADDRESS) {
    return highlightLabel(highlightedLocations, lineIn,
                          Util::stringToVecOfString(LAST_CODE_ADDR_LABEL), {});
  }
  return highlightWords(highlightedLocations, lineIn, CODE_INDICATOR, CODE);
}

vector<bool> Renderer::highlightDataWord(vector<bool> highlightedLocations,
                                         vector<string> lineIn,
                                         Instruction *inst) {
  if (inst->adr.val == LAST_ADDRESS) {
    return highlightLabel(highlightedLocations, lineIn, 
                        Util::stringToVecOfString(LAST_DATA_ADDR_LABEL),
                          {});
  }
  return highlightWords(highlightedLocations, lineIn, DATA_INDICATOR, DATA);
}

vector<bool> Renderer::highlightWords(vector<bool> highlightedLocations,
                                     vector<string> lineIn, string indicator,
                                     AddrSpace addrSpace) {
  for (size_t i = 0; i < lineIn.size(); i++) {
    if (lineIn[i] == indicator) {
      int addressValue = switchIndex[indicator] / WORD_SIZE;
      Address adr = Address(addrSpace, Util::getBoolNibb(addressValue));
      if (instructionPointingToAddress(adr)) {
        highlightedLocations[i] = true;
      }
    }
  }
  return highlightedLocations;
}

///////////

vector<bool> Renderer::highlightLabel(vector<bool> highlightedLocations,
                                     vector<string> lineIn,
                                     vector<string> label,
                                     vector<string> exclude) {
  auto it = search(begin(lineIn), end(lineIn), begin(label), end(label));
  int labelPosition = it - lineIn.begin();
  bool notFound = it == end(lineIn);
  if (notFound) {
    return highlightedLocations;
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
  return highlightedLocations;
}

/////////////////////
/// GET LIGHTBULB ///
/////////////////////

string Renderer::getLightbulb(string cIn) {
  int i = switchIndex[cIn]++;
  if (cIn == CODE_INDICATOR) {
      return getCodeBit(i);
  } else if (cIn == DATA_INDICATOR) {
      return getDataBit(i);
  } else if (cIn == REGISTER_INDICATOR) {
      return  Util::getChar(cpu.getRegister().at(i));
  } else if (cIn == CODE_ADR_INDICATOR) {
      return getLocationName(CODE, i);
  } else if (cIn == DATA_ADR_INDICATOR) {
      return getLocationName(DATA, i);
  } else if (cIn == OUTPUT_INDICATOR) {
      return getFormattedOutput(i);
  }
  cerr << "There was an error parsing a drawing file.";
  cerr << " Problem with char" << cIn << ". Will ignore it.";
  return " ";
}

string Renderer::getCodeBit(int i) {
  return getCharAt(i, &ram.state[CODE]);
}

string Renderer::getDataBit(int i) {
  return getCharAt(i, &ram.state[DATA]);
}

string Renderer::getCharAt(int i, vector<vector<bool>>* matrix) {
  int j = i / WORD_SIZE;
  i = i % WORD_SIZE;
  return Util::getChar((*matrix).at(j).at(i));
}

bool Renderer::pcPointingToAddress(int adr) {
  bool executionHasntStarted = cpu.getCycle() == 0;
  if (executionHasntStarted) {
    return false;
  }
  return Util::getInt(cpu.getPc()) == adr;
}

string Renderer::getLocationName(AddrSpace addrSpace, int index) {
  Address indicatorsAddress = Address(addrSpace, Util::getBoolNibb(index));
  if (isAddressReferenced(indicatorsAddress)) {
    return string(1, '*');
  } else {
    return string(1, '-');
  }
}

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
  bool executionHasntStarted = cpu.getCycle() == 0;
  bool executionEnded = Util::getInt(cpu.getPc()) == RAM_SIZE;
  return !(executionHasntStarted || executionEnded);
}

Instruction Renderer::initializeInstruction() {
  if (machineActive()) {
    return cpu.getInstruction();
  } else {
    return Instruction(cursor.getWord(), EMPTY_WORD, ram);
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
  vector<Instruction> *allInstructions = getAllInstructions();
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

vector<Instruction>* Renderer::getAllInstructions() {
  if (allInstructions.empty()) {
    for (vector<bool> word : ram.state.at(CODE)) {
      Instruction inst = Instruction(word, cpu.getRegister(), ram);
      allInstructions.push_back(inst);
    }
  }
  return &allInstructions;
}

bool Renderer::isAddressReferenced(Address adr) {
  vector<Instruction> *instructions = getAllInstructions();
  for (Instruction inst : *instructions) {
    if (inst.adr == adr) {
      return true;
    }
  }
  return false;
}