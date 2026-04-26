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

struct Limit {
	Limit(scls::Fraction v){value = v;};

	bool exists = true;
	char infinity = 0;
	scls::Fraction value = 0;

	// Limit sign
	char sign() {
		if(infinity != 0){return infinity;}
		return scls::sign(value.to_double());
	}
};
Limit formula_limit(scls::Formula_Base* f, Limit to) {
	Limit to_return = Limit(0);
	if(f->is_known()){to_return = Limit(*f->value<scls::Fraction>());}
	else if(f->is_final_element()) {to_return = to;}
	else {
		bool first_limit = true;

		if(f->algebra_operator_name() == std::string_view("+")) {
			for(std::size_t i = 0;i<f->algebra_elements_const().size();i++) {
				scls::Formula_Base* current_f = f->formula_element(i);
				Limit current_limit = formula_limit(current_f, to);
				if(first_limit){to_return = current_limit;first_limit = false;}
				else {
					if(to_return.infinity * current_limit.infinity == -1) {
						to_return.exists = false;
						break;
					}
					else {
						if(to_return.infinity == 0){to_return.infinity = current_limit.infinity;}
						to_return.value += current_limit.value;
					}
				}
			}
		}
		else if(f->algebra_operator_name() == std::string_view("*")) {
			for(std::size_t i = 0;i<f->algebra_elements_const().size();i++) {
				scls::Formula_Base* current_f = f->formula_element(i);
				Limit current_limit = formula_limit(current_f, to);

				if(first_limit){to_return = current_limit;first_limit = false;}
				else {
					if((std::abs(to_return.infinity) == 1 && (current_limit.infinity == 0 && current_limit.value == 0)) || (std::abs(current_limit.infinity) == 1 && (to_return.infinity == 0 && to_return.value == 0))) {
						to_return.exists = false;
						break;
					}
					else {
						if(to_return.infinity == 0){to_return.infinity = current_limit.infinity;}
						else if(current_limit.infinity != 0){to_return.infinity *= current_limit.infinity;}
						to_return.value *= current_limit.value;
					}
				}
			}
		}
		else if(f->algebra_operator_name() == std::string_view("/")) {
			scls::Formula_Base* denominator = f->formula_element(1);
			scls::Formula_Base* numerator = f->formula_element(0);
			Limit denominator_limit = formula_limit(denominator, to);
			Limit numerator_limit = formula_limit(numerator, to);

			std::cout << "A " << numerator->to_std_string(0) << " " << (int)numerator_limit.infinity << " " << (int)denominator_limit.infinity << " " << to.value.to_double() << " " << (int)to.infinity << " " << to.exists << std::endl;
			std::cout << "B " << numerator_limit.value.to_double() << " " << (int)numerator_limit.infinity << " " << (int)denominator_limit.infinity << std::endl;
			if(denominator_limit.infinity == 0 && numerator_limit.infinity == 0){
				if(denominator_limit.value == 0 && numerator_limit.value == 0){
					to_return.exists = false;
				}
				else if(denominator_limit.value == 0){
					to_return.infinity = scls::sign(numerator_limit.value.to_double());
				}
				else if(numerator_limit.value == 0) {
					to_return.value = 0;
				}
				else {
					to_return.value = numerator_limit.value / denominator_limit.value;
				}
			}
			else if(denominator_limit.infinity != 0 && numerator_limit.infinity != 0) {
				to_return.exists = false;
			}
			else {
				to_return.infinity = scls::sign(denominator_limit.sign() * numerator_limit.sign());
			}
		}
	}

	return to_return;
}

int main() {
	//Extendable_Fraction f = Extendable_Fraction(71, 7);
	//f.normalize();

	std::shared_ptr<scls::Extendable_Formula_Base> f = scls::string_to_algebra_element<scls::Extendable_Formula_Base>("exp(x)");
	//f = scls::string_to_algebra_element<scls::Extendable_Formula_Base>("((1/1307674368000) * (x) * (x) * (x) * (x) * (x) * (x) * (x) * (x) * (x) * (x) * (x) * (x) * (x) * (x) * (x)) + ((1/87178291200) * (x) * (x) * (x) * (x) * (x) * (x) * (x) * (x) * (x) * (x) * (x) * (x) * (x) * (x))");
	//std::shared_ptr<scls::Extendable_Formula_Base> g = scls::string_to_algebra_element<scls::Extendable_Formula_Base>("(x * x) / 2 + x");
	std::shared_ptr<scls::Extendable_Formula_Base> dev = scls::mclaurin(f.get(), std::string("x"), 25);

	scls::Image img = scls::Image(1000, 1000);
	scls::Vector_Base p = scls::Vector_Base(50, 0, 0, 50, 500, 500);
	scls::draw_grid(img, &p);
	scls::draw_function_graph(img, f.get(), &p, -1000, 1000);
	scls::draw_function_graph(img, dev.get(), &p, scls::Color(0, 0, 255), -1000, 1000);
	img.save_png("tests/f.png");

	std::cout << "E " << f.get()->to_std_string(0) << " " << dev.get()->replace_unknowns("x", scls::Extendable_Fraction(1)).get()->value_to_double() << std::endl;

    /*std::shared_ptr<scls::Formula_Base> f = scls::string_to_algebra_element<scls::Formula_Base>("1/4 * x + 1");

	scls::Image img = scls::Image(1000, 1000);
	scls::Vector_Base p = scls::Vector_Base(50, 0, 0, 50, 500, 500);
	scls::draw_grid(img, &p);
	scls::draw_function_graph(img, f.get(), &p, -1000, 2);
	img.save_png("tests/f.png");//*/

	/*double branches = 8;
	double center_x = 500;
	double center_y = 500;
	double r_outer = 200;
	double r_inner = 150;
	scls::Vector_Base p = scls::Vector_Base(50, 0, 0, 50, 500, 500);
	std::vector<scls::Point_2D> points = std::vector<scls::Point_2D>(branches * 2);
	const double angle_step = SCLS_PI / branches;
	for (int i = 0; i < branches * 2; ++i) {
		// On commence à -PI/2 pour que la première pointe soit vers le haut
		double angle  = static_cast<double>(i) * angle_step;
		double radius = (i % 2 == 0) ? r_outer : r_inner;

		double x = center_x + radius * std::cos(angle);
		double y = center_y + radius * std::sin(angle);
		points[i] = scls::Point_2D(500 - p.canonical_to_base_x(x, y) * 50, 500 + p.canonical_to_base_y(x, y) * 50);
	}

	scls::Image img = scls::Image(1000, 1000);
	scls::draw_grid(img, &p);
	img.fill_form(points, scls::Color(255, 0, 0));
	img.save_png("tests/f.png");//*/


	//scls::Relation_Egality r;
	//f.get()->check_relation(&r, g.get());
	//std::cout << "I " << f.get()->definition_domain().to_std_string(0) << std::endl;

	//scls::Matrix product = m.product(&a);
	//std::cout << product.to_std_string(0) << std::endl;

    //scls::Image img = scls::Image(1000, 1000, scls::Color(255, 255, 255));
	//scls::draw_by_rotation(img, scls::string_to_algebra_element<scls::Formula_Base>("50000/x"), 0);
	//img.save_png("tests/t.png");

	/*scls::Relation_Module r = scls::Relation_Module(15);
	scls::Relation_Order s = scls::Relation_Order(scls::Relation_Order::lesser_strict);
	std::cout << s.is_in_relation(164794987, 4) << " " << s.is_in_relation(4, 164794987) << std::endl;

	std::string c = std::string("3 * x + 2 ~ 0 [4]");//*/

	/*std::shared_ptr<scls::Formula_Base> f = scls::string_to_algebra_element<scls::Formula_Base>("sin(x)");
    //std::shared_ptr<scls::Formula_Base> g = scls::string_to_algebra_element<scls::Formula_Base>("-sqrt(1 - x * x)");

    scls::Plane_Base b = scls::Plane_Base(50, 50, 500, 500);
    scls::Image img = scls::Image(1000, 1000, scls::Color(255, 255, 255));
    scls::draw_grid(img, &b);
    scls::draw_function_graph(img, f.get(), &b);
    img.save_png("tests/t.png");//*/

    return 0;
}
