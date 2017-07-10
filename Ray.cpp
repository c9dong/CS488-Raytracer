#include "Ray.hpp"

using namespace glm;
using namespace std;

Ray::Ray(vec3 origin, vec3 direction)
  : origin(origin), direction(glm::normalize(direction)), index(1.0f)
{
  
}

Ray::Ray(vec3 origin, vec3 direction, float index)
  : origin(origin), direction(glm::normalize(direction)), index(index)
{
  
}

Ray::~Ray() {}