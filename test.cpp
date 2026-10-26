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

void apply_acceleration_from_field_2d(scls::Physic_Object* object, scls::Formula_Base* formula_x, scls::Formula_Base* formula_y, double delta_time) {
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

void test_draw_lines() {
    scls::Image img = scls::Image(2000, 2000, scls::Color(255, 255, 255));

    img.draw_line(100, 1000, 1800, 1000, scls::Color(0, 0, 0, 100), 15);
    img.draw_line(1000, 100, 1000, 1800, scls::Color(0, 0, 0, 100), 15);

    double t = scls::time_ns();
    //for(int i = 0;i<100;i++){img.draw_line(100, 100, 1800, 1900, scls::Color(0, 0, 0), 15);}
    std::cout << "U " << static_cast<double>(scls::time_ns() - t) / 1000000000.0 << std::endl;

    t = scls::time_ns();
    //for(int i = 0;i<1;i++){img.image()->line_points(100, 100, 1800, 1900, 15);}
    std::cout << "V " << static_cast<double>(scls::time_ns() - t) / 1000000000.0 << std::endl;

    t = scls::time_ns();
    for(int i = 0;i<1;i++){img.image()->set_pixels(img.image()->line_points(100, 100, 1800, 1900, 15), scls::Color(0, 0, 0, 100));}
    std::cout << "V " << static_cast<double>(scls::time_ns() - t) / 1000000000.0 << std::endl;

    img.save_png("./tests/l.png");
}

scls::Complex temp(scls::Complex c){return (c - scls::Complex(3)) * (c - scls::Complex(-3)) * (c - scls::Complex(0, 3)) * (c - scls::Complex(0, -3));}
int main() {
	test_draw_lines();

    return 0;
}
