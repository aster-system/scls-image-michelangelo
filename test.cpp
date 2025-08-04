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
    scls::Image test = scls::Image(1000, 1000, scls::Color(255, 255, 255));
    //test.fill_circle(500, 500, 400, 400, 45, 360, scls::Color(255, 0, 0), 25, scls::Color(0, 0, 255));
    //test.fill_circle(500, 500, 100, 400, 45, 360, scls::Color(255, 0, 0), 25, scls::Color(0, 0, 255));
    test.fill_circle(500, 500, 400, 200, 0, 70, 360, scls::Color(255, 0, 0), 75, scls::Color(0, 0, 255));
    test.save_png("tests/test_0.png");test.fill(scls::Color(255, 255, 255));
    test.fill_circle(500, 500, 400, 200, -10, 70, 360, scls::Color(255, 0, 0), 75, scls::Color(0, 0, 255));
    test.save_png("tests/test_1.png");test.fill(scls::Color(255, 255, 255));
    test.fill_circle(500, 500, 400, 200, -25, 70, 360, scls::Color(255, 0, 0), 75, scls::Color(0, 0, 255));
    test.save_png("tests/test_2.png");test.fill(scls::Color(255, 255, 255));
    test.fill_circle(500, 500, 400, 200, -65, 70, 360, scls::Color(255, 0, 0), 75, scls::Color(0, 0, 255));
    test.save_png("tests/test_3.png");

    return 0;
}
