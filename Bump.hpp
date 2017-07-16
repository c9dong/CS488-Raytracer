#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

class Bump {
public:
  Bump();
  Bump(const std::string & file_name);
  virtual ~Bump();

  virtual glm::vec3 getNormal(glm::vec3 normal, float u, float v);
  glm::vec3 colorAt(int x, int y);

  std::vector<unsigned char> *image;
  unsigned width;
  unsigned height;
};

class NoBump : public Bump {
public:
  NoBump(const std::string & file_name);
  ~NoBump();

  glm::vec3 getNormal(glm::vec3 normal, float u, float v);
};

