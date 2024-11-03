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
    //std::shared_ptr<scls::Image> img = scls::aster_system_logo();
    //img.get()->save_png("test/logo.png");

    // Define the needed datas
    Color black_color(0, 0, 0);
    int image_width = 1000;
    Color white_color(255, 204, 153);

    // Create the image
    Image img(image_width, image_width, white_color);

    // Draw each black cases
    int case_width = image_width / 8;
    for(int i = 0;i < 8;i++) {
        for(int j = 0;j < 4;j++) {
            img.fill_rect(case_width * (j * 2 + i % 2), case_width * i, case_width, case_width, black_color);
        }
    }

    // Save the image
    img.save_png("test/chess.png");

    return 0;
}
