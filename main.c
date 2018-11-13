#include "headers/sa.h"

int main (){
    Problema *p = new_problem("dados/PB1.DAT");
    //print_problem(p);
    del_problem(&p); //p se torna NULL
    return 0;
}