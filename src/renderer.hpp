#ifndef RENDERER_H
#define RENDERER_H

#include <map>
#include <string>

#include "cpu.hpp"
#include "printer.hpp"
#include "ram.hpp"
#include "cursor.hpp"

using namespace std;

class Renderer {
	public:
		static string renderState(Printer printerIn, Ram ramIn, Cpu cpuIn, Cursor cursorIn);

	private:
		Renderer (Printer printerIn, Ram ramIn, Cpu cpuIn, Cursor cursorIn);
		Printer printer;
		Ram ram;
		Cpu cpu;
		Cursor cursor;
		// Counts occurances of a char in a drawing.
		map<char, int> switchIndex;
		string insertActualValues(string lineIn);
		char getLightbulb(char cIn);
		bool pcIsPointingToAddress(int adr);
		bool pointingToAddress(AddrSpace addrSpace, int adr);
		bool instructionHasId(int id);
		char getFormattedOutput(int i);
		char getCodeBit(int i);
		char getDataBit(int i);
		bool machineNotActive();
};

#endif