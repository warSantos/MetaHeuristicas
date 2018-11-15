#ifndef sa_h
#define sa_h

#include "../headers/rep.h"

#define passo(N) printf ("passo: %d.\n", N);

void print_itens_levados (Problema *p, int op);

void print_capacidade_mochilas (Mochila *m, int qnt_mochilas);

void constroi_solucao_inicial (Problema *p);

// Simulated annealing.
void sa (Problema *p, float temperatura_inicial, 
    float temperatura_final, int iter_max, float alfa);

#endif