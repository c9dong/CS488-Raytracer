#pragma once

#include <glm/glm.hpp>

#include "Light.hpp"
#include "Ray.hpp"
#include "SceneNode.hpp"
#include "Intersection.hpp"

class Shadow {
public:
  struct ShadowColor {
    ShadowColor() : color(glm::vec3(1)), hit(false) {};
    ShadowColor(glm::vec3 color, bool hit) : color(color), hit(hit) {};
    glm::vec3 color;
    bool hit;
  };

  Shadow(glm::vec3 ambient);
  virtual ~Shadow();

  virtual glm::vec3 getColor(Intersection::Hit hit, Light *light, SceneNode *root);

protected:
  glm::vec3 ambient;
};

class SoftShadow : public Shadow {
public:
  SoftShadow(glm::vec3 ambient);
  SoftShadow(glm::vec3 ambient, float radius);
  virtual ~SoftShadow();

  glm::vec3 getColor(Intersection::Hit hit, Light *light, SceneNode *root);

protected:
  float m_radius;
};