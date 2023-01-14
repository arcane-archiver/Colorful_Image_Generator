#include "../cs225/PNG.h"
#include "../cs225/HSLAPixel.h"
#include <string>
#include "cs225_ArtHelper.hpp"

void rotate(std::string inputFile, std::string outputFile) {
  cs225::PNG png; // call default constructor of PNG class
  png.readFromFile(inputFile);
  cs225::PNG result(png.width(), png.height());

  const unsigned int LAST_X_INDEX = png.width() - 1;
  const unsigned int LAST_Y_INDEX = png.height() - 1;

  for (unsigned x = 0; x <= LAST_X_INDEX; x++) {
    for (unsigned y = 0; y <= LAST_Y_INDEX; y++) {
      result.getPixel(LAST_X_INDEX - x, LAST_Y_INDEX - y) = png.getPixel(x, y);
    }
  }

  result.writeToFile(outputFile);
}

#include <stdexcept>
#include <cmath>

cs225::PNG myArt(unsigned int width, unsigned int height) {

  cs225::PNG png(width, height);

  std::srand(823784745); // set seed

  ArtHelper help(std::min(png.width(), png.height()));
  
  help.dumpBlackPaint(png);

  help.drawGraph(png);

  // get middle coordinates of png
  const unsigned int MIDDLE_X = std::ceil(width / 2.0);
  const unsigned int MIDDLE_Y = std::ceil(height / 2.0);

  // help.drawAura(png, MIDDLE_X, MIDDLE_Y);
  help.drawAura(png, 0, 0);
  // help.drawAura(png, 0, png.height() - 1);
  help.drawAura(png, png.width() - 1, png.height() - 1);
  // help.drawAura(png, png.width() - 1, 0);

  return png;

}
