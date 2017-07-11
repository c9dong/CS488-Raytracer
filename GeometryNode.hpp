#pragma once

#include "SceneNode.hpp"
#include "Primitive.hpp"
#include "Material.hpp"

class GeometryNode : public SceneNode {
public:
	GeometryNode( const std::string & name, Primitive *prim, 
		Material *mat = nullptr );

	void setMaterial( Material *material );

  Intersection* intersect(Ray & ray, bool checkBound, glm::mat4 lastInv);

	Material *m_material;
	Primitive *m_primitive;
};
