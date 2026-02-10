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
#include "../scls-math-carl/scls_math_directory/scls_math_boolean.h"
#include "../scls-math-carl/scls_math_directory/scls_math_string.h"
#include "scls_image_directory/scls_image_table.h"
#include "scls_image_directory/scls_image_statistic.h"

SCLS_INIT;

std::vector<scls::Algebra_Element::Algebra_Operator> formula_operators = {scls::Algebra_Element::Algebra_Operator("^"), scls::Algebra_Element::Algebra_Operator("*"), scls::Algebra_Element::Algebra_Operator("+")};
class Formula_Base : public scls::Algebra_Element {
public:
    // Container of unknowns
    struct Formula_Unknown : public Algebra_Element::__Algebra_Unknown {scls::Complex value = 0;};

	// Formula_Base constructor
	Formula_Base(){};
	Formula_Base(std::string unknown_name):Formula_Base(){if(scls::string_is_complex_number(unknown_name)){a_value = scls::string_to_complex(unknown_name);}else{new_unknown(unknown_name);}};

	// Creates a new algebra element of the same type
	virtual void algebra_clone(Algebra_Element* b) const {clone(reinterpret_cast<Formula_Base*>(b));};
	virtual std::shared_ptr<Algebra_Element> algebra_clone() const {return clone();};
	virtual void clone(Formula_Base* b) const {__clone_base(b);b->a_value = a_value;};
	virtual std::shared_ptr<Formula_Base> clone() const {std::shared_ptr<Formula_Base> b = std::make_shared<Formula_Base>();__clone_base(b.get());b.get()->a_modified = a_modified;b.get()->a_value = a_value;return b;};
	virtual std::shared_ptr<Algebra_Element> new_algebra_element() const {std::shared_ptr<Formula_Base> s = std::make_shared<Formula_Base>();s.get()->a_parent=a_this_object;s.get()->a_this_object=s;s.get()->a_modified = false;return s;};
	virtual std::shared_ptr<Algebra_Element> new_algebra_element(std::string content) const {std::shared_ptr<Formula_Base> s = std::make_shared<Formula_Base>(content);s.get()->a_parent=a_this_object;s.get()->a_this_object=s;return s;};

	// Adds an element to this one
	void add(Formula_Base* formula){
		if(is_final_element() && !is_unknown() && a_value == 0){formula->clone(this);}
		else if(is_final_element() && formula->is_final_element() && !is_unknown() && !formula->is_unknown()){
			a_value += reinterpret_cast<Formula_Base*>(formula)->a_value;
		}
		else {
			if(algebra_operator_name() != std::string("+")){sub_place();set_algebra_operator(std::string("+"));}
			algebra_elements().push_back(formula->algebra_clone());
		}
	}

	// Creates the unknown
    virtual Algebra_Element::__Algebra_Unknown* create_unknown(){clear();a_unknown = std::make_shared<Formula_Unknown>();return a_unknown.get();};

	// Operates this element with another one
	virtual void operate(Algebra_Element* other, std::string operation) {
		if(!a_modified){a_modified = true;reinterpret_cast<Formula_Base*>(other)->algebra_clone(this);}
		else if(operation == std::string("+")){add(reinterpret_cast<Formula_Base*>(other));}
		else if(operation == std::string("*")){multiply(reinterpret_cast<Formula_Base*>(other));}
		else if(is_final_element() && other->is_final_element() && !is_unknown() && !other->is_unknown()){
			a_value += reinterpret_cast<Formula_Base*>(other)->a_value;
		}
		else {
			std::shared_ptr<Formula_Base> current = clone();
			sub_place();set_algebra_operator(operation);
			algebra_elements().push_back(other->algebra_clone());
		}
	};

	// Available operators for this object
	virtual const std::vector<Algebra_Operator>& operators(){return formula_operators;}

	// Replaces the unknowns
	virtual void replace_unknowns_algebra(Algebra_Element* element, Unknowns_Container* values) const {
		// The element is final
		if(is_final_element()) {
			if(is_unknown()){
				Formula_Unknown* current = reinterpret_cast<Formula_Unknown*>(values->unknown_by_name(algebra_unknown()->name));
				reinterpret_cast<Formula_Base*>(element)->a_value = current->value;
			}
			else {reinterpret_cast<Formula_Base*>(element)->a_value = a_value;}
		}
		else {Algebra_Element::replace_unknowns_algebra(element, values); }

		// Modified
		reinterpret_cast<Formula_Base*>(element)->a_modified = true;
	}
    std::shared_ptr<Formula_Base> replace_unknowns(Unknowns_Container* values) const {
        // Create the object
        std::shared_ptr<Formula_Base> s = std::make_shared<Formula_Base>();
        s.get()->a_this_object=s;replace_unknowns_algebra(s.get(), values);

        return s;
    }

	// Multiplies an element to this one
	void multiply(Formula_Base* formula){
		if(is_final_element() && !is_unknown() && a_value == 1){formula->clone(this);}
		else if(is_final_element() && formula->is_final_element() && !is_unknown() && !formula->is_unknown()){
			a_value *= reinterpret_cast<Formula_Base*>(formula)->a_value;
		}
		else {
			if(algebra_operator_name() != std::string_view("*")){sub_place();set_algebra_operator(std::string("*"));}
			algebra_elements().push_back(formula->algebra_clone());
		}
	}

	// Simplify the expression
	bool simplify_step() {
		if(algebra_operator_name() == std::string_view("+")) {
			for(std::size_t i = 0;i<algebra_elements().size();i++) {
                // Possible combinations
				for(std::size_t j = 0;j<i;j++) {
					if(algebra_elements().at(j).get()->is_equal_without_value(algebra_elements().at(i).get())) {
						if(algebra_elements().at(i).get()->is_known() && algebra_elements().at(j).get()->is_known()) {
							// Known case
							reinterpret_cast<Formula_Base*>(algebra_elements().at(j).get())->a_value += reinterpret_cast<Formula_Base*>(algebra_elements().at(i).get())->a_value;
							algebra_elements().erase(algebra_elements().begin() + i);i--;
							break;
						}
						else {
							// Operation
							Formula_Base* first_object = reinterpret_cast<Formula_Base*>(algebra_elements().at(j).get()->known_algebra_element());
							Formula_Base* second_object = reinterpret_cast<Formula_Base*>(algebra_elements().at(i).get()->known_algebra_element());
							if(first_object != 0 && second_object != 0) {first_object->a_value += second_object->a_value;}
							else if(second_object != 0){second_object->a_value++;algebra_elements()[j] = algebra_elements()[i];}
							else if(first_object != 0) {first_object->a_value++;}
							else{std::shared_ptr<Algebra_Element>f=new_algebra_element();reinterpret_cast<Formula_Base*>(f.get())->a_value=2;reinterpret_cast<Formula_Base*>(algebra_elements().at(j).get())->multiply(reinterpret_cast<Formula_Base*>(f.get()));}
							algebra_elements().erase(algebra_elements().begin() + i);--i;
							break;
						}
					}
				}
			}
		}
		return true;
	}

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
				if(i < static_cast<int>(algebra_elements_const().size()) - 1){to_return += std::string(" ") + algebra_operator_name() + std::string(" ");}
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
    scls::Unknowns_Container c;c.create_unknown<Formula_Base::Formula_Unknown>("x")->value = scls::Complex(-4);
	std::shared_ptr<Formula_Base> f = scls::string_to_algebra_element<Formula_Base>("(4 * x + 8 + 2 * x * x)");
	std::cout << f.get()->replace_unknowns(&c).get()->to_std_string(0) << std::endl;

	return 0;
}
