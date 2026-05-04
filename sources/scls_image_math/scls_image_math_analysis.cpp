//******************
//
// scls_image_math_analysis.cpp
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
// This file contains the source code of "scls_image_math_analysis.h".
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
#include "../../scls_image_directory/scls_image_math/scls_image_math_analysis.h"

namespace scls {
    // Draw a Darboux integral
    void draw_function_integral_darboux_sub(Image img, Formula_Base* f, Plane_Base* b, double start_x, double end_x, double step) {
        scls::Fraction pi_temp = scls::Fraction(31415, 10000);
        //Math_Environment::Relation_Module m = Math_Environment::Relation_Module(pi_temp);

        // Datas for the drawing
        Color fill_color_top = Color(0, 0, 255);
        Color fill_color_bottom = Color(0, 255, 0);
        step *= b->width_unit_in_canonical_base();
        end_x = b->base_x_to_canonical_x(end_x);
        start_x = b->base_x_to_canonical_x(start_x);

        scls::Point_2D last_point = scls::Point_2D(0, 0);
        double needed_x = 0;
        double zero_position_y = b->base_y_to_canonical_y(0);
        for(int j = start_x;j<img.width();j+=step){
            needed_x = b->canonical_x_to_base_x(j);
            scls::Point_2D current_point = scls::Point_2D(j, img.height() - b->base_y_to_canonical_y(f->replace_unknowns("x", needed_x).get()->value<scls::Fraction>()->to_double()));
            if(j < end_x) {
                if(last_point.x() != -1 && !((last_point.y() < 0 && current_point.y() > img.height()) || (last_point.y() > img.height() && current_point.y() < 0))){
                    if(zero_position_y < current_point.y()) {img.fill_rect(j, zero_position_y, step, current_point.y() - zero_position_y, fill_color_bottom);}
                    else{img.fill_rect(j, current_point.y(), step, zero_position_y - current_point.y(), fill_color_top);}
                }
            }
            else{break;}
            last_point = current_point;
        }
    }
    void draw_function_integral_darboux_sup(Image img, Formula_Base* f, Plane_Base* b, double start_x, double end_x, double step) {
        scls::Fraction pi_temp = scls::Fraction(31415, 10000);
        //Math_Environment::Relation_Module m = Math_Environment::Relation_Module(pi_temp);

        // Datas for the drawing
        Color fill_color_top = Color(0, 0, 255);
        Color fill_color_bottom = Color(0, 255, 0);
        step *= b->width_unit_in_canonical_base();
        end_x = b->base_x_to_canonical_x(end_x);
        start_x = b->base_x_to_canonical_x(start_x);

        scls::Point_2D last_point = scls::Point_2D(0, 0);
        double needed_x = 0;
        double zero_position_y = b->base_y_to_canonical_y(0);
        for(int j = start_x;j<img.width();j+=step){
            needed_x = b->canonical_x_to_base_x(j + step);
            scls::Point_2D current_point = scls::Point_2D(j, img.height() - b->base_y_to_canonical_y(f->replace_unknowns("x", needed_x).get()->value<scls::Fraction>()->to_double()));
            if(j < end_x) {
                if(last_point.x() != -1 && !((last_point.y() < 0 && current_point.y() > img.height()) || (last_point.y() > img.height() && current_point.y() < 0))){
                    if(zero_position_y < current_point.y()) {img.fill_rect(j, zero_position_y, step, current_point.y() - zero_position_y, fill_color_bottom);}
                    else{img.fill_rect(j, current_point.y(), step, zero_position_y - current_point.y(), fill_color_top);}
                }
            }
            else{break;}
            last_point = current_point;
        }
    }

    // Draw a Riemann integral
    void draw_function_integral_riemann(Image img, Formula_Base* f, Plane_Base* b, double start_x, double end_x, double step) {
        scls::Fraction pi_temp = scls::Fraction(31415, 10000);
        //Math_Environment::Relation_Module m = Math_Environment::Relation_Module(pi_temp);

        // Datas for the drawing
        Color fill_color_top = Color(0, 0, 255);
        Color fill_color_bottom = Color(0, 255, 0);
        step *= b->width_unit_in_canonical_base();
        end_x = b->base_x_to_canonical_x(end_x);
        start_x = b->base_x_to_canonical_x(start_x);

        scls::Point_2D last_point = scls::Point_2D(0, 0);
        double needed_x = 0;
        double zero_position_y = b->base_y_to_canonical_y(0);
        for(int j = start_x;j<img.width();j+=step){
            needed_x = b->canonical_x_to_base_x(j + step / 2.0);
            scls::Point_2D current_point = scls::Point_2D(j, img.height() - b->base_y_to_canonical_y(f->replace_unknowns("x", needed_x).get()->value<scls::Fraction>()->to_double()));
            if(j < end_x) {
                if(last_point.x() != -1 && !((last_point.y() < 0 && current_point.y() > img.height()) || (last_point.y() > img.height() && current_point.y() < 0))){
                    if(zero_position_y < current_point.y()) {img.fill_rect(j, zero_position_y, step, current_point.y() - zero_position_y, fill_color_bottom);}
                    else{img.fill_rect(j, current_point.y(), step, zero_position_y - current_point.y(), fill_color_top);}
                }
            }
            else{break;}
            last_point = current_point;
        }
    }

    // Draw a function secant / tangent
    void draw_line_entirely(Image img, double x_1, double y_1, double x_2, double y_2, Color color, int line_width) {
        // Switch if necessary
        if(x_1 > x_2){double temp = x_1;x_1 = x_2;x_2=temp;temp = y_1;y_1 = y_2;y_2=temp;}
        double dir = (y_2 - y_1) / (x_2 - x_1);

        // Left part
        double y_left_segment = (img.height() - y_1) + line_width;
        double left_segment = x_1 + (y_left_segment / dir);
        y_left_segment = img.height();

        // Right part
        double y_right_segment = y_2 + line_width;
        double right_segment = x_2 - (y_right_segment / dir);
        y_right_segment = 0;

        img.draw_line(left_segment, y_left_segment, right_segment, y_right_segment, color, line_width);
    }
    void draw_function_secant(Image img, Formula_Base* f, Plane_Base* b, double point_1, double point_2) {
        double needed_x_1 = b->base_x_to_canonical_x(point_1);
        double needed_x_2 = b->base_x_to_canonical_x(point_2);

        double value_1 = f->replace_unknowns("x", point_1).get()->value<scls::Fraction>()->to_double();
        double value_2 = f->replace_unknowns("x", point_2).get()->value<scls::Fraction>()->to_double();
        double needed_y_1 = img.height() - b->base_y_to_canonical_y(value_1);
        double needed_y_2 = img.height() - b->base_y_to_canonical_y(value_2);

        draw_line_entirely(img, needed_x_1, needed_y_1, needed_x_2, needed_y_2, scls::Color(255, 0, 0), 5);
    }
    void draw_function_tangent(Image img, Formula_Base* f, Plane_Base* b, double point) {
        draw_function_secant(img, f, b, point, point + 0.1);
    }

    // Fill the graph of a function
    void fill_function_graph(Image img, Formula_Base* f, Plane_Base* b, double start_x, double end_x) {
        scls::Fraction pi_temp = scls::Fraction(31415, 10000);
        //Math_Environment::Relation_Module m = Math_Environment::Relation_Module(pi_temp);

        // Datas for the drawing
        Color fill_color_top = Color(0, 0, 255);
        Color fill_color_bottom = Color(0, 255, 0);

        scls::Point_2D last_point = scls::Point_2D(-1, 0);
        double needed_x = 0;
        double zero_position_y = b->base_y_to_canonical_y(0);
        for(int j = 0;j<img.width();j++){
            needed_x = b->canonical_x_to_base_x(j);
            scls::Point_2D current_point = scls::Point_2D(j, img.height() - b->base_y_to_canonical_y(f->replace_unknowns("x", needed_x).get()->value<scls::Fraction>()->to_double()));
            if(needed_x < end_x) {
                if(last_point.x() != -1 && !((last_point.y() < 0 && current_point.y() > img.height()) || (last_point.y() > img.height() && current_point.y() < 0))){
                    img.draw_line(last_point.x(), last_point.y(), current_point.x(), current_point.y(), scls::Color(255, 0, 0), 5);
                    if(zero_position_y < current_point.y()) {img.fill_rect(j, zero_position_y, 1, current_point.y() - zero_position_y, fill_color_bottom);}
                    else{img.fill_rect(j, current_point.y(), 1, zero_position_y - current_point.y(), fill_color_top);}
                }
            }
            else{break;}
            last_point = current_point;
        }
    }
}
