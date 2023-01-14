#include "ColorfulImageGenerator.hpp"
#include <stdexcept>

ColorfulImageGenerator::ColorfulImageGenerator(unsigned width, unsigned height):
  WIDTH(width),
  HEIGHT(height),

  SCALE(std::min(width, height) / 800.0),

  SQUARE_WIDTH(15),
  EDGE_LENGTH(7),
  LINE_LENGTH(1),

  NODE_PAINT(ILLINI_ORANGE, 1.0, 0.5),
  EDGE_PAINT(ILLINI_BLUE, 1.0, 0.5),

  RECURSION_MAX(50000)
{
  recursionCount = 0;
}

cs225::PNG ColorfulImageGenerator::generate() const {
  cs225::PNG image(WIDTH, HEIGHT);

  void(SQUARE_WIDTH+EDGE_LENGTH+LINE_LENGTH+NODE_PAINT.h+EDGE_PAINT.h+RECURSION_MAX);

  return image;
}

void ColorfulImageGenerator::drawGraph(cs225::PNG image) const {
  const unsigned MIDDLE_X = static_cast<unsigned>(image.width() / 2.0);
  const unsigned MIDDLE_Y = static_cast<unsigned>(image.height() / 2.0);

  CanvasUtility::drawSquare(image, NODE_PAINT, SQUARE_WIDTH, MIDDLE_X, MIDDLE_Y); // first node
  recursivelyDrawGraph(image, true, CanvasUtility::CardinalDirection::North, MIDDLE_X, MIDDLE_Y);
}

void ColorfulImageGenerator::recursivelyDrawGraph(cs225::PNG &image, bool branching, CanvasUtility::CardinalDirection direction, unsigned int x, unsigned int y) const {
  // recursionCount += 1;
  // if (recursionCount >= RECURSION_MAX)
  //   return;

  unsigned int randomNumber = std::rand() % 10;

  auto position = CanvasUtility::Position(direction, x, y);

  if (direction == CanvasUtility::CardinalDirection::Null || !CanvasUtility::inBounds(image, x, y))
    return;

  if (branching == true) {
    // drawBranch(image, direction, x, y);
  } else if (randomNumber < 2) {
    CanvasUtility::drawSquare(image, NODE_PAINT, SQUARE_WIDTH, x, y);
    recursivelyDrawGraph(image, true, direction, x, y);
  } else {
    randomNumber = std::rand() % 10;
    unsigned edgeLength = static_cast<unsigned>(EDGE_LENGTH);

    if (randomNumber < 4)
      edgeLength *= 3;
    else if (randomNumber < 6)
      edgeLength *= 9;

    randomNumber = std::rand() % 4;

    switch(randomNumber) {
      case 0: direction = CanvasUtility::CardinalDirection::North; break;
      case 1: direction = CanvasUtility::CardinalDirection::South; break;
      case 2: direction = CanvasUtility::CardinalDirection::East; break;
      case 3: direction = CanvasUtility::CardinalDirection::West; break;
      default: throw std::logic_error("random number is not in [0, 3]");
    }

    // position = drawEdge(image, edgeLength, direction, x, y);
    recursivelyDrawGraph(image, false, position.direction, position.x, position.y);
  }
}

#include <cmath>
CanvasUtility::Position ColorfulImageGenerator::drawEdge(cs225::PNG &png, unsigned int edgeLength, CanvasUtility::CardinalDirection d, unsigned int x, unsigned int y) const {
  for (unsigned i = 0; i < edgeLength; i++) {
    switch (d) {
      case CanvasUtility::CardinalDirection::North: y += 1; break;
      case CanvasUtility::CardinalDirection::South: y -= 1; break;
      case CanvasUtility::CardinalDirection::East: x += 1; break;
      case CanvasUtility::CardinalDirection::West: x -= 1; break;
      case CanvasUtility::CardinalDirection::Null: throw std::logic_error("attempted to draw edge with CanvasUtility::CardinalDirection::STOP");
    }

    if (!CanvasUtility::inBounds(png, x, y) || png.getPixel(x, y).h == NODE_PAINT.h) {
      return CanvasUtility::Position(CanvasUtility::CardinalDirection::Null, x, y);
    }

    if (d == CanvasUtility::CardinalDirection::North || d == CanvasUtility::CardinalDirection::South) {
      CanvasUtility::drawLine(png, EDGE_PAINT, static_cast<unsigned>(EDGE_LENGTH), CanvasUtility::CardinalDirection::West, x, y);
      CanvasUtility::drawLine(png, EDGE_PAINT, LINE_LENGTH, CanvasUtility::CardinalDirection::East, x, y);
    } else if (d == CanvasUtility::CardinalDirection::East || d == CanvasUtility::CardinalDirection::West) {
      CanvasUtility::drawLine(png, EDGE_PAINT, LINE_LENGTH, CanvasUtility::CardinalDirection::North, x, y);
      CanvasUtility::drawLine(png, EDGE_PAINT, LINE_LENGTH, CanvasUtility::CardinalDirection::South, x, y);
    } else {
      throw std::logic_error("bad Direction d");
    }

    CanvasUtility::paintIfInBounds(png, EDGE_PAINT, x, y);
  }
  return CanvasUtility::Position(d, x, y);
}

void ColorfulImageGenerator::drawBranch(cs225::PNG &image, CanvasUtility::CardinalDirection direction, unsigned int x, unsigned int y) const {
  auto position = CanvasUtility::Position(direction, x, y);
  unsigned southY = y - static_cast<unsigned>(SQUARE_WIDTH / 2.0); 
  unsigned northY = y + static_cast<unsigned>(SQUARE_WIDTH / 2.0);
  unsigned eastX = x + static_cast<unsigned>(SQUARE_WIDTH / 2.0);
  unsigned westX = x - static_cast<unsigned>(SQUARE_WIDTH / 2.0);
  switch (direction) {
    case CanvasUtility::CardinalDirection::North:
      position = drawEdge(image, static_cast<unsigned>(EDGE_LENGTH), CanvasUtility::CardinalDirection::North, x, northY);
      recursivelyDrawGraph(image, false, position.direction, position.x, position.y);
      position = drawEdge(image, static_cast<unsigned>(EDGE_LENGTH), CanvasUtility::CardinalDirection::East, eastX, y);
      recursivelyDrawGraph(image, false, position.direction, position.x, position.y);
      position = drawEdge(image, static_cast<unsigned>(EDGE_LENGTH), CanvasUtility::CardinalDirection::West, westX, y);
      recursivelyDrawGraph(image, false, position.direction, position.x, position.y); break;
    case CanvasUtility::CardinalDirection::South:
      position = drawEdge(image, static_cast<unsigned>(EDGE_LENGTH), CanvasUtility::CardinalDirection::South, x, southY);
      recursivelyDrawGraph(image, false, position.direction, position.x, position.y);
      position = drawEdge(image, static_cast<unsigned>(EDGE_LENGTH), CanvasUtility::CardinalDirection::East, eastX, y);
      recursivelyDrawGraph(image, false, position.direction, position.x, position.y);
      position = drawEdge(image, static_cast<unsigned>(EDGE_LENGTH), CanvasUtility::CardinalDirection::West, westX, y);
      recursivelyDrawGraph(image, false, position.direction, position.x, position.y); break;
    case CanvasUtility::CardinalDirection::East:
      position = drawEdge(image, static_cast<unsigned>(EDGE_LENGTH), CanvasUtility::CardinalDirection::East, eastX, y);
      recursivelyDrawGraph(image, false, position.direction, position.x, position.y);
      position = drawEdge(image, static_cast<unsigned>(EDGE_LENGTH), CanvasUtility::CardinalDirection::North, x, northY);
      recursivelyDrawGraph(image, false, position.direction, position.x, position.y);
      position = drawEdge(image, static_cast<unsigned>(EDGE_LENGTH), CanvasUtility::CardinalDirection::South, x, southY);
      recursivelyDrawGraph(image, false, position.direction, position.x, position.y); break;
    case CanvasUtility::CardinalDirection::West:
      position = drawEdge(image, static_cast<unsigned>(EDGE_LENGTH), CanvasUtility::CardinalDirection::West, westX, y);
      recursivelyDrawGraph(image, false, position.direction, position.x, position.y);
      position = drawEdge(image, static_cast<unsigned>(EDGE_LENGTH), CanvasUtility::CardinalDirection::North, x, northY);
      recursivelyDrawGraph(image, false, position.direction, position.x, position.y);
      position = drawEdge(image, static_cast<unsigned>(EDGE_LENGTH), CanvasUtility::CardinalDirection::South, x, southY); 
      recursivelyDrawGraph(image, false, position.direction, position.x, position.y); break;
    case CanvasUtility::CardinalDirection::Null: throw std::logic_error("in branch, CanvasUtility::CardinalDirection::STOP");
  }
}
