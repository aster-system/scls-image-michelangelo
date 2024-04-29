//******************
//
// scls_image_text.h
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
// This file allows the user to use text with SCLS Images.
//

#ifndef SCLS_IMAGE_TEXT
#define SCLS_IMAGE_TEXT

// Base path to the fonts in the system
#ifdef __WIN32__ // With Windows
#define BASE_FONT_PATH std::string("C:\\Windows\\Fonts\\")
#define DEFAULT_FONT std::string("arial")
#endif
#ifdef __linux__ // With Linux
#define BASE_FONT_PATH std::string("/usr/share/fonts/truetype/")
#define DEFAULT_FONT std::string("FreeMono")
#endif

// The namespace "scls" is used to simplify the all.
namespace scls
{
    //*********
	//
	// Font handling
	//
	//*********

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

	// Load all the fonts system fonts
	static void load_system_font() {
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
	static Font get_system_font(std::string font, bool bold = false, bool italic = false, bool condensed = false, bool light = false) {
	    if(_system_fonts.size() <= 0) load_system_font();

	    std::string last_name = "";
	    if(bold) last_name += "b";
	    if(italic) last_name += "i";
	    if(condensed) last_name += "c";
	    if(light) last_name += "l";
        return _system_fonts[font + last_name];
	};

	// Print each system fonts
	static void print_system_fonts() {
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

    //*********
	//
	// Text creation
	//
	//*********

	// Part of a text gotten by a balising cut
    struct _Text_Balise_Part {
        // Content of the part
        std::string content = "";
        // Position of the start of the balise (of the first char of the content)
        unsigned int start_position = 0;
    };

	// Return the name of a formatted balise
	static std::string balise_name(std::string balise_formatted) {
	    if(balise_formatted[0] == '<') balise_formatted = balise_formatted.substr(1, balise_formatted.size() - 1);
	    if(balise_formatted[0] == '/') balise_formatted = balise_formatted.substr(1, balise_formatted.size() - 1);
	    if(balise_formatted[balise_formatted.size() - 1] == '>') balise_formatted = balise_formatted.substr(0, balise_formatted.size() - 1);
	    // Remove useless spaces
	    while(balise_formatted.size() > 0 && balise_formatted[0] == ' ') {
            balise_formatted = balise_formatted.substr(1, balise_formatted.size() - 1);
	    }

	    return cut_string(balise_formatted, " ")[0];
	};

	// Cut a string by its balises
	static std::vector<_Text_Balise_Part> cut_string_by_balise(std::string str, bool erase_blank = false, bool erase_last_if_blank = true) {
		std::string last_string = ""; // String since the last cut
		std::vector<_Text_Balise_Part> result = std::vector<_Text_Balise_Part>();
		for (int i = 0; i < static_cast<int>(str.size()); i++) // Browse the string char by char
		{
		    if(str[i] == '<') {
                _Text_Balise_Part part_to_add;
                part_to_add.content = last_string;
                part_to_add.start_position = i;
                if(last_string == "") {
                    if(!erase_blank && result.size() > 0)result.push_back(part_to_add);
                }
                else result.push_back(part_to_add);
                last_string = "";

                i++;
                while(str[i] != '>') {
                    if(i >= str.size()) break;
                    last_string += str[i];
                    i++;
                }

                part_to_add.content = "<" + last_string + ">";
                result.push_back(part_to_add);
                last_string = "";
                continue;
		    }

			last_string += str[i];
		}

		if (last_string.size() > 0 || !erase_last_if_blank) {
            _Text_Balise_Part part_to_add;
            part_to_add.content = last_string;
            result.push_back(part_to_add);
        } // Add the last non-cutted element
		return result;
	};

	// Format a balise and return it
	static std::string formatted_balise(std::string str) {
	    // Remove useless spaces
	    while(str.size() > 0 && str[0] == ' ') {
            str = str.substr(1, str.size() - 1);
	    }
	    while(str.size() > 0 && str[str.size() - 1] == ' ') {
            str = str.substr(0, str.size() - 1);
	    }

	    // Get the position of the / if there is one
        int slash_position = 0;
        bool slash_position_founded = false;
        while(str[slash_position] < str.size()) {
            if(str[slash_position] == '/') {
                slash_position_founded = true;
                break;
            }
            slash_position++;
        }

        // Format the balise
        std::string final_balise = "";
        if(slash_position_founded) final_balise = "</" + str.substr(slash_position + 1, str.size() - (2 + slash_position)) + ">";
        else final_balise = "<" + str.substr(1, str.size() - 2) + ">";

        // Format the formatted balise (help)
	    while(final_balise.size() > 0 && final_balise[0] == ' ') {
            final_balise = final_balise.substr(1, final_balise.size() - 1);
	    }
	    while(final_balise.size() > 0 && final_balise[final_balise.size() - 1] == ' ') {
            final_balise = final_balise.substr(0, final_balise.size() - 1);
	    }

        return final_balise;
	};

	// HTML formatted
	static std::string html_formatted(std::string str) {
	    std::string nl = ""; nl += static_cast<char>(10);
	    std::string np = ""; np += static_cast<char>(13);
	    str = replace(str, nl, "");
	    str = replace(str, np, "");
	    return str;
	};

	// Hiddens variables
	static FT_Library  _freetype_library;
    static bool _free_type_library_inited = false;

	// Enumeration of each text alignment possible
	enum Text_Alignment_Horizontal {
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
        Text_Alignment_Horizontal alignment = Text_Alignment_Horizontal::Left;

        // Out offset
        unsigned short out_offset_bottom_width = 0;
        unsigned short out_offset_left_width = 0;
        unsigned short out_offset_right_width = 0;
        unsigned short out_offset_top_width = 0;
    };

    // Style about a text
    struct Text_Style {
        // Horizontal alignment of the text
        Text_Alignment_Horizontal alignment_horizontal = Text_Alignment_Horizontal::Left;
        // Color of the background color
        Color background_color = white;
        // Color of the text
        Color color = black;
        // Font of the style
        Font font;
        // Font size of the style
        unsigned short font_size = 20;
        // Max width of the text (only in pixel for now)
        double max_width = 500;
    };

    // Balise in a text
    struct Text_Balise {
        // If the balise
        bool has_content = true;
        // If the balise is a paragraph or not
        bool is_paragraph = false;
        // Name of the balise
        std::string name = "";
        // Style of the balise
        Text_Style style;
    };

    // Block of text in a text
    struct Text_Block {
        // Content of the block
        std::string content = "";
    };

    // Part of a block of a text representing a word to draw
    struct _Text_Block_Part {
        // Pointer to the image
        Image* image = 0;
        // If the part is a paragraph or not
        bool is_paragraph = false;
        // Style of the part
        Text_Style style;
        // Offset in the Y axis
        int y_offset = 0;
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

    // Return the width of a char
    inline unsigned short _char_width(char character, FT_Face& face) {
        // Configure and load the FreeType glyph system
        FT_UInt index = FT_Get_Char_Index(face, (static_cast<unsigned char>(character)));
        FT_Error error = FT_Load_Glyph(face, index, FT_LOAD_NO_BITMAP);
        FT_GlyphSlot binary_datas = face->glyph;
        error = FT_Render_Glyph(binary_datas, FT_RENDER_MODE_NORMAL);

        std::cout << "U " << binary_datas->bitmap.width << std::endl;

        return static_cast<unsigned short>(binary_datas->bitmap.width);
    };

    // Return a pointer to an image with a char on it
    inline Image* _char_image(char character, FT_Face& face, int& cursor_pos, int& y_pos, Color color) {
        // Configure and load the FreeType glyph system
        FT_UInt index = FT_Get_Char_Index(face, (static_cast<unsigned char>(character)));
        FT_Error error = FT_Load_Glyph(face, index, 0);
        FT_GlyphSlot binary_datas = face->glyph;
        error = FT_Render_Glyph(binary_datas, FT_RENDER_MODE_NORMAL);

        // Create and draw the image
        unsigned short height = static_cast<unsigned short>(binary_datas->bitmap.rows);
        unsigned short width = static_cast<unsigned short>(binary_datas->bitmap.width);
        Image* img = new Image();
        img->_load_from_text_binary(reinterpret_cast<char*>(binary_datas->bitmap.buffer), width, height, color.red(), color.green(), color.blue(), color.alpha());

        // Get the position of the cursor
        cursor_pos = binary_datas->bitmap_left;
        y_pos = binary_datas->bitmap_top - height;

        return img;
    };

    // Return a pointer to an image with a text on it
    static Image* _line_image(std::string content, Text_Image_Data datas) {
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
    static Image* text_image(std::string content, Text_Image_Data datas) {
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

    class _Balise_Container {
        // Class faciliting the handle of balises
    public:
        // Most simple _Balise_Container constructor
        _Balise_Container() {

        };
        // _Balise_Container destructor
        ~_Balise_Container() {

        };

        // If the generator contains the style of a balise
        inline bool contains_defined_balise(std::string balise_name) {
            for(std::map<std::string, Text_Balise>::iterator it = a_defined_balises.begin();it!=a_defined_balises.end();it++){
                if(it->first == balise_name) return true;
            }
            return false;
        };
        // Return the style of a balise
        inline Text_Balise defined_balise(std::string balise_name) {
            for(std::map<std::string, Text_Balise>::iterator it = a_defined_balises.begin();it!=a_defined_balises.end();it++){
                if(it->first == balise_name) return it->second;
            }
            return Text_Balise();
        };

        // Load the built-ins balises
        void _load_built_in_balises() {
            Text_Balise current_balise;
            // Create the <h1> style
            current_balise.is_paragraph = true;
            current_balise.style.alignment_horizontal = Text_Alignment_Horizontal::Center;
            current_balise.style.color = red; current_balise.style.font_size = 50; current_balise.style.font = get_system_font("arialbd");
            a_defined_balises["h1"] = current_balise;
            // Create the <h2> style
            current_balise.is_paragraph = true;
            current_balise.style.alignment_horizontal = Text_Alignment_Horizontal::Left;
            current_balise.style.color = black; current_balise.style.font_size = 35; current_balise.style.font = get_system_font("arialbd");
            a_defined_balises["h2"] = current_balise;
        }
    private:
        // List of each defined balises
        std::map<std::string, Text_Balise> a_defined_balises = std::map<std::string, Text_Balise>();
    };

    class Text_Image {
        // Class containing an HTML text
    public:
        // Most simple Text_Image constructor
        Text_Image(_Balise_Container* defined_balises, std::string text) : a_defined_balises(defined_balises), a_text(text) {

        };
        // Text_Image destructor
        ~Text_Image() {

        };

        // Return an image with a block in it
        Image* _block(std::string block_text) {
            const std::string content = block_text;
            std::vector<_Text_Balise_Part> first_cutted = cut_string_by_balise(content, false, true);
            std::vector<_Text_Balise_Part> cutted = std::vector<_Text_Balise_Part>();
            for(int i = 0;i<first_cutted.size();i++) {
                if(first_cutted[i].content[0] == '<') {
                    cutted.push_back(first_cutted[i]);
                }
                else {
                    std::vector<std::string> space_cutted = cut_string(first_cutted[i].content, " ", false, true);
                    for(int j = 0;j<space_cutted.size();j++) {
                        _Text_Balise_Part part_to_add;
                        part_to_add.content = space_cutted[j];
                        cutted.push_back(part_to_add);
                    }
                }
            }

            bool is_balise = false;
            if(cutted[0].content[0] == '<') {
                std::string block_balise_name = balise_name(cutted[0].content);

                // Check if the block is an entire balise
                unsigned int level = 0;
                for(int i = 0;i<cutted.size()-1;i++) {
                    if(cutted[i].content.size() > 2 && cutted[i].content[0] == '<' && cutted[i].content[cutted[i].content.size() - 1] == '>') {
                        std::string parsed_balise_name = balise_name(cutted[i].content);
                        if(parsed_balise_name == block_balise_name) {
                            if(cutted[i].content[1] == '/') {
                                level--;
                                if(level == 0) break;
                            }
                            else {
                                level++;
                            }
                        }
                    }
                    i++;
                }

                // If the block can be an entire balise
                if(level == 1) {
                    if(defined_balises()->contains_defined_balise(block_balise_name)) {
                        current_balises().push(block_balise_name);

                        apply_current_style();
                        cutted.erase(cutted.begin());
                        cutted.erase(cutted.end() - 1);
                        is_balise = true;
                    }
                }
            }

            // Create the needed configurations
            Color current_background_color = a_current_style.background_color;
            unsigned short current_font_size = a_current_style.font_size;

            // Create each words
            unsigned int current_width = 0;
            std::vector<_Text_Block_Part> image_parts = std::vector<_Text_Block_Part>();
            unsigned int max_width = 0;
            unsigned int min_x = 0;
            unsigned int min_y = 0;
            unsigned short space_width = static_cast<unsigned short>(static_cast<double>(current_font_size) / 2.0);
            unsigned int total_height = 0;
            for(int i = 0;i<static_cast<int>(cutted.size());i++) {
                if(cutted[i].content[0] == '<') {
                    // Apply a balise

                    std::string balise = cutted[i].content;
                    if(balise.size() <= 0 || balise[balise.size() - 1] != '>') {
                        print("Warnig", "SCLS Image \"Michelangelo\"", "A balise you want to parse is badly syntaxed.");
                        continue;
                    }

                    if(balise[1] == '/') {
                        std::string balise_content = balise.substr(2, balise.size() - 3);
                        std::vector<std::string> balise_cutted = cut_string(balise_content, " ", false, true);
                        if(balise_cutted[0] == "br") {
                            // Break a line
                            current_width -= space_width;
                            if(current_width > max_width) max_width = current_width;
                            current_width = 0;
                            total_height += current_font_size;
                        }
                    }
                    else {
                        std::string balise_content = balise.substr(1, balise.size() - 2);
                        std::string current_balise_name = balise_name(balise_content);

                        if(defined_balises(current_balise_name).is_paragraph) {
                            // A new block should be created
                            _Text_Balise_Part current_text_balise_part = cutted[i];

                            // Search the end of the block
                            i++;
                            unsigned int level = 1;
                            while(i < static_cast<int>(cutted.size())) {
                                if(cutted[i].content.size() > 2 && cutted[i].content[0] == '<' && cutted[i].content[cutted[i].content.size() - 1] == '>') {
                                    std::string parsed_balise_name = balise_name(cutted[i].content);
                                    if(parsed_balise_name == current_balise_name) {
                                        if(cutted[i].content[1] == '/') {
                                            level--;
                                            if(level == 0) break;
                                        }
                                        else {
                                            level++;
                                        }
                                    }
                                }
                                i++;
                            }

                            // Parse the new block
                            unsigned int block_size = (cutted[i].start_position + cutted[i].content.size()) - current_text_balise_part.start_position;
                            std::string block_content = content.substr(current_text_balise_part.start_position, block_size);
                            _Text_Block_Part part; part.is_paragraph = true;

                            // Create the image
                            Image* new_block = _block(block_content);
                            part.image = new_block;
                            part.style = current_style();

                            apply_current_style();
                            current_width = 0;
                            image_parts.push_back(part);
                            total_height += new_block->height();

                            continue;
                        }
                    }

                    // Add an empty part
                    if(i == 0) total_height += current_font_size;
                    _Text_Block_Part part;
                    image_parts.push_back(part);
                }
                else if(cutted[i].content != "") {
                    int y_position = 0;
                    Image* image = _word(cutted[i].content, y_position);
                    if(image != 0) {
                        // Check the last part
                        if(i == 0 || image_parts[image_parts.size() - 1].is_paragraph) total_height += current_font_size;

                        // Check max width
                        if(current_style().max_width != -1 && image->width() < current_style().max_width && current_width + image->width() > current_style().max_width) {
                            // Break a line
                            current_width -= space_width;
                            if(current_width > max_width) max_width = current_width;
                            current_width = 0;
                            total_height += current_font_size;
                        }

                        _Text_Block_Part part;
                        part.image = image; part.y_offset = (current_font_size - image->height()) - y_position;
                        if(-y_position > min_y) min_y = -y_position;
                        current_width += image->width();
                        current_width += space_width;
                        image_parts.push_back(part);
                    }
                }
            }
            current_width -= space_width;
            if(current_width > max_width) max_width = current_width;

            // Create the final image and clear memory
            apply_current_style();
            unsigned int current_x = 0;
            unsigned int current_y = 0;
            Image* final_image = new Image(max_width + min_x, total_height + min_y, current_background_color);
            for(int i = 0;i<static_cast<int>(image_parts.size());i++) {
                Image* image = image_parts[i].image; if(image == 0) {
                     // A part is a balise
                    std::string balise = cutted[i].content;
                    if(balise[1] == '/') {
                        std::string balise_content = balise.substr(2, balise.size() - 3);
                        std::vector<std::string> balise_cutted = cut_string(balise_content, " ", false, true);
                        if(balise_cutted[0] == "br") {
                            // Break a line
                            current_x = 0;
                            current_y += current_font_size;
                        }
                    }
                }
                else {
                    if(image_parts[i].is_paragraph) {
                        // A part is a paragraph
                        // Paste the image
                        int x = current_x;
                        int y = current_y;

                        if(image_parts[i].style.alignment_horizontal == Text_Alignment_Horizontal::Center) {
                            x = static_cast<int>(static_cast<double>(final_image->width()) / 2.0 - static_cast<double>(image->width()) / 2.0);
                        }

                        final_image->paste(image, x, y);
                        current_y += image->height();
                        delete image;
                        continue;
                    }

                    // Apply max width
                    if(current_style().max_width != -1 && current_x + image->width() > current_style().max_width) {
                        // Break a line
                        current_x = 0;
                        current_y += current_font_size;
                    }

                    int x = current_x;
                    int y = current_y + image_parts[i].y_offset;
                    final_image->paste(image, x, y);
                    current_x += image->width();
                    if(i != static_cast<int>(image_parts.size()) - 1) current_x += space_width;
                    delete image;
                }

                // Check if the next part demand a break of line
                if(i != static_cast<int>(image_parts.size()) - 1) {
                    if(image_parts[i + 1].is_paragraph) {
                        current_x = 0;
                        current_y += current_font_size;
                    }
                }
            }

            if(is_balise) {
                current_balises().pop();
            }

            return final_image;
        };
        // Return the entire text
        Image* image(std::string text) {
            // Create the needed configurations
            std::string content = html_formatted(text);

            return _block(content);
        };
        // Save the image as an image
        inline void save_image(std::string path) {Image* img = image(text());img->save_png(path);delete img;img = 0;};
        // Create a single word of the text without any balising and spaces
        Image* _word(std::string word, int& y_position) {
            // Base variables for the creation
            std::string path = a_current_style.font.font_path;
            if(path == "") path = get_system_font("arial").font_path;

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
            error = FT_Set_Pixel_Sizes(face, 0, a_current_style.font_size);

            // Create each characters
            std::vector<Image*> characters;
            std::vector<int> cursor_pos;
            int max_height = 0;
            int min_y_position = 0;
            int to_add_font_size = 0;
            unsigned int total_width = 0;
            std::vector<unsigned int> y_pos;
            for(int i = 0;i<static_cast<int>(word.size());i++)
            {
                int cursor_position = 0;
                int y_position = 0;
                Image* image = _char_image(word[i], face, cursor_position, y_position, a_current_style.color);
                characters.push_back(image);
                cursor_pos.push_back(total_width + cursor_position);
                if(cursor_pos[cursor_pos.size() - 1] < 0) cursor_pos[cursor_pos.size() - 1] = 0; // Avoid a little bug with X position
                if(static_cast<int>(image->height()) + y_position > max_height) max_height = image->height() + y_position;
                if(y_position < to_add_font_size) to_add_font_size = y_position;
                y_pos.push_back(y_position);
                if(y_position < min_y_position) min_y_position = y_position;
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
            Image* final_image = new Image(total_width, max_height - to_add_font_size, Color(0, 0, 0, 0));
            for(int i = 0;i<static_cast<int>(characters.size());i++)
            {
                if(characters[i] != 0)
                {
                    unsigned int x = cursor_pos[i];
                    final_image->paste(characters[i], x, y_pos[i]);
                    delete characters[i]; characters[i] = 0;
                }
            }
            y_position = min_y_position;

            return final_image;
        }

        // Apply a style to the text
        inline void apply_current_style() {
            if(current_balises().size() == 0) apply_global_style();
            else apply_style(defined_balises()->defined_balise(current_balises().top()).style);
        };
        inline void apply_global_style() {apply_style(global_style());};
        inline void apply_style(Text_Style style) {a_current_style = style;};

        // Getters and setters
        inline std::stack<std::string>& current_balises() {return a_current_balises;};
        inline Text_Style current_style() {return a_current_style;};
        inline _Balise_Container* defined_balises() {return a_defined_balises;};
        inline Text_Balise defined_balises(std::string balise) {return defined_balises()->defined_balise(balise);};
        inline Text_Style& global_style() {return a_global_style;};
        inline std::string text() {return a_text;};
    private:
        // Current style used for the formatting
        Text_Style a_current_style;
        // Current balise stack in the text
        std::stack<std::string> a_current_balises = std::stack<std::string>();

        // Global style in the text
        Text_Style a_global_style;

        // Containers of each defined balises
        _Balise_Container* a_defined_balises = 0;
        // HTML Text in the image
        std::string a_text = "";
    };

    class Text_Image_Generator {
        // Class simplifying the creation of text image
    public:
        // Most simple Text_Image_Generator constructor
        Text_Image_Generator() {
            defined_balises()->_load_built_in_balises();
        };
        // Text_Image_Generator destructor
        ~Text_Image_Generator() {
            delete a_defined_balises; a_defined_balises = 0;
        }

        // Save the image in a path
        void save_image(std::string path, std::string text) {Text_Image *img = new Text_Image(defined_balises(), text);img->save_image(path);delete img;img = 0;}

        // Getters and setters
        inline _Balise_Container* defined_balises() {return a_defined_balises;};
        inline Color global_color() const {return a_global_color;};
        inline Font global_font() {if(a_global_font.font_path == "") set_global_font(get_system_font("arial"));return a_global_font;};
        inline unsigned short global_font_size() const {return a_global_font_size;};
        inline unsigned short global_out_offset_width_bottom() const {return a_global_out_offset_width_bottom;};
        inline unsigned short global_out_offset_width_left() const {return a_global_out_offset_width_left;};
        inline unsigned short global_out_offset_width_right() const {return a_global_out_offset_width_right;};
        inline unsigned short global_out_offset_width_top() const {return a_global_out_offset_width_top;};
        inline Text_Style global_style() const {return a_global_style;};
        inline void set_global_color(Color new_global_color) {a_global_color = new_global_color;};
        inline void set_global_font(Font new_font) {a_global_font = new_font;};
        inline void set_global_font_size(unsigned short new_global_font_size) {a_global_font_size = new_global_font_size;};
        inline void set_global_out_offset_width_bottom(unsigned short new_global_out_offset_width_bottom) {a_global_out_offset_width_bottom = new_global_out_offset_width_bottom;};
        inline void set_global_out_offset_width_left(unsigned short new_global_out_offset_width_left) {a_global_out_offset_width_left = new_global_out_offset_width_left;};
        inline void set_global_out_offset_width_right(unsigned short new_global_out_offset_width_right) {a_global_out_offset_width_right = new_global_out_offset_width_right;};
        inline void set_global_out_offset_width_top(unsigned short new_global_out_offset_width_top) {a_global_out_offset_width_top = new_global_out_offset_width_top;};
    private:
        // Global color in the text
        Color a_global_color = black;
        // Global font in the text
        Font a_global_font;
        // Global font size in the text
        unsigned short a_global_font_size = 20;
        // Global out bottom offset
        unsigned short a_global_out_offset_width_bottom = 0;
        // Global out left offset
        unsigned short a_global_out_offset_width_left = 0;
        // Global out right offset
        unsigned short a_global_out_offset_width_right = 0;
        // Global out top offset
        unsigned short a_global_out_offset_width_top = 0;
        // Global style of the text
        Text_Style a_global_style;

        // Containers of each defined balises
        _Balise_Container* a_defined_balises = new _Balise_Container();

        // Currently used attributes
        Text_Alignment_Horizontal current_text_alignment_horizontal = Text_Alignment_Horizontal::Center;
        Color current_background_color = white;
        Color current_color = black;
        Font current_font;
        unsigned short current_font_size = 20;
    };
}

#endif // SCLS_IMAGE_TEXT
