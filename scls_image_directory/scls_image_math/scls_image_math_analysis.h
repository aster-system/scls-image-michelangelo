//******************
//
// scls_image_math_analysis.h
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
// This file contains some tools to use analysis in image.
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

#ifndef SCLS_IMAGE_MATH_ANALYSIS
#define SCLS_IMAGE_MATH_ANALYSIS

// Include SCLS Image bigger header
#include "../scls_image_core.h"

namespace scls {
    // Draw a Darboux integral
    void draw_function_integral_darboux_sub(Image img, Formula_Base* f, Plane_Base* b, double start_x, double end_x, int parts);
    void draw_function_integral_darboux_sup(Image img, Formula_Base* f, Plane_Base* b, double start_x, double end_x, int parts);

    // Draw a Riemann integral
    void draw_function_integral_riemann(Image img, Formula_Base* f, Plane_Base* b, double start_x, double end_x, double step);

    // Draw a function secant / tangent
    void draw_function_secant(Image img, Formula_Base* f, Plane_Base* b, double point_1, double point_2);
    void draw_function_tangent(Image img, Formula_Base* f, Plane_Base* b, double point);

    // Fill the graph of a function
    void fill_function_graph(Image img, Formula_Base* f, Plane_Base* b, double start_x, double end_x);
}

#endif // SCLS_IMAGE_MATH_ANALYSIS
