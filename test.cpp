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
	int width = 128;
	Matrix m(width, width);
	for(int i = 0;i<width;i++) {
		for(int j = 0;j<width;j++) {
			(*m.element_at(i, j)) = random_int_between_included(0, 100000) - 50000;
		}
	}

	Matrix n = m;
	for(int i = 0;i<1;i++) {n = n.product(&m);}

	Textual_Math_Settings s;
	s.set_hide_if_0(false);
	to_image("<math><mi>X</mi><mo>=</mo>" + m.to_mathml(&s) +  "</math></br>""<math><mi>X</mi><msup>2</msup><mo>=</mo>" + n.to_mathml(&s) +  "</math>")->save_png("tests/matrix.png");

    return 0;
}
