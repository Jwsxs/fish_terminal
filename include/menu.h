#include "conf.h"

#ifndef MENU_H
#define MENU_H

struct Menu {
	bool menu_mode;
	int selected;

    std::vector<std::string> options;

	int width, height;

	Menu();
};

#endif
