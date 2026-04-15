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

void feed_fishes(std::vector<Fish>& fishes, int fish_amnt, float* total_money) {
	float feed_price = 1.5f;

	// alimenta de acordo com o peixe em ordem de chegada, feature o.o
	for (int f = 0; f < fish_amnt; f++) {
		Fish& fish = fishes[f];
		if (*total_money >= feed_price && fish.curnt_health < fish.fish_health) {
			*total_money -= feed_price;
			fish.curnt_health += fish.fish_health / 2;
		}
	}
}