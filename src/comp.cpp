#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "interactive_mode.hpp"
#include "noninteractive_mode.hpp"

using namespace std;

bool inputIsNotPiped();
void processArguments(int argc, const char* argv[]);
void processFilename(string filename);
void loadAllFilesFromDir(string dirname);
string getFirstFilename();

vector<string> filenames;
bool outputChars = false;

int main(int argc, const char* argv[]) {
  srand(time(NULL));
  bool interactivieMode = inputIsNotPiped();
  processArguments(argc, argv);
  if (interactivieMode) {
    InteractiveMode::startInteractiveMode(getFirstFilename());
  } else {
    if (filenames.empty()) {
      cout << "There were no files specified. Aborting.";
      exit(1);
    }
    NoninteractiveMode mode = NoninteractiveMode(filenames, outputChars);
    mode.run();
  }
}

bool inputIsNotPiped() {
  return !Util::inputIsPiped();
}

void processArguments(int argc, const char* argv[]) {
  if (argc <= 1) {
    return;
  }
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-c") == 0) {
      outputChars = true;
    } else {
      processFilename(argv[i]);
    }
  }
}

void processFilename(string filename) {
  if (Util::isADir(filename)) {
    loadAllFilesFromDir(filename);
  } else {
    filenames.push_back(filename);
  }
}

void loadAllFilesFromDir(string dirname) {
  vector<string> filesInDir = Util::getFilesInDirectory(dirname);
  for (string file : filesInDir) {
    if (Util::endsWith(file, ".cm2")) {
      filenames.push_back(file);
    }
  }
}

string getFirstFilename() {
  if (filenames.empty()) {
    return "";
  } else {
    return filenames[0];
  }
}


