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

	// Hiddens variables
	static FT_Library  _freetype_library;
    static bool _free_type_library_inited = false;

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
}

#endif // SCLS_IMAGE_TEXT
