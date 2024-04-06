#include "scls_image.h"

using namespace std;

#include <random>

scls::Image* aster_system_logo() {
    scls::Image* img = new scls::Image(1000, 1000, scls::white);

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

    return img;
}

int main()
{
    scls::Image* img = scls::sierpinski_pyramid(1024, 1024, 7);
    img->save_png("sierpinski_pyramid.png");

    delete img;
    img = scls::text_image("I want to be forever young");
    img->save_png("text.png");

    return 0;
}
