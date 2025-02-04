#include "../include/main.h"

int main() {
    try {
        initialize_bdd();
        initialize_output();

        bdd task = bddtrue;

        // Добавление всех типов ограничений
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

        // Подсчет количества решений
        std::cout << "Calculating solutions..." << std::endl;
        double satcount = bdd_satcount(task);
        std::cout << "Found " << satcount << " solutions" << std::endl;

        // Вывод информации о решениях
        out << satcount << " solutions:\n" << std::endl;

        // Если решения существуют, выводим их
        if (satcount > 0) {
            std::cout << "Processing solutions..." << std::endl;
            bdd_allsat(task, solution_handler);
            std::cout << "All solutions processed" << std::endl;
        }

        // Завершение работы
        std::cout << "Cleaning up..." << std::endl;
        cleanup_output();
        cleanup_bdd();

        std::cout << "Program completed successfully" << std::endl;
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        cleanup_output();
        cleanup_bdd();
        return 1;
    }
}

// Обработчик решений
void solution_handler(char* varset, int size) {
    static int solution_count = 0;
    solution_count++;

    out << "Solution #" << solution_count << ":\n";

    // Явно указываем тип функции
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