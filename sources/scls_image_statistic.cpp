//******************
//
// scls_image_statistic.cpp
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
// This file contains the source code of scls_image_statistic.h.
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

#include "../scls_image.h"

// The namespace "scls" is used to simplify the all.
namespace scls {
    // Creates an histogram from statistics
    Image histogram(Statistics* statistics) {
        Text_Image_Generator tig;
        double quantity_start = 0;
        double quantity_end = 500;
        double step = 100;

        // Create the quantities
        int quantities_height = 0;
        int quantities_width = 0;
        std::vector<Image> quantities = std::vector<Image>();
        Text_Style style;style.set_font_size(50);
        for(int i = quantity_start;i < quantity_end;i += step) {
            Image current_image = to_image(std::to_string(i), style);
            quantities.push_back(current_image);
            quantities_height += current_image.height();
            if(i < static_cast<int>(quantity_end) - step){quantities_height += 200;}
            if(current_image.width() > quantities_width){quantities_width = current_image.width();}
        }

        // Create the datas
        int datas_height = 0;
        int datas_width = 0;
        std::vector<Image> datas = std::vector<Image>();
        std::vector<std::shared_ptr<Statistics::Datas>> samplings = statistics->samplings();
        for(int i = 0;i < static_cast<int>(samplings.size());i++) {
            Image current_image = to_image(samplings.at(i).get()->data(), style);
            datas.push_back(current_image);
            if(current_image.height() > datas_height){datas_height = current_image.height();}
            datas_width += current_image.width();
            if(i < static_cast<int>(samplings.size()) - 1){datas_width += 10;}
        }

        // Create the image
        int current_x = 0;int current_y = 0;
        int image_height = datas_height + quantities_height;
        int image_width = datas_width + quantities_width;
        Image to_return = Image(image_width, image_height, Color(255, 255, 255));

        // Quantities
        for(int i = 0;i<static_cast<int>(quantities.size());i++) {
            current_x = 0;

            to_return.paste(quantities.at(quantities.size() - (i + 1)), current_x, current_y);

            current_y += quantities.at(i).height() + 200;
        }

        // Datas
        current_x = quantities_width;
        for(int i = 0;i<static_cast<int>(datas.size());i++) {
            current_y = to_return.height() - datas.at(i).height();

            to_return.paste(datas.at(i), current_x, current_y);

            current_x += datas.at(i).width() + 10;
        }

        // Stick
        current_x = quantities_width;
        double graph_height = quantities_height;
        for(int i = 0;i<static_cast<int>(samplings.size());i++) {
            int current_height = ((static_cast<double>(samplings.at(i).get()->size() - quantity_start) / static_cast<double>(quantity_end - quantity_start)) * graph_height);
            int current_width = datas.at(i).width() + 10;

            current_y = to_return.height() - (current_height + datas.at(i).height());
            to_return.fill_rect(current_x, current_y, current_width, current_height, Color(255, 0, 0));
            current_x += current_width;
        }

        return to_return;
    }
}
