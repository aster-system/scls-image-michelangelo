#include "scls_image.h"

using namespace std;

#include <random>

int main()
{
    scls::Image* img = scls::burning_ship_fractal(500, 500);
    img->save_png("mandelbrot.png");

    return 0;
}
