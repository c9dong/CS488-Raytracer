#pragma once

#include "SceneNode.hpp"
#include "Primitive.hpp"
#include "Material.hpp"

class IntersectNode : public SceneNode {
public:
  IntersectNode( const std::string & name );

  Intersection* intersect(Ray & ray, bool checkBound);
};
