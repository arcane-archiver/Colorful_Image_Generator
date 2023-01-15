#include <iostream>
#include <stdlib.h>

#include "../cs225/PNG.h"
#include "ColorfulImageGenerator.hpp"

#include <iostream>
#include <stdlib.h>
#include <string>
#include <unordered_map>

int main(int argc, char *argv[]) {
  std::unordered_map<std::string, std::string> parameters = {
    {"width", "500"},
    {"height", "500"},
    {"seed", "1"},
    {"output", "art.png"}
  };

  for (int index = 1; index < argc; ++index) {
    std::string arg(argv[index]);

    size_t delimiterIndex = arg.find("=");
    if (delimiterIndex == string::npos)
      continue;

    std::string key(arg.substr(0, delimiterIndex));
    std::string value(arg.substr(delimiterIndex + 1));

    if (parameters.contains(key))
      parameters[key] = value;
  }


  try {
    uint const width = static_cast<uint>(std::atoi(parameters.at("width").c_str()));
    uint const height = static_cast<uint>(std::atoi(parameters.at("height").c_str()));
    uint const seed = static_cast<uint>(std::atoi(parameters.at("seed").c_str()));
    ColorfulImageGenerator::generate(width, height, seed).writeToFile(parameters["output"]);
  } catch (...) {
    std::cout << "error" << std::endl;
    return 1;
  }

  for (auto pair : parameters)
    std::cout << pair.first << "=" << pair.second << std::endl;

  return 0;
}
