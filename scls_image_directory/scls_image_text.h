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
#if defined(__WIN32__) || defined(__WIN64__) // With Windows
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
	inline std::map<std::string, Font> __system_fonts = std::map<std::string, Font>();

	// Returns if a certain system font is stored in the program or not
	inline bool contains_system_font(std::string font) {
	    for(std::map<std::string, Font>::iterator it = __system_fonts.begin();it!=__system_fonts.end();it++) {
            if(it->first == font) return true;
	    }
	    return false;
	};

	// Load all the fonts system fonts
	inline void load_system_font() {
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
	inline Font get_system_font(std::string font, bool bold = false, bool italic = false, bool condensed = false, bool light = false) {
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
	inline void print_system_fonts() {
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
	static FT_Library  _freetype_library;
    static bool _free_type_library_inited = false;

	// Enumeration of each horizontal alignment possible
	enum Alignment_Horizontal {
	    H_Left, H_Center, H_Right, H_User_Defined
	};

	// Enumeration of each vertical alignment possible
	enum Alignment_Vertical {
	    V_Top, V_Center, V_Bottom, V_User_Defined
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
        Alignment_Horizontal alignment = Alignment_Horizontal::H_Left;

        // Out offset
        unsigned short out_offset_bottom_width = 0;
        unsigned short out_offset_left_width = 0;
        unsigned short out_offset_right_width = 0;
        unsigned short out_offset_top_width = 0;
    };

    // Style about a text
    struct Text_Style {
        // Horizontal alignment of the text
        Alignment_Horizontal alignment_horizontal = Alignment_Horizontal::H_Left;
        // Color of the background color
        Color background_color = white;
        // Color of the text
        Color color = black;
        // Font of the style
        Font font;
        // Font size of the style
        unsigned short font_size = 20;
        // Max width of the text (only in pixel for now)
        double max_width = -1;
        // Height pos of the text offset
        double text_offset_height = 0;
        // Width pos of the text offset
        double text_offset_width = 0;
        // X pos of the text offset
        double text_offset_x = 0;
        // Y pos of the text offset
        double text_offset_y = 0;
    };

    // Block of text in a text
    struct Text_Block {
        // Content of the block
        std::string content = "";
    };

    // Part of a block of a text representing a word to draw
    struct _Text_Block_Part {
        // If the block should be ignored or not
        bool ignore = false;
        // Pointer to the image
        Image* image = 0;
        // If the part is filled or not
        bool is_filled = true;
        // If the part is a paragraph or not
        bool is_paragraph = false;
        // Line of this block
        unsigned int line = 0;
        // Style of the part
        Text_Style style;
        // Offset in the Y axis
        int y_offset = 0;
    };

    // Datas about a maker of _Text_Block_Part
    struct _Text_Block_Part_Maker {
        // Height of the cursor
        int cursor_height = 0;
        // If the block contains the cursor
        bool has_cursor = false;
        // All created _Text_Block_Part by the maker
        std::vector<_Text_Block_Part> image_parts = std::vector<_Text_Block_Part>();
        // All width of each lines made by the maker
        std::vector<unsigned int> lines_width = std::vector<unsigned int>();
        // Max width of a line made
        int max_width = 0;
        // Minimim x value made
        unsigned int min_x = 0;
        // Minimim y value made
        unsigned int min_y = 0;
        // Total height of the block
        unsigned int total_height = 0;
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

        return static_cast<unsigned short>(binary_datas->bitmap.width);
    };

    // Return a pointer to an image with a char on it
    inline Image* _char_image(char character, FT_Face& face, int& cursor_pos, unsigned int& real_width, int& y_pos, Color color) {
        // Configure and load the FreeType glyph system
        FT_UInt index = FT_Get_Char_Index(face, (static_cast<unsigned char>(character)));
        FT_Error error = FT_Load_Glyph(face, index, 0);
        FT_GlyphSlot binary_datas = face->glyph;
        error = FT_Render_Glyph(binary_datas, FT_RENDER_MODE_NORMAL);
        FT_Glyph_Metrics metrics = binary_datas->metrics;

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
        if(__system_fonts.size() <= 0) load_system_font();

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
            if(datas.alignment == H_Center)x = min_x + static_cast<int>(static_cast<float>(max_width - image->width()) / 2.0);
            else if(datas.alignment == H_Right) x = min_x + max_width - image->width();
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

    //*********
	//
	// Text classes
	//
	//*********

	// Balise in a text
    struct Balise_Style_Datas : public Balise_Datas {
        // Style of the balis
        Text_Style style;
    };

    class _Balise_Style_Container : public _Balise_Container {
        // Class faciliting the handle of balises, with style
    public:

        // Most simple _Balise_Style_Container constructor
        _Balise_Style_Container() {};
        // _Balise_Style_Container destructor
        ~_Balise_Style_Container() {};

        // Return a style balise
        inline Balise_Style_Datas* defined_balise_style(std::string balise_name) { return reinterpret_cast<Balise_Style_Datas*>(defined_balise(balise_name)); };

        // Load the built-ins balises
        virtual void _load_built_in_balises() {
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
            current_balise.get()->style.color = red; current_balise.get()->style.font_size = 50; current_balise.get()->style.font = get_system_font("arialbd");
            set_defined_balise<Balise_Style_Datas>("h1", current_balise);
            // Create the <h2> style
            current_balise = std::make_shared<Balise_Style_Datas>();
            current_balise.get()->is_paragraph = true;
            current_balise.get()->style.alignment_horizontal = Alignment_Horizontal::H_Left;
            current_balise.get()->style.color = black; current_balise.get()->style.font_size = 35; current_balise.get()->style.font = get_system_font("arialbd");
            set_defined_balise<Balise_Style_Datas>("h2", current_balise);
        }
    };

	class Word_Datas {
        // Class containing the datas necessary for a word
    public:
        //*********
        //
        // Word_Datas mains functions
        //
        //*********

        // Most simple Word_Datas constructor
        Word_Datas() {};
        // Most simple Word_Datas constructor with only the content
        Word_Datas(std::string content) : a_content(content) {};
        // Simple Word_Datas constructor
        Word_Datas(std::string content, Text_Style style) : a_content(content), a_style(style) {};

        // Getters and setters
        inline std::string content() const {return a_content;};
        inline Text_Style style() const {return a_style;};

        //*********
        //
        // Cursor handling
        //
        //*********

        // Returns the position in the text at a X position
        inline unsigned int cursor_position_at_x(int x_position) {
            if(content().size() == 0 && a_characters_position.size() >= 0) return 0;

            unsigned int word_position = 0;
            while(word_position < a_characters_position.size() && a_characters_position[word_position] + a_characters_width[word_position] <= x_position) {
                word_position++;
            }

            if(word_position >= a_characters_position.size()) return content().size();
            if(static_cast<int>(x_position) - static_cast<int>(a_characters_position[word_position]) > static_cast<int>(a_characters_width[word_position] + a_characters_position[word_position]) - static_cast<int>(x_position)) {
                word_position++;
            }

            return word_position;
        };

        //*********
        //
        // Post-generation datas
        //
        //*********

        // Getters and setters
        inline std::vector<int>& characters_position() {return a_characters_position;};
        inline std::vector<unsigned int>& characters_width() {return a_characters_width;};
        inline void set_characters_position(std::vector<int> new_characters_position) {a_characters_position = new_characters_position;};
        inline void set_characters_width(std::vector<unsigned int> new_characters_width) {a_characters_width = new_characters_width;};
        inline void set_width(unsigned int new_width) {a_width = new_width;};
        inline void set_x_position(int new_x_position) {a_x_position = new_x_position;};
        inline void set_y_offset(short new_y_offset) {a_y_offset = new_y_offset;};
        inline unsigned int width() const {return a_width;};
        inline short y_offset() const {return a_y_offset;};
        inline int x_position() const {return a_x_position;};
    private:
        // Content of the line
        std::string a_content = "";
        // Style of the text
        Text_Style a_style;

        //*********
        //
        // Post-generation datas
        //
        //*********

        // X position of each characters in the word
        std::vector<int> a_characters_position = std::vector<int>();
        // Width of each characters in the word
        std::vector<unsigned int> a_characters_width = std::vector<unsigned int>();
        // Width of the image
        unsigned int a_width = 0;
        // X position of the word
        int a_x_position = 0;
        // Offset of the y position
        short a_y_offset = 0;
    };

    struct Line_Datas {
        // Struct containing the datas necessary for a line
        // Content of the line
        std::string content = "";
        // Content in plain text of the line
        std::string content_in_plain_text = "";
        // Number of the line (starting by 0)
        unsigned int line_number = 0;
        // Start of the text in the parent block in absolute text
        unsigned int start_position = 0;
        // Start of the text in the parent block in plain text
        unsigned int start_position_in_plain_text = 0;
    };

    // Type of Text_Image_Block
    enum Block_Type {
        BT_Always_Free_Memory,
        BT_Keep_Block_And_Line_In_Memory
    };

    struct Block_Datas {
        // Struct containing the datas necessary for a block
        // Block_Datas constructor
        Block_Datas(std::string block_content) : content(block_content) {}
        // Content of the block
        std::string content = "";
        // Global style in the block
        Text_Style global_style;
        // Max width of the block
        int max_width = 0;
        // Total height of the block
        int total_height = 0;
    };

    class Text_Image_Word {
        // Class containing a single word of text
    public:
        //*********
        //
        // Text_Image_Word mains functions
        //
        //*********

        // Most simple Text_Image_Word constructor to create an empty word
        Text_Image_Word() : a_datas(Word_Datas(" ")) {};
        // Simple Text_Image_Word constructor to create a word
        Text_Image_Word(Word_Datas datas) : a_datas(datas) {};
        // Most simple Text_Image_Word destructor
        ~Text_Image_Word() {};

        // Getters and setters
        inline void set_width(unsigned int new_width) {a_datas.set_width(new_width);};
        inline void set_x_position(int new_x_position) {a_datas.set_x_position(new_x_position);}
        inline void set_y_offset(short new_y_offset) { a_datas.set_y_offset(new_y_offset); };
        inline Text_Style style() const {return a_datas.style();};
        inline std::string text() const {return a_datas.content();};
        inline int x_position() const {return a_datas.x_position();};
        inline short y_offset() const {return a_datas.y_offset();};

        //*********
        //
        // Optimisation system
        //
        //*********

        // Generate the image of the word
        void generate_word() {
            // Create the needed configurations
            Color current_background_color = style().background_color;
            unsigned short current_font_size = style().font_size;

            // Base variables for the creation
            std::string path = style().font.font_path;
            if(path == "") path = get_system_font(DEFAULT_FONT).font_path;

            // Load the FreeType base system
            if(!_free_type_library_inited)
            {
                FT_Error error = FT_Init_FreeType(&_freetype_library);
                if ( error )
                {
                    print("Error", "SCLS", "Unable to load the FreeType engine.");
                    return;
                }
                _free_type_library_inited = true;
            }
            FT_Face face;
            FT_Error error = FT_New_Face(_freetype_library, path.c_str(), 0, &face);
            if (!std::filesystem::exists(path))
            {
                print("Error", "SCLS", "Unable to load the \"" + path + "\" font, it does not exist.");
                return;
            }
            else if ( error )
            {
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
            int max_height = 0;
            short min_y_position = 0;
            int to_add_font_size = 0;
            unsigned int total_width = 0;
            std::vector<unsigned int> y_pos;
            const std::string word = a_datas.content();
            for(int i = 0;i<static_cast<int>(word.size());i++)
            {
                int cursor_position = 0;
                unsigned int real_width = 0;
                int y_position = 0;
                Image* image = _char_image(word.at(i), face, cursor_position, real_width, y_position, style().color);
                characters.push_back(image); characters_x.push_back(total_width);
                cursor_pos.push_back(total_width + cursor_position);
                if(cursor_pos[cursor_pos.size() - 1] < 0) cursor_pos[cursor_pos.size() - 1] = 0; // Avoid a little bug with X position
                if(static_cast<int>(image->height()) + y_position > max_height) max_height = image->height() + y_position;
                if(y_position < to_add_font_size) to_add_font_size = y_position;
                y_pos.push_back(y_position);
                if(y_position < min_y_position) min_y_position = y_position;
                total_width += real_width;
                characters_width.push_back(real_width);
            }
            a_datas.set_characters_position(characters_x);
            a_datas.set_characters_width(characters_width);
            a_datas.set_y_offset(min_y_position);

            for(int i = 0;i<static_cast<int>(characters.size());i++)
            {
                if(characters[i] != 0)
                {
                    y_pos[i] = max_height - (characters[i]->height() + y_pos[i]);
                }
            }

            // Create the final image and clear the memory
            a_datas.set_width(total_width);
            a_last_image.reset(new Image(total_width, max_height - to_add_font_size, Color(0, 0, 0, 0)));
            unsigned char max_thread_number = 0;
            std::vector<unsigned int> positions = std::vector<unsigned int>();
            std::vector<std::thread*> threads = std::vector<std::thread*>();
            for(int i = 0;i<static_cast<int>(characters.size());i++)
            {
                if(characters[i] != 0)
                {
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
        // Paste a letter of a word in an image with thread system
        void __word_letter(Image* image_to_apply, Image* image_to_paste, unsigned int x, unsigned int y) {
            image_to_apply->paste(image_to_paste, x, y);
            delete image_to_paste; image_to_paste = 0;
        };

        // Getters and setters
        inline Word_Datas datas() const {return a_datas;};
        inline Image* image() { return a_last_image.get(); };
        std::shared_ptr<Image>& image_shared_pointer() {
            if(a_last_image.get() == 0) return a_last_image;
        };

    private:
        //*********
        //
        // Text_Image_Word mains attributes
        //
        //*********

        // Datas in the word
        Word_Datas a_datas;

        //*********
        //
        // Optimisation system
        //
        //*********

        // Last generated image
        std::shared_ptr<Image> a_last_image;
    };

    class Text_Image_Line {
        // Class containing a single line text
    public:
        //*********
        //
        // Text_Image_Line mains functions
        //
        //*********

        // Most simple Text_Image_Line constructor
        Text_Image_Line(_Balise_Style_Container* defined_balises, std::string text, Text_Style global_style) : a_defined_balises(defined_balises), a_global_style(global_style) {
            set_text(text);
        };
        // Text_Image_Line destructor
        ~Text_Image_Line() {free_memory();};

        // Returns the text in plain text
        inline std::string plain_text() const { return a_defined_balises->plain_text(text()); };
        // Returns the size of the line in plain text
        inline unsigned int plain_text_size() const { return plain_text().size(); };

        // Getters and setters
        inline Line_Datas datas() const {return a_datas;};
        inline Text_Style global_style() const {return a_global_style;};
        inline bool has_been_modified() const {return a_has_been_modified;};
        inline bool is_modified() const {return a_modified;};
        inline unsigned int line_start_position() const {return a_datas.start_position;};
        inline unsigned int line_start_position_in_plain_text() const {return a_datas.start_position_in_plain_text;};
        inline void set_datas(Line_Datas new_datas) {a_datas = new_datas;};
        inline void set_has_been_modified(bool new_has_been_modified) {a_has_been_modified = new_has_been_modified;};
        inline void set_line_start_position(unsigned int new_line_start_position) {a_datas.start_position = new_line_start_position;};
        inline void set_line_start_position_in_plain_text(unsigned int new_line_start_position_in_plain_text) {a_datas.start_position_in_plain_text = new_line_start_position_in_plain_text;};
        inline void set_modified(bool new_modified) {a_modified = new_modified;};
        inline void set_text(std::string new_text, bool move_cursor = true) {
            a_datas.content = new_text;
            a_datas.content_in_plain_text = a_defined_balises->plain_text(new_text);
            if(move_cursor) set_cursor_position_in_plain_text(a_datas.content_in_plain_text.size());
        };
        inline std::string text() const {return a_datas.content;};

        //*********
        //
        // Cursor handling
        //
        //*********

        // Returns the position of the cursor in plain text with a X position
        unsigned int cursor_position_in_plain_text_at_x(int x_position) {
            if(a_words_datas.size() == 0) return 0;

            unsigned int word_position = 0;
            int current_position = 0;
            while(word_position < a_words_datas.size() && a_words_datas[word_position].x_position() + a_words_datas[word_position].width() < x_position) {
                current_position += a_words_datas[word_position].content().size(); word_position++;
            }
            if(word_position >= a_words_datas.size()) return current_position;

            Word_Datas& word_to_apply = a_words_datas[word_position];

            return current_position + word_to_apply.cursor_position_at_x(x_position - word_to_apply.x_position());
        };

        // Getters and setters
        inline unsigned int cursor_position_in_plain_text() const {return a_cursor_position_in_plain_text;};
        inline int cursor_x() const {return a_cursor_x;};
        inline void set_cursor_position_in_plain_text(unsigned int new_cursor_position_in_plain_text) {a_cursor_position_in_plain_text = new_cursor_position_in_plain_text;};
        inline void set_use_cursor(bool new_use_cursor) {a_use_cursor = new_use_cursor;};
        inline bool use_cursor() const {return a_use_cursor;};

        //*********
        //
        // Optimisation system
        //
        //*********

        // Clear the memory from the words
        inline void clear_words() {for(int i = 0;i<static_cast<int>(a_words.size());i++) { if(a_words[i] != 0) delete a_words[i]; } a_words.clear(); };
        // Free the memory of the line
        inline void free_memory() {clear_words();a_last_image.reset();};
        // Generate the needed words
        void generate_words() {
            // Create the needed configurations
            Color current_background_color = a_global_style.background_color;
            unsigned short current_font_size = a_global_style.font_size;
            Text_Style current_style = a_global_style;

            // Create each words
            a_words_datas.clear();
            unsigned int current_position_in_plain_text = 0;
            unsigned int& current_width = a_current_width; current_width = 0;
            std::vector<_Text_Balise_Part> cutted = a_defined_balises->_cut_block(text());
            unsigned short space_width = static_cast<unsigned short>(static_cast<double>(current_font_size) / 2.0);
            std::vector<Text_Image_Word*>& words = a_words;
            short& y_offset = a_y_offset;
            for(int i = 0;i<static_cast<int>(cutted.size());i++) {
                Text_Image_Word* word_to_add = 0;
                Word_Datas data_to_add = Word_Datas();
                if(cutted[i].content.size() > 0 && cutted[i].content[0] == '<') {
                    // Apply a balise
                    std::string balise = cutted[i].content;
                    if(balise.size() <= 0 || balise[balise.size() - 1] != '>') {
                        print("Warning", "SCLS Image \"Michelangelo\"", "A balise you want to parse is badly syntaxed.");
                        continue;
                    }

                    std::string balise_content = formatted_balise(balise);
                    std::string current_balise_name = balise_name(balise_content);
                }
                else {
                    // Draw the image
                    std::string word_content = format_string_as_plain_text(cutted[i].content);
                    long long t = time_ns();
                    word_to_add = _generate_word(word_content, current_style, current_position_in_plain_text);
                    if(word_to_add != 0) {
                        word_to_add->set_x_position(current_width);
                        current_width += word_to_add->image()->width();
                        if(word_to_add->y_offset() < y_offset) y_offset = word_to_add->y_offset();
                    }
                    current_position_in_plain_text += word_content.size();
                }
                if(word_to_add != 0) data_to_add = word_to_add->datas();;

                // Add the part
                words.push_back(word_to_add);
                a_words_datas.push_back(data_to_add);
            }
        };
        // Generate a word
        Text_Image_Word* _generate_word(const std::string& word, const Text_Style& style, unsigned int start_position_in_plain_text) {
            // Create the word
            long long t = time_ns();
            Word_Datas datas = Word_Datas(word, style);
            Text_Image_Word* word_to_add = new Text_Image_Word(datas);
            word_to_add->generate_word();

            return word_to_add;
        };
        // Generates and returns an image of the line
        Image* image() {
            if(a_last_image.get() != 0) return a_last_image.get();

            // Generate the words
            generate_words();

            // Check for the cursor
            unsigned int cursor_width = 2;
            if(use_cursor() && cursor_position_in_plain_text() >= 0 && cursor_position_in_plain_text() <= datas().content_in_plain_text.size()) {
                if(static_cast<int>(a_words.size()) <= 0) {
                    a_current_width = cursor_width;
                }
            }

            // Draw the line
            unsigned int current_position_in_plain_text = 0;
            int current_x = 0;
            unsigned short space_width = static_cast<unsigned short>(static_cast<double>(global_style().font_size) / 2.0);
            a_last_image.reset(new Image(a_current_width, global_style().font_size - a_y_offset, Color(0, 0, 0, 0)));
            Image* final_image = a_last_image.get();
            for(int i = 0;i<static_cast<int>(a_words.size());i++) {
                Text_Image_Word* current_word = a_words[i];
                if(current_word != 0) {
                    Image* current_image = current_word->image();
                    if(current_image != 0) {
                        // Check for the cursor
                        if(use_cursor() && cursor_position_in_plain_text() >= 0 && cursor_position_in_plain_text() <= datas().content_in_plain_text.size()) {
                            if(cursor_position_in_plain_text() >= current_position_in_plain_text && cursor_position_in_plain_text() <= current_position_in_plain_text + current_word->text().size()) {
                                unsigned int local_cursor_position = cursor_position_in_plain_text() - current_position_in_plain_text;
                                if(local_cursor_position == current_word->text().size()) {
                                    a_cursor_x = current_x + current_image->width() - static_cast<double>(cursor_width) / 2.0;
                                }
                                else {
                                    a_cursor_x = current_x + current_word->datas().characters_position()[local_cursor_position];
                                }
                            }
                        }

                        // Paste the word
                        final_image->paste(current_image, current_x, global_style().font_size - (current_image->height() + current_word->y_offset()));
                        current_word->set_x_position(current_x); a_words_datas[i].set_x_position(current_x);
                        current_x += current_image->width();
                        current_position_in_plain_text += current_word->text().size();
                    }
                    else if(current_word->text() == " ") {
                        current_word->set_x_position(current_x); a_words_datas[i].set_x_position(current_x);
                        current_x += space_width;
                        current_position_in_plain_text++;

                        // Check for the cursor
                        if(use_cursor() && cursor_position_in_plain_text() >= 0 && cursor_position_in_plain_text() <= datas().content_in_plain_text.size()) {
                            if(cursor_position_in_plain_text() == current_position_in_plain_text) {
                                a_cursor_x = current_x;
                                if(i == static_cast<int>(a_words.size()) - 1) {
                                    a_cursor_x -= static_cast<double>(cursor_width) / 2.0;
                                }
                            }
                        }
                    }
                    delete current_word; a_words[i] = 0;
                }
            } clear_words();

            // Draw the cursor
            if(cursor_position_in_plain_text() == 0) a_cursor_x = 0;
            else a_cursor_x -= static_cast<double>(cursor_width) / 2.0;
            unsigned int cursor_height = final_image->height();
            if(use_cursor() && cursor_position_in_plain_text() >= 0 && cursor_position_in_plain_text() <= datas().content_in_plain_text.size()) {
                final_image->fill_rect(a_cursor_x, 0, cursor_width, final_image->height(), Color(0, 0, 0));
            }

            return final_image;
        }
        // Returns the shared image
        std::shared_ptr<Image>& shared_image() {return a_last_image;};
        // Paste a letter of a word in an image with thread system
        void __word_letter(Image* image_to_apply, Image* image_to_paste, unsigned int x, unsigned int y) {
            image_to_apply->paste(image_to_paste, x, y);
            delete image_to_paste; image_to_paste = 0;
        };

    private:
        //*********
        //
        // Text_Image_Line mains attributes
        //
        //*********

        // Current width of the line
        unsigned int a_current_width = 0;
        // Datas about the line
        Line_Datas a_datas;
        // Containers of each defined balises
        _Balise_Style_Container* a_defined_balises = 0;
        // Global style in the block
        Text_Style a_global_style;
        // If the line has been modified or not
        bool a_has_been_modified = true;
        // If the line is modified or not
        bool a_modified = false;
        // Y offset of the image
        short a_y_offset = 0;

        //*********
        //
        // Cursor handling
        //
        //*********

        // Position of the cursor in plain text position
        unsigned int a_cursor_position_in_plain_text = 0;
        // X position of the cursor
        int a_cursor_x = 0;
        // If the line is the cursor or not
        bool a_use_cursor = false;

        //*********
        //
        // Optimisation system
        //
        //*********

        // Last generated image
        std::shared_ptr<Image> a_last_image;
        // Last created words in the block
        std::vector<Text_Image_Word*> a_words = std::vector<Text_Image_Word*>();
        // Datas about the last created words in the block
        std::vector<Word_Datas> a_words_datas = std::vector<Word_Datas>();
    };

    class Text_Image_Block {
        // Class containing a single block text
    public:
        //*********
        //
        // Text_Image_Block mains functions
        //
        //*********

        // Text_Image_Block constructor with a Block_Datas and an user defined type
        Text_Image_Block(_Balise_Style_Container* defined_balises, std::shared_ptr<Block_Datas> datas, Block_Type type) : a_defined_balises(defined_balises), a_datas(datas), a_type(type) { set_text(a_datas.get()->content); };
        // Text_Image_Block constructor with a Block_Datas
        Text_Image_Block(_Balise_Style_Container* defined_balises, std::shared_ptr<Block_Datas> datas) : Text_Image_Block(defined_balises, datas, Block_Type::BT_Always_Free_Memory) {  };
        // Most simple Text_Image_Block constructor
        Text_Image_Block(_Balise_Style_Container* defined_balises, std::string text) : Text_Image_Block(defined_balises, text, Block_Type::BT_Always_Free_Memory) { };
        // Text_Image_Block constructor with an user defined type
        Text_Image_Block(_Balise_Style_Container* defined_balises, std::string text, Block_Type type) : Text_Image_Block(defined_balises, std::make_shared<Block_Datas>(text), type) {};
        // Text_Image_Block destructor
        ~Text_Image_Block() { free_memory(); };

        // Getters and setters
        inline Text_Style& global_style() {return a_datas.get()->global_style;};
        inline void set_text(std::string new_text, bool move_cursor = true) {a_datas.get()->content = new_text;update_line_text();};
        inline std::string text() const {return a_datas.get()->content;};
        inline Block_Type type() const {return a_type;};

        //*********
        //
        // Cursor handling
        //
        //*********

        // Returns the position of the cursor in plain text with a X position and a line
        inline unsigned int cursor_position_in_plain_text_at_line_and_x(unsigned int line, int x_position) {
            if(line < 0 || line >= lines().size()) return 0;

            Text_Image_Line* line_to_check = lines()[line];
            if(line_to_check == 0) return 0;
            return line_to_check->datas().start_position_in_plain_text + line_to_check->cursor_position_in_plain_text_at_x(x_position);
        };

        // Getters and setters
        inline Text_Image_Line* cursor_line() {return a_cursor_line;};
        inline unsigned int cursor_position_in_plain_text() const {return a_cursor_position_in_plain_text;};
        inline int cursor_x() const {return a_cursor_x;};
        inline int cursor_y() const {return a_cursor_y;};
        inline void set_cursor_position_in_plain_text(unsigned int new_cursor_position_in_plain_text) {a_cursor_position_in_plain_text = new_cursor_position_in_plain_text;};
        inline void set_use_cursor(bool new_use_cursor) {a_use_cursor = new_use_cursor;};
        inline bool use_cursor() const {return a_use_cursor;};

        //*********
        //
        // Optimisation system
        //
        //*********

        // Add text to the block
        void add_text(const std::vector<std::string>& cutted, const std::string& final_text, const std::string& text, unsigned int start_position) {
            a_datas.get()->content = final_text;
            int line_number = line_number_at_position(start_position);
            if(line_number != -1) {
                // Modify the text
                Line_Datas datas = a_lines_text[line_number];
                unsigned int inline_start_position = start_position - datas.start_position;
                std::string end_of_first_line = "";
                if(datas.content.size() - inline_start_position > 0) end_of_first_line = datas.content.substr(inline_start_position, datas.content.size() - inline_start_position);
                datas.content = datas.content.substr(0, inline_start_position) + cutted.at(0);
                datas.content_in_plain_text = a_defined_balises->plain_text(datas.content);
                a_lines_text[line_number] = datas;

                // Modify the needed lines
                if(line_number < a_lines.size()) {
                    if(a_lines[line_number] != 0) a_lines[line_number]->set_modified(true);
                    for(int i = 1;i<cutted.size();i++) {
                        a_lines.insert(a_lines.begin() + line_number + i, 0);
                    }
                }
                unsigned int current_plain_text_position = datas.start_position_in_plain_text + datas.content_in_plain_text.size() + 1;
                unsigned int current_position = datas.start_position + datas.content.size() + 5;
                for(int i = 1;i<cutted.size();i++) {
                    datas.content = cutted.at(i); datas.content_in_plain_text = a_defined_balises->plain_text(cutted.at(i));
                    datas.line_number = line_number + i;
                    datas.start_position = current_position; datas.start_position_in_plain_text = current_plain_text_position;
                    current_position += datas.content.size() + 5; current_plain_text_position += datas.content_in_plain_text.size() + 1;
                    a_lines_text.insert(a_lines_text.begin() + line_number + 1, datas);
                    line_number++;
                }
                datas.content += end_of_first_line;
                datas.content_in_plain_text += a_defined_balises->plain_text(end_of_first_line);
                a_lines_text[line_number] = datas;

                // Modify the next lines
                unsigned int plain_text_size = a_defined_balises->plain_text_size(text);
                for(int i = line_number + 1;i<static_cast<int>(a_lines_text.size());i++) {
                    a_lines_text[i].line_number += cutted.size() - 1;
                    a_lines_text[i].start_position += text.size();
                    a_lines_text[i].start_position_in_plain_text += plain_text_size;
                }
            }
            else set_text(final_text);
        };
        // Check the modified lines
        void _check_modified_lines() {
            for(int i = 0;i<static_cast<int>(a_lines.size());i++) {
                if(a_lines[i] != 0) {
                    if(a_lines[i]->is_modified()) {
                        delete a_lines[i]; a_lines[i] = 0;
                    }
                }
            }
        };
        // Clear the memory from the lines
        inline void clear_lines() {for(int i = 0;i<static_cast<int>(a_lines.size());i++) { if(a_lines[i] != 0) delete a_lines[i]; } a_lines.clear(); };
        // Delete the useless generated lines
        inline void delete_useless_generated_lines() {
            std::vector<Text_Image_Line*>& lines = a_lines;
            std::vector<Line_Datas>& cutted = a_lines_text;
            for(int i = 0;i<static_cast<int>(lines.size()) - static_cast<int>(cutted.size());i++) {
                delete lines[lines.size() - 1];
                lines.pop_back();
            }
        };
        // Free the memory of the line
        inline void free_memory() {clear_lines();};
        // Generate a single line of text
        Text_Image_Line* _generate_line(Line_Datas datas, const Text_Style& style) {
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
        // Generate the lines of the block
        void generate_lines(bool entirely = true) {
            if(!entirely && type() != Block_Type::BT_Always_Free_Memory) {
                _regenerate_lines();
            }
            else {
                free_memory();
                _regenerate_lines();
            }
        };
        // Generates the next line to be generated and returns it
        Text_Image_Line* generate_next_line(unsigned int line_number) {
            // Cut the text by line and delete useless lines
            Text_Style& current_style = a_current_style;
            std::vector<Line_Datas>& cutted = a_lines_text;
            std::vector<Text_Image_Line*>& lines = a_lines;
            int& max_width = a_datas.get()->max_width;
            int& total_height = a_datas.get()->total_height;

            // Check if the line is modified or not
            Text_Image_Line* current_line = 0;
            unsigned int current_line_x = 0;
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
        };
        // Generates the next line to be generated and returns it
        Text_Image_Line* generate_next_line() {
            Text_Image_Line* to_return = generate_next_line(a_current_line);
            a_current_line++;
            return to_return;
        };
        // Generates and returns an image with the block on it
        Image* image() {
            if(a_last_image.get() != 0) return a_last_image.get();

            // Generate the lines
            generate_lines();

            // Draw the final image
            unsigned int current_x = 0;
            unsigned int current_y = 0;
            unsigned char a_line_pasting_max_thread_number = 0;
            int& max_width = a_datas.get()->max_width;
            unsigned int min_x = 0;
            unsigned int min_y = 0;
            std::vector<std::thread*> threads = std::vector<std::thread*>();
            int& total_height = a_datas.get()->total_height;
            Image* to_return = new Image(max_width, total_height, Color(0, 0, 0, 0));
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
                            std::thread* current_thread = new std::thread(&Text_Image_Block::__image_paste, this, to_return, current_image, current_x, current_y);
                            threads.push_back(current_thread);

                            current_y += height_to_apply;
                        }
                        else {
                            unsigned int height_to_apply = current_image->height();
                            Text_Image_Block::__image_paste(to_return, current_image, current_x, current_y);
                            current_y += height_to_apply;
                        }
                    }
                }
            }
            if(type() == Block_Type::BT_Always_Free_Memory) clear_lines();

            // Wait to each thread to finish
            for(int i = 0;i<static_cast<int>(threads.size());i++) {
                threads[i]->join();
                delete threads[i];
            } threads.clear();

            a_last_image.reset(to_return);
            return to_return;
        };
        // Returns the shared pointer of the image and generates it if needed
        std::shared_ptr<Image>& image_shared_pointer() { if(a_last_image.get() == 0) image(); return a_last_image;};
        // Returns the line at a plain text position given, or 0
        Text_Image_Line* line_at_position_in_plain_text(unsigned int position) {
            int final_position = line_number_at_position_in_plain_text(position);
            if(final_position == -1) return 0;
            return lines()[final_position];
        };
        // Returns the line number at the position given, or 0
        int line_number_at_position(unsigned int position) {
            for(int i = 0;i<static_cast<int>(lines_datas().size());i++) {
                if(lines_datas()[i].start_position + lines_datas()[i].content.size() >= position) {
                    return i;
                }
            }
            return -1;
        };
        // Returns the line number at a plain text position given, or 0
        int line_number_at_position_in_plain_text(unsigned int position) {
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
        // Pastes an image on an another image for multi threading
        void __image_paste(Image* block, Image* image_2, unsigned int current_x, unsigned int current_y) {
            block->paste(image_2, current_x, current_y);
        };
        // Regenerate the lines with a new text
        void _regenerate_lines() {
            std::vector<Text_Image_Line*>& lines = a_lines;
            std::vector<Line_Datas>& cutted = a_lines_text;
            reset_line_generation();
            for(int i = 0;i<static_cast<int>(cutted.size());i++) {
                generate_next_line(i);
            }
            delete_useless_generated_lines();
        }
        // Removes a part of the text and returns the number of lines deleted
        unsigned int remove_text(const std::string& final_text, unsigned int size_to_delete, unsigned int size_to_delete_in_plain_text, unsigned int start_position) {
            unsigned int line_deleted = 0;
            int position = line_number_at_position(start_position);
            if(position != -1) {
                Line_Datas* current_datas = &lines_datas()[position];
                const unsigned int first_size_to_delete = size_to_delete;
                const unsigned int first_size_to_delete_in_plain_text = size_to_delete_in_plain_text;
                unsigned int local_position = start_position - current_datas->start_position;
                std::string* text_to_remove = &(current_datas->content);
                std::string* text_to_remove_in_plain_text = &(current_datas->content_in_plain_text);

                // Remove the needed text
                std::string content_to_keep = text_to_remove->substr(local_position, text_to_remove->size() - local_position);
                (*text_to_remove) = text_to_remove->substr(0, local_position);
                while(size_to_delete > 0 && position >= 0) {
                    local_position = start_position - current_datas->start_position;
                    if(static_cast<int>(local_position) - static_cast<int>(size_to_delete) >= 0) {
                        (*text_to_remove) = text_to_remove->substr(0, text_to_remove->size() - size_to_delete);
                        size_to_delete = 0;

                        if(position < lines().size()) {
                            lines()[position]->set_modified(true);
                        }
                    }
                    else {
                        size_to_delete -= local_position + 5;
                        if(lines().size() > position) {
                            if(lines()[position] != 0) delete lines()[position];
                            lines().erase(lines().begin() + position);
                        }
                        if(position > 0) {
                            current_datas = &lines_datas()[position - 1];
                            position--;
                            text_to_remove = &(current_datas->content);
                            text_to_remove_in_plain_text = &(current_datas->content_in_plain_text);
                        }
                        else current_datas = 0;
                        lines_datas().erase(lines_datas().begin() + position + 1);
                        line_deleted++;
                    }
                }
                if(position < lines().size()) { lines()[position]->set_modified(true); }
                (*text_to_remove) += content_to_keep;
                (*text_to_remove_in_plain_text) = a_defined_balises->plain_text(*text_to_remove);

                // Modify the next lines
                for(int i = position + 1;i<static_cast<int>(a_lines_text.size());i++) {
                    a_lines_text[i].start_position -= first_size_to_delete;
                    a_lines_text[i].start_position_in_plain_text -= first_size_to_delete_in_plain_text;
                }
            }
            return line_deleted;
        };
        // Reset the generation of lines
        inline void reset_line_generation() {
            _check_modified_lines();
            a_current_line = 0;
            a_current_style = global_style();
            a_datas.get()->max_width = 0;
            a_datas.get()->total_height = 0;
        };
        // Soft reset the block for a line renegeration
        inline void soft_reset() {
            a_cursor_line = 0;
        };
        // Update the text in each lines, without others modification
        void update_line_text() {
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
                current_position += datas.content.size() + 5;
                current_position_in_plain_text += datas.content_in_plain_text.size() + 1;
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

        // Getters and setter
        inline Block_Datas* datas() {return a_datas.get();};
        inline std::vector<Text_Image_Line*>& lines() { return a_lines; };
        inline std::vector<Line_Datas>& lines_datas() { return a_lines_text; };
    private:
        //*********
        //
        // Text_Image_Block mains attributes
        //
        //*********

        // Containers of each defined balises
        _Balise_Style_Container* a_defined_balises = 0;
        // Datas about the block
        std::shared_ptr<Block_Datas> a_datas;
        // Last generated image
        std::shared_ptr<Image> a_last_image;
        // Type of the block
        const Block_Type a_type = Block_Type::BT_Always_Free_Memory;

        //*********
        //
        // Cursor handling
        //
        //*********

        // Last line to have the cursor
        Text_Image_Line* a_cursor_line = 0;
        // Position of the cursor in plain text position
        unsigned int a_cursor_position_in_plain_text = 0;
        // X position of the cursor
        int a_cursor_x = 0;
        // Y position of the cursor
        int a_cursor_y = 0;
        // If the line is the cursor or not
        bool a_use_cursor = false;

        //*********
        //
        // Optimisation system
        //
        //*********

        // Current line which should be generated
        unsigned int a_current_line = 0;
        // Current style for the line generation
        Text_Style a_current_style;
        // Last created lines in the block
        std::vector<Text_Image_Line*> a_lines = std::vector<Text_Image_Line*>();
        // Last created lines text in the block
        std::vector<Line_Datas> a_lines_text = std::vector<Line_Datas>();
        // Maximum number of line generable in one time
        unsigned int a_max_line_number_generable = 30;
    };

    class Text_Image_Multi_Block {
        // Class containing a lot of block text
    public:
        // Most simple Text_Image constructor
        Text_Image_Multi_Block(_Balise_Style_Container* defined_balises, std::string text) : a_defined_balises(defined_balises) { set_text(text); };
        // Text_Image destructor
        ~Text_Image_Multi_Block() { __delete_blocks(); };

        // Generates and returns a block
        Text_Image_Block* _block(std::shared_ptr<Block_Datas> block) { Text_Image_Block* new_block = new Text_Image_Block(defined_balises(), block); return new_block; };
        // Deletes the blocks in the image
        void __delete_blocks(unsigned int max_size = 0) { a_blocks.clear(); };
        // Deletes the last blocks in a vector
        void free_memory() {__delete_blocks();};
        // Generate each blocks in the multiblocks
        void generate_blocks() {
            // Generate the needed datas
            update_blocks_datas();

            // Generate each blocks
            a_max_width = 0;
            a_total_height = 0;
            for(int i = 0;i<static_cast<int>(a_blocks_datas.size());i++) {
                std::shared_ptr<Text_Image_Block> new_block = std::make_shared<Text_Image_Block>(defined_balises(), a_blocks_datas[i]);
                new_block.get()->image();
                a_blocks.push_back(new_block);

                if(a_max_width < new_block.get()->datas()->max_width) a_max_width = new_block.get()->datas()->max_width;
                a_total_height += new_block.get()->datas()->total_height;
            }
        };
        // Return the entire text in an image
        Image* image(const std::string& text) {
            // Generate each blocks
            generate_blocks();
            int max_width = a_max_width;
            int total_height = a_total_height;

            // Create the final image and clear memory
            Color current_background_color = a_current_style.background_color;
            unsigned short current_font_size = a_current_style.font_size;
            unsigned int current_x = 0;
            unsigned int current_y = 0;
            Image* final_image = new Image(max_width, total_height, current_background_color);
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
                } a_blocks[i].reset();
            } __delete_blocks();

            return final_image;
        };
        // Return the entire text in an image
        Image* image() { return image(text()); };
        // Returns a shared pointer of the image
        std::shared_ptr<Image> image_shared_pointer() { std::shared_ptr<Image> to_return;to_return.reset(image());return to_return;};
        // Save the image as an image
        inline void save_image(std::string path) {Image* img = image();img->save_png(path);delete img;img = 0;};
        // Update the datas of each blocks
        void update_blocks_datas() {
            a_blocks_datas.clear();
            std::vector<std::string> cutted = a_defined_balises->_cut_multi_block(text());
            for(int i = 0;i<static_cast<int>(cutted.size());i++) {
                std::shared_ptr<Block_Datas> current_block_data = std::make_shared<Block_Datas>(cutted[i]);
                a_blocks_datas.push_back(current_block_data);
            }

            // Apply the style of each blocks
            for(int i = 0;i<static_cast<int>(a_blocks_datas.size());i++) {
                a_blocks_datas[i].get()->global_style = global_style();
            }
        };

        // Plain text handling
        // Returns the plain text of the image
        inline std::string plain_text() { return defined_balises()->plain_text(text()); };
        // Returns the size of the plain text in the image
        inline unsigned int plain_text_size() { return plain_text().size(); };

        // Getters and setters
        inline std::vector<std::shared_ptr<Text_Image_Block>>& blocks() {return a_blocks;};
        inline _Balise_Style_Container* defined_balises() {return a_defined_balises;};
        inline Balise_Style_Datas* defined_balises(std::string balise) {return defined_balises()->defined_balise_style(balise);};
        inline Text_Style& global_style() {return a_global_style;};
        inline unsigned char line_pasting_max_thread_number() const {return a_line_pasting_max_thread_number;};
        inline void set_line_pasting_max_thread_number(unsigned char new_line_pasting_max_thread_number) {a_line_pasting_max_thread_number = new_line_pasting_max_thread_number;};
        inline void set_text(std::string new_text, bool move_cursor = true) {a_text = new_text;};
        inline std::string text() const {return a_text;};
    private:
        // Current style used for the formatting
        Text_Style a_current_style;
        // Global style in the text
        Text_Style a_global_style;

        // Containers of each defined balises
        _Balise_Style_Container* a_defined_balises = 0;
        // HTML Text in the image
        std::string a_text = "";

        //*********
        //
        // Optimisation system
        //
        //*********

        // Last created blocks
        std::vector<std::shared_ptr<Text_Image_Block>> a_blocks = std::vector<std::shared_ptr<Text_Image_Block>>();
        // Last created blocks datas
        std::vector<std::shared_ptr<Block_Datas>> a_blocks_datas = std::vector<std::shared_ptr<Block_Datas>>();
        // Max numbers of thread to paste the lines
        unsigned char a_line_pasting_max_thread_number = 0;
        // Max width in the image
        int a_max_width = 0;
        // Total height of the image
        int a_total_height = 0;
    }; //*/

    class Text_Image_Generator : public _Balise_Style_Container {
        // Class simplifying the creation of text image
    public:
        // Most simple Text_Image_Generator constructor
        Text_Image_Generator() {
            _load_built_in_balises();
        };
        // Text_Image_Generator destructor
        ~Text_Image_Generator() {

        }

        // Create an image from a text and return it
        inline Image* image(std::string text) {Text_Image_Block *img = new Text_Image_Block(this, text);Image* to_return=img->image();delete img;img = 0;return to_return;};
        // Returns a newly created text image
        inline Text_Image_Block* new_text_image_block(std::string text, Block_Type type = Block_Type::BT_Always_Free_Memory) {Text_Image_Block *img = new Text_Image_Block(this, text, type);return img;};
        // Returns a newly created text image multi block
        inline Text_Image_Multi_Block* new_text_image_multi_block(std::string text) {Text_Image_Multi_Block *img = new Text_Image_Multi_Block(this, text);return img;};

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

        // Currently used attributes
        Alignment_Horizontal current_text_alignment_horizontal = Alignment_Horizontal::H_Center;
        Color current_background_color = white;
        Color current_color = black;
        Font current_font;
        unsigned short current_font_size = 20;
    };
}

#endif // SCLS_IMAGE_TEXT
