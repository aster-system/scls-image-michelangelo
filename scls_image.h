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
    // Make the logo of SCLS
    inline scls::Image* scls_logo(unsigned short width, std::string name, std::string person, unsigned short name_font_size, unsigned short person_font_size, scls::Color person_color) {
        double total_width = static_cast<double>(width);
        scls::Image* img = new scls::Image(static_cast<short>(total_width), static_cast<short>(total_width), scls::white);

        short l_width = static_cast<short>(total_width/40.0);
        short l_base_length = static_cast<short>(total_width/5.0);
        short l_height = static_cast<short>(total_width*(7.0/10.0));

        short x_first_l = static_cast<short>(total_width/20.0);
        short y_first_l = static_cast<short>(total_width/10.0);

        short x_first_2 = x_first_l + static_cast<short>((75.0/1000.0) * total_width);
        short y_first_2 = y_first_l + static_cast<short>((115.0/1000.0) * total_width);

        scls::Color color_1(0, 51, 102);
        scls::Color color_2(0, 51, 102);
        scls::Color color_3 = person_color;

        // First L
        img->fill_rect(x_first_l, y_first_l, l_base_length, l_width, color_1);
        img->fill_rect((x_first_l + l_base_length) - l_width, y_first_l + l_width, l_width, l_height, color_1);

        // Second L
        img->fill_rect(x_first_2, y_first_2, l_width, l_height, color_1);
        img->fill_rect(x_first_2 + l_width, (y_first_2 + l_height) - l_width, l_base_length, l_width, color_1);

        // Draw the SCLS logo
        scls::Text_Image_Generator* generator = new scls::Text_Image_Generator();
        short text_x = (x_first_2 + l_width + l_base_length) - static_cast<short>((75.0/1000.0) * total_width);
        short text_y = y_first_2 + static_cast<short>((75.0/1000.0) * total_width);
        short text_y_separation = static_cast<short>(total_width/20.0);
        scls::Text_Image_Data datas; datas.font_size = static_cast<short>(total_width/5.0); datas.font.font_family = "Alte Haas Grotesk"; datas.font.font_path = LOGO_FONT_PATH;
        datas.blue = color_1.blue(); datas.green = color_1.green(); datas.red = color_1.red();
        scls::Image* scls_text = generator->image("SCLS");
        img->paste(scls_text, text_x, text_y); text_y += scls_text->height() + text_y_separation; delete scls_text; scls_text = 0;
        datas.blue = color_2.blue(); datas.green = color_2.green(); datas.red = color_2.red();
        datas.font_size = name_font_size;
        scls_text = generator->image(name);
        img->paste(scls_text, text_x, text_y); text_y += scls_text->height() + text_y_separation; delete scls_text; scls_text = 0;
        datas.blue = color_3.blue(); datas.green = color_3.green(); datas.red = color_3.red();
        datas.font_size = person_font_size;
        scls_text = generator->image(person);
        img->paste(scls_text, text_x, text_y); text_y += scls_text->height() + text_y_separation; delete scls_text; scls_text = 0;
        delete generator;

        return img;
    }

    // Make the logo of SCLS easily
    inline scls::Image* scls_logo(unsigned short total_width, unsigned int to_draw = 0) {
        scls::Color color(255, 174, 0);
        std::string name = "Foundation";
        std::string person = "\"Leonhard\"";
        unsigned short name_font_size = static_cast<unsigned short>((130.0/1000.0) * total_width);
        unsigned short person_font_size = static_cast<unsigned short>((130.0/1000.0) * total_width);

        if(to_draw == 1) {
            name = "Documentalist";
            person = "\"Agatha\"";

            name_font_size = static_cast<unsigned short>((110.0/1000.0) * total_width);
            person_font_size = static_cast<unsigned short>((130.0/1000.0) * total_width);

            color = scls::Color(128, 0, 0);
        }

        return scls_logo(total_width, name, person, name_font_size, person_font_size, color);
    }

    // Make the logo of Aster System
    inline std::shared_ptr<scls::Image> aster_system_logo() {
        double total_width = 500;
        std::shared_ptr<scls::Image> img_shared_ptr = std::make_shared<scls::Image>(static_cast<short>(total_width), static_cast<short>(total_width), scls::white);
        Image* img = img_shared_ptr.get();

        short l_width = static_cast<short>(total_width/40.0);
        short l_base_length = static_cast<short>(total_width/5.0);
        short l_height = static_cast<short>(total_width*(7.0/10.0));

        short x_first_l = static_cast<short>(total_width/20.0);
        short y_first_l = static_cast<short>(total_width/10.0);

        short x_first_2 = x_first_l + static_cast<short>((75.0/1000.0) * total_width);
        short y_first_2 = y_first_l + static_cast<short>((115.0/1000.0) * total_width);

        scls::Color color_1(0, 51, 102);
        scls::Color color_2(0, 51, 102);
        scls::Color color_3(253,174,0);

        // First L
        img->fill_rect(x_first_l, y_first_l, l_base_length, l_width, color_1);
        img->fill_rect((x_first_l + l_base_length) - l_width, y_first_l + l_width, l_width, l_height, color_1);

        // Second L
        img->fill_rect(x_first_2, y_first_2, l_width, l_height, color_1);
        img->fill_rect(x_first_2 + l_width, (y_first_2 + l_height) - l_width, l_base_length, l_width, color_1);

        // Draw the Aster system logo
        std::shared_ptr<scls::Text_Image_Generator> generator = std::make_shared<scls::Text_Image_Generator>();
        short text_x = (x_first_2 + l_width + l_base_length) - static_cast<short>((75.0/1000.0) * total_width);
        short text_y = y_first_2 + static_cast<short>((75.0/1000.0) * total_width);
        short text_y_separation = static_cast<short>(total_width/20.0);
        scls::Text_Style datas;
        datas.font_size = static_cast<short>(total_width/5.0);
        datas.font.font_family = "Alte Haas Grotesk";
        datas.font.font_path = LOGO_FONT_PATH;
        datas.color = color_1;
        std::shared_ptr<scls::Image> scls_text = generator->image_shared_ptr("Aster", datas);
        img->paste(scls_text.get(), text_x, text_y); text_y += scls_text.get()->height() + text_y_separation;
        datas.color = color_2;
        datas.font_size = static_cast<short>(total_width/5.0);
        scls_text = generator->image_shared_ptr("System", datas);
        img->paste(scls_text.get(), text_x, text_y); text_y += scls_text.get()->height() + text_y_separation;

        return img_shared_ptr;
    }

    // Make the text about the Leclerc
    inline void save_leclerc_text(std::string path) {
        scls::Text_Image_Generator* image_generator = new scls::Text_Image_Generator();
        /* scls::Text_Image* leclerc = image_generator->new_text_image(scls::to_utf_8(scls::read_file("leclerc.html")));
        leclerc->global_style().max_width = 500;
        leclerc->set_use_cursor(true);
        leclerc->save_image(path);
        delete leclerc; leclerc = 0; //*/
        delete image_generator; image_generator = 0;
    }
};

#endif // SCLS_IMAGE
