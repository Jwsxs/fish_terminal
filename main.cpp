#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <cstdint>
#include <iomanip>
#include <cmath>
#include <vector>
#include <sys/ioctl.h>

// Windows default terminal size
#define WINDOW_HEIGHT 45
#define WINDOW_WIDTH 80

bool keyb_hit() {
    termios oldt;

    tcgetattr(STDIN_FILENO, &oldt);
    termios newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    int ch = getchar();
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    
    if (ch != EOF) {
        ungetc(ch, stdin);
        return true;
    }

    return false;
}

struct Fish {
    int x, y;
    int width, height;

    Fish(int w, int h): x(WINDOW_WIDTH / 2), y(WINDOW_HEIGHT / 2), width(w), height(h) {}

    static Fish create(int w, int h) {
        return Fish(w, h);
    }

	static void moveToTarget(Fish fish, int* target) {
		fish.x = *target;
		fish.y = *(target + 1);
	}
};

void draw_screen(Fish fish, int h, int w) {
    if ((h == 0 || h == WINDOW_HEIGHT - 1) && (w == 0 || w == WINDOW_WIDTH - 1)) {
        printf("  ");
    } else if (h == 0 || h == WINDOW_HEIGHT - 1) {
        printf("—");
    } else if (w == 0 || w == WINDOW_WIDTH - 1) {
        printf("〡");
    } else if ((h >= fish.y - fish.height / 2 && h < fish.y + fish.height / 2) && (w >= fish.x - fish.width / 2 && w < fish.x + fish.width / 2)) {
        printf("x");
    } else {
        printf(" ");
    }
}

int main() {
    Fish first_fish = Fish::create(5, 2);
    first_fish.x = 10;
    first_fish.y = 2;

    Fish fishes[1] = {
        first_fish,
    };

    int running = 1;
    
    int set_target_cooldown = 0;
    while (running) {
        std::cout << "\033[" << WINDOW_HEIGHT + 3 << "A";
        for (int h = 0; h < WINDOW_HEIGHT; h++) {
            for (int w = 0; w < WINDOW_WIDTH; w++) {
                
                /*
                --- draw stuff on screen
                */
                draw_screen(first_fish, h, w);
            }
            printf("\n");
        }

        std::cout << "\rcooldown: " << std::setw(2) << set_target_cooldown << "\n";
        std::cout << "\rfish_pos: " << std::setw(2) << first_fish.x << "x " << std::setw(2) << first_fish.y << "y\n";
        if (rand() % 2 == 0 && set_target_cooldown <= 0) {
            set_target_cooldown = 10 * 1000;
			int* target;
            *(target) = rand() % WINDOW_WIDTH;
            *(target+1) = rand() % WINDOW_HEIGHT;
			
			Fish::moveToTarget(first_fish, target);
            //first_fish.x += (fish_target_x != WINDOW_WIDTH) ? (fish_target_x - first_fish.x) : WINDOW_WIDTH;
            //first_fish.y += (fish_target_y != WINDOW_HEIGHT) ? (fish_target_y - first_fish.y) : WINDOW_HEIGHT;
	    	
			std::cout << "\rfish target: " << std::setw(2) << target[0] << "x " << std::setw(2) << target[1] << "y\n";
        }
        std::cout << std::flush;
        set_target_cooldown--;
        
        if (keyb_hit()) {
            char c = getchar();
            
            switch (c) {
                case 'Q':
                case 'q':
                    running = 0;
                break;
            }
        }
    }

    return 0;
}
