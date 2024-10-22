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

#include "scls_image.h"

SCLS_FOUNDATION_INIT
SCLS_MATH_INIT

int main() {
    std::shared_ptr<scls::Image> img = scls::aster_system_logo();
    img.get()->save_png("test/logo.png");

    return 0;
}
