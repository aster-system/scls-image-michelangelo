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

    // Action
    std::string Action::to_xml_text(std::string object_name){return std::string("<") + to_xml_text_name() + to_xml_text_object(object_name) + to_xml_text_time() + std::string(">");}
    std::string Action::to_xml_text_name(){return std::string("action");}
    std::string Action::to_xml_text_object(std::string object_name){if(object_name == std::string()){return std::string();}return std::string(" object=\"") + object_name + std::string("\"");}
    std::string Action::to_xml_text_time() const{if(duration == 0){return std::string();}return std::string(" time=") + scls::Fraction::from_double(duration).to_std_string(0);}

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

    // Last action of the structure
    Action* Action_Structure::last_action() const {if(static_cast<int>(a_actions.size()) <= 0){return 0;}return a_actions.at(0).get();};
    short Action_Structure::last_action_type() const {return last_action()->type;};

    // Getters and setters
    Action* Action_Structure::next_action() const{if(static_cast<int>(a_actions.size()) <= a_current_action){return 0;}return a_actions.at(a_current_action).get();}
    short Action_Structure::next_action_type() const{Action* action = next_action();if(action==0){return 0;}return action->type;}

    // Action structure
    std::string Action_Structure::to_xml_text_content(){std::string content = std::string();for(int i = 0;i<static_cast<int>(a_actions.size());i++){content += a_actions.at(i).get()->to_xml_text(std::string());if(i<static_cast<int>(a_actions.size())-1){content+=std::string("\n");}}return content;}
    std::string Action_Structure::to_xml_text(std::string object_name){return std::string("<") + to_xml_text_name() + to_xml_text_object(object_name) + std::string(">\n") + to_xml_text_content() + std::string("\n</") + to_xml_text_name() + std::string(">");}

    // Action_Container constructor
    Action_Container::Action_Container(){}
    Action_Container::Action_Container(std::shared_ptr<Action_Structure> main_thread){a_threads.push_back(main_thread);}

    // Returns a thread
    Action_Structure* Action_Container::main_thread() const {return a_threads.at(0).get();}
    Action_Structure* Action_Container::thread(int position) const {return a_threads.at(position).get();}

	//*********
	//
	// The Turtle class
	//
	//*********

	// Turtle constructor
	Turtle::Turtle(Image image):a_image(image){}

	// Turtle destructor
	Turtle::~Turtle(){}

	// Action_Fill constructor
    Turtle::Action_Fill::Action_Fill():Action(TURTLE_ACTION_FILL){};

	// Action_Move_To constructor
    Turtle::Action_Move_Forward::Action_Move_Forward(double distance):Action(TURTLE_ACTION_MOVE){a_distance = distance;};
    Turtle::Action_Move_To::Action_Move_To(Point_2D position):Action(TURTLE_ACTION_MOVE_TO){a_position = position;};

    // Action_Rotate constructor
    Turtle::Action_Rotate::Action_Rotate(double angle):Action(TURTLE_ACTION_ROTATE){a_angle = angle;};
    Turtle::Action_Rotate_Towards::Action_Rotate_Towards(Point_2D position):Action(TURTLE_ACTION_ROTATE_TOWARDS){a_position = position;};

    // Action_Pen_Down constructor
    Turtle::Action_Pen_Down::Action_Pen_Down():Action(TURTLE_ACTION_PEN_DOWN){}
    Turtle::Action_Pen_Up::Action_Pen_Up():Action(TURTLE_ACTION_PEN_UP){}

    // Clone the action
    std::shared_ptr<Action> Turtle::Action_Fill::clone(){std::shared_ptr<Action_Fill> to_return = std::make_shared<Action_Fill>();clone_base(to_return.get());return to_return;};
    std::shared_ptr<Action> Turtle::Action_Move_Forward::clone(){std::shared_ptr<Action_Move_Forward> to_return = std::make_shared<Action_Move_Forward>(a_distance);clone_base(to_return.get());return to_return;};
    std::shared_ptr<Action> Turtle::Action_Move_To::clone(){std::shared_ptr<Action_Move_To> to_return = std::make_shared<Action_Move_To>(a_position);clone_base(to_return.get());return to_return;};
    std::shared_ptr<Action> Turtle::Action_Rotate::clone(){std::shared_ptr<Action_Rotate> to_return = std::make_shared<Action_Rotate>(a_angle);clone_base(to_return.get());return to_return;};
    std::shared_ptr<Action> Turtle::Action_Rotate_Towards::clone(){std::shared_ptr<Action_Rotate_Towards> to_return = std::make_shared<Action_Rotate_Towards>(a_position);clone_base(to_return.get());to_return.get()->a_speed = a_speed;return to_return;};
    std::shared_ptr<Action> Turtle::Action_Pen_Down::clone(){std::shared_ptr<Action_Pen_Down> to_return = std::make_shared<Action_Pen_Down>();clone_base(to_return.get());return to_return;};
    std::shared_ptr<Action> Turtle::Action_Pen_Up::clone(){std::shared_ptr<Action_Pen_Up> to_return = std::make_shared<Action_Pen_Up>();clone_base(to_return.get());return to_return;};

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
	void Turtle::rotate_towards(Point_2D point){rotate_towards(point, 0);}
	void Turtle::rotate_towards(Point_2D point, double offset) {
		a_rotation = scls::vector_2d_angle(point - a_position) + offset;
	}

	// Add some actions
	void Turtle::add_action_fill(){actions()->add_action(std::make_shared<Turtle::Action_Fill>());}
    void Turtle::add_action_move_forward(double needed_distance){actions()->add_action(std::make_shared<Turtle::Action_Move_Forward>(needed_distance));}
    void Turtle::add_action_move_to(Point_2D needed_position){actions()->add_action(std::make_shared<Turtle::Action_Move_To>(needed_position));}
    void Turtle::add_action_rotate(double needed_rotation){while(needed_rotation < 0){needed_rotation += 360.0;}actions()->add_action(std::make_shared<Turtle::Action_Rotate>(needed_rotation / (180.0 / SCLS_PI)));}
    void Turtle::add_action_rotate_towards(Point_2D needed_position){actions()->add_action(std::make_shared<Turtle::Action_Rotate_Towards>(needed_position));}
    void Turtle::add_action_pen_down(){actions()->add_action(std::make_shared<Turtle::Action_Pen_Down>());}
    void Turtle::add_action_pen_up(){actions()->add_action(std::make_shared<Turtle::Action_Pen_Up>());}

    // Fills a part
    void Turtle::add_point_to_fill(Point_2D to_fill){if(!(a_to_fill.back().x() == to_fill.x() && a_to_fill.back().y() == to_fill.y())){a_to_fill.push_back(to_fill);a_to_fill_size++;}}
    void Turtle::start_filling() {a_prepare_filling = true;}
    void Turtle::stop_filling() {
        std::vector<Point_2D> needed_vector = std::vector<Point_2D>(a_to_fill_size);
        for(std::size_t i = 0;i<needed_vector.size();i++){needed_vector[i] = a_to_fill.front();a_to_fill.pop_front();}
        a_image.fill_form(needed_vector, Color(0, 0, 153));
        a_prepare_filling = false;a_to_fill_size = 0;
    }

	// Update the actions in the turtle
    double Turtle::execute_action(Action* action, double delta_time){
        if(action->type == TURTLE_ACTION_FILL) {
            // Go forward with the turtle
            Turtle::Action_Fill* m = reinterpret_cast<Turtle::Action_Fill*>(action);
            if(a_prepare_filling){stop_filling();}
            else{start_filling();}
            return -1;
        }
        else if(action->type == TURTLE_ACTION_MOVE) {
            // Go forward with the turtle
            Turtle::Action_Move_Forward* m = reinterpret_cast<Turtle::Action_Move_Forward*>(action);
            double distance = delta_time * m->a_speed;if(m->a_distance < 0){distance = -distance;}
            if(std::abs(m->a_distance_updated + distance) > std::abs(m->a_distance)){go_forward(m->a_distance - m->a_distance_updated);return std::abs(distance - (m->a_distance - m->a_distance_updated)) / m->a_speed;}
            else{go_forward(distance);m->a_distance_updated += distance;}
        }
        else if(action->type == TURTLE_ACTION_MOVE_TO) {
            // Go forward with the turtle
            Turtle::Action_Move_To* r = reinterpret_cast<Turtle::Action_Move_To*>(action);

            // Rotate
            double diff = scls::vector_2d_angle(r->a_position - a_position) - a_rotation;
            if(std::abs(diff) > 0.0001) {
                double angle = delta_time * r->a_speed_rotation;
                while(diff > SCLS_PI){diff -= SCLS_PI * 2;}
                while(diff < -SCLS_PI){diff += SCLS_PI * 2;}
                if(diff < 0){angle *= -1;}

                if(std::abs(diff) < std::abs(angle)){rotate_radians(diff);diff=0; delta_time -= std::abs(angle - diff) / r->a_speed_rotation;}
                else{rotate_radians(angle);}

                while(a_rotation < 0){a_rotation += SCLS_PI * 2.0;}
                while(a_rotation > SCLS_PI * 2.0){a_rotation -= SCLS_PI * 2.0;}
            }

            // Move
            if(std::abs(diff) < 0.0001) {
                double distance_updated = r->a_position.distance(a_position);
                double distance = delta_time * r->a_speed;
                if(distance > distance_updated){go_forward(distance_updated);return (distance - (distance_updated)) / r->a_speed;}
                else{go_forward(distance);}
            }
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
        else if(action->type == TURTLE_ACTION_ROTATE_TOWARDS) {
            // Rotate with the turtle
            Turtle::Action_Rotate_Towards* r = reinterpret_cast<Turtle::Action_Rotate_Towards*>(action);
            double angle = delta_time * r->a_speed;
            double diff = scls::vector_2d_angle(r->a_position - a_position) - a_rotation;
            while(diff > SCLS_PI){diff -= SCLS_PI * 2;}
            while(diff < -SCLS_PI){diff += SCLS_PI * 2;}
            if(diff < 0){angle *= -1;}

            if(std::abs(diff) < std::abs(angle)){rotate_radians(diff);return std::abs(angle - diff) / r->a_speed;}
            else{rotate_radians(angle);}

            while(a_rotation < 0){a_rotation += SCLS_PI * 2.0;}
            while(a_rotation > SCLS_PI * 2.0){a_rotation -= SCLS_PI * 2.0;}
        }
        else if(action->type == TURTLE_ACTION_PEN_DOWN) {pen_down();return -1;}
        else if(action->type == TURTLE_ACTION_PEN_UP) {pen_up();return -1;}

        return 0;
    }
    void Turtle::update_actions(double delta_time){
        if(actions()->next_action() != 0){
            double time_needed = 0;
            do {
                time_needed = execute_action(actions()->next_action(), delta_time);
                if(time_needed > 0){delta_time -= time_needed;if(a_prepare_filling){add_point_to_fill(a_position);}actions()->go_to_next_action();}
            } while(actions()->next_action() != 0 && time_needed > 0);

            if(time_needed == -1){actions()->go_to_next_action();}
        }
    }

    // Draw a form by only controlling advancement and rotation
    void draw_by_rotation(Image img, std::shared_ptr<scls::Formula_Base> rotation, double start_angle) {
        scls::Turtle t = scls::Turtle(img);
        t.pen_up();
        t.rotate_degrees(45);
        t.go_forward(1414 / 2);
        t.rotate_degrees(-45 + start_angle);
        t.pen_down();

        for(int i = 0;i<10000;i++) {
            t.rotate_degrees(rotation.get()->replace_unknowns("x", scls::Fraction(i)).get()->value<scls::Fraction>()->to_double() / 100.0);
            t.go_forward(1);
        }
    }

    // Draw bag
    void draw_bag(Image img, double start_angle) {
        scls::Turtle t = scls::Turtle(img);
        t.pen_up();
        t.rotate_degrees(45);
        t.go_forward(1414 / 2);
        t.rotate_degrees(-45);
        t.go_forward(400);
        t.pen_down();

        std::shared_ptr<scls::Formula_Base> f_x = scls::string_to_algebra_element<scls::Formula_Base>("400*cos(x)-200*sin(x)");
        for(int i = 0;i<10000;i++) {
            double x = f_x.get()->replace_unknowns("x", scls::Fraction(i, 1000)).get()->value<scls::Fraction>()->to_double();
            double y = x * -std::sin(static_cast<double>(i) / 1000.0);
            t.rotate_towards(scls::Point_2D(500, 500) + scls::Point_2D(x, y).rotated(start_angle));
            t.go_forward(1);
        }
    }

    // Draw lemniscate
    void draw_lemniscate_bernoulli_actions(Turtle* t, double start_angle) {
        std::shared_ptr<scls::Formula_Base> f_x = scls::string_to_algebra_element<scls::Formula_Base>("400*(sin(x)/(1+cos(x)*cos(x)))");
        for(int i = 0;i<750;i++) {
            double x = f_x.get()->replace_unknowns("x", scls::Fraction(i, 100)).get()->value<scls::Fraction>()->to_double();
            double y = x * std::cos(static_cast<double>(i) / 100.0);
            t->add_action_move_to(scls::Point_2D(540, 910) + scls::Point_2D(x, y).rotated(start_angle));
        }
    }
    void draw_lemniscate_bernoulli(Turtle* t, double start_angle) {
        std::shared_ptr<scls::Formula_Base> f_x = scls::string_to_algebra_element<scls::Formula_Base>("400*(sin(x)/(1+cos(x)*cos(x)))");
        for(int i = 0;i<10000;i++) {
            double x = f_x.get()->replace_unknowns("x", scls::Fraction(i, 1000)).get()->value<scls::Fraction>()->to_double();
            double y = x * std::cos(static_cast<double>(i) / 1000.0);
            t->rotate_towards(scls::Point_2D(540, 910) + scls::Point_2D(x, y).rotated(start_angle));
            t->go_forward(1);
        }
    }
    void draw_lemniscate_bernoulli(Image img, double start_angle) {
        scls::Turtle t = scls::Turtle(img);
        t.pen_up();
        t.rotate_degrees(45);
        t.go_forward(1414 / 2);
        t.rotate_degrees(-45);
        t.pen_down();

        draw_lemniscate_bernoulli(&t, start_angle);
    }
    void draw_lemniscate_gerono_actions(Turtle* t, double start_angle) {
        std::shared_ptr<scls::Formula_Base> f_x = scls::string_to_algebra_element<scls::Formula_Base>("400*sin(x)");
        for(int i = 0;i<750;i++) {
            double x = f_x.get()->replace_unknowns("x", scls::Fraction(i, 100)).get()->value<scls::Fraction>()->to_double();
            double y = x * std::cos(static_cast<double>(i) / 100.0);
            t->add_action_move_to(scls::Point_2D(540, 910) + scls::Point_2D(x, y).rotated(start_angle));
        }
    }
    void draw_lemniscate_gerono(Image img, double start_angle) {
        scls::Turtle t = scls::Turtle(img);
        t.pen_up();
        t.rotate_degrees(45);
        t.go_forward(1414 / 2);
        t.rotate_degrees(-45);
        t.pen_down();

        std::shared_ptr<scls::Formula_Base> f_x = scls::string_to_algebra_element<scls::Formula_Base>("400*sin(x)");
        for(int i = 0;i<10000;i++) {
            double x = f_x.get()->replace_unknowns("x", scls::Fraction(i, 1000)).get()->value<scls::Fraction>()->to_double();
            double y = x * std::cos(static_cast<double>(i) / 1000.0);
            t.rotate_towards(scls::Point_2D(500, 500) + scls::Point_2D(x, y).rotated(start_angle));
            t.go_forward(1);
        }
    }

    // Draw spirals
    void draw_spiral_archimedian_actions(Turtle* t, double start_angle) {
        std::shared_ptr<scls::Formula_Base> f = scls::string_to_algebra_element<scls::Formula_Base>("x");
        for(int i = 0;i<2000;i++) {
            t->add_action_rotate(2.5);
            t->add_action_move_forward(f.get()->replace_unknowns("x", scls::Fraction(i)).get()->value<scls::Fraction>()->to_double() / 100.0);
        }
    }
    void draw_spiral_archimedian(Image img, double start_angle) {
        scls::Turtle t = scls::Turtle(img);
        t.pen_up();
        t.rotate_degrees(45);
        t.go_forward(1414 / 2);
        t.rotate_degrees(-45 + start_angle);
        t.pen_down();

        std::shared_ptr<scls::Formula_Base> f = scls::string_to_algebra_element<scls::Formula_Base>("x");
        for(int i = 0;i<10000;i++) {
            t.rotate_degrees(1);
            t.go_forward(f.get()->replace_unknowns("x", scls::Fraction(i)).get()->value<scls::Fraction>()->to_double() / 100.0);
        }
    }
    void draw_spiral_fermat(Image img, double start_angle) {
        scls::Turtle t = scls::Turtle(img);
        t.pen_up();
        t.rotate_degrees(45);
        t.go_forward(1414 / 2);
        t.rotate_degrees(-45 + start_angle);
        t.pen_down();

        std::shared_ptr<scls::Formula_Base> f = scls::string_to_algebra_element<scls::Formula_Base>("exp(ln((x*x+1)/4))");
        for(int i = 0;i<10000;i++) {
            t.rotate_degrees(8);
            t.go_forward(f.get()->replace_unknowns("x", scls::Fraction(i)).get()->value<scls::Fraction>()->to_double() / 100.0);
        }
    }
    void draw_spiral_logarithm(Image img, double start_angle) {
        scls::Turtle t = scls::Turtle(img);
        t.pen_up();
        t.rotate_degrees(45);
        t.go_forward(1414 / 2);
        t.rotate_degrees(-45 + start_angle);
        t.pen_down();

        std::shared_ptr<scls::Formula_Base> f = scls::string_to_algebra_element<scls::Formula_Base>("exp(ln((x+1)*2))");
        for(int i = 0;i<10000;i++) {
            t.rotate_degrees(8);
            t.go_forward(f.get()->replace_unknowns("x", scls::Fraction(i)).get()->value<scls::Fraction>()->to_double() / 100.0);
        }
    }

}
