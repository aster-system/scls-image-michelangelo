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
// This file includes all the SCLS Image files.
//
//******************
// Dependencies :
//
// To work, this file needs the zlib library.
// Get it on this web site : https://www.zlib.net/.
//

#ifndef SCLS_IMAGE
#define SCLS_IMAGE

// Include SCLS Math
#ifdef __ASTER_DEV
#include "../scls-math-carl/scls_math.h"
#else
#include <scls_math.h>
#endif // __ASTER_DEV

#ifndef LOGO_FONT_PATH
// Before school PC : "C:\\Divers\\Fonts\\AlteHaasGroteskBold.ttf"
// Development PC : "E:\\Divers\\Fonts\\AlteHaasGroteskBold.ttf"
#define LOGO_FONT_PATH "E:\\Divers\\Fonts\\AlteHaasGroteskBold.ttf"
#endif // LOGO_FONT_PATH

#include "scls_image_directory/scls_image_core.h"
#include "scls_image_directory/scls_image_fractal.h"
#include "scls_image_directory/scls_image_math.h"
#include "scls_image_directory/scls_image_text.h"
#include "scls_image_directory/scls_image_type.h"

namespace scls {

    // Make the logo of Aster System
    inline std::shared_ptr<scls::Image> aster_system_logo(unsigned int width = 1000) {
        double total_width = static_cast<double>(width);
        scls::Image* img = new scls::Image(static_cast<short>(total_width * 2.3), static_cast<short>(total_width), scls::Color(255, 255, 255), SCLS_IMAGE_RGB);

        // Datas about the branchs
        double branch_height = (total_width*(8.0/10.0));
        double branch_width = (total_width/14.0);
        double branch_x = (total_width/10.0);
        double branch_y = (total_width/10.0);
        // Automatic values
        double branch_separation_width = branch_width;
        double branch_height_round = round(branch_height / 12.0);
        double branch_height_slide =round( branch_width + branch_separation_width);
        double branch_height_main = round(branch_height - (branch_height_round * 2.0 + branch_height_slide));

        // Datas about the text
        scls::Color color_text(0, 51, 102);
        double font_size = branch_height / 2.0;
        double text_x = branch_x + branch_width * 2 + branch_separation_width + branch_width / 2.0;
        double text_y = total_width / 2.0 - font_size;

        // Round values
        branch_height = round(branch_height);
        branch_separation_width = round(branch_separation_width);
        branch_width = round(branch_width);
        branch_x = round(branch_x);
        branch_y = round(branch_y);
        font_size = round(font_size);

        // Create the branch 1
        scls::Color color_1(0, 51, 102);
        // Create the first rectangle of the left branch
        double ll1_height = branch_height_main;
        double ll1_width = branch_width;
        double ll1_x = branch_x;
        double ll1_y = branch_y + branch_height_slide + branch_height_round;
        // Create the second rectangle of the left branch
        double ll2_height = (branch_height_round);
        double ll2_width = (branch_height_slide);
        double ll2_x = (ll1_x + ll1_width);
        double ll2_y = (ll1_y + ll1_height);
        // First branch
        img->fill_rect(ll1_x, ll1_y, ll1_width, ll1_height, color_1);
        img->fill_rect(ll2_x, ll2_y, ll2_width, ll2_height, color_1);
        img->fill_triangle(ll2_x, ll2_y, ll2_x, ll2_y - branch_height_slide, ll2_x + branch_height_slide, ll2_y, color_1);
        img->fill_triangle(ll1_x, ll1_y - branch_width, ll1_x, ll1_y, ll1_x + ll1_width, ll1_y, color_1);

        // Create the branch 2
        scls::Color color_2 = color_1;
        // Create the third rectangle of the left branch
        double ll3_height = branch_height_round;
        double ll3_width = branch_height_slide;
        double ll3_x = branch_x;
        double ll3_y = branch_y;
        // Create the first rectangle of the left branch
        ll1_height = (branch_height_main);
        ll1_width = (branch_width);
        ll1_x = (ll3_x + ll3_width);
        ll1_y = (ll3_y + ll3_height);
        // First branch
        img->fill_rect(ll1_x, ll1_y, ll1_width, ll1_height, color_2);
        img->fill_rect(ll3_x, ll3_y, ll3_width, ll3_height, color_2);
        img->fill_triangle(ll3_x + ll3_width, ll3_y + ll3_height + branch_height_slide, ll3_x, ll3_y + ll3_height, ll3_x + ll3_width, ll3_y + ll3_height, color_2);
        img->fill_triangle(ll1_x + ll1_width, ll1_y + ll1_height + branch_width, ll1_x, ll1_y + ll1_height, ll1_x + ll1_width, ll1_y + ll1_height, color_2);

        // Create the text
        std::shared_ptr<scls::Text_Image_Generator> tig = std::make_shared<scls::Text_Image_Generator>();
        // Paste the text "Aster"
        scls::Text_Style style;
        style.color = color_text; style.font.font_path = LOGO_FONT_PATH; style.font_size = font_size;
        std::shared_ptr<scls::Image> current_text = tig.get()->image_shared_ptr("ASTER", style);
        text_y = 0;
        img->paste(current_text.get(), text_x, text_y);
        // Paste the text "Système"
        style.color = color_text; style.font_size = font_size;
        current_text = tig.get()->image_shared_ptr("SYSTÈME", style);
        text_y = branch_y + branch_height - current_text.get()->height();
        img->paste(current_text.get(), text_x, text_y);

        return std::shared_ptr<scls::Image>(img);
    }
};

#endif // SCLS_IMAGE
