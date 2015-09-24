#ifndef OUTPUT_H
#define OUTPUT_H

#include <string>
#include <vector>

#include <signal.h>

using namespace std;

typedef void (*callback_function)(void);

// INIT
void setOutput(callback_function drawScreen, int width, int height);

// API
void redrawScreen();
void clearScreen();
void replaceLine(vector<string> s, int y);
void printString(vector<string> s, int x, int y);
void printChar(string c, int x, int y);

extern volatile sig_atomic_t screenResized;
 
#endif
