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
    Color::Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
            a_alpha = static_cast<double>(alpha) / 255.0;
            a_blue = static_cast<double>(blue) / 255.0;
            a_green = static_cast<double>(green) / 255.0;
            a_red = static_cast<double>(red) / 255.0;
    }

    // Return a color loaded from a text
    Color Color::from_std_string(std::string source) {
        // Format the text
        while(source[0] == ' ' || source[0] == '(') source = source.substr(1, source.size() - 1);
        while(source[source.size() - 1] == ' ' || source[source.size() - 1] == ')') source = source.substr(0, source.size() - 1);

        // Get the color
        Color to_return(255, 255, 255);
        std::vector<std::string> cutted = cut_string(source, ",");
        for(int i = 0;i<static_cast<int>(cutted.size());i++) {
            while(cutted[i][0] == ' ') cutted[i] = cutted[i].substr(1, cutted[i].size() - 1);
            while(cutted[i][cutted[i].size() - 1] == ' ') cutted[i] = cutted[i].substr(0, cutted[i].size() - 1);

            // Apply the color
            if(i == 0) to_return.set_red(std::stoi(cutted[i]));
            else if(i == 1) to_return.set_green(std::stoi(cutted[i]));
            else if(i == 2) to_return.set_blue(std::stoi(cutted[i]));
            else if(i == 3) to_return.set_alpha(std::stoi(cutted[i]));
        }
        return to_return;
    };

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

    // Fill the image with one color
    void Image::fill(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
        free_memory();
        a_pixels.reset(new Bytes_Set(buffer_size()));
        unsigned int current_thread_position = 0;
        unsigned int pixel_by_thread = floor(static_cast<double>((width() * height()) / static_cast<double>(a_thread_number_for_filling)));

        // Create each threads
        if(a_thread_number_for_filling > 0) {
            std::vector<std::thread*> threads = std::vector<std::thread*>();
            for(unsigned short i = 0;i<a_thread_number_for_filling - 1;i++) {
                std::thread* current_thread = new std::thread(&Image::__fill_pixel_part, this, current_thread_position, pixel_by_thread, red, green, blue, alpha);
                threads.push_back(current_thread);
                current_thread_position += pixel_by_thread;
            }
            std::thread* current_thread = new std::thread(&Image::__fill_pixel_part, this, current_thread_position, (width() * height()) - current_thread_position, red, green, blue, alpha);
            threads.push_back(current_thread);

            // Wait for each threads
            for(int i = 0;i<static_cast<int>(threads.size());i++) {
                threads[i]->join();
                delete threads[i]; threads[i] = 0;
            } threads.clear();
        }
        else {
            __fill_pixel_part(0, width() * height(), red, green, blue, alpha);
        }
    };

    // Fill a part of pixel
    void Image::__fill_pixel_part(unsigned int start_position, unsigned int length, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
        if(color_type() == SCLS_IMAGE_RGBA) {for (unsigned int i = 0; i < length; i++) {set_pixel_rgba_directly(i * 4, red, green, blue, alpha, 1);}}
        else if(color_type() == SCLS_IMAGE_RGB) {
           for (unsigned int i = 0; i < length; i++) {
                a_pixels->set_data_at((start_position + i) * 3, red);
                a_pixels->set_data_at((start_position + i) * 3 + 1, green);
                a_pixels->set_data_at((start_position + i) * 3 + 2, blue);
            }
        }
    };

    // Force a pixel to change its value
    void Image::force_pixel(unsigned short x, unsigned short y, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
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
    std::shared_ptr<Bytes_Set> Image::datas_filtered() {
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
    std::shared_ptr<__Image_Error> Image::load_from_path(std::string path) {
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
    Color Image::pixel(unsigned short x, unsigned short y) {
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
    Color Image::pixel_by_number(unsigned int position) {
        unsigned char multiplier = (bit_depht() / 8.0);
        position *= components() * (bit_depht() / 8.0);
        Color to_return = Color(255, 255, 255);

        if(color_type() == 6) {to_return = pixel_rgba_directly(position, multiplier);}
        else {
            to_return.set_rgb(a_pixels->data_at(position), a_pixels->data_at(position + multiplier), a_pixels->data_at(position + 2 * multiplier));
        }

        return to_return;
    };

    // Set datas about a specific pixel*
    void Image::set_pixel(int x, int y, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, unsigned short width_point) {
        if (width_point == 0) return;
        else if (width_point == 1) {
            // Check the position
            if (x < 0 || y < 0 || x >= width() || y >= height()) {
                print("Warning", "SCLS Image", "The position (" + std::to_string(x) + "; " + std::to_string(y) + ") you want to set is out of the image.");
                return;
            }

            unsigned char multiplier = (bit_depht() / 8.0);
            unsigned int position = (y * width() + x) * components() * (bit_depht() / 8.0);

            // Process the color
            Color color = pixel(x, y);
            double alpha_f = normalize_value(alpha, 0, 255) / 255.0;
            double blue_f = normalize_value(blue, 0, 255);
            double green_f = normalize_value(green, 0, 255);
            double red_f = normalize_value(red, 0, 255);
            // Calculate alpha
            alpha = normalize_value(alpha, 0, 255); if(color.alpha() > alpha) alpha = color.alpha();
            blue = alpha_f * blue_f + (1.0 - alpha_f) * static_cast<double>(color.blue());
            red = alpha_f * red_f + (1.0 - alpha_f) * static_cast<double>(color.red());
            green = alpha_f * green_f + (1.0 - alpha_f) * static_cast<double>(color.green());

            if(color_type() == SCLS_IMAGE_RGBA) {set_pixel_rgba_directly(position, red, green, blue, alpha, multiplier);}
            else if(color_type() == SCLS_IMAGE_RGB) {
                a_pixels->set_data_at_directly(position, red);
                a_pixels->set_data_at_directly(position + multiplier, green);
                a_pixels->set_data_at_directly(position + 2 * multiplier, blue);
            }
        }
        else {
            fill_rect(static_cast<int>(static_cast<double>(x) - static_cast<double>(width_point) / 2.0), static_cast<int>(static_cast<double>(y) - (static_cast<double>(width_point)) / 2.0), width_point, width_point, red, green, blue, alpha);
        }
    }
    void Image::set_pixel_alpha(unsigned short x, unsigned short y, unsigned char alpha) {
        if (x < 0 || y < 0 || x >= width() || y >= height()) {
            print("Warning", "SCLS Image", "The position (" + std::to_string(x) + "; " + std::to_string(y) + ") you want to set the alpha is out of the image.");
            return;
        }

        unsigned char multiplier = (bit_depht() / 8.0);
        unsigned int position = (y * width() + x) * components() * (bit_depht() / 8.0);
        if(color_type() == 6)
        {
            // Calculate alpha
            alpha = normalize_value(alpha, 0, 255);
            a_pixels->set_data_at(position + 3 * multiplier,  alpha);
        }
    }
    void Image::set_pixel_blue(unsigned short x, unsigned short y, unsigned char blue, unsigned char alpha) {
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
    void Image::set_pixel_by_number(unsigned int position, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
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
        else {
            a_pixels->set_data_at(position, red);
            a_pixels->set_data_at(position + multiplier, green);
            a_pixels->set_data_at(position + 2 * multiplier, blue);
        }
    };
    void Image::set_pixel_green(unsigned short x, unsigned short y, unsigned char green, unsigned char alpha) {
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
    void Image::set_pixel_red(unsigned short x, unsigned short y, unsigned char red, unsigned char alpha) {
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
    };
    void Image::set_pixel_rgba_directly_with_alpha(unsigned int position, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, unsigned char multiplier) {
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

    //*********
    //
    // The Image class - PNG
    //
    //*********

    Bytes_Set* Image::datas_png() {
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
    void Image::_load_all_chunks_from_png_file(Bytes_Set* file, std::shared_ptr<__Image_Error>& error_handler) {
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
        } else { fill(0, 0, 0); error_handler.get()->set_value(SCLS_IMAGE_ERROR_UNKNOW);}
    }

    // Loads the bKGD chunk from a path and returns the color
    Color Image::_load_bKGD_from_file(Bytes_Set* file, _PNG_Chunk chunk) {
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
    void Image::_load_png_IDAT_from_file_rgba(int component_size, int current_line_start_position, int last_line_start_position, int multiplier, int processed_data){
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
    void Image::_load_png_IDAT_from_file(Bytes_Set* file, std::shared_ptr<__Image_Error>& error_handler) {
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
        } else { error_handler.get()->set_value(SCLS_IMAGE_ERROR_UNKNOW); }
    }

    // Load the pHYS chunk from a path
    void Image::_load_png_pHYS_from_file(Bytes_Set* file, _PNG_Chunk chunk, std::shared_ptr<__Image_Error>& error_handler) {
        if (file != 0 && chunk.name == "pHYs" && chunk.size == 9) {
            // Read into the chunk
            a_physical_height_ratio = file->extract_uint(chunk.position + 4, true);
            a_physical_width_ratio = file->extract_uint(chunk.position, true);
            a_physical_unit = file->data_at(chunk.position + 8);
        }
        else error_handler.get()->set_value(SCLS_IMAGE_PNG_ERROR_WRONG_PHYS_CHUNK);
    };

    // Load the Image from a PNG file
    void Image::_load_png_file(Bytes_Set* file, std::shared_ptr<__Image_Error>& error_handler) {
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
    void Image::_load_png_sRGB_from_file(Bytes_Set* file, _PNG_Chunk chunk, std::shared_ptr<__Image_Error>& error_handler) {
        if (file != 0 && chunk.name == "sRGB" && chunk.size == 1) {
            // Read into the chunk
            a_srgb_value = file->data_at(chunk.position);
        }
        else error_handler.get()->set_value(SCLS_IMAGE_PNG_ERROR_WRONG_SRGB_CHUNK);
    }

    // Returns the signature of a PNG file
    std::vector<unsigned char> Image::png_signature() {
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

    //*********
    //
    // The Image class - Editing
    //
    //*********

    // Draw a circle on the image
    void Image::draw_circle(int x_center, int y_center, double radius, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, unsigned short line_width) {
        const int start_x = round(x_center - radius);
        const int start_x_inner = start_x + line_width;
        int current_x = 0;
        // Check the inner part
        double radius_inner = radius - line_width;

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

            // If the coordonate his out of the image
            if(needed_x >= 0 && needed_x < needed_width) {
                // Draw each needed pixels
                // Set the last y
                int last_y = 0;
                if(needed_x >= start_x_inner) {
                    double current_ratio = static_cast<double>(needed_x - start_x_inner) / radius_inner;
                    last_y = round(std::sin(std::acos(1.0 - current_ratio)) * radius_inner);
                }
                // Draw the circle
                for(int i = 0;i < (needed_y - last_y);i++) {
                    // Left of the circle
                    needed_x = (x_center - radius) + current_x;
                    int current_y = (y_center + (last_y + i));
                    if(current_y >= 0 && current_y < needed_height) {
                        int position = (current_y * needed_width + needed_x) * needed_components;
                        set_pixel_rgba_directly(position, red, green, blue, alpha, multiplier);
                    } current_y = (y_center - (last_y + i));
                    if(current_y >= 0 && current_y < needed_height) {
                        int position = (current_y * needed_width + needed_x) * needed_components;
                        set_pixel_rgba_directly(position, red, green, blue, alpha, multiplier);
                    }
                    // Right of the circle
                    needed_x = (x_center + radius) - current_x;
                    current_y = (y_center + (last_y + i));
                    if(current_y >= 0 && current_y < needed_height) {
                        int position = (current_y * needed_width + needed_x) * needed_components;
                        set_pixel_rgba_directly(position, red, green, blue, alpha, multiplier);
                    } current_y = (y_center - (last_y + i));
                    if(current_y >= 0 && current_y < needed_height) {
                        int position = (current_y * needed_width + needed_x) * needed_components;
                        set_pixel_rgba_directly(position, red, green, blue, alpha, multiplier);
                    }
                }
            }
        }
    }
    // Fills a circle on the image
    void Image::fill_circle(int x_center, int y_center, double radius, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
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
                if(needed_x >= 0 && needed_x < needed_width) {
                    needed_x = (x_center - radius) + current_x;
                    int current_y = (y_center + i);
                    if(current_y >= 0 && current_y < needed_height) {
                        int position = (current_y * needed_width + needed_x) * needed_components;
                        set_pixel_rgba_directly(position, red, green, blue, alpha, multiplier);
                    } current_y = (y_center - i);
                    if(current_y >= 0 && current_y < needed_height) {
                        int position = (current_y * needed_width + needed_x) * needed_components;
                        set_pixel_rgba_directly(position, red, green, blue, alpha, multiplier);
                    }
                }
                // Right of the circle
                needed_x = (x_center + radius) - current_x;
                if(needed_x >= 0 && needed_x < needed_width) {
                    int current_y = (y_center + i);
                    if(current_y >= 0 && current_y < needed_height) {
                        int position = (current_y * needed_width + needed_x) * needed_components;
                        set_pixel_rgba_directly(position, red, green, blue, alpha, multiplier);
                    } current_y = (y_center - i);
                    if(current_y >= 0 && current_y < needed_height) {
                        int position = (current_y * needed_width + needed_x) * needed_components;
                        set_pixel_rgba_directly(position, red, green, blue, alpha, multiplier);
                    }
                }
            }
        }
    }
    // Fill a circle with a gradient on the image
    void Image::fill_circle_gradient(int x_center, int y_center, double radius, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, Color (*needed_function)(double, int, int, int, unsigned char, unsigned char, unsigned char, unsigned char)) {
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

    // Draw a line on the image
    void Image::draw_line(int x_1, int y_1, int x_2, int y_2, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha, unsigned short line_width) {
        // Only case which the algorithm does not work correctly
        if(x_1 == x_2) {
            // Check the X position
            if(x_1 + static_cast<int>(line_width) < 0 || x_1 >= width()) return;
            y_2++;
            if(y_2 < y_1) {
                y_2 += y_1;
                y_1 = y_2 - y_1;
                y_2 = y_2 - y_1;
            }
            // Check the Y position
            for(int i = 0;i<y_2 - y_1;i++) set_pixel(x_1, y_1 + i, red, green, blue, alpha, line_width);
        }
        else if(y_1 + static_cast<int>(line_width) == y_2) {
            // Check the Y position
            if(y_1 < 0 || y_1 >= height()) return;
            x_2++;
            if(x_2 < x_1) {
                x_2 += x_1;
                x_1 = x_2 - x_1;
                x_2 = x_2 - x_1;
            }
            // Check the X position
            for(int i = 0;i<x_2 - x_1;i++) set_pixel(x_1 + i, y_1, red, green, blue, alpha, line_width);
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
                while(actual_y < 0) {actual_y++;actual_x += x_y_ratio;}
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

                double actual_x = x_1;
                double actual_y = y_1;

                // Check the positions
                double y_x_ratio = distance_y / distance_x;
                while(actual_x < 0) {actual_x++;actual_y += y_x_ratio;}
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

    // Draw a rectangle on the imageE
    void Image::draw_rect(unsigned short x, unsigned short y, unsigned short width, unsigned short height, unsigned int rect_width, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
        fill_rect(x, y, width, rect_width, red, green, blue, alpha);
        fill_rect(x, y + rect_width, rect_width, height - rect_width, red, green, blue, alpha);
        fill_rect(width - rect_width, y + rect_width, rect_width, height - rect_width, red, green, blue, alpha);
        fill_rect(x + rect_width, height - rect_width, width - 2 * rect_width, rect_width, red, green, blue, alpha);
    }

    // Fill a rectangle on the image
    void Image::fill_rect(int x, int y, unsigned short rect_width, unsigned short rect_height, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
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

    // Fill a rectangle on the image
    void Image::fill_triangle(short x_1, short y_1, short x_2, short y_2, short x_3, short y_3, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
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
        std::vector<long long> all_x_1_2 = partition_number(distance_y_1_2, distance_x_1_2);

        // Calculate the datas for the X2 to X3 line
        double distance_x_2_3 = static_cast<double>(x_3 - x_2);
        double distance_y_2_3 = static_cast<double>(y_3 - y_2);
        std::vector<long long> all_x_2_3 = partition_number(distance_y_2_3, distance_x_2_3);

        // Calculate the datas for the X1 to X3 line
        double distance_x_1_3 = static_cast<double>(x_3 - x_1);
        double distance_y_1_3 = static_cast<double>(y_3 - y_1);
        std::vector<long long> all_x_1_3 = partition_number(distance_y_1_3, distance_x_1_3);

        double actual_x = x_1;
        double actual_y = y_1;
        double actual_y_added = 0;

        authorized_sender().push_back("");

        // Create the first line
        unsigned short iter = 0; unsigned short iter_total = 0;
        while(actual_x < x_2) {
            draw_line(actual_x, actual_y, actual_x, actual_y + actual_y_added, red, green, blue, alpha);
            actual_x++; actual_y += all_x_1_3[iter_total];
            iter++; iter_total++; actual_y_added += all_x_1_2[iter];
        }
        // Create the second line
        iter = 0; if(distance_x_1_2 <= 0) actual_y_added = distance_y_1_2;
        while(actual_x < x_3) {
            draw_line(actual_x, actual_y, actual_x, actual_y + actual_y_added, red, green, blue, alpha);
            actual_x++; actual_y += all_x_1_3[iter_total];
            iter++; iter_total++; actual_y_added += all_x_2_3[iter];
        }

        /*draw_line(static_cast<short>(actual_x), static_cast<short>(actual_y), x_3, y_3, red, green, blue, alpha, 2);

        while (actual_y < y_2) {
            if (abs(ratio_x_y) < 1) {
                actual_y++;
                actual_x += ratio_x_y;
            }
            else {
                actual_y += ratio_y_x;
                if (distance_x_1_2 > 0) actual_x++;
                else actual_x--;
            }

            if(actual_x < 0) actual_x = 0;
            if(actual_y < 0) actual_y = 0;

            draw_line(static_cast<unsigned short>(floor(actual_x)), static_cast<unsigned short>(floor(actual_y)), x_3, y_3, red, green, blue, alpha, 2);
            draw_line(static_cast<unsigned short>(floor(actual_x)), static_cast<unsigned short>(ceil(actual_y)), x_3, y_3, red, green, blue, alpha, 2);
            draw_line(static_cast<unsigned short>(ceil(actual_x)), static_cast<unsigned short>(floor(actual_y)), x_3, y_3, red, green, blue, alpha, 2);
            draw_line(static_cast<unsigned short>(ceil(actual_x)), static_cast<unsigned short>(ceil(actual_y)), x_3, y_3, red, green, blue, alpha, 2);
            iter++;
        } //*/
        authorized_sender().pop_back();
    }

    // Flip the image on the X axis
    void Image::flip_x() {
        unsigned char* line1 = new unsigned char[width()];
        int max = height();

        for (int i = 0; i < floor(static_cast<float>(max) / 2.0); i++)
        {
            // Red
            for (int j = 0; j < width(); j++) line1[j] = a_pixels->data_at((i * width() + j) * components());
            for (int j = 0; j < width(); j++) a_pixels->set_data_at((i * width() + j) * components(), a_pixels->data_at(((max - (i + 1)) * width() + j) * components()));
            for (int j = 0; j < width(); j++) a_pixels->set_data_at(((max - (i + 1)) * width() + j) * components(), line1[j]);

            // Green
            for (int j = 0; j < width(); j++) line1[j] = a_pixels->data_at((i * width() + j) * components() + 1);
            for (int j = 0; j < width(); j++) a_pixels->set_data_at((i * width() + j) * components() + 1, a_pixels->data_at(((max - (i + 1)) * width() + j) * components() + 1));
            for (int j = 0; j < width(); j++) a_pixels->set_data_at(((max - (i + 1)) * width() + j) * components() + 1, line1[j]);

            // Blue
            for (int j = 0; j < width(); j++) line1[j] = a_pixels->data_at((i * width() + j) * components() + 2);
            for (int j = 0; j < width(); j++) a_pixels->set_data_at((i * width() + j) * components() + 2, a_pixels->data_at(((max - (i + 1)) * width() + j) * components() + 2));
            for (int j = 0; j < width(); j++) a_pixels->set_data_at(((max - (i + 1)) * width() + j) * components() + 2, line1[j]);

            if(color_type() == 6) {
                // Alpha
                for (int j = 0; j < width(); j++) line1[j] = a_pixels->data_at((i * width() + j) * components() + 3);
                for (int j = 0; j < width(); j++) a_pixels->set_data_at((i * width() + j) * components() + 3, a_pixels->data_at(((max - (i + 1)) * width() + j) * components() + 3));
                for (int j = 0; j < width(); j++) a_pixels->set_data_at(((max - (i + 1)) * width() + j) * components() + 3, line1[j]);
            }
        } a_flip_x_number++;

        delete[] line1;
    }
    // Flip the image on the Y axis
    void Image::flip_y() {
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
    void Image::paste(Image* to_paste, int x, int y, double opacity) {
        unsigned int current_thread_position = 0;
        unsigned int pixel_by_thread = floor((static_cast<double>(to_paste->width()) * static_cast<double>(to_paste->height())) / static_cast<double>(a_thread_number_for_pasting));

        // Create each threads
        if(a_thread_number_for_pasting > 1) {
            std::vector<std::thread*> threads = std::vector<std::thread*>();
            for(unsigned short i = 0;i<a_thread_number_for_pasting - 1;i++) {
                unsigned int start_x = floor(current_thread_position % to_paste->width());
                unsigned int start_y = floor(current_thread_position / to_paste->width());

                std::thread* current_thread = new std::thread(&Image::__paste_part_of_image, this, to_paste, x, y, start_x, start_y, pixel_by_thread, opacity);
                threads.push_back(current_thread);
                current_thread_position += pixel_by_thread;
            }
            unsigned int start_x = floor(current_thread_position % to_paste->width());
            unsigned int start_y = floor(current_thread_position / to_paste->width());
            std::thread* current_thread = new std::thread(&Image::__paste_part_of_image, this, to_paste, x, y, start_x, start_y, (static_cast<double>(to_paste->width()) * static_cast<double>(to_paste->height())) - current_thread_position , opacity);
            threads.push_back(current_thread);

            // Wait for each threads
            for(int i = 0;i<static_cast<int>(threads.size());i++) {
                threads[i]->join();
                delete threads[i]; threads[i] = 0;
            } threads.clear();
        }
        else {
            __paste_part_of_image(to_paste, x, y, 0, 0, (static_cast<double>(to_paste->width()) * static_cast<double>(to_paste->height())), opacity);
        }
    }
    // Paste a part of an image on this image
    void Image::__paste_part_of_image(Image* to_paste, int x_offset, int y_offset, int start_x, int start_y, int length, double opacity) {
        for(int i = 0;i<length;i++) {
            if(x_offset + start_x >= 0 && y_offset + start_y >= 0 && x_offset + start_x < width() && y_offset + start_y < height()) {
                Color pixel = to_paste->pixel(start_x, start_y);
                set_pixel(x_offset + start_x, y_offset + start_y, pixel);
            }

            start_x++; if(start_x >= to_paste->width()) {
                start_x = 0;
                start_y++;
            }
        }
    }

    // Apply a simple circle gradient in the image
    void Image::apply_gradient_circle(Color in, Color out, int x, int y, unsigned short radius) {
        for(unsigned short i = 0;i<width();i++) {
            for(unsigned short j = 0;j<height();j++) {
                double distance = std::sqrt((i - x) * (i - x) + (j - y) * (j - y));
                double pourcentage = distance / static_cast<double>(radius);
                if(pourcentage <= 1) {
                    double current_alpha = static_cast<double>(in.alpha()) + (static_cast<double>(out.alpha()) - static_cast<double>(in.alpha())) * pourcentage;
                    double current_blue = static_cast<double>(in.blue()) + (static_cast<double>(out.blue()) - static_cast<double>(in.blue())) * pourcentage;
                    double current_green = static_cast<double>(in.green()) + (static_cast<double>(out.green()) - static_cast<double>(in.green())) * pourcentage;
                    double current_red = static_cast<double>(in.red()) + (static_cast<double>(out.red()) - static_cast<double>(in.red())) * pourcentage;
                    set_pixel(i, j, current_red, current_green, current_blue, current_alpha);
                }
            }
        }
    };

    // Apply a more complex horizontal gradient from the left to the right in the image
    void Image::apply_gradient_horizontal(Color left, Color right, int start_x, int end_x) {
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
    void Image::apply_gradient_vertical(Color top, Color bottom, int start_y, int end_y) {
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

    // Returns a shared ptr of the image with a new height, adaptated
    std::shared_ptr<Image> Image::resize_adaptative_height(unsigned short new_height) {
        if(new_height < height()) {
            // Create the new image
            std::shared_ptr<Image> new_image = std::make_shared<Image>(width(), new_height, Color(255, 255, 255));
            new_image.get()->a_flip_x_number = a_flip_x_number;

            std::vector<long long> repartioned_pixels = partition_number(height(), new_height);
            unsigned short current_y = 0;
            for(int i = 0;i<new_height;i++) {
                for(int j = 0;j<width();j++) {
                    // Calculate the median color
                    unsigned int alpha_value = 0;
                    unsigned int blue_value = 0;
                    unsigned int green_value = 0;
                    unsigned int red_value = 0;
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
                    new_image.get()->set_pixel(j, i, Color(red_value, green_value, blue_value, alpha_value));
                }
                current_y += repartioned_pixels[i];
            }
            return new_image;
        }
        return std::shared_ptr<Image>();
    };

    // Returns a shared ptr of the image with a new width, adaptated
    std::shared_ptr<Image> Image::resize_adaptative_width(unsigned short new_width) {
        if(new_width < width()) {
            // Create the new image
            std::shared_ptr<Image> new_image = std::make_shared<Image>(new_width, height(), Color(255, 255, 255));
            new_image.get()->a_flip_x_number = a_flip_x_number;

            std::vector<long long> repartioned_pixels = partition_number(width(), new_width);
            unsigned short current_x = 0;
            for(int i = 0;i<new_width;i++) {
                for(int j = 0;j<height();j++) {
                    // Calculate the median color
                    unsigned int alpha_value = 0;
                    unsigned int blue_value = 0;
                    unsigned int green_value = 0;
                    unsigned int red_value = 0;
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
        return std::shared_ptr<Image>();
    };

    // Load the image from a set of binary datas coming from a FreeType text
    bool Image::_load_from_text_binary(char* datas, int width, int height, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
        a_height = height; a_width = width;
        fill(0, 0, 0, 0);

        // Create each threads
        if(a_thread_number_for_pasting_text > 0) {
            unsigned int current_thread_position = 0;
            unsigned int pixel_by_thread = floor((static_cast<double>(width) * static_cast<double>(height)) / static_cast<double>(a_thread_number_for_pasting_text));

            std::vector<std::thread*> threads = std::vector<std::thread*>();
            for(unsigned short i = 0;i<a_thread_number_for_pasting_text - 1;i++) {
                unsigned int start_x = floor(current_thread_position % width);
                unsigned int start_y = floor(current_thread_position / width);

                std::thread* current_thread = new std::thread(&Image::__load_part_from_text_binary, this, datas, current_thread_position, start_x, start_y, pixel_by_thread, red, green, blue, alpha);
                threads.push_back(current_thread);
                current_thread_position += pixel_by_thread;
            }
            unsigned int start_x = floor(current_thread_position % width);
            unsigned int start_y = floor(current_thread_position / width);
            std::thread* current_thread = new std::thread(&Image::__load_part_from_text_binary, this, datas, current_thread_position, start_x, start_y, (static_cast<double>(width) * static_cast<double>(height)) - current_thread_position, red, green, blue, alpha);
            threads.push_back(current_thread);

            // Wait for each threads
            for(int i = 0;i<static_cast<int>(threads.size());i++) {
                threads[i]->join();
                delete threads[i]; threads[i] = 0;
            } threads.clear();
        }
        else {
            __load_part_from_text_binary(datas, 0, 0, 0, (static_cast<double>(width) * static_cast<double>(height)), red, green, blue, alpha);
        }

        return true;
    };

    // Load a part of image with a FreeType text in it
    bool Image::__load_part_from_text_binary(char* datas, int offset, int start_x, int start_y, int length, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) {
        if(color_type() == SCLS_IMAGE_RGBA) {
            for(int i = 0;i<length;i++) {
                double glyph_alpha = (datas[offset + i] / 255.0);
                set_pixel(start_x, start_y, red, green, blue, static_cast<unsigned short>(glyph_alpha * static_cast<double>(alpha)));

                start_x++;
                if(start_x >= width()) {
                    start_x = 0;
                    start_y++;
                }
            }
        }
        else if(color_type() == SCLS_IMAGE_RGB) {
            for(int i = 0;i<length;i++) {
                set_pixel(start_x, start_y, red, green, blue, alpha);

                start_x++;
                if(start_x >= width()) {
                    start_x = 0;
                    start_y++;
                }
            }
        }
        return true;
    };
}
