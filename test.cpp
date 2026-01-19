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
#include "scls_image_directory/scls_image_table.h"

SCLS_INIT;

using namespace scls;

int main() {
	/*std::shared_ptr<Statistics> s = std::make_shared<Statistics>();
	s.get()->set_this_object(s);

	for(int i = 0;i<100000;i++) {
		s.get()->add_data(std::to_string(random_int_between_included(0, 50)));
	}//*/

	std::shared_ptr<Boolean> d = string_to_algebra_element<Boolean>("(((a . b . f) + (c . d . e)) . (b + c + d) . ((a . d) + (b . c) + e)) + ((e . f) + (b . e) + (b . f))");
	//std::shared_ptr<Boolean> d = string_to_algebra_element<Boolean>("(a . d) + (b . c)");

	std::shared_ptr<scls::__Table_Case> t = scls::table_from_boolean_karnaugh(d.get());
	t.get()->to_image().save_png("tests/table_karnaugh.png");
	t = scls::table_from_boolean(d.get());
	t.get()->to_image().save_png("tests/table.png");

	//std::cout << to_return << std::endl;

    return 0;
}
