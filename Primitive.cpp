#include <iostream>

#include "Primitive.hpp"
#include "Util.hpp"
#include "cs488-framework/MathUtils.hpp"

using namespace glm;
using namespace std;

Primitive::~Primitive()
{
}

Intersection::Range* Primitive::intersect(Ray &ray, bool checkBound) {
  return new Intersection::Range();
}

Sphere::~Sphere()
{
}

Intersection::Range* Sphere::intersect(Ray &ray, bool checkBound) {
  NonhierSphere s(vec3(0,0,0), 1);
  return s.intersect(ray, checkBound);
}

Cube::~Cube()
{
}

Intersection::Range* Cube::intersect(Ray &ray, bool checkBound) {
  NonhierBox b(vec3(0,0,0), 1);
  return b.intersect(ray, checkBound);
}

Cone::~Cone()
{
}

Intersection::Range* Cone::intersect(Ray &ray, bool checkBound) {
  NonhierCone c(vec3(0,0,0), 1, 1);
  return c.intersect(ray, checkBound);
}

Cylinder::~Cylinder()
{
}

Intersection::Range* Cylinder::intersect(Ray &ray, bool checkBound) {
  NonhierCylinder c(vec3(0,0,0), 1, 1);
  return c.intersect(ray, checkBound);
}

NonhierSphere::~NonhierSphere()
{
}

Intersection::Range* NonhierSphere::intersect(Ray &ray, bool checkBound) {
  vec3 &d = ray.direction;
  vec3 &a = ray.origin;
  vec3 v = a - m_pos;
  float rad = m_radius;
  float c_a = glm::dot(d,d);
  float c_b = 2.0f*glm::dot(d, v);
  float c_c = glm::dot(v, v) - (rad*rad);
  float deter = (c_b*c_b - 4 * c_a * c_c);
  
  if (deter < 0 || isZero(deter)) {
    return new Intersection::Range();
  }
  float t1 = (-c_b + sqrt(deter)) / (2*c_a);
  float t2 = (-c_b - sqrt(deter)) / (2*c_a);

  if (t1 < 0 && t2 < 0) return new Intersection::Range();

  float tmin = glm::min(t1, t2);
  float tmax = glm::max(t1, t2);
  
  vec3 pHitMin = a + tmin * d;
  vec3 pHitMax = a + tmax * d;

  vec3 pNormalMin = glm::normalize(pHitMin - m_pos);
  vec3 pNormalMax = glm::normalize(pHitMax - m_pos);
  
  Intersection::Range *r = new Intersection::Range();
  r->start = tmin;
  r->end = tmax;
  r->s_normal = pNormalMin;
  r->e_normal = -pNormalMax;
  r->hit = true;
  return r;
}

NonhierBox::~NonhierBox()
{
}

glm::vec3 NonhierBox::getNormal(glm::vec3 pHit, glm::vec3 bound_min, glm::vec3 bound_max) {
  vec3 pNormal = vec3(0,0,0);
  float ep = 0.001f;

  if (abs(pHit.x - bound_min.x) < ep) {
    pNormal = vec3(-1, 0, 0);
  }

  if (abs(pHit.x - bound_max.x) < ep) {
    pNormal = vec3(1, 0, 0);
  }

  if (abs(pHit.y - bound_min.y) < ep) {
    pNormal = vec3(0, -1, 0);
  }

  if (abs(pHit.y - bound_max.y) < ep) {
    pNormal = vec3(0, 1, 0);
  }

  if (abs(pHit.z - bound_min.z) < ep) {
    pNormal = vec3(0, 0, -1);
  }

  if (abs(pHit.z - bound_max.z) < ep) {
    pNormal = vec3(0, 0, 1);
  }
  // assert(!(pNormal.x == 0 && pNormal.y == 0 && pNormal.z == 0));
  return pNormal;
}

Intersection::Range* NonhierBox::intersect(Ray &ray, bool checkBound) {
  vec3 &d = ray.direction;
  vec3 &a = ray.origin;

  vec3 normal = vec3(-1, -1, -1);

  float r = m_size;
  vec3 bound_min = vec3(m_pos.x-r/2, m_pos.y-r/2, m_pos.z-r/2);
  vec3 bound_max = vec3(m_pos.x+r/2, m_pos.y+r/2, m_pos.z+r/2);

  float tmin = (bound_min.x - a.x) / d.x;
  float tmax = (bound_max.x - a.x) / d.x;

  if (tmin > tmax) {
    normal.x = 1;
    float temp = tmin;
    tmin = tmax;
    tmax = temp;
  }

  float tymin = (bound_min.y - a.y) / d.y;
  float tymax = (bound_max.y - a.y) / d.y;

  if (tymin > tymax) {
    normal.y = 1;
    float temp = tymin;
    tymin = tymax;
    tymax = temp;
  }

  if ((tmin > tymax) || (tymin > tmax)) {
    return new Intersection::Range();
  }

  if (tymin > tmin) {
    normal.x = 0;
    tmin = tymin;
  } else {
    normal.y = 0;
  }

  if (tymax < tmax) {
    tmax = tymax;
  }

  float tzmin = (bound_min.z - a.z) / d.z;
  float tzmax = (bound_max.z - a.z) / d.z;

  if (tzmin > tzmax) {
    normal.z = 1;
    float temp = tzmin;
    tzmin = tzmax;
    tzmax = temp;
  }

  if ((tmin > tzmax) || (tzmin > tmax)) {
    return new Intersection::Range();
  }

  if (tzmin > tmin) {
    normal.y = 0;
    normal.x = 0;
    tmin = tzmin;
  } else {
    normal.z = 0;
  }

  if (tzmax < tmax) {
    tmax = tzmax;
  }

  if (tmin < 0 && tmax < 0) return new Intersection::Range();

  float t1 = glm::min(tmin, tmax);
  float t2 = glm::max(tmin, tmax);

  vec3 pHitMin = a + t1 * d;
  vec3 pHitMax = a + t2 * d;

  vec3 pNormalMin = getNormal(pHitMin, bound_min, bound_max);
  vec3 pNormalMax = getNormal(pHitMax, bound_min, bound_max);

  Intersection::Range *range = new Intersection::Range();;
  range->start = t1;
  range->end = t2;
  range->s_normal = pNormalMin;
  range->e_normal = -pNormalMax;
  range->hit = true;
  return range;
}

NonhierCylinder::~NonhierCylinder() {

}

Intersection::Range* NonhierCylinder::intersect(Ray &ray, bool checkBound) {
  vec3 v = glm::normalize(ray.direction);
  vec3 c_dir = vec3(0, 1, 0);
  vec3 delta_p = ray.origin - vec3(m_pos.x, m_pos.y-m_height, m_pos.z);
  vec3 d1 = v - glm::dot(v, c_dir) * c_dir;
  vec3 d2 = delta_p - glm::dot(delta_p, c_dir) * c_dir;

  float a = glm::dot(d1, d1);
  float b = 2 * glm::dot(d1, d2);
  float c = glm::dot(d2, d2) - m_radius * m_radius;

  float deter = b * b - 4 * a * c;
  bool hasTMin = true;
  bool hasTMax = true;
  float t1 = 0;
  float t2 = 0;
  if (deter < 0) {
    hasTMin = false;
    hasTMax = false;
  } else {
    t1 = (-b + sqrt(deter)) / (2.0f * a);
    t2 = (-b - sqrt(deter)) / (2.0f * a);
  }

  float tmin = glm::min(t1, t2);
  float tmax = glm::max(t1, t2);

  vec3 pHitMin = ray.origin + tmin * v;
  vec3 pHitMax = ray.origin + tmax * v;

  vec3 pNormalMin = glm::normalize(pHitMin - m_pos);
  pNormalMin.y = 0;
  vec3 pNormalMax = glm::normalize(pHitMax - m_pos);
  pNormalMax.y = 0;

  if (!(pHitMin.y >= m_pos.y-m_height && pHitMin.y <= m_pos.y + m_height)) {
    hasTMin = false;
  }

  if (!(pHitMax.y >= m_pos.y-m_height && pHitMax.y <= m_pos.y + m_height)) {
    hasTMax = false;
  }

  bool hasPt1 = true;
  bool hasPt2 = true;
  float pt1;
  float pt2;
  vec3 p1 = vec3(m_pos.x, m_pos.y - m_height, m_pos.z);;
  vec3 n1 = vec3(0, -1, 0);
  float denom1 = glm::dot(v, n1);
  if (isZero(denom1)) {
    hasPt1 = false;
  } else {
    pt1 = glm::dot(p1 - ray.origin, n1) / denom1;
    float dist1 = glm::distance(p1, ray.origin + pt1 * v);
    if (dist1 > m_radius) {
      hasPt1 = false;
    }
  }

  vec3 p2 = vec3(m_pos.x, m_pos.y + m_height, m_pos.z);
  vec3 n2 = vec3(0, 1, 0);
  float denom2 = glm::dot(v, n2);
  if (isZero(denom2)) {
    hasPt2 = false;
  } else {
    pt2 = glm::dot(p2 - ray.origin, n2) / denom2;
    if (glm::distance(p2, ray.origin + pt2 * v) > m_radius) {
      hasPt2 = false;
    }
  }

  float tMinTotal = min4(
    hasTMin ? tmin : INFINITY,
    hasTMax ? tmax : INFINITY,
    hasPt1 ? pt1 : INFINITY,
    hasPt2 ? pt2 : INFINITY);

  float tMaxTotal = max4(
    hasTMin ? tmin : -INFINITY,
    hasTMax ? tmax : -INFINITY,
    hasPt1 ? pt1 : -INFINITY,
    hasPt2 ? pt2 : -INFINITY);

  if (tMinTotal == INFINITY && tMaxTotal == -INFINITY) {
    return new Intersection::Range();
  }

  vec3 pNormalMinTotal;
  vec3 pNormalMaxTotal;

  if (tMinTotal == tmin) {
    pNormalMinTotal = pNormalMin;
  } else if (tMinTotal == tmax) {
    pNormalMinTotal = pNormalMax;
  } else if (tMinTotal == pt1) {
    pNormalMinTotal = n1;
  } else {
    pNormalMinTotal = n2;
  }

  if (tMaxTotal == tmin) {
    pNormalMaxTotal = pNormalMin;
  } else if (tMaxTotal == tmax) {
    pNormalMaxTotal = pNormalMax;
  } else if (tMaxTotal == pt1) {
    pNormalMaxTotal = n1;
  } else {
    pNormalMaxTotal = n2;
  }

  Intersection::Range *range = new Intersection::Range();
  range->start = tMinTotal;
  range->end = tMaxTotal;
  range->s_normal = pNormalMinTotal;
  range->e_normal = -pNormalMaxTotal;
  range->hit = true;
  return range;
}

NonhierCone::~NonhierCone() {

}

Intersection::Range* NonhierCone::intersect(Ray &ray, bool checkBound) {
  vec3 center = vec3(m_pos.x, m_pos.y+(m_height), m_pos.z);
  float angle = atan(m_radius / (m_height * 2.0f));

  if (angle <= 0) {
    return new Intersection::Range();
  }

  vec3 v = glm::normalize(ray.direction);
  vec3 c_dir = vec3(0, 1, 0);
  vec3 delta_p = ray.origin - center;
  
  float cosA2 = cos(angle) * cos(angle);

  float dv = glm::dot(v, c_dir);
  float cov = glm::dot(delta_p, c_dir);
  float a = dv * dv - cosA2;
  float b = 2 * (dv * cov - glm::dot(v, delta_p) * cosA2);
  float c = cov * cov - glm::dot(delta_p, delta_p) * cosA2;

  float deter = b * b - 4 * a * c;
  bool hasTMax = true;
  bool hasTMin = true;
  float t1 = 0;
  float t2 = 0;
  if (deter < 0) {
    hasTMax = false;
    hasTMin = false;
  } else {
    t1 = (-b + sqrt(deter)) / (2.0f * a);
    t2 = (-b - sqrt(deter)) / (2.0f * a);
  }

  float tmin = glm::min(t1, t2);
  float tmax = glm::max(t1, t2);

  vec3 pHitMin = ray.origin + tmin * v;
  vec3 pHitMax = ray.origin + tmax * v;

  vec3 pNormalMin = glm::normalize(pHitMin - m_pos);
  pNormalMin.y = cos(angle);
  vec3 pNormalMax = glm::normalize(pHitMax - m_pos);
  pNormalMax.y = cos(angle);

  if (!(pHitMin.y > m_pos.y - m_height && pHitMin.y < m_pos.y + m_height)) {
    hasTMin = false;
  }
  if (!(pHitMax.y > m_pos.y - m_height && pHitMax.y < m_pos.y + m_height)) {
    hasTMax = false;
  }

  bool hasPt = true;
  float pt = 0;
  vec3 p1 = vec3(m_pos.x, m_pos.y-(m_height), m_pos.z);;
  vec3 n1 = vec3(0, -1, 0);
  float denom1 = glm::dot(v, n1);
  if (isZero(denom1)) {
    hasPt = false;
  } else {
    pt = glm::dot(p1 - ray.origin, n1) / denom1;
    if (glm::distance(p1, ray.origin + pt * v) > m_radius) {
      hasPt = false;
    }
  }

  float tMinTotal = min3(
    hasTMin ? tmin : INFINITY,
    hasTMax ? tmax : INFINITY,
    hasPt ? pt : INFINITY);

  float tMaxTotal = max3(
    hasTMin ? tmin : -INFINITY,
    hasTMax ? tmax : -INFINITY,
    hasPt ? pt : -INFINITY);

  if (tMinTotal == INFINITY && tMaxTotal == -INFINITY) {
    return new Intersection::Range();
  }

  vec3 pNormalMinTotal;
  vec3 pNormalMaxTotal;

  if (tMinTotal == tmin) {
    pNormalMinTotal = pNormalMin;
  } else if (tMinTotal == tmax) {
    pNormalMinTotal = pNormalMax;
  } else {
    pNormalMinTotal = n1;
  }

  if (tMaxTotal == tmin) {
    pNormalMaxTotal = pNormalMin;
  } else if (tMaxTotal == tmax) {
    pNormalMaxTotal = pNormalMax;
  } else {
    pNormalMaxTotal = n1;
  }

  Intersection::Range *range = new Intersection::Range();
  range->start = tMinTotal;
  range->end = tMaxTotal;
  range->s_normal = pNormalMinTotal;
  range->e_normal = -pNormalMaxTotal;
  range->hit = true;
  return range;
}