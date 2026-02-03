//******************
//
// scls_image_graph.cpp
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
// This file contains the source code of scls_image_graph.h.
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

#include "../scls_image_directory/scls_image_graph.h"

// The namespace "scls" is used to simplify the all.
namespace scls {
//******************
    // Graph_Base node
    //******************

    // Node_Base constructor
    Graph_Base::Node_Base::Node_Base(std::shared_ptr<__Image_Base> new_image, double x, double y, int id):a_id(id),a_image(new_image),a_x(x),a_y(y){};
    Graph_Base::Node_Base::Node_Base(){};

    // Links this node with another node
    Graph_Base::Node_Base::Link* Graph_Base::Node_Base::link(std::shared_ptr<Node_Base> node){Link* temp = link_by_object(node);if(link_by_object(node) == temp){a_links.push_back(Link());a_links[a_links.size()-1].target=node;return &a_links[a_links.size()-1];}return temp;};
    // Returns a link by the linked object
    Graph_Base::Node_Base::Link* Graph_Base::Node_Base::link_by_object(Node_Base* node){for(int i = 0;i<static_cast<int>(a_links.size());i++){if(a_links.at(i).target.lock().get() == node){return &a_links[i];}}return 0;};
    Graph_Base::Node_Base::Link* Graph_Base::Node_Base::link_by_object(std::shared_ptr<Node_Base> node){return link_by_object(node.get());};

    // Returns a node to an std::string
    std::string Graph_Base::Node_Base::to_xml_text(std::string node_object_name, std::string node_content){
        std::string links = std::string();
        for(int i = 0;i<static_cast<int>(a_links.size());i++){links += std::string("<link id=") + std::to_string(a_links.at(i).target.lock().get()->id()) + std::string(">");}
        node_content = std::string("<content>") + node_content + std::string("</content>");
        return std::string("<") + node_object_name + to_xml_text_x() + to_xml_text_y() + std::string(">") + node_content + links + std::string("</node>");
    }
    std::string Graph_Base::Node_Base::to_xml_text_x(){if(x() == 0){return std::string();}return std::string(" x=") + std::to_string(x());}
    std::string Graph_Base::Node_Base::to_xml_text_y(){if(y() == 0){return std::string();}return std::string(" y=") + std::to_string(y());}

    //******************
    // Graph
    //******************

    // Loads the graph XML balises
    void load_balises_graph(__Balise_Container* defined_balises) {
        std::shared_ptr<scls::Balise_Style_Datas> current_balise;
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        defined_balises->set_defined_balise("content", current_balise);
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        defined_balises->set_defined_balise("graph", current_balise);
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = false;
        defined_balises->set_defined_balise("link", current_balise);
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        defined_balises->set_defined_balise("node", current_balise);
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        defined_balises->set_defined_balise("nodes", current_balise);
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        defined_balises->set_defined_balise("tree", current_balise);
        current_balise = std::make_shared<scls::Balise_Style_Datas>();
        current_balise.get()->has_content = true;
        defined_balises->set_defined_balise("trees", current_balise);
    }

    // Adds a node in the graph
    int Graph_Base::add_node(std::shared_ptr<__Image_Base> image, double x, double y){std::shared_ptr<Node_Base>new_node=__create_node(image,x,y);a_nodes.push_back(new_node);return new_node.get()->id();};
    int Graph_Base::add_node(std::string value, double x, double y){Text_Style s;std::shared_ptr<Node_Base>new_node=__create_node(string_to_image(value, s),x,y);a_nodes.push_back(new_node);new_node.get()->set_value(value);return new_node.get()->id();}
    int Graph_Base::add_node(std::shared_ptr<__Image_Base> image){return add_node(image, 0, 0);};
    int Graph_Base::add_node(){return add_node(std::shared_ptr<__Image_Base>(), 0, 0);};
    // Links two nodes in the graph
    Graph_Base::Link* Graph_Base::link_nodes(int id_1, int id_2){if(id_1!=id_2&&id_1<static_cast<int>(a_nodes.size())&&id_2<static_cast<int>(a_nodes.size())){return a_nodes[id_1].get()->link(a_nodes[id_2]);}return 0;};
    // Sets the ponderation of a link node
    void Graph_Base::set_link_ponderation(int id_1, int id_2, std::shared_ptr<__Image_Base> needed_ponderation){if(id_1!=id_2&&id_1<static_cast<int>(a_nodes.size())&&id_2<static_cast<int>(a_nodes.size())){Link* needed_link = a_nodes[id_1].get()->link(a_nodes[id_2]);needed_link->ponderation = needed_ponderation;}};
    void Graph_Base::set_link_ponderation(int id_1, int id_2, std::string needed_ponderation, Text_Style style, Text_Image_Generator* tig){set_link_ponderation(id_1, id_2, tig->image_shared_ptr(needed_ponderation, style));};
    void Graph_Base::set_link_ponderation(int id_1, int id_2, std::string needed_ponderation, Text_Style style){set_link_ponderation(id_1, id_2, string_to_image(needed_ponderation, style));};

    // Loads the link from XML
    std::shared_ptr<__Balise_Container> graph_balise;
    void Graph_Base::load_from_xml(std::string current_text){if(graph_balise.get() == 0){graph_balise = std::make_shared<__Balise_Container>();load_balises_graph(graph_balise.get());}load_from_xml(xml(graph_balise, current_text));}
    void Graph_Base::load_from_xml(std::shared_ptr<XML_Text_Base> current_text) {
        // Handle a lot of balises
        for(int i = 0;i<static_cast<int>(current_text->sub_texts().size());i++){
            std::string current_balise_name = current_text->sub_texts()[i].get()->xml_balise_name();
            //std::vector<XML_Attribute>& attributes = current_text->sub_texts()[i].get()->xml_balise_attributes();
            if(current_balise_name == "node"){load_node_from_xml(current_text->sub_texts()[i]);}
        }

        // Handle a lot of late balises
        int current_node = 0;
        for(int i = 0;i<static_cast<int>(current_text->sub_texts().size());i++){
            std::string current_balise_name = current_text->sub_texts()[i].get()->xml_balise_name();
            if(current_balise_name == "node"){load_node_from_xml_late(current_node, current_text->sub_texts()[i]);current_node++;}
        }
    }
    void Graph_Base::load_node_from_xml(std::shared_ptr<XML_Text_Base> current_text) {
        // Handle the attributes
        std::string to_add = std::string();
        double x = 0;double y = 0;
        for(int i = 0;i<static_cast<int>(current_text.get()->xml_attributes().size());i++) {
            if(current_text.get()->xml_attributes()[i].name == std::string("name")){to_add = current_text.get()->xml_attributes()[i].value;}
            else if(current_text.get()->xml_attributes()[i].name == std::string("x")){x = string_to_double(current_text.get()->xml_attributes()[i].value);}
            else if(current_text.get()->xml_attributes()[i].name == std::string("y")){y = string_to_double(current_text.get()->xml_attributes()[i].value);}
        }

        // Handle a lot of balises
        if(current_text.get()->only_text()){to_add = current_text.get()->text();}
        for(int i = 0;i<static_cast<int>(current_text->sub_texts().size());i++){
            std::string balise_content = current_text->sub_texts()[i].get()->xml_balise();
            std::string current_balise_name = current_text->sub_texts()[i].get()->xml_balise_name();
            if(current_balise_name == "content"){to_add = current_text->sub_texts()[i].get()->text();}
        }

        // Creates the node
        add_node(to_add, x, y);
    }
    void Graph_Base::load_node_from_xml_late(int needed_node, std::shared_ptr<XML_Text_Base> current_text) {
        Text_Style needed_style;

        // Handle possible attributes
	    std::vector<XML_Attribute>& attributes = current_text.get()->xml_balise_attributes();
	    for(int i = 0;i<static_cast<int>(attributes.size());i++) {
            if(attributes.at(i).name == std::string("link") || attributes.at(i).name == std::string("links")){
                std::vector<std::string> cutted = cut_string(attributes.at(i).value, std::string(";"));
                for(int j = 0;j<static_cast<int>(cutted.size());j++) {
                    int needed_id = std::stoi(cutted.at(j));std::string needed_ponderation = std::string();
                    if(needed_id == needed_node){print(std::string("PLEOS Graph"), std::string("Can't link the ") + std::to_string(needed_id) + std::string(" with himself."));}
                    else {
                        link_nodes(needed_node, needed_id);
                        set_link_ponderation(needed_node, needed_id, needed_ponderation, needed_style);
                    }
                }
            }
	    }

	    // Handle the other balises
        for(int i = 0;i<static_cast<int>(current_text->sub_texts().size());i++){
            std::string balise_content = current_text->sub_texts()[i].get()->xml_balise();
            std::string current_balise_name = current_text->sub_texts()[i].get()->xml_balise_name();
            std::vector<XML_Attribute>& attributes = current_text->sub_texts()[i].get()->xml_balise_attributes();
            if(current_balise_name == "link"){
                // Link the node
                int needed_id = 0;std::string needed_ponderation = std::string();
                for(int j = 0;j<static_cast<int>(attributes.size());j++) {
                    std::string attribute_name = attributes[j].name;
                    std::string attribute_value = attributes[j].value;
                    if(attribute_name == "id") {needed_id = std::stoi(attribute_value);}
                    else if(attribute_name == "value") {needed_ponderation = attribute_value;}
                }

                // Add the link
                if(needed_id == needed_node){print(std::string("PLEOS Graph"), std::string("Can't link the ") + std::to_string(needed_id) + std::string(" with himself."));}
                else {
                    link_nodes(needed_node, needed_id);
                    set_link_ponderation(needed_node, needed_id, needed_ponderation, needed_style);
                }
            }
        }
    }

    // Returns the tree in an image
    std::shared_ptr<__Image_Base> Graph_Base::to_image(){return to_image(Text_Style());};
    std::shared_ptr<__Image_Base> Graph_Base::to_image(Text_Style needed_style) {
        std::shared_ptr<__Image_Base> to_return;
        if(static_cast<int>(a_nodes.size()) > 0) {
            // Get the needed children images
            double max_x = 0;  double min_x = 0; bool min_x_used = false; bool max_x_used = false;
            double max_y = 0;  double min_y = 0; bool min_y_used = false; bool max_y_used = false;
            int pixel_in_value = 100;
            std::vector<std::shared_ptr<__Image_Base>> images;
            for(int i = 0;i<static_cast<int>(a_nodes.size());i++) {
                std::shared_ptr<__Image_Base> current_image = a_nodes[i].get()->image_shared_ptr();
                images.push_back(current_image);double image_height = 0;double image_width = 0;
                if(current_image.get() != 0){image_height = current_image.get()->height();image_width = current_image.get()->width();}

                // Get the datas about the image
                // Datas for width
                double current_min_x = a_nodes[i].get()->x() * pixel_in_value - image_width / 2.0;
                double current_max_x = a_nodes[i].get()->x() * pixel_in_value + image_width / 2.0;
                if(current_min_x < min_x || !min_x_used){min_x = current_min_x;min_x_used=true;}
                if(current_max_x > max_x || !max_x_used){max_x = current_max_x;max_x_used=true;}
                // Datas for height
                double current_min_y = a_nodes[i].get()->y() * pixel_in_value - image_height / 2.0;
                double current_max_y = a_nodes[i].get()->y() * pixel_in_value + image_height / 2.0;
                if(current_min_y < min_y || !min_y_used){min_y = current_min_y;min_y_used=true;}
                if(current_max_y > max_y || !max_y_used){max_y = current_max_y;max_y_used=true;}
            }

            // Get all the positions
            std::vector<double> needed_x;
            std::vector<double> needed_y;
            double total_height = max_y - min_y;
            double total_width = max_x - min_x;
            for(int i = 0;i<static_cast<int>(a_nodes.size());i++) {
                // Get the needed value
                std::shared_ptr<__Image_Base> current_image = images[i];
                int image_height = 0;int image_width = 0;
                if(current_image.get() != 0){image_height = current_image.get()->height();image_width = current_image.get()->width();}

                // Datas for width
                double current_x = (a_nodes[i].get()->x() * pixel_in_value - min_x);
                current_x -= image_width / 2.0;
                needed_x.push_back(current_x);
                // Datas for height
                double current_y = total_height - (a_nodes[i].get()->y() * pixel_in_value - min_y);
                current_y -= image_height / 2.0;
                needed_y.push_back(current_y);
            }

            // Prepare the creation of the image
            int x_offset = 0;int y_offset = 0;int height_to_add = 0;int width_to_add = 0;
            x_offset = needed_style.border_left_width() + needed_style.padding_left();y_offset = needed_style.border_top_width() + needed_style.padding_top();
            height_to_add = y_offset + needed_style.border_right_width() + needed_style.padding_right();width_to_add = x_offset + needed_style.border_bottom_width() + needed_style.padding_bottom();

            // Draw the final image
            to_return = std::make_shared<__Image_Base>(total_width + width_to_add, total_height + height_to_add, Color(255, 255, 255));
            // Draw the border
            to_return.get()->draw_border(needed_style.border_left_width(), needed_style.border_top_width(), needed_style.border_bottom_width(), needed_style.border_right_width(), needed_style.border_color());

            // Draw the links
            for(int i = 0;i<static_cast<int>(a_nodes.size());i++) {
                std::vector<Link>& links = a_nodes[i].get()->links();
                for(int j = 0;j<static_cast<int>(links.size());j++) {
                    std::shared_ptr<Node_Base> current_node = links[j].target.lock();
                    int current_id = current_node.get()->id();
                    if(current_id > a_nodes[i].get()->id() || true) {
                        // Get the good link type
                        Link_Type link_type = links[j].type;

                        // Calculate with the angle
                        if(link_type == Link_Type::LT_Angle) {
                            // Get the good angle
                            double x_diff = needed_x[current_id] - needed_x[i];
                            double y_diff = needed_y[current_id] - needed_y[i];
                            double angle = vector_2d_angle(x_diff, y_diff) * (180.0 / SCLS_PI);
                            while(angle < 0){angle += 360;}
                            if(angle > 225 && angle <= 315){link_type = Link_Type::LT_Bottom;}
                            else if(angle > 135 && angle <= 225){link_type = Link_Type::LT_Left;}
                            else if(angle <= 45 || angle >= 315){link_type = Link_Type::LT_Right;}
                            else {link_type = Link_Type::LT_Top;}
                        }

                        // Get the good values
                        double current_x_end;double current_x_start;
                        double current_y_end;double current_y_start;
                        double image_height_current = 0;double image_width_current = 0;
                        if(images.at(current_id).get() != 0){image_height_current = images.at(current_id).get()->height();image_width_current = images.at(current_id).get()->width();}
                        double image_height_i = 0;double image_width_i = 0;
                        if(images.at(i).get() != 0){image_height_i = images.at(i).get()->height();image_width_i = images.at(i).get()->width();}
                        if(link_type == Link_Type::LT_Middle) {
                            // Start at the middle
                            current_x_end = needed_x[current_id] + image_width_current / 2.0;
                            current_x_start = needed_x[i] + image_width_i / 2.0;
                            current_y_end = needed_y[current_id] + image_height_current / 2.0;
                            current_y_start = needed_y[i] + image_height_i / 2.0;
                        }
                        else if(link_type == Link_Type::LT_Bottom) {
                            // Bottom link
                            current_x_end = needed_x[current_id] + image_width_current / 2.0;
                            current_y_end = needed_y[current_id] + image_height_current;
                            current_x_start = needed_x[i] + image_width_i / 2.0;
                            current_y_start = needed_y[i];
                        }
                        else if(link_type == Link_Type::LT_Left) {
                            // Left link
                            current_x_end = needed_x[current_id] + static_cast<double>(images[i].get()->width());
                            current_y_end = needed_y[current_id] + static_cast<double>(images[i].get()->height()) / 2.0;
                            current_x_start = needed_x[i];
                            current_y_start = needed_y[i] + image_height_i / 2.0;
                        }
                        else if(link_type == Link_Type::LT_Right) {
                            // Right link
                            current_x_end = needed_x[current_id] - image_width_current;
                            current_y_end = needed_y[current_id] + image_height_i / 2.0;
                            current_x_start = needed_x[i] + image_width_i;
                            current_y_start = needed_y[i] + image_height_i / 2.0;
                        }
                        else {
                            // Top link
                            current_x_end = needed_x[current_id] + image_width_current / 2.0;
                            current_y_end = needed_y[current_id];
                            current_x_start = needed_x[i] + image_width_i / 2.0;
                            current_y_start = needed_y[i] + double(image_height_current);
                        }//*/

                        // Paste the ponderation
                        if(links[j].ponderation.get() != 0) {
                            double x_middle = (current_x_start + current_x_end) / 2;
                            double y_middle = (current_y_start + current_y_end) / 2;

                            // Put the image at the center
                            x_middle -= links[j].ponderation.get()->width() / 2;
                            y_middle -= links[j].ponderation.get()->height() / 2;

                            to_return.get()->paste(links[j].ponderation.get(), x_middle + x_offset, y_middle + y_offset);
                        }

                        // Draw the link
                        to_return.get()->draw_line(current_x_start + x_offset, current_y_start + y_offset, current_x_end + x_offset, current_y_end + y_offset, Color(0, 0, 0), 2);
                    }
                }
            } //*/

            // Paste the children
            for(int i = 0;i<static_cast<int>(a_nodes.size());i++) {
                std::shared_ptr<__Image_Base> current_image = images[i];
                if(current_image.get() == 0){continue;}
                to_return.get()->paste(current_image.get(), needed_x[i] + x_offset, needed_y[i] + y_offset);
            }
        }
        return to_return;
    }

    // Getters and setters
    Graph_Base::Node_Base* Graph_Base::node(int id)const{if(id > static_cast<int>(a_nodes.size())){return 0;}return a_nodes.at(id).get();};
    std::vector<std::shared_ptr<Graph_Base::Node_Base>>& Graph_Base::nodes() {return a_nodes;};
}
