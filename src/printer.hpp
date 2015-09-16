#ifndef PRINTER_H
#define PRINTER_H

#include <string>
#include <vector>

#include "provides_output.hpp"

using namespace std;

class Printer {
  public:
    Printer(ProvidesOutput &inputIn, void (*printStateIn)(void),
            void (*sleepAndCheckForKeyIn)(void)) 
        : input(inputIn),
          printState(printStateIn),
          sleepAndCheckForKey(sleepAndCheckForKeyIn) { }
    void run();
    string getPrinterOutput();

  private:
    ProvidesOutput &input;
    void (*printState)(void);
    void (*sleepAndCheckForKey)(void);
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