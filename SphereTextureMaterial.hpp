#pragma once
#include <glm/glm.hpp>
#include <vector>

#include "TextureMaterial.hpp"

class SphereTextureMaterial : public TextureMaterial {
public:
  SphereTextureMaterial(const std::string & file_name, double m_shininess);
  virtual ~SphereTextureMaterial();

  glm::vec3 getColor(glm::vec3 pHit, 
    glm::vec3 pNormal, 
    Light *light,
    glm::mat4 inv);

};
