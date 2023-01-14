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

  ColorfulImageGenerator(width, height)
    .generate()
    .writeToFile("art.png");

  myArt(width, height).writeToFile("myArt.png");

  return 0;
}
