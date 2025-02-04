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
    for (unsigned obj = 0; obj < N; obj++) {
        out << "Object " << obj << ": ";
        
        for (unsigned prop = 0; prop < M; prop++) {
            unsigned start_bit = obj * M * LOG_N + prop * LOG_N;
            unsigned value = 0;
            
            for (unsigned bit = 0; bit < LOG_N; bit++) {
                if (start_bit + bit >= N_VAR) {
                    break;
                }
                value += (var[start_bit + bit] << bit);
            }
            
            out << value << " ";
        }
        out << "\n";
    }
    out << "\n";
}