#include "scls_image.h"

using namespace std;

#include <random>

int main()
{
    scls::Image* img = scls::sierpinski_pyramid(1024, 1024, 7);
    img->save_png("sierpinski_pyramid.png");

    return 0;
}
