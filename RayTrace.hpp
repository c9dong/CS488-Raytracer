#pragma once

#include <glm/glm.hpp>

#include "SceneNode.hpp"
#include "Light.hpp"
#include "Image.hpp"
#include "Ray.hpp"

class RayTrace {
public:
  RayTrace(
    SceneNode * root,
    Image & image,
    const glm::vec3 & eye,
    const glm::vec3 & view,
    const glm::vec3 & up,
    double fovy,
    const glm::vec3 & ambient,
    const std::list<Light *> & lights);

  virtual ~RayTrace();

  void generateImage();
private:
  // Member functions
  glm::mat4 getPointToWorldMatrix();
  glm::vec3 getRayColor(Ray & ray, glm::vec3 & background, int maxHit);
  glm::vec3 getRefractAngle(float kr, glm::vec3 &d, glm::vec3 &n);
  glm::vec3 getBackgroundColor(Ray &ray);
  // Member variables
  SceneNode * root;
  Image & image;
  const glm::vec3 & eye;
  const glm::vec3 & view;
  const glm::vec3 & up;
  double fovy;
  const glm::vec3 & ambient;
  const std::list<Light *> & lights;
};