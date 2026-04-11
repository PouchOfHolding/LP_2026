#include <stdio.h>
#include <stddef.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#define Dynvec_init_capacity 4

typedef struct {
    int x;
    int y;
}Ponto;

typedef struct Dynvec{
    Ponto *data;
    size_t length;
    size_t capacity;
}dynvec;

void dynvec_create(dynvec *v) {
    v->capacity =Dynvec_init_capacity;
    v->length=0;
    v->data= (Ponto*)malloc(sizeof(Ponto)*v->capacity);
}

void dynvec_empty(dynvec *v) {
    /*if (!v){return;}*/
    free (v->data);
    v->data = NULL;
    v->length =0;
    v->capacity =0;
}

void dynvec_resize (dynvec *v, int size){
    /*if (!v){return;}*/
    Ponto *newdata = realloc(v->data, size *sizeof(Ponto));
    /*if (!newdata){
        errno=ENOMEM;
        #ifdef DEBUG_ON
        perror ("create. erro de alocação de memoria");
        #endif
        return;
    }*/
    v->data=newdata;
    v->capacity= size;
}

void dynvec_push(dynvec *v, Ponto item){
    if (v->capacity == v->length){
        dynvec_resize (v, v->capacity ==0 ? 1: v->capacity *2);
    }
    v->data[v->length]=item;
    v->length++;
}

Ponto dynvec_get(dynvec *v, size_t index){
    /*if (!v){return;}
    if(index >= v->length){
        errno = ERANGE;
        perror ("falha ao encontrar index");
        return;
    }*/
    return v->data[index];
}
Ponto dynvec_pop(dynvec *v){
    /*if (!v || v->length == 0){ return; }*/

    Ponto valor = v->data[v->length - 1];
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

void dynvec_set(dynvec *v, size_t indice, Ponto item){
    /*if (!v){return;}
    if (indice >= v->length){
        errno=ERANGE;
        perror("erro ao colocar no indice");
        return;
    }*/
    v->data[indice]=item;
}

//det(A, B, P) = (xB − xA)(yP − yA) − (yB − yA)(xP − xA)
//det(A, B, P) > 0: P está estritamente à esquerda de (A, B)
//det(A, B, P) < 0: P está estritamente à direita de (A, B)
//det(A, B, P) = 0: P é colinear com (A, B)

//(xB-xA) e (yB-yA):Representam a direção e inclinação da reta que une os sensores A e B

long long det(Ponto a, Ponto b, Ponto p){
    long long multi1 = (long long)(b.x-a.x) * (p.y-a.y);
    long long multi2 = (long long)(b.y - a.y) * (p.x-a.x);
    return multi1-multi2;
}
//pag 76

void swap(Ponto *a, int i, int j){
    Ponto temp = a[i];
    a[i] = a[j];
    a[j] = temp;
}

int partition(Ponto *a, int l, int r){
    Ponto p = a[l];
    int m=l;
    for (int i = l + 1; i < r; i++){
        if (a[i].x < p.x || (a[i].x == p.x && a[i].y < p.y)) {
            m++;
            swap(a, i, m);
        }
    }
    swap(a, l, m);
    return m;
}
void quickrec (Ponto *a, int l, int r){
    if (r -l <=1) return; // no maximo, um elemento
    int m = partition (a,l,r);
    quickrec(a,l,m);
    quickrec(a,m+1,r);
}

void quicksort (Ponto *a, int t){
    quickrec(a,0,t);
}

dynvec filtrar_esq(Ponto a, Ponto b, dynvec *sensores){
    dynvec esq;
    dynvec_create(&esq);
    for (size_t i = 0; i < sensores->length; i++){
        if (det(a, b, sensores->data[i]) > 0) {
            dynvec_push(&esq, sensores->data[i]);
        }
    }
    return esq;
}

int encontrar_det(Ponto a, Ponto b, dynvec *possivel){
    int indice_maior = 0;
    long long maior_d = det(a, b, possivel->data[0]);

    for (size_t i = 1; i < possivel->length; i++){
        long long d = det(a, b, possivel->data[i]);
        if (d > maior_d) {
            maior_d = d;
            indice_maior = i;
        } else if (d == maior_d) {
            if (possivel->data[i].x < possivel->data[indice_maior].x || 
               (possivel->data[i].x == possivel->data[indice_maior].x && 
                possivel->data[i].y < possivel->data[indice_maior].y)){
                indice_maior = i;
            }
        }
    }
    return indice_maior;
}

void recursiva(Ponto a, Ponto b, dynvec *possivel, double *perimetro_total, dynvec *arestas_confirmadas) {

    if (possivel->length == 0){
        dynvec_push(arestas_confirmadas, a);
        dynvec_push(arestas_confirmadas, b);
        long long dx = b.x - a.x;
        long long dy = b.y - a.y;
        *perimetro_total += sqrt((double)(dx * dx + dy * dy));
        dynvec_empty(possivel);
        return;
    }

    int idx_p = encontrar_det(a, b, possivel);
    Ponto p = possivel->data[idx_p];
    dynvec esquerda_AP = filtrar_esq(a, p, possivel);
    dynvec esquerda_PB = filtrar_esq(p, b, possivel);

    dynvec_empty(possivel);
    recursiva(a, p, &esquerda_AP, perimetro_total, arestas_confirmadas);
    recursiva(p, b, &esquerda_PB, perimetro_total, arestas_confirmadas);
}

int main(){
    dynvec sensores;
    dynvec_create(&sensores);

    Ponto p_temp;
    while (scanf("%d %d", &p_temp.x, &p_temp.y) == 2){
        dynvec_push(&sensores, p_temp);
    }

    if (sensores.length > 0){
        quicksort(sensores.data, (int)sensores.length);
    }

    for (size_t i = 0; i < sensores.length; i++){
        printf("%d %d\n", sensores.data[i].x, sensores.data[i].y);
    }

    if (sensores.length >= 2){
        Ponto min = sensores.data[0];
        Ponto max = sensores.data[sensores.length - 1];
        
        double perimetro = 0;
        dynvec arestas; 
        dynvec_create(&arestas);

        dynvec sup = filtrar_esq(min, max, &sensores);
        recursiva(min, max, &sup, &perimetro, &arestas);

        dynvec inf = filtrar_esq(max, min, &sensores);
        recursiva(max, min, &inf, &perimetro, &arestas);

        printf("%d\n", (int)(arestas.length / 2));
        for (size_t i = 0; i < arestas.length; i += 2) {
            Ponto p1 = arestas.data[i];
            Ponto p2 = arestas.data[i+1];
            printf("%d %d %d %d\n", p1.x, p1.y, p2.x, p2.y);
        }

        printf("%.2f\n", perimetro);
        
        dynvec_empty(&arestas);
    }

    dynvec_empty(&sensores);
    return 0;
}