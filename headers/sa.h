#ifndef sa_h
#define sa_h

#include "../headers/rep.h"

/* Copia a solucão corrente na estrutura Problema para a estrutura
S_temporaria. */
void inicializa_so_temp (Problema *p, S_temporaria *s_t);

/* Copia a solução contida em uma estrutura temporária para a estrutura
 Porblema. */
void salva_so_temp (Problema *p, S_temporaria *s_t);

/* Insere um item denominado no parâmetro nas mochilas contidadas na
estrutura problema.*/
void inserir_probl (Problema *p, int item);

/* Remove um item da mochila denominado no parâmetro. */
void remover_probl (Problema *p, int item);

/* Realiza a inserção de um item em uma mochila contida em uma estrutura
temporária. */
void inserir_item (S_temporaria *s_temp, Problema *p, int item);

/* Realiza a remoção de um item em uma mochila contida em uma estrutura
temporária. */
void remover_item (S_temporaria *s_temp, Problema *p, int item);

/* Verifica se um item pode ser inserido nas mochilas sem ferir as 
restrições do problema. */
int cabe_mochilas(Problema *p, int item);

/* Retorna o índice de uma mochila caso a mesma esteja com suas restrições
feridas e retorna -1 caso todas as mochilas estejam OK. */
int restricao_ferida (S_temporaria p, int qnt_mochilas);

/* Constroi solução totalmente aleatória. Os objetos são inseridos nas mochilas
enquanto a capacidade das mochilas não for violada.*/
void constroi_solucao_aleatoria(Problema *p);

/* Constroi solução totalmente gulosa. Os objetos são escolhidos baseado em sua
razão e são inseridos até o limite da capacidade das mochilas ser atingido. */
void constroi_solucao_gulosa(Problema *p, Razao_item *razoes);

/* Realiza o cálculo da função obetivo de uma solução passada por parâmetros. */
float calcula_fo (Item *itens, int qnt_item);

/* Computa valor utilizado pelo algoritmo para decidir tomada de decisões sobre
movimentos de piora. */
float prob_piora (float delta, float temperatura_corrente);

Razao_item *constroi_array_razoes (Problema *p);

/* Ordena um vetor de razões em ordem ascendente. */
void ordena_razoes_asc (Razao_item *razoes, int qnt_item);

/* Calcula teperatura inicial relacionando a temperatura inicial. */
float calcula_temperatura_inicial(Problema *p, Razao_item *razoes, float alfa, int SAmax);

/* Algoritmo simulated annealing.*/
void sa (Problema *p, float temperatura_final, int iter_max, float alfa);

/* Funções para realizar screenshots sobre as estruturas do programa. */

void print_itens_levados (Problema *p, int op);

void print_capacidade_mochilas (Mochila *m, int qnt_mochilas);

#endif