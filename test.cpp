#include "scls_image.h"
#include <sys/time.h>

int main() {
    timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    long long current = static_cast<long long>(ts.tv_nsec) + static_cast<long long>(ts.tv_sec) * 1000000000;
    for(int i = 0;i<1;i++) {
        scls::Image* image_test = new scls::Image(3840, 2160, scls::Color(25, 145, 68));
        delete image_test; image_test = 0;
    }
    clock_gettime(CLOCK_REALTIME, &ts);
    long long final_time = static_cast<long long>(ts.tv_nsec) + static_cast<long long>(ts.tv_sec) * 1000000000;
    std::cout << "Time : " << static_cast<double>(final_time - current) / 1000000000.0 << " " << final_time << " " << current << " " << ts.tv_sec << std::endl;
    // Without thread : 1 creation = 0.25

    scls::save_leclerc_text("text.png");
    return 0;
}
