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
// To work, this file needs the ZLib library.
// Get it on this web site : https://www.zlib.net/.
// It also need the FreeType library.
// Get it on this web site : https://freetype.org/.
// Thoses two libraries are published under the GPL V3.0 license.
//
//******************
//
// License (LGPL V3.0) :
//
// Copyright (C) 2024 by Aster System, Inc. <https://aster-system.github.io/aster-system/>
// This file is part of SCLS.
// SCLS is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// SCLS is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with SCLS. If not, see <https://www.gnu.org/licenses/>.
//

#ifndef SCLS_IMAGE
#define SCLS_IMAGE

// Avoid some errors with libraries path
// SCLS Math
#ifndef SCLS_MATH_PATH
#ifdef __ASTER_DEV
#define SCLS_MATH_PATH "../scls-math-carl/scls_math.h"
#else
#define SCLS_MATH_PATH <scls_math.h>
#endif // __ASTER_DEV
#endif // SCLS_MATH_PATH

// Include SCLS Math
#include SCLS_MATH_PATH

// Font path of the Aster System's logo
#ifndef LOGO_FONT_PATH
// Before school PC : "C:\\Divers\\Fonts\\AlteHaasGroteskBold.ttf"
// Development PC : "E:\\Divers\\Fonts\\AlteHaasGroteskBold.ttf"
#define LOGO_FONT_PATH "E:\\Divers\\Fonts\\AlteHaasGroteskBold.ttf"
#endif // LOGO_FONT_PATH

// Define SCLS_INIT
#ifndef SCLS_IMAGE_INIT
    #define SCLS_IMAGE_INIT std::string scls::__base_font_path = std::string(BASE_FONT_PATH); std::string scls::__default_font = std::string(DEFAULT_FONT);
#endif // SCLS_IMAGE
#ifdef SCLS_INIT
#undef SCLS_INIT
#endif // SCLS_INIT
#ifndef SCLS_INIT
#define SCLS_INIT SCLS_IMAGE_INIT SCLS_MATH_INIT SCLS_FOUNDATION_INIT
#endif // SCLS_INIT

// Include the needed SCLS Image files
#include "scls_image_directory/scls_image_core.h"
#include "scls_image_directory/scls_image_text.h"

// Include some others SCLS Images files (in development)
#ifdef __ASTER_DEV
#include "scls_image_directory/scls_image_fractal.h"
#include "scls_image_directory/scls_image_math.h"
#endif // __ASTER_DEV

namespace scls {

    // Make the logo of Aster System
    inline std::shared_ptr<scls::Image> aster_system_logo(Color branch_1_color, Color branch_2_color, unsigned int image_width, std::string text_top, std::string text_bottom) {
        double total_width = static_cast<double>(image_width);

        // Datas about the branchs
        double branch_height = (total_width*(8.0/10.0));
        double branch_width = (total_width/14.0);
        double branch_x = (total_width/10.0);
        double branch_y = (total_width/10.0);
        // Automatic values
        double branch_separation_width = round(branch_width);
        double branch_height_round = round(branch_width);
        double branch_height_slide = round(branch_width + branch_separation_width);
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
        scls::Color color_1 = branch_1_color;
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

        // Create the branch 2
        scls::Color color_2 = branch_2_color;
        // Create the third rectangle of the left branch
        double ll3_height = branch_height_round;
        double ll3_width = branch_height_slide;
        double ll3_x = branch_x;
        double ll3_y = branch_y;
        // Create the first rectangle of the left branch
        double ll4_height = (branch_height_main);
        double ll4_width = (branch_width);
        double ll4_x = (ll3_x + ll3_width);
        double ll4_y = (ll3_y + ll3_height);

        // Configure the text
        int text_number = 0;
        if(text_bottom != ""){text_number++;}
        if(text_top != ""){text_number++;}

        // Create the text
        std::shared_ptr<scls::Text_Image_Generator> tig = std::make_shared<scls::Text_Image_Generator>();
        std::shared_ptr<scls::Image> text_bottom_image;
        std::shared_ptr<scls::Image> text_top_image;
        if(text_number > 0) {
            // Paste the text at the top
            scls::Text_Style style;
            style.set_color(color_text); style.set_font_path(LOGO_FONT_PATH); style.set_font_size(font_size);
            text_top_image = tig.get()->image_shared_ptr(text_top, style);
            if(text_number > 1) {
                // Paste the text at the bottom
                style.set_color(color_text);style.set_font_path(LOGO_FONT_PATH);style.set_font_size(font_size);
                text_bottom_image = tig.get()->image_shared_ptr(text_bottom, style);
            }
        }

        // Create the image
        int final_width = 0;
        if(text_bottom_image.get() != 0){final_width = text_bottom_image.get()->width();}
        if(text_top_image.get() != 0 && text_top_image.get()->width() > final_width) {final_width = text_top_image.get()->width();}
        final_width += text_x + branch_width;
        scls::Image* img = new scls::Image(static_cast<short>(final_width), static_cast<short>(total_width), scls::Color(255, 255, 255), SCLS_IMAGE_RGBA);

        // First branch
        img->fill_rect(ll1_x, ll1_y, ll1_width, ll1_height, color_1);
        img->fill_rect(ll2_x, ll2_y, ll2_width, ll2_height, color_1);
        img->fill_triangle(ll2_x, ll2_y, ll2_x, ll2_y - branch_height_slide, ll2_x + branch_height_slide, ll2_y, color_1);
        img->fill_triangle(ll1_x, ll1_y - branch_width, ll1_x, ll1_y, ll1_x + ll1_width, ll1_y, color_1);
        img->fill_circle(ll2_x, ll2_y, branch_height_round, color_1);

        // Second branch
        img->fill_rect(ll4_x, ll4_y, ll4_width, ll4_height, color_2);
        img->fill_rect(ll3_x, ll3_y, ll3_width, ll3_height, color_2);
        img->fill_triangle(ll3_x + ll3_width, ll3_y + ll3_height + branch_height_slide, ll3_x, ll3_y + ll3_height, ll3_x + ll3_width, ll3_y + ll3_height, color_2);
        img->fill_triangle(ll4_x + ll4_width, ll4_y + ll4_height + branch_width, ll4_x, ll4_y + ll4_height, ll4_x + ll4_width, ll4_y + ll4_height, color_2);
        img->fill_circle(ll4_x, ll3_y + ll3_height, ll4_width, color_2);

        // Text
        if(text_number == 1) {text_y = img->height() / 2.0 - text_top_image.get()->height() / 2.0;}
        else if(text_number == 2){text_y = 0;}
        if(text_top_image.get() != 0){img->paste(text_top_image.get(), text_x, text_y);}
        if(text_number == 2){text_y = branch_y + branch_height - text_top_image.get()->height();}
        if(text_bottom_image.get() != 0){img->paste(text_bottom_image.get(), text_x, text_y);}

        return std::shared_ptr<scls::Image>(img);
    }
    inline std::shared_ptr<scls::Image> aster_system_logo(unsigned int image_height = 1000){return aster_system_logo(scls::Color(0, 51, 102), scls::Color(0, 51, 102), image_height, "ASTER", "SYSTÈME");};
    inline std::shared_ptr<scls::Image> aster_system_logo(std::string single_text, unsigned int image_height = 1000){return aster_system_logo(scls::Color(0, 51, 102), scls::Color(0, 51, 102), image_height, single_text, "");};
    inline std::shared_ptr<scls::Image> aster_system_logo(std::string text_top, std::string text_bottom, unsigned int image_height = 1000){return aster_system_logo(scls::Color(0, 51, 102), scls::Color(0, 51, 102), image_height, text_top, text_bottom);};
};

#endif // SCLS_IMAGE
