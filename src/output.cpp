#include "../include/output.h"
#include "../include/config.h"
#include <iostream>

std::ofstream out;
char var[N_VAR];

void initialize_output() {
    out.open("../results/solutions.txt");
}

void cleanup_output() {
    out.close();
}

void print_solution() {
    // Реализация вывода решения
}