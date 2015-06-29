#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <streambuf>
#include <tuple>
#include <cmath>

#include "const.hpp"
#include "util.hpp"
#include "printer.hpp"
#include "ram.hpp"
#include "cpu.hpp"
#include "renderer.hpp"
#include "drawing.hpp"

using namespace std;

extern "C" {
	typedef void (*callback_function)(void);
	void setEnvironment();
	void setOutput(callback_function drawScreen, int width, int height);
	void printCharXY(char c, int x, int y);
	void printString(const char s[], int x, int y);
	void redrawScreen();
	void resetEnvironment();
	extern volatile sig_atomic_t screenResized;
	extern volatile sig_atomic_t pleaseExit;
}

bool interactivieMode;

Printer printer;
Ram ram;
Cpu cpu;

// Selected bit with the cursor
int cursorX = 0;
int cursorY = 0;

// Graphic representation of the computer state
vector<string> buffer;

// Saved state of a ram
vector<vector<bool>> savedRamInstructions;
vector<vector<bool>> savedRamData;

// Offset of first ram lightbulb in the asci drawing
int ramX;
int ramY;

int executionCounter = 0;
bool executionCanceled = false;

void setRamOffset() {
	tuple<int,int> t = Util::getCoordinatesOfFirstOccurance(drawing, 'a') ;
	ramX = get<0>(t);
	ramY = get<1>(t);
}

void drawScreen() {
	string out = Renderer::renderState(printer, ram, cpu);
	buffer = Util::splitString(out);
	int i = 0;
	for (string line : buffer) {
		printString(line.c_str(), 0, i++);
	}
}

void highlightCursor(bool highlight) {
	char c;
	try {
		c = buffer.at(cursorY+ramY).at(cursorX+ramX);
	} catch (int e) {
		cout << "Cursor out of bounds. Exception Nr. " << e << '\n';
		return;
	}
	if (highlight) {
		printf("\e[%dm\e[%dm", 30, 47);
	}
	printCharXY(c, cursorX+ramX, cursorY+ramY);
	if (highlight) {
		printf("\e[%dm\e[%dm", 37, 40);
	}
	fflush(stdout);
}


void switchBitUnderCursor() {
	bool newBitValue = !ram.instructions.at(cursorY).at(cursorX); // TODO
	ram.instructions.at(cursorY).at(cursorX) = newBitValue;
	// Only change char of the buffer, as to avoid screen redraw.
	buffer.at(cursorY+ramY).at(cursorX+ramX) = Util::getChar(newBitValue);
}

void eraseByteUnderCursor() {
	ram.setInstruction(Util::getBoolNibb(cursorY), Util::getBoolByte(0)); // TODO
	redrawScreen();
}

//TODO
bool switchBytesInRam(int index1, int index2) {
	if (index1 < 0 || index2 < 0 || index1 >= RAM_SIZE || index2 >= RAM_SIZE) {
		return false;
	}
	vector<bool> addr1 = Util::getBoolNibb(index1);
	vector<bool> addr2 = Util::getBoolNibb(index2);
	vector<bool> temp = ram.getInstruction(addr1); // TODO
	ram.setInstruction(addr1, ram.getInstruction(addr2)); // TODO
	ram.setInstruction(addr2, temp); // TODO
	return true;
}

void moveByteFor(int delta) {
	bool failed = !switchBytesInRam(cursorY, cursorY+delta);
	if (failed) {
		return;
	}
	cursorY += delta;
	redrawScreen();
}

void moveByteUnderCursorUp() {
	moveByteFor(-1);
}

void moveByteUnderCursorDown() {
	moveByteFor(1);
}

char readStdin(bool drawCursor) {
	char c = 0;
	errno = 0;
	ssize_t num = read(0, &c, 1);
	if (num == -1 && errno == EINTR) {
		// Exit if ctrl-c was pressed
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
 * Run every cycle.
 */
void sleepAndCheckForKey() {
	usleep(FQ*1000);

	// Exit if ctrl-c was pressed
	if (pleaseExit) {
		exit(0);
	}
	
	// Pauses execution if a key was hit, and waits for another key hit
	int keyCode = Util::getKey();
	if (keyCode) {
		// If escape was pressed
		if (keyCode == 27) {
			executionCanceled = true;
			return;
		}
		// Press key to continue
		keyCode = readStdin(false);
		// If the key was esc
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
		if(!shouldContinue) {
			return;
		}
		if (interactivieMode) {
			sleepAndCheckForKey();
		}
	}
}

/*
 * Saves the state of the ram and starts the execution of a program.
 * When execution stops, due to it reaching last address or user pressing 'esc',
 * it loads back the saved state of the ram, and resets the cpu.
 */
void run() { 
	if (executionCounter > 0) {
		printer.printEmptyLine();
	}
	savedRamInstructions = ram.instructions;
	savedRamData = ram.data;
	exec();
	// if 'esc' was pressed then it doesn't wait for keypress at the end
	if (executionCanceled) {
		executionCanceled = false;
	} else {
		readStdin(false);
	}
	ram = Ram();
	ram.instructions = savedRamInstructions;
	ram.data = savedRamData;
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
			case 65: // up
				if (cursorY > 0) {
					cursorY--;
				}
				break;
			case 66: // down
				if (cursorY < RAM_SIZE-1) {
					cursorY++;
				}
				break;
			case 67: // right
				if (cursorX < WORD_SIZE-1) {
					cursorX++;
				}
				break;
			case 68: // left
				if (cursorX > 0) {
					cursorX--;
				}
				break;
			case 107: // k
				moveByteUnderCursorUp();
				break;
			case 106: // j
				moveByteUnderCursorDown();
				break;
			case 115: // s
				saveRamToFile();
				break;
			case 32: // space
			case 102: // f
				switchBitUnderCursor();
				break;
			case 100: // d
				eraseByteUnderCursor();
				break;
			case 10: // enter
				run();
				break;
		}
		highlightCursor(true);
	}
}



/*
 * Initializes 'output.c' by sending dimensions of a 'drawing' and a 'drawScreen'  
 * callback function, that output.c will use on every screen redraw.
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
	ram.instructions.at(address).at(bitIndex) = bitValue;
}

void writeDataBitToRam(int address, int bitIndex, bool bitValue) {
	ram.data.at(address).at(bitIndex) = bitValue;
}

void writeLineToRam(string line, int address) {
	int bitIndex = 0;
	for (char c : line) {
		if (address < WORD_SIZE) { 
			writeInstructionBitToRam(address, bitIndex, getBool(c));
		} else {
			writeDataBitToRam(address, bitIndex, getBool(c));
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

/*
 * MAIN
 */

void startInteractiveMode() {
	setRamOffset();
	setEnvironment();
	prepareOutput();
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


