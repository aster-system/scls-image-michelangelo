//******************
//
// scls_image_color.cpp
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
// This file contains the source code of "scls_image_color.h".
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

// SCLS Image cor
#include "../scls_image_directory/scls_image_color.h"

// The namespace "scls" is used to simplify the all.
namespace scls {
    //*********
	//
	// Color handling
	//
	//*********

	// Basic Color constructor
	Color::Color():Color(0,0,0,255){}
    Color::Color(short red, short green, short blue):Color(red, green, blue, 255){}
    Color::Color(short red, short green, short blue, short alpha): Object() {
        a_alpha = static_cast<double>(alpha) / 255.0;
        a_blue = static_cast<double>(blue) / 255.0;
        a_green = static_cast<double>(green) / 255.0;
        a_red = static_cast<double>(red) / 255.0;
    }

    // Color copy constructor
    Color::Color(const Color& color_copy):Object(),a_alpha(color_copy.a_alpha),a_blue(color_copy.a_blue),a_green(color_copy.a_green),a_red(color_copy.a_red){};

    // Defined colors by name
    void defined_color_by_name(std::string name, Color& color){
        if(name == std::string_view("black")){color = Color(0, 0, 0);}
        else if(name == std::string_view("dark_green")){color = Color(0, 100, 0);}
        else if(name == std::string_view("blue")){color = Color(0, 0, 255);}
        else if(name == std::string_view("gray") || name == std::string_view("grey")){color = Color(96, 96, 96);}
        else if(name == std::string_view("green")){color = Color(0, 255, 0);}
        else if(name == std::string_view("light_blue")){color = Color(0, 204, 204);}
        else if(name == std::string_view("light_gray") || name == std::string_view("light_grey")){color = Color(160, 160, 160);}
        else if(name == std::string_view("light_green")){color = Color(51, 255, 51);}
        else if(name == std::string_view("light_red")){color = Color(255, 102, 102);}
        else if(name == std::string_view("orange")){color = Color(204, 102, 0);}
        else if(name == std::string_view("pink")){color = Color(255, 0, 127);}
        else if(name == std::string_view("purple") || name == std::string_view("violet")){color = Color(51, 0, 102);}
        else if(name == std::string_view("red")){color = Color(255, 0, 0);}
        else if(name == std::string_view("yellow")){color = Color(255, 255, 0);}
        else if(name == std::string_view("transparent")){color = Color(0, 0, 0, 0);}
        else if(name == std::string_view("white")){color = Color(255, 255, 255);}
    }

    // Return a color loaded from a text
    std::vector<std::string> Color::from_std_string_parts(std::string source){
        // Format the text
        while(source[0] == ' ' || source[0] == '(') source = source.substr(1, source.size() - 1);
        while(source[source.size() - 1] == ' ' || source[source.size() - 1] == ')') source = source.substr(0, source.size() - 1);

        // Get the color
        std::vector<std::string> cutted = cut_string(scls::replace(source, std::string(";"), std::string(",")) , ",");
        for(int i = 0;i<static_cast<int>(cutted.size());i++) {
            while(cutted[i][0] == ' '){cutted[i] = cutted[i].substr(1, cutted[i].size() - 1);}
            while(cutted[i][cutted[i].size() - 1] == ' '){cutted[i] = cutted[i].substr(0, cutted[i].size() - 1);}
        }

        return cutted;
    };
    Color Color::from_std_string(std::string source) {
        // Format the text
        std::vector<std::string> cutted = from_std_string_parts(source);

        // Get the color
        Color to_return(255, 255, 255);
        if(cutted.size() > 1) {
            for(int i = 0;i<static_cast<int>(cutted.size());i++) {
                std::shared_ptr<Formula_Base> value = string_to_algebra_element<Formula_Base>(cutted[i]);value.get()->simplify();
                if(i == 0) to_return.set_red(scls::normalize_value(value.get()->value<Fraction>()->to_double(), 0, 255));
                else if(i == 1) to_return.set_green(scls::normalize_value(value.get()->value<Fraction>()->to_double(), 0, 255));
                else if(i == 2) to_return.set_blue(scls::normalize_value(value.get()->value<Fraction>()->to_double(), 0, 255));
                else if(i == 3) to_return.set_alpha(scls::normalize_value(value.get()->value<Fraction>()->to_double(), 0, 255));
            }
        }
        else if (cutted.size() == 1) {defined_color_by_name(cutted.at(0), to_return);}

        return to_return;
    };

    // Return a color loaded from an XML balise
    Color Color::from_xml(std::shared_ptr<XML_Text_Base> source) {
        Color color(0, 0, 0, 255);
        for(int j = 0;j<static_cast<int>(source.get()->xml_balise_attributes().size());j++) {
            XML_Attribute& current_attribute = source.get()->xml_balise_attributes()[j];
            std::string current_attribute_name = current_attribute.name;
            std::string current_attribute_value = current_attribute.value;

            if(current_attribute_value == std::string()) {defined_color_by_name(current_attribute_name, color);}
            else {
                // Precise datas for the color
                if(current_attribute_name == "red") {color.set_red(std::stoi(current_attribute_value));}
                else if(current_attribute_name == "green") {color.set_green(std::stoi(current_attribute_value));}
                else if(current_attribute_name == "blue") {color.set_blue(std::stoi(current_attribute_value));}
                else if(current_attribute_name == "alpha") {color.set_alpha(std::stoi(current_attribute_value));}
            }
        }
        return color;
    }

    // Returns the color to an std::string
    std::string Color::to_std_string(Textual_Math_Settings* settings) {
        if(alpha() == 255){return std::string("(") + std::to_string(static_cast<int>(red())) + std::string(",") + std::to_string(static_cast<int>(green())) + std::string(",") + std::to_string(static_cast<int>(blue())) + std::string(")");}
        return std::string("(") + std::to_string(static_cast<int>(red())) + std::string(",") + std::to_string(static_cast<int>(green())) + std::string(",") + std::to_string(static_cast<int>(blue())) + std::string(",") + std::to_string(static_cast<int>(alpha())) + std::string(")");
    }

    // Operator
    bool Color::operator==(Color color) const {return color.a_red == a_red && color.a_green == a_green && color.a_blue == a_blue && color.a_alpha == a_alpha;}
    bool Color::operator!=(Color color) const {return !operator==(color);}
    Color Color::operator+(const Color& color) const {return Color((a_red + color.a_red) * 255.0, (a_green + color.a_green) * 255.0, (a_blue + color.a_blue) * 255.0, (a_alpha + color.a_alpha) * 255.0);}
    Color Color::operator-(const Color& color) const {return Color((a_red - color.a_red) * 255.0, (a_green - color.a_green) * 255.0, (a_blue - color.a_blue) * 255.0, (a_alpha - color.a_alpha) * 255.0);}
    Color Color::operator*(double proportion) const {return Color(a_red * 255.0 * proportion, a_green * 255.0 * proportion, a_blue * 255.0 * proportion);}

    // Basic Color_Mixer constructor
    Color_Mixer::Color_Mixer(Color color_1, Color color_2){a_colors.push_back(color_1);a_colors.push_back(color_2);};
    Color_Mixer::Color_Mixer(short red, short green, short blue):Color_Mixer(Color(red, green, blue)){};
    Color_Mixer::Color_Mixer(std::vector<Color> colors){a_colors=colors;}
    Color_Mixer::Color_Mixer(Color color){a_colors.push_back(color);};
    Color_Mixer::Color_Mixer(){};

    // Add a color to the mixer
    void Color_Mixer::add_color(Color new_color){a_colors.push_back(new_color);}
    // Returns the current color
    Color Color_Mixer::current_color(double total_proportion) const {
        // Asserts
        if(static_cast<int>(a_colors.size()) <= 0){return Color(0, 0, 0);}
        else if(static_cast<int>(a_colors.size()) == 1){return a_colors.at(0);}
        total_proportion -= std::floor(total_proportion);

        double current_time = 0;int needed_color = 0;
        while(current_time <= total_proportion) {needed_color++;current_time += 1.0/static_cast<double>(a_colors.size());}
        needed_color--;total_proportion-=(static_cast<double>(needed_color)/static_cast<double>(a_colors.size()));
        int next_color = needed_color + 1;if(next_color >= static_cast<int>(a_colors.size())){next_color = 0;}
        return a_colors.at(needed_color) + (a_colors.at(next_color) - a_colors.at(needed_color)) * total_proportion * static_cast<double>(a_colors.size());
    }
}
