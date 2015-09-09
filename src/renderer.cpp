#include "renderer.hpp"

#include <algorithm>
#include <fstream>
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
string Renderer::renderState(const Printer &printerIn, const Ram &ramIn, 
                             const Cpu &cpuIn, const Cursor &cursorIn) {
  Renderer instance(printerIn, ramIn, cpuIn, cursorIn);
  string out;
  for (string line : Util::splitString(drawing)) {
    out += instance.insertActualValues(line) + "\n";
  }
  out.erase(out.end() - 1);
  return out;
}

string Renderer::insertActualValues(string lineIn) {
  vector<bool> highlightedChars = getHighlightedLocations(lineIn);
  vector<bool> boldChars = getBoldLocations(lineIn);
  string lineOut;
  for (char cIn : lineIn) {
    string sOut = "";
    // Regex: [0-9a-z]
    bool charIsALightbulb = (cIn >= 'a' && cIn <= 'z') || 
                            (cIn >= '0' && cIn <= '9');
    if (charIsALightbulb) {
      sOut.push_back(getLightbulb(cIn));
    } else {
      sOut.push_back(cIn);
    }
    lineOut += sOut;
  }
  // TODO combine in one function
  // lineOut = insertEscSeqences(lineOut, highlightedChars, HIGHLIGHT_ESC,
  //                          HIGHLIGHT_END_ESC);
  // return insertEscSeqences(lineOut, boldChars, BOLD_ESC, BOLD_END_ESC);
  return insertEscSeqences(lineOut, highlightedChars, boldChars);
}

string Renderer::insertEscSeqences(string lineWithoutEscapeSeqences,
                                   vector<bool> highlightedChars,
                                   vector<bool> boldChars) {
  string lineOut = "";
  bool insideHighlightBlock = false;
  bool insideBoldBlock = false;
  for (size_t i = 0; i < lineWithoutEscapeSeqences.length(); i++) {
    bool firstCharInsideBlock = highlightedChars[i] && !insideHighlightBlock;
    if (firstCharInsideBlock) {
      lineOut += HIGHLIGHT_ESC;
      insideHighlightBlock = true;
    }
    bool firstCharOutsideBlock = !highlightedChars[i] && insideHighlightBlock;
    if (firstCharOutsideBlock) {
      lineOut += HIGHLIGHT_END_ESC;
      insideHighlightBlock = false;
    }
    firstCharInsideBlock = boldChars[i] && !insideBoldBlock;
    if (firstCharInsideBlock) {
      lineOut += BOLD_ESC;
      insideBoldBlock = true;
    }
    firstCharOutsideBlock = !boldChars[i] && insideBoldBlock;
    if (firstCharOutsideBlock) {
      lineOut += BOLD_END_ESC;
      insideBoldBlock = false;
    }
    lineOut += lineWithoutEscapeSeqences[i];
  }
  return lineOut;
}

// string Renderer::insertEscSeqences(string lineWithoutEscapeSeqences,
//                                    vector<bool> characterInsideSqence,
//                                    string seqenceStart, string seqenceStop) {
//   string lineOut = "";
//   bool insideBlock = false;
//   for (size_t i = 0; i < lineWithoutEscapeSeqences.length(); i++) {
//     bool firstCharInsideBlock = characterInsideSqence[i] && !insideBlock;
//     if (firstCharInsideBlock) {
//       lineOut += seqenceStart;
//       insideBlock = true;
//     }
//     bool firstCharOutsideBlock = !characterInsideSqence[i] && insideBlock;
//     if (firstCharOutsideBlock) {
//       lineOut += seqenceStop;
//       insideBlock = false;
//     }
//     lineOut += lineWithoutEscapeSeqences[i];
//   }
//   return lineOut;
// }

/////////////////////////////////
/// GET HIGHLIGHTED LOCATIONS ///
/////////////////////////////////

vector<bool> Renderer::getHighlightedLocations(string lineIn) {
  vector<bool> highlightedLocations (lineIn.length(), false);
  Instruction *inst = getInstruction();
  if (inst == NULL) {
    return highlightPointingInstructions(highlightedLocations, lineIn);
  }
  highlightedLocations = highlightOperators(highlightedLocations, lineIn, inst);
  if (inst->adr.space == CODE) {
    highlightedLocations = highlightCodeWord(highlightedLocations, lineIn, inst);
  } else if (inst->adr.space == DATA) {
    highlightedLocations = highlightDataWord(highlightedLocations, lineIn, inst);
  }
  return highlightedLocations;
}

////////////

vector<bool> Renderer::highlightPointingInstructions(vector<bool> highlightedLocations,
                                                    string lineIn) {
  set<int> *pointingInstructions = getIndexesOfPointingInstructions();
  for (size_t i = 0; i < lineIn.length(); i++) {
    if (lineIn[i] == 'a') {
      int addressValue = switchIndex['a'] / WORD_SIZE;
      if (pointingInstructions->count(addressValue)) {
        highlightedLocations[i] = true;
      }
    }
  }
  return highlightedLocations;
}

///////////////////

vector<bool> Renderer::highlightOperators(vector<bool> highlightedLocations,
                                         string lineIn, Instruction *inst) {
  string exclude = "";
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
  return highlightLabel(highlightedLocations, lineIn, label, exclude);
}

/////////////

vector<bool> Renderer::highlightCodeWord(vector<bool> highlightedLocations,
                                        string lineIn, Instruction *inst) {
  if (inst->adr.val == LAST_ADDRESS) {
    return highlightLabel(highlightedLocations, lineIn, LAST_CODE_ADDR_LABEL, "");
  }
  return highlightWords(highlightedLocations, lineIn, 'a', CODE);
}

vector<bool> Renderer::highlightDataWord(vector<bool> highlightedLocations,
                                        string lineIn, Instruction *inst) {
  if (inst->adr.val == LAST_ADDRESS) {
    return highlightLabel(highlightedLocations, lineIn, LAST_DATA_ADDR_LABEL, "");
  }
  return highlightWords(highlightedLocations, lineIn, 'b', DATA);
}

vector<bool> Renderer::highlightWords(vector<bool> highlightedLocations,
                                     string lineIn, char indicator,
                                     AddrSpace addrSpace) {
  for (size_t i = 0; i < lineIn.length(); i++) {
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
                                     string lineIn, string label,
                                     string exclude) {
  size_t labelPosition = lineIn.find(label);
  if (labelPosition == string::npos) {
    return highlightedLocations;
  }
  size_t excludePosition = numeric_limits<size_t>::max();
  if (exclude != "") {
    excludePosition = label.find(exclude);
  }
  for (size_t i = labelPosition; i < labelPosition + label.length(); i++) {
    bool highlight = (labelPosition != string::npos) &&
                     (i < labelPosition + excludePosition ||
                     i >= labelPosition + excludePosition + exclude.length());
    if (highlight) {
      highlightedLocations[i] = true;
    }
  }
  return highlightedLocations;
}

//////////////////////////
/// GET BOLD LOCATIONS ///
//////////////////////////

vector<bool> Renderer::getBoldLocations(string lineIn) {
  vector<bool> boldLocations (lineIn.length(), false);
  boldLocations = enboldenCodeWords(boldLocations, lineIn);
  boldLocations = enboldenDataWords(boldLocations, lineIn);
  return boldLocations;
}

vector<bool> Renderer::enboldenCodeWords(vector<bool> boldLocations,
                                         string lineIn) {
  // TODO: STOP
  // TODO: g
  return enboldenWords(boldLocations, lineIn, 'a', CODE);
}

vector<bool> Renderer::enboldenDataWords(vector<bool> boldLocations,
                                         string lineIn) {
  // TODO: IN/OUT
  // TODO: v
  return enboldenWords(boldLocations, lineIn, 'b', DATA);
}

// TODO COMBINE WITH HIGHLIGHT WORDS (pass function as pointer)
vector<bool> Renderer::enboldenWords(vector<bool> boldLocations,
                                     string lineIn, char indicator,
                                     AddrSpace addrSpace) {
  for (size_t i = 0; i < lineIn.length(); i++) {
    if (lineIn[i] == indicator) {
      int addressValue = switchIndex[indicator] / WORD_SIZE; // TODO out of loop
      Address adr = Address(addrSpace, Util::getBoolNibb(addressValue)); // TODO out of loop
      if (isAddressReferenced(adr)) { // out
        boldLocations[i] = true; // out
      }
    }
  }
  return boldLocations;
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

/////////////////////
/// GET LIGHTBULB ///
/////////////////////

char Renderer::getLightbulb(char cIn) {
  int i = switchIndex[cIn]++;
  switch (cIn) {
    case 'a':
      return getCodeBit(i);
    case 'b':
      return getDataBit(i);
    case 'p':
      return Util::getChar(pcPointingToAddress(i));
    case 'd': {
      Address adr = Address(CODE, Util::getBoolNibb(i));
      return Util::getChar(instructionPointingToAddress(adr));
    }
    case 's': {
      Address adr = Address(DATA, Util::getBoolNibb(i));
      return Util::getChar(instructionPointingToAddress(adr));
    }
    case 'r':
      return  Util::getChar(cpu.getRegister().at(i));
    case 'i':
      return Util::getChar(instructionHasId(i));
    case 'v':
      return ram.variableNames[i];
    case 'o':
      return getFormattedOutput(i);
    case 'g':
      return '-';
  }
  fprintf(stderr, "There was an error parsing a drawing file."
          " Problem with char %c. Will ignore it.", cIn);
  return ' ';
}

char Renderer::getCodeBit(int i) {
  return getCharAt(i, &ram.state[CODE]);
}

char Renderer::getDataBit(int i) {
  return getCharAt(i, &ram.state[DATA]);
}

char Renderer::getCharAt(int i, vector<vector<bool>>* matrix) {
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

char Renderer::getFormattedOutput(int i) {
  if (printer.getPrinterOutput().length() <= (unsigned) i) {
    return ' ';
  } else {
    return printer.getPrinterOutput().at(i);
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
      Instruction inst = Instruction(word, cpu.getRegister(), ram); // TODO:
      // Suspition that some register values might not be right...
      allInstructions.push_back(inst);
    }
  }
  return &allInstructions;
}
