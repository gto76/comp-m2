#ifndef OUTPUT_H
#define OUTPUT_H

//#include "const.hpp"

#include <signal.h>

using namespace std;

typedef void (*callback_function)(void);
void setOutput(callback_function drawScreen, int width, int height);
void printCharXY(char c, int x, int y);
void printString(const char s[], int x, int y);
void redrawScreen();
extern volatile sig_atomic_t screenResized;

 /*
	private:		
		int getAbsoluteX(int x);
		int getAbsoluteY(int y);
		int getAbsoluteCoordinate(int value, int console, int track);
		int coordinatesOutOfBounds(int x, int y);
		void clearScreen(void);
		void registerSigWinChCatcher(void);
		void sigWinChCatcher(int signum);
		void updateConsoleSize(void);
		void copyArray(char dest[], const char src[], int width);
*/
#endif
