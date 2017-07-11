#include "PhongMaterial.hpp"

#include <glm/ext.hpp>
#include <iostream>

#include "Intersection.hpp"
#include "Util.hpp"

using namespace glm;
using namespace std;

PhongMaterial::PhongMaterial() : Material() {}

PhongMaterial::PhongMaterial(
	const glm::vec3& kd, const glm::vec3& ks, double shininess)
	: Material()
  , m_kd(kd)
	, m_ks(ks)
	, m_shininess(shininess)
{}

PhongMaterial::~PhongMaterial()
{}

glm::vec3 PhongMaterial::getColor(glm::vec3 pHit, 
    glm::vec3 pNormal, 
    Light *light,
    glm::mat4 inv) {

  return calcPhongShading(pHit, pNormal, m_kd, m_ks, m_shininess, light);
}