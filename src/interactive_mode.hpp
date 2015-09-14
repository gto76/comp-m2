#ifndef INTERACTIVE_MODE_H
#define INTERACTIVE_MODE_H

#include <string>
#include <vector>

using namespace std;

class InteractiveMode {
  public:
    InteractiveMode(string filenameIn) 
        : view3d(View(drawing3D, LIGHTBULB_ON_3D, LIGHTBULB_OFF_3D)),
          view3db(View(drawing3Db, LIGHTBULB_ON_3D_B, LIGHTBULB_OFF_3D_B)),
          view2d(View(drawing2D, LIGHTBULB_ON_2D, LIGHTBULB_OFF_2D)),
          selectedView(&view3d),
          computer(getComputer(filenameIn)),
          printer(Printer(computer)),
          cursor(Cursor(computer.ram)),
          renderer(printer, computer.ram, computer.cpu, cursor, selectedView) { }
    void start();

  private:
    const View view3d;
    const View view3db;
    const View view2d;
    View *selectedView;
    const RandomInput input;
    const Computer computer;
    const Printer printer;
    const Cursor cursor;
    const Renderer renderer;

    // Whether esc was pressed during execution.
    bool executionCanceled = false;
    // Filename.
    string loadedFilename;
    // Number of executions.
    int executionCounter = 0;
    // Saved state of a ram. Loaded after execution ends.
    map<AddrSpace, vector<vector<bool>>> savedRamState;
    // Whether next key should be read as a char whose value shall thence be
    // inserted into ram.
    bool insertChar = false;
    bool insertNumber = false;
    vector<int> digits;
    bool shiftPressed = false;

    Computer getComputer(string filename);

    // MAIN
    void selectView();
    void prepareOutput();
    void drawScreen();
    // EXECUTION MODE
    void run();
    void exec();
    void sleepAndCheckForKey();
    // EDIT MODE
    void userInput();
    void isertCharIntoRam(char c);
    void processInputWithShift(char c);
    bool insertNumberIntoRam(char c);
    void engageInsertCharMode();
    void engageInsertNumberMode();
    void switchDrawing();
    // SAVE
    void saveRamToNewFile();
    void saveRamToCurrentFile();
    string getFreeFileName();
    void saveRamToFile(string filename);
    // KEY READER
    char readStdin(bool drawCursor);
};

#endif