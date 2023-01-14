#include "ColorfulImageGenerator.hpp"
#include <stdexcept>
#include <unordered_map>

using CanvasUtility::CardinalDirection::North;
using CanvasUtility::CardinalDirection::South;
using CanvasUtility::CardinalDirection::East;
using CanvasUtility::CardinalDirection::West;

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
  const CanvasUtility::Position position(North, MIDDLE_X, MIDDLE_Y);

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
      case 0: pivotDirection = North; break;
      case 1: pivotDirection = South; break;
      case 2: pivotDirection = East; break;
      case 3: pivotDirection = West; break;
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
      case North: y += 1; break;
      case South: y -= 1; break;
      case East: x += 1; break;
      case West: x -= 1; break;
      default: throw std::logic_error("attempted to draw edge with null position");
    }

    if (!CanvasUtility::inBounds(png, x, y) || isNodePixel(png.getPixel(x, y)))
      return CanvasUtility::Position(CanvasUtility::CardinalDirection::Null, x, y);

    switch(position.direction) {
      case North: case South:
        CanvasUtility::drawLine(png, EDGE_MARKER, LINE_LENGTH, CanvasUtility::Position(West, x, y));
        CanvasUtility::drawLine(png, EDGE_MARKER, LINE_LENGTH, CanvasUtility::Position(East, x, y));
        break;
      case East: case West:
        CanvasUtility::drawLine(png, EDGE_MARKER, LINE_LENGTH, CanvasUtility::Position(North, x, y));
        CanvasUtility::drawLine(png, EDGE_MARKER, LINE_LENGTH, CanvasUtility::Position(South, x, y));
        break;
      default:
        throw std::logic_error("bad Direction");
    }

    CanvasUtility::paintIfInBounds(png, EDGE_MARKER, x, y); // this is the "inside" of the drawn edge
  }

  return CanvasUtility::Position(position.direction, x, y);
}

void ColorfulImageGenerator::drawBranch(cs225::PNG & png, CanvasUtility::CardinalDirection const d, unsigned int const x, unsigned int const y) {
  auto pos = CanvasUtility::Position(d, x, y);
  unsigned int northY = y + static_cast<unsigned>(SQUARE_WIDTH / 2.0);
  unsigned int southY = y - static_cast<unsigned>(SQUARE_WIDTH / 2.0); 
  unsigned int eastX = x + static_cast<unsigned>(SQUARE_WIDTH / 2.0);
  unsigned int westX = x - static_cast<unsigned>(SQUARE_WIDTH / 2.0);

  std::unordered_map<CanvasUtility::CardinalDirection, CanvasUtility::Position> getBranchPosition = {
    {North, CanvasUtility::Position(North, x, northY)},
    {South, CanvasUtility::Position(South, x, southY)},
    {East, CanvasUtility::Position(East, eastX, y)},
    {West, CanvasUtility::Position(West, westX, y)}
  };

  switch (d) {
    case North:
      recursivelyDrawGraph(png, false, drawEdge(png, static_cast<unsigned>(EDGE_LENGTH), getBranchPosition.at(North)));
      recursivelyDrawGraph(png, false, drawEdge(png, static_cast<unsigned>(EDGE_LENGTH), getBranchPosition.at(East)));
      recursivelyDrawGraph(png, false, drawEdge(png, static_cast<unsigned>(EDGE_LENGTH), getBranchPosition.at(West)));
      break;
    case South:
      recursivelyDrawGraph(png, false, drawEdge(png, static_cast<unsigned>(EDGE_LENGTH), getBranchPosition.at(South)));
      recursivelyDrawGraph(png, false, drawEdge(png, static_cast<unsigned>(EDGE_LENGTH), getBranchPosition.at(East)));
      recursivelyDrawGraph(png, false, drawEdge(png, static_cast<unsigned>(EDGE_LENGTH), getBranchPosition.at(West)));
      break;
    case East:
      recursivelyDrawGraph(png, false, drawEdge(png, static_cast<unsigned>(EDGE_LENGTH), getBranchPosition.at(East)));
      recursivelyDrawGraph(png, false, drawEdge(png, static_cast<unsigned>(EDGE_LENGTH), getBranchPosition.at(North)));
      recursivelyDrawGraph(png, false, drawEdge(png, static_cast<unsigned>(EDGE_LENGTH), getBranchPosition.at(South)));
      break;
    case West:
      recursivelyDrawGraph(png, false, drawEdge(png, static_cast<unsigned>(EDGE_LENGTH), getBranchPosition.at(West)));
      recursivelyDrawGraph(png, false, drawEdge(png, static_cast<unsigned>(EDGE_LENGTH), getBranchPosition.at(North)));
      recursivelyDrawGraph(png, false, drawEdge(png, static_cast<unsigned>(EDGE_LENGTH), getBranchPosition.at(South)));
      break;
    default: throw std::logic_error("in branch, STOP");
  }
}

bool ColorfulImageGenerator::isNodePixel(cs225::HSLAPixel pixel) {
  return pixel.l == NODE_LUMINANCE_MARKER;
}

bool ColorfulImageGenerator::isEdgePixel(cs225::HSLAPixel pixel) {
  return pixel.l == EDGE_LUMINANCE_MARKER;
}
