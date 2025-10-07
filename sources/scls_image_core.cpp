//******************
//
// scls_image_core.cpp
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
// This file contains the source code of scls_image_core.h.
//
// The CRC functions are copied from the W3 Consortium website.
// See : https://www.w3.org/TR/2003/REC-PNG-20031110/#D-CRCAppendix.
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
	// Color handling
	//
	//*********

	// Basic Color constructor
    Color::Color(short red, short green, short blue):Color(red, green, blue, 255){}
    Color::Color(short red, short green, short blue, short alpha) {
        a_alpha = static_cast<double>(alpha) / 255.0;
        a_blue = static_cast<double>(blue) / 255.0;
        a_green = static_cast<double>(green) / 255.0;
        a_red = static_cast<double>(red) / 255.0;
    }

    // Defined colors by name
    void defined_color_by_name(std::string name, Color& color){
        if(name == std::string("black")){color = Color(0, 0, 0);}
        else if(name == std::string("blue")){color = Color(0, 0, 255);}
        else if(name == std::string("gray") || name == std::string("grey")){color = Color(96, 96, 96);}
        else if(name == std::string("green")){color = Color(0, 255, 0);}
        else if(name == std::string("light_blue")){color = Color(0, 204, 204);}
        else if(name == std::string("light_gray") || name == std::string("light_grey")){color = Color(160, 160, 160);}
        else if(name == std::string("light_green")){color = Color(51, 255, 51);}
        else if(name == std::string("orange")){color = Color(204, 102, 0);}
        else if(name == std::string("pink")){color = Color(255, 0, 127);}
        else if(name == std::string("red")){color = Color(255, 0, 0);}
        else if(name == std::string("yellow")){color = Color(255, 255, 0);}
        else if(name == std::string("transparent")){color = Color(0, 0, 0, 0);}
        else if(name == std::string("white")){color = Color(255, 255, 255);}
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
                if(i == 0) to_return.set_red(std::stoi(cutted[i]));
                else if(i == 1) to_return.set_green(std::stoi(cutted[i]));
                else if(i == 2) to_return.set_blue(std::stoi(cutted[i]));
                else if(i == 3) to_return.set_alpha(std::stoi(cutted[i]));
            }
        }
        else if (cutted.size() == 1) {defined_color_by_name(cutted.at(0), to_return);}

        return to_return;
    };

    // Return a color loaded from an XML balise
    Color Color::from_xml(std::shared_ptr<__XML_Text_Base> source) {
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
    bool Color::operator==(const Color& color) const {return color.a_red == a_red && color.a_green == a_green && color.a_blue == a_blue && color.a_alpha == a_alpha;}
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

    //*********
	//
	// Compression handling
	//
	//*********

	// Compress data from a char array without returning the result
    int _compress_binary(char* to_compress, unsigned int to_compress_size, char* output, unsigned int output_size, unsigned int& total_output_size, unsigned int compression_level) {
		SET_BINARY_MODE(stdin);
        SET_BINARY_MODE(stdout);

		// Create compression ENV
		unsigned int buffer_size = 16384.0 / 2.0;
		int deflate_type = Z_FINISH;
		int ret = 0;
		z_stream strm;
		strm.zalloc = Z_NULL;
		strm.zfree = Z_NULL;
		strm.opaque = Z_NULL;

		ret = deflateInit(&strm, compression_level); if (ret != Z_OK) return ret;
		strm.avail_in = to_compress_size;
		strm.next_in = (Bytef*)(to_compress);

		// Set output
        strm.avail_out = buffer_size; strm.next_out = (Bytef*)output;

		// Compress data
		do
		{
		    // Set output
			strm.avail_out = buffer_size;

			// Do the decompression
			ret = deflate(&strm, deflate_type);
			if (ret == Z_STREAM_ERROR)
			{
				(void)deflateEnd(&strm);
				return -1;
			}

			switch (ret)
			{
			case Z_NEED_DICT:
				ret = Z_DATA_ERROR;
				break;
			case Z_MEM_ERROR:
				(void)deflateEnd(&strm);
				return -2;
			case Z_BUF_ERROR:
				(void)deflateEnd(&strm);
				return -5;
			case Z_STREAM_END:
				break;
			}
		} while (strm.avail_out == 0);
		(void)deflateEnd(&strm);

		total_output_size = strm.total_out;

		return 1;
	}

	// Compress data from a char array and return the result
    char* compress_binary(char* to_compress, unsigned int to_compress_size, unsigned int& output_size, unsigned int compression_level) {
		char* output = new char[to_compress_size * 2];

		unsigned int ret = _compress_binary(to_compress, to_compress_size, output, to_compress_size + 1000, output_size, compression_level);
		if (ret != 1) return 0;

		char* final_array = new char[output_size];
		for (unsigned int i = 0; i < output_size; i++) final_array[i] = output[i];

		delete[] output;

		return final_array;
	}

	// Uncompress data from a char array
    int uncompress_binary(char* to_uncompress, unsigned int to_uncompress_size, char* output, unsigned int output_size) {
		// Create compression ENV
		int ret = 0;
		z_stream strm;
		strm.zalloc = Z_NULL;
		strm.zfree = Z_NULL;
		strm.opaque = Z_NULL;
		strm.avail_in = 0;
		strm.next_in = Z_NULL;
		ret = inflateInit(&strm);
		if (ret != Z_OK) return ret;
		strm.avail_in = to_uncompress_size;
		strm.next_in = (Bytef*)(to_uncompress);
		bool stream_end = false;

		// Uncompress data
		do {
			// Set output
			strm.avail_out = output_size;
			strm.next_out = (Bytef*)output;

			// Do the decompression
			ret = inflate(&strm, Z_NO_FLUSH);
			if (ret == Z_STREAM_ERROR)
			{
				(void)inflateEnd(&strm);
				return -1;
			}

			switch (ret)
			{
			case Z_NEED_DICT:
				ret = Z_DATA_ERROR;
				break;
			case Z_MEM_ERROR:
				(void)inflateEnd(&strm);
				return -2;
			case Z_STREAM_END:
				stream_end = true;
				break;
			}
		} while (strm.avail_out == 0 && !stream_end);
		(void)inflateEnd(&strm);

		return 1;
	}

    //*********
	//
	// The Image class
	//
	//*********

	// Image constructor
    __Image_Base::Image::Image(){}
    __Image_Base::Image::Image(std::string path):__Image_Base::Image::Image(std::make_shared<__Image_Base>(path)){}
    __Image_Base::Image::Image(unsigned short width, unsigned short height, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, unsigned int color_type):__Image_Base::Image::Image(std::make_shared<__Image_Base>(width, height, red, green, blue, alpha, color_type)){}
    __Image_Base::Image::Image(unsigned short width, unsigned short height, Color color, unsigned int color_type):__Image_Base::Image::Image(std::make_shared<__Image_Base>(width, height, color, color_type)){}
    __Image_Base::Image::Image(unsigned short width, unsigned short height, Color color):__Image_Base::Image::Image(std::make_shared<__Image_Base>(width, height, color, SCLS_IMAGE_RGBA)){}
    __Image_Base::Image::Image(unsigned short width, unsigned short height):__Image_Base::Image::Image(std::make_shared<__Image_Base>(width, height)){}
    __Image_Base::Image::Image(std::shared_ptr<__Image_Base> new_image):a_image(new_image){}

    // Fills the image with one color
    void __Image_Base::Image::fill(Color color){a_image.get()->fill(color);}
    void __Image_Base::Image::fill(int red, int green, int blue){a_image.get()->fill(red, green, blue);}

    // Drawing methods
    // Fill a circle on the image
    void __Image_Base::Image::fill_circle(int x_center, int y_center, double radius, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {a_image.get()->fill_circle(x_center, y_center, radius, red, green, blue, alpha, 0, 0, 0, 0, 0);}
    void __Image_Base::Image::fill_circle(int x_center, int y_center, double radius_x, double radius_y, double angle_start, double angle_end, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, double border_radius, unsigned char border_red, unsigned char border_green, unsigned char border_blue, unsigned char border_alpha){a_image.get()->fill_circle(x_center, y_center, radius_x, radius_y, 0, angle_start, angle_end, red, green, blue, alpha, border_radius, border_red, border_green, border_blue, border_alpha);}
    void __Image_Base::Image::fill_circle(int x_center, int y_center, double radius, double angle_start, double angle_end, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, double border_radius, unsigned char border_red, unsigned char border_green, unsigned char border_blue, unsigned char border_alpha){a_image.get()->fill_circle(x_center, y_center, radius, radius, angle_start, angle_end, red, green, blue, alpha, border_radius, border_red, border_green, border_blue, border_alpha);}
    void __Image_Base::Image::fill_circle(int x_center, int y_center, double radius, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, double border_radius, unsigned char border_red, unsigned char border_green, unsigned char border_blue, unsigned char border_alpha){a_image.get()->fill_circle(x_center,y_center,radius,0,360,red,green,blue,alpha,border_radius,border_red,border_green,border_blue,border_alpha);};
    void __Image_Base::Image::fill_circle(int x_center, int y_center, double radius, double angle_start, double angle_end, Color color, double border_radius, Color border_color){a_image.get()->fill_circle(x_center,y_center,radius,angle_start,angle_end,color.red(),color.green(),color.blue(),color.alpha(),border_radius,border_color.red(),border_color.green(),border_color.blue(),border_color.alpha());};
    void __Image_Base::Image::fill_circle(int x_center, int y_center, double radius_x, double radius_y, double angle_start, double angle_end, Color color, double border_radius, Color border_color){a_image.get()->fill_circle(x_center,y_center,radius_x,radius_y,angle_start,angle_end,color.red(),color.green(),color.blue(),color.alpha(),border_radius,border_color.red(),border_color.green(),border_color.blue(),border_color.alpha());};
    void __Image_Base::Image::fill_circle(int x_center, int y_center, double radius_x, double radius_y, double angle, double angle_start, double angle_end, Color color, double border_radius, Color border_color){a_image.get()->fill_circle(x_center,y_center,radius_x,radius_y,angle,angle_start,angle_end,color.red(),color.green(),color.blue(),color.alpha(),border_radius,border_color.red(),border_color.green(),border_color.blue(),border_color.alpha());};
    void __Image_Base::Image::fill_circle(int x_center, int y_center, double radius_x, double radius_y, Color color, double border_radius, Color border_color){a_image.get()->fill_circle(x_center,y_center,radius_x,radius_y,0,0,360,color.red(),color.green(),color.blue(),color.alpha(),border_radius,border_color.red(),border_color.green(),border_color.blue(),border_color.alpha());};
    void __Image_Base::Image::fill_circle(int x_center, int y_center, double radius, Color color, double border_radius, Color border_color){a_image.get()->fill_circle(x_center,y_center,radius,0,360,color.red(),color.green(),color.blue(),color.alpha(),border_radius,border_color.red(),border_color.green(),border_color.blue(),border_color.alpha());};
    void __Image_Base::Image::fill_circle(int x_center, int y_center, double radius, Color color){a_image.get()->fill_circle(x_center,y_center,radius,color.red(),color.green(),color.blue(),color.alpha());};
    void __Image_Base::Image::fill_circle(int x_center, int y_center, double radius_x, double radius_y, double angle, double angle_start, double angle_end, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, double border_radius, unsigned char border_red, unsigned char border_green, unsigned char border_blue, unsigned char border_alpha){a_image.get()->fill_circle(x_center, y_center, radius_x, radius_y, angle, angle_start, angle_end, red ,green, blue, alpha, border_radius, border_red, border_green, border_blue, border_alpha);}
    // Fills a rectangle on the image
    void __Image_Base::Image::fill_rect(int x, int y, unsigned short rect_width, unsigned short rect_height, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha){a_image.get()->fill_rect(x,y,rect_width,rect_height,red,green,blue,alpha);}
    void __Image_Base::Image::fill_rect(int x, int y, unsigned short rect_width, unsigned short rect_height, Color color){a_image.get()->fill_rect(x,y,rect_width,rect_height,color.red(),color.green(),color.blue(),color.alpha());}

    // Forces a pixel to change its value
    void __Image_Base::Image::force_pixel(int x, int y, Color color){return a_image.get()->force_pixel(x, y, color);}

    // Copies the image
    __Image_Base::Image __Image_Base::Image::copy_image(){return a_image.get()->copy_image();}

    // Pastes an Image on this Image
    void __Image_Base::Image::paste(__Image_Base* to_paste, int x, int y, double opacity){a_image.get()->paste(to_paste, x, y, opacity);}
    void __Image_Base::Image::paste(std::string path, int x, int y, double opacity){a_image.get()->paste(path, x, y, opacity);}
    void __Image_Base::Image::paste(Image* to_paste, int x, int y, double opacity){a_image.get()->paste(to_paste->image(), x, y, opacity);}
    void __Image_Base::Image::paste(Image to_paste, int x, int y, double opacity){a_image.get()->paste(to_paste.image(), x, y, opacity);}
    void __Image_Base::Image::paste(Image* to_paste, int x, int y){a_image.get()->paste(to_paste->image(), x, y);}
    void __Image_Base::Image::paste(Image to_paste, int x, int y){a_image.get()->paste(to_paste.image(), x, y);}

    // Get datas about a specific pixel
    Color __Image_Base::Image::pixel(unsigned short x, unsigned short y){return image()->pixel(x, y);}
    Color __Image_Base::Image::pixel_by_number(unsigned int position){return image()->pixel_by_number(position);}
    Color __Image_Base::Image::pixel_directly(unsigned int position, unsigned int multiplier){return image()->pixel_directly(position, multiplier);}
    Color __Image_Base::Image::pixel_rgba_directly(unsigned int position, unsigned int multiplier){return image()->pixel_rgba_directly(position, multiplier);}

    // Replaces a color with an other on the image
    void __Image_Base::Image::replace_color(Color old_color, Color new_color) {image()->replace_color(old_color, new_color);}

    // Returns a shared ptr of the image with a new size, adaptated
    __Image_Base::Image __Image_Base::Image::resize_adaptative(unsigned short new_width, unsigned short new_height){return image()->resize_adaptative(new_width, new_height);}
    __Image_Base::Image __Image_Base::Image::resize_adaptative(scls::Point_2D new_size){return image()->resize_adaptative(new_size.x(), new_size.y());}
    __Image_Base::Image __Image_Base::Image::resize_adaptative_height(unsigned short new_height){return image()->resize_adaptative_height(new_height);}
    __Image_Base::Image __Image_Base::Image::resize_adaptative_width(unsigned short new_width){return image()->resize_adaptative_width(new_width);}

    // Rotates the image
    void __Image_Base::Image::rotate(double angle){image()->rotate(angle);}
    __Image_Base::Image __Image_Base::Image::rotated(double angle){return image()->rotated(angle);}

    // Saves / loads
    // Save the image into the PNG format
    void __Image_Base::Image::save_png(std::string path){a_image.get()->save_png(path);}

    // Sets the color of a certain pixel
    void __Image_Base::Image::set_pixel(int x, int y, Color color){if(x >= 0 && y >= 0){a_image.get()->set_pixel(x, y, color);}}

    // Points in a circle (VERSION OF THE 19/08/25)
    std::vector<scls::Point_2D> circle_points(int x_center, int y_center, int radius, int width, int height){return circle_points(x_center, y_center, radius, 0, width, height);}
    std::vector<scls::Point_2D> circle_points(int x_center, int y_center, int radius, int border_radius, int width, int height) {
        // Format the angle
        int alpha = 0;//int blue = 0;int green = 0;int red = 0;
        int border_alpha = 255;//int border_blue = 0;int border_green = 0;int border_red = 0;
        double angle = 0;double angle_end = 360;double angle_start=0;
        double radius_x = radius;double radius_y = radius;
        while(angle <= -90.0){angle += 180.0;}while(angle >= 90.0){angle -= 180.0;}
        angle /= (180.0/SCLS_PI);

        // Datas to return
        int current_to_return = 0;
        std::vector<scls::Point_2D> to_return = std::vector<scls::Point_2D>(width * height);

        double last_angle = SCLS_PI / 3.0;double last_x = oval_vector_x(radius_x, radius_y, last_angle).rotated(angle * (180.0 / SCLS_PI)).x();
        double left_angle = SCLS_PI * 1.8;double left_x = oval_vector_x(radius_x, radius_y, left_angle).rotated(angle * (180.0 / SCLS_PI)).x();
        for(int i = 0;i<30;i++){
            double new_angle = (last_angle + left_angle) / 2.0;
            double needed_x = oval_vector_x(radius_x, radius_y, new_angle).rotated(angle * (180.0 / SCLS_PI)).x();
            if(last_x > left_x){last_angle = new_angle;last_x = needed_x;}
            else{left_angle = new_angle;left_x = needed_x;}
        }
        last_angle = 0;double last_y = scls::vector_2d_with_angle(last_angle).rotated(angle * (180.0 / SCLS_PI)).y() * oval_radius_proportion_y(radius_x, radius_y, last_angle);
        double top_angle = SCLS_PI;double top_y = scls::vector_2d_with_angle(top_angle).rotated(angle * (180.0 / SCLS_PI)).y() * oval_radius_proportion_y(radius_x, radius_y, top_angle);
        for(int i = 0;i<30;i++){
            double new_angle = (last_angle + top_angle) / 2.0;
            double needed_y = scls::vector_2d_with_angle(new_angle).rotated(angle * (180.0 / SCLS_PI)).y() * oval_radius_proportion_y(radius_x, radius_y, new_angle);
            if(last_y < top_y){last_angle = new_angle;last_y = needed_y;}
            else{top_angle = new_angle;top_y = needed_y;}
        }

        // Get the needed radius
        double drew_radius_x = std::sqrt(std::pow(radius_x * std::cos(angle), 2) + std::pow(radius_y * std::sin(angle), 2));
        double drew_y_center = std::sqrt(std::abs(std::pow(oval_radius(radius_x, radius_y, left_angle - SCLS_PI), 2) - std::pow(drew_radius_x, 2)));
        if(angle < 0){drew_y_center *= -1;}
        const int start_x = round(x_center - (drew_radius_x));
        const int start_x_inner = start_x + border_radius;
        int current_x = 0;

        // Calculate the Y datas
        double y_multiplier = 1;
        double y_center_offset_left = 0;
        double y_center_offset_left_to_add = drew_y_center / drew_radius_x;
        const int start_y_left = y_center - drew_y_center * y_multiplier;
        const int start_y_right = y_center + drew_y_center * y_multiplier;

        // Upgrade in the drawing
        double temp=angle_end;angle_end=angle_start;angle_start=temp;
        angle_end*=-1;angle_start*=-1;
        angle_end+=180.0;angle_start+=180.0;
        while(angle_end < 0.0){angle_end += 360.0;}while(angle_start < 0.0){angle_start += 360.0;}
        while(angle_end >= 360.0){angle_end -= 360.0;}while(angle_start >= 360.0){angle_start -= 360.0;}
        angle_end /= (180.0/SCLS_PI);angle_start /= (180.0/SCLS_PI);
        int needed_height = height;
        int needed_width = width;
        while(current_x < drew_radius_x) {
            // Update the coordinate
            current_x++;
            double current_x_proportion_border = current_x / drew_radius_x;
            double current_x_proportion = (current_x - border_radius) / (drew_radius_x - border_radius);
            int needed_x = start_x + current_x;
            int needed_y_left = (start_y_left + y_center_offset_left);
            int needed_y_right = (start_y_right - y_center_offset_left);
            y_center_offset_left += y_center_offset_left_to_add;

            // Get the needed x/y
            double angle_border = circle_angle_at_x(current_x_proportion_border - 1.0) + (left_angle - SCLS_PI);
            Point_2D director_border_before_angle = oval_vector_x(radius_x, radius_y, angle_border) * radius_x;
            Point_2D director_border_top = director_border_before_angle.rotated(angle * (180.0/SCLS_PI));
            int first_y_top = director_border_top.y() - (drew_y_center - y_center_offset_left);

            // Get the needed limits
            int limit_top_right = 0;
            if(angle_end != 0 && angle_end <= 90.0){
                /*double angle_border = (circle_angle_at_x(static_cast<double>(radius_x) * current_x_proportion_border, radius_x, radius_x) + left_angle) - SCLS_PI;
                Point_2D director_border_before_angle = oval_vector_x(radius_x, radius_y, angle_border);
                Point_2D director_border_top = director_border_before_angle.rotated(angle * (180.0/SCLS_PI));
                double circle_y = (director_border_top.y() * (radius_x)) - (drew_y_center - y_center_offset_left);//*/

                double x_hypothenus = ((radius_x - radius_x * current_x_proportion_border) / std::abs(std::cos(angle_end)));
                if(x_hypothenus != 0){
                    double y_hypothenus = x_hypothenus * (radius_y / radius_x);
                    Point_2D director_limit_top_before_angle = oval_vector_x(x_hypothenus, y_hypothenus, angle_end + angle) * x_hypothenus;
                    Point_2D director_limit_top = director_limit_top_before_angle.rotated(angle * (180.0/SCLS_PI));
                    double circle_y = (director_limit_top.y());
                    limit_top_right = circle_y - (drew_y_center - y_center_offset_left);
                    //std::cout << "G " << circle_y << " " << angle << " " << (radius_x * current_x_proportion_border) << " " << angle_end << " " << x_hypothenus << " " << y_hypothenus << " " << director_limit_top.y() << " " << director_limit_top_before_angle.y() << " " << limit_top_right << std::endl;
                }
            }

            // Draw each needed pixels
            // Set the last y
            #define CHECK_ANGLE(needed_angle) ((angle_end == angle_start) || (needed_angle >= angle_start && needed_angle <= angle_end) || (angle_start > angle_end && (needed_angle <= angle_end || needed_angle >= angle_start)))
            #define CHECK_ANGLE_END(angle_1, angle_2) (CHECK_ANGLE(angle_1) && !CHECK_ANGLE(angle_2) && std::abs(angle_start - angle_1) > std::abs(angle_end - angle_1))
            #define CHECK_ANGLE_START(angle_1, angle_2) (CHECK_ANGLE(angle_1) && !CHECK_ANGLE(angle_2) && std::abs(angle_start - angle_1) < std::abs(angle_end - angle_1))
            double needed_angle_bottom = SCLS_PI;double needed_angle_top = SCLS_PI;
            bool in_border_bottom = true;int last_y_bottom = 0;int last_y_top = 0;
            if(needed_x >= start_x_inner) {
                needed_angle_bottom = circle_angle_at_x(static_cast<double>(radius_x - border_radius) * current_x_proportion, radius_x - border_radius, radius_x - border_radius) + left_angle;
                needed_angle_top = circle_angle_at_x(static_cast<double>(radius_x - border_radius) * current_x_proportion, radius_x - border_radius, radius_x - border_radius) + left_angle;

                // Update angles
                needed_angle_bottom -= SCLS_PI;needed_angle_top -= SCLS_PI;
                while(needed_angle_bottom < 0){needed_angle_bottom += SCLS_PI * 2.0;}while(needed_angle_bottom >= SCLS_PI * 2.0){needed_angle_bottom -= SCLS_PI * 2.0;}
                while(needed_angle_top < 0){needed_angle_top += SCLS_PI * 2.0;}while(needed_angle_top >= SCLS_PI * 2.0){needed_angle_top -= SCLS_PI * 2.0;}

                Point_2D director_bottom = oval_vector_x(radius_x - border_radius, radius_y - border_radius, needed_angle_bottom).rotated(angle * (180.0/SCLS_PI));
                Point_2D director_top = oval_vector_x(radius_x - border_radius, radius_y - border_radius, needed_angle_top).rotated(angle * (180.0/SCLS_PI));

                double current_border_radius = std::sqrt(std::pow(radius_x, 2) + std::pow(radius_x - border_radius, 2) - 2 * radius_x * (radius_x - border_radius) * std::cos((needed_angle_bottom - angle_border) / SCLS_PI));
                last_y_bottom = (director_bottom.y() * (radius_x - current_border_radius)) - (drew_y_center - y_center_offset_left);
                last_y_top = (director_top.y() * (radius_x - current_border_radius)) - (drew_y_center - y_center_offset_left);
                in_border_bottom = false;
            }

            // Border part
            if(border_alpha > 0 && border_radius > 0) {
                // Draw the circle border
                int y_height_base = (first_y_top - last_y_top);
                // Left part of the border
                needed_x = (x_center - drew_radius_x) + current_x;
                if(needed_x >= 0 && needed_x < needed_width) {
                    // Left-bottom of the circle border
                    int i = 0;double current_angle = SCLS_PI - needed_angle_bottom;double current_angle_border = SCLS_PI - angle_border;int y_height = y_height_base;
                    if(CHECK_ANGLE_END(current_angle, current_angle_border)){y_height = round(static_cast<double>(y_height_base) * ((angle_end - current_angle) / (current_angle_border - current_angle)));}
                    if(CHECK_ANGLE_START(current_angle_border, current_angle)){i = round(static_cast<double>(y_height_base) * ((angle_start - current_angle) / (current_angle_border - current_angle)));}
                    for(;i<y_height;i++) {
                        int current_y = (needed_y_left - (last_y_top + i));
                        if((CHECK_ANGLE(current_angle) || CHECK_ANGLE(current_angle_border)) && current_y >= 0 && current_y < needed_height) {
                            to_return[current_to_return] = scls::Point_2D(needed_x, current_y);current_to_return++;
                        }
                    }
                    // Left-top of the circle border
                    i = 0;current_angle = SCLS_PI + needed_angle_top;current_angle_border = SCLS_PI + angle_border;y_height = y_height_base;
                    if(CHECK_ANGLE_END(current_angle_border, current_angle)){i = round(static_cast<double>(y_height_base) * ((angle_end - current_angle) / (current_angle_border - current_angle)));}
                    if(CHECK_ANGLE_START(current_angle, current_angle_border)){y_height = round(static_cast<double>(y_height_base) * ((angle_start - current_angle) / (current_angle_border - current_angle)));}
                    for(;i<y_height;i++) {
                        int current_y = (needed_y_left + (last_y_top + i));
                        if((CHECK_ANGLE(current_angle) || CHECK_ANGLE(current_angle_border)) && current_y >= 0 && current_y < needed_height) {
                            to_return[current_to_return] = scls::Point_2D(needed_x, current_y);current_to_return++;
                        }
                    }
                }

                // Right part of the border
                needed_x = (x_center + drew_radius_x) - current_x;
                if(needed_x >= 0 && needed_x < needed_width){
                    // Right-bottom of the circle border
                    int i = 0;double current_angle = needed_angle_bottom;double current_angle_border = angle_border;int y_height = y_height_base;
                    if(CHECK_ANGLE_END(current_angle_border, current_angle) || (in_border_bottom && current_angle_border < angle_end)){
                        double x_hypothenus = (static_cast<double>(radius_x - current_x) / std::abs(std::cos(angle_end)));
                        double y_hypothenus = radius_y - (radius_x - x_hypothenus);//x_hypothenus * (radius_y / radius_x);
                        //double corrected_angle = oval_angle_at_x(x_hypothenus, y_hypothenus, radius_x - current_x);
                        //double proportion = oval_radius_proportion_y(radius_x, radius_y, corrected_angle);
                        double circle_y = (std::abs(std::sin(angle_end)) * y_hypothenus);
                        i = circle_y - last_y_top; // * (static_cast<double>(radius_y - border_radius) / static_cast<double>(radius_x - border_radius))

                        //std::cout << "G " << i << " " << last_y_top << " " << circle_angle_at_x(static_cast<double>(radius_x - current_x) / (x_hypothenus)) << " " << (radius_x - current_x) << " " << radius_x << " " << radius_y << " " << x_hypothenus << " " << y_hypothenus << " " << (static_cast<double>(radius_y - border_radius) / static_cast<double>(radius_x - border_radius)) << std::endl;
                        //fill_circle(x_center, y_center, x_hypothenus, y_hypothenus, scls::Color(0, ((radius_x - current_x) - 260) * 20, 0), 0, scls::Color(0, 0, 0, 0));
                    }
                    if(CHECK_ANGLE_START(current_angle, current_angle_border)){y_height = round(static_cast<double>(y_height_base) * ((angle_start - current_angle) / (current_angle_border - current_angle)));}
                    for(;i<y_height;i++) {
                        int current_y = (needed_y_right - (last_y_top + i));
                        if((CHECK_ANGLE(current_angle) || CHECK_ANGLE(current_angle_border)) && current_y >= 0 && current_y < needed_height) {
                            to_return[current_to_return] = scls::Point_2D(needed_x, current_y);current_to_return++;
                        }
                    }
                    // Right-top of the circle border
                    i = 0; current_angle = SCLS_PI * 2.0 - needed_angle_top;current_angle_border = SCLS_PI * 2.0 - angle_border;y_height = y_height_base;
                    if(CHECK_ANGLE_END(current_angle, current_angle_border)){
                        double end_hypothenus = (static_cast<double>(radius_x - current_x) / std::abs(std::cos(angle_end)));
                        y_height = std::abs(std::sin(angle_end)) * end_hypothenus - last_y_top;
                    }
                    if(CHECK_ANGLE_START(current_angle_border, current_angle)){i = round(static_cast<double>(y_height_base) * ((angle_start - current_angle) / (current_angle_border - current_angle)));}
                    for(;i<y_height;i++) {
                        int current_y = (needed_y_right + (last_y_top + i));
                        if((CHECK_ANGLE(current_angle) || CHECK_ANGLE(current_angle_border)) && current_y >= 0 && current_y < needed_height) {
                            to_return[current_to_return] = scls::Point_2D(needed_x, current_y);current_to_return++;
                        }
                    }
                }
            }

            // Inner part
            bool fill_bottom = true;
            if(alpha > 0) {
                // Fill the circle
                int i = 0;
                // Left of the circle
                needed_x = (x_center - drew_radius_x) + current_x;
                if(needed_x >= 0 && needed_x < needed_width){
                    for(;i < last_y_bottom;i++) {
                        int current_y = (needed_y_left + i);
                        if(current_y >= 0 && current_y < needed_height && fill_bottom) {
                            to_return[current_to_return] = scls::Point_2D(needed_x, current_y);current_to_return++;
                        }
                    }
                    for(i = 0;i < last_y_top;i++) {
                        int current_y = (needed_y_left - i);
                        if(current_y >= 0 && current_y < needed_height) {
                            to_return[current_to_return] = scls::Point_2D(needed_x, current_y);current_to_return++;
                        }
                    }
                }

                // Right of the circle
                needed_x = (x_center + drew_radius_x) - current_x;
                if(needed_x >= 0 && needed_x < needed_width){
                    // Bottom right part of the circle
                    for(i = 0;i < last_y_bottom;i++) {
                        int current_y = (needed_y_right + i);
                        if(current_y >= 0 && current_y < needed_height && fill_bottom) {
                            to_return[current_to_return] = scls::Point_2D(needed_x, current_y);current_to_return++;
                        }
                    }

                    // Top right part of the circle
                    for(i = limit_top_right;i < last_y_top;i++) {
                        int current_y = (needed_y_right - i);
                        if(current_y >= 0 && current_y < needed_height) {
                            to_return[current_to_return] = scls::Point_2D(needed_x, current_y);current_to_return++;
                        }
                    }
                }
            }
        }

        // Format the result
        std::vector<scls::Point_2D> final_to_return = std::vector<scls::Point_2D>(current_to_return);
        for(int i = 0;i<static_cast<int>(final_to_return.size());i++){final_to_return[i] = to_return.at(i);}
        return final_to_return;
    }

    // Points in a line
    std::vector<scls::Point_2D> line_points(int x_1, int y_1, int x_2, int y_2, unsigned short line_width) {
        std::vector<scls::Point_2D> to_return;
        int height = 2000;int width = 2000;

        // Only case which the algorithm does not work correctly
        if(x_1 == x_2) {
            // Check the X position
            if(x_1 + static_cast<int>(line_width) < 0 || x_1 >= width) return std::vector<scls::Point_2D>();
            if(y_2 < y_1) {
                y_2 += y_1;
                y_1 = y_2 - y_1;
                y_2 = y_2 - y_1;
            } y_2++;
            x_1 -= line_width / 2; y_1 -= line_width / 2; x_2 -= line_width / 2; y_2 -= line_width / 2;

            // Get the line
            for(int i = 0;i<(y_2-y_1)+line_width;i++){
                for(int j = 0;j<line_width;j++){
                    to_return.push_back(scls::Point_2D(x_1 + j, y_1 + i));
                }
            }
        }
        else if(y_1 == y_2) {
            // Check the Y position
            if(y_1 < 0 || y_1 >= height) return std::vector<scls::Point_2D>();
            if(x_2 < x_1) {
                x_2 += x_1;
                x_1 = x_2 - x_1;
                x_2 = x_2 - x_1;
            } x_2++;
            x_1 -= line_width / 2; y_1 -= line_width / 2; x_2 -= line_width / 2; y_2 -= line_width / 2;

            // Get the line
            for(int i = 0;i<(x_2-x_1)+line_width;i++){
                for(int j = 0;j<line_width;j++){
                    to_return.push_back(scls::Point_2D(x_1 + i, y_1 + j));
                }
            }
        }
        else {
            x_2++; y_2++;
            double distance_x = static_cast<double>(x_2 - x_1);
            double distance_y = static_cast<double>(y_2 - y_1);

            double x_y_ratio = distance_x / distance_y;

            if (abs(x_y_ratio) < 1) {
                // Normalize positions
                if (y_1 > y_2) {
                    int temp = y_1;
                    y_1 = y_2;
                    y_2 = temp;
                    temp = x_1;
                    x_1 = x_2;
                    x_2 = temp;
                }

                double actual_x = x_1;
                double actual_y = y_1;

                // Check the positions
                if(actual_y < 0) {actual_x += x_y_ratio * std::abs(actual_y);actual_y=0;}
                if(y_2 >= height) y_2 = height - 1;
                // Draw the line
                while (actual_y < y_2) {
                    actual_y++;
                    actual_x += x_y_ratio;
                    to_return.push_back(scls::Point_2D(actual_x, actual_y));
                }
            }
            else {
                // Normalize positions
                if (x_1 > x_2) {
                    int temp = x_1;
                    x_1 = x_2;
                    x_2 = temp;
                    temp = y_1;
                    y_1 = y_2;
                    y_2 = temp;
                }

                double actual_x = x_1;
                double actual_y = y_1;

                // Check the positions
                double y_x_ratio = distance_y / distance_x;
                if(actual_x < 0) {actual_y += y_x_ratio * std::abs(actual_x);actual_x=0;}
                if(x_2 >= width) x_2 = width - 1;
                // Draw the line
                while (actual_x < x_2) {
                    actual_y += y_x_ratio;
                    actual_x++;
                    to_return.push_back(scls::Point_2D(actual_x, actual_y));
                }
            }
        }

        return to_return;
    }

    //*********
	//
	// The __Image_Base class
	//
	//*********

	// All __Image_Base constructors
    // __Image_Base most basic constructor
    __Image_Base::__Image_Base() {};
    // __Image_Base constructor with a path
    __Image_Base::__Image_Base(std::string path) : __Image_Base() {std::shared_ptr<__Image_Error> final_error = load_from_path(path);if(final_error.get()->has_error()) print("Warning", "SCLS Image \"Michelangelo\"", final_error.get()->to_std_string());};
    // __Image_Base constructor from scratch
    __Image_Base::__Image_Base(unsigned short width, unsigned short height, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, unsigned int color_type) {a_color_type = color_type;a_height = height;a_width = width;fill(red, green, blue, alpha);};
    __Image_Base::__Image_Base(unsigned short width, unsigned short height, Color color, unsigned int color_type) : __Image_Base(width, height, color.red(), color.green(), color.blue(), color.alpha(), color_type) {}
    __Image_Base::__Image_Base(unsigned short width, unsigned short height):__Image_Base(width, height, scls::Color(255, 255, 255)){}
    // __Image_Base copy constructor
    __Image_Base::__Image_Base(__Image_Base& image_copy) : __Image_Base(image_copy.width(), image_copy.height(), Color(0, 0, 0, 0)) {paste(&image_copy, 0, 0);}
    __Image_Base::__Image_Base(__Image_Base* image_copy) : __Image_Base(image_copy->width(), image_copy->height(), Color(0, 0, 0, 0)) {paste(image_copy, 0, 0);}
    // PNG_Image destructor
    __Image_Base::~__Image_Base() { free_memory(); }

	// Linear gradient color for the Image class circle
    Color fill_circle_gradient_linear(double distance, int radius, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
        return scls::Color(static_cast<double>(red) * std::abs(1.0 - distance / static_cast<double>(radius)), static_cast<double>(green) * std::abs(1.0 - distance / static_cast<double>(radius)), static_cast<double>(blue) * std::abs(1.0 - distance / static_cast<double>(radius)), static_cast<double>(alpha) * std::abs(1.0 - distance / static_cast<double>(radius)));
    }

    std::string __Image_Error::to_std_string() const {
        // Normal behavior
        if(!has_error()) return std::string("This image has no errors.");

        // Handles error
        if(value() == SCLS_IMAGE_ERROR_UNKNOW_FILE) { return std::string("The file at path \"") + path() + std::string("\" does not exist."); }
        else if(value() == SCLS_IMAGE_PNG_ERROR_BAD_FILE_SIGNATURE) { return std::string("The file at path \"") + path() + std::string("\" has no the signature of a PNG file."); }
        else if(value() == SCLS_IMAGE_PNG_ERROR_BIT_DEPHT) { return std::string("The file at path \"") + path() + std::string("\" has an unsupported bit depht (") + std::to_string(bit_depht()) + std::string(")."); }
        else if(value() == SCLS_IMAGE_PNG_ERROR_PLTE_CHUNK) { return std::string("The file at path \"") + path() + std::string("\" use the unsupported chunk PLTE (palette colors)."); }
        else if(value() == SCLS_IMAGE_PNG_ERROR_UNKNOW_COLOR_TYPE) { return std::string("The file at path \"") + path() + std::string("\" use the unsupported color type \"") + std::to_string(color_type()) + std::string("\"."); }

        return std::string("An unknow error of ID \"") + std::to_string(value()) + std::string("\" occured.");
    };

    // Copies this image and returns the result
    std::shared_ptr<__Image_Base> __Image_Base::copy_image(){std::shared_ptr<__Image_Base> to_return = std::make_shared<__Image_Base>();to_return.get()->a_height = a_height;to_return.get()->a_width = a_width;to_return.get()->free_memory();to_return.get()->a_pixels.reset(new Bytes_Set(0));to_return.get()->a_pixels.get()->add_datas(*a_pixels.get());return to_return;};

    // Create the memory needed
    void __Image_Base::__create_memory(std::shared_ptr<Bytes_Set>& current_datas, int width, int height){__free_memory(current_datas);current_datas.reset(new Bytes_Set(__buffer_size(width, height)));};
    void __Image_Base::create_memory(){__create_memory(a_pixels, width(), height());};
    // Delete the pixels in the memory
    void __Image_Base::__free_memory(std::shared_ptr<Bytes_Set>& current_datas) {current_datas.reset();};
    void __Image_Base::free_memory() {__free_memory(a_pixels);};

    // Fill the image with one color
    void __Image_Base::fill(Color color) { fill(color.red(), color.green(), color.blue(), color.alpha());};
    void __Image_Base::fill(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {__fill(a_pixels, red, green, blue, alpha, width(), height());};
    void __Image_Base::__fill(std::shared_ptr<Bytes_Set>& current_datas, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, int width, int height) {
        __create_memory(current_datas, width, height);
        unsigned int current_thread_position = 0;
        unsigned int pixel_by_thread = floor(static_cast<double>((width * height) / static_cast<double>(a_thread_number_for_filling)));

        // Create each threads
        if(a_thread_number_for_filling > 0) {
            std::vector<std::thread*> threads = std::vector<std::thread*>();
            for(unsigned short i = 0;i<a_thread_number_for_filling - 1;i++) {
                std::thread* current_thread = new std::thread(&__Image_Base::__fill_pixel_part, this, current_datas, current_thread_position, pixel_by_thread, red, green, blue, alpha);
                threads.push_back(current_thread);
                current_thread_position += pixel_by_thread;
            }
            std::thread* current_thread = new std::thread(&__Image_Base::__fill_pixel_part, this, current_datas, current_thread_position, (width * height) - current_thread_position, red, green, blue, alpha);
            threads.push_back(current_thread);

            // Wait for each threads
            for(int i = 0;i<static_cast<int>(threads.size());i++) {
                threads[i]->join();
                delete threads[i]; threads[i] = 0;
            } threads.clear();
        }
        else {__fill_pixel_part(current_datas, 0, width * height, red, green, blue, alpha);}
    }

    // Fill a part of pixel
    void __Image_Base::__fill_pixel_part(std::shared_ptr<Bytes_Set> current_datas, unsigned int start_position, unsigned int length, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
        if(color_type() == SCLS_IMAGE_RGBA) {
            int needed_components = components(); int multiplier = 1;
            for (unsigned int i = 0; i < length; i++) {
                current_datas.get()->set_data_at_directly((start_position + i) * multiplier * needed_components, red);
                current_datas.get()->set_data_at_directly((start_position + i) * multiplier * needed_components + 1 * multiplier, green);
                current_datas.get()->set_data_at_directly((start_position + i) * multiplier * needed_components + 2 * multiplier, blue);
                current_datas.get()->set_data_at_directly((start_position + i) * multiplier * needed_components + 3 * multiplier,  alpha);
            }
        }
        else if(color_type() == SCLS_IMAGE_RGB) {
           for (unsigned int i = 0; i < length; i++) {
                current_datas.get()->set_data_at((start_position + i) * 3, red);
                current_datas.get()->set_data_at((start_position + i) * 3 + 1, green);
                current_datas.get()->set_data_at((start_position + i) * 3 + 2, blue);
            }
        }
    };

    // Force a pixel to change its value
    void __Image_Base::force_pixel(unsigned short x, unsigned short y, Color color) {force_pixel(x, y, color.red(), color.green(), color.blue(), color.alpha());}
    void __Image_Base::force_pixel(unsigned short x, unsigned short y, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, int width_point) {
        if (width_point == 0) return;
        else if (width_point == 1) {
            // Check the position
            if (x < 0 || y < 0 || x >= width() || y >= height()) {
                print("Warning", "SCLS Image", "The position (" + std::to_string(x) + "; " + std::to_string(y) + ") you want to set is out of the image.");
                return;
            }

            unsigned char multiplier = (bit_depht() / 8.0);
            unsigned int position = (y * width() + x) * components() * (bit_depht() / 8.0);

            if(color_type() == SCLS_IMAGE_RGBA) {set_pixel_rgba_directly(position, red, green, blue, alpha, multiplier);}
            else if(color_type() == SCLS_IMAGE_RGB) {
                a_pixels->set_data_at_directly(position, red);
                a_pixels->set_data_at_directly(position + multiplier, green);
                a_pixels->set_data_at_directly(position + 2 * multiplier, blue);
            }
        }
        else {fill_rect_force(static_cast<int>(static_cast<double>(x) - static_cast<double>(width_point) / 2.0), static_cast<int>(static_cast<double>(y) - (static_cast<double>(width_point)) / 2.0), width_point, width_point, red, green, blue, alpha);}
    }
    void __Image_Base::force_pixel(unsigned short x, unsigned short y, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
        if (x < 0 || y < 0 || x >= width() || y >= height()) {
            print("Warning", "SCLS Image", "The position (" + std::to_string(x) + "; " + std::to_string(y) + ") you want to force is out of the image.");
            return;
        }

        unsigned int multiplier = (bit_depht() / 8.0);
        unsigned int position = (y * width() + x) * components() * multiplier;
        a_pixels->set_data_at(position, normalize_value(red, 0, 255));
        a_pixels->set_data_at(position + multiplier, normalize_value(green, 0, 255));
        a_pixels->set_data_at(position + 2 * multiplier, normalize_value(blue, 0, 255));
        if(color_type() == 6) a_pixels->set_data_at(position + 3 * multiplier, normalize_value(alpha, 0, 255));
    }

    // Returns the data filtered (like in PNG format)
    std::shared_ptr<Bytes_Set> __Image_Base::datas_filtered() {
        std::shared_ptr<Bytes_Set> to_return = std::make_shared<Bytes_Set>(datas()->datas_size() + height());
        unsigned char components_size = components();
        unsigned int height_used = height();
        unsigned int position = 0;
        unsigned int width_used = width();
        for (unsigned int i = 0; i < height_used; i++) {
            to_return.get()->set_data_at(position, 0); position++;
            for (unsigned int j = 0; j < width_used * components_size; j++) {
                to_return.get()->set_data_at(position, datas()->data_at(i * width_used * components_size + j)); position++;
            }
        } return to_return;
    };

    // Loads the image from a path
    std::shared_ptr<__Image_Error> __Image_Base::load_from_path(std::string path) {
        std::shared_ptr<__Image_Error> to_return = std::make_shared<__Image_Error>();
        to_return.get()->set_path(path);
        if (std::filesystem::exists(path) && !std::filesystem::is_directory(path)) {
            // Create the necessary things to read the PNG file
            Bytes_Set file = Bytes_Set();
            file.load_from_file(path);

            // Check if the signature is correct (137 80 78 71 13 10 26 10 for png files)
            std::string file_signature = file.extract_string(8);
            std::vector<unsigned char> signature = png_signature();
            for (int i = 0; i < static_cast<int>(signature.size()); i++) {
                if (signature[i] != static_cast<unsigned char>(file_signature[i])) {
                    to_return.get()->set_value(SCLS_IMAGE_ERROR_UNKNOW_FILE);
                    return to_return;
                }
            }

            _load_png_file(&file, to_return);
        }
        else to_return.get()->set_value(SCLS_IMAGE_ERROR_UNKNOW_FILE);
        return to_return;
    };

    // Get datas about a specific pixel
    Color __Image_Base::pixel_directly(unsigned int position, unsigned int multiplier) {Color to_return = Color(255, 255, 255);to_return.set_rgb(a_pixels->data_at_directly(position), a_pixels->data_at_directly(position + multiplier), a_pixels->data_at_directly(position + 2 * multiplier));return to_return;}
    Color __Image_Base::pixel_rgba_directly(unsigned int position, unsigned int multiplier) {Color to_return = Color(255, 255, 255);to_return.set_rgba(a_pixels->data_at_directly(position), a_pixels->data_at_directly(position + multiplier), a_pixels->data_at_directly(position + 2 * multiplier), a_pixels->data_at_directly(position + 3 * multiplier));return to_return;}
    Color __Image_Base::pixel(unsigned short x, unsigned short y) {
        Color to_return(0, 0, 0);
        if (x >= 0 && x < width() && y >= 0 && y < height()) {
            unsigned char multiplier = (bit_depht() / 8.0);
            unsigned int position = (y * width() + x) * components() * (bit_depht() / 8.0);

            if(color_type() == SCLS_IMAGE_RGBA) {
                to_return.set_rgba(a_pixels->data_at(position),
                                   a_pixels->data_at(position + multiplier),
                                   a_pixels->data_at(position + 2 * multiplier),
                                   a_pixels->data_at(position + 3 * multiplier));
            }
            else if(color_type() == SCLS_IMAGE_RGB) {
                to_return.set_rgb(a_pixels->data_at(position),
                                  a_pixels->data_at(position + multiplier),
                                  a_pixels->data_at(position + 2 * multiplier));
            }
        }
        else {
            print("Warning", "SCLS Image", "The position (" + std::to_string(x) + "; " + std::to_string(y) + ") you want to get is out of the image.");
        }
        return to_return;
    };
    Color __Image_Base::pixel_by_number(unsigned int position) {
        unsigned char multiplier = (bit_depht() / 8.0);
        position *= components() * (bit_depht() / 8.0);
        Color to_return = Color(255, 255, 255);

        if(color_type() == 6) {to_return = pixel_rgba_directly(position, multiplier);}
        else {to_return = pixel_directly(position, multiplier);}

        return to_return;
    };

    // Replaces a color with an other on the image
    void __Image_Base::replace_color(Color old_color, Color new_color) {
        int needed_height = height(); int needed_width = width();
        if(color_type() == 6) {
            for(int i = 0;i<needed_height*needed_width;i++){
                int current_position = i * components();
                if(pixel_rgba_directly(current_position, 1) == old_color){set_pixel_rgba_directly(current_position, new_color.red(), new_color.green(), new_color.blue(), new_color.alpha(), 1);}
            }
        }
    }

    // Set datas about a specific pixel
    void __Image_Base::set_pixel_directly(unsigned int position, unsigned char value){a_pixels->set_data_at_directly(position, value);};
    void __Image_Base::set_pixel_directly(unsigned int position, unsigned char red, unsigned char green, unsigned char blue, unsigned char multiplier){a_pixels->set_data_at_directly(position, red);a_pixels->set_data_at_directly(position + multiplier, green);a_pixels->set_data_at_directly(position + 2 * multiplier, blue);};
    void __Image_Base::set_pixel_rgba_directly(unsigned int position, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, unsigned char multiplier){a_pixels->set_data_at_directly(position, red);a_pixels->set_data_at_directly(position + multiplier, green);a_pixels->set_data_at_directly(position + 2 * multiplier, blue);a_pixels->set_data_at_directly(position + 3 * multiplier,  alpha);};
    void __Image_Base::set_pixel(int x, int y, Color color, unsigned short width) { set_pixel(x, y, color.red(), color.green(), color.blue(), color.alpha(), width); }
    void __Image_Base::set_pixel_by_number(unsigned int position, Color color) { set_pixel_by_number(position, color.red(), color.green(), color.blue());};
    void __Image_Base::set_pixel(int x, int y, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, unsigned short width_point) {
        if (width_point == 0) return;
        else if (width_point == 1) {
            // Check the position
            if (x < 0 || y < 0 || x >= width() || y >= height()) {
                print("Warning", "SCLS Image", "The position (" + std::to_string(x) + "; " + std::to_string(y) + ") you want to set is out of the image.");
                return;
            }

            unsigned char multiplier = (bit_depht() / 8.0);
            unsigned int position = (y * width() + x) * components() * (bit_depht() / 8.0);

            if(color_type() == SCLS_IMAGE_RGBA) {paste_pixel_rgba_directly(position, red, green, blue, alpha, multiplier);}
            else if(color_type() == SCLS_IMAGE_RGB) {
                a_pixels->set_data_at_directly(position, red);
                a_pixels->set_data_at_directly(position + multiplier, green);
                a_pixels->set_data_at_directly(position + 2 * multiplier, blue);
            }
        }
        else {fill_rect(static_cast<int>(static_cast<double>(x) - static_cast<double>(width_point) / 2.0), static_cast<int>(static_cast<double>(y) - (static_cast<double>(width_point)) / 2.0), width_point, width_point, red, green, blue, alpha);}
    }
    void __Image_Base::set_pixel_alpha(unsigned short x, unsigned short y, unsigned char alpha) {
        if (x < 0 || y < 0 || x >= width() || y >= height()) {
            print("Warning", "SCLS Image", "The position (" + std::to_string(x) + "; " + std::to_string(y) + ") you want to set the alpha is out of the image.");
            return;
        }

        unsigned char multiplier = (bit_depht() / 8.0);
        unsigned int position = (y * width() + x) * components() * (bit_depht() / 8.0);
        if(color_type() == 6){alpha = normalize_value(alpha, 0, 255);a_pixels->set_data_at(position + 3 * multiplier,  alpha);}
    }
    void __Image_Base::set_pixel_blue(unsigned short x, unsigned short y, unsigned char blue, unsigned char alpha) {
        if (x < 0 || y < 0 || x >= width() || y >= height()) {
            print("Warning", "SCLS Image", "The position (" + std::to_string(x) + "; " + std::to_string(y) + ") you want to set the blue is out of the image.");
            return;
        }

        unsigned char multiplier = (bit_depht() / 8.0);
        unsigned int position = (y * width() + x) * components() * (bit_depht() / 8.0);
        if(color_type() == 6){
            Color color = pixel(x, y);

            float alpha_f = normalize_value(alpha, 0, 255) / 255.0;
            float blue_f = normalize_value(blue, 0, 255);

            // Calculate alpha
            alpha = normalize_value(alpha, 0, 255); if(color.alpha() > alpha) alpha = color.alpha();
            blue = alpha_f * blue_f + (1.0 - alpha_f) * static_cast<float>(color.blue());

            a_pixels->set_data_at(position + 2 * multiplier, blue);
            a_pixels->set_data_at(position + 3 * multiplier, alpha);
        }
        else{a_pixels->set_data_at(position + 2 * multiplier, blue);}
    }
    void __Image_Base::set_pixel_by_number(unsigned int position, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
        unsigned char multiplier = (bit_depht() / 8.0);
        position *= components() * (bit_depht() / 8.0);
        if(color_type() == 6) {
            Color color = pixel_by_number(position);

            float alpha_f = normalize_value(alpha, 0, 255) / 255.0;
            float blue_f = normalize_value(blue, 0, 255);
            float green_f = normalize_value(green, 0, 255);
            float red_f = normalize_value(red, 0, 255);

            // Calculate alpha
            alpha = normalize_value(alpha, 0, 255); if(color.alpha() > alpha) alpha = color.alpha();
            blue = alpha_f * blue_f + (1.0 - alpha_f) * static_cast<float>(color.blue());
            red = alpha_f * red_f + (1.0 - alpha_f) * static_cast<float>(color.red());
            green = alpha_f * green_f + (1.0 - alpha_f) * static_cast<float>(color.green());

            a_pixels->set_data_at(position, red);
            a_pixels->set_data_at(position + multiplier, green);
            a_pixels->set_data_at(position + 2 * multiplier, blue);
            a_pixels->set_data_at(position + 3 * multiplier,  alpha);
        }
        else {a_pixels->set_data_at(position, red);a_pixels->set_data_at(position + multiplier, green);a_pixels->set_data_at(position + 2 * multiplier, blue);}
    };
    void __Image_Base::set_pixel_green(unsigned short x, unsigned short y, unsigned char green, unsigned char alpha) {
        if (x < 0 || y < 0 || x >= width() || y >= height()) {
            print("Warning", "SCLS Image", "The position (" + std::to_string(x) + "; " + std::to_string(y) + ") you want to set the green is out of the image.");
            return;
        }

        unsigned char multiplier = (bit_depht() / 8.0);
        unsigned int position = (y * width() + x) * components() * (bit_depht() / 8.0);
        if(color_type() == 6)
        {
            Color color = pixel(x, y);

            float alpha_f = normalize_value(alpha, 0, 255) / 255.0;
            float green_f = normalize_value(green, 0, 255);

            // Calculate alpha
            alpha = normalize_value(alpha, 0, 255); if(color.alpha() > alpha) alpha = color.alpha();
            green = alpha_f * green_f + (1.0 - alpha_f) * static_cast<float>(color.green());

            a_pixels->set_data_at(position + multiplier, green);
            a_pixels->set_data_at(position + 3 * multiplier, alpha);
        }
        else{a_pixels->set_data_at(position + multiplier, green);}
    }
    void __Image_Base::set_pixel_red(unsigned short x, unsigned short y, unsigned char red, unsigned char alpha) {
        if (x < 0 || y < 0 || x >= width() || y >= height()) {
            print("Warning", "SCLS Image", "The position (" + std::to_string(x) + "; " + std::to_string(y) + ") you want to set the red is out of the image.");
            return;
        }

        unsigned char multiplier = (bit_depht() / 8.0);
        unsigned int position = (y * width() + x) * components() * (bit_depht() / 8.0);
        if(color_type() == 6) {
            Color color = pixel(x, y);

            float alpha_f = normalize_value(alpha, 0, 255) / 255.0;
            float red_f = normalize_value(red, 0, 255);

            // Calculate alpha
            alpha = normalize_value(alpha, 0, 255); if(color.alpha() > alpha) alpha = color.alpha();
            red = alpha_f * red_f + (1.0 - alpha_f) * static_cast<float>(color.red());

            a_pixels->set_data_at(position, red);
            a_pixels->set_data_at(position + 3 * multiplier,  alpha);
        }
        else{a_pixels->set_data_at(position, red);}
    };
    void __Image_Base::set_pixel_rgba_directly_with_alpha(unsigned int position, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, unsigned char multiplier) {
        // Process the color
        Color color = pixel_rgba_directly(position, multiplier);
        double alpha_f = normalize_value(alpha, 0, 255) / 255.0;
        double blue_f = normalize_value(blue, 0, 255);
        double green_f = normalize_value(green, 0, 255);
        double red_f = normalize_value(red, 0, 255);
        // Calculate alpha
        alpha = normalize_value(alpha, 0, 255); if(color.alpha() > alpha) alpha = color.alpha();
        blue = alpha_f * blue_f + (1.0 - alpha_f) * static_cast<double>(color.blue());
        red = alpha_f * red_f + (1.0 - alpha_f) * static_cast<double>(color.red());
        green = alpha_f * green_f + (1.0 - alpha_f) * static_cast<double>(color.green());
        // Set the color
        set_pixel_rgba_directly(position, red, green, blue, alpha, multiplier);
    }

    // Returns if the image use alpha or not
    bool __Image_Base::use_alpha() const {return color_type() == SCLS_IMAGE_RGBA;};

    // Getters and setters
    unsigned int __Image_Base::__buffer_size(int width, int height) const {return height * width * components() * static_cast<unsigned int>(static_cast<double>(bit_depht()) / 8.0);};
    unsigned int __Image_Base::buffer_size() const {return __buffer_size(width(), height());};
    unsigned char __Image_Base::components() const { if (color_type() == 6) return 4; return 3; };
    unsigned int __Image_Base::bit_depht() const { return a_bit_depth; };
    unsigned int __Image_Base::color_type() const { return a_color_type; };
    Bytes_Set* __Image_Base::datas() const { return a_pixels.get(); }
    unsigned int __Image_Base::flip_x_number() const {return a_flip_x_number;};
    int __Image_Base::height() const { return a_height; };
    void __Image_Base::reset(int new_width, int new_height, Color new_color){a_height=new_height;a_width=new_width;fill(new_color);}
    void __Image_Base::set_thread_number_for_filling(unsigned short new_thread_number) {a_thread_number_for_filling = new_thread_number;};
    void __Image_Base::set_thread_number_for_pasting(unsigned short new_thread_number) {a_thread_number_for_pasting = new_thread_number;};
    void __Image_Base::set_thread_number_for_pasting_text(unsigned short new_thread_number) {a_thread_number_for_pasting_text = new_thread_number;};
    int __Image_Base::width() const { return a_width; };

    //*********
    //
    // The Image class - PNG
    //
    //*********

    Bytes_Set* __Image_Base::datas_png() {
        Bytes_Set* datas = new Bytes_Set();
        unsigned int total_size = 8;

        // Add the signature
        std::vector<unsigned char> signature = png_signature();
        for(int i = 0;i<static_cast<int>(signature.size());i++) { datas->add_data(signature[i]); }

        // Create the IDHR chunk
        std::string name = "IHDR";
        unsigned int idhr_size = 13;
        unsigned int idhr_total_size = 25;
        datas->add_uint(idhr_size, true);
        datas->add_string(name);
        datas->add_uint(width(), true);
        datas->add_uint(height(), true);
        datas->add_data(bit_depht());
        datas->add_data(color_type());
        datas->add_data(compression_method());
        datas->add_data(filter_method());
        datas->add_data(0);
        datas->add_uint(crc_32b(datas->extract_datas(idhr_total_size - 8, total_size + 4), idhr_total_size - 8, "png"), true);
        total_size += idhr_total_size;

        // Create the pHYS chunk
        name = "pHYS";
        unsigned int phys_size = 9;
        unsigned int phys_total_size = 21;
        datas->add_uint(phys_size, true);
        datas->add_string(name);
        datas->add_uint(physical_width_ratio(), true);
        datas->add_uint(physical_height_ratio(), true);
        datas->add_data(physical_unit());
        datas->add_uint(crc_32b(datas->extract_datas(phys_total_size - 8, total_size + 4), phys_total_size - 8, "png"), true);
        total_size += phys_total_size;

        // Create the IDAT chunk
        name = "IDAT";
        unsigned int idat_size = 0;
        std::shared_ptr<Bytes_Set> idat_uncompressed = datas_filtered();
        char* idat_compressed = compress_binary(idat_uncompressed->datas(), idat_uncompressed->datas_size(), idat_size, 9);
        idat_uncompressed.reset();
        unsigned int idat_total_size = idat_size + 12;
        datas->add_uint(idat_size, true);
        datas->add_string(name);
        datas->add_datas(idat_compressed, idat_size);
        datas->add_uint(crc_32b(datas->extract_datas(idat_total_size - 8, total_size + 4), idat_total_size - 8, "png"), true);
        delete[] idat_compressed;
        total_size += idat_total_size;

        // Create the IEND chunk
        name = "IEND";
        unsigned int iend_total_size = 12;
        datas->add_uint(0, true);
        datas->add_string(name);
        datas->add_uint(crc_32b(datas->extract_datas(iend_total_size - 8, total_size + 4), iend_total_size - 8, "png"), true);
        total_size += iend_total_size;

        return datas;
    }

    // Get every chunks into a PNG image
    void __Image_Base::_load_all_chunks_from_png_file(Bytes_Set* file, std::shared_ptr<__Image_Error>& error_handler) {
        std::vector<_PNG_Chunk> chunks = std::vector<_PNG_Chunk>();
        if (file != 0) {
            // Create the necessary things to read the PNG file
            a_idat_chunk.clear();
            std::string name = "";
            unsigned int size_offset = 33;

            unsigned int iter = 0;

            // Check each chunks in the file
            Color background_color = Color(0, 0, 0, 0);
            while (name != "IEND") {
                unsigned int chunk_size = file->extract_uint(size_offset, true);
                name = file->extract_string(4, size_offset + 4);

                _PNG_Chunk chunk;
                chunk.position = size_offset + 8;
                chunk.name = name;
                chunk.size = chunk_size;
                size_offset += chunk_size + 12;
                chunks.push_back(chunk);

                if (name == "pHYs") { _load_png_pHYS_from_file(file, chunk, error_handler); }
                else if (name == "IDAT") { a_idat_chunk.push_back(chunk); }
                else if (name == "sRGB") { _load_png_sRGB_from_file(file, chunk, error_handler); }
                else if (name == "bKGD") { // background_color = _load_bKGD_from_file(file, chunk);
                }
                else if (name == "PLTE") { error_handler.get()->set_value(SCLS_IMAGE_ERROR_UNKNOW); }

                // Check for an error
                if(error_handler.get()->has_error()) return;

                iter++;
            }
            fill(background_color);

            // Not implemented yet
            if(!(color_type() == SCLS_IMAGE_RGB || color_type() == SCLS_IMAGE_RGBA)) { error_handler.get()->set_value(SCLS_IMAGE_PNG_ERROR_UNKNOW_COLOR_TYPE); return; }

            // Load IDAT chunks
            if (a_idat_chunk.size() > 0) { _load_png_IDAT_from_file(file, error_handler); }
            else error_handler.get()->set_value(SCLS_IMAGE_PNG_ERROR_NO_CHUNK);
        }
        else { fill(0, 0, 0); error_handler.get()->set_value(SCLS_IMAGE_ERROR_UNKNOW);}
    }

    // Loads the bKGD chunk from a path and returns the color
    Color __Image_Base::_load_bKGD_from_file(Bytes_Set* file, _PNG_Chunk chunk) {
        if (file != 0 && chunk.name == "bKGD" && chunk.size >= 6) {
            // Read into the chunk
            Color color(0, 0, 0);
            color.set_red(file->extract_ushort(chunk.position, true));
            color.set_green(file->extract_ushort(chunk.position + 2, true));
            color.set_blue(file->extract_ushort(chunk.position + 4, true));

            return color;
        }
        return Color(0, 0, 0);
    }
    // Load a IDAT chunk grom a path
    void __Image_Base::_load_png_IDAT_from_file_rgba(int component_size, int current_line_start_position, int last_line_start_position, int multiplier, int processed_data){
        if (a_filter_type == 1) { // Apply sub filtering
            for (int i = 1; i < width(); i++){
                Color color = pixel_rgba_directly(current_line_start_position + i * component_size, multiplier);
                Color current_color = pixel_rgba_directly(current_line_start_position + (i - 1) * component_size, multiplier);
                set_pixel_rgba_directly(current_line_start_position + i * component_size, current_color.red() + color.red(),
                                        current_color.green() + color.green(),
                                        current_color.blue() + color.blue(),
                                        current_color.alpha() + color.alpha(), multiplier);
            }
        }
        else if (a_filter_type == 2 && processed_data > width()) { // Apply up filtering
            for (int i = 0; i < width(); i++){
                Color color = pixel_rgba_directly(last_line_start_position + i * component_size, multiplier);
                Color current_color = pixel_rgba_directly(current_line_start_position + i * component_size, multiplier);
                set_pixel_rgba_directly(current_line_start_position + i * component_size, current_color.red() + color.red(),
                                             current_color.green() + color.green(),
                                             current_color.blue() + color.blue(),
                                             current_color.alpha() + color.alpha(), multiplier);
            }
        }
        else if (a_filter_type == 3) {
            // Apply average filtering
            if (processed_data > width()){
                for (int i = 0; i < width(); i++){
                    Color pixel1 = Color(0, 0, 0);
                    if (i == 0) {
                        pixel1.set_red(0);
                        pixel1.set_green(0);
                        pixel1.set_blue(0);
                        pixel1.set_alpha(0);
                    }
                    else {
                        pixel1 = pixel_rgba_directly(current_line_start_position + (i - 1) * component_size, multiplier);
                    }
                    Color pixel2 = pixel_rgba_directly(last_line_start_position + i * component_size, multiplier);
                    Color current_color = pixel_rgba_directly(current_line_start_position + i * component_size, multiplier);
                    set_pixel_rgba_directly(current_line_start_position + i * component_size, current_color.red() + static_cast<unsigned char>(floor((static_cast<double>(pixel1.red()) + static_cast<double>(pixel2.red())) / 2.0)),
                                                     current_color.green() + static_cast<unsigned char>(floor((static_cast<double>(pixel1.green()) + static_cast<double>(pixel2.green())) / 2.0)),
                                                     current_color.blue() + static_cast<unsigned char>(floor((static_cast<double>(pixel1.blue()) + static_cast<double>(pixel2.blue())) / 2.0)),
                                                     current_color.alpha() + static_cast<unsigned char>(floor((static_cast<double>(pixel1.alpha()) + static_cast<double>(pixel2.alpha())) / 2.0)), multiplier);
                }
            }
        }
        else if (a_filter_type == 4 && processed_data > width()) {
            // Apply paeth filtering
            for (int i = 0; i < width(); i++) {
                if (i == 0) {
                    Color color = pixel_rgba_directly(last_line_start_position + i * component_size, multiplier);
                    Color current_color = pixel_rgba_directly(current_line_start_position + i * component_size, multiplier);
                    set_pixel_rgba_directly(current_line_start_position + i * component_size, current_color.red() + color.red(),
                                                 current_color.green() + color.green(),
                                                 current_color.blue() + color.blue(),
                                                 current_color.alpha() + color.alpha(), multiplier);
                }
                else {
                    Color pixel2 = pixel_rgba_directly(current_line_start_position + (i - 1) * component_size, multiplier);
                    Color pixel3 = pixel_rgba_directly(last_line_start_position + (i - 1) * component_size, multiplier);
                    Color pixel1 = pixel_rgba_directly(last_line_start_position + i * component_size, multiplier);
                    Color current_color = pixel_rgba_directly(current_line_start_position + i * component_size, multiplier);
                    set_pixel_rgba_directly(current_line_start_position + i * component_size, current_color.red() + static_cast<unsigned char>(paeth_function(pixel1.red(), pixel2.red(), pixel3.red())),
                                                 current_color.green() + static_cast<unsigned char>(paeth_function(pixel1.green(), pixel2.green(), pixel3.green())),
                                                 current_color.blue() + static_cast<unsigned char>(paeth_function(pixel1.blue(), pixel2.blue(), pixel3.blue())),
                                                 current_color.alpha() + static_cast<unsigned char>(paeth_function(pixel1.alpha(), pixel2.alpha(), pixel3.alpha())), multiplier);
                }
            }
        }
    }
    void __Image_Base::_load_png_IDAT_from_file(Bytes_Set* file, std::shared_ptr<__Image_Error>& error_handler) {
        std::vector<_PNG_Chunk>& chunk = a_idat_chunk;
        if (file != 0) {
            if (a_pixels == 0) { error_handler.get()->set_value(SCLS_IMAGE_ERROR_UNKNOW); return; }

            // Get the size of the chunks
            int current_size = 0;
            for (int i = 0; i < static_cast<int>(chunk.size()); i++) {
                current_size += chunk[i].size;
            }

            // Create the necessary things to read the PNG file
            Bytes_Set* all_data = new Bytes_Set();

            // Read into the chunk
            for (int i = 0; i < static_cast<int>(chunk.size()); i++) {
                all_data->add_datas(file->extract_datas(chunk[i].size, chunk[i].position), chunk[i].size);
            }

            // Set binary mode
            SET_BINARY_MODE(stdin);
            SET_BINARY_MODE(stdout);

            // Define compression variables
            int ret = 0;
            int out_size = (width() * height() * components()) + height();
            unsigned char* out = new unsigned char[out_size];
            ret = uncompress_binary(all_data->datas(), current_size, (char*)out, out_size);
            delete all_data; all_data = 0;

            if (ret != 1) { delete[] out; return; }

            // Process data
            int component_size = components();
            int current_line = -1;
            int current_line_start_position = 0;
            int current_position = 0;
            int last_line_start_position = 0;
            Color last_pixel(0, 0, 0);
            int multiplier = 1;
            int part = -1;
            int processed_data = 0;
            for (int i = 0; i < out_size; i++) {
                if (part >= 0 && part < width() * component_size) {
                    unsigned char component = part % component_size;
                    if (component == 0) {
                        // Apply current component
                        processed_data++;
                    }
                    set_pixel_directly(current_position, static_cast<unsigned char>(out[i]));
                    current_position++;
                    part++;
                }
                else {
                    // Handle the untreated datas
                    _load_png_IDAT_from_file_rgba(component_size, current_line_start_position, last_line_start_position, multiplier, processed_data);

                    // Update the needed datas
                    a_filter_type = out[i];
                    current_line++;
                    last_line_start_position = current_line_start_position;
                    current_line_start_position = current_position;
                    part = 0;
                }
            }

            // Handle last untreated datas
            if (processed_data > 0) {_load_png_IDAT_from_file_rgba(component_size, current_line_start_position, last_line_start_position, multiplier, processed_data);}

            // Free memory
            delete[] out;
        }
        else { error_handler.get()->set_value(SCLS_IMAGE_ERROR_UNKNOW); }
    }

    // Load the pHYS chunk from a path
    void __Image_Base::_load_png_pHYS_from_file(Bytes_Set* file, _PNG_Chunk chunk, std::shared_ptr<__Image_Error>& error_handler) {
        if (file != 0 && chunk.name == "pHYs" && chunk.size == 9) {
            // Read into the chunk
            a_physical_height_ratio = file->extract_uint(chunk.position + 4, true);
            a_physical_width_ratio = file->extract_uint(chunk.position, true);
            a_physical_unit = file->data_at(chunk.position + 8);
        }
        else error_handler.get()->set_value(SCLS_IMAGE_PNG_ERROR_WRONG_PHYS_CHUNK);
    };

    // Load the Image from a PNG file
    void __Image_Base::_load_png_file(Bytes_Set* file, std::shared_ptr<__Image_Error>& error_handler) {
        // Check the first chunk of the file
        a_width = file->extract_uint(16, true);
        a_height = file->extract_uint(20, true);
        a_bit_depth = file->data_at(24);
        a_color_type = file->data_at(25);
        a_compression_method = file->data_at(26);
        a_filter_method = file->data_at(27);
        a_interlace_method = file->data_at(28);

        // Not implemented yet
        if(bit_depht() != 8) { error_handler.get()->set_value(SCLS_IMAGE_PNG_ERROR_BIT_DEPHT);; }
        else _load_all_chunks_from_png_file(file, error_handler);
    }

    // Load the sRGB chunk from a path
    void __Image_Base::_load_png_sRGB_from_file(Bytes_Set* file, _PNG_Chunk chunk, std::shared_ptr<__Image_Error>& error_handler) {if (file != 0 && chunk.name == "sRGB" && chunk.size == 1) {a_srgb_value = file->data_at(chunk.position);}else{error_handler.get()->set_value(SCLS_IMAGE_PNG_ERROR_WRONG_SRGB_CHUNK);}}

    // Returns the signature of a PNG file
    std::vector<unsigned char> __Image_Base::png_signature() {
        std::vector<unsigned char> signature;
        signature.push_back(137);
        signature.push_back(80);
        signature.push_back(78);
        signature.push_back(71);
        signature.push_back(13);
        signature.push_back(10);
        signature.push_back(26);
        signature.push_back(10);

        return signature;
    }

    // Save the image into the PNG format
    void __Image_Base::save_png(std::string path) {Bytes_Set* datas = datas_png();datas->save(path);delete datas; datas = 0;}

    // Getters and setters
    unsigned int __Image_Base::compression_method() const { return a_compression_method; };
    unsigned int __Image_Base::filter_method() const { return a_filter_method; };
    unsigned int __Image_Base::interlace_method() const { return a_interlace_method; };
    unsigned int __Image_Base::physical_height_ratio() const { return a_physical_height_ratio; };
    unsigned int __Image_Base::physical_unit() const { return a_physical_unit; };
    unsigned int __Image_Base::physical_width_ratio() const { return a_physical_width_ratio; };

    //*********
    //
    // The Image class - Editing
    //
    //*********

    // Draws / fills a circle on the image
    void __Image_Base::draw_circle(int x_center, int y_center, double radius, Color color, unsigned short line_width){draw_circle(x_center,y_center,radius,color.red(),color.green(),color.blue(),color.alpha(),line_width);}
    void __Image_Base::draw_circle(int x_center, int y_center, double radius, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, unsigned short line_width) {fill_circle(x_center, y_center, radius, 0, 0, 0, 0, line_width, red, green, blue, alpha);}
    void __Image_Base::fill_circle(int x_center, int y_center, double radius, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {fill_circle(x_center, y_center, radius, red, green, blue, alpha, 0, 0, 0, 0, 0);}
    void __Image_Base::fill_circle(int x_center, int y_center, double radius_x, double radius_y, double angle_start, double angle_end, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, double border_radius, unsigned char border_red, unsigned char border_green, unsigned char border_blue, unsigned char border_alpha){fill_circle(x_center, y_center, radius_x, radius_y, 0, angle_start, angle_end, red, green, blue, alpha, border_radius, border_red, border_green, border_blue, border_alpha);}
    void __Image_Base::fill_circle(int x_center, int y_center, double radius, double angle_start, double angle_end, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, double border_radius, unsigned char border_red, unsigned char border_green, unsigned char border_blue, unsigned char border_alpha){fill_circle(x_center, y_center, radius, radius, angle_start, angle_end, red, green, blue, alpha, border_radius, border_red, border_green, border_blue, border_alpha);}
    void __Image_Base::fill_circle(int x_center, int y_center, double radius, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, double border_radius, unsigned char border_red, unsigned char border_green, unsigned char border_blue, unsigned char border_alpha){fill_circle(x_center,y_center,radius,0,360,red,green,blue,alpha,border_radius,border_red,border_green,border_blue,border_alpha);};
    void __Image_Base::fill_circle(int x_center, int y_center, double radius, double angle_start, double angle_end, Color color, double border_radius, Color border_color){fill_circle(x_center,y_center,radius,angle_start,angle_end,color.red(),color.green(),color.blue(),color.alpha(),border_radius,border_color.red(),border_color.green(),border_color.blue(),border_color.alpha());};
    void __Image_Base::fill_circle(int x_center, int y_center, double radius_x, double radius_y, double angle_start, double angle_end, Color color, double border_radius, Color border_color){fill_circle(x_center,y_center,radius_x,radius_y,angle_start,angle_end,color.red(),color.green(),color.blue(),color.alpha(),border_radius,border_color.red(),border_color.green(),border_color.blue(),border_color.alpha());};
    void __Image_Base::fill_circle(int x_center, int y_center, double radius_x, double radius_y, double angle, double angle_start, double angle_end, Color color, double border_radius, Color border_color){fill_circle(x_center,y_center,radius_x,radius_y,angle,angle_start,angle_end,color.red(),color.green(),color.blue(),color.alpha(),border_radius,border_color.red(),border_color.green(),border_color.blue(),border_color.alpha());};
    void __Image_Base::fill_circle(int x_center, int y_center, double radius_x, double radius_y, Color color, double border_radius, Color border_color){fill_circle(x_center,y_center,radius_x,radius_y,0,0,360,color.red(),color.green(),color.blue(),color.alpha(),border_radius,border_color.red(),border_color.green(),border_color.blue(),border_color.alpha());};
    void __Image_Base::fill_circle(int x_center, int y_center, double radius, Color color, double border_radius, Color border_color){fill_circle(x_center,y_center,radius,0,360,color.red(),color.green(),color.blue(),color.alpha(),border_radius,border_color.red(),border_color.green(),border_color.blue(),border_color.alpha());};
    void __Image_Base::fill_circle(int x_center, int y_center, double radius, Color color){fill_circle(x_center,y_center,radius,color.red(),color.green(),color.blue(),color.alpha());};
    void __Image_Base::fill_circle(int x_center, int y_center, double radius_x, double radius_y, double angle, double angle_start, double angle_end, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, double border_radius, unsigned char border_red, unsigned char border_green, unsigned char border_blue, unsigned char border_alpha) {
        // Format the angle
        while(angle <= -90.0){angle += 180.0;}while(angle >= 90.0){angle -= 180.0;}
        angle /= (180.0/SCLS_PI);

        double last_angle = SCLS_PI / 3.0;double last_x = oval_vector_x(radius_x, radius_y, last_angle).rotated(angle * (180.0 / SCLS_PI)).x();
        double left_angle = SCLS_PI * 1.8;double left_x = oval_vector_x(radius_x, radius_y, left_angle).rotated(angle * (180.0 / SCLS_PI)).x();
        for(int i = 0;i<30;i++){
            double new_angle = (last_angle + left_angle) / 2.0;
            double needed_x = oval_vector_x(radius_x, radius_y, new_angle).rotated(angle * (180.0 / SCLS_PI)).x();
            if(last_x > left_x){last_angle = new_angle;last_x = needed_x;}
            else{left_angle = new_angle;left_x = needed_x;}
        }
        last_angle = 0;double last_y = scls::vector_2d_with_angle(last_angle).rotated(angle * (180.0 / SCLS_PI)).y() * oval_radius_proportion_y(radius_x, radius_y, last_angle);
        double top_angle = SCLS_PI;double top_y = scls::vector_2d_with_angle(top_angle).rotated(angle * (180.0 / SCLS_PI)).y() * oval_radius_proportion_y(radius_x, radius_y, top_angle);
        for(int i = 0;i<30;i++){
            double new_angle = (last_angle + top_angle) / 2.0;
            double needed_y = scls::vector_2d_with_angle(new_angle).rotated(angle * (180.0 / SCLS_PI)).y() * oval_radius_proportion_y(radius_x, radius_y, new_angle);
            if(last_y < top_y){last_angle = new_angle;last_y = needed_y;}
            else{top_angle = new_angle;top_y = needed_y;}
        }

        // Get the needed radius
        double drew_radius_x = std::sqrt(std::pow(radius_x * std::cos(angle), 2) + std::pow(radius_y * std::sin(angle), 2));
        double drew_y_center = std::sqrt(std::abs(std::pow(oval_radius(radius_x, radius_y, left_angle - SCLS_PI), 2) - std::pow(drew_radius_x, 2)));
        if(angle < 0){drew_y_center *= -1;}
        const int start_x = round(x_center - (drew_radius_x));
        const int start_x_inner = start_x + border_radius;
        int current_x = 0;

        // Calculate the Y datas
        double y_multiplier = 1;
        double y_center_offset_left = 0;
        double y_center_offset_left_to_add = drew_y_center / drew_radius_x;
        const int start_y_left = y_center - drew_y_center * y_multiplier;
        const int start_y_right = y_center + drew_y_center * y_multiplier;

        // Upgrade in the drawing
        double adaptated_angle_end = angle_end;double adaptated_angle_start = angle_start;
        double temp=adaptated_angle_end;adaptated_angle_end=adaptated_angle_start;adaptated_angle_start=temp;
        adaptated_angle_end*=-1;adaptated_angle_start*=-1;
        adaptated_angle_end+=180.0;adaptated_angle_start+=180.0;
        while(adaptated_angle_end < 0.0){adaptated_angle_end += 360.0;}while(adaptated_angle_start < 0.0){adaptated_angle_start += 360.0;}
        while(adaptated_angle_end >= 360.0){adaptated_angle_end -= 360.0;}while(adaptated_angle_start >= 360.0){adaptated_angle_start -= 360.0;}
        adaptated_angle_end /= (180.0/SCLS_PI);adaptated_angle_start /= (180.0/SCLS_PI);angle_end /= (180.0/SCLS_PI);angle_start /= (180.0/SCLS_PI);
        int multiplier = 1;
        int needed_components = components();
        int needed_height = height();
        int needed_width = width();
        while(current_x < drew_radius_x) {
            // Update the coordinate
            current_x++;
            double current_x_proportion_border = current_x / drew_radius_x;
            double current_x_proportion = (current_x - border_radius) / (drew_radius_x - border_radius);
            int needed_x = start_x + current_x;
            int needed_y_left = (start_y_left + y_center_offset_left);
            int needed_y_right = (start_y_right - y_center_offset_left);
            y_center_offset_left += y_center_offset_left_to_add;

            // Get the needed x/y
            double angle_border = circle_angle_at_x(current_x_proportion_border - 1.0) + (left_angle - SCLS_PI);
            Point_2D director_border_before_angle = oval_vector_x(radius_x, radius_y, angle_border) * radius_x;
            Point_2D director_border_top = director_border_before_angle.rotated(angle * (180.0/SCLS_PI));
            int first_y_top = director_border_top.y() - (drew_y_center - y_center_offset_left);

            // Draw each needed pixels
            // Set the last y
            #define CHECK_ANGLE(needed_angle) ((adaptated_angle_end == adaptated_angle_start) || (needed_angle >= adaptated_angle_start && needed_angle <= adaptated_angle_end) || (adaptated_angle_start > adaptated_angle_end && (needed_angle <= adaptated_angle_end || needed_angle >= adaptated_angle_start)))
            #define CHECK_ANGLE_END(angle_1, angle_2) (CHECK_ANGLE(angle_1) && !CHECK_ANGLE(angle_2) && std::abs(adaptated_angle_start - angle_1) > std::abs(adaptated_angle_end - angle_1))
            #define CHECK_ANGLE_START(angle_1, angle_2) (CHECK_ANGLE(angle_1) && !CHECK_ANGLE(angle_2) && std::abs(adaptated_angle_start - angle_1) < std::abs(adaptated_angle_end - angle_1))
            double needed_angle_bottom = SCLS_PI;double needed_angle_top = SCLS_PI;
            bool in_border_bottom = true;int last_y_bottom = 0;int last_y_top = 0;
            if(needed_x >= start_x_inner) {
                needed_angle_bottom = circle_angle_at_x(static_cast<double>(radius_x - border_radius) * current_x_proportion, radius_x - border_radius, radius_x - border_radius) + left_angle;
                needed_angle_top = circle_angle_at_x(static_cast<double>(radius_x - border_radius) * current_x_proportion, radius_x - border_radius, radius_x - border_radius) + left_angle;

                // Update angles
                needed_angle_bottom -= SCLS_PI;needed_angle_top -= SCLS_PI;
                while(needed_angle_bottom < 0){needed_angle_bottom += SCLS_PI * 2.0;}while(needed_angle_bottom >= SCLS_PI * 2.0){needed_angle_bottom -= SCLS_PI * 2.0;}
                while(needed_angle_top < 0){needed_angle_top += SCLS_PI * 2.0;}while(needed_angle_top >= SCLS_PI * 2.0){needed_angle_top -= SCLS_PI * 2.0;}

                Point_2D director_bottom = oval_vector_x(radius_x - border_radius, radius_y - border_radius, needed_angle_bottom).rotated(angle * (180.0/SCLS_PI));
                Point_2D director_top = oval_vector_x(radius_x - border_radius, radius_y - border_radius, needed_angle_top).rotated(angle * (180.0/SCLS_PI));

                double current_border_radius = std::sqrt(std::pow(radius_x, 2) + std::pow(radius_x - border_radius, 2) - 2 * radius_x * (radius_x - border_radius) * std::cos((needed_angle_bottom - angle_border) / SCLS_PI));
                last_y_bottom = (director_bottom.y() * (radius_x - current_border_radius)) - (drew_y_center - y_center_offset_left);
                last_y_top = (director_top.y() * (radius_x - current_border_radius)) - (drew_y_center - y_center_offset_left);
                in_border_bottom = false;
            }

            // Border part
            if(border_alpha > 0 && border_radius > 0) {
                // Draw the circle border
                int y_height_base = (first_y_top - last_y_top);
                // Left part of the border
                needed_x = (x_center - drew_radius_x) + current_x;
                if(needed_x >= 0 && needed_x < needed_width) {
                    // Left-bottom of the circle border
                    int i = 0;double current_angle = SCLS_PI - needed_angle_bottom;double current_angle_border = SCLS_PI - angle_border;int y_height = y_height_base;
                    if(CHECK_ANGLE_END(current_angle, current_angle_border)){y_height = round(static_cast<double>(y_height_base) * ((adaptated_angle_end - current_angle) / (current_angle_border - current_angle)));}
                    if(CHECK_ANGLE_START(current_angle_border, current_angle)){i = round(static_cast<double>(y_height_base) * ((adaptated_angle_start - current_angle) / (current_angle_border - current_angle)));}
                    for(;i<y_height;i++) {
                        int current_y = (needed_y_left - (last_y_top + i));
                        if((CHECK_ANGLE(current_angle) || CHECK_ANGLE(current_angle_border)) && current_y >= 0 && current_y < needed_height) {
                            int position = (current_y * needed_width + needed_x) * needed_components;
                            if(use_alpha()) {paste_pixel_rgba_directly(position, border_red, border_green, border_blue, border_alpha, multiplier);}
                            else{set_pixel_directly(position, border_red, border_green, border_blue, multiplier);}
                        }
                    }
                    // Left-top of the circle border
                    i = 0;current_angle = SCLS_PI + needed_angle_top;current_angle_border = SCLS_PI + angle_border;y_height = y_height_base;
                    if(CHECK_ANGLE_END(current_angle_border, current_angle)){i = round(static_cast<double>(y_height_base) * ((adaptated_angle_end - current_angle) / (current_angle_border - current_angle)));}
                    if(CHECK_ANGLE_START(current_angle, current_angle_border)){y_height = round(static_cast<double>(y_height_base) * ((adaptated_angle_start - current_angle) / (current_angle_border - current_angle)));}
                    for(;i<y_height;i++) {
                        int current_y = (needed_y_left + (last_y_top + i));
                        if((CHECK_ANGLE(current_angle) || CHECK_ANGLE(current_angle_border)) && current_y >= 0 && current_y < needed_height) {
                            int position = (current_y * needed_width + needed_x) * needed_components;
                            if(use_alpha()) {paste_pixel_rgba_directly(position, border_red, border_green, border_blue, border_alpha, multiplier);}
                            else{set_pixel_directly(position, border_red, border_green, border_blue, multiplier);}
                        }
                    }
                }

                // Right part of the border
                needed_x = (x_center + drew_radius_x) - current_x;
                if(needed_x >= 0 && needed_x < needed_width){
                    // Right-bottom of the circle border
                    int i = 0;double current_angle = needed_angle_bottom;double current_angle_border = angle_border;int y_height = y_height_base;
                    if(CHECK_ANGLE_END(current_angle_border, current_angle) || (in_border_bottom && current_angle_border < adaptated_angle_end)){
                        double x_hypothenus = (static_cast<double>(radius_x - current_x) / std::abs(std::cos(adaptated_angle_end)));
                        double y_hypothenus = radius_y - (radius_x - x_hypothenus);//x_hypothenus * (radius_y / radius_x);
                        //double corrected_angle = oval_angle_at_x(x_hypothenus, y_hypothenus, radius_x - current_x);
                        //double proportion = oval_radius_proportion_y(radius_x, radius_y, corrected_angle);
                        double circle_y = (std::abs(std::sin(adaptated_angle_end)) * y_hypothenus);
                        i = circle_y - last_y_top; // * (static_cast<double>(radius_y - border_radius) / static_cast<double>(radius_x - border_radius))

                        //std::cout << "G " << i << " " << last_y_top << " " << circle_angle_at_x(static_cast<double>(radius_x - current_x) / (x_hypothenus)) << " " << (radius_x - current_x) << " " << radius_x << " " << radius_y << " " << x_hypothenus << " " << y_hypothenus << " " << (static_cast<double>(radius_y - border_radius) / static_cast<double>(radius_x - border_radius)) << std::endl;
                        //fill_circle(x_center, y_center, x_hypothenus, y_hypothenus, scls::Color(0, ((radius_x - current_x) - 260) * 20, 0), 0, scls::Color(0, 0, 0, 0));
                    }
                    if(CHECK_ANGLE_START(current_angle, current_angle_border)){y_height = round(static_cast<double>(y_height_base) * ((adaptated_angle_start - current_angle) / (current_angle_border - current_angle)));}
                    for(;i<y_height;i++) {
                        int current_y = (needed_y_right - (last_y_top + i));
                        if((CHECK_ANGLE(current_angle) || CHECK_ANGLE(current_angle_border)) && current_y >= 0 && current_y < needed_height) {
                            int position = (current_y * needed_width + needed_x) * needed_components;
                            if(use_alpha()) {paste_pixel_rgba_directly(position, border_red, border_green, border_blue, border_alpha, multiplier);}
                            else{set_pixel_directly(position, border_red, border_green, border_blue, multiplier);}
                        }
                    }
                    // Right-top of the circle border
                    i = 0; current_angle = SCLS_PI * 2.0 - needed_angle_top;current_angle_border = SCLS_PI * 2.0 - angle_border;y_height = y_height_base;
                    if(CHECK_ANGLE_END(current_angle, current_angle_border)){
                        double end_hypothenus = (static_cast<double>(radius_x - current_x) / std::abs(std::cos(adaptated_angle_end)));
                        y_height = std::abs(std::sin(adaptated_angle_end)) * end_hypothenus - last_y_top;
                    }
                    if(CHECK_ANGLE_START(current_angle_border, current_angle)){i = round(static_cast<double>(y_height_base) * ((adaptated_angle_start - current_angle) / (current_angle_border - current_angle)));}
                    for(;i<y_height;i++) {
                        int current_y = (needed_y_right + (last_y_top + i));
                        if((CHECK_ANGLE(current_angle) || CHECK_ANGLE(current_angle_border)) && current_y >= 0 && current_y < needed_height) {
                            int position = (current_y * needed_width + needed_x) * needed_components;
                            if(use_alpha()) {paste_pixel_rgba_directly(position, border_red, border_green, border_blue, border_alpha, multiplier);}
                            else{set_pixel_directly(position, border_red, border_green, border_blue, multiplier);}
                        }
                    }
                }
            }

            // Inner part
            bool fill_bottom = true;
            if(alpha > 0) {
                // Get the needed limits
                int last_y_top_left = last_y_top;int last_y_top_right = last_y_top;
                int start_top_left = 0;int start_top_right = 0;
                // Top left
                if(angle_start > SCLS_HALF_PI  && angle_start <= SCLS_PI){
                    double x_hypothenus = ((radius_x - radius_x * current_x_proportion_border) * std::abs(std::tan(angle_start)));
                    if(x_hypothenus > 0){
                        double y_hypothenus = x_hypothenus * (radius_y / radius_x);
                        last_y_top_left = y_hypothenus - (drew_y_center - y_center_offset_left);
                        if(last_y_top_left > last_y_top){last_y_top_left = last_y_top;}
                    }
                    else{last_y_top_left = 0;}
                }
                if(angle_end > SCLS_HALF_PI  && angle_end <= SCLS_PI){
                    double x_hypothenus = ((radius_x - radius_x * current_x_proportion_border) * std::abs(std::tan(angle_end)));
                    if(x_hypothenus > 0){
                        double y_hypothenus = x_hypothenus * (radius_y / radius_x);
                        start_top_left = y_hypothenus;
                    }
                    else{start_top_left = 0;}
                }
                // Top right
                if(angle_start != 0 && angle_start <= SCLS_PI / 2.0){
                    double x_hypothenus = ((radius_x - radius_x * current_x_proportion_border) * std::abs(std::tan(angle_start)));
                    if(x_hypothenus > 0){
                        double y_hypothenus = x_hypothenus * (radius_y / radius_x);
                        start_top_right = y_hypothenus - (drew_y_center - y_center_offset_left);
                    }
                    else{start_top_right = 0;}
                }
                if(angle_end != 0 && angle_end <= SCLS_PI / 2.0){
                    double x_hypothenus = ((radius_x - radius_x * current_x_proportion_border) * std::abs(std::tan(angle_end)));
                    if(x_hypothenus > 0){
                        double y_hypothenus = x_hypothenus * (radius_y / radius_x);
                        last_y_top_right = y_hypothenus;
                        if(last_y_top_right > last_y_top){last_y_top_right = last_y_top;}
                    }
                    else{last_y_top_right = 0;}
                }

                // Parts to use
                #define USE_PART(a_1, a_2) (angle_start <= a_2 && angle_end >= a_1) || (angle_end <= a_2 && angle_start >= a_2 && angle_end >= a_1) || (angle_start >= a_1 && angle_end >= a_1 && angle_start <= a_2)
                bool use_bottom_left = USE_PART(SCLS_PI, SCLS_PI + SCLS_HALF_PI);
                bool use_bottom_right = USE_PART(SCLS_PI + SCLS_HALF_PI, SCLS_PI * 2.0);
                bool use_top_left = USE_PART(SCLS_HALF_PI, SCLS_PI);
                bool use_top_right = USE_PART(0, SCLS_HALF_PI);
                #undef USE_PART

                // Fill the circle
                int i = 0;
                // Left of the circle
                needed_x = (x_center - drew_radius_x) + current_x;
                if(needed_x >= 0 && needed_x < needed_width){
                    if(use_bottom_left) {
                        for(;i < last_y_bottom;i++) {
                            int current_y = (needed_y_left + i);
                            if(current_y >= 0 && current_y < needed_height && fill_bottom) {
                                int position = (current_y * needed_width + needed_x) * needed_components;
                                if(use_alpha()) {paste_pixel_rgba_directly(position, red, green, blue, alpha, multiplier);}
                                else{set_pixel_directly(position, red, green, blue, multiplier);}
                            }
                        }
                    }
                    if(use_top_left) {
                        for(i = start_top_left;i < last_y_top_left;i++) {
                            int current_y = (needed_y_left - i);
                            if(current_y >= 0 && current_y < needed_height) {
                                int position = (current_y * needed_width + needed_x) * needed_components;
                                if(use_alpha()) {paste_pixel_rgba_directly(position, red, green, blue, alpha, multiplier);}
                                else{set_pixel_directly(position, red, green, blue, multiplier);}
                            }
                        }
                    }
                }

                // Right of the circle
                needed_x = (x_center + drew_radius_x) - current_x;
                if(needed_x >= 0 && needed_x < needed_width){
                    // Bottom right part of the circle
                    if(use_bottom_right) {
                        for(i = 0;i < last_y_bottom;i++) {
                            int current_y = (needed_y_right + i);
                            if(current_y >= 0 && current_y < needed_height && fill_bottom) {
                                int position = (current_y * needed_width + needed_x) * needed_components;
                                if(use_alpha()) {paste_pixel_rgba_directly(position, red, green, blue, alpha, multiplier);}
                                else{set_pixel_directly(position, red, green, blue, multiplier);}
                            }
                        }
                    }

                    // Top right part of the circle
                    if(use_top_right) {
                        for(i = start_top_right;i < last_y_top_right;i++) {
                            int current_y = (needed_y_right - i);
                            if(current_y >= 0 && current_y < needed_height) {
                                int position = (current_y * needed_width + needed_x) * needed_components;
                                if(use_alpha()) {paste_pixel_rgba_directly(position, red, green, blue, alpha, multiplier);}
                                else{set_pixel_directly(position, red, green, blue, multiplier);}
                            }
                        }
                    }
                }
            }
        }
    }
    // Fill a circle with a gradient on the image
    void __Image_Base::fill_circle_gradient(int x_center, int y_center, double radius, Color color, Color (*needed_function)(double, int, int, int, unsigned char, unsigned char, unsigned char, unsigned char)){fill_circle_gradient(x_center,y_center,radius,color.red(),color.green(),color.blue(),color.alpha(),needed_function);}
    void __Image_Base::fill_circle_gradient(int x_center, int y_center, double radius, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, Color (*needed_function)(double, int, int, int, unsigned char, unsigned char, unsigned char, unsigned char)) {
        const int start_x = round(x_center - radius);
        int current_x = 0;

        // Upgrade in the drawing
        int multiplier = 1;
        int needed_components = components();
        int needed_height = height();
        int needed_width = width();
        while(current_x < radius) {
            // Update the coordinate
            current_x++;

            // Get the needed x/y
            int needed_x = start_x + current_x;
            double current_ratio = static_cast<double>(current_x) / radius;
            int needed_y = round(std::sin(std::acos(1.0 - current_ratio)) * radius);

            // Draw the circle
            for(int i = 0;i < needed_y;i++) {
                // Left of the circle
                needed_x = (x_center - radius) + current_x;
                if(needed_x >= 0 && needed_x < needed_width) {
                    int current_y = (y_center + i);
                    if(current_y >= 0 && current_y < needed_height) {
                        int position = (current_y * needed_width + needed_x) * needed_components;
                        Color needed_color = needed_function(std::sqrt(std::pow(x_center - needed_x, 2) + std::pow(y_center - current_y, 2)), radius, needed_x - x_center, current_y - y_center, red, green, blue, alpha);
                        set_pixel_rgba_directly_with_alpha(position, needed_color.red(), needed_color.green(), needed_color.blue(), needed_color.alpha(), multiplier);
                    } current_y = (y_center - i);
                    if(current_y >= 0 && current_y < needed_height) {
                        int position = (current_y * needed_width + needed_x) * needed_components;
                        Color needed_color = needed_function(std::sqrt(std::pow(x_center - needed_x, 2) + std::pow(y_center - current_y, 2)), radius, needed_x - x_center, current_y - y_center, red, green, blue, alpha);
                        set_pixel_rgba_directly_with_alpha(position, needed_color.red(), needed_color.green(), needed_color.blue(), needed_color.alpha(), multiplier);
                    }
                }
                // Right of the circle
                needed_x = (x_center + radius) - current_x;
                if(needed_x >= 0 && needed_x < needed_width) {
                    int current_y = (y_center + i);
                    if(current_y >= 0 && current_y < needed_height) {
                        int position = (current_y * needed_width + needed_x) * needed_components;
                        Color needed_color = needed_function(std::sqrt(std::pow(x_center - needed_x, 2) + std::pow(y_center - current_y, 2)), radius, needed_x - x_center, current_y - y_center, red, green, blue, alpha);
                        set_pixel_rgba_directly_with_alpha(position, needed_color.red(), needed_color.green(), needed_color.blue(), needed_color.alpha(), multiplier);
                    } current_y = (y_center - i);
                    if(current_y >= 0 && current_y < needed_height) {
                        int position = (current_y * needed_width + needed_x) * needed_components;
                        Color needed_color = needed_function(std::sqrt(std::pow(x_center - needed_x, 2) + std::pow(y_center - current_y, 2)), radius, needed_x - x_center, current_y - y_center, red, green, blue, alpha);
                        set_pixel_rgba_directly_with_alpha(position, needed_color.red(), needed_color.green(), needed_color.blue(), needed_color.alpha(), multiplier);
                    }
                }
            }
        }
    }

    // Draw an arrow on the image
    void __Image_Base::draw_arrow(int x_1, int y_1, int x_2, int y_2, Color color, double hat_position_in_percentage, double hat_size_in_percentage, unsigned short width) {draw_arrow(x_1, y_1, x_2, y_2, color.red(), color.green(), color.blue(), color.alpha(), hat_position_in_percentage, hat_size_in_percentage, width);}
    void __Image_Base::draw_arrow(int x_1, int y_1, int x_2, int y_2, Color color, double hat_percentage, unsigned short width) {draw_arrow(x_1, y_1, x_2, y_2, color.red(), color.green(), color.blue(), color.alpha(), 1, hat_percentage, width);}
    void __Image_Base::draw_arrow(int x_1, int y_1, int x_2, int y_2, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, double hat_position_in_percentage, double hat_size_in_percentage, unsigned short line_width) {
        draw_line(x_1, y_1, x_2, y_2, red, green, blue, alpha, line_width);
        // Draw the hat of the arrow
        int x_diff = -(x_2 - x_1); int y_diff = -(y_2 - y_1);
        scls::Vector_3D base_vector = scls::Vector_3D(x_diff, 0, y_diff);
        if(x_diff == 0 && y_diff == 0){base_vector = scls::Vector_3D(-1, 0, 0);base_vector.rotate(scls::Vector_3D(0, hat_position_in_percentage, 0));hat_position_in_percentage=0;}
        // First part
        scls::Vector_3D needed_vector = base_vector.rotated(scls::Vector_3D(0, 45, 0));
        int needed_x = needed_vector.x() * hat_size_in_percentage; int needed_y = needed_vector.z() * hat_size_in_percentage;
        int current_x = x_1 + (x_2 - x_1) * hat_position_in_percentage; int current_y = y_1 + (y_2 - y_1) * hat_position_in_percentage;
        draw_line(current_x + needed_x, current_y + needed_y, current_x, current_y, red, green, blue, alpha, line_width);
        // Second part
        needed_vector = base_vector.rotated(scls::Vector_3D(0, -45, 0));
        needed_x = static_cast<double>(needed_vector.x()) * hat_size_in_percentage; needed_y = static_cast<double>(needed_vector.z()) * hat_size_in_percentage;
        current_x = x_1 + (x_2 - x_1) * hat_position_in_percentage; current_y = y_1 + (y_2 - y_1) * hat_position_in_percentage;
        draw_line(current_x + needed_x, current_y + needed_y, current_x, current_y, red, green, blue, alpha, line_width);
    }

    // Draws a border on the image
    void __Image_Base::draw_border(int top, int left, int bottom, int right, scls::Color color){draw_border(top, left, bottom, right, color.red(), color.green(), color.blue(), color.alpha());}
    void __Image_Base::draw_border(int top, int left, int bottom, int right, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
        if(top > 0) {fill_rect(0, 0, width(), top, red, green, blue, alpha);}
        if(left > 0) {fill_rect(0, 0, left, height(), red, green, blue, alpha);}
        if(bottom > 0) {fill_rect(0, height() - bottom, width(), bottom, red, green, blue, alpha);}
        if(right > 0) {fill_rect(width() - right, 0, right, height(), red, green, blue, alpha);}
    }

    // Necessary points to draw a line
    std::vector<Point_2D> __Image_Base::line_points(int x_1, int y_1, int x_2, int y_2) {
        // Only case which the algorithm does not work correctly
        int line_width = 0;
        if(x_1 == x_2) {
            // Check the X position
            if(x_1 + static_cast<int>(line_width) < 0 || x_1 >= width()){return std::vector<Point_2D>();}
            if(y_2 < y_1) {
                y_2 += y_1;
                y_1 = y_2 - y_1;
                y_2 = y_2 - y_1;
            } y_2++;

            // Get the points
            x_1 -= line_width / 2; y_1 -= line_width / 2; x_2 -= line_width / 2; y_2 -= line_width / 2;
            std::vector<scls::Point_2D> to_return = std::vector<scls::Point_2D>(y_2 - y_1, scls::Point_2D(x_1, y_1));
            for(int i = 0;i<y_2 - y_1;i++){to_return[i].set_y(y_1 + i);}
            return to_return;
        }
        else if(y_1 == y_2) {
            // Check the Y position
            if(y_1 < 0 || y_1 >= height()){return std::vector<Point_2D>();}
            if(x_2 < x_1) {
                x_2 += x_1;
                x_1 = x_2 - x_1;
                x_2 = x_2 - x_1;
            } x_2++;
            // Draw the line
            x_1 -= line_width / 2; y_1 -= line_width / 2; x_2 -= line_width / 2; y_2 -= line_width / 2;
            std::vector<scls::Point_2D> to_return = std::vector<scls::Point_2D>(x_2 - x_1, scls::Point_2D(x_1, y_1));
            for(int i = 0;i<x_2 - x_1;i++){to_return[i].set_x(x_1 + i);}
            return to_return;
        }

        // Other cases
        x_2++; y_2++;
        double distance_x = static_cast<double>(x_2 - x_1);
        double distance_y = static_cast<double>(y_2 - y_1);

        double x_y_ratio = distance_x / distance_y;

        std::vector<scls::Point_2D> to_return = std::vector<scls::Point_2D>(std::max(distance_x, distance_y), scls::Point_2D(0, 0));
        if (abs(x_y_ratio) < 1) {
            // Normalize positions
            if (y_1 > y_2) {
                int temp = y_1;
                y_1 = y_2;
                y_2 = temp;
                temp = x_1;
                x_1 = x_2;
                x_2 = temp;
            }

            double actual_x = x_1;
            double actual_y = y_1;

            // Check the positions
            if(actual_y < 0) {actual_x += x_y_ratio * std::abs(actual_y);actual_y=0;}
            if(y_2 >= height()) y_2 = height() - 1;
            // Draw the line
            int i = 0;
            while (actual_y < y_2) {
                actual_y++;
                actual_x += x_y_ratio;
                to_return[i].set_x(actual_x);to_return[i].set_y(actual_y);
                i++;
            }
        }
        else {
            // Normalize positions
            if (x_1 > x_2) {
                int temp = x_1;
                x_1 = x_2;
                x_2 = temp;
                temp = y_1;
                y_1 = y_2;
                y_2 = temp;
            }

            double actual_x = x_1;
            double actual_y = y_1;

            // Check the positions
            double y_x_ratio = distance_y / distance_x;
            if(actual_x < 0) {actual_y += y_x_ratio * std::abs(actual_x);actual_x=0;}
            if(x_2 >= width()) x_2 = width() - 1;
            // Draw the line
            int i = 0;
            while (actual_x < x_2) {
                actual_y += y_x_ratio;
                actual_x++;
                to_return[i].set_x(actual_x);to_return[i].set_y(actual_y);
                i++;
            }
        }

        // Return the result
        return to_return;
    }

    // Draw a line on the image
    void __Image_Base::draw_line(int x_1, int y_1, int x_2, int y_2, Color color, unsigned short line_width) {draw_line(x_1, y_1, x_2, y_2, color.red(), color.green(), color.blue(), color.alpha(), line_width);}
    void __Image_Base::draw_line(scls::Point_2D point_1, scls::Point_2D point_2, Color color, unsigned short line_width){draw_line(point_1.x(), point_1.y(), point_2.x(), point_2.y(), color.red(), color.green(), color.blue(), color.alpha(), line_width);};
    void __Image_Base::draw_line(int x_1, int y_1, int x_2, int y_2, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, unsigned short line_width) {
        // Only case which the algorithm does not work correctly
        if(x_1 == x_2) {
            // Check the X position
            if(x_1 + static_cast<int>(line_width) < 0 || x_1 >= width()) return;
            if(y_2 < y_1) {
                y_2 += y_1;
                y_1 = y_2 - y_1;
                y_2 = y_2 - y_1;
            } y_2++;
            // Draw the line
            x_1 -= line_width / 2; y_1 -= line_width / 2; x_2 -= line_width / 2; y_2 -= line_width / 2;
            fill_rect(x_1, y_1, line_width, (y_2 - y_1) + line_width, red, green, blue, alpha);
        }
        else if(y_1 == y_2) {
            // Check the Y position
            if(y_1 < 0 || y_1 >= height()) return;
            if(x_2 < x_1) {
                x_2 += x_1;
                x_1 = x_2 - x_1;
                x_2 = x_2 - x_1;
            } x_2++;
            // Draw the line
            x_1 -= line_width / 2; y_1 -= line_width / 2; x_2 -= line_width / 2; y_2 -= line_width / 2;
            fill_rect(x_1, y_1, (x_2 - x_1) + line_width, line_width, red, green, blue, alpha);
        }
        else {
            x_2++; y_2++;
            double distance_x = static_cast<double>(x_2 - x_1);
            double distance_y = static_cast<double>(y_2 - y_1);

            double x_y_ratio = distance_x / distance_y;

            if (abs(x_y_ratio) < 1) {
                // Normalize positions
                if (y_1 > y_2) {
                    int temp = y_1;
                    y_1 = y_2;
                    y_2 = temp;
                    temp = x_1;
                    x_1 = x_2;
                    x_2 = temp;
                }

                // Check the positions
                double actual_x = x_1;double actual_y = y_1;
                if(actual_y < 0) {actual_x += x_y_ratio * std::abs(actual_y);actual_y=0;}
                if(y_2 >= height()) y_2 = height() - 1;
                // Draw the line
                while (actual_y < y_2) {
                    actual_y++;
                    actual_x += x_y_ratio;
                    set_pixel(actual_x, actual_y, red, green, blue, alpha, line_width);
                }
            }
            else {
                // Normalize positions
                if (x_1 > x_2) {
                    int temp = x_1;
                    x_1 = x_2;
                    x_2 = temp;
                    temp = y_1;
                    y_1 = y_2;
                    y_2 = temp;
                }

                // Check the positions
                double actual_x = x_1;double actual_y = y_1;
                double y_x_ratio = distance_y / distance_x;
                if(actual_x < 0) {actual_y += y_x_ratio * std::abs(actual_x);actual_x=0;}
                if(x_2 >= width()) x_2 = width() - 1;
                // Draw the line
                while (actual_x < x_2) {
                    actual_y += y_x_ratio;
                    actual_x++;
                    set_pixel(actual_x, actual_y, red, green, blue, alpha, line_width);
                }
            }
        }
    }
    // Draws a line gradient on the image
    void __Image_Base::draw_line_gradient_force(int x_1, int y_1, int x_2, int y_2, unsigned short line_width, Color (*needed_function)(int, int, double)) {
        const int first_x = x_1;const int first_y = y_1;

        // Only case which the algorithm does not work correctly
        if(x_1 == x_2) {
            // Check the X position
            if(x_1 + static_cast<int>(line_width) < 0 || x_1 >= width()) return;
            if(y_2 < y_1) {
                y_2 += y_1;
                y_1 = y_2 - y_1;
                y_2 = y_2 - y_1;
            } y_2++;
            // Draw the line
            x_1 -= line_width / 2; y_1 -= line_width / 2; x_2 -= line_width / 2; y_2 -= line_width / 2;
            Color needed_color = needed_function(0, first_x, first_y);
            fill_rect(x_1, y_1, line_width, y_2 - y_1, needed_color.red(), needed_color.green(), needed_color.blue(), needed_color.alpha());
        }
        else if(y_1 == y_2) {
            // Check the Y position
            if(y_1 < 0 || y_1 >= height()) return;
            if(x_2 < x_1) {
                x_2 += x_1;
                x_1 = x_2 - x_1;
                x_2 = x_2 - x_1;
            } x_2++;
            // Draw the line
            x_1 -= line_width / 2; y_1 -= line_width / 2; x_2 -= line_width / 2; y_2 -= line_width / 2;
            Color needed_color = needed_function(0, first_x, first_y);
            fill_rect(x_1, y_1, x_2 - x_1, line_width, needed_color.red(), needed_color.green(), needed_color.blue(), needed_color.alpha());
        }
        else {
            x_2++; y_2++;
            double distance_x = static_cast<double>(x_2 - x_1);
            double distance_y = static_cast<double>(y_2 - y_1);

            double x_y_ratio = distance_x / distance_y;

            if (abs(x_y_ratio) < 1) {
                // Normalize positions
                if (y_1 > y_2) {
                    int temp = y_1;
                    y_1 = y_2;
                    y_2 = temp;
                    temp = x_1;
                    x_1 = x_2;
                    x_2 = temp;
                }

                double actual_x = x_1;
                double actual_y = y_1;

                // Check the positions
                if(actual_y < 0) {actual_x += x_y_ratio * std::abs(actual_y);actual_y=0;}
                if(y_2 >= height()) y_2 = height() - 1;
                // Draw the line
                while (actual_y < y_2) {
                    actual_y++;
                    actual_x += x_y_ratio;
                    Color needed_color = needed_function(std::sqrt(std::pow(actual_x - first_x, 2) + std::pow(actual_y - first_y, 2)), actual_x, actual_y);
                    force_pixel(actual_x, actual_y, needed_color.red(), needed_color.green(), needed_color.blue(), needed_color.alpha(), line_width);
                }
            }
            else {
                // Normalize positions
                if (x_1 > x_2) {
                    int temp = x_1;
                    x_1 = x_2;
                    x_2 = temp;
                    temp = y_1;
                    y_1 = y_2;
                    y_2 = temp;
                }

                double actual_x = x_1;
                double actual_y = y_1;

                // Check the positions
                double y_x_ratio = distance_y / distance_x;
                if(actual_x < 0) {actual_y += y_x_ratio * std::abs(actual_x);actual_x=0;}
                if(x_2 >= width()) x_2 = width() - 1;
                // Draw the line
                while (actual_x < x_2) {
                    actual_y += y_x_ratio;
                    actual_x++;
                    Color needed_color = needed_function(std::sqrt(std::pow(actual_x - first_x, 2) + std::pow(actual_y - first_y, 2)), actual_x, actual_y);
                    force_pixel(actual_x, actual_y, needed_color.red(), needed_color.green(), needed_color.blue(), needed_color.alpha(), line_width);
                }
            }
        }
    }

    // Draw a rectangle on the image
    void __Image_Base::draw_rect(int x, int y, int width, int height, unsigned int rect_width, Color color) {draw_rect(x, y, width, height, rect_width, color.red(), color.green(), color.blue(), color.alpha());}
    void __Image_Base::draw_rect(int x, int y, int width, int height, unsigned int rect_width, Color color, Color fill_color) {draw_rect(x, y, width, height, rect_width, color.red(), color.green(), color.blue(), color.alpha());fill_rect(x + rect_width, y + rect_width, width - rect_width * 2, height - rect_width * 2, fill_color.red(), fill_color.green(), fill_color.blue(), fill_color.alpha());}
    void __Image_Base::draw_rect(int x, int y, int width, int height, unsigned int rect_width, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
        fill_rect(x, y, width, rect_width, red, green, blue, alpha);
        fill_rect(x, y + rect_width, rect_width, height - rect_width, red, green, blue, alpha);
        fill_rect(x + (width - rect_width), y + rect_width, rect_width, height - rect_width, red, green, blue, alpha);
        fill_rect(x + rect_width, y + (height - rect_width), width - 2 * rect_width, rect_width, red, green, blue, alpha);
    }

    // Fill a rectangle on the image
    void __Image_Base::fill_rect_force(int x, int y, unsigned short rect_width, unsigned short rect_height, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha){
        // Check the position
        if(y >= static_cast<int>(height()) || x >= static_cast<int>(width()) || y + static_cast<int>(rect_height) < 0 || x + static_cast<int>(rect_width) < 0) return;
        if(x < 0) {x=-x;rect_width -= static_cast<unsigned short>(x);x = 0;}
        if(x + rect_width >= width()) rect_width = width() - x;
        if(y < 0) {y=-y;rect_height -= static_cast<unsigned short>(y);y = 0;}
        if(y + rect_height >= height()) rect_height = height() - y;
        // Normalise the values
        unsigned char needed_alpha = (normalize_value(alpha, 0, 255));
        unsigned char needed_blue = (normalize_value(blue, 0, 255));
        unsigned char needed_green = (normalize_value(green, 0, 255));
        unsigned char needed_red = (normalize_value(red, 0, 255));
        // Fill the rect
        unsigned int needed_components = components();
        unsigned int current_position = (y * width() + x) * needed_components;
        unsigned int to_add_line = (width() - rect_width) * needed_components;
        for (int i = 0; i < rect_height; i++) {
            for (int j = 0; j < rect_width; j++) {
                set_pixel_rgba_directly(current_position, needed_red, needed_green, needed_blue, needed_alpha, 1);
                current_position+=needed_components;
            } current_position += to_add_line;
        }
    }
    void __Image_Base::fill_rect(int x, int y, unsigned short width, unsigned short height, Color color) {fill_rect(x, y, width, height, color.red(), color.green(), color.blue(), color.alpha());}
    void __Image_Base::fill_rect(int x, int y, unsigned short rect_width, unsigned short rect_height, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
        // Check the position
        if(y >= static_cast<int>(height()) || x >= static_cast<int>(width()) || y + static_cast<int>(rect_height) < 0 || x + static_cast<int>(rect_width) < 0) return;
        if(x < 0) {x=-x;rect_width -= static_cast<unsigned short>(x);x = 0;}
        if(x + rect_width >= width()) rect_width = width() - x;
        if(y < 0) {y=-y;rect_height -= static_cast<unsigned short>(y);y = 0;}
        if(y + rect_height >= height()) rect_height = height() - y;
        // Normalise the values
        unsigned char needed_alpha = (normalize_value(alpha, 0, 255));
        unsigned char needed_blue = (normalize_value(blue, 0, 255));
        unsigned char needed_green = (normalize_value(green, 0, 255));
        unsigned char needed_red = (normalize_value(red, 0, 255));
        // Fill the rect
        unsigned int needed_components = components();
        unsigned int current_position = (y * width() + x) * needed_components;
        unsigned int to_add_line = (width() - rect_width) * needed_components;
        for (int i = 0; i < rect_height; i++) {
            for (int j = 0; j < rect_width; j++) {
                paste_pixel_rgba_directly(current_position, needed_red, needed_green, needed_blue, needed_alpha, 1);
                current_position+=needed_components;
            } current_position += to_add_line;
        }
    }

    // Fill a rectangle on the image
    void __Image_Base::fill_triangle(short x_1, short y_1, short x_2, short y_2, short x_3, short y_3, Color color) {fill_triangle(x_1, y_1, x_2, y_2, x_3, y_3, color.red(), color.green(), color.blue(), color.alpha());}
    void __Image_Base::fill_triangle(short x_1, short y_1, short x_2, short y_2, short x_3, short y_3, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
        // 3 should be the point with the largest X value
        if (x_1 > x_3) {
            short temp = x_1;
            x_1 = x_3;
            x_3 = temp;
            temp = y_1;
            y_1 = y_3;
            y_3 = temp;
        } if (x_2 > x_3) {
            short temp = x_2;
            x_2 = x_3;
            x_3 = temp;
            temp = y_2;
            y_2 = y_3;
            y_3 = temp;
        }

        // 2 should be the point with the largest X value of 1 and 2
        if (x_1 > x_2) {
            short temp = x_2;
            x_2 = x_1;
            x_1 = temp;
            temp = y_2;
            y_2 = y_1;
            y_1 = temp;
        }

        // Calculate the datas for the X1 to X2 line
        double distance_x_1_2 = static_cast<double>(x_2 - x_1);
        double distance_y_1_2 = static_cast<double>(y_2 - y_1);
        std::vector<long long> all_x_1_2;
        if(distance_x_1_2 > 0){all_x_1_2 = partition_number(distance_y_1_2, distance_x_1_2);}

        // Calculate the datas for the X2 to X3 line
        double distance_x_2_3 = static_cast<double>(x_3 - x_2);
        double distance_y_2_3 = static_cast<double>(y_3 - y_2);
        std::vector<long long> all_x_2_3;
        if(distance_x_2_3 > 0){all_x_2_3 = partition_number(distance_y_2_3, distance_x_2_3);}

        // Calculate the datas for the X1 to X3 line
        double distance_x_1_3 = static_cast<double>(x_3 - x_1);
        double distance_y_1_3 = static_cast<double>(y_3 - y_1);
        std::vector<long long> all_x_1_3 = partition_number(distance_y_1_3, distance_x_1_3);

        double actual_x = x_1;
        double actual_y = y_1;
        double actual_y_added = 0;
        double total_added_y = 0;

        // X drawing
        // Create the first line
        int iter = 0; int iter_total = 0;
        while(actual_x < x_2) {
            draw_line(actual_x, actual_y + total_added_y, actual_x, actual_y + actual_y_added, red, green, blue, alpha);
            actual_x++; total_added_y += all_x_1_3[iter_total];
            iter++; iter_total++; if(iter < static_cast<int>(all_x_1_2.size())){actual_y_added += all_x_1_2[iter];}
        }
        // Create the second line
        actual_y_added = distance_y_1_2; iter = 0;
        while(actual_x < x_3) {
            draw_line(actual_x, actual_y + total_added_y, actual_x, actual_y + actual_y_added, red, green, blue, alpha);
            actual_x++; total_added_y += all_x_1_3[iter_total];
            iter++; iter_total++; actual_y_added += all_x_2_3[iter];
        }
    }

    // Flip the image on the X axis
    void __Image_Base::flip_x() {
        int total_height = height();
        int total_height_needed = total_height / 2.0;

        for (int i = 0; i < total_height_needed; i++) {
            // Needed datas
            int base_current_pos = (i * width()) * components();
            int base_next_pos = ((total_height - (i + 1)) * width()) * components();

            for (int j = 0; j < width(); j++) {
                int current_pos = base_current_pos + j * components(); int next_pos = base_next_pos + j * components();

                // Red
                char temp = a_pixels.get()->data_at_directly(current_pos);
                a_pixels->set_data_at_directly(current_pos, a_pixels.get()->data_at_directly(next_pos));
                a_pixels->set_data_at_directly(next_pos, temp);

                // Green
                temp = a_pixels.get()->data_at_directly(current_pos + 1);
                a_pixels->set_data_at_directly(current_pos + 1, a_pixels.get()->data_at_directly(next_pos + 1));
                a_pixels->set_data_at_directly(next_pos + 1, temp);

                // Blue
                temp = a_pixels.get()->data_at_directly(current_pos + 2);
                a_pixels->set_data_at_directly(current_pos + 2, a_pixels.get()->data_at_directly(next_pos + 2));
                a_pixels->set_data_at_directly(next_pos + 2, temp);
            }
        }

        // Alpha
        if(color_type() == 6) {
            for (int i = 0; i < total_height_needed; i++) {
                int base_current_pos = (i * width()) * components();
                int base_next_pos = ((total_height - (i + 1)) * width()) * components();

                for (int j = 0; j < width(); j++){
                    int current_pos = base_current_pos + j * components(); int next_pos = base_next_pos + j * components();
                    char temp = a_pixels.get()->data_at_directly(current_pos + 3);
                    a_pixels->set_data_at_directly(current_pos + 3, a_pixels.get()->data_at_directly(next_pos + 3));
                    a_pixels->set_data_at_directly(next_pos + 3, temp);
                }
            }
        } a_flip_x_number++;
    }
    // Flip the image on the Y axis
    void __Image_Base::flip_y() {
        unsigned char* line1 = new unsigned char[height()];
        int max = width();

        for (int i = 0; i < floor(static_cast<float>(max) / 2.0); i++) {
            // Red
            for (int j = 0; j < height(); j++) line1[j] = a_pixels->data_at((i + j * width()) * components());
            for (int j = 0; j < height(); j++) a_pixels->set_data_at((i + j * width()) * components(), a_pixels->data_at(((max - (i + 1)) + j * width()) * components()));
            for (int j = 0; j < height(); j++) a_pixels->set_data_at(((max - (i + 1)) + j * width()) * components(), line1[j]);

            // Green
            for (int j = 0; j < height(); j++) line1[j] = a_pixels->data_at((i + j * width()) * components() + 1);
            for (int j = 0; j < height(); j++) a_pixels->set_data_at((i + j * width()) * components() + 1, a_pixels->data_at(((max - (i + 1)) + j * width()) * components() + 1));
            for (int j = 0; j < height(); j++) a_pixels->set_data_at(((max - (i + 1)) + j * width()) * components() + 1, line1[j]);

            // Blue
            for (int j = 0; j < height(); j++) line1[j] = a_pixels->data_at((i + j * width()) * components() + 2);
            for (int j = 0; j < height(); j++) a_pixels->set_data_at((i + j * width()) * components() + 2, a_pixels->data_at(((max - (i + 1)) + j * width()) * components() + 2));
            for (int j = 0; j < height(); j++) a_pixels->set_data_at(((max - (i + 1)) + j * width()) * components() + 2, line1[j]);

            if(color_type() == 6) {
                // Alpha
                for (int j = 0; j < height(); j++) line1[j] = a_pixels->data_at((i + j * width()) * components() + 3);
                for (int j = 0; j < height(); j++) a_pixels->set_data_at((i + j * width()) * components() + 3, a_pixels->data_at(((max - (i + 1)) + j * width()) * components() + 3));
                for (int j = 0; j < height(); j++) a_pixels->set_data_at(((max - (i + 1)) + j * width()) * components() + 3, line1[j]);
            }
        }

        delete[] line1;
    }

    // Paste an Image on this Image
    void __Image_Base::paste(std::string path, int x, int y, double opacity) {__Image_Base* img = new __Image_Base(path);paste(img, x, y, opacity);delete img; img = 0;};
    void __Image_Base::paste(Image* to_paste, int x, int y, double opacity){paste(to_paste->image(), x, y, opacity);}
    void __Image_Base::paste(Image to_paste, int x, int y, double opacity){paste(to_paste.image(), x, y, opacity);}
    void __Image_Base::paste(Image* to_paste, int x, int y){paste(to_paste->image(), x, y, 1.0);}
    void __Image_Base::paste(Image to_paste, int x, int y){paste(to_paste.image(), x, y, 1.0);}
    void __Image_Base::paste(__Image_Base* to_paste, int x, int y, double opacity) {
        // Assertsg
        if(to_paste == 0){print(std::string("SCLS Image"), std::string("Can't print an empty pointer (at x = ") + std::to_string(x) + std::string(" and y = ") + std::to_string(x) + std::string(")."));return;}

        unsigned int current_thread_position = 0;
        unsigned int pixel_by_thread = floor((static_cast<double>(to_paste->width()) * static_cast<double>(to_paste->height())) / static_cast<double>(a_thread_number_for_pasting));

        // Asserts
        if(x >= width() || y >= height()){return;}

        // Create each threads
        if(a_thread_number_for_pasting > 1) {
            std::vector<std::thread*> threads = std::vector<std::thread*>();
            for(unsigned short i = 0;i<a_thread_number_for_pasting - 1;i++) {
                unsigned int start_x = floor(current_thread_position % to_paste->width());
                unsigned int start_y = floor(current_thread_position / to_paste->width());

                std::thread* current_thread = new std::thread(&__Image_Base::__paste_part_of_image, this, to_paste, x, y, start_x, start_y, pixel_by_thread, opacity);
                threads.push_back(current_thread);
                current_thread_position += pixel_by_thread;
            }
            unsigned int start_x = floor(current_thread_position % to_paste->width());
            unsigned int start_y = floor(current_thread_position / to_paste->width());
            std::thread* current_thread = new std::thread(&__Image_Base::__paste_part_of_image, this, to_paste, x, y, start_x, start_y, (static_cast<double>(to_paste->width()) * static_cast<double>(to_paste->height())) - current_thread_position , opacity);
            threads.push_back(current_thread);

            // Wait for each threads
            for(int i = 0;i<static_cast<int>(threads.size());i++) {
                threads[i]->join();
                delete threads[i]; threads[i] = 0;
            } threads.clear();
        }
        else {
            // Precisely adjust the image to paste
            int start_x = 0;int start_y = 0;
            int needed_height = to_paste->height();
            int needed_width = to_paste->width();
            if(x < 0){needed_width+=x;start_x=-x;x=0;}
            if(y < 0){needed_height+=y;start_y=-y;y=0;}
            if(start_x + needed_width > width()){needed_width = (width() - start_x);}
            if(start_y + needed_height > height()){needed_height = (height() - start_y);}
            if(start_x < to_paste->width() && start_y < to_paste->height()) {__paste_part_of_image(to_paste, x, y, start_x, start_y, (needed_width * needed_height), opacity);}
        }
    }
    // Paste a part of an image on this image
    void __Image_Base::__paste_part_of_image(__Image_Base* to_paste, int x_offset, int y_offset, int start_x, int start_y, int length, double opacity) {
        // Paste image datas
        int needed_component_paste = to_paste->components();
        int needed_height_paste = to_paste->height(); int needed_width_paste = to_paste->width();
        int current_position_paste = ((start_y) * needed_width_paste + (start_x)) * needed_component_paste;
        int current_x_to_paste = start_x; int current_y_to_paste = start_y;

        // Current image datas
        int current_x = x_offset; int current_y = y_offset;
        int needed_component = components();
        int needed_height = height(); int needed_width = width();
        int current_position = ((current_y) * needed_width + (current_x)) * needed_component;
        if(color_type() == SCLS_IMAGE_RGBA) {
            for(int i = 0;i<length;i++) {
                // Sets the pixel
                Color pixel = to_paste->pixel_rgba_directly(current_position_paste, 1);
                set_pixel_rgba_directly_with_alpha(current_position, pixel.red(), pixel.green(), pixel.blue(), (static_cast<double>(pixel.alpha()) * opacity), 1);

                // Check the size
                current_position += needed_component;
                current_position_paste += needed_component_paste;
                current_x++;
                if((start_x + (current_x - x_offset)) >= needed_width_paste || current_x >= needed_width) {
                    current_x = x_offset;
                    current_y++;current_y_to_paste++;
                    if(current_y - y_offset >= needed_height_paste || current_y >= needed_height || current_y_to_paste >= to_paste->height()){break;}
                    current_position = ((current_y) * needed_width + (current_x)) * needed_component;
                    current_position_paste = ((start_y + (current_y - y_offset)) * needed_width_paste + (start_x + (current_x - x_offset))) * needed_component_paste;
                }
            }
        }
        else if(color_type() == SCLS_IMAGE_RGB) {}
    }

    // Pastes an Image to the bottom / center / top of the image
    void __Image_Base::paste_bottom_center(__Image_Base* to_paste, double offset_y){paste_bottom_center(to_paste, offset_y, 1);};
    void __Image_Base::paste_bottom_center(__Image_Base* to_paste, double offset_y, double opacity){paste(to_paste, width() / 2 - to_paste->width() / 2, (height() - to_paste->height()) - offset_y, opacity);}
    void __Image_Base::paste_center(std::shared_ptr<__Image_Base> to_paste, double opacity){paste_center(to_paste.get(), opacity);};
    void __Image_Base::paste_center(__Image_Base* to_paste, double opacity){paste(to_paste, width() / 2 - to_paste->width() / 2, height() / 2 - to_paste->height() / 2, opacity);}
    void __Image_Base::paste_top_center(__Image_Base* to_paste, double offset_y){paste_top_center(to_paste, offset_y, 1);};
    void __Image_Base::paste_top_center(__Image_Base* to_paste, double offset_y, double opacity){paste(to_paste, width() / 2 - to_paste->width() / 2, offset_y, opacity);}

    // Paste datas to a specific pixel
    void __Image_Base::paste_pixel_rgba_directly(unsigned int position, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, unsigned char multiplier){
        double alpha_double = static_cast<double>(static_cast<unsigned char>(alpha)) / 255.0;
        double multiple = (static_cast<double>(static_cast<unsigned char>(a_pixels->data_at_directly(position + 3 * multiplier))) / 255.0) * (1.0 - alpha_double);
        a_pixels->set_data_at_directly(position, static_cast<unsigned int>(static_cast<double>(static_cast<unsigned char>(red)) * alpha_double + static_cast<double>(static_cast<unsigned char>(a_pixels->data_at_directly(position))) * multiple));
        a_pixels->set_data_at_directly(position + multiplier, static_cast<unsigned int>(static_cast<double>(static_cast<unsigned char>(green)) * alpha_double + static_cast<double>(static_cast<unsigned char>(a_pixels->data_at_directly(position + multiplier))) * multiple));
        a_pixels->set_data_at_directly(position + 2 * multiplier, static_cast<unsigned int>(static_cast<double>(static_cast<unsigned char>(blue)) * alpha_double + static_cast<double>(static_cast<unsigned char>(a_pixels->data_at_directly(position + 2 * multiplier))) * multiple));
        a_pixels->set_data_at_directly(position + 3 * multiplier, static_cast<unsigned int>((alpha_double + multiple) * 255.0));
    };

    // Apply a more complex horizontal gradient from the left to the right in the image
    void __Image_Base::apply_gradient_horizontal(Color left, Color right) { apply_gradient_horizontal(left, right, 0, width()); };
    void __Image_Base::apply_gradient_horizontal(Color left, Color right, int start_x, int end_x) {
        // Calculate the variant of each colors
        unsigned int current_x = end_x - start_x;
        double alpha_variant = (static_cast<double>(right.alpha()) - static_cast<double>(left.alpha())) / static_cast<double>(current_x - 1);
        double blue_variant = (static_cast<double>(right.blue()) - static_cast<double>(left.blue())) / static_cast<double>(current_x - 1);
        double green_variant = (static_cast<double>(right.green()) - static_cast<double>(left.green())) / static_cast<double>(current_x - 1);
        double red_variant = (static_cast<double>(right.red()) - static_cast<double>(left.red())) / static_cast<double>(current_x - 1);

        // Apply the gradiant
        double current_alpha = left.alpha();
        double current_blue = left.blue();
        double current_green = left.green();
        double current_red = left.red();
        for(unsigned int i = 0;i<current_x;i++) {
            Color current_color = Color(current_red, current_green, current_blue, current_alpha);
            if(height() > 1) draw_line(start_x + i, 0, start_x + i, height() - 1, current_color);
            else draw_line(start_x + i, 0, start_x + i, 1, current_color);

            current_alpha += alpha_variant;
            current_blue += blue_variant;
            current_green += green_variant;
            current_red += red_variant;
        }
    };

    // Apply a more complex vertical gradient from the left to the right in the image
    void __Image_Base::apply_gradient_vertical(Color top, Color bottom) { apply_gradient_vertical(top, bottom, 0, height()); }
    void __Image_Base::apply_gradient_vertical(Color top, Color bottom, int start_y, int end_y) {
        // Calculate the variant of each colors
        unsigned int current_y = end_y - start_y;
        double alpha_variant = (static_cast<double>(bottom.alpha()) - static_cast<double>(top.alpha())) / static_cast<double>(current_y - 1);
        double blue_variant = (static_cast<double>(bottom.blue()) - static_cast<double>(top.blue())) / static_cast<double>(current_y - 1);
        double green_variant = (static_cast<double>(bottom.green()) - static_cast<double>(top.green())) / static_cast<double>(current_y - 1);
        double red_variant = (static_cast<double>(bottom.red()) - static_cast<double>(top.red())) / static_cast<double>(current_y - 1);

        // Apply the gradiant
        double current_alpha = top.alpha();
        double current_blue = top.blue();
        double current_green = top.green();
        double current_red = top.red();
        for(unsigned int i = 0;i<current_y;i++) {
            Color current_color = Color(current_red, current_green, current_blue, current_alpha);
            if(width() > 1) draw_line(0, start_y + i, width() - 1, start_y + i, current_color);
            else draw_line(0, start_y + i, 1, start_y + i, current_color);

            current_alpha += alpha_variant;
            current_blue += blue_variant;
            current_green += green_variant;
            current_red += red_variant;
        }
    };

    // Returns a shared ptr of the image with a new size, adaptated
    std::shared_ptr<__Image_Base> __Image_Base::resize_adaptative(unsigned short new_width, unsigned short new_height) {if(new_width == width() && new_height == height()){return copy_image();} std::shared_ptr<__Image_Base> current_image = resize_adaptative_width(new_width);if(current_image.get() == 0) return resize_adaptative_height(new_height);return current_image.get()->resize_adaptative_height(new_height); };
    std::shared_ptr<__Image_Base> __Image_Base::resize_adaptative(scls::Point_2D new_size) {return resize_adaptative(new_size.x(), new_size.y()); };
    // Returns a shared ptr of the image with a new height, adaptated
    std::shared_ptr<__Image_Base> __Image_Base::resize_adaptative_height(unsigned short new_height) {
        if(new_height < height()) {
            // Create the new image
            std::shared_ptr<__Image_Base> new_image = std::make_shared<__Image_Base>(width(), new_height, Color(0, 0, 0, 0));
            new_image.get()->a_flip_x_number = a_flip_x_number;

            std::vector<long long> repartioned_pixels = partition_number(height(), new_height);
            unsigned short current_y = 0;
            for(int i = 0;i<new_height;i++) {
                for(int j = 0;j<width();j++) {
                    // Calculate the median color
                    float alpha_value = 0;
                    float blue_value = 0;
                    float green_value = 0;
                    float red_value = 0;
                    for(int k = 0;k<repartioned_pixels[i];k++) {
                        Color current_color = pixel(j, current_y + k);
                        alpha_value += current_color.alpha();
                        blue_value += current_color.blue();
                        green_value += current_color.green();
                        red_value += current_color.red();
                    }
                    alpha_value /= repartioned_pixels[i];
                    blue_value /= repartioned_pixels[i];
                    green_value /= repartioned_pixels[i];
                    red_value /= repartioned_pixels[i];

                    // Apply the median color
                    new_image.get()->set_pixel_rgba_directly((j + i * new_image->width()) * new_image->components(), red_value, green_value, blue_value, alpha_value, 1);
                }
                current_y += repartioned_pixels[i];
            }
            return new_image;
        }
        else if(new_height == height()){return copy_image();}

        // Bigger height
        std::shared_ptr<__Image_Base> new_image = std::make_shared<__Image_Base>(width(), new_height, Color(0, 0, 0, 0));
        new_image.get()->a_flip_x_number = a_flip_x_number;

        std::vector<long long> repartioned_pixels = partition_number(new_height, height());
        int y_normal = 0;
        for(int i = 0;i<height();i++) {
            for(int j = 0;j<width();j++) {
                // Calculate the color
                Color current_color = pixel(j, i);

                // Apply the color
                for(int k = 0;k<repartioned_pixels.at(i);k++){new_image.get()->set_pixel_rgba_directly((j + (y_normal + k) * new_image->width()) * new_image->components(), current_color.red(), current_color.green(), current_color.blue(), current_color.alpha(), 1);}
            }
            y_normal+=repartioned_pixels.at(i);
        }
        return new_image;
    };
    // Returns a shared ptr of the image with a new width, adaptated
    std::shared_ptr<__Image_Base> __Image_Base::resize_adaptative_width(unsigned short new_width) {
        if(new_width < width()) {
            // Create the new image
            std::shared_ptr<__Image_Base> new_image = std::make_shared<__Image_Base>(new_width, height(), Color(0, 0, 0, 0));
            new_image.get()->a_flip_x_number = a_flip_x_number;

            std::vector<long long> repartioned_pixels = partition_number(width(), new_width);
            unsigned short current_x = 0;
            for(int i = 0;i<new_width;i++) {
                for(int j = 0;j<height();j++) {
                    // Calculate the median color
                    float alpha_value = 0;
                    float blue_value = 0;
                    float green_value = 0;
                    float red_value = 0;
                    for(int k = 0;k<repartioned_pixels[i];k++) {
                        Color current_color = pixel(current_x + k, j);
                        alpha_value += current_color.alpha();
                        blue_value += current_color.blue();
                        green_value += current_color.green();
                        red_value += current_color.red();
                    }
                    alpha_value /= repartioned_pixels[i];
                    blue_value /= repartioned_pixels[i];
                    green_value /= repartioned_pixels[i];
                    red_value /= repartioned_pixels[i];

                    // Apply the median color
                    new_image.get()->set_pixel(i, j, Color(red_value, green_value, blue_value, alpha_value));
                }
                current_x += repartioned_pixels[i];
            }
            return new_image;
        }
        else if(new_width == width()){return copy_image();}

        // Bigger width
        std::shared_ptr<__Image_Base> new_image = std::make_shared<__Image_Base>(new_width, height(), Color(0, 0, 0, 0));
        new_image.get()->a_flip_x_number = a_flip_x_number;

        std::vector<long long> repartioned_pixels = partition_number(new_width, width());
        int x_normal = 0;
        for(int i = 0;i<height();i++) {
            for(int j = 0;j<width();j++) {
                // Calculate the color
                Color current_color = pixel(j, i);

                // Apply the color
                for(int k = 0;k<repartioned_pixels.at(j);k++){new_image.get()->set_pixel_rgba_directly(((x_normal + k) + i * new_image->width()) * new_image->components(), current_color.red(), current_color.green(), current_color.blue(), current_color.alpha(), 1);}
                x_normal+=repartioned_pixels.at(j);
            }
            x_normal = 0;
        }
        return new_image;
    };

    // Rotates the image
    void __normalize_angle(double& angle){while(angle>=180.0){angle-=360.0;}while(angle<-180.0){angle+=360.0;}}
    void __Image_Base::rotate(double angle) {
        __normalize_angle(angle);
        if(angle == 0){return;}

        // Creates an image
        Point_2D new_size = rotated_size(angle);Point_2D old_size = Point_2D(width(), height());
        std::shared_ptr<Bytes_Set> new_datas = std::shared_ptr<Bytes_Set>();
        __fill(new_datas, 0, 0, 0, 0, new_size.x(), new_size.y());

        // Sets each pixels
        int multiplier = 1;
        int needed_components = components();
        for(int i = 0;i<new_size.x();i++) {
            for(int j = 0;j<new_size.y();j++) {
                Point_2D point = ((Point_2D(i, j) - new_size / 2).rotated(-angle) + old_size / 2);
                point.set_x(round(point.x()));point.set_y(round(point.y()));
                if(point.x() >= 0 && point.y() >= 0 && point.x() < width() && point.y() < height()) {
                    // Get the needed color
                    int position_base = (point.x() + point.y() * width()) * components();
                    Color needed_color = pixel_rgba_directly(position_base, multiplier);

                    // Set the color
                    int position_end = (i + j * new_size.x()) * needed_components;
                    new_datas.get()->set_data_at_directly(position_end, needed_color.red());
                    new_datas.get()->set_data_at_directly(position_end + 1, needed_color.green());
                    new_datas.get()->set_data_at_directly(position_end + 2, needed_color.blue());
                    new_datas.get()->set_data_at_directly(position_end + 3, needed_color.alpha());
                }
            }
        }

        // Sets the result
        a_height = new_size.y();
        a_pixels = new_datas;
        a_width = new_size.x();
    }
    std::shared_ptr<__Image_Base> __Image_Base::rotated(double angle) {std::shared_ptr<__Image_Base> current_copy = copy_image();current_copy.get()->rotate(angle);return current_copy;}
    Point_2D __Image_Base::rotated_size(double angle){
        Point_2D needed_vector_horizontal = Point_2D();Point_2D needed_vector_vertical = Point_2D();
        if((angle >= 0.0 && angle <= 90.0) || angle <= -90.0){
            needed_vector_horizontal = Point_2D(-width() / 2.0, -height() / 2.0).rotated(angle);
            needed_vector_vertical = Point_2D(-width() / 2.0, height() / 2.0).rotated(angle);
        }
        else{
            needed_vector_horizontal = Point_2D(-width() / 2.0, height() / 2.0).rotated(angle);
            needed_vector_vertical = Point_2D(-width() / 2.0, -height() / 2.0).rotated(angle);
        }
        return Point_2D(round(std::abs(needed_vector_horizontal.x())), round(std::abs(needed_vector_vertical.y()))) * 2.0;
    }

    // Load the image from a set of binary datas coming from a FreeType text
    bool __Image_Base::_load_from_text_binary(char* datas, int width, int height, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
        a_height = height; a_width = width;
        create_memory();
        if(alpha == 0){fill(0, 0, 0, 0);return true;}

        // Create each threads
        if(a_thread_number_for_pasting_text > 0) {
            unsigned int current_thread_position = 0;
            unsigned int pixel_by_thread = floor((static_cast<double>(width) * static_cast<double>(height)) / static_cast<double>(a_thread_number_for_pasting_text));

            std::vector<std::thread*> threads = std::vector<std::thread*>();
            for(unsigned short i = 0;i<a_thread_number_for_pasting_text - 1;i++) {
                unsigned int start_x = floor(current_thread_position % width);
                unsigned int start_y = floor(current_thread_position / width);

                std::thread* current_thread = new std::thread(&__Image_Base::__load_part_from_text_binary, this, datas, current_thread_position, start_x, start_y, pixel_by_thread, red, green, blue, alpha);
                threads.push_back(current_thread);
                current_thread_position += pixel_by_thread;
            }
            unsigned int start_x = floor(current_thread_position % width);
            unsigned int start_y = floor(current_thread_position / width);
            std::thread* current_thread = new std::thread(&__Image_Base::__load_part_from_text_binary, this, datas, current_thread_position, start_x, start_y, (static_cast<double>(width) * static_cast<double>(height)) - current_thread_position, red, green, blue, alpha);
            threads.push_back(current_thread);

            // Wait for each threads
            for(int i = 0;i<static_cast<int>(threads.size());i++) {
                threads[i]->join();
                delete threads[i]; threads[i] = 0;
            } threads.clear();
        }
        else {__load_part_from_text_binary(datas, 0, 0, 0, (static_cast<double>(width) * static_cast<double>(height)), red, green, blue, alpha);}

        return true;
    };

    // Load a part of image with a FreeType text in it
    bool __Image_Base::__load_part_from_text_binary(char* datas, int offset, int start_x, int start_y, int length, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
        int needed_component = components();
        unsigned int current_position = (start_y * width() + start_x) * needed_component;
        if(color_type() == SCLS_IMAGE_RGBA) {
            for(int i = 0;i<length;i++) {
                double glyph_alpha = static_cast<double>(static_cast<unsigned char>(datas[offset + i])) / 255.0;
                set_pixel_rgba_directly(current_position, red, green, blue, static_cast<unsigned short>(glyph_alpha * static_cast<double>(alpha)), 1);
                current_position+=needed_component;
            }
        }
        else if(color_type() == SCLS_IMAGE_RGB) {
            for(int i = 0;i<length;i++) {
                set_pixel_directly(current_position, red, green, blue, 1);
                current_position+=needed_component;
            }
        }
        return true;
    };
}
