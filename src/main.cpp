#include <iostream>
<<<<<<< HEAD:src/main.cpp
#include "process_keyboard.h"
#include <vector>
=======
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <vector>
#include <thread>
>>>>>>> 80cc8489771bc31a688089386036a79daa0edac5:main.cpp
#include <chrono>
#include <thread>

// Windows default terminal size
#define WINDOW_HEIGHT 45
#define WINDOW_WIDTH 80

struct Fish {
    int x, y;
    int width, height;

    int target_cooldown;
    int target_x, target_y;

    Fish(int w, int h):
        x(WINDOW_WIDTH / 2), y(WINDOW_HEIGHT / 2),
        width(w), height(h),
        target_cooldown(0),
        target_x(x), target_y(y) {}

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
};

char get_pixel(const Fish* fishes, int amnt, int h, int w) {
    if ((h == 0 || h == WINDOW_HEIGHT - 1) &&  (w == 0 || w == WINDOW_WIDTH - 1)) return ' ';
    if (h == 0 || h == WINDOW_HEIGHT - 1) return '-';
    if (w == 0 || w == WINDOW_WIDTH - 1) return '|';

    //agora vejo a pos de cada peixe
    for (int f = 0; f < amnt; f++) {
        const Fish& fish = fishes[f];

        if (h >= fish.y - fish.height / 2 && h < fish.y + fish.height / 2 && w >= fish.x - fish.width / 2 && w < fish.x + fish.width / 2) {
            return 'x';
        }
    }
    return ' ';
}

int main() {
    std::vector<Fish> fishes = {
        Fish::create(4, 2),
        Fish::create(6, 3),
        Fish::create(10, 2),
    };

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
		// ai entao printa tudo junto
        std::cout << frame_buff;

        for (int f = 0; f < fishes.size(); f++) {
            fishes[f].processMovement();
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

        // usleep(64 * 1000); // -> 15 fps //? POSIX -> LESS SAFE THAN ↓
        std::this_thread::sleep_for(std::chrono::milliseconds(64)); //? MORE PORTABLE -> safer for portability and type safety
        //https://stackoverflow.com/questions/48383565/usleep-vs-stdthis-threadsleep-for-when-write-read-on-a-linux-serial-port
    }

    return 0;
}
