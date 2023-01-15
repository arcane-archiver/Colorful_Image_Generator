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

  const CanvasUtility::Position MIDDLE_POSITION(North, MIDDLE_X, MIDDLE_Y);

  CanvasUtility::drawSquare(image, NODE_MARKER, SQUARE_WIDTH, MIDDLE_X, MIDDLE_Y); // first node
  recursivelyDrawGraph(image, true, MIDDLE_POSITION);
  return;

  std::stack<CanvasUtility::Position> work;
  work.push(MIDDLE_POSITION);

  while (!work.empty()) {
    CanvasUtility::Position position(work.top()); work.pop();
    unsigned int randomNumber = std::rand() % 10;

    if (randomNumber < 2) {
      /* draw a node and branch into three more edges */
      CanvasUtility::drawSquare(image, NODE_MARKER, SQUARE_WIDTH, position.x, position.y); // draw node
      drawBranch(image, position, work);
    } else {
      /* draw an edge, extending what was previously drawn */
      randomNumber = std::rand() % 10;
      unsigned edgeLength = static_cast<unsigned>(EDGE_LENGTH);
      
      if (randomNumber < 4)
        edgeLength *= 3;
      else if (randomNumber < 6)
        edgeLength *= 9;

      auto randomDirection = static_cast<CanvasUtility::CardinalDirection>(std::rand() % 4);
      CanvasUtility::Position pivotPosition = drawEdge(image, edgeLength, CanvasUtility::Position(randomDirection, position.x, position.y));
      work.push(pivotPosition);
    }
  }
}

void ColorfulImageGenerator::recursivelyDrawGraph(cs225::PNG &image, bool branching, CanvasUtility::Position const position) {
  unsigned int randomNumber = std::rand() % 10;

  if (position.direction == CanvasUtility::CardinalDirection::Null || !CanvasUtility::inBounds(image, position.x, position.y))
    return;

  if (branching == true) {
    recursivelyDrawBranch(image, position);
  } else if (randomNumber < 2) {
    CanvasUtility::drawSquare(image, NODE_MARKER, SQUARE_WIDTH, position.x, position.y);
    recursivelyDrawGraph(image, true, position);
  } else {
    randomNumber = std::rand() % 10;
    unsigned edgeLength = static_cast<unsigned>(EDGE_LENGTH);

    if (randomNumber < 4)
      edgeLength *= 3;
    else if (randomNumber < 6)
      edgeLength *= 9;

    auto pivotDirection = static_cast<CanvasUtility::CardinalDirection>(std::rand() % 4);

    CanvasUtility::Position pivotPosition = drawEdge(image, edgeLength, CanvasUtility::Position(pivotDirection, position.x, position.y));
    recursivelyDrawGraph(image, false, pivotPosition); // todo account for null direction
  }
}

CanvasUtility::Position ColorfulImageGenerator::drawEdge(cs225::PNG &png, unsigned int edgeLength, CanvasUtility::Position position) {
  for (unsigned i = 0; i < edgeLength; i++) {
    switch (position.direction) {
      case North: position.y += 1; break;
      case South: position.y -= 1; break;
      case East: position.x += 1; break;
      case West: position.x -= 1; break;
      default: throw std::logic_error("attempted to draw edge with null position");
    }

    if (!CanvasUtility::inBounds(png, position.x, position.y) || isNodePixel(png.getPixel(position.x, position.y)))
      return CanvasUtility::Position(CanvasUtility::CardinalDirection::Null, position.x, position.y);

    switch(position.direction) {
      case North: case South:
        CanvasUtility::drawLine(png, EDGE_MARKER, LINE_LENGTH, CanvasUtility::Position(West, position.x, position.y));
        CanvasUtility::drawLine(png, EDGE_MARKER, LINE_LENGTH, CanvasUtility::Position(East, position.x, position.y));
        break;
      case East: case West:
        CanvasUtility::drawLine(png, EDGE_MARKER, LINE_LENGTH, CanvasUtility::Position(North, position.x, position.y));
        CanvasUtility::drawLine(png, EDGE_MARKER, LINE_LENGTH, CanvasUtility::Position(South, position.x, position.y));
        break;
      default:
        throw std::logic_error("bad Direction");
    }

    CanvasUtility::paintIfInBounds(png, EDGE_MARKER, position.x, position.y); // this is the "inside" of the drawn edge
  }

  return position;
}

void ColorfulImageGenerator::drawBranch(cs225::PNG &png, CanvasUtility::Position const position, std::stack<CanvasUtility::Position> &work) {
  unsigned const
    northY = position.y + static_cast<unsigned>(SQUARE_WIDTH / 2.0),
    southY = position.y - static_cast<unsigned>(SQUARE_WIDTH / 2.0),
    eastX = position.x + static_cast<unsigned>(SQUARE_WIDTH / 2.0),
    westX = position.x - static_cast<unsigned>(SQUARE_WIDTH / 2.0);

  std::unordered_map<CanvasUtility::CardinalDirection, CanvasUtility::Position> const getBranchPosition = {
    {North, CanvasUtility::Position(North, position.x, northY)},
    {South, CanvasUtility::Position(South, position.x, southY)},
    {East, CanvasUtility::Position(East, eastX, position.y)},
    {West, CanvasUtility::Position(West, westX, position.y)}
  };

  unsigned wheelIndex = position.direction;
  unsigned ignoreIndex;
  switch (position.direction) {
    case North: ignoreIndex = South; break;
    case South: ignoreIndex = North; break;
    case East: ignoreIndex = West; break;
    case West: ignoreIndex = East; break;
    default: return;
  }

  for (unsigned int branchCount = 0; branchCount < 3; ++wheelIndex) {
    wheelIndex %= 4; // wheel behavior
    if (wheelIndex == ignoreIndex)
      continue;

    CanvasUtility::Position pivotPosition = drawEdge(
      png, static_cast<unsigned>(EDGE_LENGTH),
      getBranchPosition.at(static_cast<CanvasUtility::CardinalDirection>(wheelIndex))
    );

    work.push(pivotPosition);
    branchCount += 1;
  }
}

void ColorfulImageGenerator::recursivelyDrawBranch(cs225::PNG &png, CanvasUtility::Position const position) {
  unsigned const
    northY = position.y + static_cast<unsigned>(SQUARE_WIDTH / 2.0),
    southY = position.y - static_cast<unsigned>(SQUARE_WIDTH / 2.0), 
    eastX = position.x + static_cast<unsigned>(SQUARE_WIDTH / 2.0),
    westX = position.x - static_cast<unsigned>(SQUARE_WIDTH / 2.0);

  std::unordered_map<CanvasUtility::CardinalDirection, CanvasUtility::Position> const getBranchPosition = {
    {North, CanvasUtility::Position(North, position.x, northY)},
    {South, CanvasUtility::Position(South, position.x, southY)},
    {East, CanvasUtility::Position(East, eastX, position.y)},
    {West, CanvasUtility::Position(West, westX, position.y)}
  };

  unsigned wheelIndex = position.direction;
  unsigned ignoreIndex;
  switch (position.direction) {
    case North: ignoreIndex = South; break;
    case South: ignoreIndex = North; break;
    case East: ignoreIndex = West; break;
    case West: ignoreIndex = East; break;
    default: return;
  }

  for (unsigned int branchCount = 0; branchCount < 3; ++wheelIndex) {
    wheelIndex %= 4; // wheel behavior
    if (wheelIndex == ignoreIndex)
      continue;

    recursivelyDrawGraph(png, false, drawEdge(png, static_cast<unsigned>(EDGE_LENGTH),
      getBranchPosition.at(static_cast<CanvasUtility::CardinalDirection>(wheelIndex)))
    );
    branchCount += 1;
  }
}

bool ColorfulImageGenerator::isNodePixel(cs225::HSLAPixel pixel) {
  return pixel.l == NODE_LUMINANCE_MARKER;
}

bool ColorfulImageGenerator::isEdgePixel(cs225::HSLAPixel pixel) {
  return pixel.l == EDGE_LUMINANCE_MARKER;
}
