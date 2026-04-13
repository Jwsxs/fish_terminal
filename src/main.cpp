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

struct Game {
};

struct Fish {
	char fish_char;
    int x, y;
    int width, height;


    int target_cooldown;
    int target_x, target_y;

	int money_cooldown;
	//float money_amnt;
	int last_fish;
    
	Fish(int w, int h):
		fish_char(CHARS[rand() % CHARS.size()]),
        x(WINDOW_WIDTH / 2), y(WINDOW_HEIGHT / 2),
        width(w), height(h),
        target_cooldown(0),
        target_x(x), target_y(y),
		money_cooldown(rand() % 100),
		last_fish(0) {}

    static Fish create(int w, int h) {
        return Fish(w, h);
    }

    void processMovement() {
        if (target_cooldown <= 0) {
            target_cooldown = rand() % 15;
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

char get_pixel(const Fish* fishes, int amnt, int h, int w) {
    if ((h == 0 || h == WINDOW_HEIGHT - 1) &&  (w == 0 || w == WINDOW_WIDTH - 1)) return ' ';
    if (h == 0 || h == WINDOW_HEIGHT - 1) return '-';
    if (w == 0 || w == WINDOW_WIDTH - 1) return '|';

    //agora vejo a pos de cada peixe
    for (int f = 0; f < amnt; f++) {
        const Fish& fish = fishes[f];

        if (h >= fish.y - fish.height / 2 && h < fish.y + fish.height / 2 && w >= fish.x - fish.width / 2 && w < fish.x + fish.width / 2) {
            return fish.fish_char;
        }
    }
    return ' ';
}

int main() {
	srand(time(NULL));

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
				frame_buff += get_pixel(fishes.data(), fishes.size(), h, w);
            	
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

		if (fishes.size() < max_fishes) {
			if (fish_spawn_cooldown <= 0) {
				fish_spawn_cooldown = (rand() % 100);
				fishes.push_back(Fish::create(rand() % 8 + rand() % 3 + 2, rand() % 3 + 2));
			}
			fish_spawn_cooldown--;
		}

		frame_buff += "fish_spawn_cooldown " + std::to_string(fish_spawn_cooldown) + '\n';

        for (int f = 0; f < fishes.size(); f++) {
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
