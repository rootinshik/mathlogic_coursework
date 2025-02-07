#include "../include/constraints.h"

const std::vector<int>& left_indices = {3,4,5,6,7,8};
const std::vector<int>& right_indices = {3,4,6,7};
unsigned int left_delta = 3;
unsigned int right_delta = 2;

// Ограничения 1 типа (фиксированные значения)
void add_type1_constraints(bdd& task) {
    // F := F ∧ p(k1, i1, j1)

    // task &= p[1][4][3];
    task &= p[0][6][5];
    // task &= p[1][7][4];
    // task &= p[3][4][4];
    task &= p[2][7][4];
    task &= p[2][5][6];
    task &= p[2][6][0];
    // task &= p[3][7][0];
    task &= p[3][8][7];
    task &= p[1][2][8];
    task &= p[2][4][1];
    task &= p[3][5][1];
    task &= p[0][0][2];
}

// Ограничения 2 типа (эквивалентности)
void add_type2_constraints(bdd& task) {
    auto add_equivalence = [&](int k1, int j1, int k2, int j2) {
        for (int i = 0; i < N; i++) {
            // F := F ∧ (p(k1, i, j1) ↔ p(k2, i, j2))
            task &= !(p[k1][i][j1] ^ p[k2][i][j2]); // not XOR (истинно при совпадении)
        }
    };

    add_equivalence(2, 8, 0, 3);
    add_equivalence(1, 4, 0, 7);
    add_equivalence(2, 3, 3, 3);
    add_equivalence(1, 3, 0, 4);
    add_equivalence(1, 8, 0, 1);
    add_equivalence(1, 5, 2, 7);
    add_equivalence(0, 6, 2, 6);
    add_equivalence(0, 0, 1, 2);
}

// Ограничения 3 типа (соседи)
void add_type3_constraints(bdd& task) {
    auto add_neighbor = [&](int current_k, int current_j, 
                                int neighbor_k, int neighbor_j, 
                                const std::vector<int>& indices, int delta) {
        for (int i = 0; i < N; i++) {
            if (std::find(indices.begin(), indices.end(), i) != indices.end()) {
                task &= !(p[current_k][i][current_j] ^ p[neighbor_k][i - delta][neighbor_j]); // not XOR (истинно при совпадении)
            } else {
                // Если соседа нет, то свойство не может принимать это значение!
                task &= !p[current_k][i][current_j]; 
            }
        }
    };

    // TODO: Убрать дублирование
    add_neighbor(0, 4, 2, 8, left_indices, left_delta);
    add_neighbor(0, 7, 1, 1, right_indices, right_delta);
    add_neighbor(3, 2, 0, 0, left_indices, left_delta);
    add_neighbor(0, 0, 0, 3, right_indices, right_delta);
    add_neighbor(0, 5, 1, 2, left_indices, left_delta);

    //
    add_neighbor(3, 4, 3, 5, left_indices, left_delta);
}

// Ограничения 4 типа (левый ИЛИ правый сосед)
void add_type4_constraints(bdd& task) {
    auto add_adjacent = [&](int current_k, int current_j,
                           int neighbor_k, int neighbor_j,
                           const std::vector<int>& left_indices, int left_delta,
                           const std::vector<int>& right_indices, int right_delta) {
        for (int i = 0; i < N; i++) {
            bdd tmp = bddfalse; // Предпологаем, что по умолчанию ограничений нет
            bool has_neighbor = false;

            // Левый сосед
            if (std::find(left_indices.begin(), left_indices.end(), i) != left_indices.end()) {
                tmp |= !(p[current_k][i][current_j] ^ p[neighbor_k][i - left_delta][neighbor_j]); // not XOR (истинно при совпадении)
                has_neighbor = true;
            }

            // Правый сосед
            if (std::find(right_indices.begin(), right_indices.end(), i) != right_indices.end()) {
                tmp |= !(p[current_k][i][current_j] ^ p[neighbor_k][i - right_delta][neighbor_j]);
                has_neighbor = true;
            }

            if (has_neighbor) {
                task &= tmp; // Обьединяем ограничения
            } else {
                // Если соседа нет, то свойство не может принимать это значение!
                task &= !p[current_k][i][current_j]; 
            }
        }
    };

    // TODO: Убрать дублирование
    add_adjacent(0,5,0,4, left_indices, left_delta, right_indices, right_delta);
    add_adjacent(2,5,3,8, left_indices, left_delta, right_indices, right_delta);
    add_adjacent(3,6,1,0, left_indices, left_delta, right_indices, right_delta);
}

// Общие ограничения (5 и 6 типы)
void add_general_constraints(bdd& task) {
    // Ограничение 5: Уникальность значений свойств
    for (int k = 0; k < M; k++) {
        for (int i = 0; i < N; i++) {
            for (int j = i+1; j < N; j++) {
                for (int z = 0; z < N; z++) {
                    task &= (p[k][i][z] >> !p[k][j][z]); // Импликация 
                }
            }
        }
    }

    // Ограничение 6: Каждое свойство должно иметь хотя бы одно значение
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            bdd tmp = bddfalse;
            for (int z = 0; z < N; z++) {
                tmp |=  p[j][i][z]; // Хотя бы для одного есть свойство
            }
            task &= tmp;
        }
    }
}