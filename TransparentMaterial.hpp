#pragma once

#include <glm/glm.hpp>

#include "Material.hpp"

class TransparentMaterial : public Material {
public:
  TransparentMaterial();
  TransparentMaterial(double transmittance, double reflectivity, double refractIdx);
  virtual ~TransparentMaterial();

  glm::vec3 getColor(glm::vec3 pHit, 
    glm::vec3 pNormal, 
    Light *light,
    glm::mat4 inv,
    Material *lastMat);

  double transmittance;
  double reflectivity;
};
