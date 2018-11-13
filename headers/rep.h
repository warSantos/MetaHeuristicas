#ifndef rep_h
#define rep_h

	#include <stdio.h>
	#include <stdlib.h>

	/* Definição das estruturas de modelagem */
	typedef struct{
		float peso; //Peso do objeto
		int id_mochila; //-1 se não estiver em lugar nenhum
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
		float valor_otimo;
	}Problema;

	/* Funções do programa */
	Problema* new_problem(char *path);

	void print_problem(Problema *problema);

	void del_problem(Problema **problema);

#endif