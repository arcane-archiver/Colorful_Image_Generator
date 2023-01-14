#include "ColorfulImageGenerator.hpp"

ColorfulImageGenerator::ColorfulImageGenerator(unsigned width, unsigned height):
  WIDTH(width),
  HEIGHT(height),

  SCALE(std::min(width, height) / 800.0),

  SQUARE_WIDTH(15),
  EDGE_LENGTH(7),
  LINE_LENGTH(1),

  NODE_COLOR(ILLINI_ORANGE),
  EDGE_COLOR(ILLINI_BLUE),

  RECURSION_MAX(50000)

{
  recursionCount = 0;
}

cs225::PNG ColorfulImageGenerator::generate() const {
  cs225::PNG image(WIDTH, HEIGHT);

  void(SQUARE_WIDTH+EDGE_LENGTH+LINE_LENGTH+NODE_COLOR+EDGE_COLOR+RECURSION_MAX);

  return image;
}

void ColorfulImageGenerator::drawGraph(cs225::PNG canvas) const {

}

void ColorfulImageGenerator::paintIfInBounds(cs225::PNG canvas, cs225::HSLAPixel const paint, unsigned const x, unsigned const y) const {

}
