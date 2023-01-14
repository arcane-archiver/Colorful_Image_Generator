#include "../hdr/cs225_ArtHelper.hpp"
#include <stdexcept>
#include <cmath>

ArtHelper::Position::Position(ArtHelper::Direction setD, unsigned int setX, unsigned int setY): d(setD), x(setX), y(setY) { }

ArtHelper::ArtHelper(unsigned int smallestLength):
  SMALLEST_LENGTH(smallestLength),
  SQUARE_WIDTH(15), EDGE_LENGTH(7),
  LINE_LENGTH(1),
  NODE_COLOR(ILLINI_ORANGE),
  EDGE_COLOR(ILLINI_BLUE)
{
  SCALE = (double)smallestLength / 800.0;

  if (smallestLength < 80) {
    SCALE = 0.1;
  } else if
  /** STOPS SEG FAULTS
   * Without this booelan expr, along with iterate.py, makes interesting dissolved look.
   */
  (smallestLength >= 800) {
    SQUARE_WIDTH *= SCALE;
    EDGE_LENGTH *= SCALE;
    LINE_LENGTH *= SCALE;
  }

  RECURSION_COUNT = 0;
  RECURSION_MAX = 50000; // 94162 is the tested max on EWS
}

void ArtHelper::drawGraph(cs225::PNG &png) {
  const unsigned int MIDDLE_X = std::ceil(png.width() / 2.0);
  const unsigned int MIDDLE_Y = std::ceil(png.height() / 2.0);

  drawSquare(png, ArtHelper::Direction::North, MIDDLE_X, MIDDLE_Y); // first node
  recursivelyDrawGraph(png, true, ArtHelper::Direction::North, MIDDLE_X, MIDDLE_Y);
}

void ArtHelper::dumpBlackPaint(cs225::PNG & png) {
  for (unsigned x = 0; x < png.width(); x++) {
    for (unsigned y = 0; y < png.height(); y++) {
      paint(png, cs225::HSLAPixel(0.0, 1.0, 0.0), x, y);
    }
  }
}

bool ArtHelper::inBounds(const cs225::PNG & png, const unsigned int x, const unsigned int y) {
  return (0 <= x && x < png.width()) && (0 <= y && y < png.height());
}

void ArtHelper::paint(cs225::PNG & canvas, cs225::HSLAPixel paint, unsigned int x, unsigned int y) {
  bool isPaintingGraph = paint.h == NODE_COLOR || paint.h == EDGE_COLOR || paint.h == NICE_COLOR;
  // isPaintingGraph = false; // seg fault
  isPaintingGraph = true;

  if (inBounds(canvas, x, y) && (isPaintingGraph)) {
    canvas.getPixel(x, y) = paint;
  } else if (inBounds(canvas, x, y) && paint.l == 0.0 /* dump */) {
    canvas.getPixel(x, y) = paint;
  }
}

void ArtHelper::drawLine(cs225::PNG & png, double lineLength, Direction d, double x, double y) {
  if (d == Direction::North) {
    for (unsigned i = 0; i < lineLength; i++) {
      y += 1;
      paint(png, cs225::HSLAPixel(EDGE_COLOR, 1.0, 0.5), x, y);
    }
  } else if (d == Direction::South) {
    for (unsigned i = 0; i < lineLength; i++) {
      y -= 1;
      paint(png, cs225::HSLAPixel(EDGE_COLOR, 1.0, 0.5), x, y);
    }
  } else if (d == Direction::East) {
    for (unsigned i = 0; i < lineLength; i++) {
      x += 1;
      paint(png, cs225::HSLAPixel(EDGE_COLOR, 1.0, 0.5), x, y);
    }
  } else /* West */ {
    for (unsigned i = 0; i < lineLength; i++) {
      x -= 1;
      paint(png, cs225::HSLAPixel(EDGE_COLOR, 1.0, 0.5), x, y);
    }
  }
}

void ArtHelper::drawSquare(cs225::PNG & png, Direction const d, double const x_middle, double const y_middle) {
  // start corner of square
  const unsigned start_x = x_middle - std::floor(SQUARE_WIDTH / 2);
  const unsigned start_y = y_middle - std::floor(SQUARE_WIDTH / 2);

  // end corner of square
  const unsigned end_x = x_middle + std::floor(SQUARE_WIDTH / 2);
  const unsigned end_y = y_middle + std::floor(SQUARE_WIDTH / 2);

  for (unsigned x = start_x; x <= end_x; x++)
    for (unsigned y = start_y; y <= end_y; y++)
      paint(png, cs225::HSLAPixel(NODE_COLOR, 1.0, 0.5), x, y);
}

ArtHelper::Position ArtHelper::drawNode(
  cs225::PNG & png, cs225::HSLAPixel pixel, Direction const d,
  double radius, double const h, double const k
) {
  using std::sqrt;
  using std::pow;
  signed int y1 = k;
  signed int y2 = k;
  for (signed x = h - radius; x <= h + radius; x++) {
    y1 = sqrt(pow(radius, 2) - pow(x - h, 2)) + k;
    y2 = 0 - sqrt(pow(radius, 2) - pow(x - h, 2)) + k;
    for (signed row = y1; row >= y2; row--) {
      paint(png, pixel, x, row);
    }
  }
  return Position(d, h, k);
}

void ArtHelper::drawAura(cs225::PNG & png, unsigned int centerX, unsigned int centerY) {
  double colorIndex = 0.0;
  double unitOfColor = 360.0 / (double)SMALLEST_LENGTH;

  for (double x = 0; x < (double)png.width(); x++) {
    for (double y = 0; y < (double)png.height(); y++) {
      const double distance = std::sqrt(std::pow(x - centerX, 2) + std::pow(y - centerY, 2)); // distance in pixels
      cs225::HSLAPixel pixel = png.getPixel(x, y);
      if (pixel.h == NODE_COLOR) {
        colorIndex = pixel.h;
        iterateColorIndex(colorIndex, 0 - (unitOfColor * distance));
        // iterateColorIndex(colorIndex, unitOfColor * distance);
        paint(png, cs225::HSLAPixel(colorIndex, 1.0, 0.35), x, y);
      } else if (pixel.h != NICE_COLOR) {
        colorIndex = pixel.h;
        iterateColorIndex(colorIndex, unitOfColor * distance);
        paint(png, cs225::HSLAPixel(colorIndex, 1.0, 0.35), x, y);
      }
    }
  }
}

ArtHelper::Position ArtHelper::drawEdge(cs225::PNG & png, unsigned int edgeLength, Direction d, unsigned int x, unsigned int y) {
  for (unsigned i = 0; i < edgeLength; i++) {
    switch (d) {
      case Direction::North: y += 1; break;
      case Direction::South: y -= 1; break;
      case Direction::East: x += 1; break;
      case Direction::West: x -= 1; break;
      case Direction::STOP: throw std::logic_error("attempted to draw edge with Direction::STOP");
    }
    if (!inBounds(png, x, y) || png.getPixel(x, y).h == NODE_COLOR) {
      return Position(Direction::STOP, x, y);
    }
    if (d == Direction::North || d == Direction::South) {
      drawLine(png, LINE_LENGTH, Direction::East, x, y);
      drawLine(png, LINE_LENGTH, Direction::West, x, y);
    } else if (d == Direction::East || d == Direction::West) {
      drawLine(png, LINE_LENGTH, Direction::North, x, y);
      drawLine(png, LINE_LENGTH, Direction::South, x, y);
    } else {throw std::logic_error("bad Direction d");}
    paint(png, cs225::HSLAPixel(EDGE_COLOR, 1.0, 0.5), x, y);
  }
  return Position(d, x, y);
}

void ArtHelper::drawBranch(cs225::PNG & png, Direction d, unsigned int x, unsigned int y) {
  Position pos = Position(d, x, y);
  unsigned int northY = y + std::floor(SQUARE_WIDTH / 2.0);
  unsigned int southY = y - std::floor(SQUARE_WIDTH / 2.0); 
  unsigned int eastX = x + std::floor(SQUARE_WIDTH / 2.0);
  unsigned int westX = x - std::floor(SQUARE_WIDTH / 2.0);
  switch (d) {
    case Direction::North:
      pos = drawEdge(png, EDGE_LENGTH, Direction::North, x, northY);
      recursivelyDrawGraph(png, false, pos.d, pos.x, pos.y);
      pos = drawEdge(png, EDGE_LENGTH, Direction::East, eastX, y);
      recursivelyDrawGraph(png, false, pos.d, pos.x, pos.y);
      pos = drawEdge(png, EDGE_LENGTH, Direction::West, westX, y);
      recursivelyDrawGraph(png, false, pos.d, pos.x, pos.y); break;
    case Direction::South:
      pos = drawEdge(png, EDGE_LENGTH, Direction::South, x, southY);
      recursivelyDrawGraph(png, false, pos.d, pos.x, pos.y);
      pos = drawEdge(png, EDGE_LENGTH, Direction::East, eastX, y);
      recursivelyDrawGraph(png, false, pos.d, pos.x, pos.y);
      pos = drawEdge(png, EDGE_LENGTH, Direction::West, westX, y);
      recursivelyDrawGraph(png, false, pos.d, pos.x, pos.y); break;
    case Direction::East:
      pos = drawEdge(png, EDGE_LENGTH, Direction::East, eastX, y);
      recursivelyDrawGraph(png, false, pos.d, pos.x, pos.y);
      pos = drawEdge(png, EDGE_LENGTH, Direction::North, x, northY);
      recursivelyDrawGraph(png, false, pos.d, pos.x, pos.y);
      pos = drawEdge(png, EDGE_LENGTH, Direction::South, x, southY);
      recursivelyDrawGraph(png, false, pos.d, pos.x, pos.y); break;
    case Direction::West:
      pos = drawEdge(png, EDGE_LENGTH, Direction::West, westX, y);
      recursivelyDrawGraph(png, false, pos.d, pos.x, pos.y);
      pos = drawEdge(png, EDGE_LENGTH, Direction::North, x, northY);
      recursivelyDrawGraph(png, false, pos.d, pos.x, pos.y);
      pos = drawEdge(png, EDGE_LENGTH, Direction::South, x, southY); 
      recursivelyDrawGraph(png, false, pos.d, pos.x, pos.y); break;
    case Direction::STOP: throw std::logic_error("in branch, Direction::STOP");
  }
}

void ArtHelper::recursivelyDrawGraph(cs225::PNG & png, bool branching, Direction d, unsigned int x, unsigned int y) {
  RECURSION_COUNT += 1;
  if (RECURSION_COUNT >= RECURSION_MAX)
    return;

  unsigned int randomNumber = std::rand() % 10;

  Position pos = Position(d, x, y);

  if (d == Direction::STOP || !inBounds(png, x, y))
    return;

  if (branching == true) {
    drawBranch(png, d, x, y);
  } else if (randomNumber < 2) {
    drawSquare(png, d, x, y);
    recursivelyDrawGraph(png, true, d, x, y);
  } else {
    randomNumber = std::rand() % 10;
    unsigned int edgeLength = EDGE_LENGTH;
    if (randomNumber < 4) {edgeLength *= 3;} else if (randomNumber < 6) {edgeLength *= 9;}
    randomNumber = std::rand() % 4;
    switch(randomNumber) {
      case 0: d = Direction::North; break;
      case 1: d = Direction::South; break;
      case 2: d = Direction::East; break;
      case 3: d = Direction::West; break;
      default: throw std::logic_error("randNum is not in [0, 3]");
    }

    pos = drawEdge(png, edgeLength, d, x, y);
    recursivelyDrawGraph(png, false, pos.d, pos.x, pos.y);
  }
}

void ArtHelper::iterateColorIndex(double & i, double k) {
  const double START_INCLSV = 0.0;
  const double END_EXCLSV = 360.0;
  const double CUT = 0.0;
  const double CYCLE = 360.0 - CUT;
  i += k;
  while (i < START_INCLSV) {i += CYCLE;}
  while (i >= END_EXCLSV) {i -= CYCLE;}
}
