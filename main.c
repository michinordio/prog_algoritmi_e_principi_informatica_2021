#include<stdio.h>
#include<time.h>
#include <stdlib.h>
typedef struct nodo{
    __uint32_t w;
    __uint8_t visited;
    __uint16_t key;
} nodo_t;
typedef struct matrice{
    __uint32_t key;
    __uint64_t w;
    struct matrice *next;
} mat;
typedef struct l{
    int n;
    mat* head;
    int k;
} lista;

//inserisce in *indmin l'indice del nodo non visitato con peso minore
int sortMin(nodo_t *q, int dim){
    __uint8_t flag=0;
    int indmin=0;
    for(int j=1; j<dim; j++)
    {
        if(q[j].w != 0) {
            if (indmin == 0 && q[j].visited == 0){
                indmin = j;
                flag = 1;
            }
            if ((q[j].w < q[indmin].w) && (q[j].visited == 0) && (q[j].w != 0)) {
                indmin = j;
                flag = 1;
            }
        }
    }
    if(!flag)
    {
        flag = 0;
        for(int j=1; j<dim && flag==0; j++)
        {
            if(q[j].w == 0 && q[j].visited == 0)
            {
                indmin=j;
                flag=1;
            }
        }
    }
    //se no mi andava bene l'indirizzo nuovo e lo metto in *indmin
    //stampo il nodo che dijkstra sta per visitare (debug)
    //printf("nodo considerato: %d        ", indmin);
    return indmin;
}

void insertPath(__uint64_t pathlength, mat **lista_mat, lista *p){
    mat* max;
    max=p->head;
    if(p->n < p->k)
    {
        (*lista_mat)->w=pathlength;
        (*lista_mat)->key=p->n;

        (*lista_mat)->next=malloc(sizeof(mat));
        *lista_mat=(*lista_mat)->next;
    }
    else
    {
        *lista_mat=p->head;

        while((*lista_mat)!=NULL) //oppure for
        {
            if((*lista_mat)->w > max->w)
            {
                max=*lista_mat;
            }
            *lista_mat=(*lista_mat)->next;
        }
        if(pathlength < max->w){
            max->w=pathlength;
            max->key=p->n;
        }
    }
}

void printTopk(lista *p){
    //printf("\noutput topK: ");
    mat* lista_mat = p->head;
    int n = p->k;
    if (p->n < n) n = p->n;
    if (lista_mat != NULL){
        printf("%d", lista_mat->key);
        lista_mat=lista_mat->next;
    }
    for(int i=1; i<n; i++)
    {
        printf(" %d", lista_mat->key);
        lista_mat=lista_mat->next;
    }
    printf("\n");
}

__uint64_t dijkstra(int dim, __uint32_t m[dim][dim]){
    __uint64_t pathsum=0;
    int indmin=0, countvisited=dim;
    nodo_t *q;

    q = malloc((dim)*sizeof(nodo_t));          //alloco spazio per la coda
    q[0].w=0; q[0].visited=1; q[0].key=0;      //inizializzo nodo 0 in coda
    for(int i=0; i<dim; i++)                    //inizializzo altri nodi in coda inserendo la prima riga della matrice
    {
        q[i].w=m[0][i];
        q[i].visited=0;
        q[i].key=i;
    }
    int j;
    while(countvisited!=0)
    {
        indmin = sortMin(q,dim);             //funzione che salva in indmin l'indice del nodo con peso minore
        for(j=1; j<dim; j++)                //scorro riga indmin con cursore j (considero quindi i nodi adiacenti al nodo indmin)
        {
            if((q[j].w>m[indmin][j]+q[indmin].w || (q[j].w==0 && q[j].key!=0)) && m[indmin][j]!=0 && j!=indmin && q[j].visited==0)     // sostituisci se (peso_j>arco(indmin,j)+peso_indmin VEL peso_j==0) E arco!=0 E j!=indmin (no autoanello)
                q[j].w=m[indmin][j]+q[indmin].w;
        }
        q[indmin].visited=1;                    //marco nodo indmin come visitato
        countvisited--;                         //diminuisco contatore nodi da verificare; devo cambiarlo è una stronzata ma vbb
        //for(int k=0; k<dim; k++) {printf("%d ", q[k].w);}       //stampo i pesi dei nodi alla fine di questo ciclo

    }
    for(int k=0; k<dim; k++)pathsum=pathsum+q[k].w;             //sommo tutti i pesi finali
    //printf("pathsum di questa matrice: %lu\n", pathsum);
    return pathsum;
}

int main()
{
    clock_t time = clock();
    int dim = 0, k = 0;
    __uint64_t path=0; path++; //da eliminare
    int i = 0, j=0;
    //prendo in ingresso dim
    char d[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    char f, t;

    do {
        d[i] = getchar();
        i++;
    } while (d[i - 1] != ' ');
    dim = atoi(d);
    //prendo in ingresso k
    for (i = 0; i < 10; i++)d[i] = 0;
    i = 0;
    do {
        d[i] = getchar();
        i++;
    } while (d[i - 1] != '\n');
    k = atoi(d);

    mat* lista_mat;
    lista_mat=malloc(sizeof(mat));

    __uint32_t m[dim][dim];

    lista p;
    p.head=lista_mat;
    p.k=k;
    p.n=0;



    f = getchar();
    //input da file
    while (f != EOF){
        if (f == 'A'){
            fseek(stdin, 13, SEEK_CUR);
            for (i = 0; i < 10; i++)d[i] = 0;
            for(i=0; i<dim; i++)
            {
                for(j=0; j<dim; j++)
                {
                    __uint8_t flag=0;
                    for(int c=0; c<10 && flag==0; c++) {
                        t = getchar();
                        if (t != ',' && t != '\n' && t != EOF) d[c] = t;
                        else flag=1;
                    }
                    if(i!=j) m[i][j]=atoi(d);
                    else m[i][j]=0;
                    for (int l = 0; l< 10; l++)d[l] = 0;
                }
            }
            insertPath(dijkstra(dim, m),&lista_mat,&p);
            p.n++;
        } else if (f == 'T') {
            if (p.n != 0) {
                printTopk(&p);
            } else printf("\n");
            fseek(stdin, 4, SEEK_CUR);
        }
        f = getchar();
    }

    printf("exec time: %f s\n", (float) (clock() - time) / CLOCKS_PER_SEC);   //stampa pathsum e tempo impiegato
    return 0;
}