#include "conf.h"

#ifndef TEXT_H
#define TEXT_H

struct Text {
	int x, y;
	int w, h;
	std::string text;

	Text(int x, int y, std::string text);
};

#endif
