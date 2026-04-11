#include <stdio.h>


//exericio 1

int duplicacao ( int a){ return a*2;}
int triplicacao (int a) {return a*3;}
int quadruplicacao (int a){ return a*a;}

int main (void){

    int (*transformacao [3]) (int)= {duplicacao, triplicacao, quadruplicacao};

    int vetor [] = {1,2,3,4,5};
    int n = sizeof(vetor) /sizeof(vetor[0]);
    int opcao;

    printf ("escolhe (0,1,2)");
    scanf ("%d", opcao);
    
    if (opcao <0 || opcao > 2) return 1;

    for (int  i = 0; i < n; i++){
        printf("%d\n | transformado %d", vetor[i], transformacao[opcao](vetor[i]));
    }
    return 0;
}
