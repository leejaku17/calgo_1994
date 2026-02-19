/*                                                                  */
/*   KRUSKAL.C  :  Minimum Cost Spanning Tree By Kruskal's Method   */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <stdio.h>

#define MAX_VERTEX 50
#define MAX_EDGE 100

#define ONLY_FIND 0
#define UNION 1

int parent[MAX_VERTEX];
int height[MAX_VERTEX]; /* store tree's height */
int cost = 0;           /* store minimum cost */

FILE *fp;

typedef struct _edge
{
    int v1, v2;
    int weight;
} edge;

edge Edge[MAX_EDGE];

int name2int(char c)
{
    return c - 'A';
}

int int2name(int i)
{
    return i + 'A';
}

int nheap = 0; /* no of element of heap */
int heap[MAX_VERTEX];

void upheap(int h[], int k)
{
    int v;
    v = h[k]; /* indirect reference */
    while (Edge[h[k / 2]].weight >= Edge[v].weight && k / 2 > 0)
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
        if (i < nheap && Edge[h[i]].weight > Edge[h[i + 1]].weight)
            i++;
        if (Edge[v].weight <= Edge[h[i]].weight)
            break;
        h[k] = h[i];
        k = i;
    }
    h[k] = v;
}

void print_heap(int h[], int n)
{
    int i;
    printf("\n");
    for (i = 1; i <= n; i++)
    {
        printf("%c%c %d  ", int2name(Edge[h[i]].v1), int2name(Edge[h[i]].v2),
               Edge[h[i]].weight);
    }
    printf("\n");
}

void pq_insert(int h[], int v)
{
    h[++nheap] = v; /* insert */
    upheap(h, nheap);
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

void union_set2(int elem, int asso)
{
    int t;
    if (height[elem] == height[asso]) /* if height equal, increase height */
        height[asso]++;
    if (height[elem] > height[asso]) /* if height different */
    {
        t = elem;
        elem = asso;
        asso = t;
    }
    parent[elem] = asso;
}

int find_set2(int elem, int asso, int flag)
{ /* path compression */
    int i = elem, j = asso, t;
    while (parent[i] >= 0) /* find elem's root */
        i = parent[i];
    while (parent[j] >= 0) /* find asso's root */
        j = parent[j];
    if (flag == UNION && i != j)
        union_set2(i, j); /* set elem to asso's child */
    return (i != j);
}

void find_init(int elem)
{
    int i;
    for (i = 0; i < elem; i++)
        parent[i] = -1;
}

void input_edge(edge e[], int *V, int *E)
{
    char vertex[3];
    int i, j, w;
    printf("\nInput number of Vertex & Edge\n");
    fscanf(fp, "%d %d", V, E); /* input no of Vertex & Edge */
    for (j = 0; j < *E; j++)
    {
        printf("\nInput two Vertex consist of Edge & its Weight -> ");
        fscanf(fp, "%s %d", vertex, &w);

        vertex[2] = NULL;

        e[j].v1 = name2int(vertex[0]);
        e[j].v2 = name2int(vertex[1]);
        e[j].weight = w;
    }
}

void visit(int e)
{
    printf("%c%c  ", int2name(Edge[e].v1), int2name(Edge[e].v2));
    cost += Edge[e].weight;
}

void kruskal(edge e[], int V, int E)
{
    int n, try = 0;
    find_init(V);
    pq_init();

    for (n = 0; n < E; n++) /* heap construct */
        pq_insert(heap, n);

    /* print_heap(heap, nheap);  */

    n = 0; /* number of found edge */
    while (!pq_empty())
    {
        try = pq_extract(heap);

        if (find_set2(e[try].v1, e[try].v2, UNION)) /* if not cycle */
        {
            visit(try);
            n++;
        }
        if (n == V - 1)
            break;
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

    input_edge(Edge, &V, &E);
    printf("\n\nVisited Edge of Minimum Spanning Tree\n");
    kruskal(Edge, V, E);
    printf("\n\nMinimum Cost is \n %d", cost);
    fclose(fp);
}
