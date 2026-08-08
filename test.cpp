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
#include "scls_image_directory/scls_image_turtle.h"

SCLS_INIT;

void scalar_field_2d_colored(scls::Image image, scls::Formula_Base* formula, scls::Plane_Base* base) {
	// Datas

	// Browse the image
	for(int y_pixel = 0;y_pixel<image.height();y_pixel++) {
		for(int x_pixel = 0;x_pixel<image.width();x_pixel++) {
			// Datas for this pixel
			scls::Fraction x = base->canonical_x_to_base_x(x_pixel);
			scls::Fraction y = base->canonical_y_to_base_y(image.height() - y_pixel);

			scls::Fraction current_value = *formula->replace_unknowns("x", x).get()->replace_unknowns("y", y).get()->value<scls::Fraction>();
			if(current_value > 0 ){image.set_pixel(x_pixel, y_pixel, scls::Color(100.0 * current_value.to_double(), 0, 0));}
			else{image.set_pixel(x_pixel, y_pixel, scls::Color(0, 0, 100.0 * -current_value.to_double()));}
		}
	}
}

void vector_field_2d(scls::Image image, scls::Formula_Base* formula_x, scls::Formula_Base* formula_y, scls::Plane_Base* base) {
	// Datas
	int start_x_pixel = 40;
	int start_y_pixel = 40;
	int step_x_pixel = 80;
	int step_y_pixel = 80;

	// Browse the image
	int current_x_pixel = start_x_pixel;
	int current_y_pixel = start_y_pixel;
	while(current_y_pixel < image.height()) {
		while(current_x_pixel < image.width()) {
			double current_x = base->canonical_x_to_base_x(current_x_pixel);
			double current_y = base->canonical_y_to_base_y(current_y_pixel);

			// Get the needed vector
			scls::Fraction current_value_x = *formula_x->replace_unknowns("x", scls::Fraction::from_double(current_x)).get()->replace_unknowns("y", scls::Fraction::from_double(current_y)).get()->value<scls::Fraction>();
			scls::Fraction current_value_y = *formula_y->replace_unknowns("x", scls::Fraction::from_double(current_x)).get()->replace_unknowns("y", scls::Fraction::from_double(current_y)).get()->value<scls::Fraction>();
			int current_value_x_pixel = current_value_x.to_double() * 20.0;
			int current_value_y_pixel = current_value_y.to_double() * 20.0;

			image.image()->draw_arrow(current_x_pixel, image.height() - current_y_pixel, current_x_pixel + current_value_x_pixel, image.height() - (current_y_pixel + current_value_y_pixel), scls::Color(0, 0, 0), 0.3, 5);

			current_x_pixel += step_x_pixel;
		}
		current_x_pixel = start_x_pixel;
		current_y_pixel += step_y_pixel;
	}
}
void vector_field_2d_link(scls::Image image, scls::Formula_Base* formula_x, scls::Formula_Base* formula_y, scls::Plane_Base* base) {
	// Datas
	int start_x_pixel = 80;
	int start_y_pixel = 80;
	int step_x_pixel = 160;
	int step_y_pixel = 160;

	// Browse the image
	int current_x_pixel = start_x_pixel;
	int current_y_pixel = start_y_pixel;
	while(current_y_pixel < image.height()) {
		while(current_x_pixel < image.width()) {
			double current_x = base->canonical_x_to_base_x(current_x_pixel);
			double current_y = base->canonical_y_to_base_y(current_y_pixel);

			// Get the needed vector
			scls::Fraction current_value_x = *formula_x->replace_unknowns("x", scls::Fraction::from_double(current_x)).get()->replace_unknowns("y", scls::Fraction::from_double(current_y)).get()->value<scls::Fraction>();
			scls::Fraction current_value_y = *formula_y->replace_unknowns("x", scls::Fraction::from_double(current_x)).get()->replace_unknowns("y", scls::Fraction::from_double(current_y)).get()->value<scls::Fraction>();
			int current_value_x_pixel = base->base_x_to_canonical_x(current_value_x.to_double());
			int current_value_y_pixel = base->base_y_to_canonical_y(current_value_x.to_double());

			image.image()->draw_arrow(current_x_pixel, image.height() - current_y_pixel, current_value_x_pixel, image.height() - (current_value_y_pixel), scls::Color(0, 0, 0), 0.3, 5);
			//image.draw_line(current_x_pixel, current_y_pixel, current_value_x_pixel, current_value_y_pixel, scls::Color(0, 0, 0), 5);

			current_x_pixel += step_x_pixel;
		}
		current_x_pixel = start_x_pixel;
		current_y_pixel += step_y_pixel;
	}
}

void apply_force_from_field_2d(scls::Physic_Object* object, scls::Formula_Base* formula_x, scls::Formula_Base* formula_y, double delta_time) {
	double current_x = object->attached_transform()->x();
	double current_y = object->attached_transform()->y();

	// Get the needed vector
	scls::Fraction current_value_x = *formula_x->replace_unknowns("x", scls::Fraction::from_double(current_x)).get()->replace_unknowns("y", scls::Fraction::from_double(current_y)).get()->value<scls::Fraction>();
	scls::Fraction current_value_y = *formula_y->replace_unknowns("x", scls::Fraction::from_double(current_x)).get()->replace_unknowns("y", scls::Fraction::from_double(current_y)).get()->value<scls::Fraction>();
	object->apply_force(scls::Point_2D(current_value_x.to_double(), current_value_y.to_double()) * delta_time);
}
void van_der_pol_oscillator(scls::Physic_Object* object, scls::Formula_Base* formula_x, scls::Formula_Base* formula_y, double delta_time) {
	double current_x = object->attached_transform()->x();
	double current_y = object->attached_transform()->y();
	double current_x_velocity = object->attached_transform()->velocity_x();
	double current_y_velocity = object->attached_transform()->velocity_y();

	// Get the needed vector
	double w0 = 1;
	double current_value_x = w0 * (1 - current_x * current_x) * current_x_velocity - w0 * w0 * current_x;
	double current_value_y = w0 * (1 - current_y * current_y) * current_y_velocity - w0 * w0 * current_y;
	object->apply_force(scls::Point_2D(current_value_x, current_value_y) * delta_time);
}

int main() {
    scls::Image img = scls::Image(2000, 2000);scls::Plane_Base b = scls::Plane_Base::base_for_image(2000, 2000, 100, 100);
    vector_field_2d(img, scls::string_to_algebra_element<scls::Formula_Base>("x").get(), scls::string_to_algebra_element<scls::Formula_Base>("y").get(), &b);
    //img.draw_line(500, 100, 501, 900, scls::Color(255, 0, 0), 100);
    //img.draw_line(100, 100, 900, 900, scls::Color(255, 0, 0), 100);
    //img.draw_line(100, 100, 900, 100, scls::Color(255, 0, 0), 100);
    //img.draw_line(100, 100, 100, 900, scls::Color(255, 0, 0), 100);
    //img.fill_circle(900, 100, 50, scls::Color(255, 0, 0));
    //img.draw_line(100, 500, 900, 501, scls::Color(255, 0, 0), 100);

    /*for(int i = 0;i<img.width();i++){
        img.draw_line(i, i, i + 1, i + 1, scls::Color(255, 0, 0), 5);
        img.draw_line(i, i / 2, i + 1, (i + 1) / 2, scls::Color(255, 0, 0), 5);
    }//*/

    //scls::draw_grid(img, &b);
    //scls::draw_function_graph(img, scls::string_to_algebra_element<scls::Formula_Base>("ln(x)").get(), &b);
    /*std::shared_ptr<scls::Table_Base> t_shared_ptr = scls::Table_Base::new_table();scls::Table_Base* t = t_shared_ptr.get();
    scls::Text_Style s;scls::Text_Image_Generator gen;
    for(int i = 0;i<100;i++){for(int j = 0;j<100;j++){t->set_case_value(i, j, std::to_string((i + 5 * j) % (3 * i + 2)), s, &gen);}}//*/
    //t->to_image().save_png("tests/t.png");return 0;

    /*scls::Image img = scls::Image(1000, 1000, scls::Color(255, 255, 255));
    img.draw_line(100, 300, 900, 700, scls::Color(255, 0, 0, 75), 99);
    img.draw_line(100, 700, 900, 300, scls::Color(255, 0, 0, 75), 99);
    img.draw_line(300, 900, 700, 100, scls::Color(255, 0, 0, 75), 99);
    img.draw_line(300, 100, 700, 900, scls::Color(255, 0, 0, 75), 99);//*/

    img.save_png("tests/p.png");

	/*std::shared_ptr<scls::Transform_Object_2D> transform = std::make_shared<scls::Transform_Object_2D>();
	scls::Physic_Engine engine;
	std::shared_ptr<scls::Physic_Object> t = engine.new_physic_object(transform);
	t.get()->set_static(false);t.get()->set_use_gravity(false);
	t.get()->new_collision(scls::Collision_Type::GCT_Circle);
	transform.get()->set_scale_x(0.1);
	transform.get()->set_scale_y(0.1);
	transform.get()->set_x(-3);
	transform.get()->set_y(0);

	std::shared_ptr<scls::Transform_Object_2D> obstacle = std::make_shared<scls::Transform_Object_2D>();
	std::shared_ptr<scls::Physic_Object> obstacle_physic = engine.new_physic_object(obstacle);
	obstacle_physic.get()->new_collision(scls::Collision_Type::GCT_Circle);
	//obstacle_physic.get()->add_collision(-0.5, 0, 0.5, -0.5);
	obstacle_physic.get()->set_static(true);obstacle_physic.get()->set_use_gravity(false);
	obstacle.get()->set_scale_x(2);
	obstacle.get()->set_scale_y(2);
	obstacle.get()->set_x(0);
	obstacle.get()->set_y(0);

	std::shared_ptr<scls::Transform_Object_2D> obstacle_1 = std::make_shared<scls::Transform_Object_2D>();
	std::shared_ptr<scls::Physic_Object> obstacle_physic_1 = engine.new_physic_object(obstacle_1);
	obstacle_physic_1.get()->new_collision(scls::Collision_Type::GCT_Circle);
	obstacle_physic_1.get()->set_static(true);obstacle_physic_1.get()->set_use_gravity(false);
	obstacle_1.get()->set_scale_x(1);
	obstacle_1.get()->set_scale_y(1);
	obstacle_1.get()->set_x(1);
	obstacle_1.get()->set_y(2);

	std::shared_ptr<scls::Transform_Object_2D> obstacle_2 = std::make_shared<scls::Transform_Object_2D>();
	std::shared_ptr<scls::Physic_Object> obstacle_physic_2 = engine.new_physic_object(obstacle_2);
	obstacle_physic_2.get()->new_collision(scls::Collision_Type::GCT_Circle);
	obstacle_physic_2.get()->set_static(true);obstacle_physic_2.get()->set_use_gravity(false);
	obstacle_2.get()->set_scale_x(1);
	obstacle_2.get()->set_scale_y(1);
	obstacle_2.get()->set_x(1);
	obstacle_2.get()->set_y(-2);

	std::shared_ptr<scls::Plane_Base> b = std::make_shared<scls::Plane_Base>(scls::Plane_Base::base_for_image(1000, 1000, 100, 100));
	scls::Image image = scls::Image(1000, 1000, scls::Color(255, 255, 255));
	//vector_field_2d(image, scls::string_to_algebra_element<scls::Formula_Base>("-y").get(), scls::string_to_algebra_element<scls::Formula_Base>("x").get(), b.get());
	scls::Turtle turtle = scls::Turtle(image);
	turtle.set_pen_size(10);turtle.pen_up();turtle.go_to_object(transform.get(), b.get());turtle.pen_down();

	t.get()->set_velocity(scls::Point_2D(0.3, -1));

	for(int i = 0;i<560;i++) {
        std::cout << "A " << i << std::endl;
		for(int j = 0;j<340;j++) {
			transform.get()->set_x(-1.5 + static_cast<double>(j) / 110.0);
			transform.get()->set_y(static_cast<double>(i) / 110.0);

			std::map<scls::Transform_Object_2D*, std::shared_ptr<scls::Transform_Object_2D>> transforms;
			scls::Physic_Engine engine_clone;
			engine.clone(&engine_clone, transforms);
			turtle.clear_actions();
			//turtle.add_action_follow(transforms[transform.get()], b);
			turtle.pen_up();
			turtle.go_to_object(transforms[transform.get()].get(), b.get());
			turtle.pen_down();

			for(int k=0;k<1000;k++) {
				engine_clone.update_physic(0.01);
				turtle.update_actions(0.01);
			}

			scls::Point_2D current_velocity = transforms[transform.get()].get()->velocity();
			double angle = scls::vector_2d_angle(current_velocity);
			image.fill_circle(transform.get(), b.get(), 1, scls::Color(angle * 35.0, angle * 35.0, 0));
		}
	}

	t.get()->set_velocity(scls::Point_2D(-0.3, 1));

	for(int i = 0;i<560;i++) {
        std::cout << "B " << i << std::endl;
		for(int j = 0;j<340;j++) {
			transform.get()->set_x(-1.5 + static_cast<double>(j) / 110.0);
			transform.get()->set_y(-5.0 + static_cast<double>(i) / 110.0);

			std::map<scls::Transform_Object_2D*, std::shared_ptr<scls::Transform_Object_2D>> transforms;
			scls::Physic_Engine engine_clone;
			engine.clone(&engine_clone, transforms);
			turtle.clear_actions();
			//turtle.add_action_follow(transforms[transform.get()], b);
			turtle.pen_up();
			turtle.go_to_object(transforms[transform.get()].get(), b.get());
			turtle.pen_down();

			for(int k=0;k<1000;k++) {
				engine_clone.update_physic(0.01);
				turtle.update_actions(0.01);
			}

			scls::Point_2D current_velocity = transforms[transform.get()].get()->velocity();
			double angle = scls::vector_2d_angle(current_velocity);
			image.fill_circle(transform.get(), b.get(), 1, scls::Color(angle * 35.0, angle * 35.0, 0));
		}
	}

	t.get()->set_velocity(scls::Point_2D(1, 0.3));

	for(int i = 0;i<1110;i++) {
        std::cout << "C " << i << std::endl;
		for(int j = 0;j<440;j++) {
			transform.get()->set_x(-5.0 + static_cast<double>(j) / 110.0);
			transform.get()->set_y(-5.0 + static_cast<double>(i) / 110.0);

			std::map<scls::Transform_Object_2D*, std::shared_ptr<scls::Transform_Object_2D>> transforms;
			scls::Physic_Engine engine_clone;
			engine.clone(&engine_clone, transforms);
			turtle.clear_actions();
			//turtle.add_action_follow(transforms[transform.get()], b);
			turtle.pen_up();
			turtle.go_to_object(transforms[transform.get()].get(), b.get());
			turtle.pen_down();

			for(int k=0;k<1000;k++) {
				engine_clone.update_physic(0.01);
				turtle.update_actions(0.01);
			}

			scls::Point_2D current_velocity = transforms[transform.get()].get()->velocity();
			double angle = scls::vector_2d_angle(current_velocity);
			image.fill_circle(transform.get(), b.get(), 1, scls::Color(angle * 35.0, angle * 35.0, 0));
		}
	}

	t.get()->set_velocity(scls::Point_2D(-1, -0.3));

	for(int i = 0;i<1110;i++) {
        std::cout << "D " << i << std::endl;
		for(int j = 0;j<400;j++) {
			transform.get()->set_x(1.5 + static_cast<double>(j) / 110.0);
			transform.get()->set_y(-5.0 + static_cast<double>(i) / 110.0);

			std::map<scls::Transform_Object_2D*, std::shared_ptr<scls::Transform_Object_2D>> transforms;
			scls::Physic_Engine engine_clone;
			engine.clone(&engine_clone, transforms);
			turtle.clear_actions();
			//turtle.add_action_follow(transforms[transform.get()], b);
			turtle.pen_up();
			turtle.go_to_object(transforms[transform.get()].get(), b.get());
			turtle.pen_down();

			for(int k=0;k<1000;k++) {
				engine_clone.update_physic(0.01);
				turtle.update_actions(0.01);
			}

			scls::Point_2D current_velocity = transforms[transform.get()].get()->velocity();
			double angle = scls::vector_2d_angle(current_velocity);
			image.fill_circle(transform.get(), b.get(), 1, scls::Color(angle * 35.0, angle * 35.0, 0));
		}
	}

	image.fill_circle(obstacle.get(), b.get(), scls::Color(255, 0, 0));
	image.fill_circle(obstacle_1.get(), b.get(), scls::Color(255, 0, 0));
	image.fill_circle(obstacle_2.get(), b.get(), scls::Color(255, 0, 0));

	image.save_png("tests/l.png");

    return 0;//*/
}
