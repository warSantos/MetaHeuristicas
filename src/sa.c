#include "../headers/sa.h"

Razao *constroi_array_razoes (Problema *p){

    Razao *razoes = malloc (sizeof(Razao) * (p->qnt_item * p->qnt_mochilas));
    int i,j;
    // Para toda mochila i.
    for (i = 0; i < p->qnt_mochilas; i++){
        // Para todo iten j.
        for (j = 0; j < p->qnt_item; j++){
            razoes[(i + 1) * j].razao = p->restricoes[i][j];
            razoes[(i + 1) * j].id_mochila = i;
        }
    }
    return razoes;
}

void vizinhanca (){

}

void vizinhanca_grasp (){

}

void sa (Problema *p, float temperatura_inicial, 
    float temperatura_final, int iter_max, float limiar){

    int iteracoes = 0;
    float temperatura_corrente = temperatura_inicial;
    // Gerar solução inicial.
    // Enquanto o numero de iterações máximo não for atingido.
    while (temperatura_corrente > temperatura_final){
        
        // Para uma dada temperatura faça.
        for (iteracoes = 0; iteracoes < iter_max; iteracoes++){

        }
    }
}