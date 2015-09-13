#ifndef PRINTER_H
#define PRINTER_H

#include <string>
#include <vector>

using namespace std;

class Printer {
  public:
    Printer(bool interactiveModeIn, bool outputCharsIn) 
        : interactiveMode(interactiveModeIn),
          outputChars(outputCharsIn) { }
    void print(vector<bool> sIn);
    void printEmptyLine();
    string getOutput();
    string getPrinterOutput();
    void clear();
  private:
    const bool interactiveMode;
    const bool outputChars;
    string output = "";
    string printerOutput;
    bool printerOutputUpdated = false;
    string renderPrinterOutput();
};

#endif