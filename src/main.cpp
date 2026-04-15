#include <algorithm>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <thread>

#include "process_keyboard.h"
#include "fishes.h"
#include "menu.h"
#include "conf.h"

//TODO: alem de otimizaar q o fps cai mesmo capado em 15 eu acho nao sei porque

std::string getPixel(Menu* menu, const std::vector<Fish>& fishes, int h, int w) {
	// https://gist.github.com/JBlond/2fea43a3049b38287e5e9cefc87b2124
	// deus abencoe

    if ((h == 0 || h == WINDOW_HEIGHT - 1) &&  (w == 0 || w == WINDOW_WIDTH - 1)) return " ";
	if (h == 0 || h == WINDOW_HEIGHT - 1) return "-";
	if (w == 0 || w == WINDOW_WIDTH - 1) return "|";

	//agora vejo a pos de cada peixe
	for (const auto& fish : fishes) {
		std::string colored_char = ("\033[44m\033[1;" + std::to_string(fish.color) + "m" + fish.fish_char + "\033[0m");

       		if (h >= fish.y - fish.height / 2 && h < fish.y + fish.height / 2 && w >= fish.x - fish.width / 2 && w < fish.x + fish.width / 2) {
			if (menu->menu_mode) {
				// check if fish "sprite" is at border of menu -> without it, fish is getting rendered above the menu
				if ((h > WINDOW_HEIGHT / 2 - menu->height && h < WINDOW_HEIGHT / 2 + menu->height) && (w > WINDOW_WIDTH / 2 - menu->width && w < WINDOW_WIDTH / 2 + menu->width)) return "\033[40m \033[0m";
				else if ((h == WINDOW_HEIGHT / 2 - menu->height || h == WINDOW_HEIGHT / 2 + menu->height) && (w > WINDOW_WIDTH / 2 - menu->width && w < WINDOW_WIDTH / 2 + menu->width)) return "-";
				else if ((w == WINDOW_WIDTH / 2 - menu->width || w == WINDOW_WIDTH / 2 + menu->width) && (h > WINDOW_HEIGHT / 2 - menu->height && h < WINDOW_HEIGHT / 2 + menu->height)) return "|";
				else return colored_char;
			} else {
				return colored_char;
			}
        	}
    	}
	
	//draw menu
	if (menu->menu_mode) {
		//TODO: um menu decente
		if ((h == WINDOW_HEIGHT / 2 - menu->height || h == WINDOW_HEIGHT / 2 + menu->height) && (w == WINDOW_WIDTH / 2 - menu->width || w == WINDOW_WIDTH / 2 + menu->width)) return "\033[40m \033[0m";
		else if ((h == WINDOW_HEIGHT / 2 - menu->height || h == WINDOW_HEIGHT / 2 + menu->height) && (w > WINDOW_WIDTH / 2 - menu->width && w < WINDOW_WIDTH / 2 + menu->width)) return "-";
		else if ((w == WINDOW_WIDTH / 2 - menu->width || w == WINDOW_WIDTH / 2 + menu->width) && (h > WINDOW_HEIGHT / 2 - menu->height && h < WINDOW_HEIGHT / 2 + menu->height)) return "|";
		else if ((w > WINDOW_WIDTH / 2 - menu->width && w < WINDOW_WIDTH / 2 + menu->width) && (h > WINDOW_HEIGHT / 2 - menu->height && h < WINDOW_HEIGHT / 2 + menu->height)) return "\033[100m \033[0m";
	}

	return "\033[44m \033[0m";
}

int main() {
	srand(time(NULL));

	Menu menu;

	float total_money = 0.0f;

	int max_fishes = 10;
	int fish_spawn_cooldown = 10;

    	std::vector<Fish> fishes;

    	int running = 1;

    	while (running) {
			// ao invés de printar cada camada (tela-> peixe-> hud ou qualquer outra),
			// já printa tudo junto, conforme é renderizado por WIDTH e HEIGHT
			// pra isso frame_buffer,
        	std::string frame_buff;

			// que então aumenta a capacidade da string pelo tamanho da tela-> width * height
       		frame_buff.reserve(WINDOW_WIDTH * WINDOW_HEIGHT); //requests the string capacity to a planned size https://cplusplus.com/reference/string/string/reserve/

			//limpa
        	std::cout << "\033[2K\033[H";
        	for (int h = 0; h < WINDOW_HEIGHT; h++) {
            		for (int w = 0; w< WINDOW_WIDTH; w++) {
                		// e adiciona os peixes
						frame_buff += getPixel(&menu, fishes, h, w);
				
						//ou hud ou oq for necessário
						/*
						* que nesse caso poderia ser o menu, mas eu nao consegui, ai ficou pro get_pixel() :sob:
						-- print_hud();
						*/
					}
            		frame_buff += '\n';
        	}

		std::ostringstream oss;
		oss << std::fixed << std::setprecision(2) << total_money;
		frame_buff += "total_money \033[32m" + oss.str() + "\033[0m" + '\n';
		// frame_buff += "fish_amnt " + std::to_string(fishes.size()) + '\n';

		if ((int)fishes.size() < max_fishes) {
			if (fish_spawn_cooldown <= 0) {
				fish_spawn_cooldown = (rand() % 1) * 10;
				fishes.push_back(Fish::create(rand() % 10 + 2, rand() % 4 + 2));
			}
			fish_spawn_cooldown--;
		}

		// frame_buff += "fish_spawn_cooldown " + std::to_string(fish_spawn_cooldown) + '\n';
		
		for (auto& fish: fishes) {
			fish.fishLoseHealth();
			
			// frame_buff += std::to_string(fish.fish_char) + " health: " + std::to_string(fish.curnt_fish_health) + "/" + std::to_string(fish.fish_health) + " -> " + std::to_string(fish.lose_health_cooldown) +'\n';

            fish.processMovement();
			total_money += fish.giveMoney() / 100;
		}
		
		fishes.erase(std::remove_if(fishes.begin(), fishes.end(), [] (const Fish& f) { return f.curnt_health <= 0; }), fishes.end());

        	if (keyb_hit()) {
            		char c = getchar();

            		switch (c) {
						case 'Q':
						case 'q':
							running = 0;
						break;

						case 'M':
						case 'm':
							menu.menu_mode = !menu.menu_mode;
						break;

						case ' ':
							feed_fishes(fishes, fishes.size(), &total_money);
						break;
            		}

				if (menu.menu_mode) {
					switch (c) {
						case 'S':
						case 's':
							menu.selected++;
						break;
					}
				}
			}
			// ai entao printa tudo junto
	        std::cout << frame_buff;

	        // usleep(64 * 1000); // -> 15 fps //? POSIX -> LESS SAFE THAN ↓
	        std::this_thread::sleep_for(std::chrono::milliseconds(64)); //? MORE PORTABLE -> safer for portability and type safety
	        //https://stackoverflow.com/questions/48383565/usleep-vs-stdthis-threadsleep-for-when-write-read-on-a-linux-serial-port
    	}
    	return 0;
}
