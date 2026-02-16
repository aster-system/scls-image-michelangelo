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
#include "../scls-math-carl/scls_math_directory/scls_math_boolean.h"
#include "../scls-math-carl/scls_math_directory/scls_math_string.h"
#include "scls_image_directory/scls_image_table.h"
#include "scls_image_directory/scls_image_statistic.h"

SCLS_INIT;

int main() {
    scls::Unknowns_Container c;c.create_unknown<scls::Formula_Base::Formula_Unknown>("x")->value = scls::string_to_algebra_element<scls::Formula_Base>("2 * y");
	std::shared_ptr<scls::Formula_Base> f = scls::string_to_algebra_element<scls::Formula_Base>("3 * x + 4 * y + 2 * z");
	while(f.get()->simplify_step() != scls::Formula_Base::NO_SIMPLIFICATION){}
	std::cout << f.get()->replace_unknowns(&c).get()->to_std_string(0) << std::endl;

	return 0;
}
