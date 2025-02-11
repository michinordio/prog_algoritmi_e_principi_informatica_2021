#include<stdio.h>
#include<stdlib.h>


typedef struct {
    __uint64_t key;
    __uint64_t w;
} type_heap;

__uint64_t n=0, k=0, mat_index=0;

void topk(type_heap *heap)
{
    __uint64_t dim_heap= k>mat_index ? mat_index:k;
    for(__uint64_t i=0; i<dim_heap-1; i++)
    {
        printf("%ld ", heap[i].key);
    }
    printf("%ld\n", heap[dim_heap-1].key);
}

void max_bubble_up(type_heap *heap)
{
    __uint64_t index=mat_index, f_index;
    __uint8_t flag=0;
    type_heap temp;
    while(!flag){
        f_index=(index-1) >> 1;
        if(f_index==0) flag=1;
        if(heap[index].w>heap[f_index].w){
            temp=heap[index];           //temp <- figlio
            heap[index]=heap[f_index];  //figlio <- padre
            heap[f_index]=temp;         //padre <- temp
        }
        else flag=1;

        index=f_index;
    }
}

void max_bubble_down(type_heap *heap)
{
    __uint64_t f_index=0, sx_index, dx_index, max_ind;
    __uint8_t flag=0;
    type_heap temp;
    __uint64_t dim_heap=(mat_index+1)<k ? (mat_index+1):k;
    while(!flag)
    {
        sx_index=(f_index << 1)+1;
        dx_index=sx_index+1;
        max_ind=f_index;
        if(dx_index<dim_heap){     //entrambi i figli esistono

            if(heap[sx_index].w>heap[max_ind].w) max_ind=sx_index;
            if(heap[dx_index].w>heap[max_ind].w) max_ind=dx_index;
        }
        else if(sx_index<dim_heap){            //esiste solo il figlio sinistro
            if(heap[sx_index].w>heap[f_index].w) max_ind=sx_index;
        }
        else flag=1;            //non esistono figli

        if(max_ind==f_index) flag=1;
        else{
            temp=heap[f_index];   //temp <- padre
            heap[f_index]=heap[max_ind];
            heap[max_ind]=temp;
        }
        f_index=max_ind;
    }
}

void insert_pathsum(__uint64_t pathsum, type_heap *heap)
{
    __uint64_t dim_corrente=mat_index+1;
    if(dim_corrente<=k){         //se classifica non ancora piena
        heap[mat_index].w=pathsum;
        heap[mat_index].key=mat_index;
        if(mat_index>0){
            max_bubble_up(heap);
        }
    }
    else{                   //classifica piena
        if(pathsum<heap[0].w){         //se il nuovo pathsum è minore del massimo sostituisci e scorri in basso
            heap[0].w=pathsum;
            heap[0].key=mat_index;
            max_bubble_down(heap);
        }
    }
    //bdab
}

/**
 * scambio elemento di testa e elemento di coda
 * @param heap  puntatore all'heap
 * @param heap_dimension dimensione dell'heap
 */
void min_pop_and_bubble_down(type_heap *heap, __uint64_t* heap_dimension)
{
    __uint64_t dim_heap = *heap_dimension - 1;

    __uint64_t f_index=0, dx_index, sx_index, min_ind;
    __uint8_t flag=0;
    type_heap temp;
    temp=heap[0]; //temp <- testa
    heap[0]=heap[dim_heap]; //testa <- coda
    heap[dim_heap]=temp;// coda <- temp

    (*heap_dimension)--;
    if (dim_heap == 0) {
        return;
    } else {
        dim_heap--;
    }

    while(!flag)
    {
        sx_index=(f_index << 1)+1;
        dx_index=sx_index+1;

        if(dx_index<=dim_heap){     //entrambi i figli esistono
            if(heap[sx_index].w==0 && heap[dx_index].w==0) {
                flag=1;    //entrambi i figli 0 -> non fare nulla
            }
            else if(heap[f_index].w==0){            //se il padre è 0
                if(heap[dx_index].w==0) {
                    min_ind=sx_index;           //se il dx è 0, il sx è il min
                }
                else if(heap[sx_index].w==0) {
                    min_ind=dx_index;          //contrario
                }
                else {
                    min_ind = heap[sx_index].w<heap[dx_index].w ? sx_index : dx_index;    //se nessuno dei due è 0, confronta sgravatissimo
                }
            }
            else{
                if(heap[sx_index].w==0) {
                    min_ind = heap[dx_index].w<heap[f_index].w ? dx_index : f_index; // sx uguale a 0, min_ind è min tra dx e padre
                }
                else if(heap[dx_index].w==0) {
                    min_ind = heap[sx_index].w<heap[f_index].w ? sx_index : f_index; // dx uguale a 0, min_ind è min tra sx e padre
                }
                else{                                                                       //nessuno dei 3 è uguale a 0 fuck
                    min_ind=f_index;
                    if(heap[sx_index].w<heap[min_ind].w) {
                        min_ind=sx_index;
                    }
                    if(heap[dx_index].w<heap[min_ind].w) {
                        min_ind=dx_index;
                    }
                }
            }
        }
        else if(sx_index<=dim_heap){            //esiste solo il figlio sinistro
            if(heap[f_index].w==0 && heap[sx_index].w==0) {
                flag=1;       //padre e unico figlio uguali a 0
            }
            else if(heap[f_index].w==0) {
                min_ind=sx_index;               //padre =0
            }
            else if(heap[sx_index].w==0){
                min_ind=f_index;               //figlio =0
            }
            else {
                min_ind = heap[sx_index].w<heap[f_index].w ? sx_index : f_index;       //entrambi >0, confronta
            }
        }
        else flag=1;            //non esistono figli

        if(!flag){
            if(min_ind==f_index) flag=1;
            else{
                temp=heap[f_index];     //temp <- padre
                heap[f_index]=heap[min_ind];   //padre <- figlio
                heap[min_ind]=temp;       //figlio <- temp
            }
        }
        f_index = min_ind;
    }
}

void min_bubble_up(type_heap *heap, __uint64_t index)
{
    __uint8_t flag=0;
    __uint64_t f_index;
    type_heap temp;
    while(!flag)
    {
        f_index=(index - 1) >> 1;
        if(f_index == 0) flag=1;
        if(heap[f_index].w == 0 || heap[index].w < heap[f_index].w){
            temp=heap[index]; //temp <- figlio
            heap[index]=heap[f_index]; //figlio <- padre
            heap[f_index]=temp; //padre <- temp
        }
        else flag=1;
        index=f_index;
    }
}

__uint64_t dijkstra(__uint64_t dim, __uint32_t m[n][n], type_heap *v)
{
    __uint64_t dim_heap=dim-1, pathsum=0;
    __uint64_t distance_from_j, distance_of_j_from_0, i_weight, new_tot_distance, i_key, j_key;

    //heap_init
    __uint64_t curs_heap=0;
    for(__uint64_t curs_m=1; curs_m<dim; curs_m++){
        v[curs_heap].key=curs_m;
        v[curs_heap].w=m[0][curs_m];
        if(m[0][curs_m]!=0 && curs_heap!=0) min_bubble_up(v, curs_heap);
        curs_heap++;
    }
  
    for(__uint64_t i=0; i<dim_heap; )
    {
        if (v[0].w == 0) return pathsum; //questo non dovrebbe stare fuori dal for?
        pathsum+=v[0].w;
        i_weight=v[0].w;               //peso (definitivo) di v[0]
        i_key=v[0].key;             //chiave di v[0]
        for(__uint64_t j=1; j<dim_heap; j++){

            j_key=v[j].key;
            distance_from_j=m[i_key][j_key];              //arco tra elemento visitato v[0] i_key elemento ispezionato v[j]
            distance_of_j_from_0=v[j].w;                  //peso corrente di v[j]
            new_tot_distance=distance_from_j + i_weight;  //peso alternativo di v[j]=peso di v[0] + arco tra v[0] i_key v[j]
            if (
                distance_from_j != 0 && // Il nodo è raggiungibile da dove sono ora E
                (
                    distance_of_j_from_0 == 0 || // Il nodo PRIMA NON era raggiungibile, ma ORA SI    OPPURE
                    new_tot_distance < distance_of_j_from_0
                )
            )
            { // Miglioro la distanza per raggiungere j
                v[j].w=new_tot_distance;
                min_bubble_up(v, j);
            }
        }
        min_pop_and_bubble_down(v, &dim_heap);
    }
    return pathsum;
}

int main()
{
    __uint64_t dim=0, i, j, pathsum=0;
    __uint8_t flag=0;
    char t;
    char stringa[30];
    //input dim e k
    while(flag==0){
        t=getchar_unlocked();
        if(t==' ') flag=1;
        else dim=(dim << 1)+(dim << 3)+(t-48);
    }
    flag=0;
    n=dim;
    while(flag==0){
        t=getchar_unlocked();
        if(t=='\n' || t=='\r') flag=1;
        else k=(k << 1)+(k << 3)+(t-48);
    }
    while (t != '\n') {
        //printf("CARATTERE LETTO: %c", t);
        t = getchar_unlocked();
    }

    //ora che ho dim, dichiaro la matrice
    __uint32_t m[dim][dim];

    type_heap *v;
    v = malloc((dim-1) * sizeof(type_heap));

    type_heap *heap;
    heap=malloc(k * sizeof(type_heap));

    //input da file
    while (fgets(stringa, 20, stdin)){
        if (stringa[0] == 'A'){
            for(i=0; i<dim; i++)
            {
                for(j=0; j<dim; j++)
                {
                    m[i][j] = 0;
                    t = getchar_unlocked();
                    while(t >= '0' && t <= '9'){
                        m[i][j]= (m[i][j] << 3) + (m[i][j] << 1) + (t -'0');
                        t = getchar_unlocked();
                    }
                    if(i==j || j==0) m[i][j]=0;
                    if (t == '\r') t = getchar_unlocked();
                }
            }

            pathsum=dijkstra(dim, m, v);
            //printf("Matrice %ld ha peso di grafo: %ld.\n", mat_index, pathsum);
            insert_pathsum(pathsum, heap);
            pathsum=0;
            mat_index++;
        }
        else if(stringa[0]=='T'){
            //printf("\ntopk chiamato\n\n");
            if(mat_index!=0) topk(heap);
            else printf("\n");
            //fseek(stdin, 4, SEEK_CUR);
        }
    }
    return 0;
}
