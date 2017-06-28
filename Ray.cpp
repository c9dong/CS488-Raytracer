#include "Ray.hpp"

using namespace glm;
using namespace std;

Ray::Ray(vec3 & origin, vec3 & direction)
  : origin(origin), direction(direction) 
{
  
}

Ray::~Ray() {}