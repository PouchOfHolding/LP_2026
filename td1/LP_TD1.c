#include <stdio.h>
#include <stddef.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#define Dynvec_init_capacity 4

typedef struct Dynvec{
    int *data;
    size_t length;
    size_t capacity;
}dynvec;

void dynvec_create( dynvec *v){
    v->capacity = Dynvec_init_capacity;
    v->length =0;
    v->data = (int *)malloc(sizeof(int) * v->capacity);
}

void dynvec_empty (dynvec *v){
    if (!v){return;}
    free(v->data);
    v->data=NULL;
    v->capacity =0;
    v->length =0;
}

static void dynvec_resize(dynvec *v, int size){
    if (!v){return;}
    int *newdata = realloc(v->data, size * sizeof(int));
    if (!newdata){
        errno=ENOMEM;
        #ifdef DEBUG_ON
        perror ("create. erro de alocação de memoria");
        #endif
        return;
    }
    v->data=newdata;
    v->capacity= size;
}

void dynvec_push(dynvec *v, int item){
    if (!v){return;}
    if (v->length==v->capacity){
        dynvec_resize (v, v->capacity ==0 ? 1: v->capacity *2);
    }
    v->data[v->length]=item;
    v->length++;
}

  
int dynvec_get(dynvec *v, size_t index){
    if (!v){return -1;}
    if(index >= v->length){
        errno = ERANGE;
        perror ("falha ao encontrar index");
        return -1;
    }
    return v->data[index];
}

int dynvec_top(dynvec *v){

    if (!v){return -1;}
    if (v->length ==0){return -1;}
    return v->data[v->length-1];
}

int dynvec_pop(dynvec *v){
    if (!v || v->length == 0){ return -1; }

    int valor = v->data[v->length - 1];
    v->length--;
    if (v->capacity > Dynvec_init_capacity && v->length <= v->capacity / 4) {
        dynvec_resize(v, v->capacity / 2);
    }

    return valor;
}

size_t dynvec_length(dynvec *v){
    if (!v){return 0;}
    return v->length;
}

void dynvec_set(dynvec *v, size_t indice, int item){
    if (!v){return;}
    if (indice >= v->length){
        errno=ERANGE;
        perror("erro ao colocar no indice");
        return;
    }
    v->data[indice]=item;
}

int main(){
    dynvec torre;
    dynvec pilha;
    dynvec resultado;

    dynvec_create(&torre);
    dynvec_create(&pilha);
    dynvec_create(&resultado);

    int altura;

    while(scanf("%d", &altura) == 1){
        dynvec_push(&torre, altura);
        dynvec_push(&resultado, 0);
    }

for (size_t i = 0; i < dynvec_length(&torre); i++){
        int hi = dynvec_get(&torre, i);

        while (dynvec_length(&pilha) > 0){
            int topo_indice = dynvec_top(&pilha);
            int topo_altura = dynvec_get(&torre, topo_indice);
            
            int minimo = (hi < topo_altura) ? hi : topo_altura;
            dynvec_set(&resultado, i, dynvec_get(&resultado, i) + minimo);
            dynvec_set(&resultado, topo_indice, dynvec_get(&resultado, topo_indice) + minimo);

            if (hi > topo_altura){
                dynvec_pop(&pilha);
            } else if (hi == topo_altura) {
                dynvec_pop(&pilha);
                break;
            } else {
                break;
            }
        }
        dynvec_push(&pilha, i);
    }

    
    for (size_t i = 0; i < dynvec_length(&resultado); i++){
        printf("%d\n", dynvec_get(&resultado, i));
    }
    dynvec_empty(&torre);
    dynvec_empty(&pilha);
    dynvec_empty(&resultado);
    return 0;
}