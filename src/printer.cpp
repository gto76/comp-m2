#include "printer.hpp"

#include <algorithm>
#include <iostream>
#include <vector>

#include "comp.hpp"
#include "util.hpp"

using namespace std;

/*
 * If in interactive mode, then prints word together with decimal
 * representation on the printer. If not in interactive mode,
 * then prints to the stdout. If stdout is a pipe, then it doesen't
 * add the decimal representation.
 */
void Printer::print(vector<bool> wordIn) {
  if (interactivieMode) {
    output += Util::getStringWithFormatedInt(wordIn);
    printerOutputUpdated = false;
  } else {
    if (Util::outputIsPiped()) {
      cout << Util::getString(wordIn) + "\n";
    } else {
      if (outputChars) {
        char c = (char)Util::getInt(wordIn);
        //cerr << "outputing: " << c << "\n";
        cout << c;
      } else {
        cout << Util::getStringWithFormatedInt(wordIn);
      }
    }
  }
}

void Printer::printEmptyLine() {
  output += "            \n";
}

string Printer::getOutput() {
  return output;
}

string Printer::getPrinterOutput() {
  if (!printerOutputUpdated) {
    printerOutput = renderPrinterOutput();
    printerOutputUpdated = true;
  }
  return printerOutput;
}

void Printer::clear() {
  output = "";
  printerOutputUpdated = false;
}

string Printer::renderPrinterOutput() {
  if (output.length() <= 0) {
    return "|0|______________|0|";
  }
  vector<string> lines = Util::splitString(output);
  reverse(lines.begin(), lines.end());
  vector<string>  outputLines;
  for (string line : lines) {
    outputLines.push_back("|0| " + line + " |0|");
  }
  outputLines.push_back("|0|______________|0|");
  return Util::makeString(outputLines);
}
