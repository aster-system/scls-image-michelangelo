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
        // Color of the text
        Color text_color = black;
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

    class Text_Image_Creator {
        // Class allowing to simplify the creation of text image
    public:
        // Most simple Text_Image_Creator constructor
        Text_Image_Creator(std::string text) : a_text(text) {
            _load_built_in_balises();
        };
        // Text_Image_Creator destructor
        ~Text_Image_Creator() {

        }

        // Returns each text block of the creator
        std::vector<Text_Block> blocks() {
            // Parse lines
            std::vector<Text_Block> all_blocks = std::vector<Text_Block>();
            std::string content = text();
            std::string last_line = "";
            for(int i = 0;i<static_cast<int>(content.size());i++) {
                if(content[i] == '<') {
                    i++;
                    if(i >= content.size()) {
                        print("Warnig", "SCLS Image \"Michelangelo\"", "A balise you want to start is badly syntaxed.");
                        return std::vector<Text_Block>();
                    }

                    if(content[i] == '/') {
                        // End a balise
                        i++;
                        if(i >= content.size()) {
                            print("Warnig", "SCLS Image \"Michelangelo\"", "A balise you want to end is badly syntaxed.");
                            return std::vector<Text_Block>();
                        }

                        std::string balise_content = "";
                        while(content[i] != '>') {
                            if(i >= content.size()) {
                                print("Warnig", "SCLS Image \"Michelangelo\"", "The balise \"" + balise_content + "\" you want to parse is badly syntaxed.");
                                return std::vector<Text_Block>();
                            }
                            balise_content += content[i];
                            i++;
                        }

                        if(balise_content == "br") {
                            // Break a line
                            if(last_line != "") {
                                Text_Block block; block.content = last_line;
                                all_blocks.push_back(block);
                            }
                            last_line = "";
                            continue;
                        }
                        else {
                            last_line += "</" + balise_content;
                        }
                    }
                    else {
                        std::string balise_content = "";
                        while(content[i] != '>') {
                            if(i >= content.size()) {
                                print("Warnig", "SCLS Image \"Michelangelo\"", "The balise \"" + balise_content + "\" you want to parse is badly syntaxed.");
                                return std::vector<Text_Block>();
                            }
                            balise_content += content[i];
                            i++;
                        }

                        std::vector<std::string> balise_cutted = cut_string(balise_content, " ", true);
                        if(contains_defined_balise(balise_cutted[0])) {
                            if(defined_balise(balise_cutted[0]).is_paragraph) {
                                // Get all the balise
                                i++;
                                if(i >= content.size()) {
                                    print("Warnig", "SCLS Image \"Michelangelo\"", "A balise you want to parse is badly syntaxed.");
                                    return std::vector<Text_Block>();
                                }

                                balise_content = "";
                                short balise_height = 1;
                                while(i < static_cast<int>(content.size())) {
                                    if(content[i] == '<') {
                                        i++;
                                        std::string temp_balise_content = "";
                                        while(content[i] != '>') {
                                            if(i >= content.size()) {
                                                print("Warnig", "SCLS Image \"Michelangelo\"", "The balise \"" + balise_content + "\" you want to parse is badly syntaxed.");
                                                return std::vector<Text_Block>();
                                            }
                                            temp_balise_content += content[i];
                                            i++;
                                        }

                                        if(temp_balise_content[0] == '/') {
                                            balise_height--;
                                            if(balise_height == 0) break;
                                            else {
                                                balise_content += "<" + temp_balise_content;
                                            }
                                        }
                                        else {
                                            balise_content += "<" + temp_balise_content;
                                            Text_Balise to_test = defined_balise(cut_string(temp_balise_content, " ")[0]);
                                            if(to_test.has_content) balise_height++;
                                        }
                                    }
                                    balise_content += content[i];

                                    if(i >= content.size()) {
                                        print("Warnig", "SCLS Image \"Michelangelo\"", "A balise you want to parse is badly syntaxed.");
                                        return std::vector<Text_Block>();
                                    }
                                    i++;
                                }

                                // Break a line
                                if(last_line != "") {
                                    Text_Block block; block.content = last_line;
                                    all_blocks.push_back(block);
                                }
                                if(balise_content != "") {
                                    Text_Block block; block.content = balise_content;
                                    all_blocks.push_back(block);
                                }
                                last_line = "";
                                continue;
                            }
                            else {
                                last_line += "<" + balise_content;
                            }
                        }
                        else {
                            last_line += "<" + balise_content;
                        }
                    }
                }
                last_line += content[i];
            }
            if(last_line != "") {
                Text_Block block; block.content = last_line;
                all_blocks.push_back(block);
            }
            return all_blocks;
        };
        // Create a line of the text
        Image* _line(std::string line) {
            // Base variables for the creation
            unsigned int font_size = current_font_size;
            std::string path = current_font.font_path;

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
            for(int i = 0;i<static_cast<int>(line.size());i++)
            {
                // Check the balising
                if(line[i] == '<') {
                    i++;
                    if(i >= line.size()) {
                        print("Warnig", "SCLS Image \"Michelangelo\"", "A balise you want to start is badly syntaxed.");
                        return 0;
                    }

                    if(line[i] == '/') {
                        // End a balist
                        i++;
                        if(i >= line.size()) {
                            print("Warnig", "SCLS Image \"Michelangelo\"", "A balise you want to end is badly syntaxed.");
                            return 0;
                        }

                        std::string balise_content = "";
                        while(line[i] != '>') {
                            if(i >= line.size()) {
                                print("Warnig", "SCLS Image \"Michelangelo\"", "The balise \"" + balise_content + "\" you want to parse is badly syntaxed.");
                                return 0;
                            }
                            balise_content += line[i];
                            i++;
                        }
                        end_balise(balise_content);
                    }
                    else {
                        // Start a balise
                        std::string balise_content = "";
                        while(line[i] != '>') {
                            if(i >= line.size()) {
                                print("Warnig", "SCLS Image \"Michelangelo\"", "The balise \"" + balise_content + "\" you want to parse is badly syntaxed.");
                                return 0;
                            }
                            balise_content += line[i];
                            i++;
                        }
                        start_balise(balise_content);
                    }
                    continue;
                }

                if(line[i] == ' ')
                {
                    characters.push_back(0);
                    cursor_pos.push_back(0);
                    y_pos.push_back(0);
                    total_width += font_size / 2.0;
                    continue;
                }
                int cursor_position = 0;
                int y_position = 0;
                Image* image = _char_image(line[i], face, cursor_position, y_position, current_color);
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
            Image* final_image = new Image(total_width, max_height - to_add_font_size, current_background_color);
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
        }
        // Return the entire text
        Image* image() {
            // Create the needed configurations
            std::string content = text();
            current_background_color = white;
            current_color = global_color();
            current_font = global_font();
            current_font_size = global_font_size();

            std::vector<Text_Block> all_blocks = blocks();

            // Create each lines
            std::vector<Image*> image_parts = std::vector<Image*>();
            unsigned int max_width = 0;
            unsigned int min_x = global_out_offset_width_left();
            unsigned int min_y = global_out_offset_width_top();
            unsigned int total_height = 0;
            for(int i = 0;i<static_cast<int>(all_blocks.size());i++) {
                if(all_blocks[i].content == "") {
                    image_parts.push_back(0);
                    total_height += current_font_size;
                }
                else {
                    Image* image = _line(all_blocks[i].content);
                    std::cout << "P " << all_blocks[i].content << std::endl;
                    if(image != 0)
                    {
                        image_parts.push_back(image);
                        total_height += image->height();
                        if(image->width() > max_width) max_width = image->width();
                    }
                }
            }

            // Create the final image and clear memory
            Image* final_image = new Image(max_width + min_x + global_out_offset_width_right(), total_height + min_y + global_out_offset_width_bottom(), current_background_color);
            unsigned int y_position = min_y;
            for(int i = 0;i<static_cast<int>(image_parts.size());i++) {
                Image* image = image_parts[i]; if(image == 0) { y_position += current_font_size; continue; }
                unsigned int x = min_x;
                if(current_text_alignment_horizontal == Text_Alignment_Horizontal::Center)x = min_x + static_cast<int>(static_cast<float>(max_width - image->width()) / 2.0);
                else if(current_text_alignment_horizontal == Text_Alignment_Horizontal::Right) x = min_x + max_width - image->width();
                final_image->paste(image, x, y_position); y_position += image->height();
                delete image_parts[i]; image_parts[i] = 0;
            }

            return final_image;
        };
        // Save the image in a path
        void save_image(std::string path) {Image* img = image();img->save_png(path);delete img;img = 0;}

        // Apply a style of a balise
        void apply_balise_style(std::string balise_name) {
            if(!contains_defined_balise(balise_name)) return;
            apply_style(defined_balise(balise_name).style);
        };
        // Apply a style of a balise
        void apply_balise_style(Text_Balise balise_style) {
            apply_style(balise_style.style);
        };
        // Apply a style
        void apply_style(Text_Style style) {
            current_color = style.text_color;
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
        // End a balise
        void end_balise(std::string balise) {
            if(a_balises.empty()) {
                print("Warnig", "SCLS Image \"Michelangelo\"", "A balise you want to end is badly syntaxed.");
                return;
            }
            a_balises.pop();
            if(a_balises.empty()) {
                apply_style(global_style());
            }
            else {
            apply_balise_style(balise);
        }
        };
        // Load the built-ins balises
        void _load_built_in_balises() {
            Text_Balise current_balise;
            // Create the <h1> style
            current_balise.is_paragraph = true;
            current_balise.style.text_color = red;
            a_defined_balises["h1"] = current_balise;
        }
        // Start a balise
        void start_balise(std::string balise) {
            std::vector<std::string> cutted = cut_string(balise, " ", true);

            if(cutted.size() <= 0) return;
            apply_balise_style(cutted[0]);

            a_balises.push(cutted[0]);
        };

        // Getters and setters
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
        inline void set_text(std::string new_text) {a_text = new_text;};
        inline std::string text() const {return a_text;};
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
        // Text in the creator
        std::string a_text = "";

        // Stack of the balising
        std::stack<std::string> a_balises = std::stack<std::string>();
        // List of each defined balises
        std::map<std::string, Text_Balise> a_defined_balises = std::map<std::string, Text_Balise>();

        // Currently used attributes
        Text_Alignment_Horizontal current_text_alignment_horizontal = Text_Alignment_Horizontal::Center;
        Color current_background_color = white;
        Color current_color = black;
        Font current_font;
        unsigned short current_font_size = 20;
    };
}

#endif // SCLS_IMAGE_TEXT
