#include "Ray.hpp"

using namespace glm;
using namespace std;

Ray::Ray(vec3 origin, vec3 direction)
  : origin(origin), direction(direction) 
{
  
}

Ray::Ray(glm::vec3 &pHit, glm::vec3 &pNormal, Light *light)
  : origin(vec3(0)), 
    direction(vec3(0)) {
  this->origin = pHit + pNormal * 0.1f;
  this->direction = glm::normalize(light->position - origin);
}

Ray::~Ray() {}