#include "conf.h"

#include "fishes.h"
#include "menu.h"
#include "shop.h"
#include "text.h"

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

void draw_shop(Shop& shop) {
	for (int h = 0; h < WINDOW_HEIGHT; h++) {
		for (int w = 0; w < WINDOW_WIDTH; w++) {
			//shopping area
			if ((h == WINDOW_HEIGHT / 2 - shop.height / 2 || h == WINDOW_HEIGHT / 2 + shop.height / 2) && (w >= WINDOW_WIDTH / 2 - shop.width / 2 && w <= WINDOW_WIDTH / 2 + shop.width / 2)) frame_buffer[h][w] = "-";
			else if ((w == WINDOW_WIDTH / 2 - shop.width / 2|| w == WINDOW_WIDTH / 2 + shop.width / 2) && (h > WINDOW_HEIGHT / 2 - shop.height / 2 && h < WINDOW_HEIGHT / 2 + shop.height / 2)) frame_buffer[h][w] = "|";
			else if (w > WINDOW_WIDTH / 2 - shop.width / 2 && w < WINDOW_WIDTH / 2 + shop.width / 2 && h > WINDOW_HEIGHT / 2 - shop.height / 2 && h < WINDOW_HEIGHT / 2 + shop.height / 2) frame_buffer[h][w] = " ";

			//sidebar
			if ((w == WINDOW_WIDTH / 2 - shop.sidebar_width) && (h > WINDOW_HEIGHT / 2 - shop.height / 2 + 1 && h < WINDOW_HEIGHT / 2 + shop.height / 2 - 1)) frame_buffer[h][w] = "|";
		}
	}

	int i = 0, temp = 0;
	for (const auto& sb_opt: shop.sidebar_options) {
		for (int c = 0; c < (int)sb_opt.size(); c++) {
			frame_buffer[WINDOW_HEIGHT / 2 - shop.height / 2 + temp + 3][WINDOW_WIDTH / 2 - shop.width / 2 + shop.sidebar_width - 3 - sb_opt.size() / 2 + c] = sb_opt[c];
		}

		if (i == shop.selected) {
			frame_buffer[WINDOW_HEIGHT / 2 - shop.height / 2 + temp + 3][WINDOW_WIDTH / 2 - shop.width / 2 + shop.sidebar_width - 3 - sb_opt.size() / 2 - 1] = ">";
		} else {
			frame_buffer[WINDOW_HEIGHT / 2 - shop.height / 2 + temp + 3][WINDOW_WIDTH / 2 - shop.width / 2 + shop.sidebar_width - 3 - sb_opt.size() / 2 - 1] = " ";
		}
		temp += shop.sidebar_height / ((int)shop.sidebar_options.size());
		i++;
	}
}

void draw_text(int x, int y, std::string color, std::string text) {
	for (int h = 0; h < WINDOW_HEIGHT; h++) {
		int c = 0; // get current character
		for (int w = 0; w < WINDOW_WIDTH; w++) {
			if ((h == y) && ((w >= x) && (w < x + (int)text.size()))) {
				frame_buffer[h][w] = color + text[c] + "\033[0m";
				c++;
			} else {
				continue;
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
