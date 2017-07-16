#include <iostream>
#include <stdlib.h>
#include "scene_lua.hpp"

int main(int argc, char** argv)
{
  std::string filename = "Assets/simple.lua";
  int x_start;
  int y_start;
  int x_size;
  int y_size;
  int id;
  int width;
  int height;
  std::string resultname;
  if (argc >= 8) {
    filename = argv[1];
    x_start = atoi(argv[2]);
    y_start = atoi(argv[3]);
    x_size = atoi(argv[4]);
    y_size = atoi(argv[5]);
    id = atoi(argv[6]);
    width = atoi(argv[7]);
    height = atoi(argv[8]);
    resultname = argv[9];
  }

  if (!run_lua(filename, resultname, x_start, y_start, x_size, y_size, id, width, height)) {
    std::cerr << "Could not open " << filename << std::endl;
    return 1;
  }
}
