#include "conf.h"

#include "item.h"

#ifndef SHOP_H
#define SHOP_H

struct Shop {
	bool shop_mode;
	int selected;

	int width, height;
	int sidebar_width, sidebar_height;

	std::vector<std::string> sidebar_options;

	std::vector<Item> shop_items;
	
	Shop();
};

#endif
