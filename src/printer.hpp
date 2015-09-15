#ifndef PRINTER_H
#define PRINTER_H

#include <string>
#include <vector>

#include "provides_output.hpp"

using namespace std;

class Printer {
  public:
    Printer(ProvidesOutput &inputIn) : input(inputIn) { }
    void run();
    string getPrinterOutput();

  private:
    ProvidesOutput &input;
    string output = "";
    string printerOutput;
    bool printerOutputUpdated = false;

    void print(vector<bool> sIn);
    void printEmptyLine();
    string getOutput();
    void clear();
    string renderPrinterOutput();
};

#endif