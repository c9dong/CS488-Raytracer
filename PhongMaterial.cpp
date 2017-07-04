#include "PhongMaterial.hpp"

#include <glm/ext.hpp>
#include <iostream>

#include "Intersection.hpp"
#include "Util.hpp"

using namespace glm;
using namespace std;

PhongMaterial::PhongMaterial() {}

PhongMaterial::PhongMaterial(
	const glm::vec3& kd, const glm::vec3& ks, double shininess )
	: m_kd(kd)
	, m_ks(ks)
	, m_shininess(shininess)
{}

PhongMaterial::~PhongMaterial()
{}

glm::vec3 PhongMaterial::getColor(glm::vec3 pHit, 
    glm::vec3 pNormal, 
    Light *light,
    glm::mat4 inv) {

  vec3 l = normalize(light->position - vec3(pHit));

  vec3 v = -normalize(vec3(pHit));

  float n_dot_l = glm::max(dot(pNormal, l), 0.0f);

  vec3 diffuse;
  diffuse = m_kd * n_dot_l;

  vec3 specular = vec3(0.0);

  if (n_dot_l > 0.0) {
    vec3 h = normalize(v + l);
    float n_dot_h = glm::max(dot(vec3(pNormal), h), 0.0f);

    specular = m_ks * pow(n_dot_h, m_shininess);
  }

  vec3 col = light->colour * (diffuse + specular);

  return col;
}