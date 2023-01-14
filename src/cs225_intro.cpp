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

////////////////////////////////////////////////////

#include <stdexcept>
#include <cmath>

#ifndef ILLINI_ORANGE
#define ILLINI_ORANGE 11.0
#endif
#ifndef ILLINI_BLUE
#define ILLINI_BLUE 216.0
#endif
#ifndef NICE_COLOR
#define NICE_COLOR 300.0
#endif

cs225::PNG myArt(unsigned int width, unsigned int height) {

  cs225::PNG png(width, height);

  std::srand(823784745); // set seed
  const unsigned int MIDDLE_X = std::ceil(width / 2.0);
  const unsigned int MIDDLE_Y = std::ceil(height / 2.0);
  try {
    ArtHelper help(std::min(png.width(), png.height()));
    help.dump(png);

    ArtHelper::Position pos = ArtHelper::Position(ArtHelper::Direction::N, MIDDLE_X, MIDDLE_Y);
    pos = help.drawSquare(png, ArtHelper::Direction::N, MIDDLE_X, MIDDLE_Y); // START
    help.recursivelyDraw(png, true, ArtHelper::Direction::N, MIDDLE_X, MIDDLE_Y);
    const double EYE_RADIUS = 40 * help.SCALE;
    const double EYE_Y = 500 * help.SCALE;
    const double RIGHT_EYE_X = 480 * help.SCALE;
    const double LEFT_EYE_X = 320 * help.SCALE;
    const double FACE_L = 0.3;
    help.drawCircle(png, cs225::HSLAPixel(NICE_COLOR, 1.0, FACE_L), ArtHelper::Direction::STOP, EYE_RADIUS, RIGHT_EYE_X, EYE_Y);
    help.drawCircle(png, cs225::HSLAPixel(NICE_COLOR, 1.0, FACE_L), ArtHelper::Direction::STOP, EYE_RADIUS, LEFT_EYE_X, EYE_Y);
    help.drawSmile(png, cs225::HSLAPixel(NICE_COLOR, 1.0, FACE_L));
    // help.drawAura(png, RIGHT_EYE_X, EYE_Y);
    // help.drawAura(png, LEFT_EYE_X, EYE_Y);
    // help.drawAura(png, MIDDLE_X, MIDDLE_Y);
    help.drawAura(png, 0, 0);
    // help.drawAura(png, 0, png.height() - 1);
    help.drawAura(png, png.width() - 1, png.height() - 1);
    // help.drawAura(png, png.width() - 1, 0);
    png = help.flip(png);
  } catch (...) {
    png.getPixel(0, 0).h = 90;
    png.getPixel(0, 0).l = 0.5;
  
    png.getPixel(1, 0).h = 180;
    png.getPixel(1, 0).l = 0.5;
  
    png.getPixel(0, 1).h = 270;
    png.getPixel(0, 1).l = 0.5;
  }
  return png;

}
