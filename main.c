#include "headers/sa.h"

int main (){
    Problema *p = new_problem("dados/PB1.DAT");
    print_problem(p);
    sa (p, 500, 0.0001, 20, 0.85);
    del_problem(&p); //p se torna NULL
    return 0;
}