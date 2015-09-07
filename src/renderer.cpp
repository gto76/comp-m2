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

// enum Operator {
//   READ,
//   WRITE
//}

// string lineOutWithoutEscapeSeqences
// vector<bool> characterBoldOrNot

// line
  // check if contains inst.op
    // set characterBoldOrNot[location..location+inst.op.size] to true
  //...

// string Renderer::insertBoldEscSeqences(string lineOutWithoutEscapeSeqences, vector<bool> characterBoldOrNot) {
// string lineOut
// for c in characterBoldOrNot
//   if first bold
//     line out = bold start escape sequence
//   if first nonbold / last char
//     line out = bold end
//   lineOut += c
// }

string Renderer::insertActualValues(string lineIn) {
  string lineOut;
  bool lineContainsLogicOps = lineIn.find(LOGIC_OPS_INDICATOR) != string::npos;
  bool lineContainsIncDecOps = lineIn.find(INC_DEC_OPS_INDICATOR) != string::npos;
  for (char cIn : lineIn) {
    string sOut = "";
    // Regex: [0-9a-z]
    bool charIsALightbulb = (cIn >= 'a' && cIn <= 'z') || 
                            (cIn >= '0' && cIn <= '9');
    if (charIsALightbulb) {
      sOut.push_back(getLightbulb(cIn));
    } else if (lineContainsLogicOps) {
      sOut += setCharToBoldIfLogicOp(cIn);
    } else if (lineContainsIncDecOps) {
      sOut += setCharToBoldIfIncDecOp(cIn);
    } else {
      sOut.push_back(cIn);
    }
    lineOut += sOut;
  }
  return lineOut;
}

string Renderer::setCharToBoldIfLogicOp(char cIn) {
  size_t positionOfCharInLogicLabel = LOGIC_OPS_INDICATOR.find(cIn);
  bool charIsNotALogicOp = positionOfCharInLogicLabel == string::npos;
  if (charIsNotALogicOp) {
    return string(1, cIn);
  }
  Instruction *inst = getInstruction();
  if (inst == NULL) {
    return string(1, cIn);
  }
  return highlightLogicIndicator(cIn, *inst, positionOfCharInLogicLabel);

  // if (machineActive()) {
  //   Instruction inst = cpu.getInstruction();
  //   return highlightLogicIndicator(cIn, inst, positionOfCharInLogicLabel);
  // }
  // int cursorOnData = cursor.getAddressSpace() == DATA;
  // if (cursorOnData) {
  //   return string(1, cIn);
  // }
  // Instruction inst = getCursorsInstruction();
  // return highlightLogicIndicator(cIn, inst, positionOfCharInLogicLabel);
}

string Renderer::highlightLogicIndicator(char cIn, Instruction inst, 
                                  size_t positionOfCharInLogicLabel) {
  bool isLogicOp = inst.index == LOGIC_OPS_INDEX;
  if (isLogicOp) {
    bool indicatingInstruction =
        ((unsigned)inst.logicIndex == positionOfCharInLogicLabel ||
        (inst.logicIndex > 7 && positionOfCharInLogicLabel == 8));
    if (indicatingInstruction) {
      return "\033[1m" + string(1, cIn) + "\033[0;37m";
    }
  }
  return string(1, cIn);
}

string Renderer::setCharToBoldIfIncDecOp(char cIn) {
  size_t positionOfCharInIncDecLabel = INC_DEC_OPS_INDICATOR.find(cIn);
  bool charIsNotALogicOp = positionOfCharInIncDecLabel == string::npos;
  if (charIsNotALogicOp) {
    return string(1, cIn);
  }
  Instruction *inst = getInstruction();
  if (inst == NULL) {
    return string(1, cIn);
  }
  return highlightIncOrDec(cIn, *inst);

  // if (machineActive()) {
  //   return highlightIncOrDec(cIn, cpu.getInstruction());
  // } else {
  //   int cursorOnData = cursor.getAddressSpace() == DATA;
  //   if (cursorOnData) {
  //     return string(1, cIn);
  //   }
  //   return highlightIncOrDec(cIn, getCursorsInstruction());
  // }
}

string Renderer::highlightIncOrDec(char cIn, Instruction inst) {
  bool notAnIncDecInstruction = inst.index != INC_DEC_OPS_INDEX;
  if (notAnIncDecInstruction) {
    return string(1, cIn);
  }
  bool isInc = inst.logicIndex <= 7;
  if (isInc) {
    if (cIn == 'I') {
      return "\033[1m" + string(1, cIn);
    }
  } else {
    if (cIn == 'D') {
      return "\033[1m" + string(1, cIn);
    }
  }
  if (cIn == 'C') {
    return string(1, cIn) + "\033[0;37m";
  }
  return string(1, cIn);
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

  // // if (machineActive()) {
  // //   Instruction inst = cpu.getInstruction();
  // //   return inst.adr == adr;
  // // }
  // // int cursorOnData = cursor.getAddressSpace() == DATA;
  // // if (cursorOnData) {
  // //   return false;
  // // }
  // // Instruction inst = getCursorsInstruction();
  // return inst.adr == adr;
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

// Instruction& Renderer::getCursorsInstruction() {
//   if (cursorsInstruction.size() == 0) {
//     cursorsInstruction.push_back(Instruction(cursor.getWord(), EMPTY_WORD, ram));
//   }
//   return cursorsInstruction[0];
// }

bool Renderer::instructionHasId(int id) {
  Instruction *inst = getInstruction();
  if (inst == NULL) {
    return false;
  }
  return inst->index == id;

  // if (machineActive()) {
  //   return cpu.getInstructionCodeInt() == id;
  // }
  // int cursorOnData = cursor.getAddressSpace() == DATA;
  // if (cursorOnData) {
  //   return false;
  // }
  // Instruction inst = getCursorsInstruction();
  // return inst.index == id;
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