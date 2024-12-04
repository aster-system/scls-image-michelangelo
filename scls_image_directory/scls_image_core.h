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
#define SCLS_ZLIB_PATH <zlib/zlib.h>
#endif // SCLS_ZLIB_PATH
// Freetype
#ifndef SCLS_FREETYPE_PATH
#define SCLS_FREETYPE_PATH <ft2build.h>
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
namespace scls
{
    //*********
	//
	// Color handling
	//
	//*********

	class Color {
	    // Class representing an 8 bit RGBA color
    public:
        // Basic Color constructor
        Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);
        Color():Color(0,0,0,255){}
        // Color copy constructor
        Color(const Color& color_copy):a_alpha(color_copy.a_alpha),a_blue(color_copy.a_blue),a_green(color_copy.a_green),a_red(color_copy.a_red){};

        // Return a color loaded from a text
        static Color from_std_string(std::string source);

        // Getters and setters (ONLY WITH ATTRIBUTES)
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
        inline bool operator==(const Color& color) {return color.a_red == a_red && color.a_green == a_green && color.a_blue == a_blue && color.a_alpha == a_alpha;}
    private:
        // Descriptor of the color (between 0 and 1)
        double a_alpha = 1;
        double a_blue = 0;
        double a_green = 0;
        double a_red = 0;
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

	class Image {
		// Class representing a PNG image handler
	public:

	    //*********
        //
        // The Image class - Base
        //
        //*********

	    // All Image constructors
		// Image most basic constructor
		Image() {};
		// Image constructor with a path
		Image(std::string path) : Image() {std::shared_ptr<__Image_Error> final_error = load_from_path(path);if(final_error.get()->has_error()) print("Warning", "SCLS Image \"Michelangelo\"", final_error.get()->to_std_string());};
		// Image constructor from scratch
		Image(unsigned short width, unsigned short height, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255, unsigned int color_type = SCLS_IMAGE_RGBA) {a_color_type = color_type;a_height = height;a_width = width;fill(red, green, blue, alpha);};
		Image(unsigned short width, unsigned short height, Color color, unsigned int color_type = SCLS_IMAGE_RGBA) : Image(width, height, color.red(), color.green(), color.blue(), color.alpha(), color_type) {}
		// Image copy constructor
		Image(Image& image_copy) : Image(image_copy.width(), image_copy.height(), Color(0, 0, 0, 0)) {paste(&image_copy, 0, 0);}
		Image(Image* image_copy) : Image(image_copy->width(), image_copy->height(), Color(0, 0, 0, 0)) {paste(image_copy, 0, 0);}
        // PNG_Image destructor
		~Image() { free_memory(); };

		// Returns if the image use alpha or not
		inline bool use_alpha() const {return color_type() == SCLS_IMAGE_RGBA;};

		// Basic image manipulation
		// Delete the pixels in the memory
		inline void free_memory() {a_pixels.reset();};
		// Fills the image with one color
		void fill(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);
		inline void fill(Color color) { fill(color.red(), color.green(), color.blue(), color.alpha()); };
		// Fills a part of pixel
		void __fill_pixel_part(unsigned int start_position, unsigned int length, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255) ;

        // Forces a pixel to change its value
        void force_pixel(unsigned short x, unsigned short y, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);
        inline void force_pixel(unsigned short x, unsigned short y, Color color) {force_pixel(x, y, color.red(), color.green(), color.blue(), color.alpha());};

        // Returns the data filtered (like in PNG format)
        std::shared_ptr<Bytes_Set> datas_filtered();
        // Loads the image from a path
		std::shared_ptr<__Image_Error> load_from_path(std::string path);

		// Get datas about a specific pixel
		Color pixel(unsigned short x, unsigned short y);
		Color pixel_by_number(unsigned int position);
		inline Color pixel_rgba_directly(unsigned int position, unsigned int multiplier) {Color to_return = Color(255, 255, 255);to_return.set_rgba(a_pixels->data_at_directly(position), a_pixels->data_at_directly(position + multiplier), a_pixels->data_at_directly(position + 2 * multiplier), a_pixels->data_at_directly(position + 3 * multiplier));return to_return;};

		// Set datas about a specific pixel*
		inline void set_pixel_directly(unsigned int position, unsigned char value){a_pixels->set_data_at_directly(position, value);};
		inline void set_pixel_rgba_directly(unsigned int position, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, unsigned char multiplier){a_pixels->set_data_at_directly(position, red);a_pixels->set_data_at_directly(position + multiplier, green);a_pixels->set_data_at_directly(position + 2 * multiplier, blue);a_pixels->set_data_at_directly(position + 3 * multiplier,  alpha);};
        inline void set_pixel(int x, int y, Color color, unsigned short width = 1) { set_pixel(x, y, color.red(), color.green(), color.blue(), color.alpha(), width); }
        void set_pixel(int x, int y, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255, unsigned short width_point = 1);
        void set_pixel_alpha(unsigned short x, unsigned short y, unsigned char alpha);
        void set_pixel_blue(unsigned short x, unsigned short y, unsigned char blue, unsigned char alpha = 255);
        void set_pixel_by_number(unsigned int position, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);
        inline void set_pixel_by_number(unsigned int position, Color color) { set_pixel_by_number(position, color.red(), color.green(), color.blue());};
        void set_pixel_green(unsigned short x, unsigned short y, unsigned char green, unsigned char alpha = 255);
        void set_pixel_red(unsigned short x, unsigned short y, unsigned char red, unsigned char alpha = 255);
        void set_pixel_rgba_directly_with_alpha(unsigned int position, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, unsigned char multiplier);

		// Getters and setters
		// For the methods taking position : If the position is out of the image, prints an error.
		inline unsigned int buffer_size() const {return height() * width() * components() * static_cast<unsigned int>(static_cast<double>(bit_depht()) / 8.0);};
		inline unsigned char components() const { if (color_type() == 6) return 4; return 3; };

		// Getters and setters (ONLY WITH ATTRIBUTES)
		inline unsigned int bit_depht() const { return a_bit_depth; };
		inline unsigned int color_type() const { return a_color_type; };
		inline Bytes_Set* datas() const { return a_pixels.get(); }
		inline unsigned int flip_x_number() const {return a_flip_x_number;};
		inline int height() const { return a_height; };
		inline void set_thread_number_for_filling(unsigned short new_thread_number) {a_thread_number_for_filling = new_thread_number;};
        inline void set_thread_number_for_pasting(unsigned short new_thread_number) {a_thread_number_for_pasting = new_thread_number;};
        inline void set_thread_number_for_pasting_text(unsigned short new_thread_number) {a_thread_number_for_pasting_text = new_thread_number;};
		inline int width() const { return a_width; };

		//*********
        //
        // The Image class - PNG
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
        // Load the Image from a PNG file
        void _load_png_file(Bytes_Set* file, std::shared_ptr<__Image_Error>& error_handler);
        // Load the sRGB chunk from a path
        void _load_png_sRGB_from_file(Bytes_Set* file, _PNG_Chunk chunk, std::shared_ptr<__Image_Error>& error_handler);

        // Save the image into the PNG format
		inline void save_png(std::string path) {Bytes_Set* datas = datas_png();datas->save(path);delete datas; datas = 0;}

		// Returns the signature of a PNG file
        std::vector<unsigned char> png_signature();

		// Getters and setters (ONLY WITH ATTRIBUTES)
		inline unsigned int compression_method() { return a_compression_method; };
		inline unsigned int filter_method() { return a_filter_method; };
		inline unsigned int interlace_method() { return a_interlace_method; };
		inline unsigned int physical_height_ratio() { return a_physical_height_ratio; };
		inline unsigned int physical_unit() { return a_physical_unit; };
		inline unsigned int physical_width_ratio() { return a_physical_width_ratio; };

        //*********
        //
        // The Image class - Editing
        //
        //*********

        // Drawing methods
        // Draw a circle on the image
        void draw_circle(int x_center, int y_center, double radius, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255, unsigned short line_width = 1);
        inline void draw_circle(int x_center, int y_center, double radius, Color color, unsigned short line_width = 1){draw_circle(x_center,y_center,radius,color.red(),color.green(),color.blue(),color.alpha(),line_width);};
        // Fill a circle on the image
        void fill_circle(int x_center, int y_center, double radius, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);
        inline void fill_circle(int x_center, int y_center, double radius, Color color){fill_circle(x_center,y_center,radius,color.red(),color.green(),color.blue(),color.alpha());};
        // Fill a circle with a gradient on the image
        void fill_circle_gradient(int x_center, int y_center, double radius, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255, Color (*needed_function)(double, int, int, int, unsigned char, unsigned char, unsigned char, unsigned char) = &fill_circle_gradient_linear);
        inline void fill_circle_gradient(int x_center, int y_center, double radius, Color color, Color (*needed_function)(double, int, int, int, unsigned char, unsigned char, unsigned char, unsigned char) = &fill_circle_gradient_linear){fill_circle_gradient(x_center,y_center,radius,color.red(),color.green(),color.blue(),color.alpha(),needed_function);};

        // Draw a line on the image
		void draw_line(int x_1, int y_1, int x_2, int y_2, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255, unsigned short line_width = 1);
        inline void draw_line(int x_1, int y_1, int x_2, int y_2, Color color, unsigned short width = 1) {draw_line(x_1, y_1, x_2, y_2, color.red(), color.green(), color.blue(), color.alpha(), width);};

        // Draw a rectangle on the imageE
        void draw_rect(unsigned short x, unsigned short y, unsigned short width, unsigned short height, unsigned int rect_width, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);
        inline void draw_rect(unsigned short x, unsigned short y, unsigned short width, unsigned short height, unsigned int rect_width, Color color) {draw_rect(x, y, width, height, rect_width, color.red(), color.green(), color.blue(), color.alpha());};
        inline void draw_rect(unsigned short x, unsigned short y, unsigned short width, unsigned short height, unsigned int rect_width, Color color, Color fill_color) {draw_rect(x, y, width, height, rect_width, color.red(), color.green(), color.blue(), color.alpha());fill_rect(x + rect_width, y + rect_width, width - rect_width * 2, height - rect_width * 2, fill_color.red(), fill_color.green(), fill_color.blue(), fill_color.alpha());};
        // Fill a rectangle on the image
		void fill_rect(int x, int y, unsigned short rect_width, unsigned short rect_height, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);
		inline void fill_rect(int x, int y, unsigned short width, unsigned short height, Color color) {fill_rect(x, y, width, height, color.red(), color.green(), color.blue(), color.alpha());};

        // Fill a rectangle on the image
		void fill_triangle(short x_1, short y_1, short x_2, short y_2, short x_3, short y_3, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);
        inline void fill_triangle(short x_1, short y_1, short x_2, short y_2, short x_3, short y_3, Color color) {fill_triangle(x_1, y_1, x_2, y_2, x_3, y_3, color.red(), color.green(), color.blue(), color.alpha());};

        // Flip the image on the X axis
        void flip_x();
		// Flip the image on the Y axis
        void flip_y();

		// Paste an Image on this Image
        void paste(Image* to_paste, int x, int y, double opacity = 1.0);
		inline void paste(std::string path, int x, int y, double opacity = 1.0) {Image* img = new Image(path);paste(img, x, y, opacity);delete img; img = 0;};
        // Paste a part of an image on this image
        void __paste_part_of_image(Image* to_paste, int x_offset, int y_offset, int start_x, int start_y, int length, double opacity);

        //*********
        //
        // Image gradients
        //
        //*********

        // Apply a simple circle gradient in the image
        void apply_gradient_circle(Color in, Color out, int x, int y, unsigned short radius);

        // Apply a more complex horizontal gradient from the left to the right in the image
        void apply_gradient_horizontal(Color left, Color right, int start_x, int end_x);
        inline void apply_gradient_horizontal(Color left, Color right) { apply_gradient_horizontal(left, right, 0, width()); };

        // Apply a more complex vertical gradient from the left to the right in the image
        void apply_gradient_vertical(Color top, Color bottom, int start_y, int end_y);
        inline void apply_gradient_vertical(Color top, Color bottom) { apply_gradient_vertical(top, bottom, 0, height()); };

        //*********
        //
        // Image adaptative resize
        //
        //*********

        // Returns a shared ptr of the image with a new size, adaptated
        inline std::shared_ptr<Image> resize_adaptative(unsigned short new_width, unsigned short new_height) {std::shared_ptr<Image> current_image = resize_adaptative_width(new_width);if(current_image.get() == 0) return resize_adaptative_height(new_height);return current_image.get()->resize_adaptative_height(new_height); };
        // Returns a shared ptr of the image with a new height, adaptated
        std::shared_ptr<Image> resize_adaptative_height(unsigned short new_height);
        // Returns a shared ptr of the image with a new width, adaptated
        std::shared_ptr<Image> resize_adaptative_width(unsigned short new_width);


		// Load the image from a set of binary datas coming from a FreeType text
        bool _load_from_text_binary(char* datas, int width, int height, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
		// Load a part of image with a FreeType text in it
        bool __load_part_from_text_binary(char* datas, int offset, int start_x, int start_y, int length, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
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
}

#endif // SCLS_IMAGE_CORE
