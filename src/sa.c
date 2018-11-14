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

void troca_bit (Item *itens, int bit, int id_mochila){

    itens[bit].id_mochila = id_mochila;
}

void vizinhanca_grasp (){

}

void sa (Problema *p, float temperatura_inicial, 
    float temperatura_final, int iter_max, float limiar){
    
    srand (time(NULL));
    int iteracoes = 0;
    int bit;
    int i, j;
    //int itens_disponiveis = calloc (p->qnt_item, sizeof(int));
    //int itens_adicionados = calloc (p->qnt_item, sizeof(int));
    //int ultimo_adicionado = 0;
    int adicionou = 0;
    float temperatura_corrente = temperatura_inicial;
    float fo_corrente = 0, fo_vizinho;
    // Gerar solução inicial.
    // Enquanto o numero de iterações máximo não for atingido.
    while (temperatura_corrente > temperatura_final){
        // Para uma dada temperatura faça.
        for (iteracoes = 0; iteracoes < iter_max; iteracoes++){
            // Escolhendo uma vizinho aleatório.
            //TO-DO: Melhorar função de gerar vizinhança.
            bit = rand () % p->qnt_item;
            for (i = 0; i < p->qnt_mochilas; i++){
                // Se o elemento respeitar as restrições da mochila.
                if (p->mochilas[i].cap_restante <= p->restricoes[i][bit]){
                    // Reduz capacidade da mochila.
                    p->mochilas[i].cap_restante -= p->restricoes[i][bit];
                    // Marca o iten como levado.
                    p->itens[bit].id_mochila = i;
                    // Marcando o elemento como adicionado.
                    //itens_adicionados[ultimo_adicionado] = bit;
                    adicionou = 1;
                    // Atualize o profit da solução corrente.
                    fo_vizinho += p->itens[bit].profit;
                    break;
                }
            }
            // Se nenhum elemento foi adicionado.
            if (!adicionou){
                // Escolhendo um vizinho aleatório.
                //TO-DO: Melhorar função de gerar vizinhança.
                bit = rand () % p->qnt_item;
                // Desocupe a mochila e marque o item como disponível.
                p->mochilas[p->itens[bit].id_mochila].cap_restante += p->restricoes[p->itens[bit].id_mochila][bit];
                p->itens[bit].id_mochila = -1;
            }
            //if ()
        }
    }
}