#include <string>
#include <vector>
#include <cstdint>

#include <random>

std::random_device dev;
std::mt19937 rng(dev());

#ifndef CONF_H
#define CONF_H

#define WINDOW_WIDTH 120
#define WINDOW_HEIGHT 45

extern std::string frame_buffer[WINDOW_HEIGHT][WINDOW_WIDTH];

#endif
