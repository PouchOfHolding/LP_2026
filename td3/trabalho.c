#include <stdio.h>
#include <stddef.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#define Dynvec_init_capacity 4

typedef struct s_node {
void* content;
struct s_node* next;
} node;

typedef struct {
    int id;
    node* head;
    node* tail;
    node* current;
}ListaMeta;

typedef struct Dynvec{
    ListaMeta *data;
    size_t length;
    size_t capacity;
}dynvec;

ListaMeta* find_list (dynvec *v, int id){
    for (size_t i = 0; i < v->length; i++){
        if (v->data[i].id==id){
            return &(v->data[i]);
        }
    }
    return NULL;   
}

void dynvec_create(dynvec *v){
    v->capacity =Dynvec_init_capacity;
    v->length=0;
    v->data= (ListaMeta*)malloc(sizeof(ListaMeta)*v->capacity);
}

void dynvec_resize (dynvec *v, int size){
    ListaMeta *newdata = realloc(v->data, size *sizeof(ListaMeta));
    v->data=newdata;
    v->capacity= size;
}

void dynvec_push(dynvec *v, ListaMeta item){
    if (v->capacity == v->length){
        dynvec_resize (v, v->capacity ==0 ? 1: v->capacity *2);
    }
    v->data[v->length]=item;
    v->length++;
}

void dynvec_empty(dynvec *v){
    free (v->data);
    v->data = NULL;
    v->length =0;
    v->capacity =0;
}

ListaMeta dynvec_get(dynvec *v, size_t index){
    return v->data[index];
}

ListaMeta dynvec_pop(dynvec *v){
    ListaMeta valor = v->data[v->length - 1];
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

void dynvec_set(dynvec *v, size_t indice, ListaMeta item){
    v->data[indice]=item;
}


node* create_node (int valor){
    node* new_n =(node*)malloc(sizeof(node));
    if (new_n==NULL){return NULL;}
    
    int* content_ptr = (int*)malloc(sizeof(int));
    if (content_ptr == NULL){
        free (new_n);
        return NULL;
    }

    *content_ptr = valor;
    new_n->content = content_ptr;
    new_n->next=NULL;
    return new_n;
}

void create(dynvec *v, int id){
    if (find_list(v, id)!=NULL){return;}
    ListaMeta nova;
    nova.id=id;
    nova.head=NULL;
    nova.tail =NULL;
    nova.current=NULL;

    dynvec_push(v,nova);
}

void addtail(dynvec *v, int id, int valor){
    ListaMeta* Lista = find_list(v, id);
    if (Lista==NULL){return;}

    node* new_n=create_node(valor);
    if (new_n== NULL){return;}

    if (Lista->head==NULL){
        Lista->head = new_n;
        Lista->tail = new_n;
        Lista->current = new_n;
    }
    else{
        Lista->tail->next=new_n;
        Lista->tail = new_n;
    }
}

void addhead (dynvec *v, int id, int valor){
    ListaMeta* Lista =find_list(v, id);
    if (Lista == NULL){return;}

    node* new_n = create_node(valor);
    if (new_n == NULL){return;}

    if (Lista->head==NULL){
        Lista->head=Lista->tail=Lista->current=new_n;
    }
    else{
        new_n->next=Lista->head;
        Lista->head=new_n;
        Lista->current = new_n;
    }
}

void add(dynvec *v, int id, int i, int n) {

    ListaMeta* Lista = find_list(v, id);
    if (Lista == NULL) { return; }
    if (i <= 0) {
        addhead(v, id, n);
        return;
    }

    node* temp = Lista->head;
    int pos = 0;
    while (temp != NULL && pos < i - 1) {
        temp = temp->next;
        pos++;
    }

    if (temp == NULL || temp == Lista->tail) {
        addtail(v, id, n);
        return;
    }
    node* new_n = create_node(n);
    if (new_n == NULL) { return; }
    new_n->next = temp->next;
    temp->next = new_n;
}

void deltail(dynvec *v, int id) {
    ListaMeta* L = find_list(v, id);

    if (!L || !L->head) {return}; 

    node* to_remove = L->tail;

    if (L->head == L->tail) {
        L->head = L->tail = L->current = NULL;
    } 
    else {
        node* temp = L->head;
        while (temp->next != L->tail) {
            temp = temp->next;
        }
        
        L->tail = temp;
        L->tail->next = NULL; 
        
        if (L->current == to_remove) L->current = L->tail;
    }
    free(to_remove->content);
    free(to_remove);
}