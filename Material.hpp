#pragma once

#include <glm/glm.hpp>

#include "Light.hpp"

class Material {
public:
  Material();
  Material(double refractIdx);
  virtual ~Material();

  virtual glm::vec3 getColor(glm::vec3 pHit, 
    glm::vec3 pNormal, 
    Light *light,
    glm::mat4 inv,
    Material *lastMat);

  double refractIdx;
};
