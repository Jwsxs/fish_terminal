#include "fishes.h"

std::vector<std::string> CHARS = {
	"x",
	"o",
	"a",
	"e",
	"c",
	"0",
	"8",
};

Fish::Fish(int w, int h):
	fish_char(CHARS[rand() % CHARS.size()]),
	x(WINDOW_WIDTH / 2), y(WINDOW_HEIGHT / 2),
	dx(0), dy(0),
	width(w), height(h),
	fish_health(10),
    curnt_health(fish_health),
   	//target_x(x), target_y(y),
	money_cooldown(rand() % 500) {
	// evita que a cor do peixe seja igual a da agua (104)
	do {
		color = 100 + rand() % 8;
	} while (color == 104);
};

Fish Fish::create(int w, int h) { return Fish(w, h); }

void Fish::fishLoseHealth() {
	if (curnt_health > 0 && lose_health_cooldown <= 0) {
		curnt_health--;
	}
	if (curnt_health > fish_health) {
		curnt_health = fish_health;
	}

	if (lose_health_cooldown <= 0) {
		lose_health_cooldown = rand() % 90 + 100;
	}
	lose_health_cooldown--;
}

/*
void Fish::processMovement() {
    if (target_cooldown <= 0) {
        target_cooldown = rand() % 90;
        target_x = rand() % WINDOW_WIDTH;
        target_y = rand() % WINDOW_HEIGHT;
    }

    dx = (rand() % 3 - 1) * 0.1;
    dy = (rand() % 3 - 1) * 0.1;

    x += dx;
    y += dy;

    if (x < width) x = width;
    if (x > WINDOW_WIDTH - width) x = WINDOW_WIDTH - width;

    if (y < height) y = height;
    if (y > WINDOW_HEIGHT - height) y = WINDOW_HEIGHT - height;

    //x += dx;
    //y += sin(time * x) * 0.2;
}
*/

void Fish::processMovement() {
	if (target_cooldown <= 0) {
		target_cooldown = rand() % 90;
    	target_x = rand() % WINDOW_WIDTH;
	    target_y = rand() % WINDOW_HEIGHT;
	}

	if (x < target_x) x++;
	else if (x > target_x) x--;

	if (y < target_y) y++;
	else if (y > target_y) y--;

	if (x < width) x = width;
	else if (x > WINDOW_WIDTH - width) x = WINDOW_WIDTH - width;
	else if (y < height) y = height;
	else if (y > WINDOW_HEIGHT - height) y = WINDOW_HEIGHT - height;

	target_cooldown--;
}

float Fish::giveMoney() {
	float money_amnt = 0.0f;
	if (money_cooldown <= 0) {
		money_cooldown = rand() % 100;
		money_amnt = rand() % 100;
	}

	money_cooldown--;

	return money_amnt;
}

void feed_fishes(std::vector<Fish>& fishes, float& total_money) {
	float feed_price = 1.5f;

	// alimenta de acordo com o peixe em ordem de chegada, feature o.o
	for (auto& f: fishes) {
        if (total_money >= feed_price && f.curnt_health < f.fish_health) {
            total_money -= feed_price;
            f.curnt_health = f.fish_health / 2;
        }
	}
}
