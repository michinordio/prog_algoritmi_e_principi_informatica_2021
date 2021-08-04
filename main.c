#include<stdio.h>
//#include<time.h>
#include <stdlib.h>
typedef struct nodo{
    __uint32_t w;
    __uint8_t visited;
    __uint32_t key;
} nodo_t;
typedef struct matrice{
    __uint32_t key;
    __uint64_t w;
    struct matrice *next;
} mat;
typedef struct l{
    __uint32_t n;
    mat* head;
    __uint32_t k;
} lista;

inline static __uint32_t convert(char s[], __uint8_t dim)__attribute__((always_inline));

__uint32_t convert(char s[], __uint8_t dim)
{
    __uint32_t n=0;
    for(__uint8_t i=0; i<dim && s[i]!=0; i++)
    {
        n=(n<<1)+(n<<3)+((int)s[i]-48);
    }
    return n;
}
//inserisce in *indmin l'indice del nodo non visitato con peso minore
__uint32_t sortMin(nodo_t *q, __uint32_t dim){
    __uint8_t flag=0;
    __uint32_t indmin=0;
    for(__uint32_t j=1; j<dim; j++)
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
        for(__uint32_t j=1; j<dim && flag==0; j++)
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

        while((*lista_mat)!=NULL)
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
    __uint32_t n = p->k;
    if (p->n < n) n = p->n;
    if (lista_mat != NULL){
        printf("%d", lista_mat->key);
        lista_mat=lista_mat->next;
    }
    for(__uint32_t i=1; i<n; i++)
    {
        printf(" %d", lista_mat->key);
        lista_mat=lista_mat->next;
    }
    printf("\n");
}

__uint64_t dijkstra(__uint32_t dim, __uint32_t m[dim][dim]){
    __uint64_t pathsum=0;
    __uint32_t indmin=0, countvisited=dim;
    nodo_t *q;
    __uint32_t i, j;

    q = malloc((dim)*sizeof(nodo_t));          //alloco spazio per la coda
    q[0].w=0; q[0].visited=1; q[0].key=0;      //inizializzo nodo 0 in coda
    for(i=0; i<dim; i++)                    //inizializzo altri nodi in coda inserendo la prima riga della matrice
        {
        q[i].w=m[0][i];
        q[i].visited=0;
        q[i].key=i;
        }
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

    }
    for(__uint32_t k=0; k<dim; k++)pathsum=pathsum+q[k].w;             //sommo tutti i pesi finali
    //printf("pathsum di questa matrice: %lu\n", pathsum);
    return pathsum;
}

int main()
{
    __uint32_t dim, k;
    __uint32_t i, j;
    __uint8_t c=0;
    char d[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    char f, t;
    //input dim
    do {
        d[c] = getchar_unlocked();
        c++;
    } while (d[c - 1] != ' ');
    dim = convert(d, c-1);
    //prendo in ingresso k
    d[0]=0; d[1]=0; d[2]=0; d[3]=0; d[4]=0; d[5]=0; d[6]=0; d[7]=0; d[8]=0; d[9]=0;
    c = 0;
    do {
        d[c] = getchar_unlocked();
        c++;
    } while (d[c - 1] != '\n');
    k = convert(d, c-1);

    mat* lista_mat;
    lista_mat=malloc(sizeof(mat));

    __uint32_t m[dim][dim];

    lista p;
    p.head=lista_mat;
    p.k=k;
    p.n=0;

    f = getchar_unlocked();
    //input da file
    while (f != EOF){
        if (f == 'A'){
            fseek(stdin, 13, SEEK_CUR);
            d[0]=0; d[1]=0; d[2]=0; d[3]=0; d[4]=0; d[5]=0; d[6]=0; d[7]=0; d[8]=0; d[9]=0;
            for(i=0; i<dim; i++)
            {
                for(j=0; j<dim; j++)
                {
                    __uint8_t flag=0;
                    for(c=0; c<10 && flag==0; c++) {
                        t = getchar_unlocked();
                        if (t != ',' && t != '\n' && t != EOF) d[c] = t;
                        else flag=1;
                    }
                    if(i!=j) m[i][j]=convert(d,c-1);
                    else m[i][j]=0;
                    d[0]=0; d[1]=0; d[2]=0; d[3]=0; d[4]=0; d[5]=0; d[6]=0; d[7]=0; d[8]=0; d[9]=0;
                }
            }
            insertPath(dijkstra(dim, m),&lista_mat,&p);
            p.n++;
        }
        else if (f == 'T'){
            if(p.n!=0){
                printTopk(&p);
            }
            else printf("\n");
            fseek(stdin, 4, SEEK_CUR);
        }
        f = getchar_unlocked();
    }

    //printf("\n\ntime: %f\n", (float) (clock()-time)/CLOCKS_PER_SEC);   //stampa pathsum e tempo impiegato
    return 0;
}