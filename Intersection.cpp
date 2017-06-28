#include "Intersection.hpp"
#include "cs488-framework/MathUtils.hpp"

using namespace glm;
using namespace std;

Intersection::Intersection() 
  : pHit(vec3(INFINITY)), pNormal(vec3(INFINITY)), pDist(INFINITY), hit(false) {}
Intersection::Intersection(Material *mat, 
    glm::vec3 pHit, 
    glm::vec3 pNormal, 
    float pDist, 
    bool hit)
  : mat(mat), pHit(pHit), pNormal(pNormal), pDist(pDist), hit(hit)
{

}

Intersection::~Intersection() {}