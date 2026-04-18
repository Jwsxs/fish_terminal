#include "text.h"

Text::Text(int x, int y, std::string text):
	x(x), y(y),
	w((int)text.size() + 2), h(3),
	text(text) {}

Text create(int x, int y, std::string text) { return Text(x, y, text); }
