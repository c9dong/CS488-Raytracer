#include "SceneNode.hpp"

#include "cs488-framework/MathUtils.hpp"
#include "Util.hpp"
#include "PhongMaterial.hpp"

#include <iostream>
#include <sstream>
using namespace std;

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/transform.hpp>

using namespace glm;


// Static class variable
unsigned int SceneNode::nodeInstanceCount = 0;


//---------------------------------------------------------------------------------------
SceneNode::SceneNode(const std::string& name)
  : m_name(name),
	m_nodeType(NodeType::SceneNode),
	trans(mat4()),
	invtrans(mat4()),
	m_nodeId(nodeInstanceCount++)
{

}

//---------------------------------------------------------------------------------------
// Deep copy
SceneNode::SceneNode(const SceneNode & other)
	: m_nodeType(other.m_nodeType),
	  m_name(other.m_name),
	  trans(other.trans),
	  invtrans(other.invtrans)
{
	for(SceneNode * child : other.children) {
		this->children.push_front(new SceneNode(*child));
	}
}

//---------------------------------------------------------------------------------------
SceneNode::~SceneNode() {
	for(SceneNode * child : children) {
		delete child;
	}
}

//---------------------------------------------------------------------------------------
void SceneNode::set_transform(const glm::mat4& m) {
	trans = m;
	invtrans = glm::inverse(m);
}

//---------------------------------------------------------------------------------------
const glm::mat4& SceneNode::get_transform() const {
	return trans;
}

//---------------------------------------------------------------------------------------
const glm::mat4& SceneNode::get_inverse() const {
	return invtrans;
}

//---------------------------------------------------------------------------------------
void SceneNode::add_child(SceneNode* child) {
	children.push_back(child);
}

//---------------------------------------------------------------------------------------
void SceneNode::remove_child(SceneNode* child) {
	children.remove(child);
}

//---------------------------------------------------------------------------------------
void SceneNode::rotate(char axis, float angle) {
	vec3 rot_axis;

	switch (axis) {
		case 'x':
			rot_axis = vec3(1,0,0);
			break;
		case 'y':
			rot_axis = vec3(0,1,0);
	        break;
		case 'z':
			rot_axis = vec3(0,0,1);
	        break;
		default:
			break;
	}
	mat4 rot_matrix = glm::rotate(degreesToRadians(angle), rot_axis);
	set_transform( rot_matrix * trans );
}

//---------------------------------------------------------------------------------------
void SceneNode::scale(const glm::vec3 & amount) {
	set_transform( glm::scale(amount) * trans );
}

//---------------------------------------------------------------------------------------
void SceneNode::translate(const glm::vec3& amount) {
	set_transform( glm::translate(amount) * trans );
}


//---------------------------------------------------------------------------------------
int SceneNode::totalSceneNodes() const {
	return nodeInstanceCount;
}

Intersection* SceneNode::intersect(Ray &ray, bool checkBound, glm::mat4 lastInv) {
	Intersection *minIntersection = new Intersection();
	vec3 inv_origin = vec3(invtrans * vec4(ray.origin, 1));
	vec3 inv_direction = glm::normalize(vec3(invtrans * vec4(ray.direction, 0)));
	Ray inverseRay(inv_origin, inv_direction);
	
	for (SceneNode *child : children) {
		Intersection *i = child->intersect(inverseRay, checkBound, invtrans * lastInv);
		Intersection *temp = minIntersection;
		// default union
		minIntersection = temp->union_intersection(i);

		delete i;
		delete temp;
	}

	minIntersection->transformRanges(ray, inverseRay, trans, invtrans);

	return minIntersection;
}

Primitive::BoundingBox* SceneNode::getBoundingBox() {
	Primitive::BoundingBox *box = new Primitive::BoundingBox();
	for (SceneNode *child : children) {
		Primitive::BoundingBox *other = child->getBoundingBox();
		box->min_box.x = glm::min(box->min_box.x, other->min_box.x);
		box->min_box.y = glm::min(box->min_box.y, other->min_box.y);
		box->min_box.z = glm::min(box->min_box.z, other->min_box.z);
		box->max_box.x = glm::max(box->max_box.x, other->max_box.x);
		box->max_box.y = glm::max(box->max_box.y, other->max_box.y);
		box->max_box.z = glm::max(box->max_box.z, other->max_box.z);

		delete other;
	}
	return box;
}

std::vector<SceneNode *>* SceneNode::intersectBox(Primitive::BoundingBox *box) {
	vector<SceneNode *>* nodes = new vector<SceneNode *>();
	for (SceneNode *child : children) {
		vector<SceneNode *>* other = child->intersectBox(box);
		for (SceneNode *n : *other) {
			nodes->push_back(n);
		}
	}
	return nodes;
}

//---------------------------------------------------------------------------------------
std::ostream & operator << (std::ostream & os, const SceneNode & node) {

	//os << "SceneNode:[NodeType: ___, name: ____, id: ____, isSelected: ____, transform: ____"
	switch (node.m_nodeType) {
		case NodeType::SceneNode:
			os << "SceneNode";
			break;
		case NodeType::GeometryNode:
			os << "GeometryNode";
			break;
		case NodeType::JointNode:
			os << "JointNode";
			break;
		case NodeType::UnionNode:
			os << "UnionNode";
			break;
		case NodeType::IntersectNode:
			os << "IntersectNode";
			break;
		case NodeType::DifferenceNode:
			os << "DifferenceNode";
			break;
	}
	os << ":[";

	os << "name:" << node.m_name << ", ";
	os << "id:" << node.m_nodeId;

	os << "]\n";
	return os;
}
