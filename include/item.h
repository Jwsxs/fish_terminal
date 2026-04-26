#include "conf.h"

#ifndef ITEMS_H
#define ITEMS_H

class Item {
public:
    std::string name;
    std::string desc;
    float price;

    const uint8_t id; // 2⁸ = 256 IDs totais

    int box_width, box_height;
    int desc_width, desc_height;

    Item(std::string name, float price);
};

#endif