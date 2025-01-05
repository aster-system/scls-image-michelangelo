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

SCLS_INIT

using namespace scls;

int main() {
    scls::Text_Image_Generator gen;
    scls::Text_Style style; style.font_size = 300;
    gen.image_shared_ptr("<math><mi>E</mi><esh><esh><sub>S</sub><mi>dS</mi><mo>=</mo><esh><esh><esh><sub>V</sub><vec><nabla></vec><vec>E</vec><mi>dV</mi></math>", style).get()->save_png("test/math.png");

    return 0;
}
