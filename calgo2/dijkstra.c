/*                                                                  */
/*   DIJKSTRA.C  :  Get Shortest Path by Dijkstra's Algorithm       */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>

#define MAX_VERTEX 100
#define INFINITE 1000

int G[MAX_VERTEX][MAX_VERTEX];
int check[MAX_VERTEX];
int parent[MAX_VERTEX];
int distance[MAX_VERTEX];

FILE *fp;

int name2int(char c)
{
    return c - 'A';
}

int int2name(int i)
{
    return i + 'A';
}

void input_adjmatrix(int a[][MAX_VERTEX], int *V, int *E)
{
    char vertex[3];
    int i, j, k;
    int weight;
    printf("\nInput number of Vertex & Edge\n");
    fscanf(fp, "%d %d", V, E); /* input no of Vertex & Edge */
    for (i = 0; i < *V; i++)
        for (j = 0; j < *E; j++)
            a[i][j] = INFINITE;
    for (i = 0; i < *V; i++)
        a[i][i] = 0; /* assume reflexive */
    for (k = 0; k < *E; k++)
    {
        printf("\n Input two Vertex consist of Edge  -> ");
        fscanf(fp, "%s %d", vertex, &weight);
        i = name2int(vertex[0]);
        j = name2int(vertex[1]);
        a[i][j] = weight; /* symmetry for undirected graph */
        a[j][i] = weight;
    }
}

void print_adjmatrix(int a[][MAX_VERTEX], int V)
{
    int i, j;

    printf("\n   ");
    for (i = 0; i < V; i++)
        printf("%5c", int2name(i));

    printf("\n");
    for (i = 0; i < V; i++)
    {
        printf("%3c", int2name(i));
        for (j = 0; j < V; j++)
            printf("%5d", a[i][j]);
        printf("\n");
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
        if (tree[i] == -1)
            printf("^  ");
        else
            printf("%c  ", int2name(tree[i]));
    }
}

void print_distance(int d[], int x, int n)
{
    int i;
    printf("\n%c  ", int2name(x));
    for (i = 0; i < n; i++)
        printf("%5d", d[i]);
}

void print_cost(int d[], int n)
{
    int i;
    printf("vertex  ");
    for (i = 0; i < n; i++)
        printf("%5c", int2name(i));
    printf("\n-------------------------------------------------------------------\n");
    printf("cost    ");
    for (i = 0; i < n; i++)
        printf("%5d", d[i]);
}

void dijkstra(int a[][MAX_VERTEX], int s, int V)
{
    int x = 0, y, d;
    int i, checked = 0;

    for (x = 0; x < V; x++) /* initialize */
    {
        distance[x] = a[s][x];
        if (x != s)
            parent[x] = s;
    }

    check[s] = 1;
    checked++;

    print_distance(distance, s, V);

    while (checked < V)
    {
        x = 0;
        while (check[x])
            x++;
        for (i = x; i < V; i++)
            if (check[i] == 0 && distance[i] < distance[x])
                x = i;
        /* x is one of unchecked vertex whose distance from s is minimal */

        check[x] = 1; /* check vertex x */
        checked++;

        for (y = 0; y < V; y++)
        {
            if (x == y || a[x][y] >= INFINITE || check[y])
                continue;
            /* y is unchecked & exists edge (x,y) */
            d = distance[x] + a[x][y];
            if (d < distance[y])
            {
                distance[y] = d;
                parent[y] = x;
            }
        }
        print_distance(distance, x, V);
    }
}

void main(int argc, char *argv[])
{
    int V, E;
    int i;

    if (argc < 2)
        fp = stdin;
    else if ((fp = fopen(argv[1], "rt")) == NULL)
    {
        printf("\nThat file does not exist!");
        exit(1);
    }

    for (i = 0; i < MAX_VERTEX; i++)
    { /* initialize */
        check[i] = 0;
        parent[i] = -1;
    }

    input_adjmatrix(G, &V, &E);
    printf("\n\nAdjacency Matrix representaion for weighted graph");
    print_adjmatrix(G, V);

    getch();

    dijkstra(G, 5, V);

    printf("\n\nShortest Path Tree from A\n");
    print_tree(parent, V);

    printf("\n\nCost from A in shortest path\n");
    print_cost(distance, V);

    fclose(fp);
}
