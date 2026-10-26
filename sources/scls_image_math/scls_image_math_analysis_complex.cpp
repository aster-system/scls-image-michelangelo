//******************
//
// scls_image_math_analysis_complex.cpp
//
//******************
// Presentation :
//
// SCLS is a project containing base functions for C++.
// It can also be use in any projects.
//
// The Math "Carl" part represents the mathematical part of SCLS.
// It is named after one one of the greatest mathematician of all times, Carl Freiderich Gauss.
//
// This file contains the source code of "scls_image_math_analysis_complex.h".
//
//******************
//
// License (LGPL V3.0) :
//
// Copyright (C) 2024 by Aster System, Inc. <https://aster-system.github.io/aster-system/>
// This file is part of SCLS.
// SCLS is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// SCLS is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with SCLS. If not, see <https://www.gnu.org/licenses/>.
//

// Include SCLS Image bigger header
#include "../../scls_image_directory/scls_image_math/scls_image_math_analysis_complex.h"

namespace scls {
    // Represent the argument / module of a complex function on an image
    Image function_complex_to_argument_image(Complex (*complex_function)(Complex), int image_width, int image_height) {
        // Image datas
        Color_Mixer mixer = Color_Mixer({Color(255, 0, 0), Color(0, 255, 0), Color(0, 0, 255)});

        Image img = Image(image_width, image_height, Color(255, 255, 255));

        // Datas
        double start_x = -5;double start_y = -5;
        double real_height = 10;double real_width = 10;
        int width = image_width;int height= image_height;

        // Get the values
        std::vector<std::vector<Point_3D>> points = std::vector<std::vector<Point_3D>>(width, std::vector<Point_3D>(height, Point_3D(0, 0, 0)));
        std::vector<std::vector<std::shared_ptr<model_maker::Point>>> points_model = std::vector<std::vector<std::shared_ptr<model_maker::Point>>>(width, std::vector<std::shared_ptr<model_maker::Point>>(height, std::shared_ptr<model_maker::Point>()));
        std::vector<std::vector<double>> values = std::vector<std::vector<double>>(width, std::vector<double>(height, 0));
        for(int x = 0;x<width;x++) {
            for(int y = 0;y<height;y++) {
                // Get the position
                double real_x = start_x + real_width * (static_cast<double>(x) / static_cast<double>(values.size()));
                double real_y = start_y + real_height * (static_cast<double>(y) / static_cast<double>(values.at(x).size()));

                // Get the value and the point
                Complex c = Complex(real_x, real_y);
                //c = ((c - Complex(4, 0)) * (c - Complex(-2, 0)) * (c - Complex(0, 3)) * (c - Complex(0, -1))) * 0.05;
                //c = ((c * c * Complex(-2, 2)) * (c * c * Complex(-1)) * (c - Complex(2) * Complex(0, 1)) * (c - Complex(2, 0) * Complex(0, 1))) / (c * c + Complex(2, 2));
                //double current_value = c.argument().to_double();
                //double current_value = f.get()->value_to_double("x", c);
                c = (*complex_function)(c);
                double current_argument = c.argument().to_double();

                // Other datas about the point
                Color current_color = mixer.current_color((current_argument + pi) / (pi * 2.0));
                img.set_pixel(x, y, current_color);
            }
        }

        return img;
    }
    Image function_complex_to_module_image(Complex (*complex_function)(Complex), int image_width, int image_height) {
        // Module calculation datas
        double value_max = 100;

        // Image datas
        Color_Mixer mixer = Color_Mixer({Color(255, 0, 0), Color(0, 255, 0), Color(0, 0, 255)});

        Image img = Image(image_width, image_height, Color(255, 255, 255));

        // Datas
        double start_x = -5;double start_y = -5;
        double real_height = 10;double real_width = 10;
        int width = image_width;int height= image_height;

        // Get the values
        std::vector<std::vector<Point_3D>> points = std::vector<std::vector<Point_3D>>(width, std::vector<Point_3D>(height, Point_3D(0, 0, 0)));
        std::vector<std::vector<std::shared_ptr<model_maker::Point>>> points_model = std::vector<std::vector<std::shared_ptr<model_maker::Point>>>(width, std::vector<std::shared_ptr<model_maker::Point>>(height, std::shared_ptr<model_maker::Point>()));
        std::vector<std::vector<double>> values = std::vector<std::vector<double>>(width, std::vector<double>(height, 0));
        for(int x = 0;x<width;x++) {
            for(int y = 0;y<height;y++) {
                // Get the position
                double real_x = start_x + real_width * (static_cast<double>(x) / static_cast<double>(values.size()));
                double real_y = start_y + real_height * (static_cast<double>(y) / static_cast<double>(values.at(x).size()));

                // Get the value and the point
                Complex c = Complex(real_x, real_y);
                //c = ((c - Complex(4, 0)) * (c - Complex(-2, 0)) * (c - Complex(0, 3)) * (c - Complex(0, -1))) * 0.05;
                //c = ((c * c * Complex(-2, 2)) * (c * c * Complex(-1)) * (c - Complex(2) * Complex(0, 1)) * (c - Complex(2, 0) * Complex(0, 1))) / (c * c + Complex(2, 2));
                //double current_value = c.argument().to_double();
                //double current_value = f.get()->value_to_double("x", c);
                c = (*complex_function)(c);
                double current_module = c.module().to_double();
                current_module = (1.0 - std::atan(current_module / 25.0) / (pi / 2.0)) * 66.0;

                // Other datas about the point
                Color current_color = mixer.current_color(current_module / value_max);
                img.set_pixel(x, y, current_color);
            }
        }

        return img;
    }
}
