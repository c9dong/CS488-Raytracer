#include <iostream>
#include <fstream>

#include <glm/ext.hpp>

// #include "cs488-framework/ObjFileDecoder.hpp"
#include "Mesh.hpp"
#include "cs488-framework/MathUtils.hpp"

using namespace glm;
using namespace std;

Mesh::Mesh( const std::string& fname )
  : m_vertices()
  , m_faces()
{
  std::string code;
  double vx, vy, vz;
  size_t s1, s2, s3;

  vec3 minBound = vec3(INFINITY, INFINITY, INFINITY);
  vec3 maxBound = vec3(-INFINITY, -INFINITY, -INFINITY);

  std::ifstream ifs( fname.c_str() );
  while( ifs >> code ) {
    if( code == "v" ) {
      ifs >> vx >> vy >> vz;
      m_vertices.push_back( glm::vec3( vx, vy, vz ) );
      if (vx < minBound.x) {
        minBound.x = vx;
      }
      if (vy < minBound.y) {
        minBound.y = vy;
      }
      if (vz < minBound.z) {
        minBound.z = vz;
      }
      if (vx > maxBound.x) {
        maxBound.x = vx;
      }
      if (vy > maxBound.y) {
        maxBound.y = vy;
      }
      if (vz > maxBound.z) {
        maxBound.z = vz;
      }
    } else if( code == "f" ) {
      ifs >> s1 >> s2 >> s3;
      m_faces.push_back( Triangle( s1 - 1, s2 - 1, s3 - 1 ) );
    }
  }

  bound_pos = minBound;
  bound_size = glm::max(maxBound.x-minBound.x, maxBound.y-minBound.y, maxBound.z-minBound.z);
}

Intersection Mesh::intersect(Ray &ray, bool checkBound) {
  vec4 h, n;
  NonhierBox b(bound_pos, bound_size);
  Intersection boundIntersect = b.intersect(ray, checkBound);
  if (!boundIntersect.hit) {
    return boundIntersect;
  }

  float minTriDist = INFINITY;
  Intersection minIntersection;
  for (int i=0; i<m_faces.size(); i++) {
    Triangle face = m_faces[i];
    vec3 p0 = m_vertices[face.v1];
    vec3 p1 = m_vertices[face.v2];
    vec3 p2 = m_vertices[face.v3];

    vec3 r = ray.origin - p0;
    vec3 c1 = p1 - p0;
    vec3 c2 = p2 - p0;
    vec3 c3 = ray.direction * -1;

    float D = glm::determinant(mat3(
      c1.x, c1.y, c1.z,
      c2.x, c2.y, c2.z,
      c3.x, c3.y, c3.z
      ));

    float D1 = glm::determinant(mat3(
      r.x, r.y, r.z,
      c2.x, c2.y, c2.z,
      c3.x, c3.y, c3.z
      ));

    float D2 = glm::determinant(mat3(
      c1.x, c1.y, c1.z,
      r.x, r.y, r.z,
      c3.x, c3.y, c3.z
      ));

    float D3 = glm::determinant(mat3(
      c1.x, c1.y, c1.z,
      c2.x, c2.y, c2.z,
      r.x, r.y, r.z
      ));

    float beta = D1 / D;
    float alpha = D2 / D;
    float t = D3 / D;

    if (alpha < 0 || beta < 0) {
      continue;
    }
    if (alpha + beta > 1) {
      continue;
    }

    if (t < 0) {
      continue;
    }

    // if (checkBetween) {
    //   if (!(t >=0 && t <= 1)) {
    //     continue;
    //   }
    // }

    vec3 hit = ray.origin + t * ray.direction;
    float dist = glm::distance(ray.origin, hit);
    if (dist < minIntersection.pDist) {
      minIntersection.pHit = hit;
      minIntersection.pNormal = glm::normalize(glm::cross(p1-p0, p2-p0));
      minIntersection.pDist = dist;
      minIntersection.hit = true;
    }
  }

  return minIntersection;
}

std::ostream& operator<<(std::ostream& out, const Mesh& mesh)
{
  out << "mesh {";
  /*
  
  for( size_t idx = 0; idx < mesh.m_verts.size(); ++idx ) {
    const MeshVertex& v = mesh.m_verts[idx];
    out << glm::to_string( v.m_position );
  if( mesh.m_have_norm ) {
      out << " / " << glm::to_string( v.m_normal );
  }
  if( mesh.m_have_uv ) {
      out << " / " << glm::to_string( v.m_uv );
  }
  }

*/
  out << "}";
  return out;
}
