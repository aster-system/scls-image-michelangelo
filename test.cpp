#include "scls_image.h"

using namespace std;

#ifndef LOGO_FONT_PATH
#define LOGO_FONT_PATH "E:\\Divers\\Fonts\\AlteHaasGroteskBold.ttf"
#endif // LOGO_FONT_PATH

#include <random>

// Make the logo of Aster System
scls::Image* aster_system_logo() {
    short total_width = 1000;
    scls::Image* img = new scls::Image(total_width, total_width, scls::white);

    short l_width = 25;
    short l_base_length = 200;
    short l_height = 700;

    short x_first_l = 50;
    short y_first_l = 100;

    short x_first_2 = x_first_l + 75;
    short y_first_2 = y_first_l + 115;

    scls::Color l_color(0, 51, 102);

    // First L
    img->fill_rect(x_first_l, y_first_l, l_base_length, l_width, l_color);
    img->fill_rect((x_first_l + l_base_length) - l_width, y_first_l + l_width, l_width, l_height, l_color);

    // Second L
    img->fill_rect(x_first_2, y_first_2, l_width, l_height, l_color);
    img->fill_rect(x_first_2 + l_width, (y_first_2 + l_height) - l_width, l_base_length, l_width, l_color);

    // Generate the text
    short text_x = (x_first_2 + l_width + l_base_length) - 75;
    short text_y = y_first_2 + 75;
    short text_y_separation = 50;
    scls::Text_Image_Data datas; datas.font_size = 200; datas.font.font_family = "Aldo the Apache"; datas.font.font_path = LOGO_FONT_PATH;
    datas.blue = l_color.blue(); datas.green = l_color.green(); datas.red = l_color.red();
    scls::Image* scls_text = scls::text_image("SCLS", datas);
    img->paste(scls_text, text_x, text_y); text_y += scls_text->height() + text_y_separation; delete scls_text; scls_text = 0;
    datas.font_size = 130;
    scls_text = scls::text_image("Foundation", datas);
    img->paste(scls_text, text_x, text_y); text_y += scls_text->height() + text_y_separation; delete scls_text; scls_text = 0;
    datas.font_size = 130;
    scls_text = scls::text_image("\"Leonhard\"", datas);
    img->paste(scls_text, text_x, text_y); text_y += scls_text->height() + text_y_separation; delete scls_text; scls_text = 0;

    return img;
}

int main()
{
    scls::Image* img = aster_system_logo();
    img->save_png("logo.png");

    return 0;
}
