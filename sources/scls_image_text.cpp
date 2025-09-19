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
#include <charconv>
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

        for(int i = 0;i<static_cast<int>(subpaths.size());i++){if(file_extension(subpaths[i]) == "ttf"){font_files.push_back(subpaths[i]);}}

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
	    if(__system_fonts.size() <= 0){load_system_font();}

	    std::string last_name = "";
	    if(bold) last_name += "b";
	    if(italic) last_name += "i";
	    if(condensed) last_name += "c";
	    if(light) last_name += "l";

	    if(!contains_system_font(font + last_name)) {
            print("Error", "SCLS Image Michelangelo", "The \"" + font + last_name + "\" system font you want to get does not exist.");
            return __system_fonts[__default_font];
	    }

        return __system_fonts[font + last_name];
	};
	Font get_system_font(){return get_system_font(__default_font, false, false, false, false);}

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
    __Image_Base* _char_image(unsigned int character, FT_Face& face, int& cursor_pos, unsigned int& real_width, int& y_pos, Color color) {
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
        __Image_Base* img = new __Image_Base();
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

	// Merges a style with this style
    void __Text_Style_Base::merge_style(__Text_Style_Base* to_merge){
        if(to_merge->a_alignment_horizontal){set_alignment_horizontal(to_merge->a_alignment_horizontal);}
        if(to_merge->a_background_color_modified){set_background_color(to_merge->a_background_color);}
        if(to_merge->a_border_bottom_width_modified){set_border_bottom_width(to_merge->a_border_bottom_width);}
        if(to_merge->a_border_left_width_modified){set_border_left_width(to_merge->a_border_left_width);}
        if(to_merge->a_border_right_width_modified){set_border_right_width(to_merge->a_border_right_width);}
        if(to_merge->a_border_top_width_modified){set_border_top_width(to_merge->a_border_top_width);}
        if(to_merge->a_color_modified){set_color(to_merge->a_color);}
        if(to_merge->a_font_modified){set_font(to_merge->a_font);}
        if(to_merge->a_font_size_modified){set_font_size(to_merge->a_font_size);}
        if(to_merge->a_margin_bottom_modified){set_margin_bottom(to_merge->a_margin_bottom);}
        if(to_merge->a_margin_left_modified){set_margin_left(to_merge->a_margin_left);}
        if(to_merge->a_margin_right_modified){set_margin_right(to_merge->a_margin_right);}
        if(to_merge->a_margin_top_modified){set_margin_top(to_merge->a_margin_top);}

        max_width = to_merge->max_width;
    }
    void Text_Style::merge_style(Text_Style to_merge){a_datas.get()->merge_style(to_merge.a_datas.get());}

    // Returns if an XML attribute is for a text style
    bool text_style_from_xml_attribute(XML_Attribute* attribute, Text_Style style) {
        // Check attributes
        if(attribute->name == "alignment_horizontal") {
            Alignment_Horizontal needed_alignment = Alignment_Horizontal::H_Left;
            if(attribute->value == std::string("center")){needed_alignment = Alignment_Horizontal::H_Center;}
            style.set_alignment_horizontal(needed_alignment);return true;
        }
        else if(attribute->name == "background_color") {style.set_background_color(scls::Color::from_std_string(attribute->value));return true;}
        else if(attribute->name == std::string("color")) {style.set_color(scls::Color::from_std_string(attribute->value));return true;}
        else if(attribute->name == std::string("max_width")) {style.set_max_width(std::stoi(attribute->value));return true;}
        return false;
    }
    void text_style_from_xml(std::shared_ptr<__XML_Text_Base> content, Text_Style style) {
        for(int j = 0;j<static_cast<int>(content.get()->xml_balise_attributes().size());j++) {
            XML_Attribute& current_attribute = content.get()->xml_balise_attributes()[j];
            std::string current_attribute_name = current_attribute.name;
            std::string current_attribute_value = current_attribute.value;

            // Check attributes
            text_style_from_xml_attribute(&current_attribute, style);
        }
    }

	// Load the built-ins balises
    void _Balise_Style_Container::_load_built_in_balises() {
        std::shared_ptr<Balise_Style_Datas> current_balise;

        // HTML styles

        // Create the <DOCTYPE> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->has_content = false;
        current_balise.get()->is_break_line = true;
        set_defined_balise("DOCTYPE", current_balise);
        // Create the <body> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        set_defined_balise("body", current_balise);
        // Create the <br> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->has_content = false;
        current_balise.get()->is_break_line = true;
        set_defined_balise("br", current_balise);
        // Create the <div> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        current_balise.get()->is_paragraph = true;
        set_defined_balise<Balise_Style_Datas>("div", current_balise);
        // Create the <head> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        set_defined_balise("head", current_balise);
        // Create the <p> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        current_balise.get()->is_paragraph = true;
        current_balise.get()->style.set_margin_bottom(16);current_balise.get()->style.set_margin_bottom(8);
        set_defined_balise<Balise_Style_Datas>("p", current_balise);
        // Create the <h1> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        current_balise.get()->is_paragraph = true;
        current_balise.get()->style.set_alignment_horizontal(Alignment_Horizontal::H_Center);
        current_balise.get()->style.set_color(Color(255, 0, 0));current_balise.get()->style.set_font_size(50);current_balise.get()->style.set_font(get_system_font(__default_font));
        current_balise.get()->style.set_margin_bottom(16);current_balise.get()->style.set_margin_bottom(8);
        set_defined_balise<Balise_Style_Datas>("h1", current_balise);
        // Create the <h2> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        current_balise.get()->is_paragraph = true;
        current_balise.get()->style.set_font_size(35);
        current_balise.get()->style.set_margin_bottom(16);current_balise.get()->style.set_margin_bottom(8);
        set_defined_balise<Balise_Style_Datas>("h2", current_balise);
        // Create the <h3> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        current_balise.get()->is_paragraph = true;
        current_balise.get()->style.set_font_size(30);
        current_balise.get()->style.set_margin_bottom(16);current_balise.get()->style.set_margin_bottom(8);
        set_defined_balise<Balise_Style_Datas>("h3", current_balise);
        // Create the <html> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        set_defined_balise("html", current_balise);
        // Create the <important> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        current_balise.get()->style.set_color(Color(255, 0, 0));
        set_defined_balise<Balise_Style_Datas>("important", current_balise);
        // Create the <a> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        set_defined_balise<Balise_Style_Datas>("a", current_balise);
        // Create the <span> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        set_defined_balise<Balise_Style_Datas>("span", current_balise);

        // Mathematicals styles

        // Create the <frac> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        set_defined_balise("frac", current_balise);
        // Create the <math> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        current_balise.get()->style.set_font(get_system_font("DejaVuMathTeXGyre"));
        set_defined_balise<Balise_Style_Datas>("math", current_balise);
        // Create the <frac> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        set_defined_balise("mfrac", current_balise);
        // Create the <mi> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        set_defined_balise("mi", current_balise);
        // Create the <mmat> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        set_defined_balise("mmat", current_balise);
        // Create the <mo> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        set_defined_balise("mo", current_balise);
        // Create the <mover> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        set_defined_balise("mover", current_balise);
        // Create the <mrow> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        set_defined_balise("mrow", current_balise);
        // Create the <msqrt> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        set_defined_balise("msqrt", current_balise);
        // Create the <msub> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        set_defined_balise("msub", current_balise);
        // Create the <msubsup> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        set_defined_balise("msubsup", current_balise);
        // Create the <msup> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        set_defined_balise("msup", current_balise);
        // Create the <munder> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        set_defined_balise("munder", current_balise);
        // Create the <sub> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        set_defined_balise("sub", current_balise);
        // Create the <vec> style
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        set_defined_balise("mvec", current_balise);
        current_balise = std::make_shared<Balise_Style_Datas>();
        current_balise.get()->has_content = true;
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

    // Adds text in the block
    void Block_Datas::add_text(std::string text){
        if(content.get()->only_text()){content.get()->add_text(text);}
        else {
            int ns=content.get()->sub_texts().size();
            if(content.get()->sub_text(ns)->xml_balise_name()==std::string()){content.get()->sub_text(ns)->add_text(text);}
            else{content.get()->sub_text(ns)->add_sub_balise(text);}
        }
    };

    // Generate the image of the word
    void Text_Image_Word::generate_word() {
        // Base variables for the creation
        std::string path = style().font_path();
        if(path == ""){path = get_system_font(__default_font).font_path;}

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
        error = FT_Set_Pixel_Sizes(face, 0, style().font_size());

        // Create each characters
        std::vector<__Image_Base*> characters;
        std::vector<unsigned int> characters_width;
        std::vector<int> characters_x;
        std::vector<int> cursor_pos;
        unsigned int final_character = 0; int current_character_level = 0;int max_current_character_level = 0;
        int max_height = 0;
        short min_bottom_position = 0;
        int to_add_font_size = 0;
        unsigned int total_width = 0;
        std::vector<unsigned int> y_pos;
        const std::string word = a_datas.get()->content();
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
                __Image_Base* image = _char_image(final_character, face, cursor_position, real_width, y_position, style().color());
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
                final_character = 0; current_character_level = 0;max_current_character_level = 0;
            }
            else if(utf_8_level(current_character) == 1 || current_character_level == 2) {
                // Needs an UTF-8 leveling
                current_character &= 0b00011111;
                unsigned int to_compare_as_character = current_character;to_compare_as_character = to_compare_as_character << 6;
                final_character |= to_compare_as_character;
                current_character_level = 1;if(max_current_character_level == 0){max_current_character_level = 1;}
            }
            else if(utf_8_level(current_character) == 2) {
                // Needs an UTF-8 leveling
                current_character &= 0b00001111;
                unsigned int to_compare_as_character = current_character;
                to_compare_as_character = to_compare_as_character << 12;
                final_character |= to_compare_as_character;
                current_character_level = 2;max_current_character_level = 2;
            }
        }
        a_datas.get()->set_bottom_offset(min_bottom_position);
        a_datas.get()->set_characters_position(characters_x);
        a_datas.get()->set_characters_width(characters_width);
        a_datas.get()->set_top_offset(max_height - style().font_size());

        // Inverse each positions
        for(int i = 0;i<static_cast<int>(characters.size());i++) {
            if(characters[i] != 0) {
                y_pos[i] = max_height - (characters[i]->height() + y_pos[i]);
            }
        }

        // Create the final image and clear the memory
        a_datas.get()->set_width(total_width);
        a_last_image.reset(new __Image_Base(total_width, max_height - to_add_font_size, Color(0, 0, 0, 0)));
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
        for(int i = 0;i<static_cast<int>(threads.size());i++) {threads[i]->join();delete threads[i];}
        FT_Done_Face(face);
    };

    // Get an utf-8 symbol from a text
	int utf_8_symbol_by_name(std::string name) {
        if(name == "epsilon") {return 949;}
        else if(name == "esh") {return 643;}
        else if(name == "mdelta") {return 916;}
        else if(name == "mequiv") {return 8801;}
        else if(name == "mequi") {return 8660;}
        else if(name == "mexists") {return 8707;}
        else if(name == "mforall"){return 8704;}
        else if(name == "min"){return 8712;}
        else if(name == "mint"){return 8747;}
        else if(name == "mlt"){return 60;}
        else if(name == "mgt"){return 62;}
        else if(name == "mpartial") {return 948;}
        else if(name == "mto"){return 10230;}
        else if(name == "mu") {return 956;}
        else if(name == "nabla") {return 2207;}
        else if(name == "phi") {return 632;}
        else if(name == "mpi" || name == "pi") {return 960;}
        else if(name == "rho") {return 961;}
        return -1;
	}

	// Get an utf-8 symbol from a XML content
	void utf_8_symbol_xml(std::shared_ptr<__XML_Text_Base> text, bool to_html) {
	    for(int i = 0;i<static_cast<int>(text.get()->sub_texts().size());i++) {
            int potential_symbol = utf_8_symbol_by_name(text.get()->sub_texts().at(i).get()->xml_balise_name());
            if(potential_symbol != -1) {
                text.get()->sub_texts().at(i).get()->clear();
                text.get()->sub_texts().at(i).get()->set_xml_balise_name(std::string());
                if(to_html) {
                    char* c = new char[8];std::to_chars(c, c + 5, potential_symbol, 16);std::string symbol_to_str = c;delete[] c;c=0;
                    std::string needed_text = std::string("&#x") + symbol_to_str.substr(0, std::ceil(std::log(potential_symbol) / std::log(16))) + std::string(";");
                    text.get()->sub_texts().at(i).get()->parse_text(needed_text);
                }
                else {
                    std::string needed_text = std::string("");
                    add_utf_8(needed_text, potential_symbol);
                    text.get()->sub_texts().at(i).get()->parse_text(needed_text);
                }
            }
            else{utf_8_symbol_xml(text.get()->sub_texts().at(i), to_html);}
        }
	}

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
    std::shared_ptr<__Math_Part_Image> __generate_frac(std::shared_ptr<__XML_Text_Base> content, Text_Style current_style, Text_Image_Line* needed_line) {
        // Asserts
        if(content.get()->sub_texts().size() <= 0){return std::shared_ptr<__Math_Part_Image>();}
        else if(content.get()->sub_texts().size() == 1){return needed_line->generate_maths(content.get()->sub_texts()[0], current_style);}

        // Draw the needed image
        Text_Style needed_style = current_style;
        std::shared_ptr<__Math_Part_Image> denominator = needed_line->generate_maths(content.get()->sub_texts()[1], needed_style);
        std::shared_ptr<__Math_Part_Image> numerator = needed_line->generate_maths(content.get()->sub_texts()[0], needed_style);
        // Draw the image to return
        int bar_width = current_style.font_size() / 10;
        int max_width = denominator.get()->image.get()->width(); if(numerator.get()->image.get()->width() > max_width){max_width = numerator.get()->image.get()->width();}
        int needed_size = denominator.get()->image.get()->height() + numerator.get()->image.get()->height() + bar_width * 3;
        std::shared_ptr<__Math_Part_Image> to_return = std::make_shared<__Math_Part_Image>();
        std::shared_ptr<__Image_Base>& image = to_return.get()->image;
        image = std::make_shared<__Image_Base>(max_width, needed_size, current_style.background_color());
        // Paste the needed image
        int bar_length_offset = 1;
        image.get()->fill_rect(bar_length_offset, numerator.get()->image.get()->height() + bar_width, image.get()->width() - bar_length_offset * 2, bar_width, current_style.color());
        image.get()->paste(denominator.get()->image.get(), image.get()->width() / 2 - denominator.get()->image.get()->width() / 2, image.get()->height() - denominator.get()->image.get()->height());
        image.get()->paste(numerator.get()->image.get(), image.get()->width() / 2 - numerator.get()->image.get()->width() / 2, 0);
        to_return.get()->middle_bottom_offset = denominator.get()->image.get()->height() + bar_width + std::ceil(bar_width / 2);
        to_return.get()->middle_top_offset = numerator.get()->image.get()->height() + bar_width + std::floor(bar_width / 2);
        return to_return;
    }
    std::shared_ptr<__Math_Part_Image> __generate_matrice(std::shared_ptr<__XML_Text_Base> xml_content, Text_Style current_style) {
        // Load the needed datas
        std::shared_ptr<__Math_Part_Image> to_return = std::make_shared<__Math_Part_Image>();
        std::shared_ptr<__Image_Base>& image = to_return.get()->image;
        // Get the needed datas
        int dimension_x = 0; int dimension_y = 0;
        for(int i = 0;i<static_cast<int>(xml_content.get()->xml_balise_attributes().size());i++) {
            std::string current_attribute_content = xml_content.get()->xml_balise_attributes()[i].value;
            std::string current_attribute_name = xml_content.get()->xml_balise_attributes()[i].name;
            if(current_attribute_name == "x"){dimension_y = std::stoi(current_attribute_content);}
            else if(current_attribute_name == "y"){dimension_x = std::stoi(current_attribute_content);}
        }
        // Fill the matrice
        std::vector<std::vector<Fraction>> content = std::vector<std::vector<Fraction>>(dimension_y, std::vector<Fraction>(dimension_x, 0));
        for(int i = 0;i<dimension_x;i++) {
            if(i >= static_cast<int>(xml_content.get()->sub_texts().size())) {break;}
            // Get the datas
            std::shared_ptr<__XML_Text_Base> current_xml_content = xml_content.get()->sub_texts()[i];
            if(current_xml_content.get()->xml_balise_name() == "mmat") {
                for(int j = 0;j<dimension_y;j++) {
                    if(j >= static_cast<int>(current_xml_content.get()->sub_texts().size())) {break;}
                    std::shared_ptr<__XML_Text_Base> used_xml_content = current_xml_content.get()->sub_texts()[j];
                    content[j][i] = Fraction::from_std_string(used_xml_content.get()->text());
                }
            }
            else {
                content[0][i] = Fraction::from_std_string(current_xml_content.get()->text());
            }
        }

        // Draw the matrice
        Text_Image_Generator gen;Text_Image_Generator* generator = &gen;
        std::vector<std::vector<std::shared_ptr<__Image_Base>>> images = std::vector<std::vector<std::shared_ptr<__Image_Base>>>();
        // Create each images
        std::vector<int> max_height = std::vector<int>(dimension_x);
        std::vector<int> max_width = std::vector<int>(dimension_y);
        for(int i = 0;i<dimension_x;i++) {
            // Get each sub-matrices
            std::vector<std::shared_ptr<__Image_Base>> current_matrice;
            for(int j = 0;j<dimension_y;j++) {
                // Get each images
                int x = j; int y = i;
                std::shared_ptr<__Image_Base> current_image = generator->image_shared_ptr(content[x][y].to_std_string(0), current_style);
                if(max_height[y] < current_image.get()->height()){max_height[y] = current_image.get()->height();}
                if(max_width[x] < current_image.get()->width()){max_width[x] = current_image.get()->width();}
                current_matrice.push_back(current_image);
            }
            images.push_back(current_matrice);
        }

        // Create the formating
        int end_start_limit_width = max_width[0] / 2;
        int end_width = 2;
        int separation_width = current_style.font_size();
        int start_width = 2;

        // Create the final image
        int total_height = 0; for(int i = 0;i<static_cast<int>(max_height.size());i++){total_height+=max_height[i];}
        total_height += start_width * 4;
        int total_width = 0; for(int i = 0;i<static_cast<int>(max_width.size());i++){total_width+=max_width[i];}
        total_width += (static_cast<int>(max_width.size()) - 1) * separation_width + (end_width + start_width) * 2;
        int current_x = 0; int current_y = start_width * 2;
        image = std::make_shared<__Image_Base>(total_width, total_height, current_style.background_color());
        image.get()->fill_rect(0, 0, start_width, image.get()->height(), current_style.color());
        image.get()->fill_rect(0, 0,end_start_limit_width, start_width, current_style.color());
        image.get()->fill_rect(0, image.get()->height() - start_width, end_start_limit_width, start_width, current_style.color());
        current_x += start_width * 2;
        // Draw each fractions
        for(int i = 0;i<static_cast<int>(images.size());i++){
            for(int j = 0;j<static_cast<int>(images[i].size());j++){
                int needed_x = current_x + ((max_width[j] / 2) - (images[i][j].get()->width() / 2));
                image.get()->paste(images[i][j].get(), needed_x, current_y);
                current_x += max_width[j];
                // Draw the separation
                if(j < static_cast<int>(images[i].size()) - 1) {
                    current_x += separation_width;
                }
            }
            current_x = start_width * 2;
            current_y += max_height[i];
        }
        image.get()->fill_rect(image.get()->width() - end_width, 0, end_width, image.get()->height(), current_style.color());
        image.get()->fill_rect(image.get()->width() - end_start_limit_width, 0, end_start_limit_width, start_width, current_style.color());
        image.get()->fill_rect(image.get()->width() - end_start_limit_width, image.get()->height() - start_width, end_start_limit_width, start_width, current_style.color());
        current_x += end_width;

        // Return the result
        to_return.get()->middle_bottom_offset = std::ceil(image.get()->height() / 2);
        to_return.get()->middle_top_offset = std::floor(image.get()->height() / 2);
        return to_return;
    }
    std::shared_ptr<__Math_Part_Image> __generate_nabla(Text_Style current_style) {
        // Draw the nabla
        int needed_size = current_style.font_size();
        std::shared_ptr<__Math_Part_Image> to_return = std::make_shared<__Math_Part_Image>();
        std::shared_ptr<__Image_Base>& image = to_return.get()->image;
        image = std::make_shared<__Image_Base>(needed_size, needed_size, current_style.background_color());
        // Draw the triangle
        double line_width = static_cast<double>(needed_size) / 10.0;
        int first_line_width = 2;
        int first_x = 0; int first_y = 0;
        int second_x = needed_size / 2; int second_y = needed_size;
        int third_x = needed_size - 1; int third_y = 0;
        for(int i = 0;i<static_cast<int>(first_line_width);i++){image.get()->draw_line(first_x + i * line_width, first_y, second_x, second_y - i * line_width / 2.0, current_style.color(), line_width);}
        for(int i = 0;i<static_cast<int>(first_line_width);i++){image.get()->draw_line(first_x + i * line_width / 2.0, first_y + i * line_width, third_x - i * line_width / 2.0, third_y + i * line_width, current_style.color(), line_width);}
        image.get()->draw_line(second_x, second_y, third_x, third_y, current_style.color(), line_width);
        to_return.get()->middle_bottom_offset = std::ceil(image.get()->height() / 2);
        to_return.get()->middle_top_offset = std::floor(image.get()->height() / 2);
        return to_return;
    }
    std::shared_ptr<__Math_Part_Image> __generate_sub(std::shared_ptr<__XML_Text_Base> content, Text_Style current_style, Text_Image_Line* needed_line) {
        // Draw the needed image
        Text_Style needed_style = current_style; needed_style.set_font_size(current_style.font_size() / 2);
        std::shared_ptr<__Math_Part_Image> needed_part = needed_line->generate_maths(content, needed_style);
        std::shared_ptr<__Image_Base>& needed_image = needed_part.get()->image;
        // Draw the image to return
        int needed_size = current_style.font_size();
        std::shared_ptr<__Math_Part_Image> to_return = std::make_shared<__Math_Part_Image>();
        std::shared_ptr<__Image_Base>& image = to_return.get()->image;
        image = std::make_shared<__Image_Base>(needed_image.get()->width(), needed_size, current_style.background_color());
        // Paste the needed image
        image.get()->paste(needed_image.get(), 0, image.get()->height() - needed_image.get()->height());
        to_return.get()->middle_bottom_offset = std::ceil(image.get()->height() / 2);
        to_return.get()->middle_top_offset = std::floor(image.get()->height() / 2);
        return to_return;
    }
    std::shared_ptr<__Math_Part_Image> __generate_subsup(std::shared_ptr<__XML_Text_Base> content, Text_Style current_style, Text_Image_Line* needed_line) {
        // Asserts
        if(content.get()->sub_texts().size() <= 0){return std::shared_ptr<__Math_Part_Image>();}
        else if(content.get()->sub_texts().size() == 1){return needed_line->generate_maths(content.get()->sub_texts()[0], current_style);}

        // Analyse the part
        std::shared_ptr<__XML_Text_Base> sub_part = content.get()->sub_texts()[0];
        std::shared_ptr<__XML_Text_Base> sup_part = content.get()->sub_texts()[1];

        // Draw the needed image
        Text_Style needed_style = current_style;needed_style.set_font_size(current_style.font_size() / 2);
        std::shared_ptr<__Math_Part_Image> sub = needed_line->generate_maths(sub_part, needed_style);
        std::shared_ptr<__Math_Part_Image> sup = needed_line->generate_maths(sup_part, needed_style);
        // Draw the image to return
        int max_width = sub.get()->image.get()->width(); if(sup.get()->image.get()->width() > max_width){max_width = sup.get()->image.get()->width();}
        int needed_size = current_style.font_size(); //sub.get()->image.get()->height() + sup.get()->image.get()->height();
        std::shared_ptr<__Math_Part_Image> to_return = std::make_shared<__Math_Part_Image>();
        std::shared_ptr<__Image_Base>& image = to_return.get()->image;
        image = std::make_shared<__Image_Base>(max_width, needed_size, current_style.background_color());
        // Paste the needed image
        image.get()->paste(sub.get()->image.get(), image.get()->width() / 2 - sub.get()->image.get()->width() / 2, image.get()->height() - sub.get()->image.get()->height());
        image.get()->paste(sup.get()->image.get(), image.get()->width() / 2 - sup.get()->image.get()->width() / 2, 0);
        to_return.get()->middle_bottom_offset = sub.get()->image.get()->height();
        to_return.get()->middle_top_offset = sup.get()->image.get()->height();
        return to_return;
    }
    std::shared_ptr<__Math_Part_Image> __generate_sup(std::shared_ptr<__XML_Text_Base> content, Text_Style current_style, Text_Image_Line* needed_line) {
        // Draw the needed image
        int base_size = current_style.font_size();
        current_style.set_font_size(base_size / 2);
        std::shared_ptr<__Math_Part_Image> needed_part = needed_line->generate_maths(content, current_style);
        std::shared_ptr<__Image_Base>& needed_image = needed_part.get()->image;
        // Draw the image to return
        std::shared_ptr<__Math_Part_Image> to_return = std::make_shared<__Math_Part_Image>();
        std::shared_ptr<__Image_Base>& image = to_return.get()->image;
        image = std::make_shared<__Image_Base>(needed_image.get()->width(), base_size, current_style.background_color());
        // Paste the needed image
        image.get()->paste(needed_image.get(), 0, 0);
        to_return.get()->middle_bottom_offset = std::ceil(image.get()->height() / 2);
        to_return.get()->middle_top_offset = std::floor(image.get()->height() / 2);
        return to_return;
    }
    std::shared_ptr<__Math_Part_Image> __generate_text_for_maths(std::string text, Text_Style current_style, Text_Image_Line* line) {
        std::shared_ptr<Text_Image_Word> needed_word = line->_generate_word(text, current_style, 0);
        if(needed_word.get() != 0){
            std::shared_ptr<__Math_Part_Image> to_add = std::make_shared<__Math_Part_Image>();
            to_add.get()->image = needed_word.get()->image_shared_pointer();
            to_add.get()->middle_top_offset = std::floor(static_cast<double>(to_add.get()->image.get()->height() + needed_word.get()->bottom_offset()) / 2.0);
            to_add.get()->middle_bottom_offset = to_add.get()->image.get()->height() - to_add.get()->middle_top_offset;
            return to_add;
        } return std::shared_ptr<__Math_Part_Image>();
    };
    std::shared_ptr<__Math_Part_Image> __generate_vector(std::shared_ptr<__Math_Part_Image> base_image, Text_Style current_style) {
        // Draw the nabla
        int needed_size = current_style.font_size() / 2;
        std::shared_ptr<__Math_Part_Image> to_return = std::make_shared<__Math_Part_Image>();
        std::shared_ptr<__Image_Base>& image = to_return.get()->image;
        to_return.get()->bottom_offset = base_image.get()->bottom_offset;
        to_return.get()->top_offset = needed_size;
        image = std::make_shared<__Image_Base>(current_style.font_size(), base_image.get()->image.get()->height() + needed_size, current_style.background_color());
        // Paste the image
        image.get()->paste(base_image.get()->image.get(), current_style.font_size() / 2 - base_image.get()->image.get()->width() / 2, needed_size);
        // Draw the vector
        image.get()->draw_arrow(image.get()->width() / 8, needed_size / 2, image.get()->width() - image.get()->width() / 8, needed_size / 2, current_style.color(), 0.4, 2);
        to_return.get()->middle_top_offset = needed_size + std::floor(base_image.get()->image.get()->height() / 2.0);
        to_return.get()->middle_bottom_offset = image.get()->height() - to_return.get()->middle_top_offset;
        return to_return;
    }
    void __generate_maths_one_balise(std::string needed_balise_name, int& bottom_offset, std::shared_ptr<__XML_Text_Base> content, Text_Style current_style, int& needed_height, int& needed_middle_bottom_offset, int& needed_middle_top_offset, std::vector<std::shared_ptr<__Math_Part_Image>>& needed_parts, int& needed_width, int& top_offset, Text_Image_Line* line) {
        std::shared_ptr<__Math_Part_Image> needed_part;
        int potential_symbol = utf_8_symbol_by_name(needed_balise_name);
        if(potential_symbol != -1) {std::string text = std::string(""); add_utf_8(text, potential_symbol);needed_part = __generate_text_for_maths(text, current_style.new_child(), line);}
        else if(needed_balise_name == "mfrac" || needed_balise_name == "frac") {needed_part = __generate_frac(content, current_style.new_child(), line);}
        else if(needed_balise_name == "mmat") {needed_part = __generate_matrice(content, current_style.new_child());}
        else if(needed_balise_name == "msub" || needed_balise_name == "sub") {needed_part = __generate_sub(content, current_style.new_child(), line);}
        else if(needed_balise_name == "msup") {needed_part = __generate_sup(content, current_style.new_child(), line);}
        else if(needed_balise_name == "msubsup") {needed_part = __generate_subsup(content, current_style.new_child(), line);}
        else if(needed_balise_name == "mvec" || needed_balise_name == "vec") {needed_part = __generate_vector(line->generate_maths(content, current_style.new_child()), current_style);}
        else {needed_part = __generate_text_for_maths(content.get()->text(), current_style.new_child(), line);}

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
    std::shared_ptr<__Math_Part_Image> Text_Image_Line::generate_maths(std::shared_ptr<__XML_Text_Base> content, Text_Style current_style) {
        // Cut the block
        std::vector<std::shared_ptr<__Math_Part_Image>> needed_parts = std::vector<std::shared_ptr<__Math_Part_Image>>();
        int bottom_offset = 0; int needed_height = 0; int needed_width = 0; int top_offset = 0;
        int middle_bottom_offset = 0; int middle_top_offset = 0;
        std::shared_ptr<__Math_Part_Image> to_return = std::make_shared<__Math_Part_Image>();

        // EXPERIMENTAL
        current_style.set_block_style(reinterpret_cast<Balise_Style_Datas*>(a_defined_balises->defined_balise("math"))->style);

        if(content.get()->only_text()) {
            // If the content is only a text
            __generate_maths_one_balise(std::string(""), bottom_offset, content, current_style, needed_height, middle_bottom_offset, middle_top_offset, needed_parts, needed_width, top_offset, this);
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
        std::shared_ptr<__Image_Base>& image = to_return.get()->image;
        image = std::make_shared<__Image_Base>(needed_width, final_height, current_style.background_color());
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
    void Text_Image_Line::__generate_image(std::shared_ptr<Text_Image_Word>& word, std::shared_ptr<__Image_Base> resize_image, unsigned int& current_position_in_plain_text, int& current_width, int height, int width) {
        // Create the image
        word = std::make_shared<Text_Image_Word>(String(std::string()));
        if(resize_image.get()->width() > width){resize_image = resize_image.get()->resize_adaptative_width(width);}
        if(resize_image.get()->height() > height){resize_image = resize_image.get()->resize_adaptative_height(height);}
        word.get()->set_image_shared_ptr(resize_image);
        word.get()->set_x_position(current_width);
        current_position_in_plain_text++;
        current_width += resize_image.get()->width();
    }
    void Text_Image_Line::generate_word(std::shared_ptr<__XML_Text_Base> current_text, unsigned int& current_position_in_plain_text, Text_Style needed_style, std::shared_ptr<Text_Image_Word>& word_to_add) {
        if(!current_text.get()->use_balise()) {__generate_words_without_balise(current_text, needed_style, current_position_in_plain_text);}
        else {
            // Apply a balise
            std::string balise_content = current_text.get()->xml_balise();
            std::string current_balise_name = current_text.get()->xml_balise_name();
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
                    std::shared_ptr<__Image_Base>* src_img = 0;
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
                            if(global_style().max_width() > 0 && width > global_style().max_width()) {
                                width = global_style().max_width();
                                height = static_cast<int>(static_cast<double>(width) * (static_cast<double>(src_img->get()->height()) / static_cast<double>(src_img->get()->width())));
                            }
                        }
                        else {
                            print("Warning", "SCLS Image \"Michelangelo\"", "The \"" + source + "\" <img> path you want to load does not exist.");
                        }
                    }

                    // Create the image
                    if(src_img != 0){__generate_image(word_to_add, *src_img, current_position_in_plain_text, a_current_width, height, width);}
                }
            }
            else if(current_balise_name == "math") {
                // The balise is a piece of mathml
                Text_Style math_style = global_style();
                std::shared_ptr<__Image_Base> src_img = generate_maths(current_text, math_style).get()->image;
                __generate_image(word_to_add, src_img, current_position_in_plain_text, a_current_width, src_img.get()->height(), src_img.get()->width());
                word_to_add.get()->set_balise_content(balise_content);
            }
            else {
                // If the balise is another thing
                Balise_Style_Datas* needed_balise = reinterpret_cast<Balise_Style_Datas*>(a_defined_balises.get()->defined_balise(current_balise_name));
                if(needed_balise != 0) {
                    Text_Style balise_style = Text_Style();
                    balise_style.set_block_style(needed_balise->style);
                    balise_style.set_parent_style(needed_style);
                    generate_words(current_text, current_position_in_plain_text, balise_style);
                }
                else {
                    generate_words(current_text, current_position_in_plain_text, needed_style);
                }
            }


        }
    }
    void Text_Image_Line::__generate_words_without_balise(std::shared_ptr<__XML_Text_Base> text, Text_Style current_style, unsigned int& current_position_in_plain_text) {
        // Handle a single text
        std::string words_content = format_string_as_plain_text(text.get()->text());
        std::vector<std::string> words_cutted = cut_string(words_content, " ");

        // TEMPORARY
        // Edit the style if necessary
        for(int i = 0;i<static_cast<int>(text.get()->xml_attributes().size());i++) {
            if(text.get()->xml_attributes()[i].name == std::string("class") && text.get()->xml_attributes()[i].value == std::string("important")){current_style.set_color(Color(255, 0, 0));}
        }

        // Get the needed words
        for(int j = 0;j<static_cast<int>(words_cutted.size());j++) {
            Word_Datas data_to_add = Word_Datas(); data_to_add.set_balise_parent(text);
            std::string word_content = words_cutted[j];
            std::shared_ptr<Text_Image_Word> word_to_add = _generate_word(word_content, current_style, current_position_in_plain_text);
            if(word_to_add.get() != 0) {
                word_to_add.get()->set_x_position(a_current_width);
                if(word_to_add.get()->bottom_offset() < a_y_offset){a_y_offset = word_to_add.get()->bottom_offset();}
            }
            current_position_in_plain_text += word_content.size();

            // Add the part
            if(word_to_add.get() != 0){word_to_add.get()->set_balise_parent(text);data_to_add = *word_to_add.get()->datas();}
            a_words.push_back(word_to_add);
            a_words_datas.push_back(data_to_add);

            // Add the space
            if(j < static_cast<int>(words_cutted.size()) - 1) {
                word_content = std::string(" ");
                word_to_add = _generate_word(word_content, current_style, current_position_in_plain_text);
                if(word_to_add.get() != 0) {
                    word_to_add.get()->set_x_position(a_current_width);
                    if(word_to_add.get()->bottom_offset() < a_y_offset){a_y_offset = word_to_add.get()->bottom_offset();}
                }
                current_position_in_plain_text += word_content.size();

                // Add the part
                if(word_to_add.get() != 0){word_to_add.get()->set_balise_parent(text);data_to_add = *word_to_add.get()->datas();}
                a_words.push_back(word_to_add);
                a_words_datas.push_back(data_to_add);
            }
        }
    }
    void Text_Image_Line::generate_words(std::shared_ptr<__XML_Text_Base> cutted, unsigned int& current_position_in_plain_text, Text_Style needed_style) {
        // Create each words
        int& current_width = a_current_width; current_width = 0;
        std::vector<std::shared_ptr<Text_Image_Word>>& words = a_words;
        if(cutted.get()->only_text()) {__generate_words_without_balise(cutted, needed_style, current_position_in_plain_text);}
        else {
            // Handle a lot of balises
            for(int i = 0;i<static_cast<int>(cutted.get()->sub_texts().size());i++) {
                std::shared_ptr<Text_Image_Word> word_to_add;
                Word_Datas data_to_add = Word_Datas();data_to_add.set_balise_parent(cutted.get()->sub_texts()[i]);
                generate_word(cutted.get()->sub_texts()[i], current_position_in_plain_text, needed_style, word_to_add);

                // Add the part
                if(word_to_add.get() != 0){word_to_add.get()->set_balise_parent(cutted.get()->sub_texts()[i]);data_to_add = *word_to_add.get()->datas();}
                words.push_back(word_to_add);
                a_words_datas.push_back(data_to_add);
            }
        }
    };

    // Generate a simple word
    std::shared_ptr<Text_Image_Word> Text_Image_Line::_generate_word(const std::string& word, Text_Style style, unsigned int start_position_in_plain_text) {
        // Create the word
        std::shared_ptr<Word_Datas> datas = std::make_shared<Word_Datas>(word, style);
        std::shared_ptr<Text_Image_Word> word_to_add = std::make_shared<Text_Image_Word>(datas);
        word_to_add.get()->generate_word();

        return word_to_add;
    };

    // Generates and returns an image of the line
    std::shared_ptr<__Image_Base> Text_Image_Line::image_shared_ptr(Image_Generation_Type generation_type){image(generation_type);return a_last_image;}
    __Image_Base* Text_Image_Line::image(Image_Generation_Type generation_type) {
        if(a_last_image.get() != 0 && generation_type == Image_Generation_Type::IGT_Full) return a_last_image.get();

        // Generate the words
        if(generation_type == Image_Generation_Type::IGT_Full) {generate_words();}
        place_words();

        // Check for the cursor
        unsigned int cursor_width = 2;
        if(use_cursor() && cursor_position_in_plain_text() >= 0 && cursor_position_in_plain_text() <= static_cast<int>(datas().content_in_plain_text.size())) {
            if(static_cast<int>(a_words.size()) <= 0) {
                a_current_width = cursor_width;
            }
        }

        // Draw the line
        int current_position_in_plain_text = 0;
        int current_x = 0;
        int current_line = 1;int current_y = a_line_height[0];
        int max_width = global_style().max_width();
        // unsigned short space_width = static_cast<unsigned short>(static_cast<double>(global_style()->font_size()) / 2.0);
        int needed_height = total_height() - a_y_offset;
        if(max_width > 0 && a_line_height.size() > 1){a_last_image.reset(new __Image_Base(max_width, needed_height, global_style().background_color()));}
        else{a_last_image.reset(new __Image_Base(a_current_width, needed_height, global_style().background_color()));}
        __Image_Base* final_image = a_last_image.get();

        // Generate each words
        for(int i = 0;i<static_cast<int>(a_words.size());i++) {
            Text_Image_Word* current_word = a_words[i].get();
            if(current_word != 0) {
                __Image_Base* current_image = current_word->image();
                if(current_image != 0 && current_image->width() > 0) {
                    // Check for the cursor
                    if(use_cursor() && cursor_position_in_plain_text() >= 0 && cursor_position_in_plain_text() <= static_cast<int>(datas().content_in_plain_text.size())) {
                        if(cursor_position_in_plain_text() >= current_position_in_plain_text && cursor_position_in_plain_text() <= current_position_in_plain_text + static_cast<int>(current_word->text().size())) {
                            int local_cursor_position = cursor_position_in_plain_text() - current_position_in_plain_text;
                            if(local_cursor_position == static_cast<int>(current_word->text().size())) {
                                a_cursor_x = current_x + current_image->width() - static_cast<double>(cursor_width) / 2.0;
                            }
                            else {a_cursor_x = current_x + current_word->datas()->characters_position()[local_cursor_position];}
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
                    if(balise_name(current_word->datas()->balise_content()) == "math"){y = final_image->height() / 2.0 - static_cast<double>(current_image->height()) / 2.0;};

                    // Paste the word
                    final_image->paste(current_image, current_x, y);
                    current_word->set_x_position(current_x); a_words_datas[i].set_x_position(current_x);
                    current_x += current_image->width();
                    current_position_in_plain_text += current_word->text().size();

                }
            }
        }

        // Draw the cursor
        if(cursor_position_in_plain_text() == 0) a_cursor_x = 0;
        else a_cursor_x -= static_cast<double>(cursor_width) / 2.0;
        if(use_cursor() && cursor_position_in_plain_text() >= 0 && cursor_position_in_plain_text() <= static_cast<int>(datas().content_in_plain_text.size())) {
            final_image->fill_rect(a_cursor_x, 0, cursor_width, final_image->height(), global_style().color());
        }

        return final_image;
    }

    // Place the words as needed
    void Text_Image_Line::place_words() {
        a_line_height.clear();
        int current_height = global_style().font_size();
        int& current_width = a_current_width; current_width = 0;
        for(int i = 0;i<static_cast<int>(a_words.size());i++) {
            // Check the max width
            if(a_words[i].get() == 0 || (a_words[i].get()->text() == std::string("") && current_width == 0 && a_line_height.size() > 0)){continue;}
            int image_height = a_words[i].get()->image()->height();
            int image_width = a_words[i].get()->image()->width();
            if(global_style().max_width() > 0 && current_width + image_width > global_style().max_width()) {
                a_line_height.push_back(current_height);
                current_height = global_style().font_size();current_width = 0;
            }
            a_words[i].get()->set_line(a_line_height.size());

            // Update the height and the width
            if(image_height>current_height){current_height=image_height;}
            current_width += image_width;
        }

        // Add the last height
        a_line_height.push_back(current_height);
    };

    // Returns a word at a position in pixel
    std::shared_ptr<Text_Image_Word> Text_Image_Line::word_at_position_in_pixel(int x, int y) {
        // Get the needed line
        int current_height = a_line_height[0];
        int current_width = 0; int current_line = 0; int i;
        for(i = 0;i<static_cast<int>(a_words.size()) && current_height < y;i++) {
            // Asserts
            if(a_words[i].get() == 0){continue;}

            // Check the max width
            int image_width = a_words[i].get()->image()->width();
            if(global_style().max_width() > 0 && current_width + image_width > global_style().max_width()) {
                if(current_height > y){break;}
                current_line++; current_height += a_line_height[current_line];current_width=0;
            } current_width += image_width;
        }

        // Check the good x
        for(;i<static_cast<int>(a_words.size());i++) {
            // Asserts
            if(a_words[i].get() == 0){continue;}
            int image_width = a_words[i].get()->image()->width();
            if(current_width + image_width > x){break;}
            current_width += image_width;
        }

        // Get the needed word
        if(i < static_cast<int>(a_words.size())){
            return a_words[i];
        } return std::shared_ptr<Text_Image_Word>();
    }

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
    Text_Image_Line* Text_Image_Block::_generate_line(Line_Datas datas) {
        // Create the line
        Text_Image_Line* to_return = __create_line(datas);
        to_return->set_datas(datas);

        // EXPERIMENTAL
        to_return->global_style().set_max_width(global_style().max_width());

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
        std::vector<Line_Datas>& cutted = a_lines_text;
        std::vector<Text_Image_Line*>& lines = a_lines;
        int& max_width = a_datas.get()->max_width;
        int& total_height = a_datas.get()->total_height;

        // Check if the line is modified or not
        Text_Image_Line* current_line = 0;
        if(line_number < lines.size()) {
            if(lines[line_number] == 0) {
                // Create the new line
                current_line = _generate_line(cutted[line_number]);
                if(current_line != 0) {
                    __Image_Base* current_image = current_line->image();
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
                __Image_Base* current_image = current_line->image();
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
            current_line = _generate_line(cutted[line_number]);
            if(current_line != 0) {
                __Image_Base* current_image = current_line->image();
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

    // Generate the next block / word of the block
    std::shared_ptr<Text_Image_Word> Text_Image_Block::generate_next_word(){return generate_next_word(a_current_object++);}
    std::shared_ptr<Text_Image_Word> Text_Image_Block::generate_next_word(int word_number) {
        // Cut the text by line and delete useless lines
        std::vector<std::shared_ptr<Word_Datas>>& needed_word_datas = a_words_datas;
        std::vector<std::shared_ptr<Text_Image_Word>>& needed_words = a_words;

        // Check if the line is modified or not
        std::shared_ptr<Text_Image_Word> current_word;
        if(word_number < static_cast<int>(needed_words.size())) {
            if(needed_word_datas.at(word_number).get()->is_special()){needed_words[word_number].reset();}
            else {
                // Create the new word
                needed_words[word_number] = __generate_word(needed_word_datas.at(word_number));
                current_word = needed_words[word_number];
                current_word.get()->generate_word();
            }
        }
        else {
            if(needed_word_datas.at(word_number).get()->is_special()){needed_words.push_back(current_word);}
            else{
                // Create the new word
                current_word = __generate_word(needed_word_datas.at(word_number));
                current_word.get()->generate_word();

                // Add the line
                needed_words.push_back(current_word);
            }
        }

        return current_word;
    }

    // Generate a block / word of the block
    std::shared_ptr<Text_Image_Word> Text_Image_Block::__generate_word(std::shared_ptr<Word_Datas> datas) {
        // Create the line
        std::shared_ptr<Text_Image_Word> to_return = __create_word(datas);

        /*// Handle the cursor
        if(use_cursor()) {
            if(cursor_position_in_plain_text() >= datas.start_position_in_plain_text && cursor_position_in_plain_text() <= datas.start_position_in_plain_text + datas.content_in_plain_text.size()) {
                to_return->set_cursor_position_in_plain_text(cursor_position_in_plain_text() - datas.start_position_in_plain_text);
                to_return->set_use_cursor(true);

                a_cursor_line = to_return;
            }
        }//*/

        return to_return;
    }

    // Generates all the blocks / lines
    void Text_Image_Block::generate_words(bool entirely){if(entirely){free_memory();}__regenerate_words();};;
    void Text_Image_Block::generate_words(){generate_words(true);};
    void Text_Image_Block::__regenerate_words() {reset_generation();for(int i = 0;i<static_cast<int>(a_words_datas.size());i++) {generate_next_word(i);}delete_useless_generated_lines();place_datas();};

    // Returns the line at a position in pixel
    Text_Image_Line* Text_Image_Block::line_at_position_in_pixel(int x, int y, int& needed_y) {
        int current_y = 0;
        for(int i = 0;i<static_cast<int>(lines().size());i++) {
            if(lines()[i] != 0) {
                current_y += lines()[i]->image()->height();
                if(y < current_y) {needed_y=current_y-lines()[i]->image()->height();return lines()[i];}
            }
        }
        return lines()[lines().size() - 1];
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
    __Image_Base* Text_Image_Block::image(Image_Generation_Type generation_type) {
        // Generate the lines
        if(generation_type == Image_Generation_Type::IGT_Full) {generate_words();}
        else if(generation_type == Image_Generation_Type::IGT_Size) {place_datas();}

        if(content()->only_text()) {
            // Draw the final image
            unsigned int current_x = 0;
            unsigned int current_y = 0;
            unsigned char a_line_pasting_max_thread_number = 0;
            int& max_width = a_datas.get()->max_width;
            std::vector<std::thread*> threads = std::vector<std::thread*>();
            int& total_height = a_datas.get()->total_height;
            int image_height = total_height;
            if(image_height < global_style().font_size()){image_height = global_style().font_size();}
            image_height += global_style().border_bottom_width() + global_style().border_top_width() + datas()->max_last_line_bottom_offset;
            __Image_Base* to_return = new __Image_Base(max_width + global_style().border_left_width() + global_style().border_right_width(), image_height, global_style().background_color());

            // Draw the border
            to_return->fill_rect(0, 0, global_style().border_left_width(), to_return->height(), global_style().border_color());
            to_return->fill_rect(0, to_return->height() - global_style().border_bottom_width(), to_return->width(), global_style().border_bottom_width(), global_style().border_color());
            to_return->fill_rect(0, 0, to_return->width(), global_style().border_top_width(), global_style().border_color());
            to_return->fill_rect(to_return->width() - global_style().border_right_width(), 0, global_style().border_right_width(), to_return->height(), global_style().border_color());

            // Draw the words
            for(int i = 0;i<static_cast<int>(words().size());i++) {
                Text_Image_Word* current_word = words().at(i).get();
                if(current_word != 0) {
                    __Image_Base* current_image = current_word->image();
                    if(current_image != 0) {
                        // Alignment
                        if(global_style().alignment_horizontal() == H_Center){current_x = to_return->width() / 2 - current_image->width() / 2;}

                        if(a_line_pasting_max_thread_number > 0) {
                            // Check for the number of thread
                            if(static_cast<int>(threads.size()) > a_line_pasting_max_thread_number) {
                                // Wait to each thread to finish
                                for(int i = 0;i<static_cast<int>(threads.size());i++) {
                                    threads[i]->join();
                                    delete threads[i];
                                } threads.clear();
                            }

                            int current_width = current_image->width();
                            unsigned int height_to_apply = current_image->height();
                            std::thread* current_thread = new std::thread(&Text_Image_Block::__image_paste, this, to_return, current_image, current_x + global_style().border_left_width(), current_y + global_style().border_top_width());
                            threads.push_back(current_thread);

                            // Finish the result
                            current_x += current_width;
                            current_y += height_to_apply;
                        }
                        else {
                            // Get the datas
                            Text_Style style_to_apply = current_word->style();
                            int current_width = current_image->width();
                            unsigned int height_to_apply = current_image->height();
                            int y_to_apply = current_y + global_style().border_top_width() - (current_word->top_offset());

                            // Paste the word
                            Text_Image_Block::__image_paste(to_return, current_image, current_x + global_style().border_left_width(), y_to_apply);

                            // Finish the result
                            current_x += current_width;
                        }
                    }
                }
                else {
                    // Special word
                    if(words_datas().at(i).get()->is_space()){current_x += words_datas().at(i).get()->style().font_size() / 2;}
                }
            }

            // Wait to each thread to finish
            for(int i = 0;i<static_cast<int>(threads.size());i++) {
                threads[i]->join();
                delete threads[i];
            } threads.clear();

            a_last_image.reset(to_return);
            return to_return;
        }

        /*// Draw the final image
        unsigned int current_x = 0;
        unsigned int current_y = 0;
        unsigned char a_line_pasting_max_thread_number = 0;
        int& max_width = a_datas.get()->max_width;
        std::vector<std::thread*> threads = std::vector<std::thread*>();
        int& total_height = a_datas.get()->total_height;
        __Image_Base* to_return = new __Image_Base(max_width + global_style().border_left_width() + global_style().border_right_width(), total_height + global_style().border_bottom_width() + global_style().border_top_width(), global_style().background_color());
        // Draw the border
        to_return->fill_rect(0, 0, global_style().border_left_width(), to_return->height(), global_style().border_color());
        to_return->fill_rect(0, to_return->height() - global_style().border_bottom_width(), to_return->width(), global_style().border_bottom_width(), global_style().border_color());
        to_return->fill_rect(0, 0, to_return->width(), global_style().border_top_width(), global_style().border_color());
        to_return->fill_rect(to_return->width() - global_style().border_right_width(), 0, global_style().border_right_width(), to_return->height(), global_style().border_color());
        // Draw the lines
        for(int i = 0;i<static_cast<int>(lines().size());i++) {
            Text_Image_Line* current_line = lines()[i];
            if(current_line != 0) {
                __Image_Base* current_image = current_line->image(generation_type);
                if(current_image != 0) {
                    // Alignment
                    if(global_style().alignment_horizontal() == H_Center){current_x = to_return->width() / 2 - current_image->width() / 2;}

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
                        std::thread* current_thread = new std::thread(&Text_Image_Block::__image_paste, this, to_return, current_image, current_x + global_style().border_left_width(), current_y + global_style().border_top_width());
                        threads.push_back(current_thread);

                        current_y += height_to_apply;
                    }
                    else {
                        unsigned int height_to_apply = current_image->height();
                        Text_Image_Block::__image_paste(to_return, current_image, current_x + global_style().border_left_width(), current_y + global_style().border_top_width());
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
        return to_return;//*/
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

    // Places the datas in the block
    void Text_Image_Block::place_datas() {
        // Get the good text
        __XML_Text_Base* needed_content = content();
        if(needed_content->only_text()) {
            // Update the datas
            int& max_width = a_datas.get()->max_width; max_width = 0;
            int& total_height = a_datas.get()->total_height; total_height = 0;

            // Set the position
            unsigned int current_x = 0;
            unsigned int current_y = 0;
            int line_height = 0;bool new_line = false;
            for(int i = 0;i<static_cast<int>(a_words.size());i++) {
                // Special characters
                if(a_words_datas.at(i).get()->is_space()){int space_size = a_words_datas.at(i).get()->style().font_size() / 2;current_x += space_size;max_width += space_size;continue;}

                // Creates the image (if not created)
                scls::__Image_Base* word_image = a_words.at(i).get()->image();
                if(word_image == 0){continue;}

                // Check the max width and height
                int current_height = word_image->height();
                int current_width = word_image->width();
                line_height = std::max(line_height, current_height);

                // Check the position
                a_words_datas.at(i).get()->set_x_position(current_x);
                current_x += current_width;max_width += current_width;
                int y_to_apply = current_y + global_style().border_top_width() - (a_words_datas.at(i).get()->top_offset());
                a_words_datas.at(i).get()->set_y_position(y_to_apply);

                // Check the offset
                if(-a_words.at(i).get()->bottom_offset() > datas()->max_last_line_bottom_offset){datas()->max_last_line_bottom_offset = -a_words.at(i).get()->bottom_offset();}
            }

            // Finish the result
            total_height += line_height;
        }

        /*int& max_width = a_datas.get()->max_width; max_width = 0;
        int& total_height = a_datas.get()->total_height; total_height = 0;
        for(int i = 0;i<static_cast<int>(a_lines.size());i++) {
            a_lines[i]->set_max_width(global_style().max_width());
            a_lines[i]->place_words();
            a_lines[i]->image(Image_Generation_Type::IGT_Size);

            // Check the max width and height
            int current_height = a_lines[i]->image()->height();
            int current_width = a_lines[i]->image()->width();
            if(current_width > max_width) {
                max_width = current_width;
            } total_height += current_height;
        }
        //*/
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
            String global_text = String(a_datas.get()->content.get()->full_text());
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

    // Height of the block
    int Text_Image_Block::total_height() const {
        int image_height = a_datas.get()->total_height;
        if(image_height < a_datas.get()->global_style.font_size()){image_height = a_datas.get()->global_style.font_size();}
        image_height += a_datas.get()->global_style.border_bottom_width() + a_datas.get()->global_style.border_top_width() + a_datas.get()->max_last_line_bottom_offset;
        return image_height;
    }

    // Update the datas, without others modification
    void Text_Image_Block::update_datas() {
        // Get the good text
        __XML_Text_Base* needed_content = content();

        if(needed_content->only_text()) {
            // Get the needed words
            std::vector<std::string> needed_text = text().cut(" ", false, false);
            a_words_datas.clear();

            // Start the parsing with words
            unsigned int current_position = 0;
            unsigned int current_position_in_plain_text = 0;
            for(int i = 0;i<static_cast<int>(needed_text.size());i++) {
                // Create the needed datas
                scls::Text_Style style = global_style().new_child();

                // Create the datas
                std::shared_ptr<Word_Datas> current_datas = std::make_shared<Word_Datas>(needed_text.at(i), style);
                a_words_datas.push_back(current_datas);

                // Update the positions
                current_position += needed_text.at(i).size();

                // Add the space
                if(i != static_cast<int>(needed_text.size()) - 1){current_datas = std::make_shared<Word_Datas>(std::string(" "), style);a_words_datas.push_back(current_datas);}
            }
        }

        /*// Get the good text
        std::string needed_text = std::string();
        if(datas()->content.get()->balise_datas().is_paragraph){needed_text=scls::replace(text(), std::string("</br>"), std::string("<br>"));}
        else{needed_text=full_text();}

        // Start the parsing
        std::vector<std::string> cutted = cut_string(needed_text, "<br>", false, true);
        std::vector<Line_Datas>& lines_text = a_lines_text; a_lines_text.clear();
        unsigned int current_position = 0;
        unsigned int current_position_in_plain_text = 0;

        for(int i = 0;i<static_cast<int>(cutted.size());i++) {
            // Check if the line exists or not
            Line_Datas datas;
            datas.content = xml(a_defined_balises, cutted[i]);
            datas.content_in_plain_text = a_defined_balises->plain_text(cutted[i]);
            datas.line_number = i;
            datas.start_position = current_position;
            datas.start_position_in_plain_text = current_position_in_plain_text;
            lines_text.push_back(datas);

            // Update the positions
            current_position += (datas.content.get()->full_text().size() + 5);
            current_position_in_plain_text += (datas.content_in_plain_text.size() + 1);
        }

        // Handle empty lines
        if(cutted.size() <= 0) {
            Line_Datas datas;
            datas.content = __XML_Text_Base::new_xml_text(a_defined_balises, std::string(""), true);
            datas.content_in_plain_text = "";
            datas.line_number = 0;
            datas.start_position = 0;
            datas.start_position_in_plain_text = 0;
            lines_text.push_back(datas);
        }
        //*/
    };






    // Generate each blocks in the multiblocks (and delete the previous ones)
    void Text_Image_Multi_Block::generate_blocks() {a_blocks.clear();for(int i = 0;i<static_cast<int>(a_blocks_datas.size());i++) {generate_next_block(i);}}

    // Generate a block in the multiblocks
    std::shared_ptr<Text_Image_Block> Text_Image_Multi_Block::generate_next_block(int i) {
        if(i >= static_cast<int>(a_blocks.size())) {
            // Create the block
            std::shared_ptr<Text_Image_Block> new_block = __create_block(a_blocks_datas[i]);
            std::string needed_balise_name = a_blocks_datas[i].get()->content.get()->xml_balise_name();

            // Apply the needed style
            new_block.get()->global_style().set_max_width(global_style().max_width());
            new_block.get()->set_use_cursor(a_use_cursor);
            new_block.get()->image();

            // Add the block
            a_blocks.push_back(new_block);
            return new_block;
        }

        // Returns a pre-existing block
        a_blocks[i].get()->global_style().set_max_width(global_style().max_width());
        a_blocks[i].get()->reset_generation();
        return a_blocks[i];
    }

    // Return the entire text in an image
    __Image_Base* Text_Image_Multi_Block::image(Image_Generation_Type generation_type, const std::string& start_text) {
        if(generation_type == Image_Generation_Type::IGT_Full) {generate_blocks(start_text);}
        else if(generation_type == Image_Generation_Type::IGT_Size) {
            // Regenerate the block only with the size
            for(int i = 0;i<static_cast<int>(a_blocks.size());i++) {
                a_blocks[i].get()->set_max_width(global_style().max_width());
                a_blocks[i].get()->image(Image_Generation_Type::IGT_Size);
            }
        } place_blocks();

        // Create the final image and clear memory
        Color current_background_color = global_style().background_color();
        unsigned int current_x = 0;
        unsigned int current_y = 0;
        __Image_Base* final_image = new __Image_Base(a_max_width, a_total_height, current_background_color);
        for(int i = 0;i<static_cast<int>(a_blocks.size());i++) {
            Text_Image_Block* current_block = a_blocks[i].get();
            if(current_block != 0) {
                __Image_Base* image = current_block->image();
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

    // Returns the text in the multi-block
    String Text_Image_Multi_Block::text() const {
        String to_return;
        for(int i = 0;i<static_cast<int>(a_blocks_datas.size());i++) {
            if(a_blocks_datas.at(i).get() != 0){
                // Add each balise
                to_return += a_blocks_datas.at(i).get()->content.get()->full_text();
            }
        }
        return to_return;
    }

    // Update the datas of each blocks
    void Text_Image_Multi_Block::update_blocks_datas(std::shared_ptr<__XML_Text_Base> cutted) {
        a_blocks_datas.clear();
        if(cutted.get()->only_text()) {std::shared_ptr<Block_Datas> current_block_data = std::make_shared<Block_Datas>(cutted);a_blocks_datas.push_back(current_block_data);}
        else {
            // More than a single text
            std::shared_ptr<__XML_Text_Base> temp_balise;
            for(int i = 0;i<static_cast<int>(cutted.get()->sub_texts().size());i++) {
                std::string current_balise_name = cutted.get()->sub_texts()[i].get()->xml_balise_name();
                Balise_Style_Datas* needed_balise = reinterpret_cast<Balise_Style_Datas*>(a_defined_balises.get()->defined_balise(current_balise_name));
                if(needed_balise != 0 && needed_balise->is_paragraph) {
                    if(temp_balise.get() != 0) {
                        // Create a needed paragraph before this one
                        std::shared_ptr<Block_Datas> current_block_data = std::make_shared<Block_Datas>(temp_balise);
                        cutted.get()->sub_texts()[i - 1] = temp_balise;
                        Balise_Style_Datas* needed_balise_temp = reinterpret_cast<Balise_Style_Datas*>(a_defined_balises.get()->defined_balise(temp_balise.get()->xml_balise_name()));
                        if(needed_balise_temp != 0){current_block_data.get()->global_style.set_block_style(needed_balise_temp->style);}
                        a_blocks_datas.push_back(current_block_data); temp_balise.reset();
                    }

                    // Create a new paragraph
                    std::shared_ptr<Block_Datas> current_block_data = std::make_shared<Block_Datas>(cutted.get()->sub_texts()[i]);
                    if(needed_balise != 0){current_block_data.get()->global_style.set_block_style(needed_balise->style);}
                    a_blocks_datas.push_back(current_block_data);
                }
                else {
                    // Create a new paragraph for a non-paragraphed balise
                    std::shared_ptr<__XML_Text_Base> temp = cutted.get()->sub_texts()[i];
                    if(temp_balise.get() == 0){temp_balise = __XML_Text_Base::new_xml_text(a_defined_balises, std::string(""), false);}
                    else{cutted.get()->sub_texts().erase(cutted.get()->sub_texts().begin() + i);i--;}
                    temp_balise.get()->sub_texts().push_back(temp);
                    temp_balise.get()->sub_texts()[temp_balise.get()->sub_texts().size() - 1].get()->set_xml_balise_name(current_balise_name);
                }
            }

            // Handle the last possible paragraph
            if(temp_balise.get() != 0) {
                std::shared_ptr<Block_Datas> current_block_data = std::make_shared<Block_Datas>(temp_balise);
                cutted.get()->sub_texts()[cutted.get()->sub_texts().size() - 1] = temp_balise;
                Balise_Style_Datas* needed_balise_temp = reinterpret_cast<Balise_Style_Datas*>(a_defined_balises.get()->defined_balise(temp_balise.get()->xml_balise_name()));
                if(needed_balise_temp != 0){current_block_data.get()->global_style.set_block_style(needed_balise_temp->style);}
                a_blocks_datas.push_back(current_block_data); temp_balise.reset();
            }
        }

        // Apply the style of each blocks
        for(int i = 0;i<static_cast<int>(a_blocks_datas.size());i++) {a_blocks_datas[i].get()->global_style.set_parent_style(global_style());}
    };

    // Converts some well-known types into image
    std::shared_ptr<__Image_Base> to_image(std::string value, Text_Style style){Text_Image_Generator gen;return gen.image_shared_ptr(value, style);};
    std::shared_ptr<__Image_Base> to_image(std::string* value, Text_Style style){Text_Image_Generator gen;return gen.image_shared_ptr(*value, style);};
    std::shared_ptr<__Image_Base> to_image(std::string* value){Text_Style style;return to_image(value, style);};
    std::shared_ptr<__Image_Base> to_image(std::string value){Text_Style style;return to_image(&value, style);};
}
