/*                                                                  */
/*   PFS.C   :   Priority First Search                              */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include <limits.h>

#define MAX_VERTEX 100
#define UNSEEN (-INT_MAX)

typedef struct _node
{
    struct _node *next;
    int vertex;
    int weight;
} node;

node *G[MAX_VERTEX];
int check[MAX_VERTEX];

int parent[MAX_VERTEX];

FILE *fp;

int nheap = 0; /* no of element of heap */
int heap[MAX_VERTEX];

void upheap(int h[], int k)
{
    int v;
    v = h[k]; /* indirect reference */
    while (check[h[k / 2]] <= check[v] && k / 2 > 0)
    {
        h[k] = h[k / 2];
        k /= 2;
    }
    h[k] = v;
}

void downheap(int h[], int k)
{
    int i, v;
    v = h[k];
    while (k <= nheap / 2)
    {
        i = k << 1;
        if (i < nheap && check[h[i]] < check[h[i + 1]])
            i++;
        if (check[v] >= check[h[i]])
            break;
        h[k] = h[i];
        k = i;
    }
    h[k] = v;
}

void adjust_heap(int h[], int n)
{ /* buttom up heap construction */
    int k;
    for (k = n / 2; k >= 1; k--)
        downheap(h, k);
}

int pq_update(int h[], int v, int p)
{
    if (check[v] == UNSEEN) /* if vertex v is not visited */
    {
        h[++nheap] = v; /* insert */
        check[v] = p;
        upheap(h, nheap);
        return 1;
    }
    else
    {
        if (check[v] < p)
        {
            check[v] = p;          /* modify */
            adjust_heap(h, nheap); /* after modify, adjusting heap */
            return 1;
        }
        else
            return 0;
    }
}

void pq_init(void)
{
    nheap = 0;
}

int pq_empty(void)
{
    if (nheap == 0)
        return 1;
    return 0;
}

int pq_extract(int h[])
{
    int v = h[1];
    h[1] = h[nheap--];
    downheap(h, 1);
    return v;
}

int name2int(char c)
{
    return c - 'A';
}

int int2name(int i)
{
    return i + 'A';
}

void input_adjlist(node *g[], int *V, int *E)
{
    char vertex[3];
    int i, j, w;
    node *t;
    printf("\nInput number of Vertex & Edge\n");
    fscanf(fp, "%d %d", V, E); /* input no of Vertex & Edge */
    for (i = 0; i < *V; i++)   /* initialize nodes */
        g[i] = NULL;
    for (j = 0; j < *E; j++)
    {
        printf("\nInput two Vertex consist of Edge & its Weight -> ");
        fscanf(fp, "%s %d", vertex, &w);

        i = name2int(vertex[0]);
        t = (node *)malloc(sizeof(node));
        t->vertex = name2int(vertex[1]);
        t->weight = w;
        t->next = g[i];
        g[i] = t;

        i = name2int(vertex[1]); /* symmetry for undirected graph */
        t = (node *)malloc(sizeof(node));
        t->vertex = name2int(vertex[0]);
        t->weight = w;
        t->next = g[i];
        g[i] = t;
    }
}

void print_adjlist(node *g[], int V)
{
    int i;
    node *t;
    for (i = 0; i < V; i++)
    {
        printf("\n%c : ", int2name(i));
        for (t = g[i]; t != NULL; t = t->next)
            printf("-> %c:%d ", int2name(t->vertex), t->weight);
    }
}

void print_tree(int tree[], int n)
{
    int i;
    printf("son     ");
    for (i = 0; i < n; i++)
        printf("%c  ", int2name(i));
    printf("\n----------------------------------------------------\n");
    printf("parent  ");
    for (i = 0; i < n; i++)
    {
        if (i == 0)
            printf("   ");
        else
            printf("%c  ", int2name(tree[i]));
    }
}

void print_cost(int check[], int n)
{
    int i;
    int cost = 0;
    for (i = 1; i < n; i++)
        cost += check[i];
    printf("%d", cost);
}

void visit(int vertex)
{
    printf("  ->  %c ", int2name(vertex));
}

void print_heap(int h[])
{
    int i;
    printf("\n");
    for (i = 1; i <= nheap; i++)
        printf("%c:%d ", int2name(h[i]), check[h[i]]);
}

void PFS_adjlist(node *g[], int V)
{
    int i;
    node *t;
    pq_init();
    for (i = 0; i < V; i++)
    {
        check[i] = UNSEEN;
        parent[i] = 0;
    }
    for (i = 0; i < V; i++)
    {
        if (check[i] == UNSEEN)
        {
            parent[i] = -1;
            pq_update(heap, i, UNSEEN); /* first vertex */
            while (!pq_empty())
            {
                print_heap(heap);
                i = pq_extract(heap);
                check[i] = -check[i]; /* fringe to tree */
                visit(i);
                for (t = g[i]; t != NULL; t = t->next)
                    if (check[t->vertex] < 0) /* fringe or unseen */
                        if (pq_update(heap, t->vertex, -t->weight))
                        {
                            parent[t->vertex] = i;
                        }
                /* if updated, it means t->vertex's parent is i */
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
    printf("\n\nOriginal Graph\n");
    print_adjlist(G, V);
    printf("\n\nVisit order of Minimum Spanning Tree\n");
    PFS_adjlist(G, V);
    printf("\n\nTree structure\n");
    print_tree(parent, V);
    printf("\n\nMinimum Cost is \n");
    print_cost(check, V);
    fclose(fp);
}
