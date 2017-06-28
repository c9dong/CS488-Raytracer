#include "RayTrace.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "Intersection.hpp"
#include "cs488-framework/MathUtils.hpp"
#include "Util.hpp"
#include "PhongMaterial.hpp"

using namespace glm;
using namespace std;

#define SHADOW
// #define REFLECTION
// #define REFRACTION

static bool print = false;

RayTrace::RayTrace(
    SceneNode * root,
    Image & image,
    const glm::vec3 & eye,
    const glm::vec3 & view,
    const glm::vec3 & up,
    double fovy,
    const glm::vec3 & ambient,
    const std::list<Light *> & lights)
    : root(root), 
      image(image), 
      eye(eye),
      view(view),
      up(up),
      fovy(fovy),
      ambient(ambient),
      lights(lights)
{

}

RayTrace::~RayTrace() {}

void RayTrace::generateImage() {
  size_t h = image.height();
  size_t w = image.width();

  mat4 p_to_world = getPointToWorldMatrix();

  for (uint y = 0; y < h; ++y) {
    for (uint x = 0; x < w; ++x) {
      // cout << "...................." << endl;
      if (x == 100 && y == 100) print = true;
      vec4 p_world = p_to_world * vec4(float(x), float(y), 0.0f, 1.0f);

      vec3 r_origin = vec3(eye);
      vec3 r_direction = vec3(p_world) - r_origin;
      Ray ray(r_origin, r_direction);
      // vec3 background;
      // cout << p_world.x << " " << p_world.y << " " << p_world.z << " " << p_world[3] << endl;
      // if (isZero(p_world.x + p_world.y)) {
      //   background = vec3(1,0,0);
      // } else {
        // vec3 background = vec3(0, 0, float(y) / float(h));
      // }

      double latitude = sin(r_direction[1]);  // -pi/2 to pi/2
      // scale to 0 to 1
      latitude = (latitude/M_PI) + 0.5;
      vec3 background =  getBackgroundColor(ray);

      vec3 col = getRayColor(ray, background, 0);
      image(x, y, 0) = col.r;
      image(x, y, 1) = col.g;
      image(x, y, 2) = col.b;
      print = false;
    }
  }
}

mat4 RayTrace::getPointToWorldMatrix() {
  size_t h = image.height();
  size_t w = image.width();

  float nx = float(w);
  float ny = float(h);
  float d = 1.0f;

  float wh = 2 * d * tan(degreesToRadians(fovy/2.0f));
  float ww = (nx / ny) * wh;

  vec3 v_w = glm::normalize(view);
  vec3 v_u = glm::normalize(glm::cross(up, v_w));
  vec3 v_v = glm::cross(v_w, v_u);

  mat4 T1 = glm::translate(mat4(1), vec3(-nx/2.0f, -ny/2.0f, d));
  mat4 S = glm::scale(mat4(1), vec3(-ww/nx, -wh/ny, 1));
  mat4 R = mat4(
    v_u.x, v_u.y, v_u.z, 0,
    v_v.x, v_v.y, v_v.z, 0,
    v_w.x, v_w.y, v_w.z, 0,
    0, 0, 0, 1
    );
  mat4 T2 = glm::translate(mat4(1), eye);

  return T2 * R * S * T1;
}

glm::vec3 RayTrace::getRayColor(Ray & ray, glm::vec3 & background, int maxHit) {
  Intersection intersection = root->intersect(ray, false);
  vec3 col;
  if (intersection.hit) {
    col = ambient;
    for(Light * light : lights) {
      Intersection shadowIntersection;
      // check shadow
      #ifdef SHADOW
      vec3 shadow_origin = intersection.pHit + intersection.pNormal*0.1f;
      vec3 shadow_direction = (light->position - shadow_origin);
      Ray shadowRay(shadow_origin, shadow_direction);
      shadowIntersection = root->intersect(shadowRay, true);
      #endif
      if (!shadowIntersection.hit) {
        col += intersection.mat->getColor(intersection.pHit, intersection.pNormal, light);
      }
    }

    // reflection
    PhongMaterial *pMat;
    if ((pMat = dynamic_cast<PhongMaterial*>(intersection.mat))) {
      #ifdef REFLECTION
      if (!isZero(pMat->m_shininess) && maxHit < 10) {
        maxHit ++;
        vec3 ref_origin = intersection.pHit + intersection.pNormal*0.1f;
        vec3 ref_direction = 
          ray.direction - 
          2.0f * 
          glm::normalize(intersection.pNormal) * 
          glm::dot(glm::normalize(ray.direction), glm::normalize(intersection.pNormal));
        Ray refRay(ref_origin, ref_direction);
        vec3 rcol = getRayColor(refRay, background, maxHit);
        col += float(pMat->m_shininess)/100.0f * rcol;
      }
      #endif
      #ifdef REFRACTION
      if (maxHit < 10) {
        maxHit ++;
        vec3 refract_angle = getRefractAngle(1.3, ray.direction, intersection.pNormal);
        if (!isZero(refract_angle)) {
          vec3 test_origin = intersection.pHit - intersection.pNormal*0.1f;
          Ray testRay(test_origin, refract_angle);
          Intersection refractIntersection = root->intersect(testRay, false);
          assert(refractIntersection.hit);
          vec3 refract_origin = refractIntersection.pHit + refractIntersection.pNormal*0.1f;
          Ray refractRay(refract_origin, refract_angle);
          // Ray refractRay(test_origin, refract_angle);
          vec3 rcol = getRayColor(refractRay, background, maxHit);
          col += 0.5f * rcol;
        } else {
          return getBackgroundColor(ray);
        }
      }
      #endif
    }
  } else {
    col = getBackgroundColor(ray);
  }
  return col;
}

glm::vec3 RayTrace::getRefractAngle(float kr, glm::vec3 &d, glm::vec3 &n) {
  float d_dot_n = glm::dot(glm::normalize(d), glm::normalize(n));
  float root = 1.0f - kr*kr*(1.0f - d_dot_n*d_dot_n);
  if (root < 0) {
    return vec3(0);
  }
  vec3 res = kr * d + (-kr * d_dot_n - sqrt(root)) * n;
  return res;
}

glm::vec3 RayTrace::getBackgroundColor(Ray &ray) {
  vec3 normal = vec3(0, -1, 0);
  vec3 p = vec3(0, -200, 0);

  float n_dot_d = glm::dot(ray.direction, normal);
  if (isZero(n_dot_d)) {
    return vec3(1.0, 0.87, 0.68);
  }
  float t = glm::dot(p - ray.origin, normal) / n_dot_d;
  if (t < 0) {
    return vec3(1.0, 0.87, 0.68);
  }
  vec3 point = ray.origin + t * ray.direction;
  float x = point.x;
  float y = point.z;
  int size = 100;
  
  int alt = 1;
  int mult = 1;
  if (x < 0) {
    mult = -1;
  }
  if (int(x/size) % 2 == 0) {
    if (int(y/size) % 2 == 0) {
      alt = 1 * mult;
    } else {
      alt = -1 * mult;
    }
  } else {
    if (int(y/size) % 2 == 0) {
      alt = -1 * mult;
    } else {
      alt = 1 * mult;
    }
  }
  if (alt == 1) {
    return vec3(1);
  } else {
    return vec3(0.5, 0.5, 0.7);
  }
}