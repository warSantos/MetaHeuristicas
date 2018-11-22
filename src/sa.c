#include "../headers/sa.h"

void constroi_array_razoes (Problema *p){

    int i,j;
    for (i = 0; i < p->qnt_item; i++){
        float soma = 0;
        for (j = 0; j < p->qnt_mochilas; j++){
            soma += p->restricoes[j][i];
        }
        p->itens[i].razao = p->itens[i].profit / soma;
    }
}

void troca_bit (Item *itens, int bit, int adicionado){

    itens[bit].adicionado = adicionado;
}

void vizinhanca_grasp (){

}

void inserir_item (S_temporaria *s_temp, Problema *p, int item){
    int m;
    for (m = 0; m < p->qnt_mochilas; m++){
        s_temp->mochilas[m].cap_restante -= p->restricoes[m][item];
    }
    s_temp->itens[item].adicionado = 1;
    s_temp->fo += s_temp->itens[item].profit;
}

void remover_item (S_temporaria *s_temp, Problema *p, int item){
  int m;
  for (m = 0; m < p->qnt_mochilas; m++){
      s_temp->mochilas[m].cap_restante += p->restricoes[m][item];
  }
  s_temp->itens[item].adicionado = -1;
  s_temp->fo -= s_temp->itens[item].profit;
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

float prob_piora (float delta, float temperatura_corrente){

    double limiar = exp (delta / (double) temperatura_corrente);
    double prob = (double) rand () / RAND_MAX;
    if (prob < limiar){
        return 1;
    }
    return 0;
}

void inicializa_so_temp (Problema *p, S_temporaria *s_t){

    s_t->fo = p->fo_corrente;
    int i;
    for (i = 0; i < p->qnt_mochilas; i++){
        s_t->mochilas[i].cap_restante = p->mochilas[i].cap_restante;
        s_t->mochilas[i].capacidade = p->mochilas[i].capacidade;
    }
    for (i = 0; i < p->qnt_item; i++){
        s_t->itens[i].profit = p->itens[i].profit;
        s_t->itens[i].adicionado = p->itens[i].adicionado;
    }
}

void salva_so_temp (Problema *p, S_temporaria *s_t){

    p->fo_corrente = s_t->fo;
    int i;
    for (i = 0; i < p->qnt_mochilas; i++){
        p->mochilas[i].cap_restante = s_t->mochilas[i].cap_restante;
        p->mochilas[i].capacidade = s_t->mochilas[i].capacidade;
    }
    for (i = 0; i < p->qnt_item; i++){
        p->itens[i].profit = s_t->itens[i].profit;
        p->itens[i].adicionado = s_t->itens[i].adicionado;
    }
}

void constroi_solucao_inicial (Problema *p){

    int i,j;
    // Adicionando elementos com custo 0.
    for (i = 0; i < p->qnt_mochilas; i++){
        for (j = 0; j < p->qnt_item; j++){
            // Se o elemente não possuir custo para ser levado e ainda não foi levado.
            if (p->restricoes[i][j] == 0 && p->itens[j].adicionado == -1){
                p->itens[j].adicionado = i;
                p->fo_corrente += p->itens[j].profit;
            }
        }
    }
    p->fo_final = p->fo_corrente;
    printf ("==========================================================\n");
    printf ("FO Inicial: %f.\n", p->fo_final);
    printf ("==========================================================\n");
}

void sa (Problema *p, float temperatura_inicial,
    float temperatura_final, int iter_max, float alfa){

    srand (time(NULL));
    int iteracoes = 0;
    int bit;
    int i;
    S_temporaria s_temp;
    s_temp.itens = malloc (sizeof(Item) * p->qnt_item);
    s_temp.mochilas = malloc (sizeof(Mochila) * p->qnt_mochilas);
    float temperatura_corrente = temperatura_inicial;
    float delta;
    // Gerar solução inicial.
    //constroi_solucao_inicial (p);
    print_itens_levados (p, 0);
    // Enquanto o numero de iterações máximo não for atingido.
    while (temperatura_corrente > temperatura_final){
        // Para uma dada temperatura faça.
        for (iteracoes = 0; iteracoes < iter_max; iteracoes++){
            // Copiando solucao corrente para futura solucao vizinha.
            inicializa_so_temp (p, &s_temp);
            // Escolhendo uma vizinho aleatório.
            //TO-DO: Melhorar função de gerar vizinhança.
            bit = rand () % p->qnt_item;
            if (s_temp.itens[bit].adicionado == -1){
                inserir_item (&s_temp, p, bit);
                // Enquanto existir mochilas extrapoladas.
                while (restricao_ferida (s_temp, p->qnt_mochilas) != -1){
                    // Escolhendo uma vizinho aleatório.
                    //TO-DO: Melhorar função de gerar vizinhança.
                    while (1){
                        bit = rand () % p->qnt_item;
                        // Se o objeto estiver em nenhuma mochila.
                        if (s_temp.itens[bit].adicionado != -1){
                            // Removendo o item da mochila.
                            remover_item (&s_temp, p, bit);
                            break;
                        }
                    }
                }
                delta = s_temp.fo - p->fo_corrente;
                // TO-DO: Testar solucao probabilidade.
                if (delta >= 0 || prob_piora (delta, temperatura_corrente)){
                    salva_so_temp (p, &s_temp);
                }
            }else {
                // Removendo o item da mochila.
                remover_item (&s_temp, p, bit);
                // TO-DO: Testar solucao probabilidade.
                delta = s_temp.fo - p->fo_corrente;
                if (delta >= 0 || prob_piora (delta, temperatura_corrente)){
                    salva_so_temp (p, &s_temp);
                }
            }// Se a solução atual for melhor que a melhor encontrada até o momento.
            if (p->fo_final < p->fo_corrente){
                for (i = 0; i < p->qnt_mochilas; ++i){
                    p->opt_mochilas[i].cap_restante = p->mochilas[i].cap_restante;
                }
                for (i = 0; i < p->qnt_item; i++){
                    p->opt_itens[i].adicionado = p->itens[i].adicionado;
                }
                p->fo_final = p->fo_corrente;
            }
        }
        //TO-DO: Testar queda de temperatura.
        temperatura_corrente *= alfa;
    }
    print_itens_levados (p, 1);
    print_capacidade_mochilas (p->opt_mochilas, p->qnt_mochilas);
    free(s_temp.itens);
    free(s_temp.mochilas);
    //free(itens_disponiveis);
    //free(itens_adicionados);
}

void print_itens_levados (Problema *p, int op){
    float fo = 0;
    printf ("==========================================================\n");
    if(!op){
        printf ("Itens Levados.\n");
        int i;
        for (i = 0; i < p->qnt_item; i++){
            if (p->itens[i].adicionado != -1){
                printf ("Item: %d, Profit: %f, Mochila: %d, Restricao: %f.\n",
                    i, p->itens[i].profit, p->itens[i].adicionado, p->restricoes[p->itens[i].adicionado][i]);
                    fo += p->itens[i].profit;
            }
        }
    }else{
        printf ("Itens Levados (Resultado Final).\n");
        int i;
        for (i = 0; i < p->qnt_item; i++){
            if (p->opt_itens[i].adicionado != -1){
                printf ("Item: %d, Profit: %f, Mochila: %d, Restricao: %f..\n",
                    i, p->opt_itens[i].profit, p->opt_itens[i].adicionado, p->restricoes[p->opt_itens[i].adicionado][i]);
                    fo += p->opt_itens[i].profit;
            }
        }
    }
    printf ("FO: %f.\n", fo);
    printf ("==========================================================\n");
}

void print_capacidade_mochilas (Mochila *m, int qnt_mochilas){
    int i;
    for (i = 0; i < qnt_mochilas; ++i){
        printf ("Capacidade da mochila %d: %f.\n", i, m[i].cap_restante);
    }
}
