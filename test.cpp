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

SCLS_INIT;

using namespace scls;

int main() {
	std::shared_ptr<Boolean> b = std::make_shared<Boolean>("a");b.get()->add_and("b");
	std::shared_ptr<Boolean> c = std::make_shared<Boolean>("c");c.get()->add_and("d");
	std::shared_ptr<Boolean> d = b.get()->clone();d.get()->add_or(c.get());
	c = std::make_shared<Boolean>("b");c.get()->add_or("c");c.get()->add_or("d");
	d.get()->add_and(c.get());
	std::cout << "O " << d.get()->to_std_string(0) << std::endl;

	std::string diff = std::string("---------------------------------");
	std::string to_return = std::string("a | b | c | d | ((a . b) + (c . d)) . (b + c + d)\n") + diff + std::string("\n");

	for(int i = 0;i<2;i++) {
        for(int j = 0;j<2;j++) {
            for(int k = 0;k<2;k++) {
                for(int l = 0;l<2;l++) {
                    Unknowns_Container a = Unknowns_Container();
                    a.create_unknown<Boolean_Unknown>("a")->value = (i == 1);
                    a.create_unknown<Boolean_Unknown>("b")->value = (j == 1);
                    a.create_unknown<Boolean_Unknown>("c")->value = (k == 1);
                    a.create_unknown<Boolean_Unknown>("d")->value = (l == 1);

                    to_return += std::to_string(i) + std::string(" | ") + std::to_string(j) + std::string(" | ") + std::to_string(k) + std::string(" | ") + std::to_string(l) + std::string(" |         ") + d.get()->replace_unknowns(&a).get()->to_std_string(0) + std::string("\n") + diff + std::string("\n");
                }
            }
        }
	}

	write_in_file("tests/boole.txt", to_return);
	print("SCLS", to_return);

    return 0;
}
