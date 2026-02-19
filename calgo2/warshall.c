/*                                                                  */
/*   WARSHALL.C  :  Get Transitive Closure by Warshall's Algorithm  */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>

#define MAX_VERTEX 100

int G[MAX_VERTEX][MAX_VERTEX];
int closure[MAX_VERTEX][MAX_VERTEX];

FILE *fp;

int name2int(char c)
{
    return c - 'A';
}

int int2name(int i)
{
    return i + 'A';
}

void input_adjmatrix(int g[][MAX_VERTEX], int c[][MAX_VERTEX], int *V, int *E)
{
    char vertex[3];
    int i, j, k;
    printf("\nInput number of Vertex & Edge\n");
    fscanf(fp, "%d %d", V, E); /* input no of Vertex & Edge */
    for (i = 0; i < *V; i++)
        for (j = 0; j < *E; j++)
            g[i][j] = c[i][j] = 0;
    for (i = 0; i < *V; i++)
    {
        g[i][i] = 0; /* assume reflexive */
        c[i][i] = 1;
    }
    for (k = 0; k < *E; k++)
    {
        printf("\n Input two Vertex consist of Edge  -> ");
        fscanf(fp, "%s", vertex);
        i = name2int(vertex[0]);
        j = name2int(vertex[1]);
        g[i][j] = 1; /* asymmetry for directed graph */
        c[i][j] = 1;
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

void warshall(int c[][MAX_VERTEX], int V)
{
    int x, y, i;

    for (y = 0; y < V; y++)
        for (x = 0; x < V; x++)
            if (c[x][y])
                for (i = 0; i < V; i++)
                    if (c[y][i])
                        c[x][i] = 1;
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

    input_adjmatrix(G, closure, &V, &E);
    printf("\n\nAdjacency Matrix representaion for directed graph");
    print_adjmatrix(G, V);

    getch();

    warshall(closure, V);

    printf("\n\nTransitive Closure\n");
    print_adjmatrix(closure, V);

    fclose(fp);
}
