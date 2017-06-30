#pragma once

#include "SceneNode.hpp"
#include "Primitive.hpp"
#include "Material.hpp"

class DifferenceNode : public SceneNode {
public:
  DifferenceNode( const std::string & name );

  Intersection intersect(Ray & ray, bool checkBound);
};
