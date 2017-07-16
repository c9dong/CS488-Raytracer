#include "DifferenceNode.hpp"

using namespace std;
using namespace glm;

DifferenceNode::DifferenceNode(
  const std::string & name )
  : SceneNode( name )
{
  m_nodeType = NodeType::DifferenceNode;
}

Intersection* DifferenceNode::intersect(Ray & ray, bool checkBound, glm::mat4 lastInv) {
  vec3 inv_origin = vec3(invtrans * vec4(ray.origin, 1));
  vec3 inv_direction = glm::normalize(vec3(invtrans * vec4(ray.direction, 0)));
  Ray inverseRay(inv_origin, inv_direction);

  Intersection *total;
  int i = 0;
  for (SceneNode *child : children) {
    if (i == 0) {
      total = child->intersect(inverseRay, checkBound, invtrans * lastInv);
      i++;
    } else {
      Intersection *i = child->intersect(inverseRay, checkBound, invtrans * lastInv);
      Intersection *temp = total;
      // default union
      total = temp->difference_intersection(i);

      delete i;
      delete temp;
    }
  }

  total->transformRanges(ray, inverseRay, trans, invtrans);

  return total;
}