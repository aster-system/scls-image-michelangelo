//******************
//
// scls_image_text.cpp
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
// This file contains the source code of scls_image_text.h.
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
    // List of all the fonts in the system
	std::map<std::string, Font> __system_fonts = std::map<std::string, Font>();

	// Returns if a certain system font is stored in the program or not
    bool contains_system_font(std::string font) {for(std::map<std::string, Font>::iterator it = __system_fonts.begin();it!=__system_fonts.end();it++) {if(it->first == font) return true;}return false;};

    // Load all the fonts system fonts
    void load_system_font() {
        __system_fonts.clear();
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
                if(contains_string(descriptor, "Bold"))
                {
                    font.bold = true;
                    last_name += "b";
                }
                if(contains_string(descriptor, "Italic"))
                {
                    font.italic = true;
                    last_name += "i";
                }
                if(contains_string(descriptor, "Condensed"))
                {
                    font.condensed = true;
                    last_name += "c";
                }
                if(contains_string(descriptor, "Light"))
                {
                    font.light = true;
                    last_name += "l";
                }
                font.font_family = cutted[0];
                font_name = cutted[0];
            }
            __system_fonts[font_name + last_name] = font;
        }
	};

	// Return the system path of a font
    Font get_system_font(std::string font, bool bold, bool italic, bool condensed, bool light) {
	    if(__system_fonts.size() <= 0) load_system_font();

	    std::string last_name = "";
	    if(bold) last_name += "b";
	    if(italic) last_name += "i";
	    if(condensed) last_name += "c";
	    if(light) last_name += "l";

	    if(!contains_system_font(font + last_name)) {
            print("Error", "SCLS Image Michelangelo", "The \"" + font + last_name + "\" system font you want to get does not exist.");
            return __system_fonts[DEFAULT_FONT];
	    }

        return __system_fonts[font + last_name];
	};

	// Print each system fonts
    void print_system_fonts() {
	    if(__system_fonts.size() <= 0) load_system_font();

	    for(std::map<std::string, Font>::iterator it = __system_fonts.begin();it!=__system_fonts.end();it++)
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

    //*********
	//
	// Text creation
	//
	//*********

	// Hiddens variables
    FT_Library  _freetype_library;
    bool _free_type_library_inited = false;

    // Return the width of a char
    unsigned short _char_width(char character, FT_Face& face) {
        // Configure and load the FreeType glyph system
        FT_UInt index = FT_Get_Char_Index(face, (static_cast<unsigned char>(character)));
        //FT_Error error =
        FT_Load_Glyph(face, index, FT_LOAD_NO_BITMAP);
        FT_GlyphSlot binary_datas = face->glyph;
        //error =
        FT_Render_Glyph(binary_datas, FT_RENDER_MODE_NORMAL);

        return static_cast<unsigned short>(binary_datas->bitmap.width);
    };

    // Return a pointer to an image with a char on it
    Image* _char_image(unsigned int character, FT_Face& face, int& cursor_pos, unsigned int& real_width, int& y_pos, Color color) {
        // Configure and load the FreeType glyph system
        FT_UInt index = FT_Get_Char_Index(face, character);
        //FT_Error error =
        FT_Load_Glyph(face, index, 0);
        FT_GlyphSlot binary_datas = face->glyph;
        //error =
        FT_Render_Glyph(binary_datas, FT_RENDER_MODE_NORMAL);

        // Create and draw the image
        unsigned short height = static_cast<unsigned short>(binary_datas->bitmap.rows);
        unsigned short width = static_cast<unsigned short>(binary_datas->bitmap.width);
        Image* img = new Image();
        img->_load_from_text_binary(reinterpret_cast<char*>(binary_datas->bitmap.buffer), width, height, color.red(), color.green(), color.blue(), color.alpha());

        // Get the position of the cursor
        cursor_pos = binary_datas->bitmap_left;
        real_width = static_cast<unsigned int>(static_cast<double>(binary_datas->advance.x) / 64.0);
        y_pos = binary_datas->bitmap_top - height;

        return img;
    };

    //*********
	//
	// Hidden classes to handle text
	//
	//*********

	// Load the built-ins balises
    void _Balise_Style_Container::_load_built_in_balises() {
        std::shared_ptr<Balise_Style_Datas> current_balise = std::make_shared<Balise_Style_Datas>();
        // Create the <br> style
        current_balise.get()->is_break_line = true;
        set_defined_balise("br", current_balise);
        // Create the <div> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->is_paragraph = true;
        set_defined_balise<Balise_Style_Datas>("div", current_balise);
        // Create the <p> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->is_paragraph = true;
        set_defined_balise<Balise_Style_Datas>("p", current_balise);
        // Create the <h1> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->is_paragraph = true;
        current_balise.get()->style.alignment_horizontal = Alignment_Horizontal::H_Center;
        current_balise.get()->style.color = Color(255, 0, 0); current_balise.get()->style.font_size = 50; current_balise.get()->style.font = get_system_font("arialbd");
        set_defined_balise<Balise_Style_Datas>("h1", current_balise);
        // Create the <h2> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->is_paragraph = true;
        current_balise.get()->style.alignment_horizontal = Alignment_Horizontal::H_Left;
        current_balise.get()->style.color = Color(0, 0, 0); current_balise.get()->style.font_size = 35; current_balise.get()->style.font = get_system_font("arialbd");
        set_defined_balise<Balise_Style_Datas>("h2", current_balise);

        // Mathematicals styles

        // Create the <frac> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        current_balise.get()->style.color = Color(0, 0, 0);
        set_defined_balise("frac", current_balise);
        // Create the <math> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        current_balise.get()->style.color = Color(0, 0, 0); current_balise.get()->style.font_size = 40;
        set_defined_balise<Balise_Style_Datas>("math", current_balise);
        // Create the <mi> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        current_balise.get()->style.color = Color(0, 0, 0);
        set_defined_balise("mi", current_balise);
        // Create the <mo> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        current_balise.get()->style.color = Color(0, 0, 0);
        set_defined_balise("mo", current_balise);
        // Create the <mrow> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        current_balise.get()->style.color = Color(0, 0, 0);
        set_defined_balise("mrow", current_balise);
        // Create the <msup> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        current_balise.get()->style.color = Color(0, 0, 0);
        set_defined_balise("msup", current_balise);
        // Create the <sub> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        current_balise.get()->style.color = Color(0, 0, 0);
        set_defined_balise("sub", current_balise);
        // Create the <vec> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        current_balise.get()->style.color = Color(0, 0, 0);
        set_defined_balise("vec", current_balise);
    }

    // Returns the position in the text at a X position
    unsigned int Word_Datas::cursor_position_at_x(int x_position) {
        if(content().size() == 0 && a_characters_position.size() >= 0) return 0;

        int word_position = 0;
        while(word_position < static_cast<int>(a_characters_position.size()) && static_cast<int>(a_characters_position[word_position] + a_characters_width[word_position]) <= x_position) {
            word_position++;
        }

        if(word_position >= static_cast<int>(a_characters_position.size())) return content().size();
        if(static_cast<int>(x_position) - static_cast<int>(a_characters_position[word_position]) > static_cast<int>(a_characters_width[word_position] + a_characters_position[word_position]) - static_cast<int>(x_position)) {
            word_position++;
        }

        return word_position;
    };

    // Generate the image of the word
    void Text_Image_Word::generate_word() {
        // Base variables for the creation
        std::string path = style().font.font_path;
        if(path == ""){path = get_system_font(DEFAULT_FONT).font_path;}

        // Load the FreeType base system
        if(!_free_type_library_inited) {
            FT_Error error = FT_Init_FreeType(&_freetype_library);
            if ( error ) {
                print("Error", "SCLS", "Unable to load the FreeType engine.");
                return;
            }
            _free_type_library_inited = true;
        }
        FT_Face face;
        FT_Error error = FT_New_Face(_freetype_library, path.c_str(), 0, &face);
        if (!std::filesystem::exists(path)) {
            print("Error", "SCLS", "Unable to load the \"" + path + "\" font, it does not exist.");
            return;
        }
        else if ( error ) {
            print("Error", "SCLS", "Unable to load the \"" + path + "\" font.");
            return;
        }

        // Configure and load the FreeType glyph system
        error = FT_Select_Charmap(face, FT_ENCODING_UNICODE);
        error = FT_Set_Pixel_Sizes(face, 0, style().font_size);

        // Create each characters
        std::vector<Image*> characters;
        std::vector<unsigned int> characters_width;
        std::vector<int> characters_x;
        std::vector<int> cursor_pos;
        unsigned int final_character = 0; int current_character_level = 0;
        int max_height = 0;
        short min_bottom_position = 0;
        int to_add_font_size = 0;
        unsigned int total_width = 0;
        std::vector<unsigned int> y_pos;
        const std::string word = a_datas.content();
        for(int i = 0;i<static_cast<int>(word.size());i++) {
            // Check the current character
            unsigned char current_character = word.at(i);
            if(utf_8_level(current_character) == 0 || current_character_level == 1) {
                // Generate each characters
                if(final_character != 0){current_character &= 0b00111111;}
                unsigned int to_compare_as_character = current_character;
                final_character |= to_compare_as_character;
                int cursor_position = 0;
                unsigned int real_width = 0;
                int y_position = 0;
                Image* image = _char_image(final_character, face, cursor_position, real_width, y_position, style().color);
                characters.push_back(image); characters_x.push_back(total_width);
                cursor_pos.push_back(total_width + cursor_position);
                if(cursor_pos[cursor_pos.size() - 1] < 0) cursor_pos[cursor_pos.size() - 1] = 0; // Avoid a little bug with X position
                // Calculate the position for the character
                if(static_cast<int>(image->height()) + y_position > max_height) max_height = image->height() + y_position;
                if(y_position < to_add_font_size) to_add_font_size = y_position;
                y_pos.push_back(y_position);
                if(y_position < min_bottom_position){min_bottom_position = y_position;}
                total_width += real_width;
                characters_width.push_back(real_width);
                final_character = 0; current_character_level = 0;
            } else if(utf_8_level(current_character) == 1) {
                // Needs an UTF-8 leveling
                current_character &= 0b00011111;
                unsigned int to_compare_as_character = current_character;to_compare_as_character = to_compare_as_character << 6;
                final_character |= to_compare_as_character;
                current_character_level = 1;
            }
        }
        a_datas.set_bottom_offset(min_bottom_position);
        a_datas.set_characters_position(characters_x);
        a_datas.set_characters_width(characters_width);
        a_datas.set_top_offset(max_height - style().font_size);

        // Inverse each positions
        for(int i = 0;i<static_cast<int>(characters.size());i++) {
            if(characters[i] != 0) {
                y_pos[i] = max_height - (characters[i]->height() + y_pos[i]);
            }
        }

        // Create the final image and clear the memory
        a_datas.set_width(total_width);
        a_last_image.reset(new Image(total_width, max_height - to_add_font_size, Color(0, 0, 0, 0)));
        unsigned char max_thread_number = 0;
        std::vector<unsigned int> positions = std::vector<unsigned int>();
        std::vector<std::thread*> threads = std::vector<std::thread*>();
        for(int i = 0;i<static_cast<int>(characters.size());i++) {
            if(characters[i] != 0) {
                if(max_thread_number > 0) {
                    if(threads.size() >= max_thread_number) {
                        for(int i = 0;i<static_cast<int>(threads.size());i++) {
                            threads[i]->join();
                            delete threads[i];
                        } threads.clear();
                    }

                    std::thread* current_thread = new std::thread(&Text_Image_Word::__word_letter, this, a_last_image.get(), characters[i], cursor_pos[i], y_pos[i]);
                    threads.push_back(current_thread);
                }
                else {
                    __word_letter(a_last_image.get(), characters[i], cursor_pos[i], y_pos[i]);
                }
            }
        }

        // Join each threads
        for(int i = 0;i<static_cast<int>(threads.size());i++) {
            threads[i]->join();
            delete threads[i];
        }
        FT_Done_Face(face);
    };

    // Returns the position of the cursor in plain text with a X position
    int Text_Image_Line::cursor_position_in_plain_text_at_x(int x_position) {
        if(a_words_datas.size() == 0) return 0;

        int word_position = 0;
        int current_position = 0;
        while(word_position < static_cast<int>(a_words_datas.size()) && static_cast<int>(a_words_datas[word_position].x_position() + a_words_datas[word_position].width()) < x_position) {
            current_position += a_words_datas[word_position].content().size(); word_position++;
        }
        if(word_position >= static_cast<int>(a_words_datas.size())) return current_position;

        Word_Datas& word_to_apply = a_words_datas[word_position];

        return current_position + word_to_apply.cursor_position_at_x(x_position - word_to_apply.x_position());
    };

    // Generates the needed maths for a word
    std::shared_ptr<__Math_Part_Image> __generate_frac(std::shared_ptr<XML_Text> content, const Text_Style& current_style, Text_Image_Line* needed_line) {
        // Asserts
        if(content.get()->sub_texts().size() <= 0){return std::shared_ptr<__Math_Part_Image>();}
        else if(content.get()->sub_texts().size() == 1){return needed_line->generate_maths(content.get()->sub_texts()[0], current_style);}

        // Draw the needed image
        Text_Style needed_style = current_style;
        std::shared_ptr<__Math_Part_Image> denominator = needed_line->generate_maths(content.get()->sub_texts()[1], needed_style);
        std::shared_ptr<__Math_Part_Image> numerator = needed_line->generate_maths(content.get()->sub_texts()[0], needed_style);
        // Draw the image to return
        int bar_width = current_style.font_size / 10;
        int max_width = denominator.get()->image.get()->width(); if(numerator.get()->image.get()->width() > max_width){max_width = numerator.get()->image.get()->width();}
        int needed_size = denominator.get()->image.get()->height() + numerator.get()->image.get()->height() + bar_width * 3;
        std::shared_ptr<__Math_Part_Image> to_return = std::make_shared<__Math_Part_Image>();
        std::shared_ptr<Image>& image = to_return.get()->image;
        image = std::make_shared<Image>(max_width, needed_size, current_style.background_color);
        // Paste the needed image
        int bar_length_offset = 1;
        image.get()->fill_rect(bar_length_offset, numerator.get()->image.get()->height() + bar_width, image.get()->width() - bar_length_offset * 2, bar_width, current_style.color);
        image.get()->paste(denominator.get()->image.get(), image.get()->width() / 2 - denominator.get()->image.get()->width() / 2, image.get()->height() - denominator.get()->image.get()->height());
        image.get()->paste(numerator.get()->image.get(), image.get()->width() / 2 - numerator.get()->image.get()->width() / 2, 0);
        to_return.get()->middle_bottom_offset = denominator.get()->image.get()->height() + bar_width + std::ceil(bar_width / 2);
        to_return.get()->middle_top_offset = numerator.get()->image.get()->height() + bar_width + std::floor(bar_width / 2);
        return to_return;
    }
    std::shared_ptr<__Math_Part_Image> __generate_nabla(const Text_Style& current_style) {
        // Draw the nabla
        int needed_size = current_style.font_size;
        std::shared_ptr<__Math_Part_Image> to_return = std::make_shared<__Math_Part_Image>();
        std::shared_ptr<Image>& image = to_return.get()->image;
        image = std::make_shared<Image>(needed_size, needed_size, current_style.background_color);
        // Draw the triangle
        double line_width = static_cast<double>(needed_size) / 10.0;
        int first_line_width = 2;
        int first_x = 0; int first_y = 0;
        int second_x = needed_size / 2; int second_y = needed_size;
        int third_x = needed_size - 1; int third_y = 0;
        for(int i = 0;i<static_cast<int>(first_line_width);i++){image.get()->draw_line(first_x + i * line_width, first_y, second_x, second_y - i * line_width / 2.0, current_style.color, line_width);}
        for(int i = 0;i<static_cast<int>(first_line_width);i++){image.get()->draw_line(first_x + i * line_width / 2.0, first_y + i * line_width, third_x - i * line_width / 2.0, third_y + i * line_width, current_style.color, line_width);}
        image.get()->draw_line(second_x, second_y, third_x, third_y, current_style.color, line_width);
        to_return.get()->middle_bottom_offset = std::ceil(image.get()->height() / 2);
        to_return.get()->middle_top_offset = std::floor(image.get()->height() / 2);
        return to_return;
    }
    std::shared_ptr<__Math_Part_Image> __generate_sub(std::shared_ptr<XML_Text> content, const Text_Style& current_style, Text_Image_Line* needed_line) {
        // Draw the needed image
        Text_Style needed_style = current_style; needed_style.font_size /= 2;
        std::shared_ptr<__Math_Part_Image> needed_part = needed_line->generate_maths(content, needed_style);
        std::shared_ptr<Image>& needed_image = needed_part.get()->image;
        // Draw the image to return
        int needed_size = current_style.font_size;
        std::shared_ptr<__Math_Part_Image> to_return = std::make_shared<__Math_Part_Image>();
        std::shared_ptr<Image>& image = to_return.get()->image;
        image = std::make_shared<Image>(needed_image.get()->width(), needed_size, current_style.background_color);
        // Paste the needed image
        image.get()->paste(needed_image.get(), 0, image.get()->height() - needed_image.get()->height());
        to_return.get()->middle_bottom_offset = std::ceil(image.get()->height() / 2);
        to_return.get()->middle_top_offset = std::floor(image.get()->height() / 2);
        return to_return;
    }
    std::shared_ptr<__Math_Part_Image> __generate_sup(std::shared_ptr<XML_Text> content, const Text_Style& current_style, Text_Image_Line* needed_line) {
        // Draw the needed image
        Text_Style needed_style = current_style; needed_style.font_size /= 2;
        std::shared_ptr<__Math_Part_Image> needed_part = needed_line->generate_maths(content, needed_style);
        std::shared_ptr<Image>& needed_image = needed_part.get()->image;
        // Draw the image to return
        int needed_size = current_style.font_size;
        std::shared_ptr<__Math_Part_Image> to_return = std::make_shared<__Math_Part_Image>();
        std::shared_ptr<Image>& image = to_return.get()->image;
        image = std::make_shared<Image>(needed_image.get()->width(), needed_size, current_style.background_color);
        // Paste the needed image
        image.get()->paste(needed_image.get(), 0, 0);
        to_return.get()->middle_bottom_offset = std::ceil(image.get()->height() / 2);
        to_return.get()->middle_top_offset = std::floor(image.get()->height() / 2);
        return to_return;
    }
    std::shared_ptr<__Math_Part_Image> __generate_text_for_maths(std::string text, const Text_Style& current_style, Text_Image_Line* line) {
        std::shared_ptr<Text_Image_Word> needed_word = line->_generate_word(text, current_style, 0);
        if(needed_word.get() != 0){
            std::shared_ptr<__Math_Part_Image> to_add = std::make_shared<__Math_Part_Image>();
            to_add.get()->image = needed_word.get()->image_shared_pointer();
            to_add.get()->middle_top_offset = std::floor(static_cast<double>(to_add.get()->image.get()->height() + needed_word.get()->bottom_offset()) / 2.0);
            to_add.get()->middle_bottom_offset = to_add.get()->image.get()->height() - to_add.get()->middle_top_offset;
            return to_add;
        } return std::shared_ptr<__Math_Part_Image>();
    };
    std::shared_ptr<__Math_Part_Image> __generate_vector(std::shared_ptr<__Math_Part_Image> base_image, const Text_Style& current_style) {
        // Draw the nabla
        int needed_size = current_style.font_size / 2;
        std::shared_ptr<__Math_Part_Image> to_return = std::make_shared<__Math_Part_Image>();
        std::shared_ptr<Image>& image = to_return.get()->image;
        to_return.get()->bottom_offset = base_image.get()->bottom_offset;
        to_return.get()->top_offset = needed_size;
        image = std::make_shared<Image>(current_style.font_size, base_image.get()->image.get()->height() + needed_size, current_style.background_color);
        // Paste the image
        image.get()->paste(base_image.get()->image.get(), current_style.font_size / 2 - base_image.get()->image.get()->width() / 2, needed_size);
        // Draw the vector
        image.get()->draw_arrow(image.get()->width() / 8, needed_size / 2, image.get()->width() - image.get()->width() / 8, needed_size / 2, current_style.color, 0.4, 2);
        to_return.get()->middle_top_offset = needed_size + std::floor(base_image.get()->image.get()->height() / 2.0);
        to_return.get()->middle_bottom_offset = image.get()->height() - to_return.get()->middle_top_offset;
        return to_return;
    }
    void __generate_maths_one_balise(std::string needed_balise_name, int& bottom_offset, std::shared_ptr<XML_Text> content, const Text_Style& current_style, int& needed_height, int& needed_middle_bottom_offset, int& needed_middle_top_offset, std::vector<std::shared_ptr<__Math_Part_Image>>& needed_parts, int& needed_width, int& top_offset, Text_Image_Line* line) {
        std::shared_ptr<__Math_Part_Image> needed_part;
        if(needed_balise_name == "epsilon") {
            // Generate a pi sign
            std::string text = std::string(""); add_utf_8(text, 949);
            needed_part = __generate_text_for_maths(text, current_style, line);
        } else if(needed_balise_name == "esh") {
            // Generate a pi sign
            std::string text = std::string(""); add_utf_8(text, 643);
            needed_part = __generate_text_for_maths(text, current_style, line);
        } else if(needed_balise_name == "frac") {
            // Generate a fraction
            needed_part = __generate_frac(content, current_style, line);
        } else if(needed_balise_name == "mdelta") {
            // Generate a delta text
            std::string text = std::string(""); add_utf_8(text, 916);
            needed_part = __generate_text_for_maths(text, current_style, line);
        } else if(needed_balise_name == "msup") {
            // Generate a sub text
            needed_part = __generate_sup(content, current_style, line);
        } else if(needed_balise_name == "mu") {
            // Generate a mu sign
            std::string text = std::string(""); add_utf_8(text, 956);
            needed_part = __generate_text_for_maths(text, current_style, line);
        } else if(needed_balise_name == "nabla") {
            // Generate a nabla sign
            needed_part = __generate_nabla(current_style);
        } else if(needed_balise_name == "phi") {
            // Generate a pi sign
            std::string text = std::string(""); add_utf_8(text, 632);
            needed_part = __generate_text_for_maths(text, current_style, line);
        } else if(needed_balise_name == "pi") {
            // Generate a pi sign
            std::string text = std::string(""); add_utf_8(text, 960);
            needed_part = __generate_text_for_maths(text, current_style, line);
        } else if(needed_balise_name == "rho") {
            // Generate a pi sign
            std::string text = std::string(""); add_utf_8(text, 961);
            needed_part = __generate_text_for_maths(text, current_style, line);
        } else if(needed_balise_name == "sub") {
            // Generate a sub text
            needed_part = __generate_sub(content, current_style, line);
        } else if(needed_balise_name == "vec") {
            // Generate a vec sign
            needed_part = __generate_vector(line->generate_maths(content, current_style), current_style);
        } else {
            // Generate a vec sign
            needed_part = __generate_text_for_maths(content.get()->text(), current_style, line);
        }

        // Validation of the image
        if(needed_part.get() != 0) {
            needed_part.get()->balise = content.get()->xml_balise();
            // Handle height and width
            if(needed_middle_bottom_offset < needed_part.get()->middle_bottom_offset){needed_middle_bottom_offset = needed_part.get()->middle_bottom_offset;}
            if(needed_middle_top_offset < needed_part.get()->middle_top_offset){needed_middle_top_offset = needed_part.get()->middle_top_offset;}
            needed_width += needed_part.get()->image.get()->width();
            // Handle offsets
            needed_parts.push_back(needed_part);
        }
    };
    std::shared_ptr<__Math_Part_Image> Text_Image_Line::generate_maths(std::shared_ptr<XML_Text> content, Text_Style current_style) {
        // Cut the block
        std::vector<std::shared_ptr<__Math_Part_Image>> needed_parts = std::vector<std::shared_ptr<__Math_Part_Image>>();
        int bottom_offset = 0; int needed_height = 0; int needed_width = 0; int top_offset = 0;
        int middle_bottom_offset = 0; int middle_top_offset = 0;
        std::shared_ptr<__Math_Part_Image> to_return = std::make_shared<__Math_Part_Image>();

        if(content.get()->only_text()) {
            // If the content is only a text
        } else {
            // Analyse each blocks
            for(int i = 0;i<static_cast<int>(content.get()->sub_texts().size());i++) {
                std::string needed_balise_name = content.get()->sub_texts()[i].get()->xml_balise_name();
                __generate_maths_one_balise(needed_balise_name, bottom_offset, content.get()->sub_texts()[i], current_style, needed_height, middle_bottom_offset, middle_top_offset, needed_parts, needed_width, top_offset, this);
            }
        }

        // Returns the image
        int current_x = 0; int current_y = 0;
        int final_height = middle_bottom_offset + middle_top_offset;
        std::shared_ptr<Image>& image = to_return.get()->image;
        image = std::make_shared<Image>(needed_width, final_height, current_style.background_color);
        to_return.get()->middle_bottom_offset = middle_bottom_offset; to_return.get()->middle_top_offset = middle_top_offset;
        for(int i = 0;i<static_cast<int>(needed_parts.size());i++) {
            // Paste the image
            // Calculate the Y position
            current_y = middle_top_offset - needed_parts[i].get()->middle_top_offset;
            image.get()->paste(needed_parts[i].get()->image.get(), current_x, current_y);
            // Update the image
            current_x += needed_parts[i].get()->image.get()->width();
            needed_parts[i].reset();
        } return to_return;
    }

    // Generates the needed words (and balises)
    void __generate_image(std::shared_ptr<Text_Image_Word>& word, std::shared_ptr<Image> resize_image, unsigned int& current_position_in_plain_text, int& current_width, int height, int width) {
        // Create the image
        word = std::make_shared<Text_Image_Word>(String(std::string()));
        if(resize_image.get()->width() > width){resize_image = resize_image.get()->resize_adaptative_width(width);}
        if(resize_image.get()->height() > height){resize_image = resize_image.get()->resize_adaptative_height(height);}
        word.get()->set_image_shared_ptr(resize_image);
        word.get()->set_x_position(current_width);
        current_position_in_plain_text++;
        current_width += resize_image.get()->width();
    }
    void Text_Image_Line::generate_words() {
        // Create the needed configurations
        Text_Style current_style = a_global_style;

        // Create each words
        free_memory();
        unsigned int current_position_in_plain_text = 0;
        int& current_width = a_current_width; current_width = 0;
        std::vector<_Text_Balise_Part> cutted = a_defined_balises->__cut_block(text());
        std::vector<std::shared_ptr<Text_Image_Word>>& words = a_words;
        short& y_offset = a_y_offset;
        for(int i = 0;i<static_cast<int>(cutted.size());i++) {
            std::shared_ptr<Text_Image_Word> word_to_add;
            Word_Datas data_to_add = Word_Datas();
            if(cutted[i].balise_content.size() > 0 && cutted[i].balise_content[0] == '<') {
                // Apply a balise
                std::string balise = cutted[i].balise_content;
                if(balise.size() <= 0 || balise[balise.size() - 1] != '>') {
                    print("Warning", "SCLS Image \"Michelangelo\"", "A balise you want to parse is badly syntaxed.");
                    continue;
                }

                std::string balise_content = formatted_balise(balise);
                std::string current_balise_name = balise_name(balise_content);
                if(current_balise_name == "img") {
                    // The balise is an image
                    std::vector<std::string> attributes = cut_balise_by_attributes(balise_content);
                    std::string source = "";
                    int height = -1; int width = -1;
                    for(int i = 0;i<static_cast<int>(attributes.size());i++) {
                        std::string &current_attribute = attributes[i];
                        if(attribute_name(current_attribute) == "src") {
                            // Source of the image
                            source = attribute_value(current_attribute);
                            if(source.size() > 0 && source[0] == '\"') source = source.substr(1, source.size() - 1);
                            if(source.size() > 0 && source[source.size() - 1] == '\"') source = source.substr(0, source.size() - 1);
                        } else if(attribute_name(current_attribute) == "height") {
                            // Height of the image
                            height = Fraction::from_std_string(attribute_value(current_attribute)).to_double();
                        } else if(attribute_name(current_attribute) == "width") {
                            // Width of the image
                            width = Fraction::from_std_string(attribute_value(current_attribute)).to_double();
                        }
                    }

                    // Load the image from the source
                    if(source.size() > 0) {
                        std::shared_ptr<Image>* src_img = 0;
                        if(source[0] == '#') {
                            // The image is a preload image
                            source = source.substr(1, source.size() - 1);
                            src_img = a_defined_balises->image(source);
                            if(src_img != 0) {
                                // Set the size
                                if(height == -1 && width == -1) {height = src_img->get()->height();width = src_img->get()->width();}
                            }
                            else {
                                print("Warning", "SCLS Image \"Michelangelo\"", "The \"" + source + "\" <img> you want to load does not exist.");
                            }
                        }
                        else {
                            // The image is an unloaded image
                            if(std::filesystem::exists(source)) {src_img = a_defined_balises->add_image(source, source);}
                            if(src_img != 0) {
                                // Set the size
                                if(height == -1 && width == -1) {height = src_img->get()->height();width = src_img->get()->width();}
                                else if(height != -1 && width == -1) {
                                    width = static_cast<int>(static_cast<double>(height) * (static_cast<double>(src_img->get()->width()) / static_cast<double>(src_img->get()->height())));
                                } else if(height == -1 && width != -1) {
                                    height = static_cast<int>(static_cast<double>(width) * (static_cast<double>(src_img->get()->height()) / static_cast<double>(src_img->get()->width())));
                                }
                                // Last update of the size
                                if(current_style.max_width > 0 && width > current_style.max_width) {
                                    width = current_style.max_width;
                                    height = static_cast<int>(static_cast<double>(width) * (static_cast<double>(src_img->get()->height()) / static_cast<double>(src_img->get()->width())));
                                }
                            }
                            else {
                                print("Warning", "SCLS Image \"Michelangelo\"", "The \"" + source + "\" <img> path you want to load does not exist.");
                            }
                        }

                        // Create the image
                        if(src_img != 0){__generate_image(word_to_add, *src_img, current_position_in_plain_text, current_width, height, width);}
                    }
                }
                else if(current_balise_name == "math") {
                    // The balise is a piece of mathml
                    int temp_font_size = current_style.font_size;
                    current_style.font_size = 40; // EXPERIMENTAL
                    std::shared_ptr<Image> src_img = generate_maths(cutted[i].content, current_style).get()->image;
                    current_style.font_size = temp_font_size;
                    __generate_image(word_to_add, src_img, current_position_in_plain_text, current_width, src_img.get()->height(), src_img.get()->width());
                    word_to_add.get()->set_balise_content(balise_content);
                }
            }
            else {
                // Draw the image
                std::string word_content = format_string_as_plain_text(cutted[i].content);
                word_to_add = _generate_word(word_content, current_style, current_position_in_plain_text);
                if(word_to_add.get() != 0) {
                    word_to_add.get()->set_x_position(current_width);
                    if(word_to_add.get()->bottom_offset() < y_offset) y_offset = word_to_add.get()->bottom_offset();
                }
                current_position_in_plain_text += word_content.size();
            }
            if(word_to_add.get() != 0){data_to_add = word_to_add.get()->datas();}

            // Add the part
            words.push_back(word_to_add);
            a_words_datas.push_back(data_to_add);
        }
    };

    // Generate a simple word
    std::shared_ptr<Text_Image_Word> Text_Image_Line::_generate_word(const std::string& word, const Text_Style& style, unsigned int start_position_in_plain_text) {
        // Create the word
        Word_Datas datas = Word_Datas(word, style);
        std::shared_ptr<Text_Image_Word> word_to_add = std::make_shared<Text_Image_Word>(datas);
        word_to_add.get()->generate_word();

        return word_to_add;
    };

    // Generates and returns an image of the line
    Image* Text_Image_Line::image(Image_Generation_Type generation_type) {
        if(a_last_image.get() != 0 && generation_type == Image_Generation_Type::IGT_Full) return a_last_image.get();

        // Generate the words
        if(generation_type == Image_Generation_Type::IGT_Full) {
            generate_words();
        } place_words();

        // Check for the cursor
        unsigned int cursor_width = 2;
        if(use_cursor() && cursor_position_in_plain_text() >= 0 && cursor_position_in_plain_text() <= static_cast<int>(datas().content_in_plain_text.size())) {
            if(static_cast<int>(a_words.size()) <= 0) {
                a_current_width = cursor_width;
            }
        }

        // Draw the line
        Text_Style current_style = a_global_style;
        int current_position_in_plain_text = 0;
        int current_x = 0;
        int current_line = 1;int current_y = a_line_height[0];
        int max_width = a_global_style.max_width;
        unsigned short space_width = static_cast<unsigned short>(static_cast<double>(global_style().font_size) / 2.0);
        int needed_height = total_height() - a_y_offset;
        if(max_width > 0 && a_line_height.size() > 1) {
            a_last_image.reset(new Image(max_width, needed_height, current_style.background_color));
        } else {
            a_last_image.reset(new Image(a_current_width, needed_height, current_style.background_color));
        }
        Image* final_image = a_last_image.get();
        // Generate each words
        for(int i = 0;i<static_cast<int>(a_words.size());i++) {
            Text_Image_Word* current_word = a_words[i].get();
            if(current_word != 0) {
                Image* current_image = current_word->image();
                if(current_image != 0 && current_image->width() > 0) {
                    // Check for the cursor
                    if(use_cursor() && cursor_position_in_plain_text() >= 0 && cursor_position_in_plain_text() <= static_cast<int>(datas().content_in_plain_text.size())) {
                        if(cursor_position_in_plain_text() >= current_position_in_plain_text && cursor_position_in_plain_text() <= current_position_in_plain_text + static_cast<int>(current_word->text().size())) {
                            int local_cursor_position = cursor_position_in_plain_text() - current_position_in_plain_text;
                            if(local_cursor_position == static_cast<int>(current_word->text().size())) {
                                a_cursor_x = current_x + current_image->width() - static_cast<double>(cursor_width) / 2.0;
                            }
                            else {
                                a_cursor_x = current_x + current_word->datas().characters_position()[local_cursor_position];
                            }
                        }
                    }

                    // Check the max width of the line
                    if(max_width > 0 && current_x + current_image->width() > max_width) {
                        current_x = 0;
                        current_y+=a_line_height[current_line];
                        current_line++;

                        // Check if the line start with a space
                        if(current_word->text() == std::string(" ")){continue;}
                    }

                    // Check the Y of the image
                    int y = current_y - (static_cast<int>(current_image->height()) + static_cast<int>(current_word->bottom_offset()));
                    if(balise_name(current_word->datas().balise_content()) == "math"){y = final_image->height() / 2.0 - static_cast<double>(current_image->height()) / 2.0;};

                    // Paste the word
                    final_image->paste(current_image, current_x, y);
                    current_word->set_x_position(current_x); a_words_datas[i].set_x_position(current_x);
                    current_x += current_image->width();
                    current_position_in_plain_text += current_word->text().size();

                }
            }
        } //clear_words();

        // Draw the cursor
        if(cursor_position_in_plain_text() == 0) a_cursor_x = 0;
        else a_cursor_x -= static_cast<double>(cursor_width) / 2.0;
        if(use_cursor() && cursor_position_in_plain_text() >= 0 && cursor_position_in_plain_text() <= static_cast<int>(datas().content_in_plain_text.size())) {
            final_image->fill_rect(a_cursor_x, 0, cursor_width, final_image->height(), a_global_style.color);
        }

        return final_image;
    }

    // Place the words as needed
    void Text_Image_Line::place_words() {
        a_line_height.clear();
        Text_Style current_style = a_global_style;
        int current_height = current_style.font_size;
        int& current_width = a_current_width; current_width = 0;
        for(int i = 0;i<static_cast<int>(a_words.size());i++) {
            // Check the max width
            if(a_words[i].get() == 0 || (a_words[i].get()->text() == std::string("") && current_width == 0 && a_line_height.size() > 0)){continue;}
            int image_height = a_words[i].get()->image()->height();
            int image_width = a_words[i].get()->image()->width();
            if(current_style.max_width > 0 && current_width + image_width > current_style.max_width) {
                a_line_height.push_back(current_height);
                current_height = current_style.font_size;current_width = 0;
            } if(image_height>current_height){current_height=image_height;} current_width += image_width;
        } a_line_height.push_back(current_height);
    };

    //*********
	//
	// Text classes
	//
	//*********

	// Returns the position of the cursor in plain text with a X position and a line
    unsigned int Text_Image_Block::cursor_position_in_plain_text_at_line_and_x(unsigned int line, int x_position) {
        if(line < 0 || line >= lines().size()) return 0;

        Text_Image_Line* line_to_check = lines()[line];
        if(line_to_check == 0) return 0;
        return line_to_check->datas().start_position_in_plain_text + line_to_check->cursor_position_in_plain_text_at_x(x_position);
    }

    // Generate the lines of the block
    Text_Image_Line* Text_Image_Block::_generate_line(Line_Datas datas, const Text_Style& style) {
        // Create the line
        Text_Image_Line* to_return = new Text_Image_Line(a_defined_balises, datas.content, style);
        to_return->set_datas(datas);

        // Handle the cursor
        if(use_cursor()) {
            if(cursor_position_in_plain_text() >= datas.start_position_in_plain_text && cursor_position_in_plain_text() <= datas.start_position_in_plain_text + datas.content_in_plain_text.size()) {
                to_return->set_cursor_position_in_plain_text(cursor_position_in_plain_text() - datas.start_position_in_plain_text);
                to_return->set_use_cursor(true);

                a_cursor_line = to_return;
            }
        }

        return to_return;
    };

    // Generates the next line to be generated and returns it
    Text_Image_Line* Text_Image_Block::generate_next_line(unsigned int line_number) {
        // Cut the text by line and delete useless lines
        Text_Style& current_style = a_current_style;
        std::vector<Line_Datas>& cutted = a_lines_text;
        std::vector<Text_Image_Line*>& lines = a_lines;
        int& max_width = a_datas.get()->max_width;
        int& total_height = a_datas.get()->total_height;

        // Check if the line is modified or not
        Text_Image_Line* current_line = 0;
        if(line_number < lines.size()) {
            if(lines[line_number] == 0) {
                // Create the new line
                current_line = _generate_line(cutted[line_number], current_style);
                if(current_line != 0) {
                    Image* current_image = current_line->image();
                    current_line->set_has_been_modified(true);
                    if(current_image != 0) {
                        total_height += current_image->height();

                        // Check the max width
                        if(current_image->width() > max_width) {
                            max_width = current_image->width();
                        }

                        // Add the line
                        lines[line_number] = current_line;
                    }
                }
            }
            else {
                // Keep an already generated line
                current_line = lines[line_number];
                current_line->set_datas(cutted[line_number]);
                Image* current_image = current_line->image();
                if(current_image != 0) {
                    total_height += current_image->height();

                    // Check the max width
                    if(current_image->width() > max_width) {
                        max_width = current_image->width();
                    }
                }
            }
        }
        else {
            // Create the new line
            current_line = _generate_line(cutted[line_number], current_style);
            if(current_line != 0) {
                Image* current_image = current_line->image();
                if(current_image != 0) {
                    total_height += current_image->height();

                    // Check the max width
                    if(current_image->width() > max_width) {
                        max_width = current_image->width();
                    }

                    // Add the line
                    lines.push_back(current_line);
                }
            }
        }

        return current_line;
    }

    // Add text to the block
    void Text_Image_Block::add_text(String first_text) {
        // Modify the text
        String text_to_modify = text();
        int cursor_position = cursor_position_in_full_text();
        if(cursor_position > static_cast<int>(text_to_modify.size())) cursor_position = text_to_modify.size();
        String final_text = text_to_modify.substr(0, cursor_position) + first_text + text_to_modify.substr(cursor_position, text_to_modify.size() - cursor_position);
        set_text(final_text);
        // Move the cursor
        String text_plain = defined_balises()->plain_text(first_text);
        set_cursor_position_in_plain_text(cursor_position_in_plain_text() + text_plain.size());

        // Modify the image
        int line_number = line_number_at_position(cursor_position);
        int needed_line = first_text.count("</br>");
        // Add the needed lines
        if(line_number < static_cast<int>(a_lines.size())) {
            if(a_lines[line_number] != 0) {a_lines[line_number]->set_modified(true);} line_number++;
            for(int i = 0;i<needed_line;i++) {a_lines.insert(a_lines.begin() + line_number + i, 0); line_number++;}
            // Edit the next lines
            //for(;line_number<static_cast<int>(a_lines.size());line_number++) {a_lines[line_number]->set_modified(true);}
        }
    };

    // Generates and returns an image with the block on it
    Image* Text_Image_Block::image(Image_Generation_Type generation_type) {
        if(a_last_image.get() != 0 && generation_type == Image_Generation_Type::IGT_Full) return a_last_image.get();

        // Generate the lines
        if(generation_type == Image_Generation_Type::IGT_Full) {
            generate_lines();
        } else if(generation_type == Image_Generation_Type::IGT_Size) {
            place_lines();
        }

        // Draw the final image
        Text_Style current_style = global_style();
        unsigned int current_x = 0;
        unsigned int current_y = 0;
        unsigned char a_line_pasting_max_thread_number = 0;
        int& max_width = a_datas.get()->max_width;
        std::vector<std::thread*> threads = std::vector<std::thread*>();
        int& total_height = a_datas.get()->total_height;
        Image* to_return = new Image(max_width + current_style.border_left_width + current_style.border_right_width, total_height + current_style.border_bottom_width + current_style.border_top_width, current_style.background_color);
        // Draw the border
        to_return->fill_rect(0, 0, current_style.border_left_width, to_return->height(), current_style.border_color);
        to_return->fill_rect(0, to_return->height() - current_style.border_bottom_width, to_return->width(), current_style.border_bottom_width, current_style.border_color);
        to_return->fill_rect(0, 0, to_return->width(), current_style.border_top_width, current_style.border_color);
        to_return->fill_rect(to_return->width() - current_style.border_right_width, 0, current_style.border_right_width, to_return->height(), current_style.border_color);
        // Draw the lines
        for(int i = 0;i<static_cast<int>(lines().size());i++) {
            Text_Image_Line* current_line = lines()[i];
            if(current_line != 0) {
                Image* current_image = current_line->image();
                if(current_image != 0) {
                    if(a_line_pasting_max_thread_number > 0) {
                        // Check for the number of thread
                        if(static_cast<int>(threads.size()) > a_line_pasting_max_thread_number) {
                            // Wait to each thread to finish
                            for(int i = 0;i<static_cast<int>(threads.size());i++) {
                                threads[i]->join();
                                delete threads[i];
                            } threads.clear();
                        }

                        unsigned int height_to_apply = current_image->height();
                        std::thread* current_thread = new std::thread(&Text_Image_Block::__image_paste, this, to_return, current_image, current_x + current_style.border_left_width, current_y + current_style.border_top_width);
                        threads.push_back(current_thread);

                        current_y += height_to_apply;
                    }
                    else {
                        unsigned int height_to_apply = current_image->height();
                        Text_Image_Block::__image_paste(to_return, current_image, current_x + current_style.border_left_width, current_y + current_style.border_top_width);
                        current_y += height_to_apply;
                    }
                }
            }
        } // if(type() == Block_Type::BT_Always_Free_Memory && generation_type == Image_Generation_Type::IGT_Full) clear_lines();

        // Wait to each thread to finish
        for(int i = 0;i<static_cast<int>(threads.size());i++) {
            threads[i]->join();
            delete threads[i];
        } threads.clear();

        a_last_image.reset(to_return);
        return to_return;
    }

    // Returns the line number at a plain text position given, or 0
    int Text_Image_Block::line_number_at_position_in_plain_text(unsigned int position) {
        for(int i = 0;i<static_cast<int>(lines().size());i++) {
            if(lines()[i] != 0) {
                unsigned int line_size = lines()[i]->plain_text_size();
                if(lines()[i]->line_start_position_in_plain_text() + line_size >= position) {
                    return i;
                }
            }
        }
        return -1;
    };

    // Places the lines in the block
    void Text_Image_Block::place_lines() {
        int& max_width = a_datas.get()->max_width; max_width = 0;
        int& total_height = a_datas.get()->total_height; total_height = 0;
        for(int i = 0;i<static_cast<int>(a_lines.size());i++) {
            a_lines[i]->set_max_width(a_current_style.max_width);
            a_lines[i]->place_words();
            a_lines[i]->image(Image_Generation_Type::IGT_Size);

            // Check the max width and height
            int current_height = a_lines[i]->image()->height();
            int current_width = a_lines[i]->image()->width();
            if(current_width > max_width) {
                max_width = current_width;
            } total_height += current_height;
        }
    };

    // Removes a part of the text and returns the number of lines deleted
    int Text_Image_Block::remove_text(unsigned int size_to_delete) {
        int start_position = cursor_position_in_full_text();
        int deleted_line = 0;
        int position = line_number_at_position(start_position);
        if(position != -1) {
            // Modify the cursor
            set_cursor_position_in_plain_text(cursor_position_in_plain_text() - size_to_delete);
            size_to_delete = start_position - defined_balises()->first_plain_text_character_before_position_in_informatted_text(text().to_code_point(), start_position - size_to_delete);

            // Remove the needed text
            String global_text = a_datas.get()->content;
            String deleted_text = global_text.substr(start_position - size_to_delete, size_to_delete);
            global_text = global_text.substr(0, start_position - size_to_delete) + global_text.substr(start_position, global_text.size() - start_position);
            set_text(global_text);
            // Remove the needed lines
            const int line_number_after = line_number_at_position(start_position);
            int line_number = line_number_after;
            deleted_line = deleted_text.count("</br>");
            if(a_lines[line_number] != 0) {a_lines[line_number]->set_modified(true);} line_number++;
            for(int i = 0;i<deleted_line;i++) {if(static_cast<int>(a_lines.size()) > line_number) a_lines.erase(a_lines.begin() + line_number);}
        } return deleted_line;
    };

    // Update the text in each lines, without others modification
    void Text_Image_Block::update_line_text() {
        std::vector<std::string> cutted = cut_string(text(), "</br>", false, true);
        std::vector<Line_Datas>& lines_text = a_lines_text; a_lines_text.clear();
        unsigned int current_position = 0;
        unsigned int current_position_in_plain_text = 0;

        for(int i = 0;i<static_cast<int>(cutted.size());i++) {
            // Check if the line exists or not
            Line_Datas datas;
            datas.content = cutted[i];
            datas.content_in_plain_text = a_defined_balises->plain_text(cutted[i]);
            datas.line_number = i;
            datas.start_position = current_position;
            datas.start_position_in_plain_text = current_position_in_plain_text;
            lines_text.push_back(datas);

            // Update the positions
            current_position += (datas.content.size() + 5);
            current_position_in_plain_text += (datas.content_in_plain_text.size() + 1);
        }

        // Handle empty lines
        if(cutted.size() <= 0) {
            Line_Datas datas;
            datas.content = "";
            datas.content_in_plain_text = "";
            datas.line_number = 0;
            datas.start_position = 0;
            datas.start_position_in_plain_text = 0;
            lines_text.push_back(datas);
        }
    };

    // Return the entire text in an image
    Image* Text_Image_Multi_Block::image(Image_Generation_Type generation_type, const std::string& start_text) {
        if(generation_type == Image_Generation_Type::IGT_Full) {
            // Generate each blocks
            generate_blocks(start_text);
        } else if(generation_type == Image_Generation_Type::IGT_Size) {
            // Regenerate the block only with the size
            for(int i = 0;i<static_cast<int>(a_blocks.size());i++) {
                a_blocks[i].get()->set_max_width(a_global_style.max_width);
                a_blocks[i].get()->image(Image_Generation_Type::IGT_Size);
            }
        } place_blocks();

        // Create the final image and clear memory
        a_current_style = a_global_style;
        Color current_background_color = a_current_style.background_color;
        unsigned int current_x = 0;
        unsigned int current_y = 0;
        Image* final_image = new Image(a_max_width, a_total_height, current_background_color);
        for(int i = 0;i<static_cast<int>(a_blocks.size());i++) {
            Text_Image_Block* current_block = a_blocks[i].get();
            if(current_block != 0) {
                Image* image = current_block->image();
                if(image == 0) {

                }
                else {
                    int x = current_x;
                    int y = current_y;

                    final_image->paste(image, x, y);
                    current_y += image->height();
                }
            } //a_blocks[i].reset();
        } //__delete_blocks();

        return final_image;
    };

    // Place the blocks in the multi-block
    void Text_Image_Multi_Block::place_blocks() {
        a_max_width = 0;
        a_total_height = 0;
        for(int i = 0;i<static_cast<int>(a_blocks_datas.size());i++) {
            std::shared_ptr<Text_Image_Block> new_block = a_blocks[i];
            if(a_max_width < new_block.get()->datas()->max_width) a_max_width = new_block.get()->datas()->max_width;
            a_total_height += new_block.get()->datas()->total_height;
        }
    };

    // Update the datas of each blocks
    void Text_Image_Multi_Block::update_blocks_datas(String text_to_analyse) {
        a_blocks_datas.clear();
        std::vector<std::string> cutted = a_defined_balises->__cut_multi_block(text_to_analyse);
        for(int i = 0;i<static_cast<int>(cutted.size());i++) {
            std::shared_ptr<Block_Datas> current_block_data = std::make_shared<Block_Datas>(cutted[i]);
            a_blocks_datas.push_back(current_block_data);
        }

        // Apply the style of each blocks
        for(int i = 0;i<static_cast<int>(a_blocks_datas.size());i++) {
            a_blocks_datas[i].get()->global_style = global_style();
        }
    };
}
