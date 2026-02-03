//******************
//
// scls_image_tree.cpp
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

#include "../scls_image_directory/scls_image_tree.h"

// The namespace "scls" is used to simplify the all.
namespace scls {
	// Places the nodes in the tree
	void Tree::place_nodes(){
		// Place the root
		a_graph.get()->node(a_root_id)->set_x(a_root_x);
		a_graph.get()->node(a_root_id)->set_y(a_root_y);

		// Get the needed datas
		Point_2D direction = a_direction.normalized();
		Point_2D director = direction.rotated(-90);
		double distance_child_child = 1;
		//scls::Fraction distance_child_parent = 1;
		double needed_width = total_width(distance_child_child);
		if(a_children.size() > 0){needed_width -= a_children.at(0).get()->total_width(distance_child_child);}
		double start_x = (a_root_x + direction.x()) - (needed_width * director.x()) / 2;
		double start_y = (a_root_y + direction.y()) + (needed_width * direction.x()) / 2;

		// Place the children
		double current_x = start_x;
		double current_y = start_y;
		for(int i = 0;i<static_cast<int>(a_children.size());i++) {
			double child_width = a_children[i].get()->total_width(distance_child_child);
			a_children[i].get()->a_root_x = current_x;
			a_children[i].get()->a_root_y = current_y;
			a_children[i].get()->place_nodes();
			current_x += director.x() * child_width;
			current_y += director.y() * child_width;
		}
	}
}
