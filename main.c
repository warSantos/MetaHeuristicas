#include "headers/sa.h"

int main (){

    // Gerando semente de aleatoriedade.
    struct timeval t;
    gettimeofday (&t, NULL);
    srand(time(NULL) * getpid () * (int) t.tv_usec * 1000);
    // Criando instância do problema.
    Problema *p = new_problem("dados/PB5.DAT");
    print_problem(p);
    // Aplicando Simulated Annealing.
    sa (p, 0.00001, 50, 0.90);
    // Liberando memória das estruturas.
    del_problem(&p);
    return 0;
}