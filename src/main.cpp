#include <algorithm>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <thread>

//set config file for some global vars
#include "conf.h"

#include "process_keyboard.h"
#include "fishes.h"
#include "menu.h"
#include "shop.h"
#include "render.h"

std::string frame_buffer[WINDOW_HEIGHT][WINDOW_WIDTH];

int main() {
	Menu menu;
	Shop shop;

	std::random_device dev;
	std::mt19937 rng(dev());
	
	float total_money = 0.0f;

	int max_fishes = 10;
	int fish_spawn_cooldown = 10;

	std::vector<Fish> fishes;

	int running = 1;

	while (running) {
		//nas versões passadas fazia diferente:
		/*
		-- analisava cada pixel, e ai pintava de acordo com o que estava no local
		-- por mais que pareca desgastante o mais otimizado é renderização de cada camada por si só
		*/

		clear_framebuffer();
		draw_background();

		if ((int)fishes.size() < max_fishes) {
			if (fish_spawn_cooldown <= 0) {
				Fish f = Fish::create(1, 1);
				f.fish_health = std::uniform_int_distribution<std::mt19937::result_type>(0, 35)(rng);
				
				int r = std::uniform_int_distribution<std::mt19937::result_type>(1, 100)(rng);
				if (r == 100) f.rarity = 4; // ciano
				else if (r > 95) f.rarity = 3; // roxo
				else if (r > 80) f.rarity = 2; // amarelo
				else if (r > 65) f.rarity = 1; // verde
				else f.rarity = 0; // vermelho

				std::uniform_int_distribution<std::mt19937::result_type> rw(2, 14);
				f.width = rw(rng);

				std::uniform_int_distribution<std::mt19937::result_type> rh(2, 6);
				f.height = rh(rng);

				int color = 101 + f.rarity;
				f.color = (color >= 104) ? color + 1 : color;

				fishes.push_back(f);

				fish_spawn_cooldown = (rand() % 1) * 10; // entre 1 e 100 e aí 100 -> 100ms e 10000ms
			}
			fish_spawn_cooldown--;
		}

		draw_fishes(fishes);

		for (auto& f: fishes) {
			f.fishLoseHealth();
			draw_text(f.x, f.y + 3, (f.curnt_health > f. fish_health / 2) ? "\033[1;92m" : "\033[1;91m", std::to_string(f.curnt_health) + " hp");

			f.processMovement();

			f.money_cooldown--;
			if (f.money_cooldown <= 0) {
				f.last_money_given = f.giveMoney();
				total_money += f.last_money_given;
			}

			if (f.money_text) {
				draw_text(f.tx, f.ty - f.temp_text, "\033[1;92m", std::format("${:.2f}", f.last_money_given)); //std::format("${:.2f}", money_given));

				f.money_text_cooldown--;
				f.temp_text+=.25;

				if (f.money_text_cooldown <= 0) {
					f.money_text_cooldown = 30;
					f.money_text = 0;
				}
			}
		}

		//std::cout << "{total_money} \033[93m" << std::setw(2) << total_money << "\033[0m\n";
		draw_text(5, 2, "\033[1;97m", std::format("${:.2f}", total_money));
		fishes.erase(std::remove_if(fishes.begin(), fishes.end(), [] (const Fish& f) { return f.curnt_health <= 0; }), fishes.end());

		if (keyb_hit()) {
			char c = getchar();

			if ((c == 'M' || c == 'm') && !shop.shop_mode) { menu.selected = 0; menu.menu_mode = !menu.menu_mode; };
			if ((c == 'L' || c == 'l') && !menu.menu_mode) { shop.selected = 0; shop.shop_mode = !shop.shop_mode; };

			if (menu.menu_mode) {
				switch (c) {
					case 'S':
					case 's':
						menu.selected++;
					break;

					case 'W':
					case 'w':
						menu.selected--;
					break;

					case ' ':
						switch (menu.selected) {
							case 0:
								menu.menu_mode = !menu.menu_mode;
							break;
							case 1: //Configs
							case 2: //Sobre
							break;
							case 3:
								running = 0;
							break;
						}
					break;
				}
			} else if (shop.shop_mode) {
				switch (c) {
					case 'S':
					case 's':
						shop.selected++;
					break;

					case 'W':
					case 'w':
						shop.selected--;
					break;

					case ' ':
						switch(shop.selected) {
							case 0: // Comprar
							break;
							case 1: // Vender
							break;
							case 2: // Upgrades
							break;
							case 3: // Voltar
								shop.shop_mode = !shop.shop_mode;
							break;
						}
					break;
				}
			} else {
				switch (c) {
					case ' ':
					feed_fishes(fishes, total_money);
					break;
				}
			}
			if (menu.selected < 0) menu.selected = 0;
			if (menu.selected >= (int)menu.options.size() - 1) menu.selected = menu.options.size() - 1;
		}
		// ai entao printa tudo junto
		//std::cout << frame_buff;

		if (menu.menu_mode) draw_menu(menu);
		if (shop.shop_mode) draw_shop(shop);
		render_framebuffer();
		// usleep(64 * 1000); // -> 15 fps //? POSIX -> LESS SAFE THAN ↓
		std::this_thread::sleep_for(std::chrono::milliseconds(1)); //? MORE PORTABLE -> safer for portability and type safety
		//https://stackoverflow.com/questions/48383565/usleep-vs-stdthis-threadsleep-for-when-write-read-on-a-linux-serial-port
	}

	clear_framebuffer();
	return 0;
}
