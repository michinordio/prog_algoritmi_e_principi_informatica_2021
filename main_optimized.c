#include<stdio.h>
#include<time.h>
#include <stdlib.h>

#define cleard d[0] = 0;d[1] = 0;d[2] = 0;d[3] = 0;d[4] = 0;d[5] = 0;d[6] = 0;d[7] = 0;d[8] = 0;d[9] = 0;
// dijkstra priority queue node struct
typedef struct pqueue_node {
    __uint32_t w;
    __uint8_t visited;
    __uint16_t key;
} pqueue_node;
// graph topK ranking node struct
typedef struct topk_node {
    __uint32_t key;
    __uint64_t w;
    struct topk_node *next;
} topk_node;
//graph topK ranking head
typedef struct topk_head {
    int n;
    topk_node *topk_list;
    int k;
} topk_head;

// fast atoi declaration, to be inlined by gcc, callable function won't be compiled
inline static u_int32_t fastAtoi(const char *str)__attribute__((always_inline));

// atoi conversion using binary shifting
u_int32_t fastAtoi(const char *str) {
    u_int32_t val = 0;

    while (*str) val = (val << 3) + (val << 1) + (*str++ - '0');
    return val;
}

//inserisce in *indmin topk_head'indice del pqueue_node non visitato con peso minore
int sortMin(pqueue_node *q, int dim) {
    u_int8_t flag = 0;
    u_int32_t min_index = 0;

    for (int j = 1; j < dim; j++) {
        if (q[j].w != 0) {
            if (min_index == 0 && q[j].visited == 0) {
                min_index = j;
                flag = 1;
            }
            if ((q[j].w < q[min_index].w) && (q[j].visited == 0) && (q[j].w != 0)) {
                min_index = j;
                flag = 1;
            }
        }
    }
    if (!flag) {
        flag = 0;
        for (int j = 1; j < dim && flag == 0; j++) {
            if (q[j].w == 0 && q[j].visited == 0) {
                min_index = j;
                flag = 1;
            }
        }
    }
    return min_index;
}

// insert a graph in topk ranking
void insertTopK(u_int64_t pathlength, topk_node **graph, topk_head *head) {
    topk_node *max;
    max = head->topk_list;
    if (head->n < head->k) {
        (*graph)->w = pathlength;
        (*graph)->key = head->n;

        (*graph)->next = malloc(sizeof(topk_node));
        *graph = (*graph)->next;
    } else {
        *graph = head->topk_list;
        while ((*graph) != NULL) {
            if ((*graph)->w > max->w) {
                max = *graph;
            }
            *graph = (*graph)->next;
        }
        if (pathlength < max->w) {
            max->w = pathlength;
            max->key = head->n;
        }
    }
}

// print current topK ranking
void printTopk(topk_head *p) {
    //printf("\noutput topK: ");
    topk_node *lista_mat = p->topk_list;
    int n = p->k;
    if (p->n < n) n = p->n;
    if (lista_mat != NULL) {
        printf("%d", lista_mat->key);
        lista_mat = lista_mat->next;
        for (int i = 1; i < n; i++) {
            printf(" %d", lista_mat->key);
            lista_mat = lista_mat->next;
        }
    }
    printf("\n");
}

u_int64_t dijkstra(u_int32_t dim, u_int32_t m[dim][dim]) {
    u_int64_t pathsum = 0;
    u_int32_t min_index = 0, countvisited = dim;

//prio_queue allocation
    pqueue_node *q;
    q = malloc((dim) * sizeof(pqueue_node));
    q[0].w = 0;
    q[0].visited = 1;
    q[0].key = 0;

//m first row in prio_queue
    for (int i = 0; i < dim; i++) {
        q[i].w = m[0][i];
        q[i].visited = 0;
        q[i].key = i;
    }
    while (countvisited != 0) {
        min_index = sortMin(q, dim);
        //scorro riga min_index con cursore j (considero quindi i nodi adiacenti al pqueue_node min_index)
        for (int j = 1; j < dim; j++) {
            //sostituisci se (peso_j>arco(min_index,j)+peso_indmin VEL peso_j==0) E arco!=0 E j!=min_index (no autoanello)
            if ((q[j].w > m[min_index][j] + q[min_index].w || (q[j].w == 0 && q[j].key != 0)) && m[min_index][j] != 0 &&
                j != min_index && q[j].visited == 0) {
                q[j].w = m[min_index][j] + q[min_index].w;
            }
        }
        //marco pqueue_node min_index come visitato
        q[min_index].visited = 1;
        //diminuisco contatore nodi da verificare; devo cambiarlo è una stronzata ma vbb
        countvisited--;
    }
    for (int k = 0; k < dim; k++)pathsum = pathsum + q[k].w;
    return pathsum;
}

int main() {
    clock_t time = clock();
    u_int32_t dim = 0, k = 0;
    int i = 0, j = 0;
    char d[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    char f, t;
//prendo in ingresso dim
    f = getchar_unlocked();
    while (f != ' ') {
        d[i] = f;
        f = getchar_unlocked();
        i++;
    }
    dim = fastAtoi(d);
//prendo in ingresso k
    cleard
    i = 0;
    f = getchar_unlocked();
    while (f != '\n') {
        d[i] = f;
        f = getchar_unlocked();
        i++;
    }
    k = fastAtoi(d);
//topk allocation
    topk_head p;
    topk_node *lista_mat;
    lista_mat = malloc(sizeof(topk_node));
    p.topk_list = lista_mat;
    p.k = k;
    p.n = 0;
//graph cache allocation
    u_int32_t m[dim][dim];
//file parsing
    f = getchar_unlocked();
    while (f != EOF) {
        // AggiungiGrafo command handler
        if (f == 'A') {
            fseek(stdin, 13, SEEK_CUR);
            cleard
            for (i = 0; i < dim; i++) {
                for (j = 0; j < dim; j++) {
                    u_int8_t flag = 0;
                    for (int c = 0; c < 10 && flag == 0; c++) {
                        t = getchar_unlocked();
                        if (t != ',' && t != '\n' && t != EOF) d[c] = t;
                        else flag = 1;
                    }
                    if (i != j) m[i][j] = fastAtoi(d);
                    else m[i][j] = 0;
                    cleard
                }
            }
            //exec dijkstra and store result in topk_list
            insertPath(dijkstra(dim, m), &lista_mat, &p);
            p.n++;
        }
            //TopK command handler
        else if (f == 'T') {
            if (p.n != 0) {
                printTopk(&p);
            } else printf("\n");
            //printf("exec time: %f s\n", (float)(clock()-time)/CLOCKS_PER_SEC);
            fseek(stdin, 4, SEEK_CUR);
        }
        f = getchar_unlocked();
    }
    printf("exec time: %f s\n", (float) (clock() - time) / CLOCKS_PER_SEC);   //stampa pathsum e tempo impiegato
    return 0;
}