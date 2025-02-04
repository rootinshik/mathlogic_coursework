#include "../include/bdd_manager.h"
#include <../include/buddy/bdd.h>

bdd p[M][N][N];

void initialize_bdd() {
    bdd_init(100000, 10000);
    bdd_setvarnum(N_VAR);
    
    unsigned I = 0;
    for (unsigned i = 0; i < N; i++) {
        for (unsigned j = 0; j < N; j++) {
            for (int z = 0; z < M; z++) {
                p[z][i][j] = bddtrue;
                for (unsigned k = 0; k < LOG_N; k++) {
                    ((j >> k) & 1) ? 
                    p[z][i][j] &= bdd_ithvar(I + LOG_N*z + k) : 
                    p[z][i][j] &= bdd_nithvar(I + LOG_N*z + k);
                }
            }
        }
        I += LOG_N*M;
    }
}

void cleanup_bdd() {
    bdd_done();
}