#include "menu.h"

std::vector<std::string> options;

Menu::Menu():
    selected(0),
    options({"Voltar", "Configs", "Sobre", "Sair"}),
    width(10), height(options.size()) {}
