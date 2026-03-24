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

int main() {
    // Precision problem : 17614578979479 / 11245
	scls::Extendable_Int m = scls::Extendable_Int(1);
	for(int i = 0;i<1000;i++) {
        scls::Extendable_Int n = scls::Extendable_Int(i + 1);
        m.multiply(&n);
	}
	std::string needed = m.to_std_string();
	scls::write_in_file("tests/bcp.txt", needed);
	std::cout << m.to_std_string() << std::endl;//*/

	/*Mega_Cool_Integer m = Mega_Cool_Integer(1);for(int i = 0;i<30;i++){m.multiply(10);}m.multiply(1054897897);
	std::cout << m.to_std_string() << std::endl;//*/

	/*std::shared_ptr<scls::Formula_Base> f = scls::string_to_algebra_element<scls::Formula_Base>("sin(x)");
    //std::shared_ptr<scls::Formula_Base> g = scls::string_to_algebra_element<scls::Formula_Base>("-sqrt(1 - x * x)");

    scls::Plane_Base b = scls::Plane_Base(50, 50, 500, 500);
    scls::Image img = scls::Image(1000, 1000, scls::Color(255, 255, 255));
    scls::draw_grid(img, &b);
    scls::draw_function_graph(img, f.get(), &b);
    img.save_png("tests/t.png");//*/

    return 0;
}
