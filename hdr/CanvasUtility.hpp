#ifndef CANVAS_UTILITY_HPP
#define CANVAS_UTILITY_HPP

#include "../cs225/PNG.h"
#include "../cs225/HSLAPixel.h"

namespace CanvasUtility {
  void paintIfInBounds(cs225::PNG &canvas, cs225::HSLAPixel const paint, unsigned const x, unsigned const y);
}

#endif