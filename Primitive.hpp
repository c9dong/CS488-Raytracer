#pragma once

#include <glm/glm.hpp>

#include "Intersection.hpp"
#include "Ray.hpp"

class Primitive {
public:
  virtual ~Primitive();
  virtual Intersection::Range* intersect(Ray &ray, bool checkBound);
};

class Sphere : public Primitive {
public:
  virtual ~Sphere();

  Intersection::Range* intersect(Ray &ray, bool checkBound);
};

class Cube : public Primitive {
public:
  virtual ~Cube();

  Intersection::Range* intersect(Ray &ray, bool checkBound);
};

class NonhierSphere : public Primitive {
public:
  NonhierSphere(const glm::vec3& pos, double radius)
    : m_pos(pos), m_radius(radius)
  {
  }
  virtual ~NonhierSphere();

  Intersection::Range* intersect(Ray &ray, bool checkBound);

private:
  glm::vec3 m_pos;
  double m_radius;
};

class NonhierBox : public Primitive {
public:
  NonhierBox(const glm::vec3& pos, double size)
    : m_pos(pos), m_size(size)
  {
  }
  
  virtual ~NonhierBox();

  Intersection::Range* intersect(Ray &ray, bool checkBound);

private:
  glm::vec3 getNormal(glm::vec3 pHit, glm::vec3 bound_min, glm::vec3 bound_max);

  glm::vec3 m_pos;
  double m_size;
};

class NonhierCylinder : public Primitive {
public:
  NonhierCylinder(const glm::vec3& pos, double radius, double height)
    : m_pos(pos), m_radius(radius), m_height(height)
  {
  }

  virtual ~NonhierCylinder();

  Intersection::Range* intersect(Ray &ray, bool checkBound);
private:
  glm::vec3 m_pos;
  double m_radius;
  double m_height;
};

class NonhierCone : public Primitive {
public:
  NonhierCone(const glm::vec3& pos, double radius, double height)
    : m_pos(pos), m_radius(radius), m_height(height)
  {
  }

  virtual ~NonhierCone();

  Intersection::Range* intersect(Ray &ray, bool checkBound);
private:
  glm::vec3 m_pos;
  double m_radius;
  double m_height;
};
