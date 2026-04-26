#include "conf.h"

#ifndef FISHES_H
#define FISHES_H

extern std::vector<std::string> CHARS;

class Fish {
public:
	std::string fish_char;
	int rarity;
	int color;
	int x, y;
	float dx, dy;
    int width, height;
	int fish_health;
	int curnt_health;
	int lose_health_cooldown;
	int target_cooldown;
	int target_x, target_y;
	int money_cooldown;
	bool money_text;
	float last_money_given;
	float temp_text;
	int tx, ty;
	int money_text_cooldown;

	Fish(int w, int h);
	static Fish create(int w, int h);
	void fishLoseHealth();
	void processMovement();
	float giveMoney();
};

void feed_fishes(std::vector<Fish>& fishes, float& total_money);

#endif
