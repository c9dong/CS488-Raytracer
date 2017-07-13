#include "Bump.hpp"

#include <glm/ext.hpp>
#include <iostream>
#include <lodepng/lodepng.h>

#include "Util.hpp"

using namespace glm;
using namespace std;

Bump::Bump() {}
Bump::Bump(const std::string & file_name) {
  image = new vector<unsigned char>();
  unsigned error = lodepng::decode(*image, width, height, file_name.c_str());
}

Bump::~Bump() {
  
}

glm::vec3 Bump::colorAt(int x, int y) {
  int idx = 4 * (width * y + x);
  if (idx < 0) {
    idx = 0;
  }
  if (idx >= image->size() - 3) {
    idx = image->size() - 3;
  }
  float r = float((*image)[idx + 0]);
  float g = float((*image)[idx + 1]);
  float b = float((*image)[idx + 2]);
  return vec3(r, g, b) / 255.0f;
}

glm::vec3 Bump::getNormal(glm::vec3 normal, float u, float v) {
  // return normal;
  vec3 upvec = glm::normalize(vec3(-1, -1, -1));
  vec3 su = glm::normalize(glm::cross(upvec, normal));
  vec3 sv = glm::cross(su, normal);

  cout << "////////////////" << endl;
  printVec3(normal);
  printVec3(su);
  printVec3(sv);

  float di = (width - 1.0f) * u;
  float dj = (height - 1.0f) * v;

  int i = int(di);
  int j = int(dj);

  float pu = 1 - ((colorAt(i-1, j).x + colorAt(i+1, j).x) / 2.0f);
  float pv = 1 - ((colorAt(i, j-1).x + colorAt(i, j+1).x) / 2.0f);

  vec3 uvec = vec3(0);
  vec3 vvec = vec3(0);
  if (pu != 0) {
    uvec = (pu * glm::normalize(glm::cross(normal, sv)));
  }
  if (pv != 0) {
    vvec = (pv * glm::normalize(glm::cross(normal, su)));
  }

  vec3 newNormal = glm::normalize(normal + uvec + vvec);

  return newNormal;
}

////////////////////////////////////////////////////////////////////

NoBump::NoBump(const std::string & file_name) {}
NoBump::~NoBump() {}

glm::vec3 NoBump::getNormal(glm::vec3 normal, float u, float v) {
  return normal;
}