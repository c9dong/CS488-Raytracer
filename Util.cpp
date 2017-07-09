#include <math.h>
#include <iostream>

#include "Util.hpp"

using namespace glm;
using namespace std;

bool isZero(float n) {
  return fabs(n) < 0.0001;
}

bool isZero(glm::vec3 &v) {
  return isZero(v.x) && isZero(v.y) && isZero(v.z);
}

void printVec3(vec3 v) {
  cout << v.x << ", " << v.y << ", " << v.z << endl;
}

float min4(float a, float b, float c, float d) {
  return glm::min(glm::min(glm::min(a, b), c), d);
}

float max4(float a, float b, float c, float d) {
  return glm::max(glm::max(glm::max(a, b), c), d);
}

float min3(float a, float b, float c) {
  return glm::min(glm::min(a, b), c);
}

float max3(float a, float b, float c) {
  return glm::max(glm::max(a, b), c);
}

float randomNum(int dist) {
  float half = float(dist) / 2.0f;
  return float(rand() % ((dist) * 100000+1)) / 100000.0f - half;
}