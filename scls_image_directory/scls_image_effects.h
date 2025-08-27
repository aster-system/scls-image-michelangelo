//******************
//
// scls_image_effects.h
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
// This file contains some tools to use images more efficienly.
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

// Include the good header file
#include "scls_image_core.h"

// The namespace "scls" is used to simplify the all.
namespace scls {

    //*********
	//
	// Mask system
	//
	//*********

	// Gets the limit of a mask and returns it
	Image limit_from_mask_alpha(Image needed_image);
    // Creates a mask from an image
    Image mask_from_image_alpha(Image needed_image);

    // Sketchs a mask
    Image sketch_mask(Image mask, int sketch_width);
}
