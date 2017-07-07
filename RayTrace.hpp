#pragma once

#include <glm/glm.hpp>

#include "SceneNode.hpp"
#include "Light.hpp"
#include "Image.hpp"
#include "Ray.hpp"
#include "Shadow.hpp"

class RayTrace {
public:
   struct GenerateArg {
    int x_start; 
    int y_start; 
    int x_size; 
    int y_size; 
    int x_max; 
    int y_max; 
    glm::mat4 world_mat;
    RayTrace *raytrace;
  };

  RayTrace(
    SceneNode * root,
    Image & image,
    const glm::vec3 & eye,
    const glm::vec3 & view,
    const glm::vec3 & up,
    double fovy,
    const glm::vec3 & ambient,
    const std::list<Light *> & lights,
    Shadow &shadow);

  virtual ~RayTrace();

  void generateImage();
  Image& getImage();
  const glm::vec3& getEye();
  glm::vec3 getBackgroundColor(Ray &ray);
  glm::vec3 getRayColor(Ray & ray, glm::vec3 & background, int maxHit, Material *lastMat);
private:
  // Member functions
  glm::mat4 getPointToWorldMatrix();
  glm::vec3 getRefractAngle(glm::vec3 direction, glm::vec3 normal, float kr);
  glm::vec3 getReflectionAngle(glm::vec3 direction, glm::vec3 normal);
  // Member variables
  SceneNode * root;
  Image & image;
  const glm::vec3 & eye;
  const glm::vec3 & view;
  const glm::vec3 & up;
  double fovy;
  const glm::vec3 & ambient;
  const std::list<Light *> & lights;

  Shadow &shadow;
};