#include <algorithm>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <thread>
#include <random>

#include "process_keyboard.h"
#include "fishes.h"
#include "menu.h"
#include "conf.h"

std::string frame_buffer[WINDOW_HEIGHT][WINDOW_WIDTH];

void clear_framebuffer() {
    //system("clear");
    std::cout << "\033[1H";
}

void draw_background() {
    for (int h = 0; h < WINDOW_HEIGHT; h++) {
        for (int w = 0; w < WINDOW_WIDTH; w++) {
            if (h == 0 || h == WINDOW_HEIGHT - 1) frame_buffer[h][w] = "-";
            else if (w == 0 || w == WINDOW_WIDTH - 1) frame_buffer[h][w] = "|";
            else frame_buffer[h][w] = "\033[104m \033[0m";
        }
    }
}

void draw_fishes(std::vector<Fish>& fishes) {
    for (int h = 0; h < WINDOW_HEIGHT; h++) {
        for (int w = 0; w < WINDOW_WIDTH; w++) {
            for (const auto& f: fishes) {
                if (((f.x >= w - f.width / 2) && (f.x <= w + f.width / 2)) && ((f.y >= h - f.height / 2) && (f.y <= h + f.height / 2))) frame_buffer[h][w] = "\033[" + std::to_string(f.color) + "m \033[0m";// f.fish_char;
            }
        }
    }
}

void draw_menu(Menu& menu) {
    int i = 0;
    for (int h = 0; h < WINDOW_HEIGHT; h++) {
        for (int w = 0; w < WINDOW_WIDTH; w++) {
            if ((h == WINDOW_HEIGHT / 2 - menu.height || h == WINDOW_HEIGHT / 2 + menu.height - 1) && (w >= WINDOW_WIDTH / 2 - menu.width && w <= WINDOW_WIDTH / 2 + menu.width)) frame_buffer[h][w] = "-";
            else if ((w == WINDOW_WIDTH / 2 - menu.width || w == WINDOW_WIDTH / 2 + menu.width) && (h > WINDOW_HEIGHT / 2 - menu.height && h < WINDOW_HEIGHT / 2 + menu.height)) frame_buffer[h][w] = "|";
            else if (w > WINDOW_WIDTH / 2 - menu.width && w < WINDOW_WIDTH / 2 + menu.width && h > WINDOW_HEIGHT / 2 - menu.height && h < WINDOW_HEIGHT / 2 + menu.height) frame_buffer[h][w] = " ";
        }
    }
    for (const auto& opt: menu.options) {
        for (int c = 0; c < (int)opt.size(); c++) {
            frame_buffer[WINDOW_HEIGHT / 2 - menu.height / 2 + i][WINDOW_WIDTH / 2 - opt.size() / 2 + c] = opt[c];
        }

        if (i == menu.selected) {
            frame_buffer[WINDOW_HEIGHT / 2 - menu.height / 2 + i][WINDOW_WIDTH / 2 - opt.size() / 2 - 1] = ">";
        } else {
            frame_buffer[WINDOW_HEIGHT / 2 - menu.height / 2 + i][WINDOW_WIDTH / 2 - opt.size() / 2 - 1] = " ";
        }

        i++;
    }
/*
    for (int h = 0; h < WINDOW_HEIGHT; h++) {
        for (int w = 0; w < WINDOW_WIDTH; w++) {
            int i = 0;
            for (const auto& opt: menu.options) {
                frame_buffer[WINDOW_HEIGHT / 2 + i][WINDOW_WIDTH / 2 - opt.size() / 2] = opt;
                i++;
            }
        }
    }
*/
}

void render_framebuffer() {
    for (int h = 0; h < WINDOW_HEIGHT; h++) {
        for (int w = 0; w < WINDOW_WIDTH; w++) {
            std::cout << frame_buffer[h][w];
        }
        std::cout << '\n';
    }
}

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

	    //draw_hud();
    	/* O QUE EU PRECISO:
    	 * DRAW_MENU(); // -> SE TIVER ON
       	 */

       	if (menu.menu_mode) draw_menu(menu);

    	render_framebuffer();

    	if ((int)fishes.size() < max_fishes) {
    		if (fish_spawn_cooldown <= 0) {
    			fish_spawn_cooldown = (rand() % 1) * 10;
    			std::uniform_int_distribution<std::mt19937::result_type> rw(2, 10);
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

    	fishes.erase(std::remove_if(fishes.begin(), fishes.end(), [] (const Fish& f) { return f.curnt_health <= 0; }), fishes.end());

    	if (keyb_hit()) {
       		char c = getchar();
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
			} else {
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
