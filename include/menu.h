#include "conf.h"

#ifndef MENU_H
#define MENU_H

struct Menu {
	bool menu_mode;
	int selected;

    std::vector<std::string> options;

	int width, height;

	Menu():
		menu_mode(false), selected(0),
		options({"Lorem", "Ipsum", "Dolor"}),
		width(WINDOW_WIDTH / 10), height(WINDOW_HEIGHT / 4) {}
};

#endif
