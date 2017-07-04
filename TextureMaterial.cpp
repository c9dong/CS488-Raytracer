#include "TextureMaterial.hpp"

#include <glm/ext.hpp>
#include <iostream>
#include <lodepng/lodepng.h>

#include "Intersection.hpp"
#include "Util.hpp"

using namespace glm;
using namespace std;

TextureMaterial::TextureMaterial(const std::string & file_name) {
  image = new vector<unsigned char>();
  unsigned error = lodepng::decode(*image, width, height, file_name.c_str());
}

TextureMaterial::~TextureMaterial()
{}

glm::vec3 TextureMaterial::colorAt(int x, int y) {
  float r = float((*image)[4 * (width * y + x) + 0]);
  float g = float((*image)[4 * (width * y + x) + 1]);
  float b = float((*image)[4 * (width * y + x) + 2]);
  return vec3(r, g, b) / 255.0f;
}

glm::vec3 TextureMaterial::getColor(glm::vec3 pHit, 
    glm::vec3 pNormal, 
    Light *light,
    glm::mat4 inv) {

  assert(false);
  return vec3(1);
}