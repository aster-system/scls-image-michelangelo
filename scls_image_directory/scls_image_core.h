//******************
//
// scls_image_core.h
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
// This file contains the core of SCLS Image.
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

#ifndef SCLS_IMAGE_CORE
#define SCLS_IMAGE_CORE

// Avoid some errors with libraries path
// ZLib
#ifndef SCLS_ZLIB_PATH
#define SCLS_ZLIB_PATH <zlib.h>
#endif // SCLS_ZLIB_PATH
// Freetype
#ifndef SCLS_FREETYPE_PATH
#if __has_include("ft2build.h")
#define SCLS_FREETYPE_PATH <ft2build.h>
#elif __has_include("freetype/ft2build.h")
#define SCLS_FREETYPE_PATH <freetype/ft2build.h>
#endif // __has_included
#endif // SCLS_FREETYPE_PATH

// Include ZLib
#include SCLS_ZLIB_PATH
// Include Freetype
#include SCLS_FREETYPE_PATH
#include FT_FREETYPE_H

// Include basics C++ libraries
#include <map>
#include <math.h>
#include <memory>
#include <stack>
#include <thread>

// ZLib mandatory stuff
#if defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) (void)_setmode(_fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif

#define PNG_CRC_POLY 0x04C11DB7

// Define each PNG possibles errors
#ifndef SCLS_IMAGE_ERROR_UNKNOW_FILE
#define SCLS_IMAGE_ERROR_UNKNOW_FILE -1
#endif
#ifndef SCLS_IMAGE_PNG_ERROR_BAD_FILE_SIGNATURE
#define SCLS_IMAGE_PNG_ERROR_BAD_FILE_SIGNATURE -2
#endif
#ifndef SCLS_IMAGE_PNG_ERROR_BIT_DEPHT
#define SCLS_IMAGE_PNG_ERROR_BIT_DEPHT -100
#endif
#ifndef SCLS_IMAGE_PNG_ERROR_NO_CHUNK
#define SCLS_IMAGE_PNG_ERROR_NO_CHUNK -101
#endif
#ifndef SCLS_IMAGE_PNG_ERROR_PLTE_CHUNK
#define SCLS_IMAGE_PNG_ERROR_PLTE_CHUNK -102
#endif
#ifndef SCLS_IMAGE_PNG_ERROR_UNKNOW_COLOR_TYPE
#define SCLS_IMAGE_PNG_ERROR_UNKNOW_COLOR_TYPE -103
#endif
#ifndef SCLS_IMAGE_PNG_ERROR_WRONG_PHYS_CHUNK
#define SCLS_IMAGE_PNG_ERROR_WRONG_PHYS_CHUNK -104
#endif
#ifndef SCLS_IMAGE_PNG_ERROR_WRONG_SRGB_CHUNK
#define SCLS_IMAGE_PNG_ERROR_WRONG_SRGB_CHUNK -105
#endif
#ifndef SCLS_IMAGE_ERROR_UNKNOW
#define SCLS_IMAGE_ERROR_UNKNOW -2000000000
#endif

// Define each possibles image type
#ifndef SCLS_IMAGE_RGB
#define SCLS_IMAGE_RGB 2
#endif
#ifndef SCLS_IMAGE_RGBA
#define SCLS_IMAGE_RGBA 6
#endif

// The namespace "scls" is used to simplify the all.
namespace scls {
    //*********
	//
	// Color handling
	//
	//*********

	class Color {
	    // Class representing an 8 bit RGBA color
    public:
        // Basic Color constructor
        Color(short red, short green, short blue);
        Color(short red, short green, short blue, short alpha);
        Color():Color(0,0,0,255){}
        // Color copy constructor
        Color(const Color& color_copy):a_alpha(color_copy.a_alpha),a_blue(color_copy.a_blue),a_green(color_copy.a_green),a_red(color_copy.a_red){};

        // Return a color loaded from a text
        static std::vector<std::string> from_std_string_parts(std::string source);
        static Color from_std_string(std::string source);
        // Return a color loaded from an XML balise
        static Color from_xml(std::shared_ptr<__XML_Text_Base> source);
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

	//*********
	//
	// Compression handling
	//
	//*********

	// Compress data from a char array with / without returning the result
    int _compress_binary(char* to_compress, unsigned int to_compress_size, char* output, unsigned int output_size, unsigned int& total_output_size, unsigned int compression_level = 9);
    char* compress_binary(char* to_compress, unsigned int to_compress_size, unsigned int& output_size, unsigned int compression_level = 9);

	// Uncompress data from a char array
    int uncompress_binary(char* to_uncompress, unsigned int to_uncompress_size, char* output, unsigned int output_size);

    //*********
	//
	// The Image class
	//
	//*********

	// Struct representing datas about a PNG chunk
	struct _PNG_Chunk {
		// Position of the chunk in the file
		unsigned int position = 0;
		// Name of the chunk
		std::string name = "";
		// Size of the chunk
		unsigned int size = 0;
	};

	class __Image_Error {
	    // Class representing an image text error handler
    public:
        // __Image_Error constructor
        __Image_Error(){};

        // Returns if there is an error
        inline bool has_error() const {return value() != 0 && value() != 1;};
        // Returns the error in a std::string
        std::string to_std_string() const;

        // Getters and setters
        inline unsigned short bit_depht() const {return a_bit_depht;};
        inline unsigned char color_type() const {return a_color_type;};
        inline std::string path() const {return a_path;};
        inline void set_path(std::string new_path) {a_path = new_path;};
        inline void set_value(int new_value) {a_value = new_value;};
        inline int value() const {return a_value;};
    private:
        // Bit depth for the error
        unsigned short a_bit_depht = 0;
        // Color type for the error
        unsigned char a_color_type = 0;
        // Path for the error
        std::string a_path = "";
        // Value of the error
        int a_value = 0;
    };

    // Linear gradient color for the Image class
    Color fill_circle_gradient_linear(double distance, int radius, int x, int y, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);

	class __Image_Base {
		// Class representing a PNG image handler
	public:

	    //*********
        //
        // The Image class
        //
        //*********

        class Image {
            // Class representing a way to safely use "__Image_Base"
        public:
            // Image constructor
            Image();
            Image(std::string path);
            Image(unsigned short width, unsigned short height, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255, unsigned int color_type = SCLS_IMAGE_RGBA);
            Image(unsigned short width, unsigned short height, Color color, unsigned int color_type);
            Image(unsigned short width, unsigned short height, Color color);
            Image(unsigned short width, unsigned short height);
            Image(std::shared_ptr<__Image_Base> new_image);

            // Fills the image with one color
            void fill(Color color);
            void fill(int red, int green, int blue);

            // Drawing methods
            // Draws a circle on the image
            void draw_circle(int x_center, int y_center, double radius, Color color, double border_radius);
            // Fills a circle on the image
            void fill_circle(int x_center, int y_center, double radius_x, double radius_y, double angle, double angle_start, double angle_end, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, double border_radius, unsigned char border_red, unsigned char border_green, unsigned char border_blue, unsigned char border_alpha);
            void fill_circle(int x_center, int y_center, double radius_x, double radius_y, double angle_start, double angle_end, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, double border_radius, unsigned char border_red, unsigned char border_green, unsigned char border_blue, unsigned char border_alpha);
            void fill_circle(int x_center, int y_center, double radius, double angle_start, double angle_end, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, double border_radius, unsigned char border_red, unsigned char border_green, unsigned char border_blue, unsigned char border_alpha);
            void fill_circle(int x_center, int y_center, double radius, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, double border_radius, unsigned char border_red, unsigned char border_green, unsigned char border_blue, unsigned char border_alpha);
            void fill_circle(int x_center, int y_center, double radius_x, double radius_y, double angle, double angle_start, double angle_end, Color color, double border_radius, Color border_color);
            void fill_circle(int x_center, int y_center, double radius_x, double radius_y, double angle_start, double angle_end, Color color, double border_radius, Color border_color);
            void fill_circle(int x_center, int y_center, double radius, double angle_start, double angle_end, Color color, double border_radius, Color border_color);
            void fill_circle(int x_center, int y_center, double radius, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
            void fill_circle(int x_center, int y_center, double radius_x, double radius_y, Color color, double border_radius, Color border_color);
            void fill_circle(int x_center, int y_center, double radius, Color color, double border_radius, Color border_color);
            void fill_circle(int x_center, int y_center, double radius, Color color);
            // Fills a rectangle on the image
            void fill_rect(int x, int y, unsigned short rect_width, unsigned short rect_height, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);
            void fill_rect(int x, int y, unsigned short width, unsigned short height, Color color);

            // Forces a pixel to change its value
            void force_pixel(int x, int y, Color color);

            // Copies the image
            Image copy_image();

            // Pastes an Image on this Image
            void paste(__Image_Base* to_paste, int x, int y, double opacity = 1.0);
            void paste(std::string path, int x, int y, double opacity = 1.0);
            void paste(Image* to_paste, int x, int y, double opacity);
            void paste(Image to_paste, int x, int y, double opacity);
            void paste(Image* to_paste, int x, int y);
            void paste(Image to_paste, int x, int y);

            // Get datas about a specific pixel
            Color pixel(unsigned short x, unsigned short y);
            Color pixel_by_number(unsigned int position);
            Color pixel_directly(unsigned int position, unsigned int multiplier);
            Color pixel_rgba_directly(unsigned int position, unsigned int multiplier);

            // Replaces a color with an other on the image
            void replace_color(Color old_color, Color new_color);

            // Returns a shared ptr of the image with a new size, adaptated
            Image resize_adaptative(unsigned short new_width, unsigned short new_height);
            Image resize_adaptative(scls::Point_2D new_size);
            Image resize_adaptative_height(unsigned short new_height);
            Image resize_adaptative_width(unsigned short new_width);

            // Rotates the image
            void rotate(double angle);
            Image rotated(double angle);

            // Sets the color of a certain pixel
            void set_pixel(int x, int y, Color color);

            // Saves / loads
            // Save the image into the PNG format
            void save_png(std::string path);

            // Getters and setters
            inline int height() const {if(a_image.get() == 0){return 0;}return a_image.get()->height();};
            inline __Image_Base* image() const {return a_image.get();};
            inline std::shared_ptr<__Image_Base> image_shared_ptr() const {return a_image;};
            inline int width() const {if(a_image.get() == 0){return 0;}return a_image.get()->width();};
        private:
            // Stored image
            std::shared_ptr<__Image_Base> a_image;
        };

	    //*********
        //
        // The __Image_Base class - Base
        //
        //*********

	    // All __Image_Base constructors
		// __Image_Base most basic constructor
		__Image_Base();
		// __Image_Base constructor with a path
		__Image_Base(std::string path);
		// __Image_Base constructor from scratch
		__Image_Base(unsigned short width, unsigned short height, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255, unsigned int color_type = SCLS_IMAGE_RGBA);
		__Image_Base(unsigned short width, unsigned short height, Color color, unsigned int color_type = SCLS_IMAGE_RGBA);
		__Image_Base(unsigned short width, unsigned short height);
		// __Image_Base copy constructor
		__Image_Base(__Image_Base& image_copy);
		__Image_Base(__Image_Base* image_copy);
        // PNG_Image destructor
		~__Image_Base();

		// Returns if the image use alpha or not
        bool use_alpha() const;

		// Basic image manipulation
		// Copies this image and returns the result
		std::shared_ptr<__Image_Base> copy_image();
		// Creates / frees the memory needed
		void __create_memory(std::shared_ptr<Bytes_Set>& current_datas, int width, int height);
		void __free_memory(std::shared_ptr<Bytes_Set>& current_datas);
        void create_memory();
        void free_memory();
		// Fills the image with one color
		void __fill(std::shared_ptr<Bytes_Set>& current_datas, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, int width, int height);
		void fill(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);
        void fill(Color color);
		// Fills a part of pixel
		void __fill_pixel_part(std::shared_ptr<Bytes_Set> current_datas, unsigned int start_position, unsigned int length, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255) ;

        // Forces a pixel to change its value
        void force_pixel(unsigned short x, unsigned short y, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, int width_point);
        void force_pixel(unsigned short x, unsigned short y, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
        void force_pixel(unsigned short x, unsigned short y, Color color);

        // Returns the data filtered (like in PNG format)
        std::shared_ptr<Bytes_Set> datas_filtered();

        // Replaces a color with an other on the image
        void replace_color(Color old_color, Color new_color);

        // Paste datas to a specific pixel
        void paste_pixel_rgba_directly(unsigned int position, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, unsigned char multiplier);
        // Set datas about a specific pixel*
        void set_pixel_directly(unsigned int position, unsigned char value);
        void set_pixel_directly(unsigned int position, unsigned char red, unsigned char green, unsigned char blue, unsigned char multiplier);
        void set_pixel_rgba_directly(unsigned int position, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, unsigned char multiplier);
        void set_pixel(int x, int y, Color color, unsigned short width = 1);
        void set_pixel(int x, int y, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255, unsigned short width_point = 1);
        void set_pixel_alpha(unsigned short x, unsigned short y, unsigned char alpha);
        void set_pixel_blue(unsigned short x, unsigned short y, unsigned char blue, unsigned char alpha = 255);
        void set_pixel_by_number(unsigned int position, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);
        void set_pixel_by_number(unsigned int position, Color color);
        void set_pixel_green(unsigned short x, unsigned short y, unsigned char green, unsigned char alpha = 255);
        void set_pixel_red(unsigned short x, unsigned short y, unsigned char red, unsigned char alpha = 255);
        void set_pixel_rgba_directly_with_alpha(unsigned int position, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, unsigned char multiplier);

        // Get datas about a specific pixel
		Color pixel(unsigned short x, unsigned short y);
		Color pixel_by_number(unsigned int position);
        Color pixel_directly(unsigned int position, unsigned int multiplier);
        Color pixel_rgba_directly(unsigned int position, unsigned int multiplier);

		// Getters and setters
        unsigned int bit_depht() const;
        unsigned int buffer_size() const;
        unsigned int __buffer_size(int width, int height) const;
        unsigned int color_type() const;
        unsigned char components() const;
        Bytes_Set* datas() const;
        unsigned int flip_x_number() const;
        int height() const;
        void reset(int new_width, int new_height, Color new_color);
        void set_thread_number_for_filling(unsigned short new_thread_number);
        void set_thread_number_for_pasting(unsigned short new_thread_number);
        void set_thread_number_for_pasting_text(unsigned short new_thread_number);
        int width() const;

        //*********
        //
        // The Image class - Editing
        //
        //*********

        // Draw an arrow on the image
        void draw_arrow(int x_1, int y_1, int x_2, int y_2, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, double hat_position_in_percentage, double hat_size_in_percentage, unsigned short line_width);
        void draw_arrow(int x_1, int y_1, int x_2, int y_2, Color color, double hat_position_in_percentage, double hat_size_in_percentage, unsigned short width);
        void draw_arrow(int x_1, int y_1, int x_2, int y_2, Color color, double hat_percentage, unsigned short width);
        // Draws a border on the image
        void draw_border(int top, int left, int bottom, int right, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);
        void draw_border(int top, int left, int bottom, int right, scls::Color color);
        // Draw a line on the image
		void draw_line(int x_1, int y_1, int x_2, int y_2, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255, unsigned short line_width = 1);
        void draw_line(int x_1, int y_1, int x_2, int y_2, Color color, unsigned short width = 1);
        void draw_line(scls::Point_2D point_1, scls::Point_2D point_2, Color color, unsigned short width);
        // Draws a line gradient on the image
        void draw_line_gradient_force(int x_1, int y_1, int x_2, int y_2, unsigned short width, Color (*needed_function)(int, int, double));
        // Draw a rectangle on the image
        void draw_rect(int x, int y, int width, int height, unsigned int rect_width, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);
        void draw_rect(int x, int y, int width, int height, unsigned int rect_width, Color color);
        void draw_rect(int x, int y, int width, int height, unsigned int rect_width, Color color, Color fill_color);
        // Draw a circle on the image
        void draw_circle(int x_center, int y_center, double radius, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255, unsigned short line_width = 1);
        void draw_circle(int x_center, int y_center, double radius, Color color, unsigned short line_width = 1);

        // Drawing methods
        // Fills a circle on the image
        void fill_circle(int x_center, int y_center, double radius_x, double radius_y, double angle, double angle_start, double angle_end, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, double border_radius, unsigned char border_red, unsigned char border_green, unsigned char border_blue, unsigned char border_alpha);
        void fill_circle(int x_center, int y_center, double radius_x, double radius_y, double angle_start, double angle_end, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, double border_radius, unsigned char border_red, unsigned char border_green, unsigned char border_blue, unsigned char border_alpha);
        void fill_circle(int x_center, int y_center, double radius, double angle_start, double angle_end, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, double border_radius, unsigned char border_red, unsigned char border_green, unsigned char border_blue, unsigned char border_alpha);
        void fill_circle(int x_center, int y_center, double radius, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, double border_radius, unsigned char border_red, unsigned char border_green, unsigned char border_blue, unsigned char border_alpha);
        void fill_circle(int x_center, int y_center, double radius_x, double radius_y, double angle, double angle_start, double angle_end, Color color, double border_radius, Color border_color);
        void fill_circle(int x_center, int y_center, double radius_x, double radius_y, double angle_start, double angle_end, Color color, double border_radius, Color border_color);
        void fill_circle(int x_center, int y_center, double radius, double angle_start, double angle_end, Color color, double border_radius, Color border_color);
        void fill_circle(int x_center, int y_center, double radius, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
        void fill_circle(int x_center, int y_center, double radius_x, double radius_y, Color color, double border_radius, Color border_color);
        void fill_circle(int x_center, int y_center, double radius, Color color, double border_radius, Color border_color);
        void fill_circle(int x_center, int y_center, double radius, Color color);
        // Fills a circle with a gradient on the image
        void fill_circle_gradient(int x_center, int y_center, double radius, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255, Color (*needed_function)(double, int, int, int, unsigned char, unsigned char, unsigned char, unsigned char) = &fill_circle_gradient_linear);
        void fill_circle_gradient(int x_center, int y_center, double radius, Color color, Color (*needed_function)(double, int, int, int, unsigned char, unsigned char, unsigned char, unsigned char) = &fill_circle_gradient_linear);
        // Fills a rectangle on the image
		void fill_rect_force(int x, int y, unsigned short rect_width, unsigned short rect_height, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
		void fill_rect(int x, int y, unsigned short rect_width, unsigned short rect_height, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);
        void fill_rect(int x, int y, unsigned short width, unsigned short height, Color color);
        // Fills a rectangle on the image
		void fill_triangle(short x_1, short y_1, short x_2, short y_2, short x_3, short y_3, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);
        void fill_triangle(short x_1, short y_1, short x_2, short y_2, short x_3, short y_3, Color color);

        // Flips the image on the X axis
        void flip_x();
		// Flips the image on the Y axis
        void flip_y();

        // Points to draw a line
        std::vector<Point_2D> line_points(int x_1, int y_1, int x_2, int y_2);

		// Paste an __Image_Base on this __Image_Base
        void paste(__Image_Base* to_paste, int x, int y, double opacity = 1.0);
        void paste(std::string path, int x, int y, double opacity = 1.0);
        void paste(Image* to_paste, int x, int y, double opacity);
        void paste(Image to_paste, int x, int y, double opacity);
        void paste(Image* to_paste, int x, int y);
        void paste(Image to_paste, int x, int y);
		// Pastes an __Image_Base to the bottom / center / top of the image
		void paste_bottom_center(__Image_Base* to_paste, double offset_y, double opacity);
		void paste_center(__Image_Base* to_paste, double opacity = 1.0);
		void paste_top_center(__Image_Base* to_paste, double offset_y, double opacity);
        void paste_bottom_center(__Image_Base* to_paste, double offset_y);
        void paste_center(std::shared_ptr<__Image_Base> to_paste, double opacity = 1.0);
        void paste_top_center(__Image_Base* to_paste, double offset_y);
        // Paste a part of an image on this image
        void __paste_part_of_image(__Image_Base* to_paste, int x_offset, int y_offset, int start_x, int start_y, int length, double opacity);

        //*********
        //
        // __Image_Base gradients
        //
        //*********

        // Apply a more complex horizontal gradient from the left to the right in the image
        void apply_gradient_horizontal(Color left, Color right, int start_x, int end_x);
        void apply_gradient_horizontal(Color left, Color right);

        // Apply a more complex vertical gradient from the left to the right in the image
        void apply_gradient_vertical(Color top, Color bottom, int start_y, int end_y);
        void apply_gradient_vertical(Color top, Color bottom);

        //*********
        //
        // __Image_Base adaptative resize
        //
        //*********

        // Returns a shared ptr of the image with a new size, adaptated
        std::shared_ptr<__Image_Base> resize_adaptative(unsigned short new_width, unsigned short new_height);
        std::shared_ptr<__Image_Base> resize_adaptative(scls::Point_2D new_size);
        // Returns a shared ptr of the image with a new height, adaptated
        std::shared_ptr<__Image_Base> resize_adaptative_height(unsigned short new_height);
        // Returns a shared ptr of the image with a new width, adaptated
        std::shared_ptr<__Image_Base> resize_adaptative_width(unsigned short new_width, bool resize_height);
        std::shared_ptr<__Image_Base> resize_adaptative_width(unsigned short new_width);

        // Rotates the image
        void rotate(double angle);
        std::shared_ptr<__Image_Base> rotated(double angle);
        Point_2D rotated_size(double angle);

		// Load the image from a set of binary datas coming from a FreeType text
        bool _load_from_text_binary(char* datas, int width, int height, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
		// Load a part of image with a FreeType text in it
        bool __load_part_from_text_binary(char* datas, int offset, int start_x, int start_y, int length, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);

        //*********
        //
        // The __Image_Base class - Storing and loading
        //
        //*********

        // Returns the data of the image under the PNG format
        Bytes_Set* datas_png();

        // Get every chunks into a PNG image
		void _load_all_chunks_from_png_file(Bytes_Set* file, std::shared_ptr<__Image_Error>& error_handler);
		// Loads the bKGD chunk from a path and returns the color
		static Color _load_bKGD_from_file(Bytes_Set* file, _PNG_Chunk chunk);
        // Load a IDAT chunk grom a path
        void _load_png_IDAT_from_file_rgba(int component_size, int current_line_start_position, int last_line_start_position, int multiplier, int processed_data);
		void _load_png_IDAT_from_file(Bytes_Set* file, std::shared_ptr<__Image_Error>& error_handler);
        // Load the pHYS chunk from a path
		void _load_png_pHYS_from_file(Bytes_Set* file, _PNG_Chunk chunk, std::shared_ptr<__Image_Error>& error_handler);
        // Load the image from a PNG file
        void _load_png_file(Bytes_Set* file, std::shared_ptr<__Image_Error>& error_handler);
        // Load the sRGB chunk from a path
        void _load_png_sRGB_from_file(Bytes_Set* file, _PNG_Chunk chunk, std::shared_ptr<__Image_Error>& error_handler);

        // Loads the image from a path
		std::shared_ptr<__Image_Error> load_from_path(std::string path);
        // Save the image into the PNG format
        void save_png(std::string path);

		// Returns the signature of a PNG file
        std::vector<unsigned char> png_signature();

		// Getters and setters
        unsigned int compression_method() const;
        unsigned int filter_method() const;
        unsigned int interlace_method() const;
        unsigned int physical_height_ratio() const;
        unsigned int physical_unit() const;
        unsigned int physical_width_ratio() const;
    private:
	    // Base datas about the image
        // Bit depth of the image
		unsigned int a_bit_depth = 8;
		// Color type of the image
		unsigned int a_color_type = 6;
		// Number of flips by X
		unsigned int a_flip_x_number = 0;
		// Height of the image
		unsigned int a_height = 0;
		// Pixel of the image
		std::shared_ptr<Bytes_Set> a_pixels;
		// Width of the image
		unsigned int a_width = 0;

		// Datas necessary to use PNG
		// Compression method of the image
		unsigned int a_compression_method = 0;
		// Filter method of the image
		unsigned int a_filter_method = 0;
		// Filter type of the image
		unsigned char a_filter_type = 0;
		// Vector of all the IDAT chunks in the image
		std::vector<_PNG_Chunk> a_idat_chunk = std::vector<_PNG_Chunk>();
		// Interlace method of the image
		unsigned int a_interlace_method = 0;
		// Physical height of the image
		unsigned int a_physical_height_ratio = 10000;
		// Physical unit of the image (0 = unknow, 1 = meter)
		unsigned int a_physical_unit = 1;
		// Physical width of the image
		unsigned int a_physical_width_ratio = 10000;
		// Value of the sRGB chunk
		unsigned char a_srgb_value = 0;

		//*********
        //
        // Multi-threading handling
        //
        //*********

        // Number of thread created for a filling
        unsigned short a_thread_number_for_filling = 0;
        // Number of thread created for a pasting
        unsigned short a_thread_number_for_pasting = 0;
        // Number of thread created for a pasting a text
        unsigned short a_thread_number_for_pasting_text = 0;
	};
	typedef __Image_Base::Image Image;

	// Points in a circle
    std::vector<scls::Point_2D> circle_points(int x, int y, int radius, int border_radius, int width, int height);
    std::vector<scls::Point_2D> circle_points(int x, int y, int radius, int width, int height);
    // Points in a line
    std::vector<scls::Point_2D> line_points(int x_1, int y_1, int x_2, int y_2, unsigned short line_width);
}

#endif // SCLS_IMAGE_CORE
