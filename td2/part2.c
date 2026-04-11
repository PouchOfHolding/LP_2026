#ifndef STACK_H
#define STACK_H
#include<stdbool.h>
#define STACK_SIZE 1000

typedef struct {
    int data [STACK_SIZE];
    int top;
}stack;

void init (stack *s);
void push (stack *s, int value);
int pop (stack *s);
int top (stack *s);
bool is_empty (stack *s);

#endif
#include <stdio.h>

void init(stack *s){
    s->top =-1; // colocamos -1 para mostar que a pilha está vazia
                // pois se tivesse 0, maquina iria achar que o indice 0 tinha elementos
}

void push(stack *s, int value){
    if (s->top == STACK_SIZE -1){// array vai de 0 ate tamanho-1
        printf("ERROR: STACK OVERFLOW\n");
        return;
    }
    s->data[++(s->top)]= value;
    printf("Pushed: %d\n", value);    
}

int pop(stack *s){
    if (is_empty (s)){
        printf("ERROR: STACKOVERFLOW!\n");
        return -1;
    }
    int value =s->data[(s->top)--];
    printf("Popped; %D\n", value);
    return value;
}