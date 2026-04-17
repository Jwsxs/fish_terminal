#include <algorithm>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <thread>
#include <random>

//set config file for some global vars
#include "conf.h"

#include "process_keyboard.h"
#include "fishes.h"
#include "menu.h"
#include "render.h"

std::string frame_buffer[WINDOW_HEIGHT][WINDOW_WIDTH];

int main() {
    std::random_device dev;
    std::mt19937 rng(dev());
	Menu menu;

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
	    draw_fishes(fishes);

       	if (menu.menu_mode) draw_menu(menu);

    	render_framebuffer();

    	if ((int)fishes.size() < max_fishes) {
    		if (fish_spawn_cooldown <= 0) {
    			fish_spawn_cooldown = (rand() % 1) * 10;
    			std::uniform_int_distribution<std::mt19937::result_type> rw(2, 14);
    			std::uniform_int_distribution<std::mt19937::result_type> rh(2, 6);
    			fishes.push_back(Fish::create(rw(rng), rh(rng)));
    		}
    		fish_spawn_cooldown--;
    	}

    	for (auto& fish: fishes) {
    		fish.fishLoseHealth();

    		fish.processMovement();
    		total_money += fish.giveMoney() / 100;
    	}

    	std::cout << "{total_money} \033[93m" << std::setw(2) << total_money << "\033[0m\n";

    	fishes.erase(std::remove_if(fishes.begin(), fishes.end(), [] (const Fish& f) { return f.curnt_health <= 0; }), fishes.end());

    	if (keyb_hit()) {
       		char c = getchar();
       		if (menu.menu_mode) {
    			switch (c) {
    			    case 'M':
    			    case 'm':
    			        menu.menu_mode = !menu.menu_mode;
    			    break;

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
			} else {
			    switch (c) {
        			case 'M':
        			case 'm':
        			    menu.menu_mode = !menu.menu_mode;
        			break;

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

        // usleep(64 * 1000); // -> 15 fps //? POSIX -> LESS SAFE THAN ↓
        std::this_thread::sleep_for(std::chrono::milliseconds(64)); //? MORE PORTABLE -> safer for portability and type safety
        //https://stackoverflow.com/questions/48383565/usleep-vs-stdthis-threadsleep-for-when-write-read-on-a-linux-serial-port
    }
	return 0;
}
