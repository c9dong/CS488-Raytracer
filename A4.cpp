#include <glm/ext.hpp>

#include "A4.hpp"
#include "RayTrace.hpp"
#include "Shadow.hpp"

using namespace std;
using namespace glm;

#define TEST

#ifdef TEST
#include "Intersection.hpp"

void testUnion() {
	cout << "Test Union" << endl;

	Intersection::Point p1(1, 0, vec3(0), nullptr, mat4(1));
	Intersection::Point p2(5, 1, vec3(0), nullptr, mat4(1));
	Intersection::Point p3(7, 0, vec3(0), nullptr, mat4(1));
	Intersection::Point p4(11, 1, vec3(0), nullptr, mat4(1));
	Intersection::Point p5(5, 0, vec3(0), nullptr, mat4(1));
	Intersection::Point p6(7, 1, vec3(0), nullptr, mat4(1));
	Intersection::Point p7(15, 0, vec3(0), nullptr, mat4(1));
	Intersection::Point p8(17, 1, vec3(0), nullptr, mat4(1));

	Intersection::Range *r1 = new Intersection::Range(p1, p2);
	Intersection::Range *r2 = new Intersection::Range(p3, p4);
	Intersection::Range *r3 = new Intersection::Range(p5, p6);
	Intersection::Range *r4 = new Intersection::Range(p7, p8);

	Intersection *i1 = new Intersection();
	Intersection *i2 = new Intersection();
	i1->addRange(r1);
	i1->addRange(r2);

	i2->addRange(r3);
	i2->addRange(r4);

	Intersection *i3 = i1->union_intersection(i2);
	assert(i3->ranges.size() == 2);
	assert(i3->ranges[0]->start == 1);
	assert(i3->ranges[0]->end == 11);
	assert(i3->ranges[1]->start == 15);
	assert(i3->ranges[1]->end == 17);

	cout << "Test Union success" << endl;

	delete i1;
	delete i2;
	delete i3;
}

void testIntersection() {
	cout << "Test Intersection" << endl;

	Intersection::Point p1(1, 0, vec3(0), nullptr, mat4(1));
	Intersection::Point p2(5, 1, vec3(0), nullptr, mat4(1));
	Intersection::Point p3(7, 0, vec3(0), nullptr, mat4(1));
	Intersection::Point p4(11, 1, vec3(0), nullptr, mat4(1));
	Intersection::Point p5(5, 0, vec3(0), nullptr, mat4(1));
	Intersection::Point p6(7, 1, vec3(0), nullptr, mat4(1));
	Intersection::Point p7(15, 0, vec3(0), nullptr, mat4(1));
	Intersection::Point p8(17, 1, vec3(0), nullptr, mat4(1));

	Intersection::Range *r1 = new Intersection::Range(p1, p2);
	Intersection::Range *r2 = new Intersection::Range(p3, p4);
	Intersection::Range *r3 = new Intersection::Range(p5, p6);
	Intersection::Range *r4 = new Intersection::Range(p7, p8);

	Intersection *i1 = new Intersection();
	Intersection *i2 = new Intersection();
	i1->addRange(r1);
	i1->addRange(r2);

	i2->addRange(r3);
	i2->addRange(r4);

	Intersection *i3 = i1->intersect_intersection(i2);
	assert(i3->ranges.size() == 2);
	assert(i3->ranges[0]->start == 5);
	assert(i3->ranges[0]->end == 5);
	assert(i3->ranges[1]->start == 7);
	assert(i3->ranges[1]->end == 7);

	cout << "Test Intersection success" << endl;
	delete i1;
	delete i2;
	delete i3;
}

void testDifference() {
	cout << "Test Difference" << endl;

	Intersection::Point p1(1, 0, vec3(0), nullptr, mat4(1));
	Intersection::Point p2(5, 1, vec3(0), nullptr, mat4(1));
	Intersection::Point p3(7, 0, vec3(0), nullptr, mat4(1));
	Intersection::Point p4(11, 1, vec3(0), nullptr, mat4(1));
	Intersection::Point p5(3, 0, vec3(0), nullptr, mat4(1));
	Intersection::Point p6(9, 1, vec3(0), nullptr, mat4(1));
	Intersection::Point p7(15, 0, vec3(0), nullptr, mat4(1));
	Intersection::Point p8(17, 1, vec3(0), nullptr, mat4(1));

	Intersection::Range *r1 = new Intersection::Range(p1, p2);
	Intersection::Range *r2 = new Intersection::Range(p3, p4);
	Intersection::Range *r3 = new Intersection::Range(p5, p6);
	Intersection::Range *r4 = new Intersection::Range(p7, p8);

	Intersection *i1 = new Intersection();
	Intersection *i2 = new Intersection();
	i1->addRange(r1);
	i1->addRange(r2);

	i2->addRange(r3);
	i2->addRange(r4);

	Intersection *i3 = i1->difference_intersection(i2);
	assert(i3->ranges.size() == 2);
	assert(i3->ranges[0]->start == 1);
	assert(i3->ranges[0]->end == 3);
	assert(i3->ranges[1]->start == 9);
	assert(i3->ranges[1]->end == 11);

	cout << "Test Difference success" << endl;
	delete i1;
	delete i2;
	delete i3;
}

#endif

void A4_Render(
		// What to render
		SceneNode * root,

		// Image to write to, set to a given width and height
		Image & image,

		// Viewing parameters
		const glm::vec3 & eye,
		const glm::vec3 & view,
		const glm::vec3 & up,
		double fovy,

		// Lighting parameters
		const glm::vec3 & ambient,
		const std::list<Light *> & lights
) {

	#ifdef TEST
	testUnion();
	testIntersection();
	testDifference();
	#endif

  // Fill in raytracing code here...

  std::cout << "Calling A4_Render(\n" <<
		  "\t" << *root <<
          "\t" << "Image(width:" << image.width() << ", height:" << image.height() << ")\n"
          "\t" << "eye:  " << glm::to_string(eye) << std::endl <<
		  "\t" << "view: " << glm::to_string(view) << std::endl <<
		  "\t" << "up:   " << glm::to_string(up) << std::endl <<
		  "\t" << "fovy: " << fovy << std::endl <<
          "\t" << "ambient: " << glm::to_string(ambient) << std::endl <<
		  "\t" << "lights{" << std::endl;

	for(const Light * light : lights) {
		std::cout << "\t\t" <<  *light << std::endl;
	}
	std::cout << "\t}" << std::endl;
	std:: cout <<")" << std::endl;
	
	vec3 shadow_color = vec3(0.0f);
	Shadow shadow(shadow_color);
	RayTrace raytrace(root, image, eye, view, up, fovy, ambient, lights, shadow);
	raytrace.generateImage();
}
