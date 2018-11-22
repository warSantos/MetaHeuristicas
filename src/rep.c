#include "../headers/rep.h"

Problema *new_problem(char *path){
	FILE *problema = fopen(path, "r");
	if(problema == NULL){
		printf("Não reconheceu o caminho\n");
		return NULL;
	}

	int i, j;
	Problema *p = calloc(1, sizeof(Problema));
	
	//Capturano informações sobre qnt itens/mochilas
	fscanf(problema, "%d", &(p->qnt_mochilas));
	fscanf(problema, "%d", &(p->qnt_item));

	//Inicializando estruturas
	p->itens = calloc(p->qnt_item, sizeof(Item));
	p->opt_itens = calloc(p->qnt_item, sizeof(Item));
	p->mochilas = calloc(p->qnt_mochilas, sizeof(Mochila));
	p->opt_mochilas = calloc(p->qnt_mochilas, sizeof(Mochila));
	
	//Capturando os profits dos objetos
	for(i = 0; i < p->qnt_item; i++){
		float profit;
		fscanf(problema, "%f", &profit);
		if(profit < 0) printf("Objeto %d invalido\n", i);
		p->itens[i].profit = profit;
		p->opt_itens[i].profit = profit;
		p->itens[i].adicionado = -1;
		p->opt_itens[i].adicionado = -1;
	}

	//Capturando as capacidades das mochilas
	for(i = 0; i < p->qnt_mochilas; i++){
		float capacidade;
		fscanf(problema, "%f", &capacidade);
		if(capacidade<0) printf("Mochila %d invalida\n", i);
		p->mochilas[i].capacidade = capacidade;
		p->mochilas[i].cap_restante = capacidade;
	}

	//Capturando as restrições do problema
	p->restricoes = calloc(p->qnt_mochilas, sizeof(float*));
	for(i = 0; i < p->qnt_mochilas; i++){
		p->restricoes[i]=calloc(p->qnt_item, sizeof(float));
		for(j = 0; j < p->qnt_item; j++){
			float restr; 
			fscanf(problema, "%f", &restr);
			if(restr < 0) 
				printf("Restricao [%d..%d] invalida\n",i,j);
			p->restricoes[i][j] = restr;
		}
	}

	// Pegando o valor da solução ótima
	fscanf(problema, "%f", &(p->fo_otima));

	fclose(problema);
	return p;
}

void print_problem(Problema *p){
	int i, j;
	
	//Informações sobre qnt itens/mochilas e solucao otima
	printf("Qnt mochilas: %d\n", p->qnt_mochilas);
	printf("Qnt itens: %d\n", p->qnt_item);
	printf("Solucao otima: %.2f\n\n", p->fo_otima);

	//Informações sobre profits dos itens
	printf("profits dos itens\n" 
			"[item: profit]\n");
	for(i = 0; i < p->qnt_item; i++){
		printf("%d: %.2f\n", i, p->itens[i].profit);
	}printf("\n");

	//Capturando as capacidades das mochilas
	printf("Capacidade das Mochilas\n" 
			"[mochila: capacidade..capacidade_restante]\n");
	for(i = 0; i < p->qnt_mochilas; i++){
		printf("%d: %.2f..%.2f\n", i, 
				p->mochilas[i].capacidade,
				p->mochilas[i].cap_restante);
	}printf("\n");

	//Capturando as restrições do problema
	printf("Restrições do problema\n" 
			"[mochila: item1 item2 ... itemN]\n");
	printf("%4c ",' '); 
	for(i=0; i<p->qnt_item;i++) printf("%4d ", i);
	printf("\n");
	for(i = 0; i < p->qnt_mochilas; i++){
		printf("%3d: ",i); 
		for(j = 0; j < p->qnt_item; j++){
			printf("%2d.%1.0f|", 
				(int)p->restricoes[i][j],
				(((int)p->restricoes[i][j]) - 
				p->restricoes[i][j])*10);
		}
		printf("\n");
	}
}

void del_problem(Problema **problem){
	Problema *p = *problem;
	if(p == NULL){return;}

	free(p->itens);
	free(p->opt_itens);

	free(p->mochilas);
	free(p->opt_mochilas);

	int i;
	for(i = 0; i < p->qnt_mochilas; i++){
		free(p->restricoes[i]);
	}free(p->restricoes);

	free(p);
	*problem = NULL;
}