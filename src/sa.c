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

int melhor_mochila (Problema *p, int item){

    int m;
    float restricao = p->restricoes[0][item];
    // Para toda restricao existente ache a melhor.
    for (m = 0; m < p->qnt_mochilas; ++m){
        if (p->restricoes[m][item] < restricao){
            restricao = p->restricoes[m][item];
        }
    }
    // Retorne o id da mochila com menor restrição.
    return m;
}

int restricao_ferida (S_temporaria p, int qnt_mochilas){

    int m;
    // Para toda mochila.
    for (m = 0; m < qnt_mochilas; m++){
        // Se alguma mochila estiver com sua capacidade extrapolada.
        if (p.mochilas[m].cap_restante < 0){
            return m;
        }
    }
    return -1;
}

float prob_piora (){

    float prob = 0;
    return prob;
}

void inicializa_so_temp (Problema *p, S_temporaria s_t){

    s_t.fo = p->fo_corrente;
    int i;
    for (i = 0; i < p->qnt_mochilas; i++){
        s_t.mochilas[i].cap_restante = p->mochilas[i].cap_restante;
        s_t.mochilas[i].capacidade = p->mochilas[i].capacidade;
    }
    for (i = 0; i < p->qnt_item; i++){
        s_t.itens[i].profit = p->itens[i].profit;
        s_t.itens[i].id_mochila = p->itens[i].id_mochila;
    }
}

void salva_so_temp (Problema *p, S_temporaria s_t){
    
    p->fo_corrente = s_t.fo;
    int i;
    for (i = 0; i < p->qnt_mochilas; i++){        
        p->mochilas[i].cap_restante = s_t.mochilas[i].cap_restante;
        p->mochilas[i].capacidade = s_t.mochilas[i].capacidade;
    }
    for (i = 0; i < p->qnt_item; i++){        
        p->itens[i].profit = s_t.itens[i].profit;
        p->itens[i].id_mochila = s_t.itens[i].id_mochila;
    }
}

void sa (Problema *p, float temperatura_inicial, 
    float temperatura_final, int iter_max, float limiar){
    
    srand (time(NULL));
    int iteracoes = 0;
    int bit;
    int i, j;
    S_temporaria s_temp;
    s_temp.itens = malloc (sizeof(Item) * p->qnt_item);
    s_temp.mochilas = malloc (sizeof(Mochila) * p->qnt_mochilas);
    //int itens_disponiveis = calloc (p->qnt_item, sizeof(int));
    //int itens_adicionados = calloc (p->qnt_item, sizeof(int));
    //int ultimo_adicionado = 0;
    float temperatura_corrente = temperatura_inicial;
    // Gerar solução inicial.
    // Enquanto o numero de iterações máximo não for atingido.
    while (temperatura_corrente > temperatura_final){
        // Para uma dada temperatura faça.
        for (iteracoes = 0; iteracoes < iter_max; iteracoes++){
            // Copiando solucao corrente para futura solucao vizinha.
            inicializa_so_temp (p, s_temp);
            // Escolhendo uma vizinho aleatório.
            //TO-DO: Melhorar função de gerar vizinhança.
            bit = rand () % p->qnt_item;
            if (s_temp.itens[bit].id_mochila == -1){
                // Adicionando o item na mochila.
                s_temp.itens[bit].id_mochila = melhor_mochila(p, bit);
                s_temp.fo += s_temp.itens[bit].profit;
                s_temp.mochilas[s_temp.itens[bit].id_mochila].cap_restante -= p->restricoes[s_temp.itens[bit].id_mochila][bit];
                // Enquanto existir mochilas extrapoladas.
                while (restricao_ferida (s_temp, p->qnt_mochilas) != -1){
                    // Escolhendo uma vizinho aleatório.
                    //TO-DO: Melhorar função de gerar vizinhança.
                    while (1){
                        bit = rand () % p->qnt_item;
                        // Se o objeto estiver em nenhuma mochila.
                        if (s_temp.itens[bit].id_mochila != -1){
                            // Removendo o item da mochila.
                            // Adicionando o item na mochila.
                            s_temp.itens[bit].id_mochila = -1;
                            s_temp.fo -= s_temp.itens[bit].profit;
                            s_temp.mochilas[s_temp.itens[bit].id_mochila].cap_restante += p->restricoes[s_temp.itens[bit].id_mochila][bit];
                            break;
                        }
                    }
                }
                // TO-DO: Acabar função de probabilidade de piora.
                if (p->fo_corrente < s_temp.fo || prob_piora ()){
                    
                }
            }else {
                // Removendo o item da mochila.
                // Adicionando o item na mochila.
                s_temp.itens[bit].id_mochila = -1;
                s_temp.fo -= s_temp.itens[bit].profit;
                s_temp.mochilas[s_temp.itens[bit].id_mochila].cap_restante += p->restricoes[s_temp.itens[bit].id_mochila][bit];
                // TO-DO: Acabar função de probabilidade de piora.
                if (p->fo_corrente < s_temp.fo || prob_piora ()){
                    
                }
            }// Se a solução atual for melhor que a global.
            if (p->fo_final < p->fo_corrente){

            }
        }
        //TO-DO: Fazer calculo de queda de temperatura.
    }
    free(s_temp.itens);
    free(s_temp.mochilas);
    //free(itens_disponiveis);
    //free(itens_adicionados);
}