#include "conf.h"

#include "item.h"

std::random_device dev;
std::mt19937 rng(dev());

Item::Item(std::string name, float price):
	name(name), price(price), id(std::uniform_int_distribution<std::mt19937::result_type>(0, 255)(rng)) {};