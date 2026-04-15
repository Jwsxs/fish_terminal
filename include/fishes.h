#include "conf.h"

extern std::vector<std::string> CHARS;

#ifndef FISHES_H
#define FISHES_H


struct Fish {
	std::string fish_char;
	int color;
	int x, y;
    int width, height;

	int fish_health;
	int curnt_health;
	int lose_health_cooldown = 60;

	int target_cooldown = 10;
	int target_x, target_y;

	int money_cooldown;

	Fish(int w, int h):
		fish_char(CHARS[rand() % CHARS.size()]),
		color(90 + rand() % 8),
        	x(WINDOW_WIDTH / 2), y(WINDOW_HEIGHT / 2),
        	width(w), height(h),
		fish_health(w * 5 + h * 2),
		curnt_health(fish_health),
        	//target_x(x), target_y(y),
		money_cooldown(rand() % 500) {}

	static Fish create(int w, int h) {
		return Fish(w, h);
	}

	void fishLoseHealth() {
		if (curnt_health > 0 && lose_health_cooldown <= 0) {
			curnt_health--;
		}
		if (curnt_health > fish_health) {
			curnt_health = fish_health;
		}

		if (lose_health_cooldown <= 0) {
			lose_health_cooldown = rand() % 90 + 30;
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

		if (x < width) x = width;
		else if (x > WINDOW_WIDTH - 1 - width) x = WINDOW_WIDTH - width;
		else if (y < height) y = height;
		else if (y > WINDOW_HEIGHT - 1 - height) y = WINDOW_HEIGHT - height;

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

void feed_fishes(std::vector<Fish>& fishes, int fish_amnt, float* total_money);

#endif