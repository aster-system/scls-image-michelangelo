//******************
//
// scls_image_tree.h
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
// This file contains the necessary tools to use tree graph with SCLS.
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

#ifndef SCLS_IMAGE_TREE
#define SCLS_IMAGE_TREE

#include "../scls_image.h"

// The namespace "scls" is used to simplify the all.
namespace scls {
	class Tree {
        // Class representating a tree
    public:
        // Tree constructor
        Tree(std::shared_ptr<Graph_Base> graph, int root_id):a_graph(graph),a_root_id(root_id){};
        Tree():a_root_id(a_graph.get()->Graph_Base::add_node()){};
        virtual ~Tree(){};

        // Return the image of the graph attached to the tree
        std::shared_ptr<scls::__Image_Base> to_image(){place_nodes();return a_graph.get()->to_image();};

        // Adds a node in the tree
        inline Tree* add_node(std::shared_ptr<scls::__Image_Base> new_image){int current_id = a_graph.get()->Graph_Base::add_node(new_image);a_children.push_back(std::make_shared<Tree>(a_graph, current_id));Tree* current = a_children[a_children.size() - 1].get();current->set_direction(a_direction);a_graph.get()->link_nodes(a_root_id,current_id);return current;}
        inline Tree* add_node(std::string value){int current_id = a_graph.get()->add_node(value, 0, 0);a_children.push_back(std::make_shared<Tree>(a_graph, current_id));Tree* current = a_children[a_children.size() - 1].get();current->set_direction(a_direction);a_graph.get()->link_nodes(a_root_id,current_id);return current;}
        inline Tree* add_node(int sub_id, std::shared_ptr<scls::__Image_Base> new_image){Tree* needed_child = child(sub_id);if(needed_child==0){return 0;}return needed_child->add_node(new_image);};
        inline Tree* add_node(int sub_id, std::string value){return add_node(sub_id, scls::to_image(&value));}
        inline Tree* add_node(int sub_id_1, int sub_id_2, std::shared_ptr<scls::__Image_Base> new_image){Tree* needed_child = child(sub_id_1);if(needed_child==0){return 0;}return needed_child->add_node(sub_id_2, new_image);};
        inline Tree* add_node(int sub_id_1, int sub_id_2, std::string value){return add_node(sub_id_1, sub_id_2, scls::to_image(&value));}
        // Places the nodes in the tree
        void place_nodes();
        // Returns a sub-tree in the tree
        //inline Tree<E>* sub_tree(int n){while(static_cast<int>(a_nodes.size()) <= n){a_nodes.push_back(std::make_shared<Tree>());}return a_nodes[n].get();};
        // Returns the total width of the object
        inline double width(){double to_return = 0;for(int i = 0;i<static_cast<int>(a_children.size());i++){to_return+=a_children[i].get()->width();}if(to_return<=1){return 1;}return to_return;};

        // Getters and setters
        inline Tree* child(int id){for(int i = 0;i<static_cast<int>(a_children.size());i++){if(a_children.at(i).get()->root_id()==id){return a_children.at(i).get();}}return 0;};
        inline std::vector<std::shared_ptr<Tree>>& children(){return a_children;};
        inline Graph_Base* graph() const {return a_graph.get();};
        inline scls::__Image_Base* image() const {return a_graph.get()->node(a_root_id)->image();};
        inline std::shared_ptr<scls::__Image_Base> image_shared_ptr() const {return a_graph.get()->node(a_root_id)->image_shared_ptr();};
        inline auto& nodes() {return a_graph.get()->nodes();};
        inline auto* root() {return nodes()[a_root_id].get();};
        inline int root_id() const {return a_root_id;};
        inline void set_direction(Point_2D new_direction) {a_direction = new_direction;for(int i = 0;i<static_cast<int>(a_children.size());i++){a_children.at(i).get()->a_direction=new_direction;}place_nodes();};
        inline void set_image(std::shared_ptr<scls::__Image_Base> new_image){nodes().at(a_root_id).get()->set_image(new_image);};
        inline void set_image(std::string value){nodes().at(a_root_id).get()->set_image(value);};

    private:
        // Total width of the tree
        double total_width(double separation_child_child){
            if(a_children.size() <= 0){return separation_child_child;}

            double to_return = 0;
            for(int i = 0;i<static_cast<int>(a_children.size());i++){to_return += a_children.at(i).get()->total_width(separation_child_child);}
            return to_return;
        };

        // Children of this tree
        std::vector<std::shared_ptr<Tree>> a_children = std::vector<std::shared_ptr<Tree>>();
        // Attached graph for this tree
        std::shared_ptr<Graph_Base> a_graph = std::make_shared<Graph_Base>();

        // Datas for the root
        // Direction for the tree
        scls::Point_2D a_direction = scls::Point_2D(0, -1);
        // ID of the roots for this tree
        int a_root_id = 0;
        // X value of the root
        double a_root_x = 0;
        // Y value of the root
        double a_root_y = 0;
    };
}

#endif // SCLS_IMAGE_TREE
