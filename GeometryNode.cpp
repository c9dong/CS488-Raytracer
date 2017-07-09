#include "GeometryNode.hpp"
#include "Intersection.hpp"

using namespace glm;
using namespace std;

//---------------------------------------------------------------------------------------
GeometryNode::GeometryNode(
	const std::string & name, Primitive *prim, Material *mat )
	: SceneNode( name )
	, m_material( mat )
	, m_primitive( prim )
{
	m_nodeType = NodeType::GeometryNode;
}

void GeometryNode::setMaterial( Material *mat )
{
	// Obviously, there's a potential memory leak here.  A good solution
	// would be to use some kind of reference counting, as in the 
	// C++ shared_ptr.  But I'm going to punt on that problem here.
	// Why?  Two reasons:
	// (a) In practice we expect the scene to be constructed exactly
	//     once.  There's no reason to believe that materials will be
	//     repeatedly overwritten in a GeometryNode.
	// (b) A ray tracer is a program in which you compute once, and 
	//     throw away all your data.  A memory leak won't build up and
	//     crash the program.

	m_material = mat;
}

Intersection* GeometryNode::intersect(Ray &ray, bool checkBound) {
	vec3 inv_origin = vec3(invtrans * vec4(ray.origin, 1));
	vec3 inv_direction = glm::normalize(vec3(invtrans * vec4(ray.direction, 0)));
	Ray inverseRay(inv_origin, inv_direction);

	Intersection::Range *range = m_primitive->intersect(inverseRay, checkBound);

	Intersection *i = new Intersection();
	if (range->hit) {
		range->s_mat = m_material;
		range->e_mat = m_material;
		range->s_inv = invtrans;
		range->e_inv = invtrans;

		i->addRange(range);
	}

	i->transformRanges(ray, inverseRay, trans, invtrans);

	return i;
}
