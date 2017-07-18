#include "RayTrace.hpp"

#include <glm/ext.hpp>
#include <glm/gtx/transform.hpp>
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

#define NUM_THREAD_ROOT_2 4
// #define GRID_OPT
#define GRID_RESOLUTION 1

RayTrace::RayTrace(
    SceneNode * root,
    Image & image,
    const glm::vec3 & eye,
    const glm::vec3 & view,
    const glm::vec3 & up,
    double fovy,
    const glm::vec3 & ambient,
    const std::list<Light *> & lights,
    Shadow &shadow,
    double focal_dist,
    double camera_radius,
    double camera_sample_rate,
    double anti_sample_radius,
    double anti_sample_rate,
    int x_start,
    int y_start,
    int x_size,
    int y_size,
    int id)
    : root(root), 
      image(image), 
      eye(eye),
      view(view),
      up(up),
      fovy(fovy),
      ambient(ambient),
      lights(lights),
      shadow(shadow),
      focal_dist(focal_dist),
      camera_radius(camera_radius),
      camera_sample_rate(camera_sample_rate),
      anti_sample_radius(anti_sample_radius),
      anti_sample_rate(anti_sample_rate),
      x_start(x_start),
      y_start(y_start),
      x_size(x_size),
      y_size(y_size),
      id(id)
{

}

RayTrace::~RayTrace() {}

static bool print = false;
void createImagePart(void *arguments) {
  RayTrace::GenerateArg *arg = (RayTrace::GenerateArg *)arguments;
  int x_start = arg->x_start;
  int x_size = arg->x_size;
  int x_max = arg->x_max;
  int y_start = arg->y_start;
  int y_size = arg->y_size;
  int y_max = arg->y_max;
  mat4 world_mat = arg->world_mat;
  RayTrace *raytrace = arg->raytrace;

  float anti_sample_rate = float(raytrace->getAntiSampleRate());
  float anti_sample_radius = float(raytrace->getAntiSampleRadius());

  for (int x=x_start; x<std::min(x_start+x_size, x_max); x++) {
    for (int y=y_start; y<std::min(y_start+y_size, y_max); y++) {
      // cout << x << " " << y << endl;
      // if (x == 125 && y == 200) print = true;
      vec3 col = vec3(0);
      for (float x_delta=-anti_sample_radius; x_delta<=anti_sample_radius; x_delta+=anti_sample_rate) {
        for (float y_delta=-anti_sample_radius; y_delta<=anti_sample_radius; y_delta+=anti_sample_rate) {
          float new_x = float(x)+x_delta;
          float new_y = float(y)+y_delta;

          vec4 p_world = world_mat * vec4(new_x, new_y, 0.0f, 1.0f);

          vec3 r_origin = vec3(raytrace->getEye());
          vec3 r_direction = vec3(p_world) - r_origin;
          float focalPlane = float(raytrace->getFocalDist());
          float camera_rad = float(raytrace->getCameraRadius());
          float camera_sample = float(raytrace->getCameraSampleRate());

          float t = (focalPlane - p_world.z) / r_direction.z;
          vec3 focalPoint = vec3(p_world.x*t, p_world.y*t, p_world.z*t);

          int random_sample_size = 16;
          vec3 final_col = vec3(0);
          for (float x_eye_delta=-camera_rad; x_eye_delta<=camera_rad; x_eye_delta+=camera_sample) {
            for (float y_eye_delta=-camera_rad; y_eye_delta<=camera_rad; y_eye_delta+=camera_sample) {
              vec3 new_p = vec3(p_world.x + x_eye_delta, p_world.y + y_eye_delta, p_world.z);
              vec3 r_f_origin = new_p;
              vec3 r_f_direction = glm::normalize(focalPoint - r_f_origin);
              Ray ray(r_f_origin, r_f_direction);
              vec3 background = raytrace->getBackgroundColor(ray);
              final_col += raytrace->getRayColor(ray, background, 0, nullptr);
            }
          }

          float camera_sample_size = (((2 * camera_rad / camera_sample) + 1) * ((2 * camera_rad / camera_sample) + 1));
          if (camera_sample_size != 0) {
            final_col = final_col / camera_sample_size;
          }
          col += final_col;
        }  
      }
      float anti_sample_size = (((2 * anti_sample_radius / anti_sample_rate) + 1) * ((2 * anti_sample_radius / anti_sample_rate) + 1));
      if (anti_sample_size != 0) {
        col = col / anti_sample_size;
      }

      raytrace->getImage()(x, y, 0) = col.r;
      raytrace->getImage()(x, y, 1) = col.g;
      raytrace->getImage()(x, y, 2) = col.b;
      if (print) {
        raytrace->getImage()(x, y, 0) = 0;
        raytrace->getImage()(x, y, 1) = 0;
        raytrace->getImage()(x, y, 2) = 0;
        print = false;
      }
    }
  }
}

Image& RayTrace::getImage() {
  return image;
}

const glm::vec3& RayTrace::getEye() {
  // mat4 m1 = glm::rotate(degreesToRadians(45.0f), vec3(1.0f, 0.0f, 0.0f));
  // mat4 m2 = glm::rotate(degreesToRadians(35.0f), vec3(0.0f, 1.0f, 0.0f));
  // vec4 v = vec4(0.0f, 0.0f, 10.0f, 1.0f);
  // mat4 t = m1*m2;
  // vec3 vvv =  vec3(t * v);
  // printVec3(vvv);
  return eye;
}

void RayTrace::generateImage() {
  #ifdef GRID_OPT
  cout << "GRID OPTIMIZATION" << endl;
  generateGrid();
  #endif
  size_t h = image.height();
  size_t w = image.width();

  mat4 p_to_world = getPointToWorldMatrix();

  GenerateArg *args = new GenerateArg();
  args->x_start = x_start;
  args->x_size = x_size;
  args->x_max = int(w);
  args->y_start = y_start;
  args->y_size = y_size;
  args->y_max = int(h);
  args->world_mat = p_to_world;
  args->raytrace = this;

  createImagePart((void *)args);
}

void RayTrace::generateGrid() {
  world_box = root->getBoundingBox();
  // printVec3(world_box->min_box);
  // printVec3(world_box->max_box);
  int i = -1;
  int j = -1;
  int k = -1;
  grid = new std::vector< std::vector<std::vector<std::vector<SceneNode *>* >* >* >();
  for (float x = world_box->min_box.x; x < world_box->max_box.x + GRID_RESOLUTION; x += GRID_RESOLUTION) {
    i++;
    // cout << "x: " << i << endl;
    grid->push_back(new std::vector<std::vector<std::vector<SceneNode *>* >* >());
    for (float y = world_box->min_box.y; y < world_box->max_box.y + GRID_RESOLUTION; y += GRID_RESOLUTION) {
      j++;
      // cout << "y: " << j << endl;
      (*grid)[i]->push_back(new std::vector<std::vector<SceneNode *>* >());
      for (float z = world_box->min_box.z; z < world_box->max_box.z + GRID_RESOLUTION; z += GRID_RESOLUTION) {
        k++;
        // cout << "z: " << k << endl;
        vec3 c_min = vec3(x, y, z);
        vec3 c_max = vec3(x+GRID_RESOLUTION, y+GRID_RESOLUTION, z+GRID_RESOLUTION);
        Primitive::BoundingBox *c_box = new Primitive::BoundingBox(c_min, c_max);
        vector<SceneNode *> *nodes = root->intersectBox(c_box);
        (*(*grid)[i])[j]->push_back(nodes);
        delete c_box;
      }
      k = -1;
    }  
    j = -1;
  }
  i = -1;
}

Intersection* RayTrace::intersect(Ray &ray) {
  float signx = sign(ray.direction.x);
  float signy = sign(ray.direction.y);
  float signz = -sign(ray.direction.z);
  float bx = world_box->min_box.x;
  float by = world_box->min_box.y;
  float bz = world_box->min_box.z;
  float tx = (bx - ray.origin.x) / ray.direction.x;
  if (tx < 0) {
    tx = (world_box->max_box.x - ray.origin.x) / ray.direction.x;
  }
  float ty = (by - ray.origin.y) / ray.direction.y;
  if (ty < 0) {
    ty = (world_box->max_box.y - ray.origin.y) / ray.direction.y;
  }
  float tz = (bz - ray.origin.z) / ray.direction.z;
  if (tz < 0) {
    tz = (world_box->max_box.z - ray.origin.z) / ray.direction.z;
  }
  float t = min3(tx, ty, tz);

  vec3 p = ray.origin + ray.direction * t;
  float xs = findNext(p.x, world_box->min_box.x, world_box->max_box.x, GRID_RESOLUTION);
  float ys = findNext(p.y, world_box->min_box.y, world_box->max_box.y, GRID_RESOLUTION);
  float zs = findNext(p.z, world_box->min_box.z, world_box->max_box.z, GRID_RESOLUTION);

  Intersection *total = new Intersection();
  while ((xs >= world_box->min_box.x && xs <= world_box->max_box.x) &&
          (ys >= world_box->min_box.y && ys <= world_box->max_box.y) &&
          (zs >= world_box->min_box.z && zs <= world_box->max_box.z)) {
    int x = int((xs - world_box->min_box.x) / GRID_RESOLUTION);
    int y = int((ys - world_box->min_box.y) / GRID_RESOLUTION);
    int z = int((zs - world_box->min_box.z) / GRID_RESOLUTION);

    vector<SceneNode *> *nodes = (*(*(*grid)[x])[y])[z];
    for (SceneNode *n : *nodes) {
      Intersection *i = n->intersect(ray, false, mat4(1));
      Intersection *temp = total;
      // default union
      total = temp->union_intersection(i);

      delete i;
      delete temp;
    }

    tx = (xs + GRID_RESOLUTION * signx - ray.origin.x) / ray.direction.x;
    if (tx < 0) tx = INFINITY;
    ty = (ys + GRID_RESOLUTION * signy - ray.origin.y) / ray.direction.y;
    if (ty < 0) ty = INFINITY;
    tz = (zs + GRID_RESOLUTION * signz - ray.origin.z) / ray.direction.z;
    if (tz < 0) tz = INFINITY;
    t = min3(tx, ty, tz);
    if (t == INFINITY) {
      break;
    }

    p = ray.origin + ray.direction * t;
    xs = findNext(p.x, world_box->min_box.x, world_box->max_box.x, GRID_RESOLUTION);
    ys = findNext(p.y, world_box->min_box.y, world_box->max_box.y, GRID_RESOLUTION);
    zs = findNext(p.z, world_box->min_box.z, world_box->max_box.z, GRID_RESOLUTION);
  }

  // for (std::vector<std::vector<std::vector<SceneNode *>* >* > *v1 : *grid) {
  //   for (std::vector<std::vector<SceneNode *>* > *v2 : *v1) {
  //     for (std::vector<SceneNode *> *v3 : *v2) {
  //       for (SceneNode *n : *v3) {
  //         Intersection *i = n->intersect(ray, false, mat4(1));
  //         Intersection *temp = total;
  //         // default union
  //         total = temp->union_intersection(i);

  //         delete i;
  //         delete temp;
  //       }
  //     }
  //   }
  // }

  return total;
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

glm::vec3 RayTrace::getRayColor(Ray &ray, glm::vec3 & background, int maxHit, Material *lastMat) {
  #ifdef GRID_OPT
  Intersection *intersection = intersect(ray);
  #endif
  #ifndef GRID_OPT
  Intersection *intersection = root->intersect(ray, false, mat4(1));
  #endif
  vec3 col;
  Intersection::Hit hit = intersection->getFirstHit(ray, false);
  delete intersection;
  if (hit.hit) {
    col = ambient;
    TransparentMaterial *tMat;
    if ((tMat = dynamic_cast<TransparentMaterial*>(hit.mat))) {
      if (maxHit < 10) {
        int new_maxHit = maxHit + 1;
        if (tMat->transmittance != 0) {
          float kr = 1.0f / tMat->refractIdx;
          vec3 origin = hit.pHit - hit.pNormal*0.01f;
          vec3 refract_direction = getRefractAngle(ray.direction, hit.pNormal, kr);
          Ray refractRay(origin, refract_direction);
          vec3 refractColor;
          // refractColor = getRayColor(refractRay, background, new_maxHit, nullptr);
          Intersection *internal_intersection = root->intersect(refractRay, false, mat4(1));
          Intersection::Hit internalHit = internal_intersection->getFirstHit(refractRay, false);
          delete internal_intersection;
          vec3 internal_origin = internalHit.pHit - internalHit.pNormal*0.01f;
          vec3 internal_refract_direction = getRefractAngle(refractRay.direction, internalHit.pNormal, tMat->refractIdx);
          Ray newRefractRay(internal_origin, internal_refract_direction);
          refractColor = getRayColor(newRefractRay, background, new_maxHit, nullptr);
          return refractColor;
        } else {
          vec3 reflect_origin = hit.pHit + hit.pNormal*0.01f;
          vec3 reflect_direction = getReflectionAngle(ray.direction, hit.pNormal);
          Ray reflectRay(reflect_origin, reflect_direction);
          vec3 reflectColor;
          reflectColor = getRayColor(reflectRay, background, new_maxHit, nullptr);
          return reflectColor;
        }
      } else {
        cout << "dfd" << endl;
        Ray forwardRay(hit.pHit + ray.direction*0.01f, ray.direction);
        col = getRayColor(forwardRay, background, maxHit, tMat);
      }
    } else {
      for(Light *light : lights) {
        col += shadow.getColor(hit, light, root);
      }
      col = col / float(lights.size());
    }
  } else {
    col = getBackgroundColor(ray);
  }
  return col;
}

double RayTrace::getCameraRadius() {
  return camera_radius;
}

double RayTrace::getCameraSampleRate() {
  return camera_sample_rate;
}

double RayTrace::getFocalDist() {
  return focal_dist;
}

double RayTrace::getAntiSampleRadius() {
  return anti_sample_radius;
}

double RayTrace::getAntiSampleRate() {
  return anti_sample_rate;
}

glm::vec3 RayTrace::getRefractAngle(glm::vec3 direction, glm::vec3 normal, float kr) {
  // assert(abs(glm::dot(normal, normal) - 1.0f) < 0.00001);
  // assert(abs(glm::dot(direction, direction) - 1.0f) < 0.00001);
  float d_dot_n = glm::dot((direction), (normal));
  float root = 1.0f - kr*kr*(1.0f - d_dot_n*d_dot_n);
  if (root < 0) {
    return getReflectionAngle(direction, normal);
    // assert(false);
  }
  vec3 res = (-kr * d_dot_n - sqrt(root)) * (normal) + kr * (direction);
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