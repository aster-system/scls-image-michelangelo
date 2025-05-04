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

#ifndef SCLS_IMAGE_TEXT
#define SCLS_IMAGE_TEXT

// Base path to the fonts in the system
#if defined(__WIN32__) || defined(__WIN64__) // With Windows
#define BASE_FONT_PATH std::string("C:\\Windows\\Fonts\\")
#define DEFAULT_FONT std::string("arial")
#endif
#ifdef __linux__ // With Linux
#ifndef BASE_FONT_PATH
#define BASE_FONT_PATH std::string("/usr/share/fonts/truetype/")
#endif // BASE_FONT_PATH
#ifndef DEFAULT_FONT
#define DEFAULT_FONT std::string("FreeSans")
#endif // DEFAULT_FONT
#endif

// The namespace "scls" is used to simplify the all.
namespace scls {
    //*********
	//
	// Font handling
	//
	//*********

	//Basic font datas
	extern std::string __base_font_path;
	extern std::string __default_font;

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

	// Returns if a certain system font is stored in the program or not
    bool contains_system_font(std::string font);
	// Load all the fonts system fonts
    void load_system_font();
	// Return the system path of a font
    Font get_system_font(std::string font, bool bold = false, bool italic = false, bool condensed = false, bool light = false);
    Font get_system_font();
	// Print each system fonts
    void print_system_fonts();

    //*********
	//
	// Text creation
	//
	//*********

	// Enumeration of each horizontal alignment possible
	enum Alignment_Horizontal {H_Left, H_Center, H_Right, H_User_Defined};

	// Enumeration of each vertical alignment possible
	enum Alignment_Vertical {V_Top, V_Center, V_Bottom, V_User_Defined};

	// Style about a text
    class Text_Style {
    public:

        // Text_Style constructor
        Text_Style(){};

        // Returns the block of this style
        inline Text_Style* block_style() const {return a_block_style.lock().get();};
        // Merges a style with this style
        void merge_style(Text_Style to_merge);
        // Returns the parent of this style
        inline Text_Style* parent_style() const {return a_parent_style.lock().get();};
        // Create a children for this style
        inline std::shared_ptr<Text_Style> new_child(){std::shared_ptr<Text_Style> child = std::make_shared<Text_Style>();child.get()->a_this_style=child;child.get()->a_parent_style=a_this_style;return child;};
        // Sets the shared ptr to the block style
        inline void set_block_style(std::shared_ptr<Text_Style> block_style){a_block_style=block_style;};
        // Sets the shared ptr to the parent style
        inline void set_parent_style(std::weak_ptr<Text_Style> parent_style){a_parent_style=parent_style;};
        // Sets the shared ptr to this style
        inline void set_this_style(std::weak_ptr<Text_Style> this_style){a_this_style=this_style;};

        // Getters and setters
        // Alignment horizontal
        inline Alignment_Horizontal alignment_horizontal() const {if(a_alignment_horizontal_modified || parent_style() == 0){return a_alignment_horizontal;}else{return parent_style()->alignment_horizontal();}};
        inline void set_alignment_horizontal(Alignment_Horizontal new_alignment_horizontal){a_alignment_horizontal=new_alignment_horizontal;a_alignment_horizontal_modified=true;};
        inline void unset_alignment_horizontal(){a_alignment_horizontal_modified=false;};
        // Background color
        inline Color background_color() const {if(a_background_color_modified || parent_style() == 0){return a_background_color;}else{return parent_style()->background_color();}};
        inline void set_background_color(Color new_background_color){a_background_color=new_background_color;a_background_color_modified=true;};
        inline void unset_background_color(){a_background_color_modified=false;};
        // Border
        inline int border_bottom_width() const {if(a_border_bottom_width_modified || (parent_style() == 0 && block_style() == 0)){return a_border_bottom_width;}else if(block_style() != 0){if(block_style()->border_bottom_width_modified()){return block_style()->border_bottom_width();}else if(parent_style()==0){return a_border_bottom_width;}}return parent_style()->border_bottom_width();};
        inline bool border_bottom_width_modified() const {return a_border_bottom_width_modified;};
        inline int border_left_width() const {if(a_border_left_width_modified || (parent_style() == 0 && block_style() == 0)){return a_border_left_width;}else if(block_style() != 0){if(block_style()->border_left_width_modified()){return block_style()->border_left_width();}else if(parent_style()==0){return a_border_left_width;}}return parent_style()->border_left_width();};
        inline bool border_left_width_modified() const {return a_border_left_width_modified;};
        inline int border_right_width() const {if(a_border_right_width_modified || (parent_style() == 0 && block_style() == 0)){return a_border_right_width;}else if(block_style() != 0){if(block_style()->border_right_width_modified()){return block_style()->border_right_width();}else if(parent_style()==0){return a_border_right_width;}}return parent_style()->border_right_width();};
        inline bool border_right_width_modified() const {return a_border_right_width_modified;};
        inline int border_top_width() const {if(a_border_top_width_modified || (parent_style() == 0 && block_style() == 0)){return a_border_top_width;}else if(block_style() != 0){if(block_style()->border_top_width_modified()){return block_style()->border_top_width();}else if(parent_style()==0){return a_border_top_width;}}return parent_style()->border_top_width();};
        inline bool border_top_width_modified() const {return a_border_top_width_modified;};
        inline void set_border_bottom_width(int new_border_bottom_width) {a_border_bottom_width = new_border_bottom_width;a_border_bottom_width_modified=true;};
        inline void set_border_left_width(int new_border_left_width) {a_border_left_width = new_border_left_width;a_border_left_width_modified=true;};
        inline void set_border_right_width(int new_border_right_width) {a_border_right_width = new_border_right_width;a_border_right_width_modified=true;};
        inline void set_border_top_width(int new_border_top_width) {a_border_top_width = new_border_top_width;a_border_top_width_modified=true;};
        inline void set_border_width(int new_border_width){set_border_bottom_width(new_border_width);set_border_left_width(new_border_width);set_border_right_width(new_border_width);set_border_top_width(new_border_width);};
        // Color
        inline Color color() const {if(a_color_modified || (parent_style() == 0 && block_style() == 0)){return a_color;}else if(block_style()!=0){if(block_style()->color_modified()){return block_style()->color();}else if(parent_style()==0){return a_color;}}return parent_style()->color();};
        inline bool color_modified() const {return a_color_modified;};
        inline void set_color(Color new_color){a_color=new_color;a_color_modified=true;};
        inline void unset_color(){a_color_modified=false;};
        // Font
        inline Font font()const{if(a_font_modified || (parent_style() == 0 && block_style() == 0)){return a_font;}else if(block_style()!=0){if(block_style()->font_modified()){return block_style()->font();}else if(parent_style()==0){return a_font;}}return parent_style()->font();};
        inline bool font_modified() const {return a_font_modified;};
        inline std::string font_path()const{return font().font_path;};
        inline void set_font(Font new_font){a_font=new_font;a_font_modified=true;}
        inline void set_font_path(std::string new_font_path){a_font.font_path=new_font_path;a_font_modified=true;};
        inline void unset_font(){a_font_modified=false;};
        // Font size
        inline unsigned short font_size() const {if(a_font_size_modified || (parent_style() == 0 && block_style() == 0)){return a_font_size;}else if(block_style()!=0){if(block_style()->font_size_modified()){return block_style()->font_size();}else if(parent_style()==0){return a_font_size;}}return parent_style()->font_size();};
        inline bool font_size_modified() const {return a_font_size_modified;};
        inline void set_font_size(unsigned short new_font_size){a_font_size=new_font_size;a_font_size_modified=true;};
        inline void unset_font_size(){a_font_size_modified=false;};
        // Margin
        inline int margin_bottom() const {if(a_margin_bottom_modified || (parent_style() == 0 && block_style() == 0)){return a_margin_bottom;}else if(block_style() != 0){if(block_style()->margin_bottom_modified()){return block_style()->margin_bottom();}else if(parent_style()==0){return a_margin_bottom;}}return parent_style()->margin_bottom();};
        inline bool margin_bottom_modified() const {return a_margin_bottom_modified;};
        inline int margin_left() const {if(a_margin_left_modified || (parent_style() == 0 && block_style() == 0)){return a_margin_left;}else if(block_style() != 0){if(block_style()->margin_left_modified()){return block_style()->margin_left();}else if(parent_style()==0){return a_margin_left;}}return parent_style()->margin_left();};
        inline bool margin_left_modified() const {return a_margin_left_modified;};
        inline int margin_right() const {if(a_margin_right_modified || (parent_style() == 0 && block_style() == 0)){return a_margin_right;}else if(block_style() != 0){if(block_style()->margin_right_modified()){return block_style()->margin_right();}else if(parent_style()==0){return a_margin_right;}}return parent_style()->margin_right();};
        inline bool margin_right_modified() const {return a_margin_right_modified;};
        inline int margin_top() const {if(a_margin_top_modified || (parent_style() == 0 && block_style() == 0)){return a_margin_top;}else if(block_style() != 0){if(block_style()->margin_top_modified()){return block_style()->margin_top();}else if(parent_style()==0){return a_margin_top;}}return parent_style()->margin_top();};
        inline bool margin_top_modified() const {return a_margin_top_modified;};
        inline void set_margin_bottom(int new_margin_bottom) {a_margin_bottom = new_margin_bottom;a_margin_bottom_modified=true;};
        inline void set_margin_left(int new_margin_left) {a_margin_left = new_margin_left;a_margin_left_modified=true;};
        inline void set_margin_right(int new_margin_right) {a_margin_right = new_margin_right;a_margin_right_modified=true;};
        inline void set_margin_top(int new_margin_top) {a_margin_top = new_margin_top;a_margin_top_modified=true;};
        // Padding
        inline int padding_bottom() const {return a_padding_bottom;};
        inline int padding_left() const {return a_padding_left;};
        inline int padding_right() const {return a_padding_right;};
        inline int padding_top() const {return a_padding_top;};
        inline void set_padding_bottom(int new_padding_bottom) {a_padding_bottom = new_padding_bottom;};
        inline void set_padding_left(int new_padding_left) {a_padding_left = new_padding_left;};
        inline void set_padding_right(int new_padding_right) {a_padding_right = new_padding_right;};
        inline void set_padding_top(int new_padding_top) {a_padding_top = new_padding_top;};

        // TEMPORARY
        // Max width of the text (only in pixel for now)
        int max_width = -1;
        // Height pos of the text offset
        double text_offset_height = 0;
        // Width pos of the text offset
        double text_offset_width = 0;
        // X pos of the text offset
        double text_offset_x = 0;
        // Y pos of the text offset
        double text_offset_y = 0;
        // Color of the border
        Color border_color = scls::Color(0, 0, 0);

    private:

        // Horizontal alignment of the text
        Alignment_Horizontal a_alignment_horizontal = Alignment_Horizontal::H_Left;
        bool a_alignment_horizontal_modified = false;
        // Color of the background color
        Color a_background_color = Color(255, 255, 255);
        bool a_background_color_modified = false;
        // Width of the border
        bool a_border_bottom_width_modified = false;bool a_border_left_width_modified = false;bool a_border_right_width_modified = false;bool a_border_top_width_modified = false;
        int a_border_bottom_width = 0;int a_border_left_width = 0;int a_border_right_width = 0;int a_border_top_width = 0;
        // Color font of the style
        Color a_color = Color(0, 0, 0);
        bool a_color_modified = false;
        // Font of the style
        Font a_font;
        bool a_font_modified = false;
        // Font size of the style
        unsigned short a_font_size = 20;
        bool a_font_size_modified = false;
        // Margin of the style
        int a_margin_bottom = 0;int a_margin_left = 0;int a_margin_right = 0;int a_margin_top = 0;
        bool a_margin_bottom_modified = false;int a_margin_left_modified = false;int a_margin_right_modified = false;int a_margin_top_modified = false;
        // Padding of the style
        int a_padding_bottom = 0;int a_padding_left = 0;int a_padding_right = 0;int a_padding_top = 0;

        // Block-style-arent of this style
        std::weak_ptr<Text_Style> a_block_style;
        // Parent of this style
        std::weak_ptr<Text_Style> a_parent_style;
        // Shared ptr to this style
        std::weak_ptr<Text_Style> a_this_style;
    };

    // Returns if an XML attribute is for a text style
    bool text_style_from_xml_attribute(XML_Attribute* attribute, Text_Style* style);
    // Handle a text style from XML
    void text_style_from_xml(std::shared_ptr<XML_Text> content, Text_Style* style);

    //*********
	//
	// Classes used to handle datas
	//
	//*********

	// Balise in a text
    struct Balise_Style_Datas : public Balise_Datas {Balise_Style_Datas(){style.get()->set_this_style(style);};std::shared_ptr<Text_Style> style=std::make_shared<Text_Style>();};;

    class _Balise_Style_Container : public __Balise_Container {
        // Class faciliting the handle of balises, with style
    public:

        // Most simple _Balise_Style_Container constructor
        _Balise_Style_Container() {};
        // _Balise_Style_Container destructor
        virtual ~_Balise_Style_Container() {};

        // Return a style balise
        inline Balise_Style_Datas* defined_balise_style(std::string balise_name) { return reinterpret_cast<Balise_Style_Datas*>(defined_balise(balise_name)); };

        // Load the built-ins balises
        virtual void _load_built_in_balises();

        //*********
        //
        // _Balise_Style_Container images
        //
        //*********

        // Adds an image to the container
        inline std::shared_ptr<Image>* add_image(std::string name, std::shared_ptr<Image> img) {std::shared_ptr<Image>* check_img=image(name);if(check_img!=0){return check_img;}a_images[name]=img;return &a_images[name];};
        inline std::shared_ptr<Image>* add_image(std::string name, std::string path) {std::shared_ptr<Image>* check_img=image(name);if(check_img!=0){return check_img;}return add_image(name, std::make_shared<scls::Image>(path));};
        // Returns if the container contains an image
        inline bool contains_image(std::string src) {for(std::map<std::string, std::shared_ptr<Image>>::iterator it = a_images.begin();it!=a_images.end();it++){if(it->first==src)return true;}return false;};
        inline std::shared_ptr<Image>* image(std::string src) {for(std::map<std::string, std::shared_ptr<Image>>::iterator it = a_images.begin();it!=a_images.end();it++){if(it->first==src)return &it->second;}return 0;};
        // Creates a new image in the container
        inline std::shared_ptr<Image>* new_image(std::string name, std::string img_path) {if(contains_image(name))return 0;std::shared_ptr<Image> img=std::make_shared<Image>(img_path);return add_image(name,img);};

    private:

        //*********
        //
        // _Balise_Style_Container images
        //
        //*********

        // Every loaded images, with their source name as key
        std::map<std::string, std::shared_ptr<Image>> a_images;
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
        Word_Datas(String content) : a_content(content) {};
        // Simple Word_Datas constructor
        Word_Datas(String content, Text_Style style) : a_content(content), a_style(style) {};

        // Getters and setters
        inline String content() const {return a_content;};
        inline Text_Style style() const {return a_style;};

        //*********
        //
        // Cursor handling
        //
        //*********

        // Returns the position in the text at a X position
        inline unsigned int cursor_position_at_x(int x_position);

        //*********
        //
        // Post-generation datas
        //
        //*********

        // Getters and setters
        inline String balise_content()const{return a_balise_content;};
        inline std::shared_ptr<XML_Text> balise_parent()const{return a_balise_parent;};
        inline short bottom_offset() const {return a_bottom_offset;};
        inline std::vector<int>& characters_position() {return a_characters_position;};
        inline std::vector<unsigned int>& characters_width() {return a_characters_width;};
        inline void set_balise_content(std::string new_balise_content){a_balise_content=new_balise_content;};
        inline void set_balise_parent(std::shared_ptr<XML_Text> new_balise_parent){a_balise_parent=new_balise_parent;};
        inline void set_bottom_offset(short new_bottom_offset) {a_bottom_offset = new_bottom_offset;};
        inline void set_characters_position(std::vector<int> new_characters_position) {a_characters_position = new_characters_position;};
        inline void set_characters_width(std::vector<unsigned int> new_characters_width) {a_characters_width = new_characters_width;};
        inline void set_top_offset(short new_top_offset) {a_top_offset = new_top_offset;};
        inline void set_width(unsigned int new_width) {a_width = new_width;};
        inline void set_x_position(int new_x_position) {a_x_position = new_x_position;};
        inline short top_offset() const {return a_top_offset;};
        inline unsigned int width() const {return a_width;};
        inline int x_position() const {return a_x_position;};
    private:
        // Content of the balise
        String a_balise_content;
        // Parent XML balise of this word
        std::shared_ptr<XML_Text> a_balise_parent;
        // Content of the line
        String a_content;
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

        // Offsets
        // Offset of the bottom position
        short a_bottom_offset = 0;
        // Offset of the top position
        short a_top_offset = 0;
    };

    struct Line_Datas {
        // Struct containing the datas necessary for a line
        // Line_Datas constructor
        Line_Datas(){a_global_style.get()->set_this_style(a_global_style);};

        // Content of the line
        std::shared_ptr<XML_Text> content;
        // Content in plain text of the line
        String content_in_plain_text;
        // Global style in the line
        std::shared_ptr<Text_Style> a_global_style = std::make_shared<Text_Style>();

        // Number of the line (starting by 0)
        unsigned int line_number = 0;
        // Start of the text in the parent block in absolute text
        unsigned int start_position = 0;
        // Start of the text in the parent block in plain text
        unsigned int start_position_in_plain_text = 0;
    };

    // Type of Text_Image_Block
    enum Block_Type {BT_Always_Free_Memory,BT_Keep_Block_And_Line_In_Memory};

    struct Block_Datas {
        // Struct containing the datas necessary for a block
        // Block_Datas constructor
        Block_Datas(std::shared_ptr<XML_Text> block_content, std::shared_ptr<Text_Style> new_global_style) : content(block_content),global_style(new_global_style) {global_style.get()->set_this_style(global_style);}
        Block_Datas(std::shared_ptr<XML_Text> block_content) : content(block_content) {global_style.get()->set_this_style(global_style);}

        // Adds text in the block
        void add_text(std::string text);

        // Content of the balise
        std::shared_ptr<XML_Text> content;

        // Global style in the block
        std::shared_ptr<Text_Style> global_style = std::make_shared<Text_Style>();

        // Max width of the block
        int max_width = 0;
        // Total height of the block
        int total_height = 0;
    };

    //*********
	//
	// Hidden classes to handle text
	//
	//*********

    class Text_Image_Word {
        // Class containing a single word of text
    public:
        //*********
        //
        // Text_Image_Word mains functions
        //
        //*********

        // Most simple Text_Image_Word constructor to create an empty word
        Text_Image_Word() : a_datas(Word_Datas(std::string(" "))) {};
        // Simple Text_Image_Word constructor to create a little text
        Text_Image_Word(String text) : a_datas(Word_Datas(text)) {};
        // Simple Text_Image_Word constructor to create a word
        Text_Image_Word(Word_Datas datas) : a_datas(datas) {};
        // Most simple Text_Image_Word destructor
        virtual ~Text_Image_Word() {};

        // Getters and setters
        inline short bottom_offset() const {return a_datas.bottom_offset();};
        inline void set_balise_content(std::string new_balise_content){a_datas.set_balise_content(new_balise_content);};
        inline void set_bottom_offset(short new_bottom_offset) {a_datas.set_bottom_offset(new_bottom_offset); };
        inline void set_top_offset(short new_top_offset) {a_datas.set_top_offset(new_top_offset); };
        inline void set_width(unsigned int new_width) {a_datas.set_width(new_width);};
        inline void set_x_position(int new_x_position) {a_datas.set_x_position(new_x_position);}
        inline Text_Style style() const {return a_datas.style();};
        inline String text() const {return a_datas.content();};
        inline short top_offset() const {return a_datas.top_offset();};
        inline int x_position() const {return a_datas.x_position();};

        //*********
        //
        // Optimisation system
        //
        //*********

        // Generate the image of the word
        void generate_word();
        // Paste a letter of a word in an image with thread system
        void __word_letter(Image* image_to_apply, Image* image_to_paste, unsigned int x, unsigned int y) {image_to_apply->paste(image_to_paste, x, y);delete image_to_paste; image_to_paste = 0;};

        // Getters and setters
        inline Word_Datas datas() const {return a_datas;};
        inline Image* image() { return a_last_image.get(); };
        std::shared_ptr<Image> image_shared_pointer() {return a_last_image;};
        inline void set_balise_parent(std::shared_ptr<XML_Text> new_balise_parent){a_datas.set_balise_parent(new_balise_parent);};
        inline void set_image_shared_ptr(std::shared_ptr<Image> img) {a_last_image=img;};

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

    // Struct necessary to properly generate maths equations
    struct __Math_Part_Image {
        // Balise of the part
        std::string balise = "";
        // Image for the part
        std::shared_ptr<Image> image;

        // Offset of the bottom
        int bottom_offset = 0;
        // Offset of the middle
        int middle_bottom_offset = 0;
        int middle_top_offset = 0;
        // Offset of the top
        int top_offset = 0;
    };

    // Type of possible image generation
	enum Image_Generation_Type {IGT_Full, IGT_Size};

	// Get an utf-8 symbol from a text
	int utf_8_symbol_by_name(std::string name);
	// Get an utf-8 symbol from a XML content
	void utf_8_symbol_xml(std::shared_ptr<scls::XML_Text> text, bool to_html);

    class Text_Image_Line {
        // Class containing a single line text
    public:
        //*********
        //
        // Text_Image_Line mains functions
        //
        //*********

        // Most simple Text_Image_Line constructor
        Text_Image_Line(std::shared_ptr<_Balise_Style_Container> defined_balises, std::shared_ptr<XML_Text> text) : a_defined_balises(defined_balises) {set_text(text);};
        // Text_Image_Line destructor
        virtual ~Text_Image_Line() {free_memory();};

        // Returns the text in plain text
        inline String plain_text() const { return a_defined_balises.get()->plain_text(text()->full_text()); };
        // Returns the size of the line in plain text
        inline unsigned int plain_text_size() const { return plain_text().size(); };

        // Getters and setters
        inline Line_Datas datas() const {return a_datas;};
        inline Text_Style* global_style() const {return a_datas.a_global_style.get();};
        inline std::shared_ptr<Text_Style> global_style_shared_ptr() const {return a_datas.a_global_style;};
        inline bool has_been_modified() const {return a_has_been_modified;};
        inline bool is_modified() const {return a_modified;};
        inline unsigned int line_start_position() const {return a_datas.start_position;};
        inline unsigned int line_start_position_in_plain_text() const {return a_datas.start_position_in_plain_text;};
        inline void set_datas(Line_Datas new_datas) {a_datas = new_datas;};
        inline void set_has_been_modified(bool new_has_been_modified) {a_has_been_modified = new_has_been_modified;};
        inline void set_line_start_position(unsigned int new_line_start_position) {a_datas.start_position = new_line_start_position;};
        inline void set_line_start_position_in_plain_text(unsigned int new_line_start_position_in_plain_text) {a_datas.start_position_in_plain_text = new_line_start_position_in_plain_text;};
        inline void set_max_width(int new_max_width) {global_style()->max_width = new_max_width;if(a_update_at_max_width_modification){generate_words();}};
        inline void set_modified(bool new_modified) {a_modified = new_modified;};
        inline void set_text(std::shared_ptr<XML_Text> new_text, bool move_cursor = true) {a_datas.content = new_text;a_datas.content_in_plain_text = a_defined_balises.get()->plain_text(new_text.get()->full_text());if(move_cursor){set_cursor_position_in_plain_text(a_datas.content_in_plain_text.size());}};
        inline XML_Text* text() const {return a_datas.content.get();};
        inline std::shared_ptr<XML_Text> text_shared_ptr() const {return a_datas.content;};

        //*********
        //
        // Cursor handling
        //
        //*********

        // Returns the position of the cursor in plain text with a X position
        int cursor_position_in_plain_text_at_x(int x_position);
        // Returns a word at a position in pixel
        std::shared_ptr<Text_Image_Word> word_at_position_in_pixel(int x, int y);

        // Getters and setters
        inline int cursor_position_in_plain_text() const {return a_cursor_position_in_plain_text;};
        inline int cursor_x() const {return a_cursor_x;};
        inline void set_cursor_position_in_plain_text(unsigned int new_cursor_position_in_plain_text) {a_cursor_position_in_plain_text = new_cursor_position_in_plain_text;};
        inline void set_use_cursor(bool new_use_cursor) {a_use_cursor = new_use_cursor;};
        inline bool use_cursor() const {return a_use_cursor;};

        //*********
        //
        // Optimisation system
        //
        //*********

        // Clears the memory from the words
        inline void clear_words() { a_words.clear(); };
        // Frees the memory of the line
        inline void free_memory() {clear_words();a_last_image.reset();a_update_at_max_width_modification = false;};
        // Generates the needed maths for a word
        std::shared_ptr<__Math_Part_Image> generate_maths(std::shared_ptr<XML_Text> content, Text_Style current_style);
        inline std::shared_ptr<__Math_Part_Image> generate_maths(std::string content, Text_Style current_style){return generate_maths(xml(a_defined_balises, content), current_style);};
        // Generates the needed words (and balises)
        static void __generate_image(std::shared_ptr<Text_Image_Word>& word, std::shared_ptr<Image> resize_image, unsigned int& current_position_in_plain_text, int& current_width, int height, int width);
        virtual void generate_word(std::shared_ptr<XML_Text> current_text, unsigned int& current_position_in_plain_text, std::shared_ptr<Text_Style> needed_style, std::shared_ptr<Text_Image_Word>& word_to_add);
        void __generate_words_without_balise(std::shared_ptr<XML_Text> text, Text_Style current_style, unsigned int& current_position_in_plain_text);
        void generate_words(std::shared_ptr<XML_Text> cutted, unsigned int& current_position_in_plain_text, std::shared_ptr<Text_Style> needed_style);
        inline void generate_words(){free_memory();unsigned int current_position_in_plain_text = 0; generate_words(text_shared_ptr(), current_position_in_plain_text, global_style_shared_ptr());};
        // Generates a word
        std::shared_ptr<Text_Image_Word> _generate_word(const std::string& word, const Text_Style& style, unsigned int start_position_in_plain_text);
        // Generates and returns an image of the line
        Image* image(Image_Generation_Type generation_type);
        inline Image* image() {return image(Image_Generation_Type::IGT_Full);};
        // Place the words as needed
        void place_words();
        // Returns the shared image
        std::shared_ptr<Image>& shared_image() {return a_last_image;};
        // Returns the total height of the line
        inline unsigned int total_height() {int to_return = 0;for(int i = 0;i<static_cast<int>(a_line_height.size());i++){to_return+=a_line_height[i];}return to_return;};
        // Paste a letter of a word in an image with thread system
        void __word_letter(Image* image_to_apply, Image* image_to_paste, unsigned int x, unsigned int y) {image_to_apply->paste(image_to_paste, x, y);delete image_to_paste; image_to_paste = 0;};

    protected:

        //*********
        //
        // Text_Image_Line mains attributes
        //
        //*********

        // Current width of the line
        int a_current_width = 0;

    private:
        //*********
        //
        // Text_Image_Line mains attributes
        //
        //*********

        // Datas about the line
        Line_Datas a_datas;
        // Containers of each defined balises
        std::shared_ptr<_Balise_Style_Container> a_defined_balises;
        // If the line has been modified or not
        bool a_has_been_modified = true;
        // Number of sublines in this line
        bool a_modified = false;
        // Needed height of the lines
        std::vector<int> a_line_height = std::vector<int>();
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
        // If the line should be update at max-width modification
        bool a_update_at_max_width_modification = false;
        // Last created words in the block
        std::vector<std::shared_ptr<Text_Image_Word>> a_words = std::vector<std::shared_ptr<Text_Image_Word>>();
        // Datas about the last created words in the block
        std::vector<Word_Datas> a_words_datas = std::vector<Word_Datas>();
    };

    //*********
	//
	// Text classes
	//
	//*********

    class Text_Image_Block {
        // Class containing a single block text
    public:
        //*********
        //
        // Text_Image_Block mains functions
        //
        //*********

        // Text_Image_Block constructor with a Block_Datas and an user defined type
        Text_Image_Block(std::shared_ptr<_Balise_Style_Container> defined_balises, std::shared_ptr<Block_Datas> datas, Block_Type type) : a_defined_balises(defined_balises), a_datas(datas), a_type(type) { set_text(a_datas.get()->content); };
        Text_Image_Block(std::shared_ptr<_Balise_Style_Container> defined_balises, std::shared_ptr<Block_Datas> datas) : Text_Image_Block(defined_balises, datas, Block_Type::BT_Always_Free_Memory) {  };
        Text_Image_Block(std::shared_ptr<_Balise_Style_Container> defined_balises, String text, Block_Type type) : Text_Image_Block(defined_balises, std::make_shared<Block_Datas>(xml(defined_balises, text)), type) {};
        Text_Image_Block(std::shared_ptr<_Balise_Style_Container> defined_balises, String text) : Text_Image_Block(defined_balises, text, Block_Type::BT_Always_Free_Memory) { };
        Text_Image_Block(std::shared_ptr<_Balise_Style_Container> defined_balises, String text, std::shared_ptr<Text_Style> style, Block_Type type) : Text_Image_Block(defined_balises, std::make_shared<Block_Datas>(xml(defined_balises, text), style), type) {};
        Text_Image_Block(std::shared_ptr<_Balise_Style_Container> defined_balises, String text, std::shared_ptr<Text_Style> style) : Text_Image_Block(defined_balises, text, style, Block_Type::BT_Always_Free_Memory) { };
        // Text_Image_Block destructor
        virtual ~Text_Image_Block() { free_memory(); };

        // Getters and setters
        inline String full_text() const {return a_datas.get()->content.get()->full_text();};
        inline Text_Style* global_style() {return a_datas.get()->global_style.get();};
        inline std::shared_ptr<Text_Style> global_style_shared_ptr() {return a_datas.get()->global_style;};
        inline void set_text(std::shared_ptr<XML_Text> new_text, bool move_cursor = true) {a_datas.get()->content = new_text;update_line_text();};
        inline void set_text(String new_text, bool move_cursor = true) {set_text(xml(a_defined_balises, new_text.to_std_string()), move_cursor);};
        inline void set_text(std::string new_text, bool move_cursor = true) {set_text(String(to_utf_8_code_point(new_text)), move_cursor);};
        inline String text() const {return a_datas.get()->content.get()->text();};
        inline Block_Type type() const {return a_type;};

        //*********
        //
        // Cursor handling
        //
        //*********

        // Returns the position of the cursor in plain text with a X position and a line
        unsigned int cursor_position_in_plain_text_at_line_and_x(unsigned int line, int x_position);

        // Getters and setters
        inline Text_Image_Line* cursor_line() {return a_cursor_line;};
        inline unsigned int cursor_position_in_full_text() const {return defined_balises()->plain_text_position_to_unformatted_text_position(text().to_code_point(), cursor_position_in_plain_text());};
        inline unsigned int cursor_position_in_plain_text() const {return a_cursor_position_in_plain_text;};
        inline int cursor_x() const {return a_cursor_x;};
        inline int cursor_y() const {return a_cursor_y;};
        inline void set_cursor_position_in_plain_text(unsigned int new_cursor_position_in_plain_text) {a_cursor_position_in_plain_text = new_cursor_position_in_plain_text;};
        inline void set_max_width(int new_max_width) {global_style()->max_width = new_max_width;};
        inline void set_use_cursor(bool new_use_cursor) {a_use_cursor = new_use_cursor;};
        inline bool use_cursor() const {return a_use_cursor;};

        //*********
        //
        // Optimisation system
        //
        //*********

        // Returns the line at a position in pixel
        Text_Image_Line* line_at_position_in_pixel(int x, int y, int& needed_y);

        // Check the modified lines
        inline void _check_modified_lines() {for(int i = 0;i<static_cast<int>(a_lines.size());i++) {if(a_lines[i] != 0) {if(a_lines[i]->is_modified()) {delete a_lines[i]; a_lines[i] = 0;}}}};
        // Clear the memory from the lines
        inline void clear_lines() {for(int i = 0;i<static_cast<int>(a_lines.size());i++) { if(a_lines[i] != 0) delete a_lines[i]; } a_lines.clear(); };
        // Creates and returns a line for the block
        virtual Text_Image_Line* __create_line(Line_Datas& needed_datas){return new Text_Image_Line(a_defined_balises, needed_datas.content);};
        // Delete the useless generated lines
        inline void delete_useless_generated_lines() {for(int i = 0;i<static_cast<int>(a_lines.size()) - static_cast<int>(a_lines_text.size());i++) {delete a_lines[a_lines.size() - 1];a_lines.pop_back();}};
        // Generate all the lines of the block
        Text_Image_Line* _generate_line(Line_Datas datas);
        inline void generate_lines(bool entirely){if(entirely){free_memory();}_regenerate_lines();};;
        inline void generate_lines(){generate_lines(true);};
        // Generates the next line to be generated and returns it
        Text_Image_Line* generate_next_line(unsigned int line_number);
        Text_Image_Line* generate_next_line() {Text_Image_Line* to_return = generate_next_line(a_current_line);a_current_line++;return to_return;};
        // Regenerate the lines with a new text
        inline void _regenerate_lines() {reset_line_generation();for(int i = 0;i<static_cast<int>(a_lines_text.size());i++) {generate_next_line(i);} delete_useless_generated_lines();};
        // Reset the generation of lines
        inline void reset_line_generation() {_check_modified_lines();a_current_line = 0;a_datas.get()->max_width = 0;a_datas.get()->total_height = 0;};

        // Add text to the block
        void add_text(String first_text);
        // Free the memory of the line
        inline void free_memory() {clear_lines();};
        // Generates and returns an image with the block on it
        Image* image(Image_Generation_Type generation_type);
        inline Image* image() {return image(Image_Generation_Type::IGT_Full);};
        // Returns the shared pointer of the image and generates it if needed
        inline std::shared_ptr<Image>& image_shared_pointer(Image_Generation_Type generation_type) {image(generation_type);return a_last_image;};
        inline std::shared_ptr<Image>& image_shared_pointer() { return image_shared_pointer(Image_Generation_Type::IGT_Full);};
        // Returns the line at a plain text position given, or 0
        inline Text_Image_Line* line_at_position_in_plain_text(unsigned int position) {int final_position = line_number_at_position_in_plain_text(position);if(final_position == -1){return 0;}return lines()[final_position];};
        // Returns the line number at the position given, or 0
        inline int line_number_at_position(unsigned int position) {for(int i = 1;i<static_cast<int>(lines_datas().size());i++) {if(lines_datas()[i].start_position > position) {return i - 1;}} return lines_datas().size() - 1;};
        // Returns the line number at a plain text position given, or 0
        int line_number_at_position_in_plain_text(unsigned int position);
        // Pastes an image on an another image for multi threading
        void __image_paste(Image* block, Image* image_2, unsigned int current_x, unsigned int current_y) {block->paste(image_2, current_x, current_y);};
        // Places the lines in the block
        void place_lines();
        // Removes a part of the text and returns the number of lines deleted
        int remove_text(unsigned int size_to_delete);
        // Soft reset the block for a line renegeration
        inline void soft_reset() {a_cursor_line = 0;};
        // Update the text in each lines, without others modification
        void update_line_text();

        // Getters and setter
        inline Block_Datas* datas() {return a_datas.get();};
        inline _Balise_Style_Container* defined_balises() const {return a_defined_balises.get();};
        inline std::shared_ptr<_Balise_Style_Container> defined_balises_shared_ptr() {return a_defined_balises;};
        inline std::vector<Text_Image_Line*>& lines() { return a_lines; };
        inline std::vector<Line_Datas>& lines_datas() { return a_lines_text; };
    private:
        //*********
        //
        // Text_Image_Block mains attributes
        //
        //*********

        // Containers of each defined balises
        std::shared_ptr<_Balise_Style_Container> a_defined_balises;
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
        Text_Image_Multi_Block(std::shared_ptr<_Balise_Style_Container> defined_balises, std::string text) : a_defined_balises(defined_balises) {a_global_style.get()->set_this_style(a_global_style);set_text(text); };
        Text_Image_Multi_Block(std::shared_ptr<_Balise_Style_Container> defined_balises, std::string text, std::shared_ptr<Text_Style> style) : a_defined_balises(defined_balises) {a_global_style=style;a_global_style.get()->set_this_style(a_global_style);set_text(text); };
        // Text_Image destructor
        virtual ~Text_Image_Multi_Block() {__delete_blocks();};

        // Generates and returns a block
        Text_Image_Block* _block(std::shared_ptr<Block_Datas> block) { Text_Image_Block* new_block = new Text_Image_Block(a_defined_balises, block); return new_block; };
        // Creates and returns a Text_Image_Block
        virtual std::shared_ptr<Text_Image_Block>__create_block(std::shared_ptr<Block_Datas>needed_datas){return std::make_shared<Text_Image_Block>(a_defined_balises, needed_datas);};
        // Deletes the blocks in the image
        void __delete_blocks(unsigned int max_size = 0) { a_blocks.clear(); };
        // Deletes the last blocks in a vector
        void free_memory() {a_blocks.clear();a_blocks_datas.clear();};
        // Generates each blocks in the multiblocks (and delete the previous ones)
        void generate_blocks();
        inline void generate_blocks(std::string text_to_analyse) {update_blocks_datas(text_to_analyse);generate_blocks();};
        std::shared_ptr<Text_Image_Block> generate_next_block(int i);
        // Return the entire text in an image
        Image* image(Image_Generation_Type generation_type, const std::string& start_text);
        Image* image() {return image(Image_Generation_Type::IGT_Full, text());};
        // Returns a shared pointer of the image
        std::shared_ptr<Image> image_shared_pointer() { return image_shared_pointer(Image_Generation_Type::IGT_Full);};
        std::shared_ptr<Image> image_shared_pointer(Image_Generation_Type generation_type) { std::shared_ptr<Image> to_return;to_return.reset(image(generation_type, text()));return to_return;};
        // Place the blocks in the multi-block
        void place_blocks();
        // Save the image as an image
        inline void save_image(std::string path) {Image* img = image();img->save_png(path);delete img;img = 0;};
        // Update the datas of each blocks
        void update_blocks_datas(std::shared_ptr<XML_Text> cutted);
        inline void update_blocks_datas(String text_to_analyse){update_blocks_datas(xml(a_defined_balises, text_to_analyse));};
        inline void update_blocks_datas(){return update_blocks_datas(xml(a_defined_balises, text()));};

        // Plain text handling
        // Returns the plain text of the image
        inline std::string plain_text() { return defined_balises()->plain_text(text()); };
        // Returns the size of the plain text in the image
        inline unsigned int plain_text_size() { return plain_text().size(); };

        // Getters and setters
        inline std::vector<std::shared_ptr<Text_Image_Block>>& blocks() {return a_blocks;};
        inline std::vector<std::shared_ptr<Block_Datas>>& blocks_datas() {return a_blocks_datas;};
        inline _Balise_Style_Container* defined_balises() {return a_defined_balises.get();};
        inline std::shared_ptr<_Balise_Style_Container> defined_balises_shared_ptr() {return a_defined_balises;};
        inline Balise_Style_Datas* defined_balises(std::string balise) {return defined_balises()->defined_balise_style(balise);};
        inline Text_Style* global_style() const {return a_global_style.get();};
        inline std::shared_ptr<Text_Style> global_style_shared_ptr() const {return a_global_style;};
        inline unsigned char line_pasting_max_thread_number() const {return a_line_pasting_max_thread_number;};
        inline void set_line_pasting_max_thread_number(unsigned char new_line_pasting_max_thread_number) {a_line_pasting_max_thread_number = new_line_pasting_max_thread_number;};
        inline void set_text(String new_text) {a_blocks.clear();update_blocks_datas(new_text);};
        inline void set_use_cursor(bool use_cursor){a_use_cursor = use_cursor;};
        inline void set_xml_text(std::shared_ptr<XML_Text> new_text) {a_blocks.clear();update_blocks_datas(new_text);};
        String text() const;
    private:
        // Global style in the text
        std::shared_ptr<Text_Style> a_global_style = std::make_shared<Text_Style>();

        // Containers of each defined balises
        std::shared_ptr<_Balise_Style_Container> a_defined_balises;

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
        // If the block use cursor or not
        bool a_use_cursor = false;
    }; //*/

    class Text_Image_Generator {
        // Class simplifying the creation of text image
    public:
        // Most simple Text_Image_Generator constructor
        Text_Image_Generator() {a_balises.get()->_load_built_in_balises();};
        // Text_Image_Generator destructor
        virtual ~Text_Image_Generator() {}

        // Create an image from a text and return it
        inline Image* image(std::string text) {Text_Image_Block *img = new Text_Image_Block(a_balises, text);Image* to_return=img->image();delete img;img = 0;return to_return;};
        // Create an image from a text and return it
        template <typename T = Text_Image_Block>
        inline std::shared_ptr<Image> image_shared_ptr(std::string text, Text_Style style) {std::shared_ptr<T> img = std::make_shared<T>(a_balises, text, std::make_shared<Text_Style>(style));return img.get()->image_shared_pointer();};
        inline std::shared_ptr<Image> image_shared_ptr(Fraction fraction, Text_Style style){return image_shared_ptr(fraction.to_mathml(), style);};
        // Returns a newly created text image
        inline Text_Image_Block* new_text_image_block(std::string text, Block_Type type = Block_Type::BT_Always_Free_Memory) {Text_Image_Block *img = new Text_Image_Block(a_balises, text, type);return img;};
        // Returns a newly created text image multi block
        template <typename T = Text_Image_Multi_Block> T* new_text_image_multi_block(std::string text) {T *img = new T(a_balises, text);return img;};

        // Methods to directly use balises
        inline std::string plain_text(std::string text)const{return a_balises.get()->plain_text(text);};
        inline int plain_text_position_to_unformatted_text_position(std::string text, int cursor_position)const{return a_balises.get()->plain_text_position_to_unformatted_text_position(text, cursor_position);}

        // Getters and setters
        inline _Balise_Style_Container* balises() const {return a_balises.get();};
        inline std::shared_ptr<_Balise_Style_Container> balises_shared_ptr() {return a_balises;};
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
        // Balises in the generator
        std::shared_ptr<_Balise_Style_Container> a_balises = std::make_shared<_Balise_Style_Container>();

        // Global color in the text
        Color a_global_color = Color(0, 0, 0);
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
        Color current_background_color = Color(255, 255, 255);
        Color current_color = Color(0, 0, 0);
        Font current_font;
        unsigned short current_font_size = 20;
    };

    // Converts some well-known types into image
    inline std::shared_ptr<Image> to_image(std::string* value, scls::Text_Style style){Text_Image_Generator gen;return gen.image_shared_ptr(*value, style);};
    inline std::shared_ptr<Image> to_image(std::string* value){Text_Style style;return to_image(value, style);};
}

#endif // SCLS_IMAGE_TEXT
