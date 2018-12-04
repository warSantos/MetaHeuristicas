#include "../headers/sa.h"

/* FUNÇÕES PARA CÓPIA DE ESTRUTURAS */

void inicializa_so_temp (Problema *p, S_temporaria *s_t){
    // Copiar FO corrente para FO temporária.
    s_t->fo = p->fo_corrente;
    int i;
    for (i = 0; i < p->qnt_mochilas; i++){
        // Copiar estrutura da mochilas para solução temporária.
        s_t->mochilas[i].cap_restante = p->mochilas[i].cap_restante;
        s_t->mochilas[i].capacidade = p->mochilas[i].capacidade;
    }
    for (i = 0; i < p->qnt_item; i++){
        // Copiar estrutura dos itens para solução temporária.
        s_t->itens[i].profit = p->itens[i].profit;
        s_t->itens[i].adicionado = p->itens[i].adicionado;
    }
}

void salva_so_temp (Problema *p, S_temporaria *s_t){
    // Copiar FO temporária para FO corrente.
    p->fo_corrente = s_t->fo;
    int i;
    for (i = 0; i < p->qnt_mochilas; i++){
        // Copiar estrutura das mochilas temporárias para solução corrente.
        p->mochilas[i].cap_restante = s_t->mochilas[i].cap_restante;
        p->mochilas[i].capacidade = s_t->mochilas[i].capacidade;
    }
    // Copiar estrutura dos itens temporários para solução corrente.
    for (i = 0; i < p->qnt_item; i++){
        p->itens[i].profit = s_t->itens[i].profit;
        p->itens[i].adicionado = s_t->itens[i].adicionado;
    }
}

/* FUNÇÕES DE MANIPULAÇÃO DE OBJETOS */

void inserir_probl (Problema *p, int item){
    int m;
    // Se o item ja estiver na mochila. Retorne.
    if(p->itens[item].adicionado == 1) return;
    // Se não estiver.
    for (m = 0; m < p->qnt_mochilas; m++){
        // Atualize a capacidade restante da mochilas.
        p->mochilas[m].cap_restante -= p->restricoes[m][item];
    }
    // Marque o iten como adicionado.
    p->itens[item].adicionado = 1;
    // Atualize a função objetivo.
    p->fo_otima = p->fo_corrente += p->itens[item].profit;
}

void remover_probl (Problema *p, int item){
    int m;
    // Se o item não estiver na mochila. Retorne.
    if(p->itens[item].adicionado == 0) return;
    for (m = 0; m < p->qnt_mochilas; m++){
        // Atualize a capacidade restante das mochilas.
        p->mochilas[m].cap_restante += p->restricoes[m][item];
    }
    // Marque o item como disponível.
    p->itens[item].adicionado = 0;
    // Atualize a função objetivo.
    p->fo_otima = p->fo_corrente -= p->itens[item].profit;
}

void inserir_item (S_temporaria *s_temp, Problema *p, int item){
    int m;
    // Se o item ja estiver na mochila. Retorne.
    if(s_temp->itens[item].adicionado == 1) return;
    // Se não estiver.
    for (m = 0; m < p->qnt_mochilas; m++){
        // Atualize a capacidade restante das mochilas.
        s_temp->mochilas[m].cap_restante -= p->restricoes[m][item];
    }
    // Marque o iten como adicionado.
    s_temp->itens[item].adicionado = 1;
    // Atualize a função objetivo.
    s_temp->fo += s_temp->itens[item].profit;
}

void remover_item (S_temporaria *s_temp, Problema *p, int item){
    int m;
    // Se o item não estiver na mochila. Retorne.
    if(s_temp->itens[item].adicionado == 0) return;
    // Se estiver na mochila.
    for (m = 0; m < p->qnt_mochilas; m++){
        // Atualize a capacidade restante das mochilas.
        s_temp->mochilas[m].cap_restante += p->restricoes[m][item];
    }
    // Marque o iten como adicionado.
    s_temp->itens[item].adicionado = 0;
    // Atualize a função objetivo.
    s_temp->fo -= s_temp->itens[item].profit;
}

/* FUNÇÕES PARA CONTROLE DE RESTRIÇÕES */

int cabe_mochilas(Problema *p, int item){
    int i;
    // Para cada intem i.
    for(i = 0; i < p->qnt_mochilas; i++){
        // Se i couber na mochila.
        if(p->restricoes[i][item] > p->mochilas[i].cap_restante){
            return 0;
        }
    }
    return 1;
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

/* HEURÍSTICAS CONSTRUTIVAS */

void constroi_solucao_aleatoria(Problema *p){

    int item = rand() % p->qnt_item;
    // Enquanto existir espaço na mochila.
    while(cabe_mochilas(p,item)){
        // Inseri item na mochila.
        inserir_probl(p, item);
        item = rand() % p->qnt_item;
    }
}

void constroi_solucao_gulosa(Problema *p, Razao_item *razoes){
    int i, j, aux;
    ordena_razoes_asc (razoes, p->qnt_item);
    // Enquanto existir espaço na mochila.
    for(i = 0; i < p->qnt_item && cabe_mochilas(p, razoes[i].id_item); i++){
        // Insira o item na mochila.
        inserir_probl(p, razoes[i].id_item);
    }
}

/* FUNÇÕES DO ÂMBITO DO PROBLEMA */

float calcula_fo (Item *itens, int qnt_item){

    int i;
    float fo = 0;
    for (i = 0; i < qnt_item; i++){
        if (itens[i].adicionado == 1){
            fo += itens[i].profit;
        }
    }
    return fo;
}

float prob_piora (float delta, float temperatura_corrente){

    double limiar = exp (delta / (double) temperatura_corrente);
    double prob = (double) rand () / RAND_MAX;
    // Se a relação exp((delta/temperatura)) for menor que
    // um valor aleatório entre (0,1) return true (movimento aceito).
    if (prob < limiar){
        return 1;
    }
    return 0;
}

Razao_item *constroi_array_razoes (Problema *p){

    int i,j;
    Razao_item *razoes = malloc (sizeof (Razao_item) * p->qnt_item);
    // Para cada item.
    for (i = 0; i < p->qnt_item; i++){
        float soma = 0;
        // Para toda mochila.
        for (j = 0; j < p->qnt_mochilas; j++){
            soma += p->restricoes[j][i];
        }
        // Calcule a razão deste item e adicione ao vetor.
        razoes[i].razao = p->itens[i].profit / soma;
        razoes[i].id_item = i;
    }    
    return razoes;
}

void ordena_razoes_asc (Razao_item *razoes, int qnt_item){
    
    int i,j;
    Razao_item aux;
    // Ordenando os items de forma crescente pela razao.
    for (i = 0; i < qnt_item; i++){
        for (j = 0; j < qnt_item; j++){
            // Se o item i tiver razao menor que o item j.
            if (razoes[i].razao < razoes[j].razao){
                // Troque i com j.
                aux = razoes[i];
                razoes[i] = razoes[j];
                razoes[j] = aux;
            }
        }
    }
}

float calcula_temperatura_inicial(Problema *p, Razao_item *razoes, float alfa, int SAmax){
    
    // numero aleatorio entre ZERO e UM
    float r;
    // temperatura corrente
    float temperatura;
    // numero de iteracoes na temperatura corrente
    int iter, i;
    // variacao de energia
    float delta;
    // Quantidade de movimentos aceitos.
    int aceitos = 0;
    // Mínimos de movimentos aceitos.
    int min_aceitos = (int) (alfa * SAmax);
	float fo, fo_viz;
    temperatura = 2;
    // Construindo solução viável aleatória.
	constroi_solucao_aleatoria (p);
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
                if (p->itens[bits[i]].adicionado == 0){
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
            // Adicione ou remova os itens sorteados anteriormente.
            for (i = 0; i < 2; i++){
                if (p->itens[bits[i]].adicionado == 0){
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
    return temperatura;
}

/* FUNÇÕES PARA GERAR VIZINHOS */

void busca_local (Razao_item *razoes, Problema *p, S_temporaria *s_t){
    int i, id_item = -1; // id do item temporário.
    float r_temp; // razão temporária.
    while (restricao_ferida (*s_t, p->qnt_mochilas) != -1){
        r_temp = -1;
        id_item = -1;
        // Iterando sobre a razão os itens e procurando o melhor vizinho ainda
        // não adicionado.
        for (i = 0; i < p->qnt_item; i++){
            if (!s_t->itens[i].adicionado && razoes[i].razao > r_temp){
                r_temp = razoes[i].razao;
                id_item = i;
            }
        }
        // Inserindo item na mochila.
        if (id_item > -1){
            inserir_item (s_t, p, id_item);
        }
    }
    // Corrijingo capacidade da mochila.
    if (id_item > -1){
        remover_item (s_t, p, id_item);
    }
}

void sa (Problema *p, float temperatura_final, int iter_max, float alfa){

    int iteracoes = 0;
    int item;
    int i, smax = (p->qnt_item * (p->qnt_item - 1) / 2);
    float delta;
    S_temporaria s_temp;
    s_temp.itens = malloc (sizeof(Item) * p->qnt_item);
    s_temp.mochilas = malloc (sizeof(Mochila) * p->qnt_mochilas);
    
    // Construindo arrays de custo beneficio baseado na razao dos itens.
    Razao_item *razoes_asc = constroi_array_razoes (p);
    ordena_razoes_asc (razoes_asc, p->qnt_item);
    
    // Calculando temperatura inicial.
    float temperatura_corrente = calcula_temperatura_inicial (p, razoes_asc, alfa, smax);
    
    // Enquanto o numero de iterações máximo não for atingido.
    while (temperatura_corrente > temperatura_final){        
        // Para uma dada temperatura faça.
        for (iteracoes = 0; iteracoes < iter_max; iteracoes++){
            // Copiando solucao corrente para futura solucao vizinha.
            inicializa_so_temp (p, &s_temp);
            // Escolhendo uma vizinho aleatório.
            item = rand () % p->qnt_item;
            if (s_temp.itens[item].adicionado == 0){
                inserir_item (&s_temp, p, item);
                // Enquanto existir mochilas extrapoladas.
                while (restricao_ferida (s_temp, p->qnt_mochilas) != -1){
                    // Escolhendo uma vizinho aleatório.
                    while (1){
                        item = rand () % p->qnt_item;
                        // Se o objeto estiver adicioando.
                        if (s_temp.itens[item].adicionado == 1){
                            // Removendo o item da mochila.
                            remover_item (&s_temp, p, item);
                            break;
                        }
                    }
                }
                // Aplicando busca local.
                busca_local (razoes_asc, p, &s_temp);
                delta = s_temp.fo - p->fo_corrente;
                if (delta >= 0 || prob_piora (delta, temperatura_corrente)){
                    salva_so_temp (p, &s_temp);
                }
            }else {
                // Removendo o item da mochila.
                remover_item (&s_temp, p, item);
                delta = s_temp.fo - p->fo_corrente;
                if (delta >= 0 || prob_piora (delta, temperatura_corrente)){
                    salva_so_temp (p, &s_temp);
                }
            }
            // Se a solução atual for melhor que a melhor encontrada até o momento.
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
        temperatura_corrente *= alfa;
    }
    print_itens_levados (p, 1);
    print_capacidade_mochilas (p->opt_mochilas, p->qnt_mochilas);
    free(s_temp.itens);
    free(s_temp.mochilas);
    free(razoes_asc);
}

void print_itens_levados (Problema *p, int op){
    float fo = 0;
    printf ("==========================================================\n");
    if(!op){
        printf ("Itens Levados.\n");
        int i;
        for (i = 0; i < p->qnt_item; i++){
            if (p->itens[i].adicionado == 1){
                printf ("Item: %d, Profit: %f, Mochila: %d, Restricao: %f.\n",
                    i, p->itens[i].profit, p->itens[i].adicionado, p->restricoes[p->itens[i].adicionado][i]);
                    fo += p->itens[i].profit;
            }
        }
    }else{
        printf ("Itens Levados (Resultado Final).\n");
        int i;
        for (i = 0; i < p->qnt_item; i++){
            if (p->opt_itens[i].adicionado == 1){
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
