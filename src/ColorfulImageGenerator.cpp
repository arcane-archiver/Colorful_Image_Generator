#include "ColorfulImageGenerator.hpp"
#include <stdexcept>

cs225::PNG ColorfulImageGenerator::generate(unsigned width, unsigned height) {
  cs225::PNG image(width, height);
  drawGraph(image);
  return image;
}

cs225::PNG ColorfulImageGenerator::generate(unsigned width, unsigned height, unsigned seed) {
  std::srand(seed); // set seed
  return generate(width, height);
}

void ColorfulImageGenerator::drawGraph(cs225::PNG &image) {
  const unsigned MIDDLE_X = static_cast<unsigned>(image.width() / 2.0);
  const unsigned MIDDLE_Y = static_cast<unsigned>(image.height() / 2.0);
  const CanvasUtility::Position position(CanvasUtility::CardinalDirection::North, MIDDLE_X, MIDDLE_Y);

  CanvasUtility::drawSquare(image, NODE_MARKER, SQUARE_WIDTH, MIDDLE_X, MIDDLE_Y); // first node
  recursivelyDrawGraph(image, true, position);
}

void ColorfulImageGenerator::recursivelyDrawGraph(cs225::PNG &image, bool branching, CanvasUtility::Position const position) {
  unsigned int randomNumber = std::rand() % 10;
  auto const x = position.x;
  auto const y = position.y;

  if (position.direction == CanvasUtility::CardinalDirection::Null || !CanvasUtility::inBounds(image, x, y))
    return;

  if (branching == true) {
    drawBranch(image, position.direction, x, y);
  } else if (randomNumber < 2) {
    CanvasUtility::drawSquare(image, NODE_MARKER, SQUARE_WIDTH, x, y);
    recursivelyDrawGraph(image, true, position);
  } else {
    randomNumber = std::rand() % 10;
    unsigned edgeLength = static_cast<unsigned>(EDGE_LENGTH);

    if (randomNumber < 4)
      edgeLength *= 3;
    else if (randomNumber < 6)
      edgeLength *= 9;

    randomNumber = std::rand() % 4;

    CanvasUtility::CardinalDirection pivotDirection = position.direction;
    switch(randomNumber) {
      case 0: pivotDirection = CanvasUtility::CardinalDirection::North; break;
      case 1: pivotDirection = CanvasUtility::CardinalDirection::South; break;
      case 2: pivotDirection = CanvasUtility::CardinalDirection::East; break;
      case 3: pivotDirection = CanvasUtility::CardinalDirection::West; break;
      default: throw std::logic_error("random number is not in [0, 3]");
    }

    CanvasUtility::Position pivotPosition = drawEdge(image, edgeLength, CanvasUtility::Position(pivotDirection, position.x, position.y));
    recursivelyDrawGraph(image, false, pivotPosition);
  }
}

CanvasUtility::Position ColorfulImageGenerator::drawEdge(cs225::PNG &png, unsigned int edgeLength, CanvasUtility::Position const position) {
  unsigned x = position.x;
  unsigned y = position.y;
  for (unsigned i = 0; i < edgeLength; i++) {
    switch (position.direction) {
      case CanvasUtility::CardinalDirection::North: y += 1; break;
      case CanvasUtility::CardinalDirection::South: y -= 1; break;
      case CanvasUtility::CardinalDirection::East: x += 1; break;
      case CanvasUtility::CardinalDirection::West: x -= 1; break;
      case CanvasUtility::CardinalDirection::Null: throw std::logic_error("attempted to draw edge with CanvasUtility::CardinalDirection::STOP");
    }

    if (!CanvasUtility::inBounds(png, x, y) || isNodePixel(png.getPixel(x, y))) {
      return CanvasUtility::Position(CanvasUtility::CardinalDirection::Null, x, y);
    }

    if (position.direction == CanvasUtility::CardinalDirection::North || position.direction == CanvasUtility::CardinalDirection::South) {
      CanvasUtility::drawLine(png, EDGE_MARKER, LINE_LENGTH, CanvasUtility::CardinalDirection::West, x, y);
      CanvasUtility::drawLine(png, EDGE_MARKER, LINE_LENGTH, CanvasUtility::CardinalDirection::East, x, y);
    } else if (position.direction == CanvasUtility::CardinalDirection::East || position.direction == CanvasUtility::CardinalDirection::West) {
      CanvasUtility::drawLine(png, EDGE_MARKER, LINE_LENGTH, CanvasUtility::CardinalDirection::North, x, y);
      CanvasUtility::drawLine(png, EDGE_MARKER, LINE_LENGTH, CanvasUtility::CardinalDirection::South, x, y);
    } else {
      throw std::logic_error("bad Direction d");
    }

    CanvasUtility::paintIfInBounds(png, EDGE_MARKER, x, y);
  }
  return CanvasUtility::Position(position.direction, x, y);
}

void ColorfulImageGenerator::drawBranch(cs225::PNG &image, CanvasUtility::CardinalDirection direction, unsigned int x, unsigned int y) {
  auto position = CanvasUtility::Position(direction, x, y);
  unsigned southY = y - static_cast<unsigned>(SQUARE_WIDTH / 2.0); 
  unsigned northY = y + static_cast<unsigned>(SQUARE_WIDTH / 2.0);
  unsigned eastX = x + static_cast<unsigned>(SQUARE_WIDTH / 2.0);
  unsigned westX = x - static_cast<unsigned>(SQUARE_WIDTH / 2.0);
  switch (direction) {
    case CanvasUtility::CardinalDirection::North:
      position = drawEdge(image, static_cast<unsigned>(EDGE_LENGTH), CanvasUtility::Position(CanvasUtility::CardinalDirection::North, x, northY));
      recursivelyDrawGraph(image, false, position);
      position = drawEdge(image, static_cast<unsigned>(EDGE_LENGTH), CanvasUtility::Position(CanvasUtility::CardinalDirection::East, eastX, y));
      recursivelyDrawGraph(image, false, position);
      position = drawEdge(image, static_cast<unsigned>(EDGE_LENGTH), CanvasUtility::Position(CanvasUtility::CardinalDirection::West, westX, y));
      recursivelyDrawGraph(image, false, position); break;
    case CanvasUtility::CardinalDirection::South:
      position = drawEdge(image, static_cast<unsigned>(EDGE_LENGTH), CanvasUtility::Position(CanvasUtility::CardinalDirection::South, x, southY));
      recursivelyDrawGraph(image, false, position);
      position = drawEdge(image, static_cast<unsigned>(EDGE_LENGTH), CanvasUtility::Position(CanvasUtility::CardinalDirection::East, eastX, y));
      recursivelyDrawGraph(image, false, position);
      position = drawEdge(image, static_cast<unsigned>(EDGE_LENGTH), CanvasUtility::Position(CanvasUtility::CardinalDirection::West, westX, y));
      recursivelyDrawGraph(image, false, position); break;
    case CanvasUtility::CardinalDirection::East:
      position = drawEdge(image, static_cast<unsigned>(EDGE_LENGTH), CanvasUtility::Position(CanvasUtility::CardinalDirection::East, eastX, y));
      recursivelyDrawGraph(image, false, position);
      position = drawEdge(image, static_cast<unsigned>(EDGE_LENGTH), CanvasUtility::Position(CanvasUtility::CardinalDirection::North, x, northY));
      recursivelyDrawGraph(image, false, position);
      position = drawEdge(image, static_cast<unsigned>(EDGE_LENGTH), CanvasUtility::Position(CanvasUtility::CardinalDirection::South, x, southY));
      recursivelyDrawGraph(image, false, position); break;
    case CanvasUtility::CardinalDirection::West:
      position = drawEdge(image, static_cast<unsigned>(EDGE_LENGTH), CanvasUtility::Position(CanvasUtility::CardinalDirection::West, westX, y));
      recursivelyDrawGraph(image, false, position);
      position = drawEdge(image, static_cast<unsigned>(EDGE_LENGTH), CanvasUtility::Position(CanvasUtility::CardinalDirection::North, x, northY));
      recursivelyDrawGraph(image, false, position);
      position = drawEdge(image, static_cast<unsigned>(EDGE_LENGTH), CanvasUtility::Position(CanvasUtility::CardinalDirection::South, x, southY)); 
      recursivelyDrawGraph(image, false, position); break;
    case CanvasUtility::CardinalDirection::Null: throw std::logic_error("in branch, CanvasUtility::CardinalDirection::STOP");
  }
}

bool ColorfulImageGenerator::isNodePixel(cs225::HSLAPixel pixel) {
  return pixel.l == NODE_LUMINANCE_MARKER;
}

bool ColorfulImageGenerator::isEdgePixel(cs225::HSLAPixel pixel) {
  return pixel.l == EDGE_LUMINANCE_MARKER;
}
