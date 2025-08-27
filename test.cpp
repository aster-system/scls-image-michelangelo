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
    scls::Text_Image_Generator tig;Text_Style needed_style;needed_style.set_font_size(50);needed_style.set_max_width(1000);
    needed_style.set_background_color(scls::Color(0, 0, 0, 0));needed_style.set_color(scls::Color(0, 0, 0));
    Image img = tig.image(std::string("Boeing"), needed_style);
    img.save_png(std::string("tests/rotated/boeing.png"));
    for(int i = 0;i<15;i++){img.rotated((i + 1) * 6).save_png(std::string("tests/rotated/boeing_") + std::to_string(i) + std::string(".png"));}

    return 0;
}
