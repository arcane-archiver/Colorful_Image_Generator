#include <iostream>
#include <stdlib.h>

#include "../cs225/PNG.h"
#include "ColorfulImageGenerator.hpp"

#include <iostream>
#include <stdlib.h>
#include <string>

int main(int argc, char *argv[]) {
  uint const width = 500;
  uint const height = 500;

  uint seed = 1;
  try {
    seed = static_cast<uint>(std::stoi(argv[0+1]));
  } catch (...) {
    std::cout << "invalid seed" << std::endl;
    return 1;
  }

  auto name = "art.png";

  try {
    ColorfulImageGenerator::generate(width, height, seed).writeToFile(name);
  } catch (...) {
    std::cout << "error" << std::endl;
    return 1;
  }

  std::cout << "saved " << name << std::endl;

  return 0;
}
