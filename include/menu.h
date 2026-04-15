#include "conf.h"

#ifndef MENU_H
#define MENU_H

extern std::vector<std::string>menu_options;

struct Menu {
	bool menu_mode;
	int selected;

	int width, height;

	Menu():
		menu_mode(false),
		width(WINDOW_WIDTH / 10), height(WINDOW_HEIGHT / 4) {}
};

#endif