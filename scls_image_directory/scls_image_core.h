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

#ifndef SCLS_IMAGE_CORE
#define SCLS_IMAGE_CORE

#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>
#include <math.h>
#include <scls_foundation.h>
#include <zlib/zlib.h>

// Base path to the fonts in the system
#ifdef __WIN32__ // With Windows
#define BASE_FONT_PATH std::string("C:\\Windows\\Fonts\\")
#define DEFAULT_FONT std::string("arial")
#endif
#ifdef __linux__ // With Linux
#define BASE_FONT_PATH std::string("/usr/share/fonts/truetype/")
#define DEFAULT_FONT std::string("FreeMono")
#endif

// ZLib mandatory stuff
#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) (void)_setmode(_fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif

#define PNG_CRC_POLY 0x04C11DB7

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
        Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255) {
            a_alpha = static_cast<double>(alpha) / 255.0;
            a_blue = static_cast<double>(blue) / 255.0;
            a_green = static_cast<double>(green) / 255.0;
            a_red = static_cast<double>(red) / 255.0;
        };

        // Getters and setters (ONLY WITH ATTRIBUTES)
        inline unsigned char alpha() {return static_cast<unsigned char>(a_alpha * 255.0);};
        inline unsigned char blue() {return static_cast<unsigned char>(a_blue * 255.0);};
        inline unsigned char green() {return static_cast<unsigned char>(a_green * 255.0);};
        inline unsigned char red() {return static_cast<unsigned char>(a_red * 255.0);};
        inline void set_alpha(unsigned char new_alpha) {a_alpha = static_cast<double>(new_alpha) / 255.0;};
        inline void set_blue(unsigned char new_blue) {a_blue = static_cast<double>(new_blue) / 255.0;};
        inline void set_green(unsigned char new_green) {a_green = static_cast<double>(new_green) / 255.0;};
        inline void set_red(unsigned char new_red) {a_red = static_cast<double>(new_red) / 255.0;};
        inline void set_rgb(unsigned char new_red, unsigned char new_green, unsigned char new_blue) { set_red(new_red); set_green(new_green); set_blue(new_blue); };
        inline void set_rgba(unsigned char new_red, unsigned char new_green, unsigned char new_blue, unsigned char new_alpha) { set_rgb(new_red, new_green, new_blue); set_alpha(new_alpha); };
    private:
        // Descriptor of the color (between 0 and 1)
        double a_alpha = 1;
        double a_blue = 0;
        double a_green = 0;
        double a_red = 0;
	};

	// Predefined colors
	static Color black = Color(0, 0, 0);
	static Color blue = Color(0, 0, 255);
	static Color brown = Color(102, 51, 0);
	static Color green = Color(0, 255, 0);
	static Color grey = Color(128, 128, 128);
	static Color orange = Color(255, 128, 0);
	static Color pink = Color(255, 51, 255);
	static Color purple = Color(77, 50, 146);
	static Color red = Color(255, 0, 0);
	static Color yellow = Color(255, 255, 0);
	static Color white = Color(255, 255, 255);

	//*********
	//
	// Compression handling
	//
	//*********

	// Compress data from a char array without returning the result
	inline int _compress_binary(char* to_compress, unsigned int to_compress_size, char* output, unsigned int output_size, unsigned int& total_output_size, unsigned int compression_level = 9) {
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
	};

	// Compress data from a char array and return the result
	inline char* compress_binary(char* to_compress, unsigned int to_compress_size, unsigned int& output_size, unsigned int compression_level = 9) {
		char* output = new char[to_compress_size * 2];

		unsigned int ret = _compress_binary(to_compress, to_compress_size, output, to_compress_size + 1000, output_size, compression_level);
		if (ret != 1) return 0;

		char* final_array = new char[output_size];
		for (unsigned int i = 0; i < output_size; i++) final_array[i] = output[i];

		delete[] output;

		return final_array;
	}

	// Uncompress data from a char array
	inline int uncompress_binary(char* to_uncompress, unsigned int to_uncompress_size, char* output, unsigned int output_size) {
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
		do
		{
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
	};

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
		// Image constructor from scratch, easier to use
		Image(unsigned short width, unsigned short height, Color color, unsigned int color_type = 6) : Image(width, height, color.red(), color.green(), color.blue(), color.alpha(), color_type) {

		}
		// Image constructor from scratch
		Image(unsigned short width, unsigned short height, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255, unsigned int color_type = 6) {
			a_color_type = color_type;
			a_height = height;
			a_width = width;

			fill(red, green, blue, alpha);
		};
        // PNG_Image destructor
		~Image() { free_memory(); };

		// Basic image manipulation
		// Delete the pixels in the memory
		void free_memory() {
			if (a_pixels != 0)
			{
				delete[] a_pixels; a_pixels = 0;
			}
		};
		// Fill the image with one color
		void fill(Color color) { fill(color.red(), color.green(), color.blue(), color.alpha()); };
		// Fill the image with one color
		void fill(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255) {
			free_memory();
			unsigned int position = 0; unsigned int total_size = height() * width() * components() * (bit_depht() / 8.0);
			a_pixels = new Bytes_Set(total_size);
			if(color_type() == 6) {
                for (unsigned int i = 0; i < height(); i++)
                {
                    for (unsigned int j = 0; j < width(); j++)
                    {
                        a_pixels->set_data_at(position, red); position++;
                        a_pixels->set_data_at(position, green); position++;
                        a_pixels->set_data_at(position, blue); position++;
                        a_pixels->set_data_at(position, alpha); position++;
                    }
                }
            }
            else {
               for (unsigned int i = 0; i < height(); i++)
                {
                    for (unsigned int j = 0; j < width(); j++)
                    {
                        a_pixels->set_data_at(position, red); position++;
                        a_pixels->set_data_at(position, green); position++;
                        a_pixels->set_data_at(position, blue); position++;
                    }
                }
            }
		};
        // Force a pixel to change its value
        inline void force_pixel(unsigned short x, unsigned short y, Color color) {
            force_pixel(x, y, color.red(), color.green(), color.blue(), color.alpha());
        };
        // Force a pixel to change its value
		inline void force_pixel(unsigned short x, unsigned short y, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255) {
            if (x < 0 || y < 0 || x >= width() || y >= height()) {
                print("Warning", "SCLS Image", "The position (" + std::to_string(x) + "; " + std::to_string(y) + ") you want to force is out of the image.");
                return;
            }

            unsigned int multiplier = (bit_depht() / 8.0);
			unsigned int position = (y * width() + x) * components() * multiplier;
            a_pixels->set_data_at(position, red);
            a_pixels->set_data_at(position + multiplier, green);
            a_pixels->set_data_at(position + 2 * multiplier, blue);

            if(color_type() == 6) a_pixels->set_data_at(position + 3 * multiplier, alpha);
		};

		// Getters and setters (ONLY WITHOUT ATTRIBUTES)
		inline unsigned char components() { if (color_type() == 6) return 4; return 3; };
		inline Bytes_Set* datas_filtered() {
		    Bytes_Set* to_return = new Bytes_Set(datas()->datas_size() + height());
		    unsigned int position = 0;
			for (unsigned int i = 0; i < height(); i++)
			{
			    to_return->set_data_at(position, 0); position++;
			    if((bit_depht() / 8.0) == 1) {
                    for (unsigned int j = 0; j < width() * components(); j++) {
                        to_return->set_data_at(position, datas()->data_at(i * width() * components() + j)); position++;
                    }
                }
                else {
                    for (unsigned int j = 0; j < width() * components(); j++) {
                        to_return->set_data_at(position, datas()->data_at(i * width() * components() + j)); position++;
                        // to_return->add_ushort(datas()->data_at((i * width() * components() + j) * 2), true);
                    }
                }
			}
			return to_return;
		};
		Color pixel(unsigned short x, unsigned short y) {
			Color to_return(0, 0, 0);
			if (x >= 0 && x < width() && y >= 0 && y < height()) {
			    unsigned char multiplier = (bit_depht() / 8.0);
				unsigned int position = (y * width() + x) * components() * (bit_depht() / 8.0);

				if(color_type() == 6)
                {
                    to_return.set_rgba(a_pixels->data_at(position), a_pixels->data_at(position + multiplier), a_pixels->data_at(position + 2 * multiplier), a_pixels->data_at(position + 3 * multiplier));
                }
                else
                {
                    to_return.set_rgb(a_pixels->data_at(position), a_pixels->data_at(position + multiplier), a_pixels->data_at(position + 2 * multiplier));
                }
			}
			else {
                print("Warning", "SCLS Image", "The position (" + std::to_string(x) + "; " + std::to_string(y) + ") you want to get is out of the image.");
			}
			return to_return;
		}
		inline std::vector<unsigned char> png_signature() {
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
		};
        inline void set_pixel(unsigned short x, unsigned short y, Color color, unsigned short width = 1) {
			set_pixel(x, y, color.red(), color.green(), color.blue(), color.alpha(), width);
		}
		inline void set_pixel(unsigned short x, unsigned short y, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255, unsigned short width_point = 1) {
			if (x < 0 || y < 0 || x >= width() || y >= height()) {
                print("Warning", "SCLS Image", "The position (" + std::to_string(x) + "; " + std::to_string(y) + ") you want to set is out of the image.");
                return;
			}

			if (width_point == 0) return;
			else if (width_point == 1) {

			    unsigned char multiplier = (bit_depht() / 8.0);
			    unsigned int position = (y * width() + x) * components() * (bit_depht() / 8.0);
			    if(color_type() == 6)
                {
                    Color color = pixel(x, y);

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
                else
                {
                    a_pixels->set_data_at(position, red);
                    a_pixels->set_data_at(position + multiplier, green);
                    a_pixels->set_data_at(position + 2 * multiplier, blue);
                }
			}
			else
			{
				draw_rect(static_cast<unsigned short>(x - static_cast<float>(width_point) / 2.0), static_cast<unsigned short>(y - (static_cast<float>(width_point)) / 2.0), width_point, width_point, red, green, blue, alpha);
			}
		}
        inline void set_pixel_alpha(unsigned short x, unsigned short y, unsigned char alpha) {
            if (x < 0 || y < 0 || x >= width() || y >= height()) {
                print("Warning", "SCLS Image", "The position (" + std::to_string(x) + "; " + std::to_string(y) + ") you want to set the alpha is out of the image.");
                return;
			}

			unsigned char multiplier = (bit_depht() / 8.0);
            unsigned int position = (y * width() + x) * components() * (bit_depht() / 8.0);
            if(color_type() == 6)
            {
                Color color = pixel(x, y);

                float alpha_f = normalize_value(alpha, 0, 255) / 255.0;

                // Calculate alpha
                alpha = normalize_value(alpha, 0, 255); if(color.alpha() > alpha) alpha = color.alpha();

                a_pixels->set_data_at(position + 3 * multiplier,  alpha);
            }
        }
        inline void set_pixel_blue(unsigned short x, unsigned short y, unsigned char blue, unsigned char alpha = 255) {
            if (x < 0 || y < 0 || x >= width() || y >= height()) {
                print("Warning", "SCLS Image", "The position (" + std::to_string(x) + "; " + std::to_string(y) + ") you want to set the blue is out of the image.");
                return;
			}

			unsigned char multiplier = (bit_depht() / 8.0);
            unsigned int position = (y * width() + x) * components() * (bit_depht() / 8.0);
            if(color_type() == 6)
            {
                Color color = pixel(x, y);

                float alpha_f = normalize_value(alpha, 0, 255) / 255.0;
                float blue_f = normalize_value(blue, 0, 255);

                // Calculate alpha
                alpha = normalize_value(alpha, 0, 255); if(color.alpha() > alpha) alpha = color.alpha();
                blue = alpha_f * blue_f + (1.0 - alpha_f) * static_cast<float>(color.blue());

                a_pixels->set_data_at(position + 2 * multiplier, blue);
                a_pixels->set_data_at(position + 3 * multiplier, alpha);
            }
            else
            {
                a_pixels->set_data_at(position + 2 * multiplier, blue);
			}
        }
        inline void set_pixel_green(unsigned short x, unsigned short y, unsigned char green, unsigned char alpha = 255) {
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
            else
            {
                a_pixels->set_data_at(position + multiplier, green);
			}
        }
        inline void set_pixel_red(unsigned short x, unsigned short y, unsigned char red, unsigned char alpha = 255) {
            if (x < 0 || y < 0 || x >= width() || y >= height()) {
                print("Warning", "SCLS Image", "The position (" + std::to_string(x) + "; " + std::to_string(y) + ") you want to set the red is out of the image.");
                return;
			}

			unsigned char multiplier = (bit_depht() / 8.0);
            unsigned int position = (y * width() + x) * components() * (bit_depht() / 8.0);
            if(color_type() == 6)
            {
                Color color = pixel(x, y);

                float alpha_f = normalize_value(alpha, 0, 255) / 255.0;
                float red_f = normalize_value(red, 0, 255);

                // Calculate alpha
                alpha = normalize_value(alpha, 0, 255); if(color.alpha() > alpha) alpha = color.alpha();
                red = alpha_f * red_f + (1.0 - alpha_f) * static_cast<float>(color.red());

                a_pixels->set_data_at(position, red);
                a_pixels->set_data_at(position + 3 * multiplier,  alpha);
            }
            else
            {
                a_pixels->set_data_at(position, red);
			}
        }

		// Getters and setters (ONLY WITH ATTRIBUTES)
		inline unsigned int bit_depht() { return a_bit_depth; };
		inline unsigned int color_type() { return a_color_type; };
		inline Bytes_Set* datas() { return a_pixels; }
		inline unsigned int height() { return a_height; };
		inline unsigned int width() { return a_width; };

		//*********
        //
        // The Image class - PNG
        //
        //*********

        // Image constructor with a path
		Image(std::string path) : Image() {
		    load_from_path(path);
		};
        // Returns the data of the image under the PNG format
		inline Bytes_Set* data_png() {
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

			// Creathe the pHYS chunk
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
			Bytes_Set* idat_uncompressed = datas_filtered();
			char* idat_compressed = compress_binary(idat_uncompressed->datas(), idat_uncompressed->datas_size(), idat_size, 9);
			delete idat_uncompressed;
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
        // Save the image into the PNG format
		inline void save_png(std::string path) {
			Bytes_Set* datas = data_png();
			datas->save(path);
			delete datas; datas = 0;
		}

		// Getters and setters (ONLY WITH ATTRIBUTES)
		inline unsigned int compression_method() { return a_compression_method; };
		inline unsigned int filter_method() { return a_filter_method; };
		inline unsigned int interlace_method() { return a_interlace_method; };
		inline bool is_loadable() { return a_loadable; };
		inline unsigned int physical_height_ratio() { return a_physical_height_ratio; };
		inline unsigned int physical_unit() { return a_physical_unit; };
		inline unsigned int physical_width_ratio() { return a_physical_width_ratio; };





		// Draw a line on the image
		void draw_line(unsigned short x_1, unsigned short y_1, unsigned short x_2, unsigned short y_2, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255, unsigned short width = 1) {
			if (width == 1 || true)
			{
				float distance_x = static_cast<float>(x_2 - x_1);
				float distance_y = static_cast<float>(y_2 - y_1);

				float x_y_ratio = distance_x / distance_y;

				if (abs(x_y_ratio) < 1)
				{
					// Normalize positions
					if (y_1 > y_2)
					{
						unsigned short temp = y_1;
						y_1 = y_2;
						y_2 = temp;
						temp = x_1;
						x_1 = x_2;
						x_2 = temp;
					}

					float actual_x = x_1;
					float actual_y = y_1;

					while (actual_y < y_2)
					{
						actual_y++;
						actual_x += x_y_ratio;
						set_pixel(static_cast<unsigned short>(actual_x), static_cast<unsigned short>(actual_y), red, green, blue, alpha, width);
					}
				}
				else
				{
					// Normalize positions
					if (x_1 > x_2)
					{
						unsigned short temp = x_1;
						x_1 = x_2;
						x_2 = temp;
						temp = y_1;
						y_1 = y_2;
						y_2 = temp;
					}

					float actual_x = x_1;
					float actual_y = y_1;

					float y_x_ratio = distance_y / distance_x;
					while (actual_x < x_2)
					{
						actual_y += y_x_ratio;
						actual_x++;
						set_pixel(static_cast<unsigned short>(actual_x), static_cast<unsigned short>(actual_y), red, green, blue, alpha, width);
					}
				}
			}
		};
		// Draw a rectangle on the image
		void draw_rect(unsigned short x, unsigned short y, unsigned short width, unsigned short height, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255) {
			for (int i = 0; i < width; i++)
			{
				for (int j = 0; j < height; j++)
				{
					set_pixel(x + i, y + j, red, green, blue, alpha);
				}
			}
		};
		// Fill a rectangle on the image
		void fill_triangle(unsigned short x_1, unsigned short y_1, unsigned short x_2, unsigned short y_2, unsigned short x_3, unsigned short y_3, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255) {
			// 3 should be the point with the largest X value
			if (x_1 > x_3)
			{
				unsigned short temp = x_1;
				x_1 = x_3;
				x_3 = temp;
				temp = y_1;
				y_1 = y_3;
				y_3 = temp;
			}

			// 3 should be the point with the largest X value
			if (x_2 > x_3)
			{
				unsigned short temp = x_2;
				x_2 = x_3;
				x_3 = temp;
				temp = y_2;
				y_2 = y_3;
				y_3 = temp;
			}

			// 2 should be the point with the largest Y value of 1 and 2
			if (y_1 > y_2)
			{
				unsigned short temp = x_2;
				x_2 = x_1;
				x_1 = temp;
				temp = y_2;
				y_2 = y_1;
				y_1 = temp;
			}

			float distance_x_1_2 = static_cast<float>(x_2 - x_1);
			float distance_y_1_2 = static_cast<float>(y_2 - y_1);

			float actual_x = x_1;
			float actual_y = y_1;
			float ratio_x_y = distance_x_1_2 / distance_y_1_2;
			float ratio_y_x = abs(distance_y_1_2 / distance_x_1_2);

			unsigned short iter = 0;

			while (actual_y < y_2 && iter < 1000)
			{
				if (abs(ratio_x_y) < 1)
				{
					actual_y++;
					actual_x += ratio_x_y;
				}
				else
				{
					actual_y += ratio_y_x;
					if (distance_x_1_2 > 0) actual_x++;
					else actual_x--;
				}

				draw_line(static_cast<unsigned short>(floor(actual_x)), static_cast<unsigned short>(floor(actual_y)), x_3, y_3, red, green, blue, alpha, 2);
				draw_line(static_cast<unsigned short>(floor(actual_x)), static_cast<unsigned short>(ceil(actual_y)), x_3, y_3, red, green, blue, alpha, 2);
				draw_line(static_cast<unsigned short>(ceil(actual_x)), static_cast<unsigned short>(floor(actual_y)), x_3, y_3, red, green, blue, alpha, 2);
				draw_line(static_cast<unsigned short>(ceil(actual_x)), static_cast<unsigned short>(ceil(actual_y)), x_3, y_3, red, green, blue, alpha, 2);
				iter++;
			}
		};
		// Flip the image on the X axis
		inline void flip_x() {
			unsigned char* line1 = new unsigned char[width()];
			unsigned int max = height();

			for (int i = 0; i < floor(static_cast<float>(max) / 2.0); i++)
			{
				// Red
				for (unsigned int j = 0; j < width(); j++) line1[j] = a_pixels->data_at((i * width() + j) * components());
				for (unsigned int j = 0; j < width(); j++) a_pixels->set_data_at((i * width() + j) * components(), a_pixels->data_at(((max - (i + 1)) * width() + j) * components()));
				for (unsigned int j = 0; j < width(); j++) a_pixels->set_data_at(((max - (i + 1)) * width() + j) * components(), line1[j]);

				// Green
				for (unsigned int j = 0; j < width(); j++) line1[j] = a_pixels->data_at((i * width() + j) * components() + 1);
				for (unsigned int j = 0; j < width(); j++) a_pixels->set_data_at((i * width() + j) * components() + 1, a_pixels->data_at(((max - (i + 1)) * width() + j) * components() + 1));
				for (unsigned int j = 0; j < width(); j++) a_pixels->set_data_at(((max - (i + 1)) * width() + j) * components() + 1, line1[j]);

				// Blue
				for (unsigned int j = 0; j < width(); j++) line1[j] = a_pixels->data_at((i * width() + j) * components() + 2);
				for (unsigned int j = 0; j < width(); j++) a_pixels->set_data_at((i * width() + j) * components() + 2, a_pixels->data_at(((max - (i + 1)) * width() + j) * components() + 2));
				for (unsigned int j = 0; j < width(); j++) a_pixels->set_data_at(((max - (i + 1)) * width() + j) * components() + 2, line1[j]);

				// Alpha
				for (unsigned int j = 0; j < width(); j++) line1[j] = a_pixels->data_at((i * width() + j) * components() + 3);
				for (unsigned int j = 0; j < width(); j++) a_pixels->set_data_at((i * width() + j) * components() + 3, a_pixels->data_at(((max - (i + 1)) * width() + j) * components() + 3));
				for (unsigned int j = 0; j < width(); j++) a_pixels->set_data_at(((max - (i + 1)) * width() + j) * components() + 3, line1[j]);
			}

			delete[] line1;
		};
		// Flip the image on the Y axis
		inline void flip_y() {
			unsigned char* line1 = new unsigned char[height()];
			unsigned int max = width();

			for (int i = 0; i < floor(static_cast<float>(max) / 2.0); i++)
			{
				// Red
				for (unsigned int j = 0; j < height(); j++) line1[j] = a_pixels->data_at((i + j * width()) * components());
				for (unsigned int j = 0; j < height(); j++) a_pixels->set_data_at((i + j * width()) * components(), a_pixels->data_at(((max - (i + 1)) + j * width()) * components()));
				for (unsigned int j = 0; j < height(); j++) a_pixels->set_data_at(((max - (i + 1)) + j * width()) * components(), line1[j]);

				// Green
				for (unsigned int j = 0; j < height(); j++) line1[j] = a_pixels->data_at((i + j * width()) * components() + 1);
				for (unsigned int j = 0; j < height(); j++) a_pixels->set_data_at((i + j * width()) * components() + 1, a_pixels->data_at(((max - (i + 1)) + j * width()) * components() + 1));
				for (unsigned int j = 0; j < height(); j++) a_pixels->set_data_at(((max - (i + 1)) + j * width()) * components() + 1, line1[j]);

				// Blue
				for (unsigned int j = 0; j < height(); j++) line1[j] = a_pixels->data_at((i + j * width()) * components() + 2);
				for (unsigned int j = 0; j < height(); j++) a_pixels->set_data_at((i + j * width()) * components() + 2, a_pixels->data_at(((max - (i + 1)) + j * width()) * components() + 2));
				for (unsigned int j = 0; j < height(); j++) a_pixels->set_data_at(((max - (i + 1)) + j * width()) * components() + 2, line1[j]);

				// Alpha
				for (unsigned int j = 0; j < height(); j++) line1[j] = a_pixels->data_at((i + j * width()) * components() + 3);
				for (unsigned int j = 0; j < height(); j++) a_pixels->set_data_at((i + j * width()) * components() + 3, a_pixels->data_at(((max - (i + 1)) + j * width()) * components() + 3));
				for (unsigned int j = 0; j < height(); j++) a_pixels->set_data_at(((max - (i + 1)) + j * width()) * components() + 3, line1[j]);
			}

			delete[] line1;
		};
		// Get every chunks into a PNG image
		std::vector<_PNG_Chunk> _get_all_chunks_from_path(std::string path) {
			std::vector<_PNG_Chunk> to_return = std::vector<_PNG_Chunk>();
			if (std::filesystem::exists(path) && !std::filesystem::is_directory(path))
			{
				// Create the necessary things to read the PNG file
				Bytes_Set file = Bytes_Set();
				file.load_from_file(path);
				a_idat_chunk.clear();
				std::string name = "";
				unsigned int size_offset = 33;

				unsigned int iter = 0;

				// Check each chunks in the file
				while (name != "IEND") {
                    unsigned int chunk_size = file.extract_uint(size_offset, true);
					name = file.extract_string(4, size_offset + 4);

					_PNG_Chunk chunk;
					chunk.position = size_offset + 8;
					chunk.name = name;
					chunk.size = chunk_size;
					size_offset += chunk_size + 12;
					to_return.push_back(chunk);

					if (name == "pHYs")
					{
						_load_pHYS_from_path(path, chunk);
					}
					else if (name == "IDAT" && is_loadable())
					{
						a_idat_chunk.push_back(chunk);
					}
					else if (name == "sRGB")
					{
						_load_sRGB_from_path(path, chunk);
					}
					else if (name == "PLTE" || name == "bKGD") // Not implemented yet
					{
						a_loadable = false;
					}

					iter++;
				}

				fill(0, 0, 0);

				// Load IDAT chunks
				if (a_idat_chunk.size() > 0 && is_loadable())
				{
					a_error_load = _load_IDAT_from_path(path);
					if (a_error_load != 1) a_loadable = false;
				}
			}
			return to_return;
		};
		// Load the base data of an image from a path
		bool _load_base_from_path(std::string path) {
			if (std::filesystem::exists(path) && !std::filesystem::is_directory(path))
			{
				// Create the necessary things to read the PNG file
				Bytes_Set file = Bytes_Set();
				file.load_from_file(path);

				// Check if the signature is correct (137 80 78 71 13 10 26 10 for png files)
				std::string file_signature = file.extract_string(8);
				std::vector<unsigned char> signature = png_signature();
				for (int i = 0; i < static_cast<int>(signature.size()); i++)
				{
				    if (signature[i] != static_cast<unsigned char>(file_signature[i])) return false;
				}

				// Check the first chunk of the file
				a_width = file.extract_uint(16, true);
				a_height = file.extract_uint(20, true);
				a_bit_depth = file.extract_data(24);
				a_color_type = file.extract_data(25);
				a_compression_method = file.extract_data(26);
				a_filter_method = file.extract_data(27);
				a_interlace_method = file.extract_data(28);
				fill(0, 0, 0, 0);

				return true;
			}
			return false;
		}
		// Load the image from a path
		bool load_from_path(std::string path) {
			if (_load_base_from_path(path))
			{
				_get_all_chunks_from_path(path);
				return true;
			}
			return false;
		};
		// Load the image from a set of binary datas coming from a FreeType text
		inline bool _load_from_text_binary(char* datas, unsigned short width, unsigned short height, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
		    a_height = height; a_width = width;
		    fill(0, 0, 0, 0);
		    if(color_type() == 6) {
                for(int i = 0;i<height;i++) {
                    for(int j = 0;j<width;j++) {
                        float glyph_alpha = (datas[i * width + j] / 255.0);
                        set_pixel(j, i, red, green, blue, static_cast<unsigned short>(glyph_alpha * static_cast<float>(alpha)));
                    }
                }
            }
            else {
                for(int i = 0;i<height;i++) {
                    for(int j = 0;j<width;j++) {
                        set_pixel(j, i, red, green, blue);
                    }
                }
            }
            return true;
		};
		// Load the image from a set of PNG binary data
		inline bool _load_from_binary_PNG(char* datas, unsigned int size) {
			write_in_file_binary("_temp.png", datas, size);

			bool result = load_from_path("_temp.png");

			std::remove("_temp.png");

			return result;
		};
		// Load a IDAT chunk grom a path
		char _load_IDAT_from_path(std::string path) {
			std::vector<_PNG_Chunk>& chunk = a_idat_chunk;
			if (std::filesystem::exists(path) && !std::filesystem::is_directory(path))
			{
				if (a_pixels == 0) return -1;

				// Get the size of the chunks
				unsigned int current_size = 0;
				for (int i = 0; i < static_cast<int>(chunk.size()); i++)
				{
					current_size += chunk[i].size;
				}

				// Create the necessary things to read the PNG file
				char* header = new char[current_size];
				char* header_part = new char[current_size];
				unsigned int total_size = 0;
				current_size = 0;

				// Read into the chunk
				for (int i = 0; i < static_cast<int>(chunk.size()); i++)
				{
					read_file_binary(path, header_part, chunk[i].size, chunk[i].position);
					total_size += chunk[i].size;

					for (unsigned int j = 0; j < chunk[i].size; j++)
					{
						(header[current_size + j]) = (header_part[j]);
					}
					current_size += chunk[i].size;
					delete[] header_part;
					header_part = new char[a_chunk_size];
				}
				delete[] header_part;
				header_part = 0;

                // Set binary mode
				SET_BINARY_MODE(stdin);
				SET_BINARY_MODE(stdout);

				// Define compression variables
				int ret = 0;
				unsigned int out_size = (width() * height() * components()) + height();
				unsigned char* out = new unsigned char[out_size];
				ret = uncompress_binary(header, current_size, (char*)out, out_size);

				if (ret != 1)
				{
					delete[] header;
					delete[] out;
					return ret;
				}

				// Process data
				unsigned int a_processed_data = 0;
				unsigned char component_size = components();
				unsigned int current_collumn = -1;
				unsigned int current_line = -1;
				Color last_pixel(0, 0, 0);
				unsigned int part = -1;
				for (unsigned int i = 0; i < out_size; i++) {
					if (part >= 0 && part < width() * component_size)
					{
						unsigned char component = part % component_size;
						if (component == 0) // Apply red component
						{
							a_processed_data++;
							current_collumn++;
							set_pixel_red(current_collumn, current_line, (unsigned char)(out[i]));
						}
						else if (component == 1) // Apply green component
						{
							set_pixel_green(current_collumn, current_line, (unsigned char)(out[i]));
						}
						else if (component == 2) // Apply blue component
						{
							set_pixel_blue(current_collumn, current_line, (unsigned char)(out[i]));
						}
						else if (component == 3) // Apply alpha component
						{
							set_pixel_alpha(current_collumn, current_line, (unsigned char)(out[i]));
						}
						part++;
					}
					else
					{
						if (a_processed_data > 0)
						{
							if (a_filter_type == 1) // Apply sub filtering
							{
								for (unsigned int i = 1; i < width(); i++)
								{
									Color color = pixel(i - 1, current_line);
									set_pixel_red(i, current_line, pixel(i, current_line).red() + color.red());
									set_pixel_green(i, current_line, pixel(i, current_line).green() + color.green());
									set_pixel_blue(i, current_line, pixel(i, current_line).blue() + color.blue());
									set_pixel_alpha(i, current_line, pixel(i, current_line).alpha() + color.alpha());
								}
							}
							else if (a_filter_type == 2 && a_processed_data > width()) // Apply up filtering
							{
								for (unsigned int i = 0; i < width(); i++)
								{
									Color color = pixel(i, current_line - 1);
									set_pixel_red(i, current_line, pixel(i, current_line).red() + color.red());
									set_pixel_green(i, current_line, pixel(i, current_line).green() + color.green());
									set_pixel_blue(i, current_line, pixel(i, current_line).blue() + color.blue());
									set_pixel_alpha(i, current_line, pixel(i, current_line).alpha() + color.alpha());
								}
							}
							else if (a_filter_type == 3) // Apply average filtering
							{
								if (a_processed_data > width())
								{
									for (unsigned int i = 0; i < width(); i++)
									{
										Color final_pixel = black;
										Color pixel1 = black;
										if (i == 0)
										{
											pixel1.set_red(0);
											pixel1.set_red(0);
											pixel1.set_red(0);
											pixel1.set_red(0);
										}
										else
										{
											pixel1 = pixel(i - 1, current_line);
										}
										Color pixel2 = pixel(i, current_line - 1);
										set_pixel_red(static_cast<unsigned short>(i), static_cast<unsigned short>(current_line), pixel(i, current_line).red() + static_cast<unsigned char>(floor((static_cast<float>(pixel1.red()) + static_cast<float>(pixel2.red())) / 2.0)));
										set_pixel_green(static_cast<unsigned short>(i), static_cast<unsigned short>(current_line), pixel(i, current_line).green() + static_cast<unsigned char>(floor((static_cast<float>(pixel1.green()) + static_cast<float>(pixel2.green())) / 2.0)));
										set_pixel_blue(static_cast<unsigned short>(i), static_cast<unsigned short>(current_line), pixel(i, current_line).blue() + static_cast<unsigned char>(floor((static_cast<float>(pixel1.blue()) + static_cast<float>(pixel2.blue())) / 2.0)));
										set_pixel_alpha(static_cast<unsigned short>(i), static_cast<unsigned short>(current_line), pixel(i, current_line).alpha() + static_cast<unsigned char>(floor((static_cast<float>(pixel1.alpha()) + static_cast<float>(pixel2.alpha())) / 2.0)));
									}
								}
							}
							else if (a_filter_type == 4 && current_line > 0) // Apply paeth filtering
							{
								for (unsigned int i = 0; i < width(); i++)
								{
									if (i == 0)
									{
										Color color = pixel(i, current_line - 1);
										set_pixel_red(static_cast<unsigned short>(i), static_cast<unsigned short>(current_line), pixel(i, current_line).red() + color.red());
										set_pixel_green(static_cast<unsigned short>(i), static_cast<unsigned short>(current_line), pixel(i, current_line).green() + color.green());
										set_pixel_blue(static_cast<unsigned short>(i), static_cast<unsigned short>(current_line), pixel(i, current_line).blue() + color.blue());
										set_pixel_alpha(static_cast<unsigned short>(i), static_cast<unsigned short>(current_line), pixel(i, current_line).alpha() + color.alpha());
									}
									else
									{
										Color pixel2 = pixel(i - 1, current_line);
										Color pixel3 = pixel(i - 1, current_line - 1);
										Color pixel1 = pixel(i, current_line - 1);
										set_pixel_red(static_cast<unsigned short>(i), static_cast<unsigned short>(current_line), pixel(i, current_line).red() + static_cast<unsigned char>(paeth_function(pixel1.red(), pixel2.red(), pixel3.red())));
										set_pixel_green(static_cast<unsigned short>(i), static_cast<unsigned short>(current_line), pixel(i, current_line).green() + static_cast<unsigned char>(paeth_function(pixel1.green(), pixel2.green(), pixel3.green())));
										set_pixel_blue(static_cast<unsigned short>(i), static_cast<unsigned short>(current_line), pixel(i, current_line).blue() + static_cast<unsigned char>(paeth_function(pixel1.blue(), pixel2.blue(), pixel3.blue())));
										set_pixel_alpha(static_cast<unsigned short>(i), static_cast<unsigned short>(current_line), pixel(i, current_line).alpha() + static_cast<unsigned char>(paeth_function(pixel1.alpha(), pixel2.alpha(), pixel2.alpha())));
									}
								}
							}
						}

						a_filter_type = out[i];
						current_collumn = -1;
						current_line++;
						part = 0;
					}
				}

				if (a_processed_data > 0) {
					if (a_filter_type == 1) // Apply sub filtering
					{
						for (unsigned int i = 1; i < width(); i++)
						{
							Color color = pixel(i - 1, current_line);
							set_pixel_red(i, current_line, pixel(i, current_line).red() + color.red());
							set_pixel_green(i, current_line, pixel(i, current_line).green() + color.green());
							set_pixel_blue(i, current_line, pixel(i, current_line).blue() + color.blue());
							set_pixel_alpha(i, current_line, pixel(i, current_line).alpha() + color.alpha());
						}
					}
					else if (a_filter_type == 2 && a_processed_data > width()) // Apply up filtering
					{
						for (unsigned int i = 0; i < width(); i++)
						{
							Color color = pixel(i, current_line - 1);
							set_pixel_red(i, current_line, pixel(i, current_line).red() + color.red());
							set_pixel_green(i, current_line, pixel(i, current_line).green() + color.green());
							set_pixel_blue(i, current_line, pixel(i, current_line).blue() + color.blue());
							set_pixel_alpha(i, current_line, pixel(i, current_line).alpha() + color.alpha());
						}
					}
					else if (a_filter_type == 3) // Apply average filtering
					{
						if (a_processed_data > width())
						{
							for (unsigned int i = 0; i < width(); i++)
							{
								Color final_pixel = black;
								Color pixel1 = black;
								if (i == 0)
								{
									pixel1.set_red(0);
									pixel1.set_green(0);
									pixel1.set_blue(0);
									pixel1.set_alpha(0);
								}
								else
								{
									pixel1 = pixel(i - 1, current_line);
								}
								Color pixel2 = pixel(i, current_line - 1);
								set_pixel_red(i, current_line, pixel(i, current_line).red() + static_cast<unsigned char>(floor((static_cast<float>(pixel1.red()) + static_cast<float>(pixel2.red())) / 2.0)));
								set_pixel_green(i, current_line, pixel(i, current_line).green() + static_cast<unsigned char>(floor((static_cast<float>(pixel1.green()) + static_cast<float>(pixel2.green())) / 2.0)));
								set_pixel_blue(i, current_line, pixel(i, current_line).blue() + static_cast<unsigned char>(floor((static_cast<float>(pixel1.blue()) + static_cast<float>(pixel2.blue())) / 2.0)));
								set_pixel_alpha(i, current_line, pixel(i, current_line).alpha() + static_cast<unsigned char>(floor((static_cast<float>(pixel1.alpha()) + static_cast<float>(pixel2.alpha())) / 2.0)));
							}
						}
					}
					else if (a_filter_type == 4 && a_processed_data > width()) // Apply paeth filtering
					{
						for (unsigned int i = 0; i < width(); i++)
						{
							if (i == 0)
							{
								Color color = pixel(i, current_line - 1);
								set_pixel_red(i, current_line, pixel(i, current_line).red() + color.red());
								set_pixel_green(i, current_line, pixel(i, current_line).green() + color.green());
								set_pixel_blue(i, current_line, pixel(i, current_line).blue() + color.blue());
								set_pixel_alpha(i, current_line, pixel(i, current_line).alpha() + color.alpha());
							}
							else
							{
								Color pixel2 = pixel(i - 1, current_line);
								Color pixel3 = pixel(i - 1, current_line - 1);
								Color pixel1 = pixel(i, current_line - 1);
								set_pixel_red(i, current_line, pixel(i, current_line).red() + static_cast<unsigned char>(paeth_function(pixel1.red(), pixel2.red(), pixel3.red())));
								set_pixel_green(i, current_line, pixel(i, current_line).green() + static_cast<unsigned char>(paeth_function(pixel1.green(), pixel2.green(), pixel3.green())));
								set_pixel_blue(i, current_line, pixel(i, current_line).blue() + static_cast<unsigned char>(paeth_function(pixel1.blue(), pixel2.blue(), pixel3.blue())));
								set_pixel_alpha(i, current_line, pixel(i, current_line).alpha() + static_cast<unsigned char>(paeth_function(pixel1.alpha(), pixel2.alpha(), pixel2.alpha())));
							}
						}
					}
				}

				// Free memory
				delete[] header;
				delete[] out;
			}
			else
			{
				return -4;
			}
			return 1;
		};
		// Load the pHYS chunk from a path
		bool _load_pHYS_from_path(std::string path, _PNG_Chunk chunk) {
			if (std::filesystem::exists(path) && !std::filesystem::is_directory(path) && chunk.name == "pHYs" && chunk.size == 9)
			{
				// Create the necessary things to read the PNG file
				Bytes_Set file = Bytes_Set();
				file.load_from_file(path);

				// Read into the chunk
				a_physical_height_ratio = file.extract_uint(chunk.position + 4, true);
				a_physical_width_ratio = file.extract_uint(chunk.position, true);
				a_physical_unit = file.extract_data(chunk.position + 8);

				return true;
			}
			return false;
		};
		// Load the sRGB chunk from a path
		bool _load_sRGB_from_path(std::string path, _PNG_Chunk chunk) {
			if (std::filesystem::exists(path) && !std::filesystem::is_directory(path) && chunk.name == "sRGB" && chunk.size == 1)
			{
				// Read into the chunk
				Bytes_Set file = Bytes_Set();
				file.load_from_file(path);
				a_srgb_value = file.extract_data(chunk.position);

				return true;
			}
			else
			{
				return false;
			}
		}
		// Paste an Image on this Image
		inline void paste(Image* to_paste, unsigned short x, unsigned short y, float opacity = 1.0, bool force_pasting = false) {
            for(unsigned int i = 0;i<to_paste->height();i++)
            {
                for(unsigned int j = 0;j<to_paste->width();j++)
                {
                    unsigned short final_x = x + j;
                    unsigned short final_y = y + i;

                    if(final_x >= width()) break;
                    if(final_y >= height()) return;

                    Color pixel = to_paste->pixel(j, i);

                    if(force_pasting) force_pixel(final_x, final_y, pixel);
                    else
                    {
                        pixel.set_alpha(static_cast<float>(pixel.alpha()) * opacity);
                        set_pixel(final_x, final_y, pixel);
                    }
                }
            }
		};
		// Paste an Image from a path to this Image
		inline void paste(std::string path, unsigned short x, unsigned short y, float opacity = 1.0, bool force_pasting = false) {
		    Image* img = new Image(path);
		    paste(img, x, y, opacity, force_pasting);
		    delete img; img = 0;
		};

    private:
	    // Base datas about the image
        // Bit depth of the image
		unsigned int a_bit_depth = 8;
		// Color type of the image
		unsigned int a_color_type = 6;
		// Height of the image
		unsigned int a_height = 0;
		// Pixel of the image
		Bytes_Set* a_pixels = 0;
		// Width of the image
		unsigned int a_width = 0;

		// Datas necessary to use PNG
		// Compression method of the image
		unsigned int a_compression_method = 0;
		// Filter method of the image
		unsigned int a_filter_method = 0;
		// Filter type of the image
		unsigned char a_filter_type = 0;
		// Interlace method of the image
		unsigned int a_interlace_method = 0;
		// If the image can be loaded or not
		bool a_loadable = true;
		// Physical height of the image
		unsigned int a_physical_height_ratio = 10000;
		// Physical unit of the image (0 = unknow, 1 = meter)
		unsigned int a_physical_unit = 1;
		// Physical width of the image
		unsigned int a_physical_width_ratio = 10000;
		// Value of the sRGB chunk
		unsigned char a_srgb_value = 0;




		// Size of a chunk to decode an image
		unsigned int a_chunk_size = static_cast<unsigned int>(pow(2, 24) - 1);
		// Error during the loading (1 = normal)
		char a_error_load = 1;
		// Vector of all the IDAT chunks in the image
		std::vector<_PNG_Chunk> a_idat_chunk = std::vector<_PNG_Chunk>();
	};

	// Fonts datas
	struct Font {
	    // Path descriptor
        std::string font_family = "";
        std::string font_path = "";

        // Aspect descriptor
        bool bold = false;
        bool condensed = false;
        bool italic = false;
        bool light = false;
	};

	// List of all the fonts in the system
	static std::map<std::string, Font> _system_fonts = std::map<std::string, Font>();
	static FT_Library  _freetype_library;
    static bool _free_type_library_inited = false;

	// Load all the fonts system fonts
	inline void load_system_font() {
        _system_fonts.clear();
        std::vector<std::string> font_files = std::vector<std::string>();
        std::vector<std::string> subpaths = directory_content(BASE_FONT_PATH, true);

        for(int i = 0;i<static_cast<int>(subpaths.size());i++)
        {
            if(file_extension(subpaths[i]) == "ttf")
            {
                font_files.push_back(subpaths[i]);
            }
        }

        for(int i = 0;i<static_cast<int>(font_files.size());i++)
        {
            Font font; font.font_path = font_files[i];
            std::string font_name = "";
            std::string font_full_name = file_name(font_files[i]);
            std::vector<std::string> cutted = cut_string(font_full_name, "-");
            std::string last_name = "";
            if(cutted.size() > 0)
            {
                std::string descriptor = cutted[cutted.size() - 1];
                if(contains(descriptor, "Bold"))
                {
                    font.bold = true;
                    last_name += "b";
                }
                if(contains(descriptor, "Italic"))
                {
                    font.italic = true;
                    last_name += "i";
                }
                if(contains(descriptor, "Condensed"))
                {
                    font.condensed = true;
                    last_name += "c";
                }
                if(contains(descriptor, "Light"))
                {
                    font.light = true;
                    last_name += "l";
                }
                font.font_family = cutted[0];
                font_name = cutted[0];
            }
            _system_fonts[font_name + last_name] = font;
        }
	};

	// Return the system path of a font
	inline Font get_system_font(std::string font, bool bold = false, bool italic = false, bool condensed = false, bool light = false) {
	    if(_system_fonts.size() <= 0) load_system_font();

	    std::string last_name = "";
	    if(bold) last_name += "b";
	    if(italic) last_name += "i";
	    if(condensed) last_name += "c";
	    if(light) last_name += "l";
        return _system_fonts[font + last_name];
	};

	// Print each system fonts
	inline void print_system_fonts() {
	    if(_system_fonts.size() <= 0) load_system_font();

	    for(std::map<std::string, Font>::iterator it = _system_fonts.begin();it!=_system_fonts.end();it++)
        {
            std::string message = "System font \"" + it->second.font_family + "\"";
            if(it->second.bold) message += " bold";
            if(it->second.condensed) message += " condensed";
            if(it->second.italic) message += " italic";
            if(it->second.light) message += " light";
            message += ".";
            std::string command_parameter = "\"" + it->second.font_family + "\"";
            if(it->second.bold) { command_parameter += ", true"; if(it->second.italic || it->second.condensed || it->second.light) command_parameter += ", "; }
            else if(it->second.italic || it->second.condensed || it->second.light) command_parameter += ", false, ";
            if(it->second.italic) { command_parameter += "true"; if(it->second.condensed || it->second.light) command_parameter += ", "; }
            else if(it->second.condensed || it->second.light) command_parameter += "false, ";
            if(it->second.condensed) {command_parameter += "true"; if(it->second.light) command_parameter += ", "; }
            else if(it->second.light) command_parameter += "false, ";
            if(it->second.light) command_parameter += "true";
            message += " Use \"scls::get_system_font(" + command_parameter + ")\" to get it.";
            print("Information", "SCLS Fonts", message);
        }
	};

	// Enumeration of each text alignment possible
	enum Text_Alignment {
	    Left, Center, Right
	};

	// Datas about the text to draw
    struct Text_Image_Data {
        // Color of the text
        unsigned char alpha = 255;
        unsigned char blue = 0;
        unsigned char green = 0;
        unsigned char red = 0;

        // Color of the background
        unsigned char background_alpha = 0;
        unsigned char background_blue = 0;
        unsigned char background_green = 0;
        unsigned char background_red = 0;

        // Font particularity
        Font font;
        unsigned short font_size = 50;

        // Multi line caracteristic
        Text_Alignment alignment = Left;

        // Out offset
        unsigned short out_offset_bottom_width = 0;
        unsigned short out_offset_left_width = 0;
        unsigned short out_offset_right_width = 0;
        unsigned short out_offset_top_width = 0;
    };

    // Return a pointer to an image with a char on it
    inline Image* _char_image(char character, FT_Face& face, int& cursor_pos, int& y_pos, Text_Image_Data datas) {
        // Configure and load the FreeType glyph system
        FT_UInt index = FT_Get_Char_Index(face, (static_cast<unsigned char>(character)));
        FT_Error error = FT_Load_Glyph(face, index, 0);
        FT_GlyphSlot binary_datas = face->glyph;
        error = FT_Render_Glyph(binary_datas, FT_RENDER_MODE_NORMAL);

        // Create and draw the image
        unsigned short height = static_cast<unsigned short>(binary_datas->bitmap.rows);
        unsigned short width = static_cast<unsigned short>(binary_datas->bitmap.width);
        Image* img = new Image();
        img->_load_from_text_binary(reinterpret_cast<char*>(binary_datas->bitmap.buffer), width, height, datas.red, datas.green, datas.blue, datas.alpha);

        // Get the position of the cursor
        cursor_pos = binary_datas->bitmap_left;
        y_pos = binary_datas->bitmap_top - height;

        return img;
    };

    // Return a pointer to an image with a text on it
    inline Image* _line_image(std::string content, Text_Image_Data datas) {
        // Base variables for the creation
        unsigned int font_size = datas.font_size;
        std::string path = datas.font.font_path;

        // Load the FreeType base system
        if(!_free_type_library_inited)
        {
            FT_Error error = FT_Init_FreeType(&_freetype_library);
            if ( error )
            {
                print("Error", "SCLS", "Unable to load the FreeType engine.");
                return 0;
            }
            _free_type_library_inited = true;
        }
        FT_Face face;
        FT_Error error = FT_New_Face(_freetype_library, path.c_str(), 0, &face);
        if (!std::filesystem::exists(path))
        {
            print("Error", "SCLS", "Unable to load the \"" + path + "\" font, it does not exist.");
            return 0;
        }
        else if ( error )
        {
            print("Error", "SCLS", "Unable to load the \"" + path + "\" font.");
            return 0;
        }

        // Configure and load the FreeType glyph system
        error = FT_Select_Charmap(face, FT_ENCODING_UNICODE);
        error = FT_Set_Pixel_Sizes(face, 0, font_size);

        // Create each characters
        std::vector<Image*> characters;
        std::vector<int> cursor_pos;
        int max_height = 0;
        int to_add_font_size = 0;
        unsigned int total_width = 0;
        std::vector<unsigned int> y_pos;
        for(int i = 0;i<static_cast<int>(content.size());i++)
        {
            if(content[i] == ' ')
            {
                characters.push_back(0);
                cursor_pos.push_back(0);
                y_pos.push_back(0);
                total_width += font_size / 2.0;
                continue;
            }
            int cursor_position = 0;
            int y_position = 0;
            Image* image = _char_image(content[i], face, cursor_position, y_position, datas);
            characters.push_back(image);
            cursor_pos.push_back(total_width + cursor_position);
            if(cursor_pos[cursor_pos.size() - 1] < 0) cursor_pos[cursor_pos.size() - 1] = 0; // Avoid a little bug with X position
            if(static_cast<int>(image->height()) + y_position > max_height) max_height = image->height() + y_position;
            if(y_position < to_add_font_size) to_add_font_size = y_position;
            y_pos.push_back(y_position);
            total_width += image->width() + cursor_position;
        }

        for(int i = 0;i<static_cast<int>(characters.size());i++)
        {
            if(characters[i] != 0)
            {
                y_pos[i] = max_height - (characters[i]->height() + y_pos[i]);
            }
        }

        // Create the final image and clear the memory
        Image* final_image = new Image(total_width, max_height - to_add_font_size, datas.background_red, datas.background_green, datas.background_blue, datas.background_alpha);
        for(int i = 0;i<static_cast<int>(characters.size());i++)
        {
            if(characters[i] != 0)
            {
                unsigned int x = cursor_pos[i];
                final_image->paste(characters[i], x, y_pos[i]);
                delete characters[i]; characters[i] = 0;
            }
        }

        return final_image;
    };

    // Return a pointer to an image with a text on it
    inline Image* text_image(std::string content, Text_Image_Data datas) {
        // Construct each text parts
        std::vector<std::string> parts = cut_string(content, "\n");

        // Load the font if necessary
        if(_system_fonts.size() <= 0) load_system_font();


        if(datas.font.font_family == "") datas.font = get_system_font(DEFAULT_FONT);

        // Create each lines
        std::vector<Image*> image_parts = std::vector<Image*>();
        unsigned int max_width = 0;
        unsigned int min_x = datas.out_offset_left_width;
        unsigned int min_y = datas.out_offset_top_width;
        unsigned int total_height = 0;
        for(int i = 0;i<static_cast<int>(parts.size());i++)
        {
            if(parts[i] == "") {
                image_parts.push_back(0);
                total_height += datas.font_size;
            }
            else {
                Image* image = _line_image(parts[i], datas);
                if(image != 0)
                {
                    image_parts.push_back(image);
                    total_height += image->height();
                    if(image->width() > max_width) max_width = image->width();
                }
            }
        }

        // Create the final image and clear memory
        Image* final_image = new Image(max_width + min_x + datas.out_offset_right_width, total_height + min_y + datas.out_offset_bottom_width, datas.background_red, datas.background_green, datas.background_blue, datas.background_alpha);
        unsigned int y_position = min_y;
        for(int i = 0;i<static_cast<int>(image_parts.size());i++)
        {
            Image* image = image_parts[i]; if(image == 0) { y_position += datas.font_size; continue; }
            unsigned int x = min_x;
            if(datas.alignment == Center)x = min_x + static_cast<int>(static_cast<float>(max_width - image->width()) / 2.0);
            else if(datas.alignment == Right) x = min_x + max_width - image->width();
            final_image->paste(image, x, y_position); y_position += image->height();
            delete image_parts[i]; image_parts[i] = 0;
        }

        return final_image;
    };

    // Most simple text_image function
    inline Image* text_image(std::string content) {
        Text_Image_Data datas;
        return text_image(content, datas);
    }
}

#endif // SCLS_IMAGE_CORE
