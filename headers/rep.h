#ifndef rep_h
#define rep_h

	#include <stdio.h>
	#include <stdlib.h>
	#include <time.h>

	/* Definição das estruturas de modelagem */
	typedef struct{
		float profit; //Peso do objeto.
		int id_mochila; //-1 se não estiver em nenhuma mochila.
	}Item;

	typedef struct{
		float capacidade;
		float cap_restante;
	}Mochila;

	typedef struct{
		int qnt_item;
		int qnt_mochilas;
		Item *itens;
		Mochila *mochilas; 
		float **restricoes;
		float fo_otima;
	}Problema;

	typedef struct razao {
		float razao;
		int id_mochila;
	} Razao;

	/* Funções do programa */
	Problema* new_problem(char *path);

	void print_problem(Problema *problema);

	void del_problem(Problema **problema);



#endif