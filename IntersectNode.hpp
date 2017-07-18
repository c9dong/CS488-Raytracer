#pragma once

#include "SceneNode.hpp"
#include "Primitive.hpp"
#include "Material.hpp"

class IntersectNode : public SceneNode {
public:
  IntersectNode( const std::string & name );

  Intersection* intersect(Ray & ray, bool checkBound, glm::mat4 lastInv);
  Primitive::BoundingBox* getBoundingBox();
  std::vector<SceneNode *>* intersectBox(Primitive::BoundingBox *box);
};
