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
    scls::Image c = scls::Image(200, 400, scls::Color(255, 255, 255));
    c.draw_circle(100, 99, 100, scls::Color(0, 0, 0), 15);c.fill_rect(92, 195, 16, 205, scls::Color(0, 0, 0));
    scls::Image i = scls::Image(c.width(), c.height() * 20, scls::Color(0, 0, 0));
    for(int j = 0;j<20;j++) {i.paste(c, 0, j * c.height());}
    i.save_png("tests/test.png");

    return 0;
}
