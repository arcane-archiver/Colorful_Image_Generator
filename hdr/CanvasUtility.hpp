#ifndef CANVAS_UTILITY_HPP
#define CANVAS_UTILITY_HPP

#include "../cs225/PNG.h"
#include "../cs225/HSLAPixel.h"

namespace CanvasUtility {
  enum CardinalDirection { North, South, East, West, Null };

  struct Position {
    CardinalDirection direction;
    unsigned int x;
    unsigned int y;
    Position(CardinalDirection setDirection, unsigned int setX, unsigned int setY);
  };

  bool inBounds(cs225::PNG const &canvas, const unsigned int x, const unsigned int y);
  void paintIfInBounds(cs225::PNG &canvas, cs225::HSLAPixel const paint, unsigned const x, unsigned const y);

  void drawSquare(
    cs225::PNG &png,
    cs225::HSLAPixel const paint,
    double const squareWidth,
    unsigned const middle_x,
    unsigned const middle_y
  );

  void drawLine(cs225::PNG &canvas, cs225::HSLAPixel paint, double lineLength, CardinalDirection direction, unsigned x, unsigned y);
}

#endif