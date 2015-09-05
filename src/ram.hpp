#ifndef RAM_H
#define RAM_H

#include <vector>
#include <map>

#include "addr_space.hpp"
#include "address.hpp"

using namespace std;

class Ram {
	public:
		Ram();
		map<AddrSpace, vector<vector<bool>>> state;
		//vector<bool> get(AddrSpace addrSpace, vector<bool> adr) const;
		vector<bool> get(Address adr) const;
		//void set(AddrSpace addrSpace, vector<bool> adr, vector<bool> wordIn);
		void set(Address adr, vector<bool> wordIn);
		string getString() const;

	private:
		vector<bool> getLastAddress(AddrSpace addrSpace) const;
		vector<bool> getInput() const;
		void saveWord(AddrSpace addrSpace, int address, vector<bool> wordIn);
		void assignToLastAddress(AddrSpace addrSpace, vector<bool> wordIn);
		string getString(AddrSpace addrSpace) const;
};

#endif