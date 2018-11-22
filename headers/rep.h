#ifndef rep_h
#define rep_h

	#include <stdio.h>
	#include <stdlib.h>
	#include <time.h>
	#include <math.h>

	/* Definição das estruturas de modelagem */
	typedef struct{
		float profit; //Peso do objeto.
		int adicionado; //-1 se não estiver em nenhuma mochila.
		float razao;
	}Item;

	typedef struct{
		float capacidade;
		float cap_restante;
	}Mochila;

	typedef struct{
		Item *itens;
		Mochila *mochilas;
		float fo;
	}S_temporaria;

	typedef struct{
		int qnt_item;
		int qnt_mochilas;
		Item *itens;
		Item *opt_itens;
		Mochila *mochilas;
		Mochila *opt_mochilas;
		float **restricoes;
		float fo_corrente;
		float fo_final;
		float fo_otima;
	}Problema;

	/* Funções do programa */
	Problema* new_problem(char *path);

	void print_problem(Problema *problema);

	void del_problem(Problema **problema);

#endif