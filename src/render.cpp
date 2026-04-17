#include "conf.h"
#include "fishes.h"
#include "menu.h"
#include "render.h"

#include <iostream>

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
}

void render_framebuffer() {
    for (int h = 0; h < WINDOW_HEIGHT; h++) {
        for (int w = 0; w < WINDOW_WIDTH; w++) {
            std::cout << frame_buffer[h][w];
        }
        std::cout << '\n';
    }
}
