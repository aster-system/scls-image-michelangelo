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

int main() {
	std::shared_ptr<scls::Formula_Base> f = scls::string_to_algebra_element<scls::Formula_Base>("sqrt(ln(3 * x + 1))");
	std::cout << "I " << f.get()->definition_domain().to_std_string(0) << std::endl;

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
