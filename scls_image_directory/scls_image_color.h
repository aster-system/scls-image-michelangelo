//******************
//
// scls_image_color.h
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
// This file contains the needed tools to handle colors.
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

#ifndef SCLS_IMAGE_COLOR
#define SCLS_IMAGE_COLOR

// SCLS Image cor
#include "../scls_image_root.h"

// The namespace "scls" is used to simplify the all.
namespace scls {
    //*********
	//
	// Color handling
	//
	//*********

	class Color : public Object {
	    // Class representing an 8 bit RGBA color
    public:
        // Basic Color constructor
        Color(short red, short green, short blue);
        Color(short red, short green, short blue, short alpha);
        Color();
        // Color copy constructor
        Color(const Color& color_copy);

        // Return a color loaded from a text
        static std::vector<std::string> from_std_string_parts(std::string source);
        static Color from_std_string(std::string source);
        // Return a color loaded from an XML balise
        static Color from_xml(std::shared_ptr<XML_Text_Base> source);
        // Returns the color to an std::string
        std::string to_std_string(Textual_Math_Settings* settings);

        // Getters and setters
        inline unsigned char alpha() const {return static_cast<unsigned char>(a_alpha * 255.0);};
        inline unsigned char blue() const {return static_cast<unsigned char>(a_blue * 255.0);};
        inline unsigned char green() const {return static_cast<unsigned char>(a_green * 255.0);};
        inline unsigned char red() const {return static_cast<unsigned char>(a_red * 255.0);};
        inline void set_alpha(unsigned char new_alpha) {a_alpha = static_cast<double>(new_alpha) / 255.0;};
        inline void set_blue(unsigned char new_blue) {a_blue = static_cast<double>(new_blue) / 255.0;};
        inline void set_green(unsigned char new_green) {a_green = static_cast<double>(new_green) / 255.0;};
        inline void set_red(unsigned char new_red) {a_red = static_cast<double>(new_red) / 255.0;};
        inline void set_rgb(unsigned char new_red, unsigned char new_green, unsigned char new_blue) { set_red(new_red); set_green(new_green); set_blue(new_blue); };
        inline void set_rgba(unsigned char new_red, unsigned char new_green, unsigned char new_blue, unsigned char new_alpha) { set_rgb(new_red, new_green, new_blue); set_alpha(new_alpha); };

        // Operator
        bool operator==(Color color) const;
        bool operator!=(Color color) const;
        Color operator+(const Color& color) const;
        Color operator-(const Color& color) const;
        Color operator*(double proportion) const;
    private:
        // Descriptor of the color (between 0 and 1)
        double a_alpha = 1;
        double a_blue = 0;
        double a_green = 0;
        double a_red = 0;
	};

	// Defined colors by name
    void defined_color_by_name(std::string name, Color& color);

	class Color_Mixer {
	    // Class representing a mixer of color
    public:
        // Basic Color_Mixer constructor
        Color_Mixer(short red, short green, short blue);
        Color_Mixer(Color color_1, Color color_2);
        Color_Mixer(std::vector<Color> colors);
        Color_Mixer(Color color);
        Color_Mixer();

        // Add a color to the mixer
        void add_color(Color new_color);
        // Returns the current color
        Color current_color(double total_proportion) const;

    private:
        // Colors in the mixer
        std::vector<Color> a_colors;
	};
}

#endif // SCLS_IMAGE_COLOR
