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