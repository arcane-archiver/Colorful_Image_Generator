#pragma once

#include "../cs225/PNG.h"

#ifndef ILLINI_ORANGE
#define ILLINI_ORANGE 11.0
#endif
#ifndef ILLINI_BLUE
#define ILLINI_BLUE 216.0
#endif
#ifndef NICE_COLOR
#define NICE_COLOR 300.0
#endif

class ArtHelper {
  public:
    enum Direction {N, S, E, W, STOP};
    struct Position {
      Direction d;
      unsigned int x;
      unsigned int y;
      Position(Direction setD, unsigned int setX, unsigned int setY);
    };

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
    ArtHelper(unsigned int smallestLength);

    void dump(cs225::PNG &png);

    bool inBounds(const cs225::PNG & png, const unsigned int x, const unsigned int y);

    cs225::HSLAPixel & safePixel(cs225::PNG & png, const unsigned int x, const unsigned int y);

    std::string ICON = "\\";
    void iterateIcon();
    void paint(cs225::PNG & png, cs225::HSLAPixel pixel, unsigned int x, unsigned int y);

    void drawLine(cs225::PNG & png, double lineLength, Direction d, double x, double y);

    Position drawSquare(cs225::PNG & png, Direction d, double x, double y);

    Position drawCircle(
      cs225::PNG &png, cs225::HSLAPixel pixel, Direction d,
      double radius, double h, double k
    );

    Position drawNode(
      cs225::PNG &png, cs225::HSLAPixel pixel, Direction d,
      double radius, double h, double k
    );

    void drawParabola(cs225::PNG & png, cs225::HSLAPixel pixel, double a, double k, double h);

    void drawSmile(cs225::PNG & png, cs225::HSLAPixel pixel);

    void drawAura(cs225::PNG & png, unsigned int centerX, unsigned int centerY);

    Position drawEdge(cs225::PNG & png, unsigned int edgeLength, Direction d, unsigned int x, unsigned int y);

    void drawBranch(cs225::PNG & png, Direction d, unsigned int x, unsigned int y);

    void recursivelyDraw(cs225::PNG & png, bool branching, Direction d, unsigned int x, unsigned int y);

    cs225::PNG flip(cs225::PNG & png);

  private:
    void iterateColorIndex(double & i, double k);
};
