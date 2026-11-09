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

    //img.draw_line(100, 1000, 1800, 1000, scls::Color(0, 0, 0, 100), 15);
    //img.draw_line(1000, 100, 1000, 1800, scls::Color(0, 0, 0, 100), 15);

    double t = scls::time_ns();
    for(int i = 0;i<1;i++){if(i>0){img.set_editing_change_next_value(false);} img.draw_line(100, 100, 1900, 1800, scls::Color(0, 0, 0, 100), 15);}
    std::cout << "U " << static_cast<double>(scls::time_ns() - t) / 1000000000.0 << std::endl;

    t = scls::time_ns();
    //for(int i = 0;i<1;i++){img.image()->line_points(100, 100, 1800, 1900, 15);}
    std::cout << "V " << static_cast<double>(scls::time_ns() - t) / 1000000000.0 << std::endl;

    t = scls::time_ns();
    //for(int i = 0;i<1;i++){img.image()->set_pixels(img.image()->line_points(100, 100, 1900, 1800, 15), scls::Color(0, 0, 0, 100));}
    std::cout << "V " << static_cast<double>(scls::time_ns() - t) / 1000000000.0 << std::endl;

    img.save_png("./tests/l.png");
}

scls::Complex temp(scls::Complex c){return (c - scls::Complex(3)) * (c - scls::Complex(-3)) * (c - scls::Complex(0, 3)) * (c - scls::Complex(0, -3));}
int main() {
	//test_draw_lines();

	//scls::Image img = scls::Image(1000, 1000, scls::Color(255, 255, 255));
	//std::shared_ptr<scls::Plane_Base> b = std::make_shared<scls::Plane_Base>(scls::Plane_Base::base_for_image(img.width(), img.height(), 200, 200));
	//scls::draw_grid(img, b.get());

	//scls::Turtle t = scls::Turtle(img);t.set_plane_base(b);t.set_t(-1.0);
	std::vector<scls::Point_2D> points = {scls::Point_2D(-1, -1), scls::Point_2D(0, 1), scls::Point_2D(1, 1)};
	//for(int i = 0;i<8;i++){points.push_back(points.at(i));}
	//scls::put_lagrange_interpolation_in_turtle(&t, points);
	//t.simulate(10, 0.01);

	//std::string r = std::string("cos(3 * x + y) + exp(7 * x ^ y + ln(42x * y)) - 4 * y");
	//std::shared_ptr<scls::Formula_Base_Template<scls::Double>> f = scls::string_to_algebra_element<scls::Formula_Base_Template<scls::Double>>(r);
	std::shared_ptr<scls::Formula_Base_Template<scls::Double>> f = scls::lagrange_interpolator(points);
	std::string c = f.get()->to_cpp_function("formula");
	std::cout << "Formula : " << f.get()->to_std_string(0) << std::endl;
	std::cout << "C++ : " << c << std::endl;
	//execute_code(c);

	//std::size_t n = 4;
	//double left_offset = -scls::pi * n;
	//std::vector<scls::Point_2D> p = {scls::Point_2D(-5, 5), scls::Point_2D(-4, 1), scls::Point_2D(-3, 3), scls::Point_2D(-2, 2), scls::Point_2D(-1, 0), scls::Point_2D(0, -3), scls::Point_2D(1, 0), scls::Point_2D(2, 2), scls::Point_2D(3, 3), scls::Point_2D(4, 1), scls::Point_2D(5, 5)};

	//scls::draw_function_graph(img, "cos(x)", &b, scls::Color(0, 0, 255));
	//scls::draw_function_graph(img, scls::lagrange_interpolator(p).get(), &b, scls::Color(0, 255, 0));
	//img.save_png("tests/m.png");

	/*std::string r = std::string("(3 x + 4) > (2 x - 5)");

	std::shared_ptr<scls::Formula_Base_Template<scls::Double>> formula = scls::string_to_algebra_element<scls::Formula_Base_Template<scls::Double>>("3 * x + 2");
	std::shared_ptr<scls::Formula_Base_Template<scls::Double>> formula_2 = scls::string_to_algebra_element<scls::Formula_Base_Template<scls::Double>>(" 3 * x * x + 2");
	std::cout << "U " << formula.get()->in_relation(formula_2.get(), "=") << std::endl;
	formula = formula.get()->replace_unknowns("x", scls::Double(0));
	formula_2 = formula_2.get()->replace_unknowns("x", scls::Double(0));
	std::cout << "V " << formula.get()->in_relation(formula_2.get(), "=") << std::endl;

	int x = 5;
	bool value = x > 5;//*/

    return 0;
}
