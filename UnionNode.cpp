#include "UnionNode.hpp"

using namespace std;
using namespace glm;

UnionNode::UnionNode(
  const std::string & name )
  : SceneNode( name )
{
  m_nodeType = NodeType::UnionNode;
}

Intersection* UnionNode::intersect(Ray & ray, bool checkBound, glm::mat4 lastInv) {
  vec3 inv_origin = vec3(invtrans * vec4(ray.origin, 1));
  vec3 inv_direction = glm::normalize(vec3(invtrans * vec4(ray.direction, 0)));
  Ray inverseRay(inv_origin, inv_direction);
  
  Intersection *total = new Intersection();
  for (SceneNode *child : children) {
    Intersection *i = child->intersect(inverseRay, checkBound, invtrans * lastInv);
    Intersection *temp = total;
    // default union
    total = temp->union_intersection(i);

    delete i;
    delete temp;
  }

  total->transformRanges(ray, inverseRay, trans, invtrans);

  return total;
}