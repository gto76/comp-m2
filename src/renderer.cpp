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
  vector<bool> characterBoldOrNot = getBoldLocations(lineIn);
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
  return insertBoldEscSeqences(lineOut, characterBoldOrNot);
}

vector<bool> Renderer::getBoldLocations(string lineIn) {
  vector<bool> boldLocations (lineIn.length(), false);
  Instruction *inst = getInstruction();
  if (inst == NULL) {
    return enboldenPointingInstructions(boldLocations, lineIn);
    //return boldLocations;
  }
  boldLocations = enboldenOperators(boldLocations, lineIn, inst);
  if (inst->adr.space == CODE) {
    boldLocations = enboldenCodeWord(boldLocations, lineIn, inst);
  } else if (inst->adr.space == DATA) {
    boldLocations = enboldenDataWord(boldLocations, lineIn, inst);
  }
  return boldLocations;
}

vector<bool> Renderer::enboldenPointingInstructions(vector<bool> boldLocations,
                                                    string lineIn) {
  set<int> pointingInstructions = getIndexesOfPointingInstructions();
  for (size_t i = 0; i < lineIn.length(); i++) {
    if (lineIn[i] == 'a') {
      int addressValue = switchIndex['a'] / WORD_SIZE;
      if (pointingInstructions.count(addressValue)) {
        boldLocations[i] = true;
      }
    }
  }
  return boldLocations;
}

set<int> Renderer::getIndexesOfPointingInstructions() {
  if (pointingInstructions.empty()) {
    set<int> temp = generatePointingInstructions();
    if (!temp.empty()) {
      pointingInstructions = temp;
    } else {
      pointingInstructions.insert(-1);
    }
  }
  return pointingInstructions;
}

set<int> Renderer::generatePointingInstructions() {
  vector<Instruction> allInstructions = getAllInstructions();
  set<int> out;
  int i = 0;
  for (Instruction inst : allInstructions) {
    if (inst.adr == cursor.getAddress()) {
      out.insert(i);
    }
    i++;
  }
  return out;
}

vector<Instruction> Renderer::getAllInstructions() {
  vector<Instruction> instructions;
  for (vector<bool> word : ram.state.at(CODE)) {
    Instruction inst = Instruction(word, cpu.getRegister(), ram); // TODO:
    // Suspition that some register values might not be right...
    instructions.push_back(inst);
  }
  return instructions;
}

vector<bool> Renderer::enboldenOperators(vector<bool> boldLocations,
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
  return enboldenLabel(boldLocations, lineIn, label, exclude);
}

vector<bool> Renderer::enboldenLabel(vector<bool> boldLocations,
                                     string lineIn, string label,
                                     string exclude) {
  size_t labelPosition = lineIn.find(label);
  if (labelPosition == string::npos) {
    return boldLocations;
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
      boldLocations[i] = true;
    }
  }
  return boldLocations;
}

vector<bool> Renderer::enboldenCodeWord(vector<bool> boldLocations,
                                        string lineIn, Instruction *inst) {
  if (inst->adr.val == LAST_ADDRESS) {
    return enboldenLabel(boldLocations, lineIn, LAST_CODE_ADDR_LABEL, "");
  }
  return enboldenWords(boldLocations, lineIn, 'a', CODE);
}

vector<bool> Renderer::enboldenDataWord(vector<bool> boldLocations,
                                        string lineIn, Instruction *inst) {
  if (inst->adr.val == LAST_ADDRESS) {
    return enboldenLabel(boldLocations, lineIn, LAST_DATA_ADDR_LABEL, "");
  }
  return enboldenWords(boldLocations, lineIn, 'b', DATA);
}

vector<bool> Renderer::enboldenWords(vector<bool> boldLocations,
                                     string lineIn, char indicator,
                                     AddrSpace addrSpace) {
  for (size_t i = 0; i < lineIn.length(); i++) {
    if (lineIn[i] == indicator) {
      int addressValue = switchIndex[indicator] / WORD_SIZE;
      Address adr = Address(addrSpace, Util::getBoolNibb(addressValue));
      if (instructionPointingToAddress(adr)) {
        boldLocations[i] = true;
      }
    }
  }
  return boldLocations;
}

string Renderer::insertBoldEscSeqences(string lineWithoutEscapeSeqences,
                                       vector<bool> characterBoldOrNot) {
  string lineOut = "";
  bool insideBoldBlock = false;
  for (size_t i = 0; i < lineWithoutEscapeSeqences.length(); i++) {
    bool firstBoldCharOfBlock = characterBoldOrNot[i] && !insideBoldBlock;
    if (firstBoldCharOfBlock) {
      // lineOut += BOLD_ESC;
      lineOut += HIGHLIGHT_ESC;
      insideBoldBlock = true;
    }
    bool firstNonBoldCharOfBlock = !characterBoldOrNot[i] && insideBoldBlock;
    if (firstNonBoldCharOfBlock) {
      // lineOut += BOLD_END_ESC;
      lineOut += HIGHLIGHT_END_ESC;
      insideBoldBlock = false;
    }
    lineOut += lineWithoutEscapeSeqences[i];
  }
  return lineOut;
}

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
  }
  fprintf(stderr, "There was an error parsing a drawing file."
          " Problem with char %c. Will ignore it.", cIn);
  return ' ';
}

bool Renderer::pcPointingToAddress(int adr) {
  bool executionHasntStarted = cpu.getCycle() == 0;
  if (executionHasntStarted) {
    return false;
  }
  return Util::getInt(cpu.getPc()) == adr;
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

char Renderer::getFormattedOutput(int i) {
  if (printer.getPrinterOutput().length() <= (unsigned) i) {
    return ' ';
  } else {
    return printer.getPrinterOutput().at(i);
  }
}

char getCharAt(int i, vector<vector<bool>>* matrix) {
  int j = i / WORD_SIZE;
  i = i % WORD_SIZE;
  return Util::getChar((*matrix).at(j).at(i));
}

char Renderer::getCodeBit(int i) {
  return getCharAt(i, &ram.state[CODE]);
}

char Renderer::getDataBit(int i) {
  return getCharAt(i, &ram.state[DATA]);
}

bool Renderer::machineActive() {
  bool executionHasntStarted = cpu.getCycle() == 0;
  bool executionEnded = Util::getInt(cpu.getPc()) == RAM_SIZE;
  return !(executionHasntStarted || executionEnded);
}