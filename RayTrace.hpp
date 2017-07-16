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
    Shadow &shadow,
    double focal_dist,
    double camera_radius,
    double camera_sample_rate,
    double anti_sample_radius,
    double anti_sample_rate,
    int x_start,
    int y_start,
    int x_size,
    int y_size,
    int id);

  virtual ~RayTrace();

  void generateImage();
  Image& getImage();
  const glm::vec3& getEye();
  double getFocalDist();
  double getCameraRadius();
  double getCameraSampleRate();
  double getAntiSampleRadius();
  double getAntiSampleRate();

  glm::vec3 getBackgroundColor(Ray &ray);
  glm::vec3 getRayColor(Ray &ray, glm::vec3 & background, int maxHit, Material *lastMat);

  int x_start;
  int y_start;
  int x_size;
  int y_size;
  int id;
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
  double focal_dist;
  double camera_radius;
  double camera_sample_rate;
  double anti_sample_radius;
  double anti_sample_rate;

  Shadow &shadow;
};