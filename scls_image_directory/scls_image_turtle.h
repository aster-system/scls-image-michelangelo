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

#ifndef SCLS_IMAGE_TURTLE
#define SCLS_IMAGE_TURTLE

#include "../scls_image.h"

// The namespace "scls" is used to simplify the all.
namespace scls {

    // Action than the robot can do
    struct Action {
        #define ACTION_STRUCTURE 1

        // Action constructor
        Action(short action_type);

        // Clone the action
        virtual std::shared_ptr<Action> clone() = 0;
        void clone_base(Action* to_clone);

        // Soft-resets the action
        virtual void soft_reset();

        // Returns the action to a XML text
        virtual std::string to_xml_text(std::string object_name);
        virtual std::string to_xml_text_name();
        std::string to_xml_text_object(std::string object_name);
        std::string to_xml_text_time() const;

        // Duration of the action
        double duration = 0;
        // Passed time at executing this action
        double passed_time = 0;
        // If the object should be save in XML
        bool save_to_xml_text = false;
        // Step of the action
        unsigned short step = 0;
        // Type of the action
        const short type = -1;
        // If the action should directly pass to the other at the end
        bool direct_pass_at_end = false;
    };

    // Structure action
    struct Action_Structure : public Action {
        // Action structure

        // Action_Structure constructor
        Action_Structure();
        Action_Structure(short action_type);

        // Adds an action
        std::shared_ptr<Action> add_action(std::shared_ptr<Action> needed_action);
        // Clears the actions
        void clear_actions();
        // Deletes the last action
        void delete_last_action();

        // Go to the first / next action
        void go_to_first_action();
        void go_to_next_action();
        // If the current action is the end action
        bool is_end_action() const;
        // Returns a last action
        Action* last_action() const;
        short last_action_type() const;
        // Returns the next action
        Action* next_action() const;
        short next_action_type() const;

        // Clone the action
        virtual std::shared_ptr<Action> clone();
        virtual std::shared_ptr<Action_Structure> clone_as_structure();
        void clone_content(std::shared_ptr<Action_Structure> target);

        // Returns the action to a XML text
        std::string to_xml_text_content();
        virtual std::string to_xml_text(std::string object_name);

        // Getters and setters
        inline std::vector<std::shared_ptr<Action>>& actions(){return a_actions;};
        inline int current_action() const {return a_current_action;};
    private:
        // Needed actions
        std::vector<std::shared_ptr<Action>> a_actions;
        // Current action
        int a_current_action = 0;
    };

    // Container action
    class Action_Container {
        // Class representing an entire container of action
    public:
        // Action_Container constructor
        Action_Container();
        Action_Container(std::shared_ptr<Action_Structure> main_thread);
        template <typename T> Action_Container():Action_Container(std::make_shared<T>()){};

        // Returns a thread
        Action_Structure* main_thread() const;
        Action_Structure* thread(int position) const;

        // Creates a new thread
        template <typename T> std::shared_ptr<T> new_thread(){std::shared_ptr<T> t = std::make_shared<T>();a_threads.push_back(t);return t;};

        // Getters and setters
        inline int threads_number() const {return a_threads.size();};
    private:
        // Threads in the container
        std::vector<std::shared_ptr<Action_Structure>> a_threads;
    };

	//*********
	//
	// The Turtle class
	//
	//*********

	class Turtle {
		// Class representing a turtle
	public:

	    struct Action_Fill : public Action {
	        #define TURTLE_ACTION_FILL 1003

	        // Action_Fill constructor
            Action_Fill();

            // Clone the action
            virtual std::shared_ptr<Action> clone();
	    };

	    struct Action_Move_Forward : public Action {
	        #define TURTLE_ACTION_MOVE 1001

	        // Action_Move_Forward constructor
            Action_Move_Forward(double distance);

            // Clone the action
            virtual std::shared_ptr<Action> clone();

            // Needed position
            double a_distance;
            double a_distance_updated = 0;

            // Speed
            double a_speed = 100;
	    };

	    struct Action_Rotate : public Action {
	        #define TURTLE_ACTION_ROTATE 1002

	        // Action_Rotate constructor
            Action_Rotate(double angle);

            // Clone the action
            virtual std::shared_ptr<Action> clone();

            // Needed position
            double a_angle;
            double a_angle_updated = 0;

            // Speed
            double a_speed = SCLS_PI;
	    };

		// Turtle constructor
		Turtle(Image image);
		// Turtle destructor
		~Turtle();

		// Getters and setters
		inline Action_Structure* actions() const {return a_actions.get();};
		inline Point_2D position() const {return a_position;};
		inline double rotation() const {return a_rotation;};
		inline void set_pen_size(int new_pen_size){a_pen_size = new_pen_size;};

		//*********
		// Handle the turtle
		//*********

		// Fills a part
		void add_point_to_fill(Point_2D to_fill);
		void start_filling();
		void stop_filling();

		// Move
		void go_forward(double distance);

		// Handle the pen
		void pen_down();
		void pen_up();

		// Rotation
		void rotate_degrees(double rotation);
		void rotate_radians(double rotation);

		// Add some actions
		void add_action_fill();
		void add_action_move_forward(double needed_distance);
		void add_action_rotate(double needed_rotation);

		// Update the actions in the turtle
		double execute_action(Action* action, double delta_time);
		void update_actions(double delta_time);

	private:

	    // Actions to do
	    std::shared_ptr<Action_Structure> a_actions = std::make_shared<Action_Structure>();

	    // Point to fill
	    bool a_prepare_filling = false;
	    std::list<Point_2D> a_to_fill;
	    int a_to_fill_size = 0;

		// Pen state
		bool a_pen = true;
		int a_pen_size = 3;

		// Position of the turtle
		Point_2D a_position = Point_2D(0, 0);

		// Rotation
		double a_rotation = 0;

		// Attached image
		Image a_image;
	};

}

#endif // SCLS_IMAGE_TURTLE
