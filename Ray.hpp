#pragma once

#include <glm/glm.hpp>

#include "Light.hpp"

class Ray {
public:
  Ray(glm::vec3 origin, glm::vec3 direction);
  Ray(glm::vec3 &pHit, glm::vec3 &pNormal, Light *light);
  virtual ~Ray();

  glm::vec3 origin;
  glm::vec3 direction;
private:
  
};