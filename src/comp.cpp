#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <streambuf>
#include <string>
#include <tuple>
#include <vector>

#include "const.hpp"
#include "cpu.hpp"
#include "cursor.hpp"
#include "drawing.hpp"
#include "printer.hpp"
#include "ram.hpp"
#include "renderer.hpp"
#include "util.hpp"
#include "output.hpp"

using namespace std;

extern "C" {
	extern volatile sig_atomic_t pleaseExit;
	void setEnvironment();
	void resetEnvironment();
}

//////////////////////////
////////// VARS //////////
//////////////////////////

// Two global variables.
bool interactivieMode;
bool executionCanceled = false;

Printer printer;
Ram ram;
Cpu cpu;

// Graphic representation of the computer state.
vector<string> buffer;

int executionCounter = 0;

// Saved state of a ram.
map<AddrSpace, vector<vector<bool>>> savedRamState;

// Class for keeping track of and moving around cursor.
Cursor cursor;

//////////////////////////
/////// FUNCTIONS ////////
//////////////////////////

void drawScreen() {
	string out = Renderer::renderState(printer, ram, cpu);
	buffer = Util::splitString(out);
	int i = 0;
	for (string line : buffer) {
		printString(line.c_str(), 0, i++);
	}
}

char getCharUnderCursor() {
	return buffer.at(cursor.getY()).at(cursor.getX());
}

void highlightCursor(bool highlight) {
	char c;
	try {
		c = getCharUnderCursor();
		//fprintf(stderr, "Char under cursor %c, y %d, x %d highlight %d",
		//		c, cursor.getY(), cursor.getX(), highlight);
	} catch (int e) {
		cout << "Cursor out of bounds. Exception Nr. " << e << '\n';
		return;
	}
	if (highlight) {
		printf("\e[%dm\e[%dm", 30, 47);
	}
	printCharImediately(c, cursor.getX(), cursor.getY());
	if (highlight) {
		printf("\e[%dm\e[%dm", 37, 40);
	}
	fflush(stdout);
}

void switchBitUnderCursor() {
	bool bitValue = cursor.getBit(); 
	cursor.setBit(!bitValue);
	buffer.at(cursor.getY()).at(cursor.getX()) = 
		Util::getChar(!bitValue);
}

void eraseByteUnderCursor() {
	cursor.setWord(Util::getBoolByte(0));
	redrawScreen();
}

char readStdin(bool drawCursor) {
	char c = 0;
	errno = 0;
	ssize_t num = read(0, &c, 1);
	if (num == -1 && errno == EINTR) {
		// Exits if ctrl-c was pressed.
		if (pleaseExit) {
			exit(0);
		}
		redrawScreen();
		if (drawCursor) {
			highlightCursor(true);
		}
		return readStdin(drawCursor);
	}
	return c;
}

/*
 * Runs every cycle.
 */
void sleepAndCheckForKey() {
	usleep(FQ*1000);
	// Exit if ctrl-c was pressed.
	if (pleaseExit) {
		exit(0);
	}
	// Pauses execution if a key was hit, and waits for another key hit.
	if (int keyCode = Util::getKey()) {
		// If escape was pressed.
		if (keyCode == 27) {
			executionCanceled = true;
			return;
		}
		// Press key to continue.
		keyCode = readStdin(false);
		// If the key was escape.
		if (keyCode == 27) {
			executionCanceled = true;
		}
	}
}

void exec() {
	while(!executionCanceled) {
		bool shouldContinue = cpu.step();
		if (interactivieMode) {
			redrawScreen();
		}
		if (!shouldContinue) {
			return;
		}
		if (interactivieMode) {
			sleepAndCheckForKey();
		}
	}
}

/*
 * Saves the state of the ram and starts the execution of a program.
 * When execution stops, due to it reaching last address or user pressing 
 * 'esc', it loads back the saved state of the ram, and resets the cpu.
 */
void run() { 
	if (executionCounter > 0) {
		printer.printEmptyLine();
	}
	savedRamState = ram.state;
	exec();
	// If 'esc' was pressed then it doesn't wait for keypress at the end.
	if (executionCanceled) {
		executionCanceled = false;
	} else {
		readStdin(false);
	}
	ram = Ram();
	ram.state = savedRamState;
	cpu = Cpu();
	redrawScreen();
	executionCounter++;
}

string getFreeFileName() {
	int i = 0;
	while (Util::fileExists(SAVE_FILE_NAME + to_string(++i)));
	return SAVE_FILE_NAME + to_string(i);
}

void saveRamToFile() {
	string fileName = getFreeFileName();
	ofstream fileStream(fileName);
    fileStream << ram.getString();
    fileStream.close();
}

void userInput() {
	while(1) {
		char c = readStdin(true);
		highlightCursor(false);

		switch (c) {
			case 65:  // up
				cursor.decreaseY();
				break;
			case 66:  // down
				cursor.increaseY();
				break;
			case 67:  // right
				cursor.increaseX();
				break;
			case 68:  // left
				cursor.decreaseX();
				break;
			case 107:  // k
				cursor.moveByteUp();
				redrawScreen();
				break;
			case 106:  // j
				cursor.moveByteDown();
				redrawScreen();
				break;
			case 115:  // s
				saveRamToFile();
				break;
			case 32:  // space
			case 102:  // f
				switchBitUnderCursor();
				break;
			case 100:  // d
				eraseByteUnderCursor();
				break;
			case 9:  // tab
				cursor.switchAddressSpace();
				redrawScreen();
				break;
			case 10:  // enter
				run();
				break;
		}
		highlightCursor(true);
	}
}

/*
 * Initializes 'output.c' by sending dimensions of a 'drawing' and 
 * a 'drawScreen' callback function, that output.c will use on every 
 * screen redraw.
 */
void prepareOutput() {
	size_t drawingWidth = 0;
	size_t drawingHeight = 0;
	for (string line : Util::splitString(drawing)) {
		drawingWidth = std::max(drawingWidth, line.length());
		drawingHeight++;
	}
	setOutput(&drawScreen, drawingWidth, drawingHeight);
}

bool getBool(char c) {
	return c == '*';
}

void writeInstructionBitToRam(int address, int bitIndex, bool bitValue) {
	ram.state[CODE].at(address).at(bitIndex) = bitValue;
}

void writeDataBitToRam(int address, int bitIndex, bool bitValue) {
	ram.state[DATA].at(address).at(bitIndex) = bitValue;
}

void writeLineToRam(string line, int address) {
	int bitIndex = 0;
	for (char c : line) {
		if (address < RAM_SIZE) { 
			writeInstructionBitToRam(address, bitIndex, getBool(c));
		} else {
			writeDataBitToRam(address-RAM_SIZE, bitIndex, getBool(c));
		}
		if (++bitIndex >= WORD_SIZE) {
			return;
		}
	}
}

void loadRamFromFileStream(ifstream* fileStream) {
	int address = 0;
	while (!fileStream->eof()) {
		string line;
    	getline(*fileStream, line);
    	bool lineEmptyOrAComment = line.empty() || line[0] == '#';
    	if (lineEmptyOrAComment) {
    		continue;
    	}
    	writeLineToRam(line, address);
		if (++address >= 2*RAM_SIZE) {
			return;
		}
  	} 
}

void checkIfInputIsPiped() {
	interactivieMode = !Util::inputIsPiped();
}

void loadRamIfFileSpecified(int argc, const char* argv[]) {
	if (argc <= 1) {
		return;
	}
   	ifstream fileStream;    
	fileStream.open(argv[1]);   
	if (fileStream.fail()) {
    	fprintf(stderr, "Invalid filename '%s'. Aborting ram load.", argv[1]);
   	} else {
   		loadRamFromFileStream(&fileStream);
      	fileStream.close();  
   	}
}

//////////////////////////
////////// MAIN //////////
//////////////////////////

void startInteractiveMode() {
	setEnvironment();
	prepareOutput();
	clearScreen();
	clearScreen();
	redrawScreen();
	highlightCursor(true);
	userInput();
}

int main(int argc, const char* argv[]) {
	srand(time(NULL));
	checkIfInputIsPiped();
	loadRamIfFileSpecified(argc, argv);
	if (interactivieMode) {
		startInteractiveMode();
	} else {
		exec();
	}
}


