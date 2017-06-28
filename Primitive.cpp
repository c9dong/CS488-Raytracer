#include <iostream>

#include "Primitive.hpp"
#include "Util.hpp"
#include "cs488-framework/MathUtils.hpp"

using namespace glm;
using namespace std;

Primitive::~Primitive()
{
}

Intersection Primitive::intersect(Ray &ray, bool checkBound) {
  return Intersection();
}

Sphere::~Sphere()
{
}

Intersection Sphere::intersect(Ray &ray, bool checkBound) {
  NonhierSphere s(vec3(0,0,0), 1);
  return s.intersect(ray, checkBound);
}

Cube::~Cube()
{
}

Intersection Cube::intersect(Ray &ray, bool checkBound) {
  NonhierBox b(vec3(0,0,0), 1);
  return b.intersect(ray, checkBound);
}

NonhierSphere::~NonhierSphere()
{
}

Intersection NonhierSphere::intersect(Ray &ray, bool checkBound) {
  vec3 &d = ray.direction;
  vec3 &a = ray.origin;
  vec3 v = a - m_pos;
  float rad = m_radius;
  float c_a = glm::dot(d,d);
  float c_b = 2.0f*glm::dot(d, v);
  float c_c = glm::dot(v, v) - (rad*rad);
  float deter = (c_b*c_b - 4 * c_a * c_c);
  
  if (deter < 0 || isZero(deter)) {
    return Intersection();
  }
  float t1 = (-c_b + sqrt(deter)) / (2*c_a);
  float t2 = (-c_b - sqrt(deter)) / (2*c_a);
  float t = glm::min(t1, t2);
  if (t1 < 0 && t1 < 0) {
    return Intersection();
  } else if (t1 < 0) {
    t = t2;
  } else if (t2 < 0) {
    t = t1;
  } else {
    t = glm::min(t1, t2);
  }
  // if (checkBound) {
  //   if (!((t1>=0 && t1<=1) || (t2>=0 && t2<=1))) {
  //     return Intersection();
  //   }
  //   if (t1>=0 && t1<=1) {
  //     t = t1;
  //   }
  //   if (t2>=0 && t2<=1) {
  //     t = glm::min(t, t2);
  //   }
  // }

  vec3 pHit = a + t*d;
  vec3 pNormal = glm::normalize(pHit - m_pos);
  return Intersection(nullptr, pHit, pNormal, glm::distance(a, pHit), true);
}

NonhierBox::~NonhierBox()
{
}

Intersection NonhierBox::intersect(Ray &ray, bool checkBound) {
  vec3 &d = ray.direction;
  vec3 &a = ray.origin;

  vec3 normal = vec3(-1, -1, -1);

  float r = m_size;
  vec3 bound_min = vec3(m_pos.x, m_pos.y, m_pos.z);
  vec3 bound_max = vec3(m_pos.x+r, m_pos.y+r, m_pos.z+r);

  float tmin = (bound_min.x - a.x) / d.x;
  float tmax = (bound_max.x - a.x) / d.x;
  if (d.x == 0) {
    tmin = -INFINITY;
    tmax = -INFINITY;
  }

  if (tmin > tmax) {
    normal.x = 1;
    float temp = tmin;
    tmin = tmax;
    tmax = temp;
  }

  float tymin = (bound_min.y - a.y) / d.y;
  float tymax = (bound_max.y - a.y) / d.y;

  if (d.y == 0) {
    tymin = -INFINITY;
    tymax = -INFINITY;
  }

  if (tymin > tymax) {
    normal.y = 1;
    float temp = tymin;
    tymin = tymax;
    tymax = temp;
  }

  if ((tmin > tymax) || (tymin > tmax)) {
    return Intersection();
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

  if (d.z == 0) {
    tzmin = -INFINITY;
    tzmax = -INFINITY;
  }

  if (tzmin > tzmax) {
    normal.z = 1;
    float temp = tzmin;
    tzmin = tzmax;
    tzmax = temp;
  }

  if ((tmin > tzmax) || (tzmin > tmax)) {
    return Intersection();
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

  float t = tmin;

  if (tmin < 0 && tmax < 0) {
    return Intersection();
  } else if (tmin < 0) {
    t = tmax;
  } else if (tmax < 0) {
    t = tmin;
  } else {
    t = glm::min(tmin, tmax);
  }

  // if (checkBound) {
  //   if (!((tmin>=0 && tmin<=1) || (tmax>=0 && tmax<=1))) {
  //     return Intersection();
  //   }
  //   if (tmin>=0 && tmin<=1) {
  //     t = tmin;
  //   }
  //   if (tmax>=0 && tmax<=1) {
  //     t = glm::min(t, tmax);
  //   }
  // }

  vec3 pHit = a + t*d;
  vec3 pNormal = vec3(0,0,0);
  float ep = 0.001f;
  // cout << "///////////////////" << endl;
  // cout << pHit.x - bound_min.x << endl;
  if (abs(pHit.x - bound_min.x) < ep) {
    pNormal = vec3(-1, 0, 0);
  }
  // cout << pHit.x - bound_max.x << endl;
  if (abs(pHit.x - bound_max.x) < ep) {
    pNormal = vec3(1, 0, 0);
  }
  // cout << pHit.y - bound_min.y << endl;
  if (abs(pHit.y - bound_min.y) < ep) {
    pNormal = vec3(0, -1, 0);
  }
  // cout << pHit.y - bound_max.y << endl;
  if (abs(pHit.y - bound_max.y) < ep) {
    pNormal = vec3(0, 1, 0);
  }
  // cout << pHit.z - bound_min.z << endl;
  if (abs(pHit.z - bound_min.z) < ep) {
    pNormal = vec3(0, 0, -1);
  }
  // cout << pHit.z - bound_max.z << endl;
  if (abs(pHit.z - bound_max.z) < ep) {
    pNormal = vec3(0, 0, 1);
  }
  assert(!(pNormal.x == 0 && pNormal.y == 0 && pNormal.z == 0));

  return Intersection(nullptr, pHit, pNormal, glm::distance(a, pHit), true);
}

NonhierCylinder::~NonhierCylinder() {

}

Intersection NonhierCylinder::intersect(Ray &ray, bool checkBound) {
  vec3 v = ray.direction;
  vec3 c_dir = vec3(0, 1, 0);
  vec3 delta_p = ray.origin - m_pos;
  vec3 d1 = v - glm::dot(v, c_dir) * c_dir;
  vec3 d2 = delta_p - glm::dot(delta_p, c_dir) * c_dir;

  float a = glm::dot(d1, d1);
  float b = 2 * glm::dot(d1, d2);
  float c = glm::dot(d2, d2) - m_radius * m_radius;

  float deter = b * b - 4 * a * c;
  if (deter < 0) {
    return Intersection();
  }

  float t1 = (-b + sqrt(deter)) / (2.0f * a);
  float t2 = (-b - sqrt(deter)) / (2.0f * a);

  float t;
  if (t1 < 0 && t2 < 0) {
    return Intersection();
  } else if (t1 < 0) {
    t = t2;
  } else if (t2 < 0) {
    t = t1;
  } else {
    t = glm::min(t1, t2);
  }

  vec3 pHit = ray.origin + t * v;
  if (pHit.y >= m_pos.y && pHit.y <= m_pos.y + m_height) {
    vec3 pNormal = pHit - m_pos;
    pNormal.y = 0;
    pNormal = glm::normalize(pNormal);

    return Intersection(nullptr, pHit, pNormal, glm::distance(ray.origin, pHit), true);
  } else {
    vec3 p1 = m_pos;
    vec3 n1 = vec3(0, -1, 0);
    float denom1 = glm::dot(v, n1);
    if (isZero(denom1)) {
      return Intersection();
    }
    float pt1 = glm::dot(p1 - ray.origin, n1) / denom1;
    if (glm::distance(p1, ray.origin + pt1 * v) > m_radius) {
      pt1 = -1;
    }

    vec3 p2 = vec3(m_pos.x, m_pos.y + m_height, m_pos.z);
    vec3 n2 = vec3(0, 1, 0);
    float denom2 = glm::dot(v, n2);
    if (isZero(denom2)) {
      return Intersection();
    }
    float pt2 = glm::dot(p2 - ray.origin, n2) / denom2;
    if (glm::distance(p2, ray.origin + pt2 * v) > m_radius) {
      pt2 = -1;
    }

    vec3 pNormal;
    if (pt1 < 0 && pt2 < 0) {
      return Intersection();
    } else if (pt1 < 0) {
      t = pt2;
      pNormal = n2;
    } else if (pt2 < 0) {
      t = pt1;
      pNormal = n1;
    } else {
      if (pt1 < pt2) {
        t = pt1;
        pNormal = n1;
      } else {
        t = pt2;
        pNormal = n2;
      }
    }

    pHit = ray.origin + t * v;
    return Intersection(nullptr, pHit, pNormal, glm::distance(ray.origin, pHit), true);
  }
  return Intersection();
}

NonhierCone::~NonhierCone() {

}

Intersection NonhierCone::intersect(Ray &ray, bool checkBound) {
  vec3 center = vec3(m_pos.x, m_pos.y+m_height, m_pos.z);
  float angle = atan(m_radius / m_height);

  if (angle <= 0) {
    return Intersection();
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

  // float q = d2;
  // float w = pva;
  // cout << ray.origin.y << endl;

  float deter = b * b - 4 * a * c;
  if (deter < 0) {
    return Intersection();
  }

  float t1 = (-b + sqrt(deter)) / (2.0f * a);
  float t2 = (-b - sqrt(deter)) / (2.0f * a);

  float t;
  if (t1 < 0 && t2 < 0) {
    return Intersection();
  } else if (t1 < 0) {
    t = t2;
  } else if (t2 < 0) {
    t = t1;
  } else {
    t = glm::min(t1, t2);
  }

  vec3 pHit = ray.origin + t * v;
  if (pHit.y > m_pos.y && pHit.y < m_pos.y + m_height) {
    vec3 pNormal = pHit - m_pos;
    pNormal.y = cos(angle);
    pNormal = glm::normalize(pNormal);

    return Intersection(nullptr, pHit, pNormal, glm::distance(ray.origin, pHit), true);
  } else {
    vec3 p1 = m_pos;
    vec3 n1 = vec3(0, -1, 0);
    float denom1 = glm::dot(v, n1);
    if (isZero(denom1)) {
      return Intersection();
    }
    float pt1 = glm::dot(p1 - ray.origin, n1) / denom1;
    if (glm::distance(p1, ray.origin + pt1 * v) <= m_radius) {
      t = pt1;
    } else {
      return Intersection();
    }

    pHit = ray.origin + t * v;
    vec3 pNormal = n1;

    return Intersection(nullptr, pHit, pNormal, glm::distance(ray.origin, pHit), true);
  }
  return Intersection();
}