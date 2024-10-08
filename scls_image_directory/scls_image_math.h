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
// This file contains some thing to use properly the "SCLS Math Karl" library.
//

#include "scls_image_text.h"

// The namespace "scls" is used to simplify the all.
namespace scls {

    //*********
	//
	// Function drawer
	//
	//*********

    class Function_Drawer {
        // Class representating a drawer of function

    public:
        // Function drawer class
        Function_Drawer(){};

        // Returns an image position from a complex position
        inline Complex complex_position_to_image_position(Complex number) const {
            int x = std::round(((number.real() - start_x()) * (scls::Fraction(image_width()) / width_x())).to_double());
            int y = image_height() - std::round(((number.imaginary() - start_y()) * (scls::Fraction(image_height()) / width_y())).to_double());
            return Complex(x, y);
        };
        // Execute the function with some parameter of the function
        Complex function_result(Complex before) const {
            // Get the needed value
            scls::Complex after = (before - scls::Complex(1))/(before + scls::Complex(0, 1));
            return after;
        };
        // Return an image of the function
        std::shared_ptr<Image> image() {
            // Create the image
            std::shared_ptr<scls::Image> img = std::make_shared<scls::Image>(image_width(), image_height(), scls::Color(255, 255, 255));

            // Do pixel by pixel
            for(double i = 0;i<image_height();i++) {
                for(double j = 0;j<image_width();j++) {
                    scls::Complex c = image_position_to_complex_position(i, j);
                    scls::Complex value = function_result(c);

                    // Draw the point
                    Complex point = complex_position_to_image_position(value);
                    int x_final = point.real().to_double();
                    int y_final = point.imaginary().to_double();
                    if(x_final < 0 || y_final < 0 || x_final >= image_width() || y_final >= image_height()) { continue;
                        //img.get()->set_pixel(i, j, scls::Color(x_final * (256.0 / image_width()), y_final * (256.0 / image_height()), 0));
                    } else {
                        //img.get()->set_pixel(x_final, y_final, scls::Color(0, 0, 0));
                    }
                    //img.get()->set_pixel(i, j, scls::Color(x_final * (256.0 / image_width()), y_final * (256.0 / image_height()), 0));
                    img.get()->set_pixel(x_final, y_final, scls::Color(i * (256.0 / image_width()), j * (256.0 / image_height()), 0));
                }
            }

            // Trace the X and Y axis
            Complex pos_0 = complex_position_to_image_position(Complex(0, 0));
            img.get()->fill_rect(pos_0.real().to_double() - 1, 0, 3, image_height(), scls::Color(255, 0, 0));
            img.get()->fill_rect(0, pos_0.imaginary().to_double() - 1, image_width(), 3, scls::Color(255, 0, 0));

            // Return the image
            return img;
        };
        // Convert an image position to a complex position
        inline Complex image_position_to_complex_position(int image_x, int image_y) const {
            scls::Fraction x = start_x() + width_x() * (scls::Fraction(1, image_width()) * image_x);
            scls::Fraction y = start_y() + width_y() * (scls::Fraction(1, image_height()) * image_y);
            return Complex(x, y);
        };

        // Getters and setters
        inline unsigned int image_height() const {return a_image_height;}
        inline unsigned int image_width() const {return a_image_width;}
        inline Fraction start_x() const noexcept {return a_start_x;};
        inline Fraction start_y() const noexcept {return a_start_y;};
        inline Fraction width_x() const noexcept {return a_width_x;};
        inline Fraction width_y() const noexcept {return a_width_y;};

    private:

        // Images datas
        unsigned int a_image_height = 2000;
        unsigned int a_image_width = 2000;

        // Current limits datas
        Fraction a_start_x = Fraction(-2);
        Fraction a_start_y = Fraction(-2);
        Fraction a_width_x = Fraction(4);
        Fraction a_width_y = Fraction(4);
    };
}
