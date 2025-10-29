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
    scls::Image c = scls::Image("C:/Users/mmatt/Pictures/roche_xbox.png");
    scls::Image i = scls::Image(c.width() * 10, c.height() * 10, scls::Color(0, 0, 0));
    for(int j = 0;j<10;j++) {
        for(int k = 0;k<10;k++) {
            i.paste(c, j * c.width(), k * c.height());
        }
    }
    i.save_png("test.png");

    return 0;
}
