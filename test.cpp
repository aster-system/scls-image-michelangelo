#include "scls_image.h"

using namespace std;

#ifndef LOGO_FONT_PATH
// Before school PC : "C:\\Divers\\Fonts\\AlteHaasGroteskBold.ttf"
// Development PC : "E:\\Divers\\Fonts\\AlteHaasGroteskBold.ttf"
#define LOGO_FONT_PATH "E:\\Divers\\Fonts\\AlteHaasGroteskBold.ttf"
#endif // LOGO_FONT_PATH

#include <random>

// Make the logo of SCLS
scls::Image* scls_logo(unsigned short width, std::string name, std::string person, unsigned short name_font_size, unsigned short person_font_size, scls::Color person_color) {
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
    short text_x = (x_first_2 + l_width + l_base_length) - static_cast<short>((75.0/1000.0) * total_width);
    short text_y = y_first_2 + static_cast<short>((75.0/1000.0) * total_width);
    short text_y_separation = static_cast<short>(total_width/20.0);
    scls::Text_Image_Data datas; datas.font_size = static_cast<short>(total_width/5.0); datas.font.font_family = "Alte Haas Grotesk"; datas.font.font_path = LOGO_FONT_PATH;
    datas.blue = color_1.blue(); datas.green = color_1.green(); datas.red = color_1.red();
    scls::Image* scls_text = scls::text_image("SCLS", datas);
    img->paste(scls_text, text_x, text_y); text_y += scls_text->height() + text_y_separation; delete scls_text; scls_text = 0;
    datas.blue = color_2.blue(); datas.green = color_2.green(); datas.red = color_2.red();
    datas.font_size = name_font_size;
    scls_text = scls::text_image(name, datas);
    img->paste(scls_text, text_x, text_y); text_y += scls_text->height() + text_y_separation; delete scls_text; scls_text = 0;
    datas.blue = color_3.blue(); datas.green = color_3.green(); datas.red = color_3.red();
    datas.font_size = person_font_size;
    scls_text = scls::text_image(person, datas);
    img->paste(scls_text, text_x, text_y); text_y += scls_text->height() + text_y_separation; delete scls_text; scls_text = 0;

    return img;
}

// Make the logo of SCLS easily
scls::Image* scls_logo(unsigned short total_width, unsigned int to_draw = 0) {
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
scls::Image* aster_system_logo() {
    double total_width = 5000;
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
    scls::Color color_3(253,174,0);

    // First L
    img->fill_rect(x_first_l, y_first_l, l_base_length, l_width, color_1);
    img->fill_rect((x_first_l + l_base_length) - l_width, y_first_l + l_width, l_width, l_height, color_1);

    // Second L
    img->fill_rect(x_first_2, y_first_2, l_width, l_height, color_1);
    img->fill_rect(x_first_2 + l_width, (y_first_2 + l_height) - l_width, l_base_length, l_width, color_1);

    // Draw the Aster system logo
    short text_x = (x_first_2 + l_width + l_base_length) - static_cast<short>((75.0/1000.0) * total_width);
    short text_y = y_first_2 + static_cast<short>((75.0/1000.0) * total_width);
    short text_y_separation = static_cast<short>(total_width/20.0);
    scls::Text_Image_Data datas; datas.font_size = static_cast<short>(total_width/5.0); datas.font.font_family = "Alte Haas Grotesk"; datas.font.font_path = LOGO_FONT_PATH;
    datas.blue = color_1.blue(); datas.green = color_1.green(); datas.red = color_1.red();
    scls::Image* scls_text = scls::text_image("Aster", datas);
    img->paste(scls_text, text_x, text_y); text_y += scls_text->height() + text_y_separation; delete scls_text; scls_text = 0;
    datas.blue = color_2.blue(); datas.green = color_2.green(); datas.red = color_2.red();
    datas.font_size = static_cast<short>(total_width/5.0);
    scls_text = scls::text_image("System", datas);
    img->paste(scls_text, text_x, text_y); text_y += scls_text->height() + text_y_separation; delete scls_text; scls_text = 0;

    return img;
}

int main()
{
    scls::Text_Image_Generator* image_generator = new scls::Text_Image_Generator();
    // image_creator->set_global_out_offset_width_bottom(50);
    // image_creator->set_global_out_offset_width_left(50);
    // image_creator->set_global_out_offset_width_right(50);
    // image_creator->set_global_out_offset_width_top(50);
    image_generator->save_image("text.png", "<h1> <b> Le char <i> Leclerc </i> </b> </h1></br>Le char Leclerc est un char de combat de</br>conception française, construit par Nexter.");
    delete image_generator; image_generator = 0;

    return 0;
}
