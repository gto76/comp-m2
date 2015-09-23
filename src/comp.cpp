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
bool outputChars = false;
bool bufferOutput = false;
bool compile = false;
bool parse = false;

int main(int argc, const char* argv[]) {
  srand(time(NULL));
  bool interactivieMode = inputIsNotPiped();
  processArguments(argc, argv);
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
    NoninteractiveMode mode = NoninteractiveMode(filenames, outputChars,
                                                 bufferOutput);
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
    if (strcmp(argv[i], "-c") == 0) {
      outputChars = true;
    } else if (strcmp(argv[i], "compile") == 0) {
      compile = true;
    } else if (strcmp(argv[i], "parse") == 0) {
      parse = true;
    } else if (strcmp(argv[i], "-C") == 0) {
      outputChars = true;
      bufferOutput = true;
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
  string source = Parser::parse(filenames, outputChars);
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


