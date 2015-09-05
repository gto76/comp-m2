#ifndef RENDERER_H
#define RENDERER_H

#include <map>
#include <string>

#include "address.hpp"
#include "cpu.hpp"
#include "cursor.hpp"
#include "instruction.hpp"
#include "printer.hpp"
#include "ram.hpp"
#include "specific_instruction.hpp"

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
		vector<Instruction> cursorsInstruction; // Just a way to leave instruction uninitialized at
		// Renderers creation, and optionaly create it later (intended to hold only zero or one instruction).
		// Counts occurances of a char in a drawing.
		map<char, int> switchIndex;
		string insertActualValues(string lineIn);
		char getLightbulb(char cIn);
		bool pcPointingToAddress(int adr);
		bool instructionPointingToAddress(Address adr);
		bool instructionHasId(int id);
		char getFormattedOutput(int i);
		char getCodeBit(int i);
		char getDataBit(int i);
		bool machineNotActive();
		Instruction& getCursorsInstruction();
};

#endif