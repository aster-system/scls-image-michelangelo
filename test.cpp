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
	std::shared_ptr<scls::__Formula> f = scls::string_to_formula("(3x*x*x+4x-8)(5x*x*x*x+8x*x*x+2x)+ln(3x*x+2x-4)");
	std::cout << f.get()->to_std_string(0) << std::endl;

    return 0;
}
