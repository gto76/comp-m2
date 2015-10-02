#include "comp.hpp"

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "parser.hpp"
#include "interactive_mode.hpp"
#include "noninteractive_mode.hpp"
#include "util.hpp"

using namespace std;

void assertFilenames();
bool inputIsNotPiped();
void processArguments(int argc, const char* argv[]);
void processFilename(string filename);
string getFilenameOut();
void saveSourceToFile(string filenameOut);
void loadAllFilesFromDir(string dirname);
string getFirstFilename();

vector<string> filenames;
string dirname;
bool interactivieMode = true;
bool outputChars = false;
bool outputNumbers = false;
bool inputChars = false;
bool rawInput = false;
bool compile = false;
bool parse = false;

int main(int argc, const char* argv[]) {
  srand(time(NULL));
  processArguments(argc, argv);
  if (interactivieMode == true) {
    interactivieMode = inputIsNotPiped();
  }
  if (outputChars == false) {
    outputNumbers = !Util::outputIsPiped();
  }
  if (compile) {
    assertFilenames();
    string filenameOut = getFilenameOut();
    string sourceNameOut = "/tmp/"+filenameOut+".cpp";
    saveSourceToFile(sourceNameOut);
    string command = GCC_COMMAND+" "+filenameOut+" "+sourceNameOut;
    system(command.c_str());
  } else if (parse) {
    assertFilenames();
    string filenameOut = getFilenameOut();
    cout << filenameOut << endl;
    saveSourceToFile(filenameOut+".cpp");
  } else if (interactivieMode) {
    InteractiveMode::startInteractiveMode(getFirstFilename());
  } else {
    assertFilenames();
    NoninteractiveMode mode = NoninteractiveMode(filenames, outputNumbers,
                                                 outputChars, inputChars, 
                                                 rawInput);
    mode.run();
  }
}

void assertFilenames() {
  if (filenames.empty()) {
    cout << "There were no files specified. Aborting.";
    exit(1);
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
    const char *arg = argv[i];
    if (Util::contains({ "-n", "--noninteractive"}, arg)) {
      interactivieMode = false;
    } else if (Util::contains({ "-c", "--char-output"}, arg)) {
      interactivieMode = false;
      outputChars = true;
    } else if (Util::contains({ "-f", "--filter"}, arg)) {
      interactivieMode = false;
      outputChars = true;
      inputChars = true;
    } else if (Util::contains({ "-g", "--game"}, arg)) {
      interactivieMode = false;
      outputChars = true;
      inputChars = true;
      rawInput = true;
    } else if (Util::contains({ "compile" }, arg)) {
      compile = true;
    } else if (Util::contains({ "parse" }, arg)) {
      parse = true;
    } else {
      processFilename(argv[i]);
    }
  }
}

void processFilename(string filename) {
  if (Util::isADir(filename)) {
    dirname = filename;
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

string getFilenameOut() {
  string fullname = filenames.back();
  // Names source file and executable by directory name if present,
  // else by the last input file.
  if (dirname != "") {
    fullname = dirname;
  }
  size_t slashIndex = fullname.find_last_of('/');
  if (slashIndex != string::npos) {
    if(slashIndex == fullname.length()-1) {
      fullname.erase(fullname.length()-1);
      slashIndex = fullname.find_last_of('/');
      if (slashIndex != string::npos) {
        fullname = fullname.substr(slashIndex + 1);
      }
    } else {
      fullname = fullname.substr(slashIndex + 1);
    }
  }
  size_t lastindex = fullname.find_last_of("."); 
  if (lastindex == string::npos) {
    return fullname;
  } else {
    return fullname.substr(0, lastindex);
  }
}

void saveSourceToFile(string filenameOut) {
  string source = Parser::parse(filenames, outputChars, inputChars, rawInput);
  ofstream out(filenameOut);
  out << source;
  out.close();
}

string getFirstFilename() {
  if (filenames.empty()) {
    return "";
  } else {
    return filenames[0];
  }
}


