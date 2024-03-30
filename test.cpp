#include "scls_image.h"

using namespace std;

#include <random>

int main()
{
    scls::Image* img = scls::mandelbrot_fractal(1000, 1000);
    img->save_png("mandelbrot.png");

    return 0;
}
