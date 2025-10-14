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
	// Style handling
	//
	//*********

	// Enumeration of each horizontal alignment possible
	enum Alignment_Horizontal {H_Left, H_Center, H_Right, H_User_Defined};

	// Enumeration of each vertical alignment possible
	enum Alignment_Vertical {V_Top, V_Center, V_Bottom, V_User_Defined};

	// Style about a text
	class __Text_Style_Base {
    public:

        // __Text_Style_Base constructor
        static std::shared_ptr<__Text_Style_Base> new_text_style(){std::shared_ptr<__Text_Style_Base> to_return = std::shared_ptr<__Text_Style_Base>(new __Text_Style_Base());to_return.get()->set_this_style(to_return);return to_return;};

        // Returns the block of this style
        inline __Text_Style_Base* block_style() const {return a_block_style.lock().get();};
        inline std::shared_ptr<__Text_Style_Base> block_style_shared_ptr() const {return a_block_style.lock();};
        // Merges a style with this style
        void merge_style(__Text_Style_Base* to_merge);
        // Returns the parent of this style
        inline __Text_Style_Base* parent_style() const {return a_parent_style.lock().get();};
        inline std::shared_ptr<__Text_Style_Base> parent_style_shared_ptr() const {return a_parent_style.lock();};
        // Create a children for this style
        inline std::shared_ptr<__Text_Style_Base> new_child(){std::shared_ptr<__Text_Style_Base> child = std::shared_ptr<__Text_Style_Base>(new __Text_Style_Base());child.get()->a_this_style=child;child.get()->a_parent_style=a_this_style;return child;};
        // Sets the shared ptr to the block style
        inline void set_block_style(std::shared_ptr<__Text_Style_Base> block_style){a_block_style=block_style;};
        // Sets the shared ptr to the parent style
        inline void set_parent_style(std::weak_ptr<__Text_Style_Base> parent_style){a_parent_style=parent_style;};
        // Sets the shared ptr to this style
        inline void set_this_style(std::weak_ptr<__Text_Style_Base> this_style){a_this_style=this_style;};

        // Getters and setters
        // Alignment horizontal
        inline Alignment_Horizontal alignment_horizontal() const {if(a_alignment_horizontal_modified || (parent_style() == 0 && block_style() == 0)){return a_alignment_horizontal;}else if(block_style() != 0){if(block_style()->alignment_horizontal_modified()){return block_style()->alignment_horizontal();}else if(parent_style()==0){return a_alignment_horizontal;}}return parent_style()->alignment_horizontal();};
        inline bool alignment_horizontal_modified() const {return a_alignment_horizontal_modified;};
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
        // Max width
        inline unsigned short max_width() const {if(a_max_width_modified || (parent_style() == 0 && block_style() == 0)){return a_max_width;}else if(block_style()!=0){if(block_style()->max_width_modified()){return block_style()->max_width();}else if(parent_style()==0){return a_max_width;}}return parent_style()->max_width();};
        inline bool max_width_modified() const {return a_max_width_modified;};
        inline void set_max_width(unsigned short new_max_width){a_max_width=new_max_width;a_max_width_modified=true;};
        inline void unset_max_width(){a_max_width_modified=false;};
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
        // Height / width / x / y pos of the text offset
        double text_offset_height = 0;
        double text_offset_width = 0;
        double text_offset_x = 0;
        double text_offset_y = 0;
        // Color of the border
        Color border_color = scls::Color(0, 0, 0);

    private:

        // __Text_Style_Base constructor
        __Text_Style_Base(){};

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
        // Max width of the text
        int a_max_width = 0;
        bool a_max_width_modified = false;
        // Padding of the style
        int a_padding_bottom = 0;int a_padding_left = 0;int a_padding_right = 0;int a_padding_top = 0;

        // Block-style-arent of this style
        std::weak_ptr<__Text_Style_Base> a_block_style;
        // Parent of this style
        std::weak_ptr<__Text_Style_Base> a_parent_style;
        // Shared ptr to this style
        std::weak_ptr<__Text_Style_Base> a_this_style;
	};
    class Text_Style {
    public:

        // Text_Style constructor
        Text_Style(std::shared_ptr<__Text_Style_Base> base):a_datas(base){a_datas.get()->set_this_style(a_datas);};
        Text_Style(){};

        // Returns the block / parent of this style
        inline Text_Style block_style() const {return a_datas.get()->block_style_shared_ptr();};
        inline Text_Style parent_style() const {return a_datas.get()->parent_style_shared_ptr();};
        // Merges a style with this style
        void merge_style(Text_Style to_merge);
        // Create a children for this style
        inline Text_Style new_child(){Text_Style child = Text_Style();child.a_datas.get()->set_parent_style(a_datas);return child;};
        // Sets the shared ptr to the block / parent style
        inline void set_block_style(Text_Style block_style){a_datas.get()->set_block_style(block_style.a_datas);};
        inline void set_parent_style(Text_Style parent_style){a_datas.get()->set_parent_style(parent_style.a_datas);};

        // Getters and setters
        // Alignment horizontal
        inline Alignment_Horizontal alignment_horizontal() const {return a_datas.get()->alignment_horizontal();};
        inline void set_alignment_horizontal(Alignment_Horizontal new_alignment_horizontal){a_datas.get()->set_alignment_horizontal(new_alignment_horizontal);};
        inline void unset_alignment_horizontal(){a_datas.get()->unset_alignment_horizontal();};
        // Background color
        inline Color background_color() const {return a_datas.get()->background_color();};
        inline void set_background_color(Color new_background_color){a_datas.get()->set_background_color(new_background_color);};
        inline void unset_background_color(){a_datas.get()->unset_background_color();};
        // Border
        inline int border_bottom_width() const {return a_datas.get()->border_bottom_width();};
        inline bool border_bottom_width_modified() const {return a_datas.get()->border_bottom_width_modified();};
        inline int border_left_width() const {return a_datas.get()->border_left_width();};
        inline bool border_left_width_modified() const {return a_datas.get()->border_left_width_modified();};
        inline int border_right_width() const {return a_datas.get()->border_right_width();};
        inline bool border_right_width_modified() const {return a_datas.get()->border_right_width_modified();};
        inline int border_top_width() const {return a_datas.get()->border_top_width();};
        inline bool border_top_width_modified() const {return a_datas.get()->border_top_width_modified();};
        inline void set_border_bottom_width(int new_border_bottom_width) {a_datas.get()->set_border_bottom_width(new_border_bottom_width);};
        inline void set_border_left_width(int new_border_left_width) {a_datas.get()->set_border_left_width(new_border_left_width);};
        inline void set_border_right_width(int new_border_right_width) {a_datas.get()->set_border_right_width(new_border_right_width);};
        inline void set_border_top_width(int new_border_top_width) {a_datas.get()->set_border_top_width(new_border_top_width);};
        inline void set_border_width(int new_border_width){a_datas.get()->set_border_width(new_border_width);};
        // Color
        inline Color color() const {return a_datas.get()->color();};
        inline bool color_modified() const {return a_datas.get()->color_modified();};
        inline void set_color(Color new_color){a_datas.get()->set_color(new_color);};
        inline void unset_color(){a_datas.get()->unset_color();};
        // Font
        inline Font font()const{return a_datas.get()->font();};
        inline bool font_modified() const {return a_datas.get()->font_modified();};
        inline std::string font_path()const{return a_datas.get()->font_path();};
        inline void set_font(Font new_font){a_datas.get()->set_font(new_font);}
        inline void set_font_path(std::string new_font_path){a_datas.get()->set_font_path(new_font_path);};
        inline void unset_font(){a_datas.get()->unset_font();};
        // Font size
        inline unsigned short font_size() const {return a_datas.get()->font_size();};
        inline bool font_size_modified() const {return a_datas.get()->font_size_modified();};
        inline void set_font_size(unsigned short new_font_size){a_datas.get()->set_font_size(new_font_size);};
        inline void unset_font_size(){a_datas.get()->unset_font_size();};
        // Margin
        inline int margin_bottom() const {return a_datas.get()->margin_bottom();};
        inline bool margin_bottom_modified() const {return a_datas.get()->margin_bottom_modified();};
        inline int margin_left() const {return a_datas.get()->margin_left();};
        inline bool margin_left_modified() const {return a_datas.get()->margin_left_modified();};
        inline int margin_right() const {return a_datas.get()->margin_right();};
        inline bool margin_right_modified() const {return a_datas.get()->margin_right_modified();};
        inline int margin_top() const {return a_datas.get()->margin_top();};
        inline bool margin_top_modified() const {return a_datas.get()->margin_top_modified();};
        inline void set_margin_bottom(int new_margin_bottom) {a_datas.get()->set_margin_bottom(new_margin_bottom);};
        inline void set_margin_left(int new_margin_left) {a_datas.get()->set_margin_left(new_margin_left);};
        inline void set_margin_right(int new_margin_right) {a_datas.get()->set_margin_right(new_margin_right);};
        inline void set_margin_top(int new_margin_top) {a_datas.get()->set_margin_top(new_margin_top);};
        // Max width
        int max_width() const {return a_datas.get()->max_width();};
        void set_max_width(int new_max_width){a_datas.get()->set_max_width(new_max_width);};
        // Padding
        inline int padding_bottom() const {return a_datas.get()->padding_bottom();};
        inline int padding_left() const {return a_datas.get()->padding_left();};
        inline int padding_right() const {return a_datas.get()->padding_right();};
        inline int padding_top() const {return a_datas.get()->padding_top();};
        inline void set_padding_bottom(int new_padding_bottom) {a_datas.get()->set_padding_bottom(new_padding_bottom);};
        inline void set_padding_left(int new_padding_left) {a_datas.get()->set_padding_left(new_padding_left);};
        inline void set_padding_right(int new_padding_right) {a_datas.get()->set_padding_right(new_padding_right);};
        inline void set_padding_top(int new_padding_top) {a_datas.get()->set_padding_top(new_padding_top);};

        // TEMP
        Color border_color() const {return a_datas.get()->border_color;}
        void set_border_color(Color new_border_color){a_datas.get()->border_color = new_border_color;};
        void set_text_offset_height(int new_text_offset_height){a_datas.get()->text_offset_height = new_text_offset_height;};
        void set_text_offset_width(int new_text_offset_width){a_datas.get()->text_offset_width = new_text_offset_width;};
        void set_text_offset_x(int new_text_offset_x){a_datas.get()->text_offset_x = new_text_offset_x;};
        void set_text_offset_y(int new_text_offset_y){a_datas.get()->text_offset_y = new_text_offset_y;};
        int text_offset_height() const {return a_datas.get()->text_offset_height;};
        int text_offset_width() const {return a_datas.get()->text_offset_width;};
        int text_offset_x() const {return a_datas.get()->text_offset_x;};
        int text_offset_y() const {return a_datas.get()->text_offset_y;};
    private:

        // Datas for this style
        std::shared_ptr<__Text_Style_Base> a_datas = __Text_Style_Base::new_text_style();
    };

    // Returns if an XML attribute is for a text style
    bool text_style_from_xml_attribute(XML_Attribute* attribute, Text_Style style);
    // Handle a text style from XML
    void text_style_from_xml(std::shared_ptr<__XML_Text_Base> content, Text_Style style);

    //*********
	//
	// Classes used to handle datas
	//
	//*********

	// Balise in a text
    struct Balise_Style_Datas : public Balise_Datas {Balise_Style_Datas(){};Text_Style style;};

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
        inline std::shared_ptr<__Image_Base>* add_image(std::string name, std::shared_ptr<__Image_Base> img) {std::shared_ptr<__Image_Base>* check_img=image(name);if(check_img!=0){return check_img;}a_images[name]=img;return &a_images[name];};
        inline std::shared_ptr<__Image_Base>* add_image(std::string name, std::string path) {std::shared_ptr<__Image_Base>* check_img=image(name);if(check_img!=0){return check_img;}return add_image(name, std::make_shared<__Image_Base>(path));};
        // Returns if the container contains an image
        inline bool contains_image(std::string src) {for(std::map<std::string, std::shared_ptr<__Image_Base>>::iterator it = a_images.begin();it!=a_images.end();it++){if(it->first==src)return true;}return false;};
        inline std::shared_ptr<__Image_Base>* image(std::string src) {for(std::map<std::string, std::shared_ptr<__Image_Base>>::iterator it = a_images.begin();it!=a_images.end();it++){if(it->first==src)return &it->second;}return 0;};
        // Creates a new image in the container
        inline std::shared_ptr<__Image_Base>* new_image(std::string name, std::string img_path) {if(contains_image(name))return 0;std::shared_ptr<__Image_Base> img=std::make_shared<__Image_Base>(img_path);return add_image(name,img);};

    private:

        //*********
        //
        // _Balise_Style_Container images
        //
        //*********

        // Every loaded images, with their source name as key
        std::map<std::string, std::shared_ptr<__Image_Base>> a_images;
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

        // Special characters
        bool is_space() const {return a_content == std::string(" ");};
        bool is_special() const {return is_space();};

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
        inline std::shared_ptr<__XML_Text_Base> balise_parent()const{return a_balise_parent;};
        inline short bottom_offset() const {return a_bottom_offset;};
        inline std::vector<int>& characters_position() {return a_characters_position;};
        inline std::vector<unsigned int>& characters_width() {return a_characters_width;};
        inline int line() const {return a_line;};
        inline void set_balise_content(std::string new_balise_content){a_balise_content=new_balise_content;};
        inline void set_balise_parent(std::shared_ptr<__XML_Text_Base> new_balise_parent){a_balise_parent=new_balise_parent;};
        inline void set_bottom_offset(short new_bottom_offset) {a_bottom_offset = new_bottom_offset;};
        inline void set_characters_position(std::vector<int> new_characters_position) {a_characters_position = new_characters_position;};
        inline void set_characters_width(std::vector<unsigned int> new_characters_width) {a_characters_width = new_characters_width;};
        inline void set_line(int new_line){a_line = new_line;};
        inline void set_top_offset(short new_top_offset) {a_top_offset = new_top_offset;};
        inline void set_width(unsigned int new_width) {a_width = new_width;};
        inline void set_x_position(int new_x_position) {a_x_position = new_x_position;};
        inline void set_y_position(int new_y_position) {a_y_position = new_y_position;};
        inline short top_offset() const {return a_top_offset;};
        inline unsigned int width() const {return a_width;};
        inline int x_position() const {return a_x_position;};
        inline int y_position() const {return a_y_position;};
    private:
        // Content of the balise
        String a_balise_content;
        // Parent XML balise of this word
        std::shared_ptr<__XML_Text_Base> a_balise_parent;
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
        // Line of the word
        int a_line = 0;
        // Width of the image
        unsigned int a_width = 0;
        // X / Y position of the word
        int a_x_position = 0;
        int a_y_position = 0;

        // Offsets
        // Offset of the bottom position
        short a_bottom_offset = 0;
        // Offset of the top position
        short a_top_offset = 0;
    };

    // Type of Text_Image_Block
    enum Block_Type {BT_Always_Free_Memory,BT_Keep_Block_And_Line_In_Memory};

    struct Block_Datas {
        // Struct containing the datas necessary for a block
        // Block_Datas constructor
        Block_Datas(std::shared_ptr<__XML_Text_Base> block_content, Text_Style new_global_style) : content(block_content),global_style(new_global_style) {}
        Block_Datas(std::shared_ptr<__XML_Text_Base> block_content) : content(block_content) {}

        // Adds text in the block
        void add_text(std::string text);

        // Getters and setters
        inline scls::Alignment_Horizontal alignment_horizontal() const {return global_style.alignment_horizontal();};
        inline Balise_Datas* balise_datas() const {return content.get()->balise_datas();};
        inline void set_x_position(int new_x_position) {a_x_position = new_x_position;};
        inline void set_y_position(int new_y_position) {a_y_position = new_y_position;};
        inline int x_position() const {return a_x_position;};
        inline int y_position() const {return a_y_position;};

        // Content of the balise
        std::shared_ptr<__XML_Text_Base> content;

        // Global style in the block
        Text_Style global_style;

        // Max last line bottom offset
        int max_last_line_bottom_offset = 0;
        // Max width of the block
        int max_width = 0;
        // Total height of the block
        int total_height = 0;

        // X / Y position of the word
        int a_x_position = 0;
        int a_y_position = 0;
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

        // Text_Image_Word constructors
        Text_Image_Word() : a_datas(std::make_shared<Word_Datas>(std::string(" "))) {};
        Text_Image_Word(String text) : a_datas(std::make_shared<Word_Datas>(text)) {};
        Text_Image_Word(std::shared_ptr<Word_Datas> datas) : a_datas(datas) {};
        // Most simple Text_Image_Word destructor
        virtual ~Text_Image_Word() {};

        // Getters and setters
        inline short bottom_offset() const {return a_datas.get()->bottom_offset();};
        inline String content() const {return a_datas.get()->content();};
        inline void set_balise_content(std::string new_balise_content){a_datas.get()->set_balise_content(new_balise_content);};
        inline void set_bottom_offset(short new_bottom_offset) {a_datas.get()->set_bottom_offset(new_bottom_offset); };
        inline void set_line(int new_line){a_datas.get()->set_line(new_line);};
        inline void set_top_offset(short new_top_offset) {a_datas.get()->set_top_offset(new_top_offset); };
        inline void set_width(unsigned int new_width) {a_datas.get()->set_width(new_width);};
        inline void set_x_position(int new_x_position) {a_datas.get()->set_x_position(new_x_position);}
        inline Text_Style style() const {return a_datas.get()->style();};
        inline String text() const {return a_datas.get()->content();};
        inline short top_offset() const {return a_datas.get()->top_offset();};
        inline int x_position() const {return a_datas.get()->x_position();};
        inline int y_position() const {return a_datas.get()->y_position();};

        //*********
        //
        // Optimisation system
        //
        //*********

        // Generate the image of the word
        void generate_word();
        // Paste a letter of a word in an image with thread system
        void __word_letter(__Image_Base* image_to_apply, __Image_Base* image_to_paste, unsigned int x, unsigned int y) {image_to_apply->paste(image_to_paste, x, y);delete image_to_paste; image_to_paste = 0;};

        // Getters and setters
        inline Word_Datas* datas() const {return a_datas.get();};
        inline __Image_Base* image() { return a_last_image.get(); };
        std::shared_ptr<__Image_Base> image_shared_pointer() {return a_last_image;};
        inline void set_balise_parent(std::shared_ptr<__XML_Text_Base> new_balise_parent){a_datas.get()->set_balise_parent(new_balise_parent);};
        inline void set_image_shared_ptr(std::shared_ptr<__Image_Base> img) {a_last_image=img;};

    private:
        //*********
        //
        // Text_Image_Word mains attributes
        //
        //*********

        // Datas in the word
        std::shared_ptr<Word_Datas> a_datas;

        //*********
        //
        // Optimisation system
        //
        //*********

        // Last generated image
        std::shared_ptr<__Image_Base> a_last_image;
    };

    // Struct necessary to properly generate maths equations
    struct __Math_Part_Image {
        // Balise of the part
        std::string balise = "";
        // __Image_Base for the part
        std::shared_ptr<__Image_Base> image;

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
	void utf_8_symbol_xml(std::shared_ptr<__XML_Text_Base> text, bool to_html);

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
        Text_Image_Block(std::shared_ptr<_Balise_Style_Container> defined_balises, String text, Text_Style style, Block_Type type) : Text_Image_Block(defined_balises, std::make_shared<Block_Datas>(xml(defined_balises, text), style), type) {};
        Text_Image_Block(std::shared_ptr<_Balise_Style_Container> defined_balises, String text, Text_Style style) : Text_Image_Block(defined_balises, text, style, Block_Type::BT_Always_Free_Memory) { };
        Text_Image_Block(std::shared_ptr<_Balise_Style_Container> defined_balises, std::shared_ptr<__XML_Text_Base> text, Text_Style style, Block_Type type) : Text_Image_Block(defined_balises, std::make_shared<Block_Datas>(text, style), type) {};
        Text_Image_Block(std::shared_ptr<_Balise_Style_Container> defined_balises, std::shared_ptr<__XML_Text_Base> text, Text_Style style) : Text_Image_Block(defined_balises, text, style, Block_Type::BT_Always_Free_Memory) { };
        // Text_Image_Block destructor
        virtual ~Text_Image_Block() { free_memory(); };

        // Getters and setters
        inline Balise_Datas* balise_datas() const {return a_datas.get()->balise_datas();};
        inline __XML_Text_Base* content() const {return a_datas.get()->content.get();};
        inline std::shared_ptr<__XML_Text_Base> content_shared_ptr() const {return a_datas.get()->content;};
        inline String full_text() const {return a_datas.get()->content.get()->full_text();};
        inline Text_Style global_style() {return a_datas.get()->global_style;};
        inline void set_text(std::shared_ptr<__XML_Text_Base> new_text, bool move_cursor = true) {a_datas.get()->content = new_text;update_datas();};
        inline void set_text(String new_text, bool move_cursor = true) {set_text(xml(a_defined_balises, new_text.to_std_string()), move_cursor);};
        inline void set_text(std::string new_text, bool move_cursor = true) {set_text(String(to_utf_8_code_point(new_text)), move_cursor);};
        inline String text() const {return a_datas.get()->content.get()->text();};
        inline Block_Type type() const {return a_type;};

        //*********
        //
        // Cursor handling
        //
        //*********

        // Getters and setters
        inline unsigned int cursor_position_in_full_text() const {return defined_balises()->plain_text_position_to_unformatted_text_position(text().to_code_point(), cursor_position_in_plain_text());};
        inline unsigned int cursor_position_in_plain_text() const {return a_cursor_position_in_plain_text;};
        inline int cursor_x() const {return a_cursor_x;};
        inline int cursor_y() const {return a_cursor_y;};
        inline void set_cursor_position_in_plain_text(unsigned int new_cursor_position_in_plain_text) {a_cursor_position_in_plain_text = new_cursor_position_in_plain_text;};
        inline void set_max_width(int new_max_width) {global_style().set_max_width(new_max_width);};
        inline void set_use_cursor(bool new_use_cursor) {a_use_cursor = new_use_cursor;};
        inline bool use_cursor() const {return a_use_cursor;};

        //*********
        //
        // Optimisation system
        //
        //*********

        // Creates and returns a block / word for the block
        virtual std::shared_ptr<Text_Image_Block> __create_block(std::shared_ptr<Block_Datas> needed_datas){return std::make_shared<Text_Image_Block>(a_defined_balises, needed_datas);};
        virtual std::shared_ptr<Text_Image_Word> __create_word(std::shared_ptr<Word_Datas> needed_datas){return std::make_shared<Text_Image_Word>(needed_datas);};

        // Generates a piece of math in the block
        std::shared_ptr<__Math_Part_Image> generate_maths(std::shared_ptr<__XML_Text_Base> content, Text_Style current_style);

        // Generate a block / word of the block
        std::shared_ptr<Text_Image_Block> __generate_block(std::shared_ptr<Block_Datas> datas, Image block_image);
        virtual std::shared_ptr<Text_Image_Block> __generate_block(std::shared_ptr<Block_Datas> datas);
        std::shared_ptr<Text_Image_Block> generate_next_block(int word_number);
        std::shared_ptr<Text_Image_Block> generate_next_block();
        std::shared_ptr<Text_Image_Word> __generate_word(std::string datas, Text_Style current_style);
        std::shared_ptr<Text_Image_Word> __generate_word(std::shared_ptr<Word_Datas> datas);
        std::shared_ptr<Text_Image_Word> generate_next_word(int word_number);
        std::shared_ptr<Text_Image_Word> generate_next_word();

        // Frees the memory of the line
        void free_memory();
        // Generates all the blocks / lines
        void generate_blocks(bool entirely);
        void generate_blocks();
        void __regenerate_blocks();
        void generate_words(bool entirely);
        void generate_words();
        void __regenerate_words();
        // Resets the generation of lines
        void reset_generation();

        // Height of the block
        int total_height() const;







        // Add text to the block
        void add_text(String first_text);
        // Generates and returns an image with the block on it
        __Image_Base* image(Image_Generation_Type generation_type);
        inline __Image_Base* image() {return image(Image_Generation_Type::IGT_Full);};
        // Returns the shared pointer of the image and generates it if needed
        inline std::shared_ptr<__Image_Base>& image_shared_pointer(Image_Generation_Type generation_type) {image(generation_type);return a_last_image;};
        inline std::shared_ptr<__Image_Base>& image_shared_pointer() { return image_shared_pointer(Image_Generation_Type::IGT_Full);};
        // Returns the line number at a plain text position given, or 0
        int line_number_at_position_in_plain_text(unsigned int position);
        // Pastes an image on an another image for multi threading
        void __image_paste(__Image_Base* block, __Image_Base* image_2, unsigned int current_x, unsigned int current_y) {block->paste(image_2, current_x, current_y);};
        // Places the datas in the block
        void place_datas();
        // Removes a part of the text and returns the number of lines deleted
        int remove_text(unsigned int size_to_delete);
        // Update the text in each lines, without others modification
        void update_datas();

        // Getters and setter
        inline std::vector<std::shared_ptr<Text_Image_Block>>& blocks() {return a_blocks;};
        inline std::vector<std::shared_ptr<Block_Datas>>& blocks_datas() {return a_blocks_datas;};
        inline Block_Datas* datas() {return a_datas.get();};
        inline _Balise_Style_Container* defined_balises() const {return a_defined_balises.get();};
        inline std::shared_ptr<_Balise_Style_Container> defined_balises_shared_ptr() {return a_defined_balises;};
        inline int lines_number() const {return a_lines_size.size();};
        inline std::vector<scls::Point_2D>& lines_size() {return a_lines_size;};
        inline int paste_x()const{return a_paste_x;};
        inline void set_start_x(int new_start_x){a_start_x = new_start_x;a_fixed_image.reset();};
        inline int start_x()const{return a_start_x;};
        inline std::vector<std::shared_ptr<Text_Image_Word>>& words() { return a_words; };
        inline std::vector<std::shared_ptr<Word_Datas>>& words_datas() { return a_words_datas; };
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
        std::shared_ptr<__Image_Base> a_last_image;
        // Type of the block
        const Block_Type a_type = Block_Type::BT_Always_Free_Memory;

        //*********
        //
        // Cursor handling
        //
        //*********

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

        // Size of each lines
        std::vector<scls::Point_2D> a_lines_size = std::vector<scls::Point_2D>();
        // Words in each lines
        std::vector<std::vector<std::shared_ptr<Text_Image_Word>>> a_lines_words = std::vector<std::vector<std::shared_ptr<Text_Image_Word>>>();

        // Fixed image
        std::shared_ptr<__Image_Base> a_fixed_image;

        // Mathematical part
        std::shared_ptr<__Math_Part_Image> a_math_datas;
        // Last created sub-blocks / words in the block
        std::vector<std::shared_ptr<Text_Image_Block>> a_blocks = std::vector<std::shared_ptr<Text_Image_Block>>();
        std::vector<std::shared_ptr<Text_Image_Word>> a_words = std::vector<std::shared_ptr<Text_Image_Word>>();
        // Last created sub-blocks / words datas in the block
        std::vector<std::shared_ptr<Block_Datas>> a_blocks_datas = std::vector<std::shared_ptr<Block_Datas>>();
        std::vector<std::shared_ptr<Word_Datas>> a_words_datas = std::vector<std::shared_ptr<Word_Datas>>();
        // Current block / word to be generated
        unsigned int a_current_object = 0;
        // X to paste in the parent block
        int a_paste_x = 0;
        // X to start the placement
        int a_start_x = 0;
    };

    class Text_Image_Generator {
        // Class simplifying the creation of text image
    public:
        // Most simple Text_Image_Generator constructor
        Text_Image_Generator() {a_balises.get()->_load_built_in_balises();};
        // Text_Image_Generator destructor
        virtual ~Text_Image_Generator() {}

        // Create an image from a text and return it
        inline Image image(std::string text) {Text_Image_Block *img = new Text_Image_Block(a_balises, text);Image to_return=img->image_shared_pointer();delete img;img = 0;return to_return;};
        inline Image image(std::string text, Text_Style style) {return image_shared_ptr(text, style);};
        // Create an image from a text and return it
        template <typename T = Text_Image_Block> std::shared_ptr<__Image_Base> image_shared_ptr(std::shared_ptr<__XML_Text_Base> text, Text_Style style) {std::shared_ptr<T> img = std::make_shared<T>(a_balises, text, style);return img.get()->image_shared_pointer();};
        template <typename T = Text_Image_Block> std::shared_ptr<__Image_Base> image_shared_ptr(std::string text, Text_Style style) {std::shared_ptr<T> img = std::make_shared<T>(a_balises, text, style);return img.get()->image_shared_pointer();};
        inline std::shared_ptr<__Image_Base> image_shared_ptr(Fraction fraction, Text_Style style){return image_shared_ptr(fraction.to_mathml(), style);};
        // Returns a newly created text image
        template <typename T = Text_Image_Block> std::shared_ptr<T> new_text_image_block_shared_ptr(std::shared_ptr<__XML_Text_Base> text, Text_Style style) {return std::shared_ptr<T>(new_text_image_block(text, style));};
        template <typename T = Text_Image_Block> T* new_text_image_block(std::shared_ptr<__XML_Text_Base> text, Text_Style style) {T *img = new T(a_balises, text, style);return img;};
        template <typename T = Text_Image_Block> T* new_text_image_block(std::string text, Text_Style style) {T *img = new T(a_balises, text, style);return img;};
        template <typename T = Text_Image_Block> T* new_text_image_block(std::string text) {T *img = new T(a_balises, text, Text_Style());return img;};

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
    std::shared_ptr<__Image_Base> to_image(std::string* value, scls::Text_Style style);
    std::shared_ptr<__Image_Base> to_image(std::string value, scls::Text_Style style);
    std::shared_ptr<__Image_Base> to_image(std::string* value);
    std::shared_ptr<__Image_Base> to_image(std::string value);
}

#endif // SCLS_IMAGE_TEXT
