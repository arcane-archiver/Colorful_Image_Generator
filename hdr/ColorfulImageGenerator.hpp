#ifndef COLORFUL_IMAGE_GENERATOR_HPP
#define COLORFUL_IMAGE_GENERATOR_HPP

#include "../cs225/PNG.h"
#include "../cs225/HSLAPixel.h"
#include "CanvasUtility.hpp"

class ColorfulImageGenerator {
  private:
    enum PixelType { Node, Edge, Background };

  public:
    static constexpr double ILLINI_ORANGE = 11.0;
    static constexpr double ILLINI_BLUE = 216.0;

  private:
    const unsigned WIDTH;
    const unsigned HEIGHT;

  private:
    const double SCALE;
    const double SQUARE_WIDTH;
    const double EDGE_LENGTH;
    const double LINE_LENGTH;
    const cs225::HSLAPixel NODE_PAINT;
    const cs225::HSLAPixel EDGE_PAINT;
    const unsigned RECURSION_MAX;

  private:
    unsigned recursionCount;

  public:
    ColorfulImageGenerator() = delete;
    ColorfulImageGenerator(unsigned width, unsigned height);

  public:
    cs225::PNG generate() const;

  private:
    void drawGraph(cs225::PNG image) const;
    void recursivelyDrawGraph(cs225::PNG &image, bool branching, CanvasUtility::CardinalDirection direction, unsigned int x, unsigned int y) const;
    CanvasUtility::Position drawEdge(cs225::PNG & png, unsigned int edgeLength, CanvasUtility::CardinalDirection d, unsigned int x, unsigned int y) const;
    void drawBranch(cs225::PNG &image, CanvasUtility::CardinalDirection direction, unsigned int x, unsigned int y) const;
};

#endif