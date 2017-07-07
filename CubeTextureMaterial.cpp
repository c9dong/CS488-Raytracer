#include "CubeTextureMaterial.hpp"

#include <glm/ext.hpp>
#include <iostream>
#include <lodepng/lodepng.h>

#include "Intersection.hpp"
#include "Util.hpp"
#include "cs488-framework/MathUtils.hpp"

using namespace glm;
using namespace std;

CubeTextureMaterial::CubeTextureMaterial(const std::string & file_name) : TextureMaterial(file_name) {}

CubeTextureMaterial::~CubeTextureMaterial()
{}

glm::vec3 CubeTextureMaterial::getColor(glm::vec3 pHit, 
    glm::vec3 pNormal, 
    Light *light,
    glm::mat4 inv) {

  pHit = vec3(inv * vec4(pHit, 1.0f));
  float x;
  float y;
  if (pNormal.x == abs(1.0f)) {
    x = -pHit.z;
    y = pHit.y + 0.5;
  } else if (pNormal.y == abs(1.0f)) {
    x = -pHit.z;
    y = pHit.x + 0.5;
  } else {
    x = pHit.x + 0.5;
    y = pHit.y + 0.5;
  }
  float u = x;
  float v = 1.0f - y;

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

  return c00 * (1-up) * (1-vp) + c01 * (1-up) * vp + c10 * up * (1-vp) + c11 * up * vp;
  // m_shininess = 25.0f;

  // vec3 l = normalize(light->position - vec3(pHit));

  // vec3 v2 = -normalize(vec3(pHit));

  // float n_dot_l = glm::max(dot(pNormal, l), 0.0f);

  // vec3 diffuse;
  // diffuse = m_kd * n_dot_l;

  // vec3 specular = vec3(0.0);

  // if (n_dot_l > 0.0) {
  //   vec3 h = normalize(v2 + l);
  //   float n_dot_h = glm::max(dot(vec3(pNormal), h), 0.0f);

  //   specular = m_ks * pow(n_dot_h, m_shininess);
  // }

  // vec3 col = light->colour * (diffuse + specular);

  // return col;
}