#include "../include/output.h"
#include "../include/config.h"
#include <iostream>

std::ofstream out;
char var[N_VAR];

void initialize_output() {
    // ИЗ_ЗА НЕПРАВИЛЬНОГО ПУТИ Я ПОТРАТИЛ 1.5 ЧАСА НА ДЕБАГ ДАЖЕ КОМИТ ОТКАТИЛ
    out.open("./results/solutions.txt");
}

void cleanup_output() {
    out.close();
}

void print_solution() {
    for (unsigned i = 0; i < N; i++) {
        out << "Object " << i << ": ";
        for (unsigned j = 0; j < M; j++) {
            unsigned J = i * M * LOG_N + j * LOG_N;
            unsigned num = 0;
            
            if (J + LOG_N > N_VAR) {
                continue;
            }
            
            for (unsigned k = 0; k < LOG_N; k++) {
                num |= (var[J + k] << k);
            }
            out << num << " ";
        }
        out << "\n";
    }
    out << std::endl;
}
