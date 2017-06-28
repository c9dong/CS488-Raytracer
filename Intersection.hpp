#pragma once

#include <glm/glm.hpp>

#include "Material.hpp"

class Intersection {
public:
  Intersection();
  Intersection(Material *mat, 
    glm::vec3 pHit, 
    glm::vec3 pNormal, 
    float pDist, 
    bool hit);
  virtual ~Intersection();

  Material *mat;
  glm::vec3 pHit;
  glm::vec3 pNormal;
  float pDist;
  bool hit;
private:

};