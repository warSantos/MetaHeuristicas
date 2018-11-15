#ifndef sa_h
#define sa_h

#include "../headers/rep.h"

void constroi_solucao_inicial (Problema *p);

// Simulated annealing.
void sa (Problema *p, float temperatura_inicial, 
    float temperatura_final, int iter_max, float alfa);

#endif