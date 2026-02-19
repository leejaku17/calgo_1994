/*                                                                  */
/*   CLOSURE.C  :  Get Transitive Closure By Depth First Search     */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <stdio.h>
#include <stdlib.h>
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

        i = name2int(vertex[0]); /* asymmetry for directed graph */
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

void DFS_directed(node *a[], int V)
{ /* Get Transitive Closure by Depth First Search */
    int i, j;
    node *t;
    init_stack();
    for (i = 0; i < V; i++)
    {
        for (j = 0; j < V; j++)
            check[j] = 0; /* clear */

        push(i);
        check[i] = 1;
        printf("\n %c : ", int2name(i));
        while (!stack_empty())
        {
            j = pop();
            visit(j);
            for (t = a[j]; t != NULL; t = t->next)
                if (check[t->vertex] == 0)
                {
                    push(t->vertex);
                    check[t->vertex] = 1;
                }
        }
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
    printf("\n\nAdjacency List representaion for graph");
    print_adjlist(G, V);
    printf("\n\nTransitive Closure by Depth First Search\n");
    DFS_directed(G, V);

    fclose(fp);
}
