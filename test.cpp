#include "scls_image.h"

using namespace std;

#ifndef LOGO_FONT_PATH
// Before school PC : "C:\\Divers\\Fonts\\AlteHaasGroteskBold.ttf"
// Development PC : "E:\\Divers\\Fonts\\AlteHaasGroteskBold.ttf"
#define LOGO_FONT_PATH "C:\\Divers\\Fonts\\AlteHaasGroteskBold.ttf"
#endif // LOGO_FONT_PATH

#include <random>

// Make the logo of Aster System
scls::Image* aster_system_logo(unsigned int to_draw = 0) {
    double total_width = 2000;
    scls::Image* img = new scls::Image(static_cast<short>(total_width), static_cast<short>(total_width), scls::white);

    short l_width = static_cast<short>(total_width/40.0);
    short l_base_length = static_cast<short>(total_width/5.0);
    short l_height = static_cast<short>(total_width*(7.0/10.0));

    short x_first_l = static_cast<short>(total_width/20.0);
    short y_first_l = static_cast<short>(total_width/10.0);

    short x_first_2 = x_first_l + static_cast<short>((75.0/1000.0) * total_width);
    short y_first_2 = y_first_l + static_cast<short>((115.0/1000.0) * total_width);

    scls::Color l_color(0, 51, 102);

    // First L
    img->fill_rect(x_first_l, y_first_l, l_base_length, l_width, l_color);
    img->fill_rect((x_first_l + l_base_length) - l_width, y_first_l + l_width, l_width, l_height, l_color);

    // Second L
    img->fill_rect(x_first_2, y_first_2, l_width, l_height, l_color);
    img->fill_rect(x_first_2 + l_width, (y_first_2 + l_height) - l_width, l_base_length, l_width, l_color);

    // Chose which logo to draw
    if(to_draw == 0) {
        // Draw the Aster System logo
        short text_x = (x_first_2 + l_width + l_base_length) - static_cast<short>((75.0/1000.0) * total_width);
        short text_y = static_cast<short>(total_width / 2.0 - (200.0/1000.0) * total_width);
        short text_y_separation = static_cast<short>(total_width/20.0);
        scls::Text_Image_Data datas; datas.font_size = static_cast<short>((200.0/1000.0) * total_width); datas.font.font_family = "Alte Haas Grotesk"; datas.font.font_path = LOGO_FONT_PATH;
        datas.blue = l_color.blue(); datas.green = l_color.green(); datas.red = l_color.red();
        scls::Image* scls_text = scls::text_image("Aster", datas);
        img->paste(scls_text, text_x, text_y); text_y += scls_text->height() + text_y_separation; delete scls_text; scls_text = 0;
        datas.font_size = static_cast<short>((200.0/1000.0) * total_width);
        scls_text = scls::text_image("System", datas);
        img->paste(scls_text, text_x, text_y); text_y += scls_text->height() + text_y_separation; delete scls_text; scls_text = 0;
    }
    else if(to_draw == 1) {
        // Draw the SCLS logo
        short text_x = (x_first_2 + l_width + l_base_length) - static_cast<short>((75.0/1000.0) * total_width);
        short text_y = y_first_2 + static_cast<short>((75.0/1000.0) * total_width);
        short text_y_separation = static_cast<short>(total_width/20.0);
        scls::Text_Image_Data datas; datas.font_size = static_cast<short>(total_width/5.0); datas.font.font_family = "Alte Haas Grotesk"; datas.font.font_path = LOGO_FONT_PATH;
        datas.blue = l_color.blue(); datas.green = l_color.green(); datas.red = l_color.red();
        scls::Image* scls_text = scls::text_image("SCLS", datas);
        img->paste(scls_text, text_x, text_y); text_y += scls_text->height() + text_y_separation; delete scls_text; scls_text = 0;
        datas.font_size = static_cast<short>((130.0/1000.0) * total_width);
        scls_text = scls::text_image("Foundation", datas);
        img->paste(scls_text, text_x, text_y); text_y += scls_text->height() + text_y_separation; delete scls_text; scls_text = 0;
        datas.font_size = static_cast<short>((130.0/1000.0) * total_width);
        scls_text = scls::text_image("\"Leonhard\"", datas);
        img->paste(scls_text, text_x, text_y); text_y += scls_text->height() + text_y_separation; delete scls_text; scls_text = 0;
    }

    return img;
}

int main()
{
    scls::Image* img = aster_system_logo(0);
    img->save_png("logo.png");

    return 0;
}
