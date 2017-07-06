#include "Shadow.hpp"
#include "Intersection.hpp"

using namespace std;
using namespace glm;

Shadow::Shadow(glm::vec3 ambient) : ambient(ambient) {}

glm::vec3 Shadow::getColor(Intersection::Hit hit, Light *light, SceneNode *root) {
  vec3 shadow_origin = hit.pHit + hit.pNormal * 0.1f;
  vec3 shadow_direction = glm::normalize(light->position - shadow_origin);
  Ray shadowRay(shadow_origin, shadow_direction);
  vec3 color = ambient;
  Intersection intersect = root->intersect(shadowRay, true);
  if (!intersect.getFirstHit(shadowRay).hit) {
    color = hit.mat->getColor(hit.pHit, hit.pNormal, light, hit.inv);
  }
  return color;
}

Shadow::~Shadow() {}

SoftShadow::SoftShadow(glm::vec3 ambient) 
  : Shadow(ambient), m_radius(50.0f) {}

SoftShadow::SoftShadow(glm::vec3 ambient, float radius) 
  : Shadow(ambient), m_radius(radius) {}

SoftShadow::~SoftShadow() {}

glm::vec3 SoftShadow::getColor(Intersection::Hit hit, Light *light, SceneNode *root) {
  vec3 shadow_origin = hit.pHit + hit.pNormal * 0.1f;
  vec3 color = ambient;

  float start_x = light->position.x - m_radius;
  float start_y = light->position.y - m_radius;
  float delta = 10.0f;

  float sample = (2 * m_radius / delta) * (2 * m_radius / delta);
  for (float i = start_x; i < light->position.x + m_radius; i+=delta) {
    for (float j = start_y; j < light->position.y + m_radius; j+=delta) {
      vec3 light_pos = vec3(i, j, light->position.z);
      vec3 shadow_direction = glm::normalize(light_pos - shadow_origin);
      Ray shadowRay(shadow_origin, shadow_direction);
      Light *new_light = new Light(*light);
      new_light->position = light_pos;

      Intersection intersect = root->intersect(shadowRay, true);
      if (!intersect.getFirstHit(shadowRay).hit) {
        color += hit.mat->getColor(hit.pHit, hit.pNormal, new_light, hit.inv);
      }
    }
  }
  color = color / sample;

  return color;
}