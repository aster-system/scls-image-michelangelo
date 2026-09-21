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
// This file contains some tools to handle fields with images.
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

#ifndef SCLS_IMAGE_MATH_FIELD
#define SCLS_IMAGE_MATH_FIELD

#include "../../scls_image.h"

// The namespace "scls" is used to simplify the all.
namespace scls {
	// Point in a grid
	std::vector<scls::Point_2D> points_grid(int image_width, int image_height);

    // Do a colored scalar field
    void scalar_field_2d_colored(scls::Image image, scls::Formula_Base* formula, scls::Plane_Base* base);

    // Do an arrowed vector field
    void vector_field_2d(scls::Image image, std::vector<scls::Formula_Base*> formula_x, std::vector<scls::Formula_Base*> formula_y, scls::Plane_Base* base, std::vector<scls::Point_2D> arrow_starts);
    void vector_field_2d(scls::Image image, std::vector<scls::Formula_Base*> formula_x, std::vector<scls::Formula_Base*> formula_y, scls::Plane_Base* base);
    void vector_field_2d(scls::Image image, scls::Formula_Base* formula_x, scls::Formula_Base* formula_y, scls::Plane_Base* base);

    // Do an arrow-linked vector field
    void vector_field_2d_link(scls::Image image, scls::Formula_Base* formula_x, scls::Formula_Base* formula_y, scls::Plane_Base* base);
}

#endif // SCLS_IMAGE_MATH_FIELD
