//******************
//
// scls_image_fractal.h
//
//******************
// Presentation :
//
// SCLS is a project containing base functions for C++.
// It can also be use in any projects.
//
// The Image "Michelangelo" part represents a lot of usefull datas manipulation for images.
// It is named after one of the greatest painter of all time, Michelangelo.
//
// This file is a test file, using fractals to test the images.
//

#include "scls_image_core.h"

// The namespace "scls" is used to simplify the all.
namespace scls
{
    // Return an image with a Mandelbrot fractal drawn on it
    Image* burning_ship_fractal(unsigned short width, unsigned short height) {
        // Define the size of the fractale
        double min_x = -2.5;
        double min_y = -2.5;
        double width_fractal = 5;
        double height_fractal = 5;

        // Create the image
        Image* img = new Image(width, height, 255, 255, 255);

        // Draw the fractal pixel by pixel
        for(int i = 0;i<width;i++) {
            for(int j = 0;j<height;j++) {
                // Define each needed complex
                double c_imaginary = min_y + (j / static_cast<double>(height)) * height_fractal;
                double c_real = min_x + (i / static_cast<double>(width)) * width_fractal;
                double z_imaginary = 0;
                double z_real = 0;

                // Define the needed variables to test the function
                unsigned char it = 0;
                unsigned char max_iteration = 20;
                unsigned char max_size = 100;

                // Do the function
                while(it < max_iteration && sqrt(z_imaginary * z_imaginary + z_real * z_real) < max_size) {
                    double temp_real = z_real;
                    z_real = abs(temp_real * temp_real - z_imaginary * z_imaginary - 1 + c_real);
                    z_imaginary = abs(2 * z_imaginary * temp_real + c_imaginary);

                    it++;
                }

                // Draw the pixel
                if(it == max_iteration) {
                    img->set_pixel(i, j, 0, 0, 0);
                }
                else {
                    img->set_pixel(i, j, (static_cast<double>(it) / static_cast<double>(max_iteration)) * 255.0, (static_cast<double>(it) / static_cast<double>(max_iteration)) * 255.0, 255 - (static_cast<double>(it) / static_cast<double>(max_iteration)) * 255.0);
                }
            }
        }

        // Return the image
        return img;
    };

    // Return an image with a Mandelbrot fractal drawn on it
    Image* mandelbrot_fractal(unsigned short width, unsigned short height) {
        // Define the size of the fractale
        double min_x = -2.5;
        double min_y = -2.5;
        double width_fractal = 5;
        double height_fractal = 5;

        // Create the image
        Image* img = new Image(width, height, 255, 255, 255);

        // Draw the fractal pixel by pixel
        for(int i = 0;i<width;i++) {
            for(int j = 0;j<height;j++) {
                // Define each needed complex
                double c_imaginary = min_y + (j / static_cast<double>(height)) * height_fractal;
                double c_real = min_x + (i / static_cast<double>(width)) * width_fractal;
                double z_imaginary = 0;
                double z_real = 0;

                // Define the needed variables to test the function
                unsigned char it = 0;
                unsigned char max_iteration = 20;
                unsigned char max_size = 100;

                // Do the function
                while(it < max_iteration && sqrt(z_imaginary * z_imaginary + z_real * z_real) < max_size) {
                    double temp_real = z_real;
                    z_real = temp_real * temp_real - z_imaginary * z_imaginary - 1 + c_real;
                    z_imaginary = 2 * z_imaginary * temp_real + c_imaginary;

                    it++;
                }

                // Draw the pixel
                if(it == max_iteration) {
                    img->set_pixel(i, j, 0, 0, 0);
                }
                else {
                    img->set_pixel(i, j, (static_cast<double>(it) / static_cast<double>(max_iteration)) * 255.0, (static_cast<double>(it) / static_cast<double>(max_iteration)) * 255.0, 255 - (static_cast<double>(it) / static_cast<double>(max_iteration)) * 255.0);
                }
            }
        }

        // Return the image
        return img;
    };
}
