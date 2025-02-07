#pragma once

#include <functional>
#include <fstream>

#include "../include/constraints.h"
#include "../include/config.h"

std::ofstream out;
char var[N_VAR];
bdd p[M][N][N];

void solution_handler(char*, int);
void print_solution();
void initialize_bdd();
