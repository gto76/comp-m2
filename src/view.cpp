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

/*
 * Returns the character that represents lightbulb, either on or off.
 */
string View::getLightbulb(bool value) const {
  if (value) {
    return lightBulbOn;
  } else {
    return lightBulbOff;
  }
}