/*                                                                  */
/*   STRONG.C  :  Get stongly connected components in adjlist       */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <stdio.h>
#include <alloc.h>
#include "intstack.c"

#define MAX_VERTEX 100

typedef struct _node /* node for adjacency list */
{
    struct _node *next;
    int vertex;
} node;

FILE *fp;
node *G[MAX_VERTEX];
int check[MAX_VERTEX] = {
    0,
};
int order = 0;

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

int strong_recur(node *g[], int i)
{
    int m, min;
    int k;
    int flag;
    node *t;
    check[i] = min = ++order;
    push(i);
    for (t = g[i]; t != NULL; t = t->next)
    {
        if (check[t->vertex] == 0)
            m = strong_recur(g, t->vertex);
        else
            m = check[t->vertex];
        if (m < min)
            min = m;
    }
    if (min == check[i])
    {
        flag = 0;
        while ((k = pop()) != i)
        {
            printf(" %c ", int2name(k));
            check[k] = MAX_VERTEX + 1;
            flag = 1;
        }
        if (flag)
            printf(" %c\n", int2name(k));
    }
    return min;
}

void strong(node *g[], int V)
{
    int i, k;
    for (i = 0; i < V; i++)
        check[i] = 0;
    for (i = 0; i < V; i++)
    {
        if (check[i] == 0)
            strong_recur(g, i);
    }
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

    printf("\nStrongly connected components\n");
    strong(G, V);

    fclose(fp);
}
