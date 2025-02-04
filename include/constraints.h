#pragma once

#include <vector>
#include "buddy/bdd.h"
#include "bdd_manager.h"
#include "config.h"

void add_type1_constraints(bdd& task);
void add_type2_constraints(bdd& task);
void add_type3_constraints(bdd& task);
void add_type4_constraints(bdd& task);
void add_general_constraints(bdd& task);
