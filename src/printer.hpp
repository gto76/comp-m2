#ifndef PRINTER_H
#define PRINTER_H

#include <string>
#include <vector>

using namespace std;

class Printer {
  public:
    Printer(ProvidesOutput inputIn) 
        : input(inputIn) { }
    void run();
  private:
    // const bool interactiveMode;
    // const bool outputChars;
    const ProvidesOutput input;
    string output = "";
    string printerOutput;
    bool printerOutputUpdated = false;

    void print(vector<bool> sIn);
    void printEmptyLine();
    string getOutput();
    string getPrinterOutput();
    void clear();
    string renderPrinterOutput();
};

#endif