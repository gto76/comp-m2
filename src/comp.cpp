#include <cstring>
#include <string>
#include <vector>

#include "interactive_mode.hpp"
#include "noninteractive_mode.hpp"

using namespace std;

bool inputIsNotPiped();
void processArguments(int argc, const char* argv[]);
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
      filenames.push_back(argv[i]);
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


