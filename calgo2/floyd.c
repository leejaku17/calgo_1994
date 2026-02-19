/*                                                                  */
/*   FLOYD.C  :  All Shortest Paths by Floyd's Algorithm            */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>

#define MAX_VERTEX 100
#define INFINITE 1000

int G[MAX_VERTEX][MAX_VERTEX];
int distance[MAX_VERTEX][MAX_VERTEX];

FILE *fp;

int name2int(char c)
{
    return c - 'A';
}

int int2name(int i)
{
    return i + 'A';
}

void input_adjmatrix(int g[][MAX_VERTEX], int d[][MAX_VERTEX], int *V, int *E)
{
    char vertex[3];
    int i, j, k;
    int weight;
    printf("\nInput number of Vertex & Edge\n");
    fscanf(fp, "%d %d", V, E); /* input no of Vertex & Edge */
    for (i = 0; i < *V; i++)
        for (j = 0; j < *E; j++)
            g[i][j] = d[i][j] = INFINITE;
    for (i = 0; i < *V; i++)
        g[i][i] = d[i][i] = 0; /* assume reflexive */
    for (k = 0; k < *E; k++)
    {
        printf("\n Input two Vertex consist of Edge  -> ");
        fscanf(fp, "%s %d", vertex, &weight);
        i = name2int(vertex[0]);
        j = name2int(vertex[1]);
        g[i][j] = weight; /* asymmetry for directed graph */
        d[i][j] = weight;
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
        {
            if (a[i][j] >= INFINITE)
                printf("%5c", 'x');
            else
                printf("%5d", a[i][j]);
        }
        printf("\n");
    }
}

void floyd(int d[][MAX_VERTEX], int V)
{
    int x, y, i;

    for (y = 0; y < V; y++)
        for (x = 0; x < V; x++)
            for (i = 0; i < V; i++)
                if (d[x][y] + d[y][i] < d[x][i])
                    d[x][i] = d[x][y] + d[y][i];
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

    input_adjmatrix(G, distance, &V, &E);
    printf("\n\nAdjacency Matrix representaion for directed graph");
    print_adjmatrix(G, V);

    getch();

    floyd(distance, V);

    printf("\n\nAll Shortest Paths\n");
    print_adjmatrix(distance, V);

    fclose(fp);
}
