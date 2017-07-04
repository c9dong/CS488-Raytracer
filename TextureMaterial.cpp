#include "TextureMaterial.hpp"

#include <glm/ext.hpp>
#include <iostream>
#include <lodepng/lodepng.h>

#include "Intersection.hpp"
#include "Util.hpp"

using namespace glm;
using namespace std;

TextureMaterial::TextureMaterial(const std::string & file_name) {
  image = new vector<unsigned char>();
  unsigned error = lodepng::decode(*image, width, height, file_name.c_str());
}

TextureMaterial::~TextureMaterial()
{}

glm::vec3 TextureMaterial::colorAt(int x, int y) {
  float r = float((*image)[4 * (width * y + x) + 0]);
  float g = float((*image)[4 * (width * y + x) + 1]);
  float b = float((*image)[4 * (width * y + x) + 2]);
  return vec3(r, g, b) / 255.0f;
}

glm::vec3 TextureMaterial::getColor(glm::vec3 pHit, 
    glm::vec3 pNormal, 
    Light *light,
    glm::mat4 inv) {

  pHit = vec3(inv * vec4(pHit, 1.0f));
  float x = pHit.x;
  float y = pHit.y;
  float u = x;
  float v = 1.0f - y;

  // cout << u << " " << v << endl;

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