//******************
//
// scls_image_math_field.cpp
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
// This file contains the source code of "scls_image_math_field.h".
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

#include "../../scls_image_directory/scls_image_math/scls_image_math_field.h"

// The namespace "scls" is used to simplify the all.
namespace scls {
	// Point in a grid
	std::vector<scls::Point_2D> points_grid(int image_width, int image_height) {
        // Datas
        int start_x_pixel = 40;
        int start_y_pixel = 40;
        int step_x_pixel = 80;
        int step_y_pixel = 80;
        std::list<scls::Point_2D> arrow_starts_list;
        std::size_t arrow_starts_list_n = 0;

        // Browse the image
        int current_x_pixel = start_x_pixel;
        int current_y_pixel = start_y_pixel;
        while(current_y_pixel < image_height) {
            while(current_x_pixel < image_width) {
            	arrow_starts_list.push_back(scls::Point_2D(current_x_pixel, current_y_pixel));
            	arrow_starts_list_n++;
            	current_x_pixel += step_x_pixel;
            }
            current_x_pixel = start_x_pixel;
            current_y_pixel += step_y_pixel;
        }

        // List to vector
        std::vector<scls::Point_2D> arrow_starts = std::vector<scls::Point_2D>(arrow_starts_list_n);
        for(std::size_t i = 0;i<arrow_starts_list_n;i++){arrow_starts[i] = arrow_starts_list.front();arrow_starts_list.pop_front();}
        return arrow_starts;
	}

	// Do a colored scalar field
    void scalar_field_2d_colored(scls::Image image, scls::Formula_Base* formula, scls::Plane_Base* base) {
        // Datas

        // Browse the image
        for(int y_pixel = 0;y_pixel<image.height();y_pixel++) {
            for(int x_pixel = 0;x_pixel<image.width();x_pixel++) {
                // Datas for this pixel
                scls::Fraction x = base->canonical_x_to_base_x(x_pixel);
                scls::Fraction y = base->canonical_y_to_base_y(image.height() - y_pixel);

                scls::Fraction current_value = *formula->replace_unknowns("x", x).get()->replace_unknowns("y", y).get()->value<scls::Fraction>();
                if(current_value > 0 ){image.set_pixel(x_pixel, y_pixel, scls::Color(100.0 * current_value.to_double(), 0, 0));}
                else{image.set_pixel(x_pixel, y_pixel, scls::Color(0, 0, 100.0 * -current_value.to_double()));}
            }
        }
    }

    // Do an arrowed vector field
    void vector_field_2d(scls::Image image, std::vector<scls::Formula_Base*> formula_x, std::vector<scls::Formula_Base*> formula_y, scls::Plane_Base* base, std::vector<scls::Point_2D> arrow_starts) {
    	// Browse the image
		for(std::size_t i = 0;i<arrow_starts.size();i++) {
			// Get the position
			int current_x_pixel = arrow_starts.at(i).x();
			int current_y_pixel = arrow_starts.at(i).y();
			double current_x = base->canonical_x_to_base_x(current_x_pixel);
			double current_y = base->canonical_y_to_base_y(current_y_pixel);

			// Get the needed vector
			scls::Fraction current_value_x = 0;
			scls::Fraction current_value_y = 0;
			for(std::size_t j = 0;j<formula_x.size();j++){current_value_x += *formula_x.at(j)->replace_unknowns("x", scls::Fraction::from_double(current_x)).get()->replace_unknowns("y", scls::Fraction::from_double(current_y)).get()->value<scls::Fraction>();}
			for(std::size_t j = 0;j<formula_y.size();j++){current_value_y += *formula_y.at(j)->replace_unknowns("x", scls::Fraction::from_double(current_x)).get()->replace_unknowns("y", scls::Fraction::from_double(current_y)).get()->value<scls::Fraction>();}
			int current_value_x_pixel = current_value_x.to_double() * 20.0;
			int current_value_y_pixel = current_value_y.to_double() * 20.0;

			image.image()->draw_arrow(current_x_pixel, image.height() - current_y_pixel, current_x_pixel + current_value_x_pixel, image.height() - (current_y_pixel + current_value_y_pixel), scls::Color(0, 0, 0), 0.3, 5);
		}
    }
    void vector_field_2d(scls::Image image, std::vector<scls::Formula_Base*> formula_x, std::vector<scls::Formula_Base*> formula_y, scls::Plane_Base* base) {
        // Field
    	std::vector<scls::Point_2D> arrow_starts = points_grid(image.width(), image.height());
        vector_field_2d(image, formula_x, formula_y, base, arrow_starts);
    }
    void vector_field_2d(scls::Image image, scls::Formula_Base* formula_x, scls::Formula_Base* formula_y, scls::Plane_Base* base) {vector_field_2d(image, std::vector<scls::Formula_Base*>(1, formula_x), std::vector<scls::Formula_Base*>(1, formula_y), base);}

    // Do an arrow-linked vector field
    void vector_field_2d_link(scls::Image image, scls::Formula_Base* formula_x, scls::Formula_Base* formula_y, scls::Plane_Base* base) {
        // Datas
        int start_x_pixel = 80;
        int start_y_pixel = 80;
        int step_x_pixel = 160;
        int step_y_pixel = 160;

        // Browse the image
        int current_x_pixel = start_x_pixel;
        int current_y_pixel = start_y_pixel;
        while(current_y_pixel < image.height()) {
            while(current_x_pixel < image.width()) {
                double current_x = base->canonical_x_to_base_x(current_x_pixel);
                double current_y = base->canonical_y_to_base_y(current_y_pixel);

                // Get the needed vector
                scls::Fraction current_value_x = *formula_x->replace_unknowns("x", scls::Fraction::from_double(current_x)).get()->replace_unknowns("y", scls::Fraction::from_double(current_y)).get()->value<scls::Fraction>();
                scls::Fraction current_value_y = *formula_y->replace_unknowns("x", scls::Fraction::from_double(current_x)).get()->replace_unknowns("y", scls::Fraction::from_double(current_y)).get()->value<scls::Fraction>();
                int current_value_x_pixel = base->base_x_to_canonical_x(current_value_x.to_double());
                int current_value_y_pixel = base->base_y_to_canonical_y(current_value_x.to_double());

                image.image()->draw_arrow(current_x_pixel, image.height() - current_y_pixel, current_value_x_pixel, image.height() - (current_value_y_pixel), scls::Color(0, 0, 0), 0.3, 5);
                //image.draw_line(current_x_pixel, current_y_pixel, current_value_x_pixel, current_value_y_pixel, scls::Color(0, 0, 0), 5);

                current_x_pixel += step_x_pixel;
            }
            current_x_pixel = start_x_pixel;
            current_y_pixel += step_y_pixel;
        }
    }
}
