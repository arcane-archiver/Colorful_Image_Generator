#include <iostream>
#include <stdlib.h>

#include "cs225_intro.hpp"
#include "../cs225/PNG.h"
#include "ColorfulImageGenerator.hpp"

#include <iostream>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  // if (argc == 1+1) {
  //   unsigned int width = (unsigned int)atoi(argv[1]);
  //   std::cout << "Calling myArt(" << width << ", " << width << ") and saving it as `art.png`." << std::endl;

  //   cs225::PNG png = myArt(width, width);
  //   png.writeToFile("art.png");
  // } else {
  //   std::cout << "Need an integer argument." << std::endl;
  // }

  unsigned width = 500;
  // cs225::PNG image = ColorfulImageGenerator::generateImage(width, width);
  cs225::PNG image = myArt(width, width);
  image.writeToFile("art.png");

  return 0;
}
