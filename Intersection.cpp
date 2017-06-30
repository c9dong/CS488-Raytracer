#include "Intersection.hpp"
#include "cs488-framework/MathUtils.hpp"

#include <iostream>
#include <algorithm>
#include <vector>

using namespace glm;
using namespace std;

Intersection::Intersection() {
}

Intersection::~Intersection() {
}

bool sortFunc(Intersection::Range &r1, Intersection::Range &r2) {
  return r1.start < r2.start;
}

bool sortPoints(const Intersection::Point *p1, const Intersection::Point *p2) {
  if (p1->value == p2->value) {
    return p1->position < p2->position;
  }
  return p1->value < p2->value;
}

void Intersection::addRange(Range *range) {
  ranges.push_back(range);
}

Intersection Intersection::intersect_intersection(Intersection &other) {
  vector<Intersection::Point*> points;
  for (Range *r : ranges) {
    Intersection::Point *p1 = new Point(r->start, 0, r->s_normal, r->s_mat);
    Intersection::Point *p2 = new Point(r->end, 1, r->e_normal, r->e_mat);
    points.push_back(p1);
    points.push_back(p2);
  }
  for (Range *r : (other.ranges)) {
    Intersection::Point *p1 = new Point(r->start, 0, r->s_normal, r->s_mat);
    Intersection::Point *p2 = new Point(r->end, 1, r->e_normal, r->e_mat);
    points.push_back(p1);
    points.push_back(p2);
  }
  sort(points.begin(), points.end(), sortPoints);

  int counter = 0;
  Point *startPoint;
  Intersection newIntersection;
  for (Intersection::Point *p : points) {
    int lastCounter = counter;
    if (p->position == 0) {
      counter ++;
      startPoint = p;
    } else {
      counter --;
    }
    if (lastCounter - counter == 1 && !(lastCounter + counter == 1)) {
      newIntersection.addRange(new Range(*startPoint, *p));
    }
  }

  return newIntersection;
}

Intersection Intersection::union_intersection(Intersection &other) {
  vector<Intersection::Point*> points;
  for (Range *r : ranges) {
    Intersection::Point *p1 = new Point(r->start, 0, r->s_normal, r->s_mat);
    Intersection::Point *p2 = new Point(r->end, 1, r->e_normal, r->e_mat);
    points.push_back(p1);
    points.push_back(p2);
  }
  for (Range *r : (other.ranges)) {
    Intersection::Point *p1 = new Point(r->start, 0, r->s_normal, r->s_mat);
    Intersection::Point *p2 = new Point(r->end, 1, r->e_normal, r->e_mat);
    points.push_back(p1);
    points.push_back(p2);
  }
  sort(points.begin(), points.end(), sortPoints);

  int counter = 0;
  Point *startPoint;
  Intersection newIntersection;
  for (Intersection::Point *p : points) {
    int lastCounter = counter;
    if (p->position == 0) {
      counter ++;
    } else {
      counter --;
    }
    if (lastCounter == 0 && counter == 1) {
      startPoint = p;
    }
    if (lastCounter == 1 && counter == 0) {
      newIntersection.addRange(new Range(*startPoint, *p));
    }
  }

  return newIntersection;
}

Intersection Intersection::difference_intersection(Intersection &other) {
  vector<Intersection::Point*> points;
  for (Range *r : ranges) {
    Intersection::Point *p1 = new Point(r->start, -1, r->s_normal, r->s_mat);
    Intersection::Point *p2 = new Point(r->end, 1, r->e_normal, r->e_mat);
    points.push_back(p1);
    points.push_back(p2);
  }
  for (Range *r : (other.ranges)) {
    Intersection::Point *p1 = new Point(r->start, 0, r->s_normal, r->s_mat);
    Intersection::Point *p2 = new Point(r->end, 2, r->e_normal, r->e_mat);
    points.push_back(p1);
    points.push_back(p2);
  }
  sort(points.begin(), points.end(), sortPoints);

  int counter = 0;
  Point *startPoint = nullptr;
  Intersection newIntersection;
  for (Intersection::Point *p : points) {
    if (p->position == 0 || p->position == -1) {
      counter ++;
    } else {
      counter --;
    }
    if (p->position == -1 && counter == 1) {
      startPoint = p;
    } 

    if (p->position == 2 && counter != 0) {
      startPoint = p;
    }

    if (p->position == 0 && counter > 1) {
      newIntersection.addRange(new Range(*startPoint, *p));
    }

    if (p->position == 1 && counter == 0) {
      newIntersection.addRange(new Range(*startPoint, *p));
    }
  }

  return newIntersection;

  // for (Range r1 : ranges) {
  //   for (Range r2 : other.ranges) {
  //     Range newRange;
  //     if (r1.start <= r2.start) {
  //       newRange.start = r1.start;
  //       newRange.s_normal = r1.s_normal;
  //       newRange.s_mat = r1.s_mat;
  //       newRange.hit = true;
  //       if (r1.end <= r2.start) {
  //         newRange.end = r1.end;
  //         newRange.e_normal = r1.e_normal;
  //         newRange.e_mat = r1.e_mat;
  //       } else {
  //         newRange.end = r2.start;
  //         newRange.e_normal = r1.s_normal;
  //         newRange.e_mat = r1.s_mat;
  //       }
  //       newIntersection.addRange(newRange);
  //     } else {
  //       if (r2.end <= r1.end) {
  //         Range newRange;
  //         newRange.start = r2.end;
  //         newRange.s_normal = r2.e_normal;
  //         newRange.s_mat = r2.e_mat;
  //         newRange.end = r1.end;
  //         newRange.e_normal = r1.e_normal;
  //         newRange.e_mat = r1.e_mat;
  //         newIntersection.addRange(newRange);
  //       }
  //     }
  //   }
  // }
  // return newIntersection;
}

void Intersection::transformRanges(Ray &ray, Ray &inv_ray, glm::mat4 trans, glm::mat4 invtrans) {
  for (int i=0; i<ranges.size(); i++) {
    Range *range = ranges[i];
    vec3 minHitPrime = vec3(trans * vec4(inv_ray.origin + range->start * inv_ray.direction, 1));
    vec3 maxHitPrime = vec3(trans * vec4(inv_ray.origin + range->end * inv_ray.direction, 1));
    float startPrime = 0;
    float endPrime = 0;
    if (ray.direction.x != 0) {
      startPrime = (minHitPrime - ray.origin).x / ray.direction.x;
      endPrime = (maxHitPrime - ray.origin).x / ray.direction.x;
    } else if (ray.direction.y != 0) {
      startPrime = (minHitPrime - ray.origin).y / ray.direction.y;
      endPrime = (maxHitPrime - ray.origin).y / ray.direction.y;
    } else if (ray.direction.z != 0) {
      startPrime = (minHitPrime - ray.origin).z / ray.direction.z;
      endPrime = (maxHitPrime - ray.origin).z / ray.direction.z;
    }
    range->start = startPrime;
    range->end = endPrime;

    range->s_normal = glm::normalize(glm::transpose((mat3(invtrans))) * range->s_normal);
    range->e_normal = glm::normalize(glm::transpose((mat3(invtrans))) * range->e_normal);
  }
}

Intersection::Hit Intersection::getFirstHit(Ray &ray) {
  Intersection::Hit minHit;
  float minT = INFINITY;
  for (Range *r : ranges) {
    if (r->start >= 0 && r->start < minT) {
      minT = r->start;
      minHit.pNormal = r->s_normal;
      minHit.mat = r->s_mat;
    }

    if (r->end >= 0 && r->end < minT) {
      minT = r->end;
      minHit.pNormal = r->e_normal;
      minHit.mat = r->e_mat;
    }
  }

  if (minT == INFINITY) {
    minHit.hit = false;
    return minHit;
  }

  minHit.hit = true;

  minHit.pHit = ray.origin + minT * ray.direction;
  return minHit;
}