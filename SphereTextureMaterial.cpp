#include "SphereTextureMaterial.hpp"

#include <glm/ext.hpp>
#include <iostream>
#include <lodepng/lodepng.h>

#include "Intersection.hpp"
#include "Util.hpp"
#include "cs488-framework/MathUtils.hpp"

using namespace glm;
using namespace std;

SphereTextureMaterial::SphereTextureMaterial(const std::string & file_name, double m_shininess) 
: TextureMaterial(file_name, m_shininess) {}

SphereTextureMaterial::~SphereTextureMaterial()
{}

glm::vec3 SphereTextureMaterial::getColor(glm::vec3 pHit, 
    glm::vec3 pNormal, 
    Light *light,
    glm::mat4 inv) {

  vec3 r_pNormal = glm::normalize(glm::transpose((mat3(glm::inverse(inv)))) * pNormal);

  float u = 0.5f + (atan2(r_pNormal.z, r_pNormal.x) / (2 * PI));
  float v = 0.5f + asin(r_pNormal.y) / PI;

  float di = (width - 1.0f) * u;
  float dj = (height - 1.0f) * v;

  int i = int(di);
  int j = int(dj);

  float up = di - i;
  float vp = dj - j;


  vec3 c00 = colorAt(i, j);
  vec3 c01 = colorAt(i, j+1);
  vec3 c10 = colorAt(i+1, j);
  vec3 c11 = colorAt(i+1, j+1);

  vec3 m_kd = c00 * (1-up) * (1-vp) + c01 * (1-up) * vp + c10 * up * (1-vp) + c11 * up * vp;
  vec3 m_ks = vec3(0.5, 0.5, 0.5);

  return calcPhongShading(pHit, pNormal, m_kd, m_ks, m_shininess, light);
}