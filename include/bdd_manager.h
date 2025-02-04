#pragma once
#include <buddy/bdd.h>
#include "config.h"

extern bdd p[M][N][N];

void initialize_bdd();
void cleanup_bdd();