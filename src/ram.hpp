#ifndef RAM_H
#define RAM_H

#include <vector>

#include "const.hpp"

using namespace std;

class Ram {
	public:
		vector<vector<bool>> instructions = vector<vector<bool>>(RAM_SIZE, vector<bool>(WORD_SIZE));
		vector<vector<bool>> data = vector<vector<bool>>(RAM_SIZE, vector<bool>(WORD_SIZE));
		vector<bool> getInstruction(vector<bool> adr);
		vector<bool> getData(vector<bool> adr);
		void setInstruction(vector<bool> adr, vector<bool> wordIn);
		void setData(vector<bool> adr, vector<bool> wordIn);
		string getString();

	private:
		void saveWord(int address, vector<bool> wordIn, bool toInstructions);
};

#endif