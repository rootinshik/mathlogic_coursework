#pragma once
#include <fstream>
#include "config.h"

extern std::ofstream out;
extern char var[N_VAR];

void initialize_output();
void cleanup_output();
void print_solution();