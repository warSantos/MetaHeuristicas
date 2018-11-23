#include "headers/sa.h"

int main (){
    Problema *p = new_problem("dados/PB5.DAT");
    print_problem(p);
    sa (p, 0.00001, 50, 0.85);
    del_problem(&p); //p se torna NULL
    return 0;
}