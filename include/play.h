#pragma once
#include <random>
#include <vector>
#include "engine.h"

void print_screen(const std::vector<std::vector<int>>& screen);
void print_line_result(const LineResult& result);
void print_screen_evaluation(const ScreenEvaluation& result);
void play_game(const std::vector<std::vector<int>>& reels, std::mt19937& gen);