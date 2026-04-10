#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

// Windows default terminal size
#define WINDOW_HEIGHT 28
#define WINDOW_WIDTH 120

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

    Fish(int w, int h): x(0), y(0), width(w), height(h) {}

    static Fish create(int w, int h) {
        return Fish(w, h);
    }

    static void draw(Fish fish) {
        for (int h = fish.y - fish.height / 2; h < fish.y + fish.height / 2; h++) {
            for (int w = fish.x - fish.width / 2; w < fish.x + fish.width / 2; w++) {
                printf("x");
            }
            printf("\n");
        }
    }
};

int main() {
    Fish test = Fish::create(5, 2);
    test.x = 10;
    test.y = 2;

    int running = 1;
    while (running) {
        std::cout << "\033[" << WINDOW_HEIGHT << "A";
        for (int h = 0; h < WINDOW_HEIGHT; h++) {
            for (int w = 0; w < WINDOW_WIDTH; w++) {
                int fish_target_y = rand() % WINDOW_HEIGHT;
                int fish_target_x = rand() % WINDOW_WIDTH;

                /*
                --- draw stuff on screen

                */

                if ((h >= test.y - test.height / 2 && h < test.y + test.height / 2) && (w >= test.x - test.width / 2 && w < test.x + test.width / 2)) {
                    printf("x");
                } else {
                    printf(" ");
                }
            }
            printf("\n");
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
    }

    return 0;
}