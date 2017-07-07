#include "TransparentMaterial.hpp"

#include <glm/ext.hpp>
#include <iostream>

#include "Intersection.hpp"
#include "Util.hpp"

using namespace glm;
using namespace std;

TransparentMaterial::TransparentMaterial() : Material() {}

TransparentMaterial::TransparentMaterial(double transmittance, double reflectivity, double refractIdx)
  : Material(refractIdx), transmittance(transmittance), reflectivity(reflectivity) {}

TransparentMaterial::~TransparentMaterial()
{}

glm::vec3 TransparentMaterial::getColor(glm::vec3 pHit, 
    glm::vec3 pNormal, 
    Light *light,
    glm::mat4 inv,
    Material *lastMat) {

  return vec3(0);
}