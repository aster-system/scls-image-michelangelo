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

SCLS_INIT;

using namespace scls;

int main() {
	std::shared_ptr<Boolean> d = string_to_algebra_element<Boolean>("(((a . b . f) + (c . d . e)) . (b + c + d) . ((a . d) + (b . c) + e)) + ((e . f) + (b . e) + (b . f))");
	std::string to_return = d.get()->verity_table();
	write_in_file("tests/boole.txt", to_return);
	std::cout << to_return << std::endl;

    return 0;
}
