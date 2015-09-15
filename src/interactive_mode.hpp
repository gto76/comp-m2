#ifndef INTERACTIVE_MODE_H
#define INTERACTIVE_MODE_H

#include <string>
#include <vector>

#include "computer.hpp"
#include "cursor.hpp"
#include "drawing3D.hpp"
#include "drawing3Db.hpp"
#include "drawing2D.hpp"
#include "load.hpp"
#include "output.hpp"
#include "printer.hpp"
#include "random_input.hpp"
#include "renderer.hpp"
#include "view.hpp"

using namespace std;

class InteractiveMode {
  public:
    InteractiveMode(string filenameIn) {
        // : view3d(View(drawing3D, LIGHTBULB_ON_3D, LIGHTBULB_OFF_3D)),
        //   // view3db(View(drawing3Db, LIGHTBULB_ON_3D_B, LIGHTBULB_OFF_3D_B)),
        //   view2d(View(drawing2D, LIGHTBULB_ON_2D, LIGHTBULB_OFF_2D)),
        //   selectedView(&view3d),
        //   computer(getComputer(filenameIn)),
        //   printer(Printer(computer)),
        //   cursor(Cursor(computer.ram)) {  
        view3d = View(drawing3D, LIGHTBULB_ON_3D, LIGHTBULB_OFF_3D);
        view3db = View(drawing3Db, LIGHTBULB_ON_3D_B, LIGHTBULB_OFF_3D_B); 
        view2d = View(drawing2D, LIGHTBULB_ON_2D, LIGHTBULB_OFF_2D);
        selectedView = &view3d;

        static Ram ram = Ram(input);
        Load::fillRamWithFile(filenameIn.c_str(), ram);
        computer = Computer(0, ram, redrawScreen, sleepAndCheckForKey);

        // computer = getComputer(filenameIn);
        printer = Printer(computer);
        cursor = Cursor(computer.ram);

        // Whether esc was pressed during execution.
        executionCanceled = false;
        // Filename.
        loadedFilename = filenameIn;
        // Number of executions.
        executionCounter = 0;
        // Whether next key should be read as a char whose value shall thence be
        // inserted into ram.
        insertChar = false;
        insertNumber = false;
        shiftPressed = false;
    }
    static void start();

  private:
    static View view3d;
    static View view3db;
    static View view2d;
    static View *selectedView;
    static RandomInput input;
    static Computer computer;
    static Printer printer;
    static Cursor cursor;

    // Whether esc was pressed during execution.
    static bool executionCanceled;
    // Filename.
    static string loadedFilename;
    // Number of executions.
    static int executionCounter;
    // Saved state of a ram. Loaded after execution ends.
    static map<AddrSpace, vector<vector<bool>>> savedRamState;
    // Whether next key should be read as a char whose value shall thence be
    // inserted into ram.
    static bool insertChar;
    static bool insertNumber;
    static vector<int> digits;
    static bool shiftPressed ;

    //static Computer getComputer(string filename);

    // MAIN
    static void selectView();
    static void prepareOutput();
    static void drawScreen();
    // EXECUTION MODE
    static void run();
    static void exec();
    static void sleepAndCheckForKey();
    // EDIT MODE
    static void userInput();
    static void isertCharIntoRam(char c);
    static void processInputWithShift(char c);
    static bool insertNumberIntoRam(char c);
    static void engageInsertCharMode();
    static void engageInsertNumberMode();
    static void switchDrawing();
    // SAVE
    static void saveRamToNewFile();
    static void saveRamToCurrentFile();
    static string getFreeFileName();
    static void saveRamToFile(string filename);
    // KEY READER
    static char readStdin(bool drawCursor);
};

#endif