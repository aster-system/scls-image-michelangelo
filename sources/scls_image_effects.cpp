//******************
//
// scls_image_effects.cpp
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
// This file contains the source code of scls_image_effects.h.
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
#include "../scls_image.h"

// The namespace "scls" is used to simplify the all.
namespace scls {

    //*********
	//
	// Mask system
	//
	//*********

	// Gets the limit of a mask and returns it
	Image limit_from_mask_alpha(Image needed_image) {
        scls::Color color_0 = scls::Color(0, 0, 0, 0);
        Image created_image = Image(needed_image.width(), needed_image.height(), color_0);

        for(int i = 0;i<static_cast<int>(needed_image.width());i++){
            for(int j = 0;j<static_cast<int>(needed_image.height());j++){
                int alpha = needed_image.pixel(i, j).alpha();
                if(alpha != 0){
                    alpha = 255;

                    // Check if the pixel is a limit
                    bool good = (i == 0 || needed_image.pixel(i - 1, j).alpha() == 0);
                    if(!good){good = (i == needed_image.width() - 1 || needed_image.pixel(i + 1, j).alpha() == 0);}
                    if(!good){good = (j == 0 || needed_image.pixel(i, j - 1).alpha() == 0);}
                    if(!good){good = (j == needed_image.height() - 1 || needed_image.pixel(i, j + 1).alpha() == 0);}

                    if(good) {
                        scls::Color current_color = scls::Color(alpha, alpha, alpha, alpha);
                        created_image.set_pixel(i, j, current_color);
                    }
                }
            }
        }

        return created_image;
	}

    // Creates a mask from an image
    Image mask_from_image_alpha(Image needed_image) {
        scls::Color color_0 = scls::Color(0, 0, 0, 0);
        Image created_image = Image(needed_image.width(), needed_image.height(), color_0);

        for(int i = 0;i<static_cast<int>(needed_image.width());i++){
            for(int j = 0;j<static_cast<int>(needed_image.height());j++){
                int alpha = needed_image.pixel(i, j).alpha();
                if(alpha != 0){alpha = 255;}
                scls::Color current_color = scls::Color(alpha, alpha, alpha, alpha);
                created_image.set_pixel(i, j, current_color);
            }
        }

        return created_image;
    }

    // Sketchs a mask
    Image sketch_mask(Image mask, int sketch_width) {
        Image created_image = mask.copy_image();

        for(int i = 0;i<static_cast<int>(mask.width());i++){
            for(int j = 0;j<static_cast<int>(mask.height());j++){
                if(mask.pixel(i, j).red() == 255){created_image.fill_circle(i, j, sketch_width * 2 + 1, scls::Color(0, 0, 0));}
            }
        }

        return created_image;
    }
}
