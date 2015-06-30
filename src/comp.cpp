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
#include "drawing.hpp"
#include "printer.hpp"
#include "ram.hpp"
#include "renderer.hpp"
#include "util.hpp"

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
vector<vector<bool>> savedRamInstructions;
vector<vector<bool>> savedRamData;

// Coordinates of first ram lightbulb in the ascii drawing.
tuple<int, int> instRamPosition = Util::getCoordinatesOfFirstOccurance(drawing, 'a');
tuple<int, int> dataRamPosition = Util::getCoordinatesOfFirstOccurance(drawing, 'b');

bool cursorAtInstRam = true;

//int cursorX = 0;
//int cursorY = 0;

// Selected bit with the cursor.
tuple<int, int> instCursorPosition = tuple<int, int>(0, 0);
tuple<int, int> dataCursorPosition = tuple<int, int>(0, 0);


void drawScreen() {
	string out = Renderer::renderState(printer, ram, cpu);
	buffer = Util::splitString(out);
	int i = 0;
	for (string line : buffer) {
		printString(line.c_str(), 0, i++);
	}
}

int getCursorPosition(int i) {
	if (cursorAtInstRam) {
		return get<i>(instCursorPosition);
	} else {
		return get<i>(dataCursorPosition);
	}
}

int getCursorX() {
	return getCursorPosition(0);
}

int getCursorY() {
	return getCursorPosition(1);
}

int getRamPosition(int i) {
	if (cursorAtInstRam) {
		return get<i>(instRamPosition);
	} else {
		return get<i>(dataRamPosition);
	}
}

int getRamPositionX() {
	return getRam(0);
}

int getRamPositionY() {
	return getRam(1);
}

int getCursorAbsoluteX() {
	getCursorX() + getRamPositionX();
}

int getCursorAbsoluteY() {
	getCursorY() + getRamPositionY();
}

char getCharUnderCursor() {
	return buffer.at(getCursorAbsoluteX()).
				  at(getCursorAbsoluteY());
}

void highlightCursor(bool highlight) {
	char c;
	try {
		c = getCharUnderCursor(); // buffer.at(cursorY+get<1>(instRamPosition))
				  //.at(cursorX+get<0>(instRamPosition));
	} catch (int e) {
		cout << "Cursor out of bounds. Exception Nr. " << e << '\n';
		return;
	}
	if (highlight) {
		printf("\e[%dm\e[%dm", 30, 47);
	}
	printCharXY(c, getCursorAbsoluteY(), //cursorX+get<0>(instRamPosition), 
				   getCursorAbsoluteX()); //cursorY+get<1>(instRamPosition));
	if (highlight) {
		printf("\e[%dm\e[%dm", 37, 40);
	}
	fflush(stdout);
}

bool getRamAt(int addr, int bitIndex) {
	if (cursorAtInstRam) {
		return ram.instructions.at(addr).at(bitIndex);
	} else {
		return ram.data.at(addr).at(bitIndex);
	}
}

void setRamAt(int addr, int bitIndex, bool value) {
	if (cursorAtInstRam) {
		ram.instructions.at(addr).at(bitIndex) = value;
	} else {
		ram.data.at(addr).at(bitIndex) = value;
	}
}

vector<bool> getByteRamAt(int addr) {
	if (cursorAtInstRam) {
		return ram.getInstruction(Util::getBoolNibb(addr));
	} else {
		return ram.getData(Util::getBoolNibb(addr));
	}
}

void setByteRamAt(int addr, vector<bool> value) {
	if (cursorAtInstRam) {
		ram.setInstruction(Util::getBoolNibb(addr), value);
	} else {
		ram.setData(Util::getBoolNibb(addr), value);
	}
}

void switchBitUnderCursor() {
	bool bitValue = getRamAt(getCursorY(), getCursorX()); // ram.instructions.at(cursorY).at(cursorX); 
	setRamAt(getCursorY(), getCursorX(), !bitValue); // ram.instructions.at(cursorY).at(cursorX) = !bitValue;
	// Only change char of the buffer, as to avoid screen redraw.
	buffer.at(getCursorAbsoluteY()).at(getCursorAbsoluteX()) = 
		Util::getChar(!bitValue);
}

void eraseByteUnderCursor() {
	setByteRamAt(getCursorY(), Util::getBoolByte(0));
	//ram.setInstruction(Util::getBoolNibb(cursorY), Util::getBoolByte(0));
	redrawScreen();
}

//TODO
bool switchBytesInRam(int index1, int index2) {
	if (index1 < 0 || index2 < 0 || index1 >= RAM_SIZE || index2 >= RAM_SIZE) {
		return false;
	}
	//vector<bool> addr1 = Util::getBoolNibb(index1);
	//vector<bool> addr2 = Util::getBoolNibb(index2);

	vector<bool> temp = getByteRamAt(index1);
	setByteRamAt(index1, getByteRamAt(index2));
	setByteRamAt(index2, temp);

	// vector<bool> temp = ram.getInstruction(addr1); 
	// ram.setInstruction(addr1, ram.getInstruction(addr2)); 
	// ram.setInstruction(addr2, temp); 
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

void moveCursorToOtherRam() { 
	cursorAtInstRam = !cursorAtInstRam;
	redrawScreen();
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
	savedRamInstructions = ram.instructions;
	savedRamData = ram.data;
	exec();
	// If 'esc' was pressed then it doesn't wait for keypress at the end.
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
			case 65:  // up
				if (cursorY > 0) {
					cursorY--;
				}
				break;
			case 66:  // down
				if (cursorY < RAM_SIZE-1) {
					cursorY++;
				}
				break;
			case 67:  // right
				if (cursorX < WORD_SIZE-1) {
					cursorX++;
				}
				break;
			case 68:  // left
				if (cursorX > 0) {
					cursorX--;
				}
				break;
			case 107:  // k
				moveByteUnderCursorUp();
				break;
			case 106:  // j
				moveByteUnderCursorDown();
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
				moveCursorToOtherRam(); 
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


