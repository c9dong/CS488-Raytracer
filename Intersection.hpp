#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <iostream>

#include "Material.hpp"
#include "Ray.hpp"

class Intersection {
public:
  struct Point {
    Point() {};
    Point(float value, int position, glm::vec3 normal, Material *mat) {
        this->value = value;
        this->position = position;
        this->normal = normal;
        this->mat = mat;
      };
    Point(const Point &p) : value(p.value), normal(p.normal), mat(p.mat) {};

    void set(Point &p) {
      value = p.value;
      position = p.position;
      normal = p.normal;
      mat = p.mat;
    }
    float value = 0.0f;
    int position = 0; // 0 = start, 1 = end
    glm::vec3 normal;
    Material *mat;
  };

  struct Range {
    Range() : hit(false){};
    Range(Point &sp, Point &ep) {
      start = sp.value;
      s_normal = sp.normal;
      s_mat = sp.mat;

      end = ep.value;
      e_normal = ep.normal;
      e_mat = ep.mat;

      hit = true;
    }
    Range(const Range& r) 
    : start(r.start), 
      end(r.end), 
      s_normal(r.s_normal), 
      e_normal(r.e_normal),
      s_mat(r.s_mat),
      e_mat(r.e_mat),
      hit(r.hit) {};

    float start;
    float end;
    glm::vec3 s_normal;
    glm::vec3 e_normal;
    Material *s_mat;
    Material *e_mat;
    bool hit;
  };

  struct Hit {
    glm::vec3 pHit;
    glm::vec3 pNormal;
    Material *mat;
    bool hit;
  };

  Intersection();
  virtual ~Intersection();

  void addRange(Range *range);
  Intersection intersect_intersection(Intersection &other);
  Intersection union_intersection(Intersection &other);
  Intersection difference_intersection(Intersection &other);

  Intersection::Hit getFirstHit(Ray &ray);
  void transformRanges(Ray &ray, Ray &inv_ray, glm::mat4 trans, glm::mat4 invtrans);


  std::vector<Range *> ranges;
private:

};