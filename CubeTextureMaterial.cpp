#include "CubeTextureMaterial.hpp"

#include <glm/ext.hpp>
#include <iostream>
#include <lodepng/lodepng.h>

#include "Intersection.hpp"
#include "Util.hpp"
#include "cs488-framework/MathUtils.hpp"

using namespace glm;
using namespace std;

CubeTextureMaterial::CubeTextureMaterial(const std::string & file_name, double m_shininess)
 : TextureMaterial(file_name, m_shininess) {}

CubeTextureMaterial::~CubeTextureMaterial()
{}

glm::vec3 CubeTextureMaterial::getColor(glm::vec3 pHit, 
    glm::vec3 pNormal, 
    Light *light,
    glm::mat4 inv) {
  vec3 r_pNormal = glm::normalize(glm::transpose((mat3(glm::inverse(inv)))) * pNormal);
  vec3 r_pHit = vec3(inv * vec4(pHit, 1.0f));
  float x;
  float y;
  if (isZero(fabs(r_pNormal.x) - 1.0f)) {
    x = r_pHit.z;
    y = r_pHit.y;
  } else if (isZero(fabs(r_pNormal.y) - 1.0f)) {
    x = r_pHit.x;
    y = r_pHit.z;
  } else if (isZero(fabs(r_pNormal.z) - 1.0f)) {
    x = r_pHit.x;
    y = r_pHit.y;
  } else {
    printVec3(r_pNormal);
    assert(false);
  }

  x += 0.5;
  y += 0.5;

  float u = x;
  float v = 1.0f - y;
  int index = 0;

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
  vec3 m_ks = vec3(0);

  return calcPhongShading(pHit, pNormal, m_kd, m_ks, m_shininess, light);
}