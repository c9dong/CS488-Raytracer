#pragma once

#include <glm/glm.hpp>

#include "SceneNode.hpp"
#include "Light.hpp"
#include "Image.hpp"

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
		const std::list<Light *> & lights,
		double focal_dist,
		double camera_radius,
		double camera_sample_rate,
		double anti_sample_radius,
    double anti_sample_rate,
    int x_start,
    int y_start,
    int x_size,
    int y_size,
    int id
);
