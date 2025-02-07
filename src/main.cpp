#include "../include/main.h"

int main() {
    try {
        initialize_bdd();
        out.open("./results/solutions.txt");

        bdd task = bddtrue;

        std::cout << "Adding type 1 constraints..." << std::endl;
        add_type1_constraints(task);

        std::cout << "Adding type 2 constraints..." << std::endl;
        add_type2_constraints(task);

        std::cout << "Adding type 3 constraints..." << std::endl;
        add_type3_constraints(task);

        std::cout << "Adding type 4 constraints..." << std::endl;
        add_type4_constraints(task);

        std::cout << "Adding general constraints..." << std::endl;
        add_general_constraints(task);

        std::cout << "Calculating solutions..." << std::endl;
        double satcount = bdd_satcount(task);
        std::cout << "Found " << satcount << " solutions" << std::endl;

        out << satcount << " solutions:\n" << std::endl;

        if (satcount > 0) {
            std::cout << "Processing solutions..." << std::endl;
            bdd_allsat(task, solution_handler);
            std::cout << "All solutions processed" << std::endl;
        }

        std::cout << "Cleaning up..." << std::endl;
        out.close();
        bdd_done();

        std::cout << "Program completed successfully" << std::endl;
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        out.close();
        bdd_done();
        return 1;
    }
}

void solution_handler(char* varset, int size) {
    static int solution_count = 0;
    solution_count++;

    out << "Solution #" << solution_count << ":\n";

    std::function<void(char*, unsigned, unsigned)> build_solution = 
    [&](char* varset, unsigned n, unsigned I) {
        if (I == n-1) {
            var[I] = varset[I] >= 0 ? varset[I] : 0;
            print_solution();
            if (varset[I] < 0) {
                var[I] = 1;
                print_solution();
            }
            return;
        }

        if (varset[I] >= 0) {
            var[I] = varset[I];
            build_solution(varset, n, I+1);
        } else {
            var[I] = 0;
            build_solution(varset, n, I+1);
            var[I] = 1;
            build_solution(varset, n, I+1);
        }
    };

    build_solution(varset, size, 0);
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

void initialize_bdd() {
    bdd_init(100000, 10000);
    bdd_setvarnum(N_VAR);
    
    unsigned I = 0;
    for (unsigned i = 0; i < N; i++) {
        for (unsigned j = 0; j < N; j++) {
            for (int z = 0; z < M; z++) {
                p[z][i][j] = bddtrue;
                for (unsigned k = 0; k < LOG_N; k++) { // Инициализируем биты значения конкретного свойства
                    ((j >> k) & 1) ? // Пробегаемся по всем битам
                    p[z][i][j] &= bdd_ithvar(I + LOG_N*z + k) : 
                    p[z][i][j] &= bdd_nithvar(I + LOG_N*z + k);
                }
            }
        }
        I += LOG_N*M; // идем к следующему обьекту
    }
}