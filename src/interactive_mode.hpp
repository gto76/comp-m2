#ifndef INTERACTIVE_MODE_H
#define INTERACTIVE_MODE_H

#include <string>

using namespace std;

// Whether esc was pressed during execution.
//extern bool executionCanceled;

class InteractiveMode {
  public:
    static void startInteractiveMode(string filename);
    // static bool executionCanceled;
};

#endif