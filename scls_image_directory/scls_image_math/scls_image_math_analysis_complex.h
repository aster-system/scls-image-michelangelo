//******************
//
// scls_image_math_analysis_complex.h
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
// This file contains some tools to use complex analysis in image.
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

#ifndef SCLS_IMAGE_MATH_ANALYSIS_COMPLEX
#define SCLS_IMAGE_MATH_ANALYSIS_COMPLEX

// Include SCLS Image bigger header
#include "../scls_image_core.h"

namespace scls {
    // Represent the argument / module of a complex function on an image
    Image function_complex_to_argument_image(Complex (*complex_function)(Complex), int image_width, int image_height);
    Image function_complex_to_module_image(Complex (*complex_function)(Complex), int image_width, int image_height);
}

#endif // SCLS_IMAGE_MATH_ANALYSIS_COMPLEX
