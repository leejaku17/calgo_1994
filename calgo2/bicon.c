/*                                                                  */
/*   BICON.C  :  Biconnectivity of Graph by adjacency list          */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <stdio.h>
#include <alloc.h>
#include "intstack.c"

#define MAX_VERTEX 100

typedef struct _node
{
    struct _node *next;
    int vertex;
} node;

node *G[MAX_VERTEX];
int check[MAX_VERTEX];
int order = 0;
int son_of_root = 0;

FILE *fp;

int name2int(char c)
{
    return c - 'A';
}

int int2name(int i)
{
    return i + 'A';
}

void input_adjlist(node *a[], int *V, int *E)
{
    char vertex[3];
    int i, j;
    node *t;
    printf("\nInput number of Vertex & Edge\n");
    fscanf(fp, "%d %d", V, E); /* input no of Vertex & Edge */
    for (i = 0; i < *V; i++)   /* initialize nodes */
        a[i] = NULL;
    for (j = 0; j < *E; j++)
    {
        printf("\nInput two Vertex consist of Edge  -> ");
        fscanf(fp, "%s", vertex);

        i = name2int(vertex[0]);
        t = (node *)malloc(sizeof(node));
        t->vertex = name2int(vertex[1]);
        t->next = a[i];
        a[i] = t;

        i = name2int(vertex[1]); /* symmetry for undirected graph */
        t = (node *)malloc(sizeof(node));
        t->vertex = name2int(vertex[0]);
        t->next = a[i];
        a[i] = t;
    }
}

void print_adjlist(node *a[], int V)
{
    int i;
    node *t;
    for (i = 0; i < V; i++)
    {
        printf("\n%c : ", int2name(i));
        for (t = a[i]; t != NULL; t = t->next)
            printf("-> %c ", int2name(t->vertex));
    }
}

void visit(int vertex)
{
    printf("%c ", int2name(vertex));
}

void DFS_recur(node *a[], int i)
{
    node *t;
    check[i] = 1;
    visit(i);
    for (t = a[i]; t != NULL; t = t->next)
        if (check[t->vertex] == 0)
            DFS_recur(a, t->vertex);
}

void DFS_adjlist(node *a[], int V)
{
    int i;
    for (i = 0; i < V; i++)
        check[i] = 0;
    for (i = 0; i < V; i++)
        if (check[i] == 0)
            DFS_recur(a, i);
}

int AP_recur(node *a[], int i)
{
    node *t;
    int min, m;
    check[i] = min = ++order;
    for (t = a[i]; t != NULL; t = t->next)
    {
        if (i == 0 && check[t->vertex] == 0)
            son_of_root++;
        if (check[t->vertex] == 0)
        {
            m = AP_recur(a, t->vertex);
            if (m < min)
                min = m;                 /* if back edge */
            if (m >= check[i] && i != 0) /* except root */
                printf("* %c %2d : %d\n", int2name(i), check[i], m);
            /* if back-edge is not exist, node i is articulation point */
            else
                printf("  %c %2d : %d\n", int2name(i), check[i], m);
        }
        else if (check[t->vertex] < min)
            min = check[t->vertex];
    }
    return min;
}

void AP_search(node *a[], int V)
{
    int i, n = 0;
    node *t;
    for (i = 0; i < V; i++)
        check[i] = 0;
    order = son_of_root = 0;

    /* first, check other vertex */
    AP_recur(a, 0);

    /* now, check son of root */
    if (son_of_root > 1)
        printf("* ");
    else
        printf("  ");
    printf("%c son: %d\n", int2name(0), son_of_root);
}

void main(int argc, char *argv[])
{
    int V, E;

    if (argc < 2)
        fp = stdin;
    else if ((fp = fopen(argv[1], "rt")) == NULL)
    {
        printf("\n That file does not exist!");
        exit(1);
    }

    input_adjlist(G, &V, &E);

    print_adjlist(G, V);

    printf("\nSpanning Tree By DFS\n");
    DFS_adjlist(G, V);

    printf("\nList of Articulation point -> \n");
    AP_search(G, V);

    fclose(fp);
}