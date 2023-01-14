#include <iostream>
#include <stdlib.h>

#include "cs225_intro.hpp"
#include "../cs225/PNG.h"
#include "ColorfulImageGenerator.hpp"

#include <iostream>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  const unsigned width = 500;
  const unsigned height = 500;
  const unsigned seed = 823784745;

  ColorfulImageGenerator::generate(width, height, seed).writeToFile("art.png");
  myArt(width, height, seed).writeToFile("myArt.png");

  return 0;
}
