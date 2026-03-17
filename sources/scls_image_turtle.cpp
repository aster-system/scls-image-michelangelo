//******************
//
// scls_image_turtle.h
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
// This file contains some tools to handle statistics on image..
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

#include "../scls_image_directory/scls_image_turtle.h"

// The namespace "scls" is used to simplify the all.
namespace scls {

    // Action constructor
    Action::Action(short action_type):type(action_type){};

    // Clone the action
    void Action::clone_base(Action* to_clone){}

    // Soft-resets the action
    void Action::soft_reset(){};

    // Action_Structure constructor
    Action_Structure::Action_Structure():Action_Structure(ACTION_STRUCTURE){};
    Action_Structure::Action_Structure(short action_type):Action(action_type){};

    // Adds an action
    std::shared_ptr<Action> Action_Structure::add_action(std::shared_ptr<Action> needed_action){a_actions.push_back(needed_action);return needed_action;}

    // Clears the actions
    void Action_Structure::clear_actions(){a_actions.clear();a_current_action = 0;};

    // Clone a structure
    std::shared_ptr<Action> Action_Structure::clone(){return clone_as_structure();}
    std::shared_ptr<Action_Structure> Action_Structure::clone_as_structure() {std::shared_ptr<Action_Structure> new_action = std::make_shared<Action_Structure>(type);clone_content(new_action);return new_action;}
    void Action_Structure::clone_content(std::shared_ptr<Action_Structure> target){for(int i = 0;i<static_cast<int>(a_actions.size());i++){target.get()->add_action(a_actions.at(i).get()->clone());}}

    // Deletes the last action
    void Action_Structure::delete_last_action(){if(a_actions.size() > 0){a_actions.erase(a_actions.begin());}};

    // Go to the next action
    void Action_Structure::go_to_first_action(){a_current_action=0;Action* action = next_action();if(action != 0){action->soft_reset();}}
    void Action_Structure::go_to_next_action(){a_current_action++;Action* action = next_action();if(action != 0){action->soft_reset();}}

    // If the current action is the end action
    bool Action_Structure::is_end_action() const{return a_current_action >= static_cast<int>(a_actions.size());}

    // Getters and setters
    Action* Action_Structure::next_action() const{if(static_cast<int>(a_actions.size()) <= a_current_action){return 0;}return a_actions.at(a_current_action).get();}
    short Action_Structure::next_action_type() const{Action* action = next_action();if(action==0){return 0;}return action->type;}

	//*********
	//
	// The Turtle class
	//
	//*********

	// Turtle constructor
	Turtle::Turtle(Image image):a_image(image){}

	// Turtle destructor
	Turtle::~Turtle(){}

	// Action_Move_Forward constructor
    Turtle::Action_Move_Forward::Action_Move_Forward(double distance):Action(TURTLE_ACTION_MOVE){a_distance = distance;};

    // Action_Rotate constructor
    Turtle::Action_Rotate::Action_Rotate(double angle):Action(TURTLE_ACTION_ROTATE){a_angle = angle;};

    // Clone the action
    std::shared_ptr<Action> Turtle::Action_Move_Forward::clone(){std::shared_ptr<Action_Move_Forward> to_return = std::make_shared<Action_Move_Forward>(a_distance);clone_base(to_return.get());return to_return;};
    std::shared_ptr<Action> Turtle::Action_Rotate::clone(){std::shared_ptr<Action_Rotate> to_return = std::make_shared<Action_Rotate>(a_angle);clone_base(to_return.get());return to_return;};

	//*********
	// Handle the turtle
	//*********

	// Move
	void Turtle::go_forward(double distance) {
		Point_2D new_position = a_position + vector_2d_with_angle(a_rotation) * distance;

		if(a_pen){a_image.draw_line(a_position.x(), a_position.y(), new_position.x(), new_position.y(), Color(0, 0, 153), a_pen_size);}

		a_position = new_position;
	}

	// Handle the pen
	void Turtle::pen_down(){a_pen = true;}
	void Turtle::pen_up(){a_pen = false;}

	// Rotation
	void Turtle::rotate_degrees(double rotation){rotate_radians(rotation / (180.0 / SCLS_PI));}
	void Turtle::rotate_radians(double rotation){a_rotation += rotation;}

	// Add some actions
    void Turtle::add_action_move_forward(double needed_distance){actions()->add_action(std::make_shared<Turtle::Action_Move_Forward>(needed_distance));}
    void Turtle::add_action_rotate(double needed_rotation){while(needed_rotation < 0){needed_rotation += 360.0;}actions()->add_action(std::make_shared<Turtle::Action_Rotate>(needed_rotation / (180.0 / SCLS_PI)));}

	// Update the actions in the turtle
    double Turtle::execute_action(Action* action, double delta_time){
        if(action->type == TURTLE_ACTION_MOVE) {
            // Go forward with the turtle
            Turtle::Action_Move_Forward* m = reinterpret_cast<Turtle::Action_Move_Forward*>(action);
            double distance = delta_time * m->a_speed;
            if(m->a_distance_updated + distance > m->a_distance){go_forward(m->a_distance - m->a_distance_updated);return (distance - (m->a_distance - m->a_distance_updated)) / m->a_speed;}
            else{go_forward(distance);m->a_distance_updated += distance;}
        }
        else if(action->type == TURTLE_ACTION_ROTATE) {
            // Rotate with the turtle
            Turtle::Action_Rotate* r = reinterpret_cast<Turtle::Action_Rotate*>(action);
            double angle = delta_time * r->a_speed;
            double diff = (r->a_angle - (r->a_angle_updated));
            while(diff > SCLS_PI){diff -= SCLS_PI * 2;}
            while(diff < -SCLS_PI){diff += SCLS_PI * 2;}
            if(diff < 0){angle *= -1;}

            if(std::abs(r->a_angle - r->a_angle_updated) < std::abs(angle)){rotate_radians(r->a_angle - r->a_angle_updated);return std::abs(angle - (r->a_angle - r->a_angle_updated)) / r->a_speed;}
            else{rotate_radians(angle);r->a_angle_updated += angle;}

            // Update
            while(r->a_angle_updated < 0){r->a_angle_updated += SCLS_PI * 2.0;}
            while(r->a_angle_updated > SCLS_PI * 2.0){r->a_angle_updated -= SCLS_PI * 2.0;}
        }

        return 0;
    }
    void Turtle::update_actions(double delta_time){
        if(actions()->next_action() != 0){
            double time_needed = 0;
            do {
                time_needed = execute_action(actions()->next_action(), delta_time);
                if(time_needed > 0){delta_time -= time_needed;actions()->go_to_next_action();}
            } while(actions()->next_action() != 0 && time_needed > 0);

            if(time_needed == -1){actions()->go_to_next_action();}
        }
    }
}
