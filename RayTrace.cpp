#include "RayTrace.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <pthread.h>

#include "Intersection.hpp"
#include "cs488-framework/MathUtils.hpp"
#include "Util.hpp"
#include "Material.hpp"
#include "PhongMaterial.hpp"
#include "TransparentMaterial.hpp"

using namespace glm;
using namespace std;

#define NUM_THREAD_ROOT_2 3

#define ANTIALISING

// #define REFLECTION
// #define REFRACTION

RayTrace::RayTrace(
    SceneNode * root,
    Image & image,
    const glm::vec3 & eye,
    const glm::vec3 & view,
    const glm::vec3 & up,
    double fovy,
    const glm::vec3 & ambient,
    const std::list<Light *> & lights,
    Shadow &shadow)
    : root(root), 
      image(image), 
      eye(eye),
      view(view),
      up(up),
      fovy(fovy),
      ambient(ambient),
      lights(lights),
      shadow(shadow)
{

}

RayTrace::~RayTrace() {}

void *createImagePart(void *arguments) {
  RayTrace::GenerateArg *arg = (RayTrace::GenerateArg *)arguments;
  int x_start = arg->x_start;
  int x_size = arg->x_size;
  int x_max = arg->x_max;
  int y_start = arg->y_start;
  int y_size = arg->y_size;
  int y_max = arg->y_max;
  mat4 world_mat = arg->world_mat;
  RayTrace *raytrace = arg->raytrace;

  for (int x=x_start; x<std::min(x_start+x_size, x_max); x++) {
    for (int y=y_start; y<std::min(y_start+y_size, y_max); y++) {
      vec3 col = vec3(0);
      #ifdef ANTIALISING
      for (float x_delta=-0.5; x_delta<=0.5; x_delta+=0.5) {
        for (float y_delta=-0.5; y_delta<=0.5; y_delta+=0.5) {
          float new_x = float(x)+x_delta;
          float new_y = float(y)+y_delta;

          vec4 p_world = world_mat * vec4(new_x, new_y, 0.0f, 1.0f);

          vec3 r_origin = vec3(raytrace->getEye());
          vec3 r_direction = vec3(p_world) - r_origin;
          Ray ray(r_origin, r_direction);

          vec3 background = raytrace->getBackgroundColor(ray);

          col += raytrace->getRayColor(ray, background, 0, nullptr);
        }  
      }
      col = col / 9.0f;
      #endif
      #ifndef ANTIALISING
      vec4 p_world = world_mat * vec4(x, y, 0.0f, 1.0f);

      vec3 r_origin = vec3(raytrace->getEye());
      vec3 r_direction = vec3(p_world) - r_origin;
      Ray ray(r_origin, r_direction);

      vec3 background = raytrace->getBackgroundColor(ray);

      col += raytrace->getRayColor(ray, background, 0, nullptr);
      #endif

      raytrace->getImage()(x, y, 0) = col.r;
      raytrace->getImage()(x, y, 1) = col.g;
      raytrace->getImage()(x, y, 2) = col.b;
    }
  }
  pthread_exit(NULL);
}

Image& RayTrace::getImage() {
  return image;
}

const glm::vec3& RayTrace::getEye() {
  return eye;
}

void RayTrace::generateImage() {
  size_t h = image.height();
  size_t w = image.width();

  mat4 p_to_world = getPointToWorldMatrix();

  int h_size = int(ceil(float(h) / NUM_THREAD_ROOT_2));
  int w_size = int(ceil(float(w) / NUM_THREAD_ROOT_2));

  pthread_t threads[NUM_THREAD_ROOT_2 * NUM_THREAD_ROOT_2];
  for (int i=0; i<NUM_THREAD_ROOT_2; i++) {
    for (int j=0; j<NUM_THREAD_ROOT_2; j++) {
      GenerateArg *args = new GenerateArg();
      args->x_start = i * w_size;
      args->x_size = w_size;
      args->x_max = int(w);
      args->y_start = j * h_size;
      args->y_size = h_size;
      args->y_max = int(h);
      args->world_mat = p_to_world;
      args->raytrace = this;

      int rc = pthread_create(&threads[i*NUM_THREAD_ROOT_2+j], NULL, createImagePart, (void *)args);
      if (rc){
         cout << "Error:unable to create thread," << rc << endl;
         exit(-1);
      }
    }
  }
  for (int i=0; i<NUM_THREAD_ROOT_2*NUM_THREAD_ROOT_2; i++) {
    pthread_join(threads[i], NULL);
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

// glm::vec3 RayTrace::getRayColor(Ray & ray, glm::vec3 & background, int maxHit, Material *lastMat) {
//   Intersection intersection = root->intersect(ray, false);
//   Intersection::Hit hit = intersection.getFirstHit(ray);
//   vec3 col = getBackgroundColor(ray);
//   if (hit.hit) {
//     Material *mat = hit->mat;
//     PhongMaterial *pMat;
//     TransparentMaterial *tMat;
//     if ((tMat = dynamic_cast<TransparentMaterial*>(hit.mat))) {
//       if (maxHit < 10) {
//         col = getRefractiveAndReflectiveColor();
//       } else {
//         return vec3(0);
//       }
//     } else if ((pMat = dynamic_cast<PhongMaterial*>(hit.mat))) {
//       col = getPhongColor();
//     } else {
//       assert(false);
//     }
//   }
// }

// glm::vec3 RayTrace::getRefractiveAndReflectiveColor(Ray &ray, Intersection::Hit hit, TransparentMaterial *tMat, int maxHit) {
//   // Refractive
//   vec3 refractColor;
//   float kr = 1.0f / tMat->refractIdx;
//   vec3 origin = hit.pHit - hit.pNormal*0.1f;
//   vec3 refract_direction = getRefractAngle(ray.direction, hit.pNormal, kr);
//   Ray refractRay(origin, refract_direction);
//   maxHit ++;
//   refractColor = getRayColor(refractRay, background, maxHit);
//   return refractColor;
// }

glm::vec3 RayTrace::getRayColor(Ray & ray, glm::vec3 & background, int maxHit, Material *lastMat) {
  Intersection *intersection = root->intersect(ray, false);
  vec3 col;
  Intersection::Hit hit = intersection->getFirstHit(ray);
  delete intersection;
  if (hit.hit) {
    col = ambient;
    TransparentMaterial *tMat;
    if ((tMat = dynamic_cast<TransparentMaterial*>(hit.mat))) {
      if (maxHit < 10) {
        float kr = 1.0f / tMat->refractIdx;
        vec3 origin = hit.pHit - hit.pNormal*0.1f;
        vec3 refract_direction = getRefractAngle(ray.direction, hit.pNormal, kr);
        Ray refractRay(origin, refract_direction);
        Intersection *internal_intersection = root->intersect(refractRay, false);
        Intersection::Hit internalHit = internal_intersection->getFirstHit(refractRay);
        delete internal_intersection;
        vec3 internal_origin = internalHit.pHit - internalHit.pNormal*0.1f;
        vec3 internal_refract_direction = getRefractAngle(refractRay.direction, internalHit.pNormal, kr);
        Ray newRefractRay(internal_origin, internal_refract_direction);
        int new_maxHit = maxHit + 1;
        vec3 refractColor;
        refractColor = getRayColor(newRefractRay, background, new_maxHit, nullptr);
        // return refractColor;

        vec3 reflect_origin = hit.pHit + hit.pNormal*0.1f;
        vec3 reflect_direction = getReflectionAngle(ray.direction, hit.pNormal);
        Ray reflectRay(reflect_origin, reflect_direction);
        vec3 reflectColor;
        reflectColor = getRayColor(reflectRay, background, new_maxHit, nullptr);
        return refractColor * float(tMat->transmittance) + reflectColor * float(tMat->reflectivity);
      } else {
        cout << "dfd" << endl;
        Ray forwardRay = Ray(hit.pHit + ray.direction*0.1f, ray.direction);
        col = getRayColor(forwardRay, background, maxHit, tMat);
      }
    } else {
      for(Light *light : lights) {
        col += shadow.getColor(hit, light, root);
      }
      col = col / float(lights.size());
    }

    // reflection
    // PhongMaterial *pMat;
    // if ((pMat = dynamic_cast<PhongMaterial*>(hit.mat))) {
    //   #ifdef REFLECTION
    //   if (pMat->m_refract != 1 && maxHit < 10) {
    //     maxHit ++;
    //     vec3 ref_origin = hit.pHit + hit.pNormal*0.1f;
    //     vec3 ref_direction = 
    //       ray.direction - 
    //       2.0f * 
    //       glm::normalize(hit.pNormal) * 
    //       glm::dot(glm::normalize(ray.direction), glm::normalize(hit.pNormal));
    //     Ray refRay(ref_origin, ref_direction);
    //     vec3 rcol = getRayColor(refRay, background, maxHit);
    //     col = float(1.0f-pMat->m_refract) * col + float(pMat->m_refract) * rcol;
    //     // col += float(pMat->m_shininess)/100.0f * rcol;
    //   }
    //   #endif
      // #ifdef REFRACTION
      // if (pMat->m_refract != 1.0f && maxHit < 10) {
      //   maxHit ++;
      //   vec3 refract_angle = getRefractAngle(1.0f / pMat->m_refract, ray.direction, hit.pNormal);
      //   if (!isZero(refract_angle)) {
      //     vec3 test_origin = hit.pHit - hit.pNormal*0.1f;
      //     // Ray testRay(test_origin, refract_angle);
      //     // Intersection refractIntersection = root->intersect(testRay, false);
      //     // Intersection::Hit refractHit = refractIntersection.getFirstHit(testRay);
      //     // assert(refractHit.hit);
      //     // vec3 refract_origin = refractHit.pHit + refractHit.pNormal*0.1f;
      //     // Ray refractRay(refract_origin, refract_angle);
      //     Ray refractRay(test_origin, refract_angle);
      //     vec3 rcol = getRayColor(refractRay, background, maxHit);
      //     col = float(1.0f-pMat->m_refract) * col + float(pMat->m_refract) * rcol;
      //   } else {
      //     return getBackgroundColor(ray);
      //   }
      // }
      // #endif
    // }
  } else {
    col = getBackgroundColor(ray);
  }
  return col;
}

glm::vec3 RayTrace::getRefractAngle(glm::vec3 direction, glm::vec3 normal, float kr) {
  float d_dot_n = -glm::dot(glm::normalize(direction), glm::normalize(normal));
  float root = 1.0f - kr*kr*(1.0f - d_dot_n*d_dot_n);
  if (root < 0) {
    assert(false);
  }
  vec3 res = kr * glm::normalize(direction) + (kr * d_dot_n - sqrt(root)) * glm::normalize(normal);
  return res;
}

glm::vec3 RayTrace::getReflectionAngle(glm::vec3 direction, glm::vec3 normal) {
  vec3 ref_direction = 
    direction - 
    2.0f * 
    glm::normalize(normal) * 
    glm::dot(glm::normalize(direction), glm::normalize(normal));
  return ref_direction;
}

glm::vec3 RayTrace::getBackgroundColor(Ray &ray) {
  return vec3(1.0, 0.87, 0.68);
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