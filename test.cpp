#include "scls_image.h"

int main() {
    long long current = scls::time_ns();
    unsigned int iter = 10;
    scls::Image* image_test = new scls::Image(3840, 2160, scls::Color(25, 145, 68));
    for(unsigned int i = 0;i<iter;i++) { image_test->fill(scls::Color(25, 145, 68)); }
    long long final_time = scls::time_ns();
    std::cout << "Time : " << static_cast<double>(final_time - current) / 1000000000.0 << std::endl;
    image_test->save_png("test.png"); delete image_test; image_test = 0;
    // With 1 threads : 1 creation = 0.25
    // With 10 threads : 10 creation = 0.6
    // With 50 threads : 10 creation = 0.6

    // scls::save_leclerc_text("text.png");
    return 0;
}
