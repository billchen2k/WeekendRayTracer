#include <iostream>
#include <fstream>
#include "color.h"

int main() {
    const int image_width = 256;
    const int image_height = 256;

    std::ofstream fout("out/image.ppm");

    fout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(float(i) / (image_width - 1),
                              double(j) / (image_height - 1), 0.25);
            write_color(fout, pixel_color);
        }
    }

    fout.close();
    std::cerr << "\nDone.\n";
    return 0;
}