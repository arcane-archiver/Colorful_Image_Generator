#include <iostream>
#include <stdlib.h>

#include "intro.hpp"
#include "../cs225/PNG.h"

#include <iostream>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc == 1+1) {
    unsigned int width = (unsigned int)atoi(argv[1]);
    std::cout << "Calling myArt(" << width << ", " << width << ") and saving it as `art.png`." << std::endl;

    cs225::PNG png = myArt(width, width);
    png.writeToFile("art.png");
  } else {
    std::cout << "Need an integer argument." << std::endl;
  }
  
  return 0;
}
