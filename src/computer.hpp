#ifndef COMPUTER_H
#define COMPUTER_H

#include "provides_output.hpp"

using namespace std;

class Computer : public virtual ProvidesOutput {
  public:
    Computer(bool outputCharsIn,
             ProvidesOutput inputIn)
        : outputChars(outputCharsIn),
          input(inputIn) { }

    virtual string getOutput() const;

  private:
    //const bool interactivieMode;
    // const bool outputChars;

    // Filename.
    // string loadedFilename;

    ProvidesOutput input;

    // Main components.
    // Printer printer;
    Ram ram = Ram(printer);
    Cpu cpu = Cpu(&ram);

    // Graphic representation of the computer's state.
    // vector<vector<string>> buffer;

    // Cycle counter.
    bool executionCanceled = false;
    int executionCounter = 0;

    // Saved state of a ram. Loaded after execution ends.
    map<AddrSpace, vector<vector<bool>>> savedRamState;

    // Object for keeping track of, and moving around cursor.
    Cursor cursor = Cursor(ram);

    // Two views.
    // View VIEW_3D = View(drawing3D, LIGHTBULB_ON_3D, LIGHTBULB_OFF_3D);
    // View VIEW_3D_B = View(drawing3Db, LIGHTBULB_ON_3D_B, LIGHTBULB_OFF_3D_B);
    // View VIEW_2D = View(drawing2D, LIGHTBULB_ON_2D, LIGHTBULB_OFF_2D);
    // View *selectedView = &VIEW_3D;

    // Whether next key should be read as a char whose value shall thence be
    // inserted into ram.
    // bool insertChar = false;
    // bool insertNumber = false;
    // vector<int> digits;
    // bool shiftPressed = false;

};

#endif