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
    // Returns an image with a Mandelbrot fractal drawn on it
    inline Image* burning_ship_fractal(unsigned short width, unsigned short height) {
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

    // Returns an image with a Mandelbrot fractal drawn on it
    inline Image* mandelbrot_fractal(unsigned short width, unsigned short height) {
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

    // Returns an image with a modulo drawn on it
    inline Image* modulo_circle(unsigned short width, unsigned short height, unsigned int base, unsigned int side) {
        Image* img = new Image(width, height, 255, 255, 255);

        double max_x = 0.5;
		double min_x = 0.5;

		// Create each points of the group
		std::vector<double> x_positions = std::vector<double>();
		std::vector<double> y_positions = std::vector<double>();
		for (unsigned int i = 0; i < side; i++)
		{
			double theta = (2 * 3.1415 * i) / side + 3.1415 / 4.0;
			double x = cos(theta);
			double y = sin(theta);

			if (x > max_x) max_x = x;
			if (x < min_x) min_x = x;

			x_positions.push_back(x);
			y_positions.push_back(y);
		}

		double ratio = width * 0.45;

		for (int i = 0; i < static_cast<int>(x_positions.size()); i++)
		{
			x_positions[i] = x_positions[i] * ratio + width / 2.0;
		}

		for (int i = 0; i < static_cast<int>(y_positions.size()); i++)
		{
			y_positions[i] = y_positions[i] * ratio + height / 2.0;
		}

		for(int i = 0;i<static_cast<int>(x_positions.size() - 1);i++) {
            img->draw_line(x_positions[i], y_positions[i], x_positions[i + 1], y_positions[i + 1], Color(0, 0, 0, 0), 2);
		}
		img->draw_line(x_positions[x_positions.size() - 1], y_positions[x_positions.size() - 1], x_positions[0], y_positions[0], Color(0, 0, 0, 0), 2);

		for(int i = 0;i<static_cast<int>(x_positions.size());i++) {
            img->set_pixel(x_positions[i], y_positions[i], Color(0, 0, 0, 0), 5);
		}

		// Calculate the modulo
		for(unsigned int i = 0;i<side;i++) {
            unsigned int result = base * i;
            unsigned int pos = result % side;

            img->draw_line(x_positions[i], y_positions[i], x_positions[pos], y_positions[pos], Color(0, 0, 0, 0), 1);
		}

		return img;
    }

    // Returns an image with a Sierpinski carpet drawn on it
    inline Image* sierpinski_carpet(unsigned short width, unsigned short height, unsigned char step) {
        // Create the image
        Image* img = new Image(width, height, 255, 255, 255);

        if(step == 0) {
            img->draw_rect(0, 0, width, height, static_cast<double>(width) / 3.0, Color(0, 0, 0, 0));
        }
        else {
            Image *child = sierpinski_carpet(static_cast<unsigned short>(static_cast<double>(width) / 3.0), static_cast<unsigned short>(static_cast<double>(height) / 3.0), step - 1);
            img->paste(child, 0, 0);
            img->paste(child, (static_cast<double>(width) / 3.0), 0);
            img->paste(child, (static_cast<double>(width) / 3.0) * 2.0, 0);
            img->paste(child, 0, static_cast<double>(height) / 3.0);
            img->paste(child, (static_cast<double>(width) / 3.0) * 2.0, static_cast<double>(height) / 3.0);
            img->paste(child, 0, (static_cast<double>(height) / 3.0) * 2.0);
            img->paste(child, (static_cast<double>(width) / 3.0), (static_cast<double>(height) / 3.0) * 2.0);
            img->paste(child, (static_cast<double>(width) / 3.0) * 2.0, (static_cast<double>(height) / 3.0) * 2.0);
            delete child; child = 0;
        }

        return img;
    };

    // Returns an image with a Sierpinski pyramid drawn on it
    inline Image* sierpinski_pyramid(unsigned short width, unsigned short height, unsigned char step, char current_step = -1) {
        // Create the image
        Image* img = new Image(width, height, 255, 255, 255, 255);

        if(step == 0) {
            img->fill_triangle(0, height, width / 2.0, 0, width, height, Color(0, 0, 0, 0));
            if(current_step != -1) return img;
        }
        else {
            scls::Image* child = sierpinski_pyramid(width / 2.0, height / 2.0, step - 1, step - 1);
            img->paste(child, 0, height / 2.0);
            img->paste(child, width / 2.0, height / 2.0);
            img->paste(child, width / 4.0, 0);
            if(current_step != -1) return img;
        }

        return img;
    }
}
