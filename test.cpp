//******************
//
// scls_image.h
//
//******************
// Presentation :
//
// SCLS is a project containing base functions for C++.
// It can also be use in any projects.
//
// The Image "Michelangelo" part represents a lot of usefull datas manipulation for images.
// It is named after one of the greatest painter of all time, Michelangelo.
//
// This file is a test file for the library.
//

#define SCLS_FOUNDATION_PATH "../scls-foundation-leonhard/scls_foundation.h"
#define SCLS_MATH_PATH "../scls-math-carl/scls_math.h"
#include "scls_image.h"
#include "../scls-math-carl/scls_math_directory/scls_math_2d_physic.h"
#include "../scls-math-carl/scls_math_directory/scls_math_boolean.h"
#include "../scls-math-carl/scls_math_directory/scls_math_permutation.h"
#include "../scls-math-carl/scls_math_directory/scls_math_string.h"
#include "scls_image_directory/scls_image_core.h"
#include "scls_image_directory/scls_image_table.h"
#include "scls_image_directory/scls_image_statistic.h"

SCLS_INIT;

void draw_grid(scls::Image img, scls::Plane_Base* base) {
	double min_x = base->canonical_x_to_base_x(0);
	double max_x = base->canonical_x_to_base_x(img.width());
	double min_y = base->canonical_y_to_base_y(img.height());
	double max_y = base->canonical_y_to_base_y(0);

	// Calculate the start of the grid
	double step_x = 1;double step_y = 1;
	double x_start = std::ceil(min_x * (1.0/step_x)) * step_x;
	double y_start = std::ceil(min_y * (1.0/step_y)) * step_y;

	// Trace the X lines
	double current_x = x_start;
	while(current_x <= max_x) {
		img.draw_line(base->base_x_to_canonical_x(current_x), 0, base->base_x_to_canonical_x(current_x), img.height(), scls::Color(0, 0, 0), 1);
		current_x += step_x;
	}

	// Trace the Y lines
	double current_y = y_start;
	while(current_y <= max_y) {
		img.draw_line(0, base->base_y_to_canonical_y(current_y), img.width(), base->base_y_to_canonical_y(current_y), scls::Color(0, 0, 0), 1);
		current_y += step_y;
	}

	// Central grid
	double needed_y = base->base_y_to_canonical_y(0);
	img.draw_line(0, needed_y, img.width(), needed_y, scls::Color(0, 0, 0), 3);
	double needed_x = base->base_x_to_canonical_x(0);
	img.draw_line(needed_x, 0, needed_x, img.height(), scls::Color(0, 0, 0), 3);
}

int main() {
	std::vector<scls::Point_2D> wing = {scls::Point_2D(-0.25, -0.25), scls::Point_2D(0.25, -0.25), scls::Point_2D(0.25, 0.25)};
	scls::Physic_Engine engine = scls::Physic_Engine();

	// Generates wing
	std::shared_ptr<scls::Transform_Object_2D> wing_transform = std::make_shared<scls::Transform_Object_2D>();
	wing_transform.get()->set_position(scls::Point_2D(0, 0));
	wing_transform.get()->set_scale(scls::Point_2D(1, 1));
	std::shared_ptr<scls::Physic_Object> wing_physic = engine.new_physic_object(wing_transform);
	wing_physic.get()->set_static(true);wing_physic.get()->set_use_gravity(0);wing_physic.get()->add_collisions(wing);

	// Particules
	std::vector<std::shared_ptr<scls::Physic_Object>> particles = std::vector<std::shared_ptr<scls::Physic_Object>>();
	std::vector<std::shared_ptr<scls::Transform_Object_2D>> transforms = std::vector<std::shared_ptr<scls::Transform_Object_2D>>();
	for(int i = 0;i<30;i++) {
		for(int j = 0;j<30;j++) {
			std::shared_ptr<scls::Transform_Object_2D> particle_transform = std::make_shared<scls::Transform_Object_2D>();
			particle_transform.get()->set_position(scls::Point_2D(-1 - static_cast<double>(i) * 0.2  + scls::random_fraction(scls::Fraction(0), scls::Fraction(1, 5), 3).to_double(), -3 + static_cast<double>(j) * 0.2 + scls::random_fraction(scls::Fraction(0), scls::Fraction(1, 5), 3).to_double()));
			particle_transform.get()->set_scale(scls::Point_2D(0.02, 0.02));
			std::shared_ptr<scls::Physic_Object> particle_physic = engine.new_physic_object(particle_transform);
			particle_physic.get()->new_collision(scls::Collision_Type::GCT_Circle);
			particle_physic.get()->set_static(false);particle_physic.get()->set_use_gravity(0);
			particle_physic.get()->set_velocity(scls::Point_2D(1, 0));
			particles.push_back(particle_physic);transforms.push_back(particle_transform);
		}
	}

	// Simulation
	double delta_time = 0.01;
	double time = 5.0;
	for(int i = 0;i<time/delta_time;i++) {
		engine.update_physic(delta_time);
	}

	// Creates the image
	scls::Plane_Base base = scls::Plane_Base(200, 200, 500, 500);
	scls::Image image = scls::Image(1000, 1000);
	image.fill_form(canonical_points_to_base_points(&base, wing), scls::Color(255, 0, 0));
	for(std::size_t i = 0;i<particles.size();i++) {
		image.fill_circle(base.base_x_to_canonical_x(particles.at(i).get()->attached_transform()->absolute_x()), base.base_y_to_canonical_y(-particles.at(i).get()->attached_transform()->absolute_y()), base.base_scale_x_to_canonical_scale_x(particles.at(i).get()->attached_transform()->absolute_scale_x()), base.base_scale_y_to_canonical_scale_y(particles.at(i).get()->attached_transform()->absolute_scale_y()), scls::Color(0, 0, 255));
	}
	image.save_png(std::string("tests/avion.png"));

    return 0;
}
