#include "DifferenceNode.hpp"

using namespace std;
using namespace glm;

DifferenceNode::DifferenceNode(
  const std::string & name )
  : SceneNode( name )
{
  m_nodeType = NodeType::DifferenceNode;
}

Intersection DifferenceNode::intersect(Ray & ray, bool checkBound) {
  vec3 inv_origin = vec3(invtrans * vec4(ray.origin, 1));
  vec3 inv_direction = glm::normalize(vec3(invtrans * vec4(ray.direction, 0)));
  Ray inverseRay(inv_origin, inv_direction);
  
  assert (children.size() == 2);
  Intersection left;
  Intersection right;
  int i = 0;
  for (SceneNode *child : children) {
    if (i == 0) {
      left = child->intersect(inverseRay, checkBound);
    } else {
      right = child->intersect(inverseRay, checkBound);
    }
    i ++;
  }

  Intersection intersection = left.difference_intersection(right);
  intersection.transformRanges(ray, inverseRay, trans, invtrans);
  
  return intersection;
}