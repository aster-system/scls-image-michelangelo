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
    scls::aster_system_logo("ASTER", "CITY").get()->save_png("tests/aster_city.png");
    std::shared_ptr<scls::__Image_Base> test = std::make_shared<__Image_Base>(1000, 1000, scls::Color(255, 255, 255));
    test.get()->fill_circle(100, 500, 300, scls::Color(255, 0, 0), 5, scls::Color(0, 0, 255));
    test.get()->save_png("tests/test.png");

    return 0;
}
