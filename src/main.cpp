#include <algorithm>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <chrono>
#include <thread>

#include "process_keyboard.h"
// #include "fishes.h"

// Windows default terminal size
#define WINDOW_HEIGHT 45
#define WINDOW_WIDTH 120

std::vector<char> CHARS = {
	'x',
	'o',
	'a',
	'e',
	'c',
	'0',
	'8',
};

struct Menu {
	bool menu_mode;
	int selected;

	int width, height;

	Menu():
		menu_mode(false),
		width(WINDOW_WIDTH / 3), height(WINDOW_HEIGHT / 3) {}
};

struct Fish {
	char fish_char;
    int x, y;
    int width, height;

	int fish_health;
	int curnt_fish_health;
	int lose_health_cooldown = 60;

    int target_cooldown = 10;
    int target_x, target_y;

	int money_cooldown;

	Fish(int w, int h):
		fish_char(CHARS[rand() % CHARS.size()]),
        x(WINDOW_WIDTH / 2), y(WINDOW_HEIGHT / 2),
        width(w), height(h),
		fish_health(w * 5 + h * 2),
		curnt_fish_health(fish_health),
        target_x(x), target_y(y),
		money_cooldown(rand() % 500) {}

    static Fish create(int w, int h) {
        return Fish(w, h);
    }

	void fishHealth() {
		if (curnt_fish_health > 0 && lose_health_cooldown <= 0) {
			curnt_fish_health--;
		}
		if (curnt_fish_health > fish_health) {
			curnt_fish_health = fish_health;
		}

		if (lose_health_cooldown <= 0) {
			lose_health_cooldown = 60;
		}
		lose_health_cooldown--;
	}

    void processMovement() {
        if (target_cooldown <= 0) {
            target_cooldown = rand() % 90;
			target_x = rand() % WINDOW_WIDTH;
            target_y = rand() % WINDOW_HEIGHT;
        }

        if (x < target_x) x++;
        else if (x > target_x) x--;

        if (y < target_y) y++;
        else if (y > target_y) y--;

        target_cooldown--;
    }

	float giveMoney() {
		float money_amnt = 0.0f;
		if (money_cooldown <= 0) {
			money_cooldown = rand() % 100;
			money_amnt = rand() % 100;
		}

		money_cooldown--;

		return money_amnt;
	}
};

char get_pixel(Menu* menu, const std::vector<Fish>& fishes, int amnt, int h, int w) {
    //agora vejo a pos de cada peixe
	if ((h == 0 || h == WINDOW_HEIGHT - 1) &&  (w == 0 || w == WINDOW_WIDTH - 1)) return ' ';
	if (h == 0 || h == WINDOW_HEIGHT - 1) return '-';
	if (w == 0 || w == WINDOW_WIDTH - 1) return '|';

	for (const Fish& fish : fishes) {
        if (h >= fish.y - fish.height / 2 && h < fish.y + fish.height / 2 && w >= fish.x - fish.width / 2 && w < fish.x + fish.width / 2) {
            return fish.fish_char;
        }
    }

	if (menu->menu_mode) {
		if ((h == WINDOW_HEIGHT / 2 - menu->height || h == WINDOW_HEIGHT / 2 + menu->height) &&  (w == WINDOW_WIDTH / 2 - menu->width || w == WINDOW_WIDTH / 2 + menu->width)) return ' ';
		if (h == WINDOW_HEIGHT / 2 - menu->height || h == WINDOW_HEIGHT / 2 + menu->height) return '-';
		if (w == WINDOW_WIDTH / 2 - menu->width || w == WINDOW_WIDTH / 2 + menu->width) return '|';
	}

    return ' ';
}

void feed_fishes(std::vector<Fish>& fishes, int fish_amnt, float* total_money) {
	float feed_price = 1.5f;

	for (int f = 0; f < fish_amnt; f++) {
		Fish& fish = fishes[f];
		if (*total_money >= feed_price) {
			*total_money -= feed_price;
			fish.curnt_fish_health += fish.fish_health / 2;
		}
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
		// ao invés de printar cada camada (tela-> peixe-> hud ou qualquer outra),
		// já printa tudo junto, conforme é renderizado o WIDTH e HEIGHT
		// pra isso frame_buffer,
        std::string frame_buff;

		// que então aumenta a capacidade da string pelo tamanho da tela-> width * height
        frame_buff.reserve(WINDOW_WIDTH * WINDOW_HEIGHT); //requests the string capacity to a planned size https://cplusplus.com/reference/string/string/reserve/

		//limpa
        std::cout << "\033[2K\033[H";
        for (int h = 0; h < WINDOW_HEIGHT; h++) {
            for (int w = 0; w< WINDOW_WIDTH; w++) {
                // e adiciona os peixes
				frame_buff += get_pixel(&menu, fishes, fishes.size(), h, w);

				//ou hud ou oq for necessário
				/*
				-- print_hud();
				*/
			}
            frame_buff += '\n';
        }

		std::ostringstream oss;
		oss << std::fixed << std::setprecision(2) << total_money;
		frame_buff += "total_money " + oss.str() + '\n';
		frame_buff += "fish_amnt " + std::to_string(fishes.size()) + '\n';

		if ((int)fishes.size() < max_fishes) {
			if (fish_spawn_cooldown <= 0) {
				fish_spawn_cooldown = (rand() % 100);
				fishes.push_back(Fish::create(rand() % 10 + 2, rand() % 4 + 2));
			}
			fish_spawn_cooldown--;
		}

		frame_buff += "fish_spawn_cooldown " + std::to_string(fish_spawn_cooldown) + '\n';

		for (int f = 0; f < (int)fishes.size(); f++) {
			fishes[f].fishHealth();

			frame_buff += "fish " + std::to_string(f) + "'s health: " + std::to_string(fishes[f].curnt_fish_health) + "/" + std::to_string(fishes[f].fish_health) + " -> " + std::to_string(fishes[f].lose_health_cooldown) +'\n';

            fishes[f].processMovement();
			total_money += fishes[f].giveMoney() / 100;
		}

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
        }

		// ai entao printa tudo junto
        std::cout << frame_buff;

        // usleep(64 * 1000); // -> 15 fps //? POSIX -> LESS SAFE THAN ↓
        std::this_thread::sleep_for(std::chrono::milliseconds(64)); //? MORE PORTABLE -> safer for portability and type safety
        //https://stackoverflow.com/questions/48383565/usleep-vs-stdthis-threadsleep-for-when-write-read-on-a-linux-serial-port
    }
    return 0;
}
