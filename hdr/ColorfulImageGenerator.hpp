#ifndef COLORFUL_IMAGE_GENERATOR_HPP
#define COLORFUL_IMAGE_GENERATOR_HPP

#include "../cs225/PNG.h"

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
    const double NODE_COLOR;
    const double EDGE_COLOR;
    const unsigned RECURSION_MAX;

  private:
    unsigned recursionCount;

  public:
    ColorfulImageGenerator() = delete;
    ColorfulImageGenerator(unsigned width, unsigned height);

  public:
    cs225::PNG generate();

  private:
    void drawGraph();
};

#endif