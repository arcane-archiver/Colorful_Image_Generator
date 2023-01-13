#include "../cs225/PNG.h"
#include "../cs225/HSLAPixel.h"

#include <string>


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

enum Direction {N, S, E, W, STOP};
struct Position {
  Direction d;
  unsigned int x;
  unsigned int y;
  Position(Direction setD, unsigned int setX, unsigned int setY): d(setD), x(setX), y(setY) {}
};

class ArtHelper {
  public:
  double SMALLEST_LENGTH;
  double SCALE;
    double BOUNDARY_CORNER_MIN;
    double BOUNDARY_CORNER_MAX;
  double SQUARE_WIDTH;
  double EDGE_LENGTH;
  double LINE_LENGTH;
  double NODE_COLOR;
  double EDGE_COLOR;
  unsigned int RECURSION_COUNT;
  unsigned int RECURSION_MAX;

  ArtHelper() = delete;
  ArtHelper(unsigned int smallestLength):
    SMALLEST_LENGTH(smallestLength),
    SQUARE_WIDTH(15), EDGE_LENGTH(7),
    LINE_LENGTH(1),
    NODE_COLOR(ILLINI_ORANGE),
    EDGE_COLOR(ILLINI_BLUE)
  {
    SCALE = (double)smallestLength / 800.0;

    unsigned int borderSpace = static_cast<unsigned int>(200 * SCALE);
    BOUNDARY_CORNER_MIN = borderSpace;
    BOUNDARY_CORNER_MAX = smallestLength - borderSpace;

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

  void dump(cs225::PNG & png) {
    for (unsigned x = 0; x < png.width(); x++) {
      for (unsigned y = 0; y < png.height(); y++) {
        paint(png, cs225::HSLAPixel(0.0, 1.0, 0.0), x, y);
      }
    }
  }

  bool inBounds(const cs225::PNG & png, const unsigned int x, const unsigned int y) {
    return (0 <= x && x < png.width()) && (0 <= y && y < png.height());
  }

  cs225::HSLAPixel & safePixel(cs225::PNG & png, const unsigned int x, const unsigned int y) {
    if (inBounds(png, x, y)) {return png.getPixel(x, y);} else {throw std::logic_error(
      "unsafe pixel: (" + std::to_string(x) + ", " + std::to_string(y) + ")"
    );}
  }

  std::string ICON = "\\";
  void iterateIcon() {
    if (ICON == "|") {
      ICON = "/";
    } else if (ICON == "/") {
      ICON = "-";
    } else if (ICON == "-") {
      ICON = "\\";
    } else if (ICON == "\\") {
      ICON = "|";
    } else {throw std::logic_error("bad ICON");}
  }
  void paint(cs225::PNG & png, cs225::HSLAPixel pixel, unsigned int x, unsigned int y) {
    bool outsideBoundary = (
      !(
        /* inside */
        (BOUNDARY_CORNER_MIN <= x && x <= BOUNDARY_CORNER_MAX) &&
        (BOUNDARY_CORNER_MIN <= y && y <= BOUNDARY_CORNER_MAX)
      )
    );
    bool drawingSpecial = pixel.h == NODE_COLOR || pixel.h == EDGE_COLOR || pixel.h == NICE_COLOR;
  
    if (inBounds(png, x, y) && (outsideBoundary || drawingSpecial)) {
      safePixel(png, x, y) = pixel;
      iterateIcon();
    } else if (inBounds(png, x, y) && pixel.l == 0.0 /* dump */) {
      safePixel(png, x, y) = pixel;
      iterateIcon();
    }
  }

  void drawLine(cs225::PNG & png, double lineLength, Direction d, double x, double y) {
    if (d == Direction::N) {
      for (unsigned i = 0; i < lineLength; i++) {
        y += 1;
        paint(png, cs225::HSLAPixel(EDGE_COLOR, 1.0, 0.5), static_cast<unsigned int>(x), static_cast<unsigned int>(y));
      }
    } else if (d == Direction::S) {
      for (unsigned i = 0; i < lineLength; i++) {
        y -= 1;
        paint(png, cs225::HSLAPixel(EDGE_COLOR, 1.0, 0.5), static_cast<unsigned int>(x), static_cast<unsigned int>(y));
      }
    } else if (d == Direction::E) {
      for (unsigned i = 0; i < lineLength; i++) {
        x += 1;
        paint(png, cs225::HSLAPixel(EDGE_COLOR, 1.0, 0.5), static_cast<unsigned int>(x), static_cast<unsigned int>(y));
      }
    } else /* West */ {
      for (unsigned i = 0; i < lineLength; i++) {
        x -= 1;
        paint(png, cs225::HSLAPixel(EDGE_COLOR, 1.0, 0.5), static_cast<unsigned int>(x), static_cast<unsigned int>(y));
      }
    }
  }

  Position drawSquare(cs225::PNG & png, Direction d, double x, double y) {
    for (
      unsigned x_idx = static_cast<unsigned>(x - std::floor(SQUARE_WIDTH / 2));
      x_idx <= x + std::floor(SQUARE_WIDTH / 2);
      x_idx++
    ) {
      for (
        unsigned y_idx = static_cast<unsigned>(y - std::floor(SQUARE_WIDTH / 2));
        y_idx <= y + std::floor(SQUARE_WIDTH / 2);
        y_idx++
      ) {
        paint(png, cs225::HSLAPixel(NODE_COLOR, 1.0, 0.5), x_idx, y_idx);
      }
    }

    return Position(d, static_cast<unsigned int>(x), static_cast<unsigned int>(y));
  }

  Position drawCircle(
    cs225::PNG & png, cs225::HSLAPixel pixel, Direction d,
    double radius, double h, double k
  ) {
    using std::sqrt;
    using std::pow;
    signed int y1 = static_cast<signed int>(k);
    signed int y2 = static_cast<signed int>(k);
    for (signed x = static_cast<signed>(h - radius); x <= h + radius; x++) {
      y1 = static_cast<signed int>(sqrt(pow(radius, 2) - pow(x - h, 2)) + k);
      y2 = 0 - static_cast<signed int>(sqrt(pow(radius, 2) - pow(x - h, 2)) + k);
      for (signed row = y1; row >= y2; row--) {
        double hue = pixel.h;
        try {
          hue = safePixel(png, static_cast<unsigned int>(x), static_cast<unsigned int>(row)).h;
        } catch (std::logic_error e) {
          break;
        }
        if (hue == NODE_COLOR) {paint(png, pixel, static_cast<unsigned int>(x), static_cast<unsigned int>(row));}
        else if (hue == EDGE_COLOR) {continue;}
        else {paint(png, cs225::HSLAPixel(pixel.h, pixel.s, pixel.l + 0.1), static_cast<unsigned int>(x), static_cast<unsigned int>(row));}
      }
    }
    return Position(d, static_cast<unsigned>(h), static_cast<unsigned>(k));
  }

  Position drawNode(
    cs225::PNG & png, cs225::HSLAPixel pixel, Direction d,
    double radius, double h, double k
  ) {
    using std::sqrt;
    using std::pow;
    signed int y1 = static_cast<signed>(k);
    signed int y2 = static_cast<signed>(k);
    for (signed x = static_cast<signed>(h - radius); x <= h + radius; x++) {
      y1 = static_cast<signed>(sqrt(pow(radius, 2) - pow(x - h, 2)) + k);
      y2 = 0 - static_cast<signed>(sqrt(pow(radius, 2) - pow(x - h, 2)) + k);
      for (signed row = y1; row >= y2; row--) {
        paint(png, pixel, static_cast<unsigned>(x), static_cast<unsigned>(row));
      }
    }
    return Position(d, static_cast<unsigned>(h), static_cast<unsigned>(k));
  }

  void drawParabola(cs225::PNG & png, cs225::HSLAPixel pixel, double a, double k, double h) {
    using std::pow;
    double y = h;
    for (double x = 0; x < (signed)png.width(); x++) {
      y = a * pow(x - k, 2) + h;
      paint(png, pixel, static_cast<unsigned>(x), static_cast<unsigned>(y));
    }
  }

  void drawSmile(cs225::PNG & png, cs225::HSLAPixel pixel) {
    using std::pow;
    double a1 = 1.0/(375.0 * SCALE); double k1 = (400.0) * SCALE; double h1 = (300.0) * SCALE; // above
    double a2 = 1.0/(160.0 * SCALE); double k2 = (400.0) * SCALE; double h2 = (230.0) * SCALE; // below

    for (signed x = 0; x < (signed int)png.width(); x++) {
      double y1 = a1 * pow(x - k1, 2) + h1;
      double y2 = a2 * pow(x - k2, 2) + h2;
      for (signed row = static_cast<signed>(y1); row >= y2; row--) {
        double hue = pixel.h;
        try {
          hue = safePixel(png, static_cast<unsigned>(x), static_cast<unsigned>(row)).h;
        } catch (std::logic_error) {
          break;
        }
        if (hue == NODE_COLOR) {paint(png, pixel, static_cast<unsigned>(x), static_cast<unsigned>(row));}
        else if (hue == EDGE_COLOR) {continue;}
        else {paint(png, cs225::HSLAPixel(pixel.h, pixel.s, pixel.l + 0.1), static_cast<unsigned>(x), static_cast<unsigned>(row));}
      }
    }
  }

  void drawAura(cs225::PNG & png, unsigned int centerX, unsigned int centerY) {
    double colorIndex = 0.0;
    double unitOfColor = 360.0 / (double)SMALLEST_LENGTH;

    for (double x = 0; x < (double)png.width(); x++) {
      for (double y = 0; y < (double)png.height(); y++) {
        const double distance = std::sqrt(std::pow(x - centerX, 2) + std::pow(y - centerY, 2)); // distance in pixels
        cs225::HSLAPixel pixel = safePixel(png, static_cast<unsigned>(x), static_cast<unsigned>(y));
        if (pixel.h == NODE_COLOR) {
          colorIndex = pixel.h;
          iterateColorIndex(colorIndex, 0 - (unitOfColor * distance));
          // iterateColorIndex(colorIndex, unitOfColor * distance);
          paint(png, cs225::HSLAPixel(colorIndex, 1.0, 0.35), static_cast<unsigned>(x), static_cast<unsigned>(y));
        } else if (pixel.h != NICE_COLOR) {
          colorIndex = pixel.h;
          iterateColorIndex(colorIndex, unitOfColor * distance);
          paint(png, cs225::HSLAPixel(colorIndex, 1.0, 0.35), static_cast<unsigned>(x), static_cast<unsigned>(y));
        }
      }
    }
  }

  Position drawEdge(cs225::PNG & png, unsigned int edgeLength, Direction d, unsigned int x, unsigned int y) {
    for (unsigned i = 0; i < edgeLength; i++) {
      switch (d) {
        case Direction::N: y += 1; break;
        case Direction::S: y -= 1; break;
        case Direction::E: x += 1; break;
        case Direction::W: x -= 1; break;
        case Direction::STOP: throw std::logic_error("attempted to draw edge with Direction::STOP");
      }
      if (!inBounds(png, x, y) || safePixel(png, x, y).h == NODE_COLOR) {
        return Position(Direction::STOP, x, y);
      }
      if (d == Direction::N || d == Direction::S) {
        drawLine(png, LINE_LENGTH, Direction::E, x, y);
        drawLine(png, LINE_LENGTH, Direction::W, x, y);
      } else if (d == Direction::E || d == Direction::W) {
        drawLine(png, LINE_LENGTH, Direction::N, x, y);
        drawLine(png, LINE_LENGTH, Direction::S, x, y);
      } else {throw std::logic_error("bad Direction d");}
      paint(png, cs225::HSLAPixel(EDGE_COLOR, 1.0, 0.5), x, y);
    }
    return Position(d, x, y);
  }

  void drawBranch(cs225::PNG & png, Direction d, unsigned int x, unsigned int y) {
    Position pos = Position(d, x, y);
    unsigned int northY = y + static_cast<unsigned>(std::floor(SQUARE_WIDTH / 2.0));
    unsigned int southY = y - static_cast<unsigned>(std::floor(SQUARE_WIDTH / 2.0)); 
    unsigned int eastX = x + static_cast<unsigned>(std::floor(SQUARE_WIDTH / 2.0));
    unsigned int westX = x - static_cast<unsigned>(std::floor(SQUARE_WIDTH / 2.0));
    switch (d) {
      case Direction::N:
        pos = drawEdge(png, static_cast<unsigned>(EDGE_LENGTH), Direction::N, x, northY);
        recursivelyDraw(png, false, pos.d, pos.x, pos.y);
        pos = drawEdge(png, static_cast<unsigned>(EDGE_LENGTH), Direction::E, eastX, y);
        recursivelyDraw(png, false, pos.d, pos.x, pos.y);
        pos = drawEdge(png, static_cast<unsigned>(EDGE_LENGTH), Direction::W, westX, y);
        recursivelyDraw(png, false, pos.d, pos.x, pos.y); break;
      case Direction::S:
        pos = drawEdge(png, static_cast<unsigned>(EDGE_LENGTH), Direction::S, x, southY);
        recursivelyDraw(png, false, pos.d, pos.x, pos.y);
        pos = drawEdge(png, static_cast<unsigned>(EDGE_LENGTH), Direction::E, eastX, y);
        recursivelyDraw(png, false, pos.d, pos.x, pos.y);
        pos = drawEdge(png, static_cast<unsigned>(EDGE_LENGTH), Direction::W, westX, y);
        recursivelyDraw(png, false, pos.d, pos.x, pos.y); break;
      case Direction::E:
        pos = drawEdge(png, static_cast<unsigned>(EDGE_LENGTH), Direction::E, eastX, y);
        recursivelyDraw(png, false, pos.d, pos.x, pos.y);
        pos = drawEdge(png, static_cast<unsigned>(EDGE_LENGTH), Direction::N, x, northY);
        recursivelyDraw(png, false, pos.d, pos.x, pos.y);
        pos = drawEdge(png, static_cast<unsigned>(EDGE_LENGTH), Direction::S, x, southY);
        recursivelyDraw(png, false, pos.d, pos.x, pos.y); break;
      case Direction::W:
        pos = drawEdge(png, static_cast<unsigned>(EDGE_LENGTH), Direction::W, westX, y);
        recursivelyDraw(png, false, pos.d, pos.x, pos.y);
        pos = drawEdge(png, static_cast<unsigned>(EDGE_LENGTH), Direction::N, x, northY);
        recursivelyDraw(png, false, pos.d, pos.x, pos.y);
        pos = drawEdge(png, static_cast<unsigned>(EDGE_LENGTH), Direction::S, x, southY); 
        recursivelyDraw(png, false, pos.d, pos.x, pos.y); break;
      case Direction::STOP: throw std::logic_error("in branch, Direction::STOP");
    }
  }

  void recursivelyDraw(cs225::PNG & png, bool branching, Direction d, unsigned int x, unsigned int y) {
    RECURSION_COUNT += 1;
    if (RECURSION_COUNT >= RECURSION_MAX) {return;}

    unsigned int randNum = std::rand() % 10;
    Position pos = Position(d, x, y);
    if (d == Direction::STOP || !inBounds(png, x, y)) {return;}
    if (branching == true) {
      drawBranch(png, d, x, y);
    } else if (randNum < 2) {
      pos = drawSquare(png, d, x, y);
      recursivelyDraw(png, true, pos.d, pos.x, pos.y);
    } else {
      randNum = std::rand() % 10;
      unsigned int edgeLength = static_cast<unsigned>(EDGE_LENGTH);
      if (randNum < 4) {edgeLength *= 3;} else if (randNum < 6) {edgeLength *= 9;}
      randNum = std::rand() % 4;
      switch(randNum) {
        case 0: d = Direction::N; break;
        case 1: d = Direction::S; break;
        case 2: d = Direction::E; break;
        case 3: d = Direction::W; break;
        default: throw std::logic_error("randNum is not in [0, 3]");
      }
      pos = drawEdge(png, edgeLength, d, x, y);
      recursivelyDraw(png, false, pos.d, pos.x, pos.y);
    }
  }

  cs225::PNG flip(cs225::PNG & png) {
    cs225::PNG result(png.width(), png.height());

    const unsigned int LAST_X_INDEX = png.width() - 1;
    const unsigned int LAST_Y_INDEX = png.height() - 1;

    for (unsigned x = 0; x <= LAST_X_INDEX; x++) {
      for (unsigned y = 0; y <= LAST_Y_INDEX; y++) {
        safePixel(result, x, LAST_Y_INDEX - y) = safePixel(png, x, y);
      }
    }
    return result;
  }

  private:
  void iterateColorIndex(double & i, double k) {
    const double START_INCLSV = 0.0;
    const double END_EXCLSV = 360.0;
    const double CUT = 0.0;
    const double CYCLE = 360.0 - CUT;
    i += k;
    while (i < START_INCLSV) {i += CYCLE;}
    while (i >= END_EXCLSV) {i -= CYCLE;}
  }

};

cs225::PNG myArt(unsigned int width, unsigned int height) {

  cs225::PNG png(width, height);

  std::srand(823784745); // set seed
  const unsigned int MIDDLE_X = static_cast<unsigned>(std::ceil(width / 2.0));
  const unsigned int MIDDLE_Y = static_cast<unsigned>(std::ceil(height / 2.0));
  try {
    ArtHelper help(std::min(png.width(), png.height()));
    help.dump(png);

    Position pos = Position(Direction::N, MIDDLE_X, MIDDLE_Y);
    pos = help.drawSquare(png, Direction::N, MIDDLE_X, MIDDLE_Y); // START
    help.recursivelyDraw(png, true, Direction::N, MIDDLE_X, MIDDLE_Y);
    const double EYE_RADIUS = 40 * help.SCALE;
    const double EYE_Y = 500 * help.SCALE;
    const double RIGHT_EYE_X = 480 * help.SCALE;
    const double LEFT_EYE_X = 320 * help.SCALE;
    const double FACE_L = 0.3;
    help.drawCircle(png, cs225::HSLAPixel(NICE_COLOR, 1.0, FACE_L), Direction::STOP, EYE_RADIUS, RIGHT_EYE_X, EYE_Y);
    help.drawCircle(png, cs225::HSLAPixel(NICE_COLOR, 1.0, FACE_L), Direction::STOP, EYE_RADIUS, LEFT_EYE_X, EYE_Y);
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
