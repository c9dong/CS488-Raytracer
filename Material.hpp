#pragma once

#include <glm/glm.hpp>

#include "Bump.hpp"
#include "Light.hpp"

class Material {
public:
  Material();
  Material(double refractIdx);
  virtual ~Material();

  virtual glm::vec3 getColor(glm::vec3 pHit, 
    glm::vec3 pNormal, 
    Light *light,
    glm::mat4 inv);

  glm::vec3 calcPhongShading(glm::vec3 pHit, glm::vec3 pNormal, glm::vec3 kd, glm::vec3 ks, float shininess, Light *light);

  void setBump(Bump *bump);

  Bump *bump;
  double refractIdx;
};
