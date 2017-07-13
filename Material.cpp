#include "Material.hpp"

using namespace std;
using namespace glm;

Material::Material() : Material(1.0)
{}

Material::Material(double refractIdx) : refractIdx(refractIdx), bump(new NoBump("a"))
{}

Material::~Material()
{}

glm::vec3 Material::getColor(glm::vec3 pHit, 
    glm::vec3 pNormal, 
    Light *light,
    glm::mat4 inv) {
  assert(false);
}

glm::vec3 Material::calcPhongShading(glm::vec3 pHit, glm::vec3 pNormal, glm::vec3 kd, glm::vec3 ks, float shininess, Light *light) {
  vec3 l = normalize(light->position - vec3(pHit));

  vec3 v = -normalize(vec3(pHit));

  float n_dot_l = glm::max(dot(pNormal, l), 0.0f);

  vec3 diffuse;
  diffuse = kd * n_dot_l;

  vec3 specular = vec3(0.0);

  if (n_dot_l > 0.0) {
    vec3 h = normalize(v + l);
    float n_dot_h = glm::max(dot(vec3(pNormal), h), 0.0f);

    specular = ks * pow(n_dot_h, shininess);
  }

  vec3 col = light->colour * (diffuse + specular);

  return col;
}

void Material::setBump(Bump *bump) {
  this->bump = bump;
}