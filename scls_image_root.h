//******************
//
// scls_image_root.h
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
// This file includes all the SCLS Image root files.
//
//******************
// Dependencies :
//
// To work, this file needs the ZLib library.
// Get it on this web site : https://www.zlib.net/.
// It also need the FreeType library.
// Get it on this web site : https://freetype.org/.
// Thoses two libraries are published under the GPL V3.0 license.
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

#ifndef SCLS_IMAGE_ROOT
#define SCLS_IMAGE_ROOT

// Avoid some errors with libraries path
// SCLS Math
#ifndef SCLS_MATH_PATH
#ifdef __ASTER_DEV
#define SCLS_MATH_PATH "../scls-math-carl/scls_math.h"
#else
#define SCLS_MATH_PATH <scls_math.h>
#endif // __ASTER_DEV
#endif // SCLS_MATH_PATH

// Include SCLS Math
#include SCLS_MATH_PATH

// Font path of the Aster System's logo
#ifndef LOGO_FONT_PATH
// Before school PC : "/usr/share/fonts/truetype/AlteHaasGroteskBold.ttf"
// Development PC : "E:\\Divers\\Fonts\\AlteHaasGroteskBold.ttf"
#define LOGO_FONT_PATH "E:\\Divers\\Fonts\\AlteHaasGroteskBold.ttf"
#endif // LOGO_FONT_PATH

// Define SCLS_INIT
#ifndef SCLS_IMAGE_INIT
    #define SCLS_IMAGE_INIT std::string scls::__base_font_path = std::string(BASE_FONT_PATH); std::string scls::__default_font = std::string(DEFAULT_FONT);
#endif // SCLS_IMAGE
#ifdef SCLS_INIT
#undef SCLS_INIT
#endif // SCLS_INIT
#ifndef SCLS_INIT
#define SCLS_INIT SCLS_IMAGE_INIT SCLS_MATH_INIT SCLS_FOUNDATION_INIT
#endif // SCLS_INIT

#endif // SCLS_IMAGE_ROOT
