#ifndef OUTPUT_H
#define OUTPUT_H

#include <string>
#include <vector>

#include <signal.h>

using namespace std;

typedef void (*callback_function)(void);

void initOutput(callback_function drawScreen, int width, int height);
void redrawScreen();
void clearScreen();
void replaceBufferLine(vector<string> s, int y);

extern volatile sig_atomic_t screenResized;
 
#endif
