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
    std::shared_ptr<scls::Image> img = std::make_shared<scls::Image>("/home/matto/Images/joueur.png");

    // Save the image
    img.get()->save_png("test/test1.png");
    img.get()->load_from_path("/home/matto/Images/mur.png");
    img.get()->save_png("test/test2.png");

    return 0;
}
