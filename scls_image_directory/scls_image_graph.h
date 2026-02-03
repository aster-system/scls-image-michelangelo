//******************
//
// scls_image_graph.h
//
//******************
// Presentation :
//
// PLEOS, by Aster System, is a project which aims education.
// By using the power of SCLS and other Aster System's tools, we created  this.
// We want to make education easier for everyone (teachers, as students and pupils).
// The software is made in french, because the main goal is France educational system.
// For more information, see : https://aster-system.github.io/aster-system/projects/pleos.html.
//
// This file contains the necessary tools to use graph with SCLS.
//
//******************
//
// License (GPL V3.0) :
//
// Copyright (C) 2024 by Aster System, Inc. <https://aster-system.github.io/aster-system/>
// This file is part of PLEOS.
// PLEOS is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
// PLEOS is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with PLEOS. If not, see <https://www.gnu.org/licenses/>.
//

#ifndef SCLS_IMAGE_GRAPH
#define SCLS_IMAGE_GRAPH

#include "../scls_image.h"

// The namespace "scls" is used to simplify the all.
namespace scls {

    // Loads the graph XML balises
    void load_balises_graph(__Balise_Container* defined_balises);

	class Graph_Base {
        // Class representating a graph
    public:
        // Node in a __Graph_Base
        class Node_Base {
        public:
            // Links for a node in a graph
            enum Link_Type{LT_Middle, LT_Angle, LT_Bottom, LT_Left, LT_Right, LT_Top, LT_X, LT_Y};
            struct Link{std::shared_ptr<scls::__Image_Base> ponderation;Link_Type type = Link_Type::LT_Middle;std::weak_ptr<Node_Base> target;};

            // __Node_Base constructor
            Node_Base(std::shared_ptr<scls::__Image_Base> new_image, double x, double y, int id);
            Node_Base();
            virtual ~Node_Base(){};

            // Links this node with another node
            Link* link(std::shared_ptr<Node_Base> node);
            // Returns a link by the linked object
            Link* link_by_object(Node_Base* node);
            Link* link_by_object(std::shared_ptr<Node_Base> node);

            // Returns a node to an std::string
            virtual std::string to_xml_text(std::string node_object_name, std::string node_content);
            std::string to_xml_text_x();
            std::string to_xml_text_y();

            // Getters and setters
            inline int id() const {return a_id;};
            inline scls::__Image_Base* image() const {return a_image.get();};
            inline std::shared_ptr<scls::__Image_Base> image_shared_ptr() const {return a_image;};
            inline std::vector<Link>& links() {return a_links;};
            inline scls::Text_Style& style() {return a_style;};
            inline void set_image(std::shared_ptr<scls::__Image_Base> new_value){a_image = new_value;};
            inline void set_image(const std::string value){style().set_alignment_horizontal(scls::H_Center);std::shared_ptr<scls::__Image_Base> to_return = scls::string_to_image(value, style());set_image(to_return);a_value = value;};
            inline void set_style(scls::Text_Style new_style){a_style = new_style;};
            inline void set_value(const std::string new_value){a_value = new_value;};
            inline void set_x(const double new_x){a_x = new_x;};
            inline void set_y(const double new_y){a_y = new_y;};
            inline std::string value() const {return a_value;}
            inline double x() const {return a_x;};
            inline double y() const {return a_y;};
        private:
            // ID of the node
            int a_id = 0;
            // Value in the node
            std::string a_value = std::string();
            std::shared_ptr<scls::__Image_Base> a_image;
            // Links to others nodes
            std::vector<Link> a_links;
            // Style of the node
            scls::Text_Style a_style;
            // X value of the node
            double a_x = 0;
            // Y value of the node
            double a_y = 0;
        }; typedef Node_Base::Link Link;typedef Node_Base::Link_Type Link_Type;

        // __Graph_Base constructor
        Graph_Base(){};
        virtual ~Graph_Base(){};

        // Adds a node in the graph
        virtual std::shared_ptr<Node_Base> __create_node(std::shared_ptr<scls::__Image_Base> image, double x, double y){std::shared_ptr<Graph_Base::Node_Base>to_return=std::make_shared<Node_Base>(image, x, y, a_nodes.size());return to_return;};
        int add_node(std::shared_ptr<scls::__Image_Base> image, double x, double y);
        int add_node(std::string value, double x, double y);
        int add_node(std::shared_ptr<scls::__Image_Base> image);
        int add_node();
        // Links two nodes in the graph
        Link* link_nodes(int id_1, int id_2);
        // Sets the ponderation of a link node
        void set_link_ponderation(int id_1, int id_2, std::shared_ptr<scls::__Image_Base> needed_ponderation);
        void set_link_ponderation(int id_1, int id_2, std::string needed_ponderation, scls::Text_Style style, scls::Text_Image_Generator* tig);
        void set_link_ponderation(int id_1, int id_2, std::string needed_ponderation, scls::Text_Style style);

        // Loads the link from XML
        void load_from_xml(std::string current_text);
        void load_from_xml(std::shared_ptr<scls::XML_Text_Base> current_text);
        void load_node_from_xml(std::shared_ptr<scls::XML_Text_Base> current_text);
        void load_node_from_xml_late(int needed_node, std::shared_ptr<scls::XML_Text_Base> current_text);

        // Getters and setters
        Node_Base* node(int id)const;
        std::vector<std::shared_ptr<Node_Base>>& nodes();

        // Returns the tree in an image
        std::shared_ptr<scls::__Image_Base> to_image(scls::Text_Style needed_style);
        std::shared_ptr<scls::__Image_Base> to_image();
        // Returns the graph to an XML text
        virtual std::string to_xml_text(){return to_xml_text(std::string("graph"));};
        virtual std::string to_xml_text(std::string object_name){return std::string();};
        virtual std::string to_xml_text_content(){return std::string();};

    private:
        // Nodes in the tree
        std::vector<std::shared_ptr<Node_Base>> a_nodes;
    };
}

#endif // SCLS_IMAGE_GRAPH
