#ifndef sa_h
#define sa_h

#include "../headers/rep.h"

// Simulated annealing.
void sa (Problema *p, float temperatura_inicial, float temperatura_final, int iter_max){

    int iteracoes = 0;
    float temperatura_corrente = temperatura_inicial;
    // Gerar solução inicial.
    // Iniciar temperatura.
    // Enquanto o numero de iterações máximo não for atingido.
    while (temperatura_corrente > temperatura_final){
        
        // Para uma dada temperatura faça.
        for (iteracoes = 0; iteracoes < iter_max; iteracoes++){

        }
    }
}
#endif