#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
} ListaMeta;

typedef struct Dynvec {
    ListaMeta *data;
    size_t length;
    size_t capacity;
} dynvec;

ListaMeta* find_list(dynvec *v, int id) {
    for (size_t i = 0; i < v->length; i++) {
        if (v->data[i].id == id) {
            return &(v->data[i]);
        }
    }
    return NULL;   
}

void dynvec_create(dynvec *v) {
    v->capacity = Dynvec_init_capacity;
    v->length = 0;
    v->data = (ListaMeta*)malloc(sizeof(ListaMeta) * v->capacity);
}

void dynvec_resize(dynvec *v, int size) {
    ListaMeta *newdata = realloc(v->data, size * sizeof(ListaMeta));
    if (newdata == NULL) return;
    v->data = newdata;
    v->capacity = size;
}

void dynvec_push(dynvec *v, ListaMeta item) {
    if (v->capacity == v->length) {
        dynvec_resize(v, v->capacity == 0 ? 1 : v->capacity * 2);
    }
    v->data[v->length] = item;
    v->length++;
}

int ja_libertado(node** visitados, int tam, node* alvo) {
    for(int i = 0; i < tam; i++) {
        if(visitados[i] == alvo) return 1;
    }
    return 0;
}

void dynvec_empty(dynvec *v) {
    if (v->data != NULL) {
        node** libertados = NULL;
        int tam_libertados = 0;

        for (size_t i = 0; i < v->length; i++) {
            node* curr = v->data[i].head;
            int count = 0; 
            while (curr != NULL && count < 10000) { 
                if (ja_libertado(libertados, tam_libertados, curr)) {
                    break;
                }
                node* next = curr->next;

                libertados = realloc(libertados, sizeof(node*) * (tam_libertados + 1));
                libertados[tam_libertados++] = curr;

                free(curr->content);
                free(curr);

                curr = next;
                count++;
            }
        }
        free(libertados);
        free(v->data);
    }
    v->data = NULL;
    v->length = 0;
    v->capacity = 0;
}

node* create_node(int valor) {
    node* new_n = (node*)malloc(sizeof(node));
    if (new_n == NULL) { return NULL; }
    
    int* content_ptr = (int*)malloc(sizeof(int));
    if (content_ptr == NULL) {
        free(new_n);
        return NULL;
    }

    *content_ptr = valor;
    new_n->content = content_ptr;
    new_n->next = NULL;
    return new_n;
}

void create(dynvec *v, int id) {
    if (find_list(v, id) != NULL) { return; }
    ListaMeta nova;
    nova.id = id;
    nova.head = NULL;
    nova.tail = NULL;
    nova.current = NULL;
    dynvec_push(v, nova);
}

void addhead(dynvec *v, int id, int valor) {
    ListaMeta* Lista = find_list(v, id);
    if (Lista == NULL) { return; }

    node* new_n = create_node(valor);
    if (new_n == NULL) { return; }

    if (Lista->head == NULL) {
        Lista->head = new_n;
        Lista->tail = new_n;
        Lista->current = new_n;
    } else {
        new_n->next = Lista->head;
        Lista->head = new_n;
        Lista->current = Lista->head; 
    }
}

void addtail(dynvec *v, int id, int valor) {
    ListaMeta* Lista = find_list(v, id);
    if (Lista == NULL) { return; }

    node* new_n = create_node(valor);
    if (new_n == NULL) { return; }

    if (Lista->head == NULL) {
        Lista->head = new_n;
        Lista->tail = new_n;
        Lista->current = new_n;
    } else {
        Lista->tail->next = new_n;
        Lista->tail = new_n;
    }
}

void add(dynvec *v, int id, int i, int n) {
    ListaMeta* Lista = find_list(v, id);
    if (Lista == NULL) { return; }
    
    if (i <= 0) {
        addhead(v, id, n);
        return;
    }
    
    if (Lista->head == NULL) {
        addhead(v, id, n);
        return;
    }

    node* temp = Lista->head;
    int pos = 0;

    while (pos < i - 1 && temp->next != NULL) {
        temp = temp->next;
        pos++;
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

void delhead(dynvec *v, int id) {
    ListaMeta* L = find_list(v, id);
    if (L == NULL || L->head == NULL) return;

    node* to_remove = L->head;

    if (L->head == L->tail) {
        L->head = NULL;
        L->tail = NULL;
        L->current = NULL;
    } else {
        L->head = to_remove->next;
        L->current = L->head; 
    }

    free(to_remove->content);
    free(to_remove);
}

void deltail(dynvec *v, int id) {
    ListaMeta* L = find_list(v, id);
    if (L == NULL || L->head == NULL) return;

    node* to_remove = L->tail;

    if (L->head == L->tail) {
        L->head = NULL;
        L->tail = NULL;
        L->current = NULL;
    } else {
        node* temp = L->head;
        while (temp->next != L->tail) {
            temp = temp->next;
        }
        temp->next = NULL;
        L->tail = temp;
        L->current = L->head; 
    }
    free(to_remove->content);
    free(to_remove);
}

void del(dynvec *v, int id, int i) {
    ListaMeta* L = find_list(v, id);
    if (L == NULL || L->head == NULL) return;
    
    if (i <= 0) {
        delhead(v, id);
        return;
    }

    node* prev = L->head;
    int pos = 0;

    while (pos < i - 1 && prev->next != NULL) {
        prev = prev->next;
        pos++;
    }

    if (prev->next == NULL) return;

    node* to_remove = prev->next;
    
    if (to_remove == L->tail) {
        deltail(v, id);
        return;
    }

    prev->next = to_remove->next;
    L->current = L->head; 

    free(to_remove->content);
    free(to_remove);
}

void print(dynvec *v, int id) {
    ListaMeta* L = find_list(v, id);
    if (L == NULL || L->current == NULL) {
        printf("NULL\n");
        return;
    }
    int valor = *(int*)(L->current->content);
    printf("%d\n", valor);
    L->current = L->current->next;
}

void restart(dynvec *v, int id) {
    ListaMeta* L = find_list(v, id);
    if (L == NULL) return;
    L->current = L->head;
}

void find(dynvec *v, int id, int n) {
    ListaMeta* L = find_list(v, id);
    if (L == NULL || L->head == NULL) {
        printf("No\n");
        return;
    }

    node* temp = L->head;
    int count = 0;
    while (temp != NULL && count < 10000) {
        if (*(int*)(temp->content) == n) {
            printf("Yes\n");
            return;
        }
        temp = temp->next;
        count++;
    }
    printf("No\n");
}

void len(dynvec *v, int id) {
    ListaMeta* L = find_list(v, id);
    if (L == NULL || L->head == NULL) {
        printf("0\n");
        return;
    }

    int count = 0;
    node* temp = L->head;
    while (temp != NULL && count < 10000) {
        count++;
        if (temp == L->tail && L->tail->next != NULL) {
            break;
        }
        temp = temp->next;
    }
    printf("%d\n", count);
}

void concat(dynvec *v, int id1, int id2) {
    ListaMeta* L1 = find_list(v, id1);
    ListaMeta* L2 = find_list(v, id2);

    if (L1 == NULL || L2 == NULL || L2->head == NULL)
        return;

    if (L1->head == NULL) {
        L1->head = L2->head;
        L1->tail = L2->tail;
        L1->current = L2->head;
    } else {
        L1->tail->next = L2->head;
        L1->tail = L2->tail;
    }
}

void cycle(dynvec *v, int id) {
    ListaMeta* L = find_list(v, id);
    if (L == NULL || L->head == NULL) {
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

int main() {
    dynvec v;
    dynvec_create(&v);

    char comando[20];
    int id, id2, i, n;

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
    }
    
    dynvec_empty(&v);
    return 0;
}