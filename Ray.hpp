#pragma once

#include <glm/glm.hpp>

#include "Light.hpp"

class Ray {
public:
  Ray(glm::vec3 & origin, glm::vec3 & direction);
  virtual ~Ray();

  glm::vec3 & origin;
  glm::vec3 & direction;
private:
  
};