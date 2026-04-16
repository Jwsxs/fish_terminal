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

std::string frame_buffer[WINDOW_HEIGHT][WINDOW_WIDTH];

void clear_framebuffer() {
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

void render_framebuffer() {
    for (int h = 0; h < WINDOW_HEIGHT; h++) {
        for (int w = 0; w < WINDOW_WIDTH; w++) {
            std::cout << frame_buffer[h][w];
        }
        std::cout << '\n';
    }
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

    	render_framebuffer();

    	if ((int)fishes.size() < max_fishes) {
    		if (fish_spawn_cooldown <= 0) {
    			fish_spawn_cooldown = (rand() % 1) * 10;
    			fishes.push_back(Fish::create(rand() % 10 + 2, rand() % 4 + 2));
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
        //std::cout << frame_buff;

        // usleep(64 * 1000); // -> 15 fps //? POSIX -> LESS SAFE THAN ↓
        std::this_thread::sleep_for(std::chrono::milliseconds(64)); //? MORE PORTABLE -> safer for portability and type safety
        //https://stackoverflow.com/questions/48383565/usleep-vs-stdthis-threadsleep-for-when-write-read-on-a-linux-serial-port
    }
	return 0;
}
