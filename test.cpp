//******************
//
// scls_image.h
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
// This file is a test file for the library.
//

#define SCLS_FOUNDATION_PATH "../scls-foundation-leonhard/scls_foundation.h"
#define SCLS_MATH_PATH "../scls-math-carl/scls_math.h"
#include "scls_image.h"
#include "../scls-math-carl/scls_math_directory/scls_math_string.h"
#include "scls_image_directory/scls_image_tree.h"
#include "scls_image_directory/scls_image_statistic.h"

SCLS_INIT;

class __Formula : public scls::__Algebra_Element {
public:
	// __Formula constructor
	__Formula(){};
	__Formula(std::string unknown_name):__Formula(){if(scls::string_is_complex_number(unknown_name)){a_value = scls::string_to_complex(unknown_name);}else{new_unknown(unknown_name);}};

	// Creates a new algebra element of the same type
	virtual void algebra_clone(__Algebra_Element* b) const {clone(reinterpret_cast<__Formula*>(b));};
	virtual std::shared_ptr<__Algebra_Element> algebra_clone() const {return clone();};
	virtual void clone(__Formula* b) const {__clone_base(b);b->a_value = a_value;};
	virtual std::shared_ptr<__Formula> clone() const {std::shared_ptr<__Formula> b = std::make_shared<__Formula>();__clone_base(b.get());b.get()->a_modified = a_modified;b.get()->a_value = a_value;return b;};
	virtual std::shared_ptr<__Algebra_Element> new_algebra_element() const {std::shared_ptr<__Formula> s = std::make_shared<__Formula>();s.get()->a_parent=a_this_object;s.get()->a_this_object=s;s.get()->a_modified = false;return s;};
	virtual std::shared_ptr<__Algebra_Element> new_algebra_element(std::string content) const {std::shared_ptr<__Formula> s = std::make_shared<__Formula>(content);s.get()->a_parent=a_this_object;s.get()->a_this_object=s;return s;};

	// Adds an element to this one
	void add(__Formula* formula){
		if(is_final_element() && !is_unknown() && a_value == 0){formula->clone(this);}
		else if(is_final_element() && formula->is_final_element() && !is_unknown() && !formula->is_unknown()){
			a_value = reinterpret_cast<__Formula*>(formula)->a_value;
		}
		else {
			if(algebra_operator() != std::string("+")){sub_place();set_algebra_operator(std::string("+"));}
			algebra_elements().push_back(formula->algebra_clone());
		}
	}

	// Operates this element with another one
	virtual void operate(__Algebra_Element* other, std::string operation) {
		if(!a_modified){a_modified = true;reinterpret_cast<__Formula*>(other)->algebra_clone(this);}
		else if(operation == std::string("+")){add(reinterpret_cast<__Formula*>(other));}
		else if(is_final_element() && other->is_final_element() && !is_unknown() && !other->is_unknown()){
			a_value += reinterpret_cast<__Formula*>(other)->a_value;
		}
		else {
			std::shared_ptr<__Formula> current = clone();
			sub_place();set_algebra_operator(operation);
			algebra_elements().push_back(other->algebra_clone());
		}
	};

	// Returns the element to a simple std::string
	virtual std::string to_mathml(scls::Textual_Math_Settings* settings) const{return std::string();};
	virtual std::string to_std_string(scls::Textual_Math_Settings* settings) const{
		std::string to_return = std::string();
		if(is_final_element()){
			if(!is_unknown()){return a_value.to_std_string(settings);}
			else{to_return = algebra_unknown()->name;}
		}
		else {
			for(int i = 0;i<static_cast<int>(algebra_elements_const().size());i++) {
				to_return += std::string("(");
				to_return += algebra_elements_const().at(i).get()->to_std_string(settings);
				to_return += std::string(")");
				if(i < static_cast<int>(algebra_elements_const().size()) - 1){to_return += std::string(" ") + algebra_operator() + std::string(" ");}
			}
		}
		return to_return;
	};

private:
	// If the element is modified or not
	bool a_modified = true;
	// Value of the formula
	scls::Complex a_value = scls::Complex(0);
};

int main() {
	//std::shared_ptr<__Formula> formula = scls::string_to_algebra_element<__Formula>("(3 * x + 2 + 7 * x) ^ (4 * x)");
	//std::cout << formula.get()->to_std_string(0) << std::endl;

	scls::Tree g = scls::Tree();
	g.set_image("Mathématiques");
	scls::Tree* c = g.add_node(std::string("Algèbre"));
	c = g.add_node(std::string("Analyse"));
	c= g.add_node(std::string("Logique"));
	c= g.add_node(std::string("Géométrie"));
	g.to_image().get()->save_png("tests/g.png");

    return 0;
}
