#include "shop.h"

std::vector<std::string> sidebar_options;

Shop::Shop():
	shop_mode(0),
	selected(0),
	width(WINDOW_WIDTH - 40), height(WINDOW_HEIGHT - 20),
	sidebar_width(width / 5), sidebar_height(height),
	sidebar_options({"Comprar", "Vender", "Upgrades", "Voltar"}) {}
