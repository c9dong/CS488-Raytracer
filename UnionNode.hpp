#pragma once

#include "SceneNode.hpp"
#include "Primitive.hpp"
#include "Material.hpp"

class UnionNode : public SceneNode {
public:
  UnionNode( const std::string & name );

  Intersection* intersect(Ray & ray, bool checkBound, glm::mat4 lastInv);
};
