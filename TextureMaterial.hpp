#pragma once
#include <glm/glm.hpp>
#include <vector>

#include "PhongMaterial.hpp"

class TextureMaterial : public PhongMaterial {
public:
  TextureMaterial(const std::string & file_name);
  virtual ~TextureMaterial();

  glm::vec3 getColor(glm::vec3 pHit, 
    glm::vec3 pNormal, 
    Light *light,
    glm::mat4 inv);

  glm::vec3 colorAt(int x, int y);

  std::vector<unsigned char> *image;
  unsigned width;
  unsigned height;

};
