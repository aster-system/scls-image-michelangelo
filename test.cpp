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
    std::shared_ptr<scls::__Image_Base> test = std::make_shared<__Image_Base>(1000, 1000, scls::Color(255, 255, 255));
    test.get()->fill_circle(500, 500, 400, 300, 45, 0, 360, scls::Color(255, 0, 0), 5, scls::Color(0, 0, 255));
    test.get()->save_png("tests/test_0.png");
    test = std::make_shared<__Image_Base>(1000, 1000, scls::Color(255, 255, 255));
    test.get()->fill_circle(500, 500, 400, 300, -45, 0, 360, scls::Color(255, 0, 0), 5, scls::Color(0, 0, 255));
    test.get()->save_png("tests/test_1.png");

    return 0;
}
