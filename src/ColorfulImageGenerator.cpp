#include "ColorfulImageGenerator.hpp"
#include <unordered_map>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>

using CanvasUtility::CardinalDirection::North;
using CanvasUtility::CardinalDirection::South;
using CanvasUtility::CardinalDirection::East;
using CanvasUtility::CardinalDirection::West;

cs225::PNG ColorfulImageGenerator::generate(unsigned width, unsigned height) {
  cs225::PNG image(width, height);
  const unsigned MIDDLE_X = static_cast<unsigned>(image.width() / 2.0);
  const unsigned MIDDLE_Y = static_cast<unsigned>(image.height() / 2.0);
  drawGraph(image);

  std::vector<std::pair<unsigned, unsigned>> auraOrigins = {
    {MIDDLE_X, MIDDLE_Y},
    {0, 0},
    {0, image.height() - 1},
    {image.width() - 1, image.height() - 1},
    {image.width() - 1, 0}
  };

  std::shuffle(std::begin(auraOrigins), std::end(auraOrigins), std::default_random_engine(static_cast<unsigned>(std::rand())));
  size_t const index = 0;
  drawAura(image, auraOrigins.at(index).first, auraOrigins.at(index).second);
  drawAura(image, auraOrigins.at(index + 1).first, auraOrigins.at(index + 1).second);

  return image;
}

cs225::PNG ColorfulImageGenerator::generate(unsigned width, unsigned height, unsigned seed) {
  std::srand(seed); // set seed
  return generate(width, height);
}

int count = 0;
void ColorfulImageGenerator::drawGraph(cs225::PNG &image) {
  const unsigned MIDDLE_X = static_cast<unsigned>(image.width() / 2.0);
  const unsigned MIDDLE_Y = static_cast<unsigned>(image.height() / 2.0);
  const CanvasUtility::Position MIDDLE_POSITION(North, MIDDLE_X, MIDDLE_Y);

  CanvasUtility::drawSquare(image, NODE_MARKER, SQUARE_WIDTH, MIDDLE_X, MIDDLE_Y); // first node
  std::stack<CanvasUtility::Position> work;
  drawBranch(image, MIDDLE_POSITION, work);

  while (!work.empty()) {
    CanvasUtility::Position position(work.top()); work.pop();

    // base case
    if (position.direction == CanvasUtility::CardinalDirection::Null || !CanvasUtility::inBounds(image, position.x, position.y))
      continue;

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
      if (randomDirection == CanvasUtility::reverse(position.direction))
        randomDirection = position.direction; // no going backwards

      CanvasUtility::Position pivotPosition = drawEdge(image, edgeLength, CanvasUtility::Position(randomDirection, position.x, position.y));
      work.push(pivotPosition);
    }
  }
}

CanvasUtility::Position ColorfulImageGenerator::drawEdge(cs225::PNG &image, unsigned int edgeLength, CanvasUtility::Position position) {
  static auto const invalid_direction_error = std::logic_error("invalid direction on draw edge");

  for (unsigned i = 0; i < edgeLength; i++) {
    switch (position.direction) {
      case North: position.y += 1; break;
      case South: position.y -= 1; break;
      case East: position.x += 1; break;
      case West: position.x -= 1; break;
      default: throw invalid_direction_error;
    }

    if (!CanvasUtility::inBounds(image, position.x, position.y) || isNodePixel(image.getPixel(position.x, position.y)))
      return CanvasUtility::Position(CanvasUtility::CardinalDirection::Null, position.x, position.y);

    switch(position.direction) {
      case North: case South:
        CanvasUtility::drawLine(image, EDGE_MARKER, LINE_LENGTH, CanvasUtility::Position(West, position.x, position.y));
        CanvasUtility::drawLine(image, EDGE_MARKER, LINE_LENGTH, CanvasUtility::Position(East, position.x, position.y));
        break;
      case East: case West:
        CanvasUtility::drawLine(image, EDGE_MARKER, LINE_LENGTH, CanvasUtility::Position(North, position.x, position.y));
        CanvasUtility::drawLine(image, EDGE_MARKER, LINE_LENGTH, CanvasUtility::Position(South, position.x, position.y));
        break;
      default:
        throw invalid_direction_error;
    }

    CanvasUtility::paintIfInBounds(image, EDGE_MARKER, position.x, position.y); // this is the "inside" of the drawn edge
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

bool ColorfulImageGenerator::isNodePixel(cs225::HSLAPixel pixel) {
  return pixel.l == NODE_LUMINANCE_MARKER;
}

bool ColorfulImageGenerator::isEdgePixel(cs225::HSLAPixel pixel) {
  return pixel.l == EDGE_LUMINANCE_MARKER;
}

ColorfulImageGenerator::ColorWheel::ColorWheel(double color) {
  setColor(color);
}

double ColorfulImageGenerator::ColorWheel::getColor() const {
  return _color;
}

#include <iostream>
#include <cassert>
void ColorfulImageGenerator::ColorWheel::setColor(double const color) {
  const double FULL_CYCLE = 360.0; // (end exclusive)

  // set
  _color = color;

  // attempt bound
  while (_color < 0.0)
    _color += FULL_CYCLE;

  while (_color >= FULL_CYCLE)
    _color -= FULL_CYCLE;

  // clamp
  if (_color < 0.0 || _color >= FULL_CYCLE)
    _color = 0.0;
}

void ColorfulImageGenerator::ColorWheel::addColor(double offset) {
  setColor(_color + offset);
}

void ColorfulImageGenerator::drawAura(cs225::PNG &image, unsigned centerX, unsigned centerY) {
  double const SMALLEST_LENGTH = std::min(image.width(), image.height());
  ColorWheel colorWheel(0.0);
  double unitOfColor = 360.0 / static_cast<double>(SMALLEST_LENGTH);

  for (unsigned x = 0; x < image.width(); x++) {
    for (unsigned y = 0; y < image.height(); y++) {
      // distance in pixels
      const double distance = std::sqrt(
        std::pow(static_cast<int>(x) - static_cast<int>(centerX), 2) + std::pow(static_cast<int>(y) - static_cast<int>(centerY), 2)
      );
      cs225::HSLAPixel pixel = image.getPixel(x, y);
      double const unitsOfColor = unitOfColor * distance;

      colorWheel.setColor(pixel.h);
      if (isNodePixel(pixel) || isEdgePixel(pixel))
        colorWheel.addColor(-unitsOfColor);
      else
        colorWheel.addColor(unitsOfColor);
      CanvasUtility::paintIfInBounds(image, cs225::HSLAPixel(colorWheel.getColor(), 1.0, 0.35), x, y);

    }
  }
}
