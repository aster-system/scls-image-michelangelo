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
    std::string text = std::string("<h1>Algèbre</h1><p>L'exposant d'un nombre :</p><p>Définissons <math><mi>u</mi><msubsup><mi>A</mi><mi>B</mi></msubsup></math></p><p>Définissons <math><mi>a</mi><mo>=</mo><mfrac><mrow><mi>A</mi><msub>C</msub></mrow><mrow><mi>B</mi><msup>D</msup></mrow></mfrac></math></p>");

    scls::Text_Image_Generator tig;Text_Style needed_style;needed_style.set_font_size(150);
    needed_style.set_background_color(scls::Color(255, 255, 255, 255));needed_style.set_color(scls::Color(0, 0, 0));
    needed_style.set_max_width(5000);
    Image img = tig.image(text, needed_style);
    img.save_png("tests/leclerc.png");

    return 0;
}
