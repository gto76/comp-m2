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
vector<vector<string>> Renderer::renderState(const Printer &printerIn, const Ram &ramIn, 
                             const Cpu &cpuIn, const Cursor &cursorIn) {
  Renderer instance(printerIn, ramIn, cpuIn, cursorIn);
  vector<vector<string>> out;
  for (vector<string> line : Util::splitIntoLines(drawing)) {
    out.push_back(instance.insertActualValues(line)); // + "\n";
  }
  //out.erase(out.end() - 1);
  return out;
}

vector<string> Renderer::insertActualValues(vector<string> lineIn) {
  // vector<bool> highlightedChars = getHighlightedLocations(lineIn);
  // vector<bool> boldChars = getBoldLocations(lineIn);
  // vector<bool> dimChars = getDimLocations(lineIn);
  vector<string> lineOut;
  for (string cIn : lineIn) {
    string sOut = "";
    // Regex: [0-9a-z]
    bool charIsALightbulb = ALL_INDICATORS.count(cIn);
    // bool charIsALightbulb = (cIn >= 'a' && cIn <= 'z') || 
    //                         (cIn >= '0' && cIn <= '9');
    if (charIsALightbulb) {
      sOut += getLightbulb(cIn);
    } else {
      sOut += cIn;
    }
    lineOut.push_back(sOut);
  }
  return lineOut;
  // return insertEscSeqences(lineOut, highlightedChars, boldChars, dimChars);
}

string Renderer::insertEscSeqences(string lineWithoutEscapeSeqences,
                                   vector<bool> highlightedChars,
                                   vector<bool> boldChars,
                                   vector<bool> dimChars) {
  string lineOut = "";
  bool insideHighlightBlock = false;
  bool insideBoldBlock = false;
  bool insideDimBlock = false;
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
    firstCharInsideBlock = dimChars[i] && !insideDimBlock;
    if (firstCharInsideBlock) {
      lineOut += DIM_ESC;
      insideDimBlock = true;
    }
    firstCharOutsideBlock = !dimChars[i] && insideDimBlock;
    if (firstCharOutsideBlock) {
      lineOut += DIM_END_ESC;
      insideDimBlock = false;
    }
    lineOut += lineWithoutEscapeSeqences[i];
  }
  return lineOut;
}

// NOT USED:
// TODO derepetition
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

// vector<bool> Renderer::getHighlightedLocations(string lineIn) {
//   vector<bool> highlightedLocations (lineIn.length(), false);
//   highlightedLocations = highlightPc(highlightedLocations, lineIn);
//   highlightedLocations = highlightCursor(highlightedLocations, lineIn);
//   Instruction *inst = getInstruction();
//   if (inst == NULL) {
//     return highlightPointingInstructions(highlightedLocations, lineIn);
//   }
//   highlightedLocations = highlightOperators(highlightedLocations, lineIn, inst);
//   if (inst->adr.space == CODE) {
//     highlightedLocations = highlightCodeWord(highlightedLocations, lineIn, 
//                                             inst);
//   } else if (inst->adr.space == DATA) {
//     highlightedLocations = highlightDataWord(highlightedLocations, lineIn,
//                                              inst);
//   }
//   return highlightedLocations;
// }

// vector<bool> Renderer::highlightPc(vector<bool> highlightedLocations,
//                                        string lineIn) {
//   if (!machineActive()) {
//     return highlightedLocations;
//   }
//   for (size_t i = 0; i < lineIn.length(); i++) {
//     if (lineIn[i] == CODE_ADR_INDICATOR) {
//       int index = switchIndex[CODE_ADR_INDICATOR];
//       highlightedLocations[i] = pcPointingToAddress(index);;
//     }
//   }
//   return highlightedLocations;
// }

// /////////

// vector<bool> Renderer::highlightCursor(vector<bool> highlightedLocations,
//                                        string lineIn) {
//   if (machineActive()) {
//     return highlightedLocations;
//   }
//   if (cursor.getAddressSpace() == CODE) {
//     highlightedLocations = findCursor(highlightedLocations, lineIn, CODE_INDICATOR);
//   } else if (cursor.getAddressSpace() == DATA) {
//     highlightedLocations = findCursor(highlightedLocations, lineIn, DATA_INDICATOR);
//   }
//   return highlightedLocations;
// }

// vector<bool> Renderer::findCursor(vector<bool> highlightedLocations,
//                                   string lineIn, string c) {
//   int indexDelta = 0;
//   for (size_t i = 0; i < lineIn.length(); i++) {
//     if (lineIn[i] == c) {
//       int lightbulbIndex = switchIndex[c] + indexDelta++;
//       if (cursor.getAbsoluteBitIndex() == lightbulbIndex) {
//         highlightedLocations[i] = true;
//       }
//     }
//   }
//   return highlightedLocations;
// }

// ////////////

// vector<bool> Renderer::highlightPointingInstructions(
//     vector<bool> highlightedLocations, string lineIn) {
//   set<int> *pointingInstructions = getIndexesOfPointingInstructions();
//   for (size_t i = 0; i < lineIn.length(); i++) {
//     if (lineIn[i] == CODE_INDICATOR) {
//       int addressValue = switchIndex[CODE_INDICATOR] / WORD_SIZE;
//       if (pointingInstructions->count(addressValue)) {
//         highlightedLocations[i] = true;
//       }
//     }
//   }
//   return highlightedLocations;
// }

// ///////////////////

// vector<bool> Renderer::highlightOperators(vector<bool> highlightedLocations,
//                                          string lineIn, Instruction *inst) {
//   string exclude = "";
//   if (inst->isLogic()) {
//     exclude = LOGIC_OPS_INDICATOR[min(inst->logicIndex, 8)];
//   }
//   if (inst->index == INC_DEC_OPS_INDEX) {
//     if (inst->logicIndex <= 7) {
//       exclude = "INC";
//     } else {
//       exclude = "DEC";
//     }
//   }
//   string label = " " + inst->label;
//   label.append(11 - inst->label.length(), ' ');  
//   return highlightLabel(highlightedLocations, lineIn, label, exclude);
// }

// /////////////

// vector<bool> Renderer::highlightCodeWord(vector<bool> highlightedLocations,
//                                         string lineIn, Instruction *inst) {
//   if (inst->adr.val == LAST_ADDRESS) {
//     return highlightLabel(highlightedLocations, lineIn,
//                           LAST_CODE_ADDR_LABEL, "");
//   }
//   return highlightWords(highlightedLocations, lineIn, CODE_INDICATOR, CODE);
// }

// vector<bool> Renderer::highlightDataWord(vector<bool> highlightedLocations,
//                                         string lineIn, Instruction *inst) {
//   if (inst->adr.val == LAST_ADDRESS) {
//     return highlightLabel(highlightedLocations, lineIn, LAST_DATA_ADDR_LABEL,
//                           "");
//   }
//   return highlightWords(highlightedLocations, lineIn, DATA_INDICATOR, DATA);
// }

// vector<bool> Renderer::highlightWords(vector<bool> highlightedLocations,
//                                      string lineIn, string indicator,
//                                      AddrSpace addrSpace) {
//   for (size_t i = 0; i < lineIn.length(); i++) {
//     if (lineIn[i] == indicator) {
//       int addressValue = switchIndex[indicator] / WORD_SIZE;
//       Address adr = Address(addrSpace, Util::getBoolNibb(addressValue));
//       if (instructionPointingToAddress(adr)) {
//         highlightedLocations[i] = true;
//       }
//     }
//   }
//   return highlightedLocations;
// }

// ///////////

// vector<bool> Renderer::highlightLabel(vector<bool> highlightedLocations,
//                                      string lineIn, string label,
//                                      string exclude) {
//   size_t labelPosition = lineIn.find(label);
//   if (labelPosition == string::npos) {
//     return highlightedLocations;
//   }
//   size_t excludePosition = numeric_limits<size_t>::max();
//   if (exclude != "") {
//     excludePosition = label.find(exclude);
//   }
//   for (size_t i = labelPosition; i < labelPosition + label.length(); i++) {
//     bool highlight = (labelPosition != string::npos) &&
//                      (i < labelPosition + excludePosition ||
//                      i >= labelPosition + excludePosition + exclude.length());
//     if (highlight) {
//       highlightedLocations[i] = true;
//     }
//   }
//   return highlightedLocations;
// }

// //////////////////////////
// /// GET BOLD LOCATIONS ///
// //////////////////////////

// vector<bool> Renderer::getBoldLocations(string lineIn) {
//   vector<bool> boldLocations (lineIn.length(), false);
//   // boldLocations = enboldenReferencedCodeIndicators(boldLocations, lineIn);
//   // boldLocations = enboldenReferencedDataIndicators(boldLocations, lineIn);
//   return boldLocations;
// }

// vector<bool> Renderer::enboldenReferencedCodeIndicators(
//     vector<bool> boldLocations, string lineIn) {
//   return enboldenIndicators(boldLocations, lineIn, CODE_ADR_INDICATOR, CODE);
// }

// vector<bool> Renderer::enboldenReferencedDataIndicators(
//     vector<bool> boldLocations, string lineIn) {
//   return enboldenIndicators(boldLocations, lineIn, DATA_ADR_INDICATOR, DATA);
// }

// // // TODO COMBINE WITH HIGHLIGHT WORDS (pass function as pointer)
// vector<bool> Renderer::enboldenIndicators(vector<bool> boldLocations,
//                                           string lineIn, string indicator,
//                                           AddrSpace addrSpace) {
//   for (size_t i = 0; i < lineIn.length(); i++) {
//     if (lineIn[i] == indicator) {
//       int addressValue = switchIndex[indicator]; // TODO out of loop
//       Address adr = Address(addrSpace, Util::getBoolNibb(addressValue)); // TODO out of loop
//       if (isAddressReferenced(adr)) { // out
//         boldLocations[i] = true; // out
//       }
//     }
//   }
//   return boldLocations;
// }

// /////////////////////////
// /// GET DIM LOCATIONS ///
// /////////////////////////

// vector<bool> Renderer::getDimLocations(string lineIn) {
//   vector<bool> dimLocations (lineIn.length(), false);
//   // dimLocations = dimUnreferencedCodeIndicators(dimLocations, lineIn);
//   // dimLocations = dimUnreferencedDataIndicators(dimLocations, lineIn);
//   return dimLocations;
// }

// vector<bool> Renderer::dimUnreferencedCodeIndicators(
//     vector<bool> dimLocations, string lineIn) {
//   return dimIndicators(dimLocations, lineIn, CODE_ADR_INDICATOR, CODE);
// }

// vector<bool> Renderer::dimUnreferencedDataIndicators(
//     vector<bool> dimLocations, string lineIn) {
//   return dimIndicators(dimLocations, lineIn, DATA_ADR_INDICATOR, DATA);
// }

// // // TODO COMBINE WITH HIGHLIGHT WORDS (pass function as pointer)
// vector<bool> Renderer::dimIndicators(vector<bool> dimLocations,
//                                           string lineIn, string indicator,
//                                           AddrSpace addrSpace) {
//   for (size_t i = 0; i < lineIn.length(); i++) {
//     if (lineIn[i] == indicator) {
//       int addressValue = switchIndex[indicator]; // TODO out of loop
//       Address adr = Address(addrSpace, Util::getBoolNibb(addressValue)); // TODO out of loop
//       if (!isAddressReferenced(adr)) { // out
//         dimLocations[i] = true; // out
//       }
//     }
//   }
//   return dimLocations;
// }

// NOT USED:

// vector<bool> Renderer::enboldenCodeWords(vector<bool> boldLocations,
//                                          string lineIn) {
//   // TODO: STOP
//   // TODO: g
//   return enboldenWords(boldLocations, lineIn, 'a', CODE);
// }

// vector<bool> Renderer::enboldenDataWords(vector<bool> boldLocations,
//                                          string lineIn) {
//   // TODO: IN/OUT
//   // TODO: v
//   return enboldenWords(boldLocations, lineIn, 'b', DATA);
// }

// // TODO COMBINE WITH HIGHLIGHT WORDS (pass function as pointer)
// vector<bool> Renderer::enboldenWords(vector<bool> boldLocations,
//                                      string lineIn, char indicator,
//                                      AddrSpace addrSpace) {
//   for (size_t i = 0; i < lineIn.length(); i++) {
//     if (lineIn[i] == indicator) {
//       int addressValue = switchIndex[indicator] / WORD_SIZE; // TODO out of loop
//       Address adr = Address(addrSpace, Util::getBoolNibb(addressValue)); // TODO out of loop
//       if (isAddressReferenced(adr)) { // out
//         boldLocations[i] = true; // out
//       }
//     }
//   }
//   return boldLocations;
// }


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

  // switch (cIn) {
  //   case CODE_INDICATOR:
  //     return getCodeBit(i);
  //   case DATA_INDICATOR:
  //     return getDataBit(i);
  //   // case "p":
  //   //   return Util::getChar(pcPointingToAddress(i));
  //   // case "d": {
  //   //   Address adr = Address(CODE, Util::getBoolNibb(i));
  //   //   return Util::getChar(instructionPointingToAddress(adr));
  //   // }
  //   // case "s": {
  //   //   Address adr = Address(DATA, Util::getBoolNibb(i));
  //   //   return Util::getChar(instructionPointingToAddress(adr));
  //   // }
  //   case REGISTER_INDICATOR:
  //     return  Util::getChar(cpu.getRegister().at(i));
  //   // case "i":
  //   //   return Util::getChar(instructionHasId(i));
  //   case CODE_ADR_INDICATOR:
  //     return getLocationName(CODE, i);
  //   case DATA_ADR_INDICATOR:
  //     return getLocationName(DATA, i);
  //   case OUTPUT_INDICATOR:
  //     return getFormattedOutput(i);
  // }
  // fprintf(stderr, "There was an error parsing a drawing file."
  //         " Problem with char %s. Will ignore it.", cIn);
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
  return "-";
  // char name = ram.names[addrSpace][index];
  // if (name == DEFAULT_VAR_NAME) {
  //   Address indicatorsAddress = Address(addrSpace, Util::getBoolNibb(index));
  //   if (isAddressReferenced(indicatorsAddress)) {
  //     return '*';
  //   } else {
  //     return DEFAULT_VAR_NAME;
  //   }
  // } else {
  //   return name;
  // }
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
      Instruction inst = Instruction(word, cpu.getRegister(), ram); // TODO:
      // Suspition that some register values might not be right...
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