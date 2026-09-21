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
	//scls::Image img = scls::Image(2000, 2000);scls::Plane_Base b = scls::Plane_Base::base_for_image(2000, 2000, 100, 100);
    //scls::vector_field_2d(img, {scls::string_to_algebra_element<scls::Formula_Base>("(x-2)*10*exp(-((x - 2)*(x - 2)+(y - 2)*(y - 2)))").get(), scls::string_to_algebra_element<scls::Formula_Base>("(x+2)*10*exp(-((x + 2)*(x + 2)+(y + 2)*(y + 2)))").get()}, {scls::string_to_algebra_element<scls::Formula_Base>("(y-2)*10*exp(-((x - 2)*(x - 2)+(y - 2)*(y - 2)))").get(), scls::string_to_algebra_element<scls::Formula_Base>("(y+2)*10*exp(-((x + 2)*(x + 2)+(y + 2)*(y + 2)))").get()}, &b);

    /*img.draw_line(100, 500, 900, 500, scls::Color(255, 0, 0), 21);
    img.draw_line(100, 800, 900, 800, scls::Color(255, 0, 0), 21);
    img.draw_line(100, 500, 100, 800, scls::Color(255, 0, 0), 21);
    img.draw_line(900, 500, 900, 800, scls::Color(255, 0, 0), 21);

    img.draw_line(100, 100, 900, 100, scls::Color(255, 0, 0), 22);
    img.draw_line(100, 400, 900, 400, scls::Color(255, 0, 0), 22);
    img.draw_line(100, 100, 100, 400, scls::Color(255, 0, 0), 22);
    img.draw_line(900, 100, 900, 400, scls::Color(255, 0, 0), 22);

    img.set_pixel(100, 100, scls::Color(0, 0, 255));
    img.set_pixel(100, 400, scls::Color(0, 0, 255));
    img.set_pixel(900, 100, scls::Color(0, 0, 255));
    img.set_pixel(900, 400, scls::Color(0, 0, 255));
    img.set_pixel(100, 500, scls::Color(0, 0, 255));
    img.set_pixel(100, 800, scls::Color(0, 0, 255));
    img.set_pixel(900, 500, scls::Color(0, 0, 255));
    img.set_pixel(900, 800, scls::Color(0, 0, 255));//*/

	scls::Matrix c = scls::Matrix::matrix_column({scls::string_to_algebra_element<scls::Formula_Base>("d"), scls::string_to_algebra_element<scls::Formula_Base>("e"), scls::string_to_algebra_element<scls::Formula_Base>("f")});

	scls::Matrix m = scls::Matrix::matrix_random_integer_included_between(10, 10, -30, 30);
	scls::Matrix n = scls::Matrix::matrix_random_integer_included_between(10, 10, -30, 30);
    scls::Text_Style s;s.set_font_size(30);

    scls::Image img = scls::string_to_image("<math>" + m.to_mathml(0) + "<mo>.</mo>" + n.to_mathml(0) + "<mo>=</mo>" + m.product(&n).to_mathml(0) + "</math>", s);std::cout << m.to_std_string(0) << std::endl;
    img.save_png("tests/p.png");
    //img.draw_line(100, 100, 900, 900, scls::Color(255, 0, 0), 100);
    //img.draw_line(100, 100, 900, 100, scls::Color(255, 0, 0), 100);
    //img.draw_line(100, 100, 100, 900, scls::Color(255, 0, 0), 100);
    //img.fill_circle(900, 100, 50, scls::Color(255, 0, 0));
    //img.draw_line(100, 500, 900, 501, scls::Color(255, 0, 0), 100);

    //img.draw_line(0, 0, 2000, 2000, scls::Color(255, 0, 0), 5);
    //img.draw_line(0, 0, 2000, 1000, scls::Color(255, 0, 0), 5);
    //img.draw_line(0, 50, 2000, 50, scls::Color(255, 0, 0), 5);
    //img.draw_line(0, 0, 1000, 2000, scls::Color(255, 0, 0), 5);

    /*for(int i = 0;i<img.width();i++){
        img.draw_line(i, i, i + 1, i + 1, scls::Color(255, 0, 0), 5);
        img.draw_line(i, i / 2, i + 1, (i + 1) / 2, scls::Color(255, 0, 0), 5);
        img.draw_line(i, 50, i + 1, 50, scls::Color(255, 0, 0), 5);
        img.draw_line(i / 2, i, (i + 1) / 2, i + 1, scls::Color(255, 0, 0), 5);
    }//*/

    //scls::draw_grid(img, &b);
    //scls::draw_function_graph(img, scls::string_to_algebra_element<scls::Formula_Base>("x/20").get(), &b);
    //img.draw_line(100, 500, 500, 200, scls::Color(255, 0, 0), 10);
    //img.draw_line(100, 500, 500, 400, scls::Color(255, 0, 0), 10);
    //img.draw_line(100, 500, 500, 600, scls::Color(255, 0, 0), 10);
    //img.draw_line(100, 500, 500, 500, scls::Color(255, 0, 0), 10);
    //img.draw_line(100, 100, 1900, 1900, scls::Color(0, 255, 0), 5);
    //img.draw_line(100, 100, 1900, 1800, scls::Color(0, 0, 255), 5);
   /* std::shared_ptr<scls::Table_Base> t_shared_ptr = scls::Table_Base::new_table();scls::Table_Base* t = t_shared_ptr.get();
    scls::Text_Style s;scls::Text_Image_Generator gen;s.set_font_size(40);
    for(int i = 0;i<500;i++){t->set_case_value(0, i, std::to_string(scls::random_int_between_included(0, std::max((int)(500.0 - i * 2.0), 0))), s, &gen);}//*/
    //t->to_image().save_png("tests/t.png");return 0;

	/*int n = 5000;

	double time = scls::time_ns();
	scls::Double d = 0;
    for(int i = 1;i<n;i++) {
    	scls::Double td = 1;
    	for(int j = 1;j<i;j++){td *= j;}
    	d += td.sqrt();
    }
    std::cout << "E " << std::setprecision(15) << (scls::time_ns() - time) / std::pow(10, 9) << std::endl;
    std::cout << "F " << d.to_double() << std::endl;

    time = scls::time_ns();
	scls::Fraction d_2 = 0;
	for(int i = 1;i<n;i++) {
		scls::Fraction td_2 = 1;
		for(int j = 1;j<i;j++){td_2 *= j;}
		d_2 += td_2.sqrt();
	}
	std::cout << "E " << (scls::time_ns() - time) / std::pow(10, 9) << std::endl;
	std::cout << "F " << d_2.to_double() << std::endl;//*/

    /*scls::Image img = scls::Image(1000, 1000, scls::Color(255, 255, 255));
    img.draw_line(100, 300, 900, 700, scls::Color(255, 0, 0, 75), 99);
    img.draw_line(100, 700, 900, 300, scls::Color(255, 0, 0, 75), 99);
    img.draw_line(300, 900, 700, 100, scls::Color(255, 0, 0, 75), 99);
    img.draw_line(300, 100, 700, 900, scls::Color(255, 0, 0, 75), 99);//*/

    //scls::Image img = scls::Image(2000, 2000);scls::Plane_Base b = scls::Plane_Base::base_for_image(2000, 2000, 100, 100);
    //img.fill_circle(1000, 1000, 600, 600, scls::Color(255, 0, 0), 20, scls::Color(0, 255, 0));

    /*for(int i = 0;i<10;i++){
        img.image()->fill_oval(100 + i * 20, 1000, 1 + i, 1 + i, 255, 0, 0, 255, 0, 0, 0, 0, 0);
    }

    double n = scls::time_ns();
    for(int i = 0;i<1000;i++){
        img.image()->fill_circle(1000, 1500, 250, 450, scls::Color(255, 0, 0), 50, scls::Color(0, 0, 0));
    }
    std::cout << (scls::time_ns() - n) / 1000000000.0 << std::endl;n = scls::time_ns();
    for(int i = 0;i<1000;i++){
        img.image()->fill_oval(1000, 500, 250, 450, 255, 0, 0, 255, 50, 0, 0, 0, 255);
    }
    std::cout << (scls::time_ns() - n) / 1000000000.0 << std::endl;n = scls::time_ns();


    img.save_png("tests/p.png");//*/

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
