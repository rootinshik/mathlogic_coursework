#pragma once

#include <vector>
#include <algorithm>

#include "buddy/bdd.h"
#include "config.h"

extern bdd p[M][N][N];

void add_type1_constraints(bdd&);
void add_type2_constraints(bdd&);
void add_type3_constraints(bdd&);
void add_type4_constraints(bdd&);
void add_general_constraints(bdd&);
