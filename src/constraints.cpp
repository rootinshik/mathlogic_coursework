#include "../include/bdd_manager.h" 
#include "../include/constraints.h"
#include <algorithm>

// Ограничения 1 типа (фиксированные значения)
void add_type1_constraints(bdd& task) {
    task &= p[1][4][3];
    task &= p[0][6][5];
    task &= p[1][7][4];
    task &= p[3][4][4];
    task &= p[2][7][4];
    task &= p[2][5][6];
    task &= p[2][6][0];
    task &= p[3][7][0];
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
            task &= !(p[k1][i][j1] ^ p[k2][i][j2]);
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

// Ограничения 3 типа (левые соседи)
void add_type3_constraints(bdd& task) {
    auto add_left_neighbor = [&](int current_k, int current_j, 
                                int neighbor_k, int neighbor_j, 
                                const std::vector<int>& indices, int delta) {
        for (int i = 0; i < N; i++) {
            if (std::find(indices.begin(), indices.end(), i) != indices.end()) {
                task &= !(p[current_k][i][current_j] ^ p[neighbor_k][i - delta][neighbor_j]);
            } else {
                task &= !p[current_k][i][current_j];
            }
        }
    };

    add_left_neighbor(0, 4, 2, 8, {3,4,5,6,7,8}, 3);
    add_left_neighbor(0, 7, 1, 1, {3,4,6,7}, 2);
    add_left_neighbor(3, 2, 0, 0, {3,4,5,6,7,8}, 3);
    add_left_neighbor(0, 0, 0, 3, {3,4,6,7}, 2);
    add_left_neighbor(0, 5, 1, 2, {3,4,5,6,7,8}, 3);
}

// Ограничения 4 типа (левый ИЛИ правый сосед)
void add_type4_constraints(bdd& task) {
    auto add_adjacent = [&](int current_k, int current_j,
                           int neighbor_k, int neighbor_j,
                           const std::vector<int>& left_indices, int left_delta,
                           const std::vector<int>& right_indices, int right_delta) {
        for (int i = 0; i < N; i++) {
            bdd tmp = bddfalse;
            bool has_neighbor = false;

            // Левый сосед
            if (std::find(left_indices.begin(), left_indices.end(), i) != left_indices.end()) {
                tmp |= !(p[current_k][i][current_j] ^ p[neighbor_k][i - left_delta][neighbor_j]);
                has_neighbor = true;
            }

            // Правый сосед
            if (std::find(right_indices.begin(), right_indices.end(), i) != right_indices.end()) {
                tmp |= !(p[current_k][i][current_j] ^ p[neighbor_k][i - right_delta][neighbor_j]);
                has_neighbor = true;
            }

            if (has_neighbor) {
                task &= tmp;
            } else {
                task &= !p[current_k][i][current_j];
            }
        }
    };

    add_adjacent(0,5,0,4, {3,4,5,6,7,8}, 3, {3,4,6,7}, 2);
    add_adjacent(2,5,3,8, {3,4,5,6,7,8}, 3, {3,4,6,7}, 2);
    add_adjacent(3,6,1,0, {3,4,5,6,7,8}, 3, {3,4,6,7}, 2);
}

// Общие ограничения (5 и 6 типы)
void add_general_constraints(bdd& task) {
    // Ограничение 5: Уникальность значений свойств
    for (int k = 0; k < M; k++) {
        for (int i = 0; i < N; i++) {
            for (int j = i+1; j < N; j++) {
                for (int z = 0; z < N; z++) {
                    task &= (p[k][i][z] >> !p[k][j][z]);
                }
            }
        }
    }

    // Ограничение 6: Каждое свойство должно иметь хотя бы одно значение
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            bdd tmp = bddfalse;
            for (int z = 0; z < N; z++) {
                tmp |= p[j][i][z];
            }
            task &= tmp;
        }
    }
}