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
    if (newdata != NULL || size == 0) {
        v->data=newdata;
        v->capacity= size;
    }
}

void dynvec_push(dynvec *v, ListaMeta item){
    if (v->capacity == v->length){
        dynvec_resize (v, v->capacity ==0 ? 1: v->capacity *2);
    }
    v->data[v->length]=item;
    v->length++;
}

void dynvec_empty(dynvec *v){
    if (v->data != NULL) {
        for (size_t i = 0; i < v->length; i++) {
            node* curr = v->data[i].head;
            while (curr != NULL) {
                node* next_node = curr->next;
                free(curr->content);
                free(curr);
                curr = next_node;
            }
        }
        free (v->data);
    }
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

    if (temp == NULL) {
        return; 
    }
    
    if (temp == Lista->tail) {
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

    if (!L || !L->head) {return;} 

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
        if (L->current == to_remove) L->current = L->head;
    }
    free(to_remove->content);
    free(to_remove);
}

void delhead(dynvec *v, int id) {
    ListaMeta* L = find_list(v, id);
    if (!L || !L->head) return; 

    node* to_remove = L->head;
    L->head = to_remove->next;
    
    if (L->head == NULL) {L->tail = NULL;}
 
    if (L->current == to_remove) {L->current = L->head;}

    free(to_remove->content);
    free(to_remove);
}

void del(dynvec *v, int id, int i) {
    ListaMeta* L = find_list(v, id);

    if (!L || !L->head) return; 
    if (i <= 0) {
        delhead(v, id);
        return;
    }

    node* prev = L->head;
    int pos = 0;

    while (prev->next && pos < i - 1) {
        prev = prev->next;
        pos++;
    }

    node* to_remove = prev->next;
    
    if (!to_remove) return; 
    prev->next = to_remove->next;

    if (to_remove == L->tail) L->tail = prev;

    if (to_remove == L->current) L->current = L->head; 

    free(to_remove->content);
    free(to_remove);
}

void print(dynvec *v, int id) {
    ListaMeta* L = find_list(v, id);
    
    if (!L || !L->current) {
        printf("NULL\n");
        return;
    }
    int valor = *(int*)(L->current->content);
    printf("%d\n", valor);
    L->current = L->current->next;
}

void restart(dynvec *v, int id) {
    ListaMeta* L = find_list(v, id);
    if (!L) return; 
    L->current = L->head;
}

void find(dynvec *v, int id, int n) {
    ListaMeta* L = find_list(v, id);
    if (!L || !L->head) {
        printf("No\n");
        return;
    }

    node* temp = L->head;
    while (temp != NULL) {
        if (*(int*)(temp->content) == n) {
            printf("Yes\n");
            return; 
        }
        temp = temp->next;
    }

    printf("No\n");
}

void len(dynvec *v, int id) {
    ListaMeta* L = find_list(v, id);
    
    if (!L || !L->head) {
        printf("0\n");
        return;
    }

    int count = 0;
    node* temp = L->head;

    while (temp != NULL) {
        count++;
        temp = temp->next;
    }

    printf("%d\n", count);
}

void concat(dynvec *v, int id1, int id2) {
    ListaMeta* L1 = find_list(v, id1);
    ListaMeta* L2 = find_list(v, id2);

    if (!L1 || !L2 || !L2->head) return;

    if (!L1->head) {
        L1->head = L2->head;
        L1->tail = L2->tail;
    } else {
        L1->tail->next = L2->head;
        L1->tail = L2->tail;
    }

    L2->head = L2->tail = L2->current = NULL;
}

void cycle(dynvec *v, int id) {
    ListaMeta* L = find_list(v, id);
    
    if (!L || !L->head) {
        printf("No\n");
        return;
    }

    node* lento = L->head;
    node* rapido = L->head;

    while (rapido != NULL && rapido->next != NULL) {
        lento = lento->next;
        rapido = rapido->next->next;

        if (lento == rapido) {
            printf("Yes, %d\n", *(int*)(lento->content));
            return;
        }
    }

    printf("No\n");
}

// Função auxiliar para limpar caracteres de nova linha e espaços decorrentes do input
void consumir_espacos() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    dynvec v;
    dynvec_create(&v);

    char comando[20];
    int id, id2, i, n;

    // Loop principal: lê o comando até chegar ao fim do ficheiro (EOF)
    while (scanf("%19s", comando) != EOF) {
        if (strcmp(comando, "create") == 0) {
            scanf("%d", &id);
            create(&v, id);
        } 
        else if (strcmp(comando, "addhead") == 0) {
            scanf("%d %d", &id, &n);
            addhead(&v, id, n);
        } 
        else if (strcmp(comando, "addtail") == 0) {
            scanf("%d %d", &id, &n);
            addtail(&v, id, n);
        } 
        else if (strcmp(comando, "add") == 0) {
            scanf("%d %d %d", &id, &i, &n);
            add(&v, id, i, n);
        } 
        else if (strcmp(comando, "delhead") == 0) {
            scanf("%d", &id);
            delhead(&v, id);
        } 
        else if (strcmp(comando, "deltail") == 0) {
            scanf("%d", &id);
            deltail(&v, id);
        } 
        else if (strcmp(comando, "del") == 0) {
            scanf("%d %d", &id, &i);
            del(&v, id, i);
        } 
        else if (strcmp(comando, "print") == 0) {
            scanf("%d", &id);
            print(&v, id);
        } 
        else if (strcmp(comando, "restart") == 0) {
            scanf("%d", &id);
            restart(&v, id);
        } 
        else if (strcmp(comando, "find") == 0) {
            scanf("%d %d", &id, &n);
            find(&v, id, n);
        } 
        else if (strcmp(comando, "len") == 0) {
            scanf("%d", &id);
            len(&v, id);
        } 
        else if (strcmp(comando, "concat") == 0) {
            scanf("%d %d", &id, &id2);
            concat(&v, id, id2);
        } 
        else if (strcmp(comando, "cycle") == 0) {
            scanf("%d", &id);
            cycle(&v, id);
        }
        
        consumir_espacos();
    }

    // Nota 8: Antes de fechar o programa, libertamos toda a memória alocada
    dynvec_empty(&v);

    return 0;
}