#ifndef VIEW_H
#define VIEW_H

#include <string>
#include <vector>

#include "util.hpp"

using namespace std;

class View {
  public:
    vector<vector<string>> lines;
    string lightBulbOn;
    string lightBulbOff;
    size_t height;
    size_t width;
    View(vector<string> drawingIn, string lightBulbOnIn, string lightBulbOffIn);
    string getLightbulb(bool value);
    bool operator == (const View& v) const {
      return lines == v.lines && 
             lightBulbOn == v.lightBulbOn && 
             lightBulbOff == v.lightBulbOff;
    }
};

#endif