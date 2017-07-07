#include "Material.hpp"

using namespace std;
using namespace glm;

Material::Material() : refractIdx(1.0)
{}

Material::Material(double refractIdx) : refractIdx(refractIdx)
{}

Material::~Material()
{}

glm::vec3 Material::getColor(glm::vec3 pHit, 
    glm::vec3 pNormal, 
    Light *light,
    glm::mat4 inv,
    Material *lastMat) {
  assert(false);
}