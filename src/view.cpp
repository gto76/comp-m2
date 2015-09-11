#include "view.hpp"

#include "util.hpp"

#include <string>

View::View(vector<string> drawingIn, string lightBulbOnIn, string lightBulbOffIn) {
  lines = Util::splitIntoLines(drawingIn);
  lightBulbOn = lightBulbOnIn;
  lightBulbOff = lightBulbOffIn;
  height = lines.size();
  width = Util::getSizeOfLargestElement(lines);
}

string View::getLightbulb(bool value) {
  if (value) {
    return lightBulbOn;
  } else {
    return lightBulbOff;
  }
}