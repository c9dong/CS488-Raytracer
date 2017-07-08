#include "Shadow.hpp"
#include "Intersection.hpp"

using namespace std;
using namespace glm;

Shadow::Shadow(glm::vec3 ambient) : ambient(ambient) {}

glm::vec3 Shadow::getColor(Intersection::Hit hit, Light *light, SceneNode *root) {
  vec3 shadow_origin = hit.pHit + hit.pNormal * 0.1f;
  vec3 shadow_direction = glm::normalize(light->position - shadow_origin);
  Ray shadowRay(shadow_origin, shadow_direction);
  vec3 color = vec3(0);
  Intersection intersect = root->intersect(shadowRay, true);
  Intersection::Hit shadow_hit = intersect.getFirstHit(shadowRay);
  if (shadow_hit.hit) {
    float light_dist = glm::distance(light->position, shadowRay.origin);
    float hit_dist = glm::distance(shadow_hit.pHit, shadowRay.origin);
    if (light_dist > hit_dist) {
      return color;
    }
  }
  return hit.mat->getColor(hit.pHit, hit.pNormal, light, hit.inv, nullptr);
}

Shadow::~Shadow() {}

SoftShadow::SoftShadow(glm::vec3 ambient) 
  : Shadow(ambient), m_radius(2.0f) {}

SoftShadow::SoftShadow(glm::vec3 ambient, float radius) 
  : Shadow(ambient), m_radius(radius) {}

SoftShadow::~SoftShadow() {}

glm::vec3 SoftShadow::getColor(Intersection::Hit hit, Light *light, SceneNode *root) {
  vec3 shadow_origin = hit.pHit + hit.pNormal * 0.1f;
  vec3 color = ambient;

  float start_x = light->position.x - m_radius;
  float start_y = light->position.y - m_radius;
  float delta = 0.5f;

  float sample = (2 * m_radius / delta) * (2 * m_radius / delta);
  for (float i = start_x; i < light->position.x + m_radius; i+=delta) {
    for (float j = start_y; j < light->position.y + m_radius; j+=delta) {
      vec3 light_pos = vec3(i, j, light->position.z);
      vec3 shadow_direction = glm::normalize(light_pos - shadow_origin);
      Ray shadowRay(shadow_origin, shadow_direction);
      Light *new_light = new Light(*light);
      new_light->position = light_pos;

      vec3 shadow_color = Shadow::getColor(hit, new_light, root);
      color += shadow_color;
    }
  }
  color = color / sample;

  return color;
}

NoShadow::NoShadow(glm::vec3 ambient) : Shadow(ambient) {}

glm::vec3 NoShadow::getColor(Intersection::Hit hit, Light *light, SceneNode *root) {
  vec3 color = hit.mat->getColor(hit.pHit, hit.pNormal, light, hit.inv, nullptr);
  return color;
}

NoShadow::~NoShadow() {}