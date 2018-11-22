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

void inserir_probl (Problema *p, int item){
    int m;
    if(p->itens[item].adicionado != -1) return;
    for (m = 0; m < p->qnt_mochilas; m++){
        p->mochilas[m].cap_restante -= p->restricoes[m][item];
    }
    p->itens[item].adicionado = 1;
    p->fo_otima = p->fo_corrente += p->itens[item].profit;
}

void remover_probl (Problema *p, int item){
    int m;
    if(p->itens[item].adicionado == -1) return;
    for (m = 0; m < p->qnt_mochilas; m++){
        p->mochilas[m].cap_restante += p->restricoes[m][item];
    }
    p->itens[item].adicionado = -1;
    p->fo_otima = p->fo_corrente -= p->itens[item].profit;
}

int cabe_mochilas(Problema *p, int item){
    int i;
    for(i = 0; i < p->qnt_mochilas; i++){
        if(p->restricoes[i][item] > p->mochilas[i].cap_restante){
            return 0;
        }
    }
    return 1;
}

void constroi_solucao_aleatoria(Problema *p){
    srand(time(NULL));

    int item = rand() % p->qnt_item;
    while(cabe_mochilas(p,item)){
        inserir_probl(p, item);
        item = rand() % p->qnt_item;
    }
}

void constroi_solucao_gulosa(Problema *p){
    int i, j, aux;
    int itens[p->qnt_item];
    for(i = 0; i < p->qnt_item; itens[i] = i, i++);

    // Ordenando os itens com melhores razões
    for(i = 0; i < p->qnt_item; i++){
        for(j = 0; j < p->qnt_item; j++){
            if(p->itens[itens[i]].razao < p->itens[itens[j]].razao){
                aux = itens[i];
                itens[i] = itens[j];
                itens[j] = aux;
            }
        }
    }
    for(i = 0; i < p->qnt_item && cabe_mochilas(p, itens[i]); i++){
        inserir_probl(p, itens[i]);
    }
}

void inserir_item (S_temporaria *s_temp, Problema *p, int item){
    int m;
    if(s_temp->itens[item].adicionado != -1) return;
    for (m = 0; m < p->qnt_mochilas; m++){
        s_temp->mochilas[m].cap_restante -= p->restricoes[m][item];
    }
    s_temp->itens[item].adicionado = 1;
    s_temp->fo += s_temp->itens[item].profit;
}

void remover_item (S_temporaria *s_temp, Problema *p, int item){
    int m;
    if(s_temp->itens[item].adicionado == -1) return;
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

float calcula_fo (Item *itens, int qnt_item){

    int i;
    float fo = 0;
    for (i = 0; i < qnt_item; i++){
        if (itens[i].adicionado != -1){
            fo += itens[i].profit;
        }
    }
    return fo;
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

float calcula_temperatura_inicial(Problema *p, float alfa, int SAmax){
    
    float r;              // numero aleatorio entre ZERO e UM
    float temperatura;    // temperatura corrente
    int iter, i;              // numero de iteracoes na temperatura corrente
    int posicao_escolhida; // objeto escolhido
    float delta;             // variacao de energia
    int aceitos, min_aceitos;
	float fo, fo_viz;
	
    temperatura = 2;
	constroi_solucao_aleatoria (p);
		   
    aceitos = 0;
    min_aceitos = (int) (alfa * SAmax);
    while (aceitos < min_aceitos){
		iter = 0;
		while (iter < SAmax){
            iter++;
			fo = p->fo_corrente;
            int bits[2];
	
            bits[0] = ((float)rand()/RAND_MAX) * p->qnt_item;
            bits[1] = ((float)rand()/RAND_MAX) * p->qnt_item;
            while (bits[0] == bits[1]) bits[1] = ((float)rand()/RAND_MAX) * p->qnt_item;

            for (i = 0; i < 2; i++){
                if (p->itens[bits[i]].adicionado == -1){
                    inserir_probl (p, bits[i]);
                }else {
                    remover_probl (p, bits[i]);
                }
            }

	  	    /* calcule a variacao de energia */
            fo_viz = calcula_fo(p->itens, p->qnt_item);
			delta = fo_viz - fo;
			
	  	    /* se houver melhora, aceite o vizinho */
	        if (delta > 0){
                aceitos++;
        	}
	        else {
				/* se houver piora, aceite o vizinho com uma determinada
                 probabilidade */
	            r = ((float)rand()/RAND_MAX);
                if (r < exp(delta / temperatura)){
                    aceitos++;
                }
	        }
            for (i = 0; i < 2; i++){
                if (p->itens[bits[i]].adicionado == -1){
                    inserir_probl (p, bits[i]);
                }else {
                    remover_probl (p, bits[i]);
                }
            }
		}
		if (aceitos < min_aceitos){
            aceitos = 0;
            temperatura = temperatura * 1.1;
        }
    }
    printf("temperatura inicial = %f \n",temperatura);    
    return temperatura;
}

void sa (Problema *p, float temperatura_final, int iter_max, float alfa){

    srand (time(NULL));
    int iteracoes = 0;
    int bit;
    int i, smax = (p->qnt_item * (p->qnt_item - 1) / 2);
    S_temporaria s_temp;
    s_temp.itens = malloc (sizeof(Item) * p->qnt_item);
    s_temp.mochilas = malloc (sizeof(Mochila) * p->qnt_mochilas);

    float temperatura_corrente = calcula_temperatura_inicial (p, alfa, smax);
    float delta;
    // Gerar solução inicial.
    constroi_solucao_aleatoria(p);
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
