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
    std::string text = std::string("<h1>Le char Leclerc</h1><h2>Motorisation</h2><p>Le char Leclerc est équipé d'une turbine de 1500 chevaux, accéléré par un moteur d'hélicoptère.</p><h2>Armement</h2><p>Le Leclerc est armé d'un canon de 120mm, expulsant une flèche à approximativement 1 mètre de pénétration.</p>");

    scls::Text_Image_Generator tig;Text_Style needed_style;needed_style.set_font_size(20);needed_style.set_max_width(1000);
    needed_style.set_background_color(scls::Color(255, 255, 255, 255));needed_style.set_color(scls::Color(0, 0, 0));
    needed_style.set_max_width(600);
    Image img = tig.image(text, needed_style);
    img.save_png("tests/leclerc.png");

    return 0;
}
