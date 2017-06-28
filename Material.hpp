#pragma once

#include <glm/glm.hpp>

#include "Light.hpp"

class Material {
public:
  virtual ~Material();

  virtual glm::vec3 getColor(glm::vec3 pHit, 
    glm::vec3 pNormal, 
    Light *light);
protected:
  Material();
};
