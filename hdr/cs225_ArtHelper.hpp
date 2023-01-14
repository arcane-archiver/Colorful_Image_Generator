#pragma once

#include "../cs225/PNG.h"

class ArtHelper {
  public:
    static constexpr double ILLINI_ORANGE = 11.0;
    static constexpr double ILLINI_BLUE = 216.0;
    static constexpr double NICE_COLOR = 300.0;

    enum Direction {North, South, East, West, STOP};
    struct Position {
      Direction d;
      unsigned int x;
      unsigned int y;
      Position(Direction setD, unsigned int setX, unsigned int setY);
    };

    double SMALLEST_LENGTH;
    double SCALE;
    double SQUARE_WIDTH;
    double EDGE_LENGTH;
    double LINE_LENGTH;
    double NODE_COLOR;
    double EDGE_COLOR;
    unsigned int RECURSION_COUNT;
    unsigned int RECURSION_MAX;

    ArtHelper() = delete;
    ArtHelper(unsigned int smallestLength);

    void drawGraph(cs225::PNG &png);

  public:
    void dumpBlackPaint(cs225::PNG &png);

  private:
    bool inBounds(const cs225::PNG & png, const unsigned int x, const unsigned int y);

    void paint(cs225::PNG & png, cs225::HSLAPixel pixel, unsigned int x, unsigned int y);

    void drawLine(cs225::PNG & png, double lineLength, Direction d, double x, double y);

    void drawSquare(cs225::PNG & png, Direction d, double x, double y);

    Position drawNode(
      cs225::PNG &png, cs225::HSLAPixel pixel, Direction d,
      double radius, double h, double k
    );

  public:
    void drawAura(cs225::PNG & png, unsigned int centerX, unsigned int centerY);

  private:
    Position drawEdge(cs225::PNG & png, unsigned int edgeLength, Direction d, unsigned int x, unsigned int y);

    void drawBranch(cs225::PNG & png, Direction d, unsigned int x, unsigned int y);

    void recursivelyDrawGraph(cs225::PNG & png, bool branching, Direction d, unsigned int x, unsigned int y);

  private:
    void iterateColorIndex(double & i, double k);
};
