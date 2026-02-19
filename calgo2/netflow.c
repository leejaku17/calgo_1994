/*                                                                  */
/*   NETFLOW.C  :  Network Flow Problem                             */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include <values.h>
#include "intqueue.c"

#define MAX_VERTEX 20
#define SOURCE 'S'
#define SINK 'T'

int Capacity[MAX_VERTEX][MAX_VERTEX];
int Flow[MAX_VERTEX][MAX_VERTEX];
int Residual[MAX_VERTEX][MAX_VERTEX];
int check[MAX_VERTEX];
int parent[MAX_VERTEX];
int path[MAX_VERTEX];

FILE *fp;

int name2int(char c)
{
    if (c == SOURCE)
        return 0;
    if (c == SINK)
        return 1;
    return c - 'A' + 2;
}

int int2name(int i)
{
    if (i == 0)
        return SOURCE;
    if (i == 1)
        return SINK;
    return i + 'A' - 2;
}

void clear_matrix(int a[][MAX_VERTEX], int V)
{
    int i, j;
    for (i = 0; i < V; i++)
        for (j = 0; j < V; j++)
            a[i][j] = 0;
}

void input_adjmatrix(int a[][MAX_VERTEX], int *V, int *E)
{
    char vertex[3];
    int capacity;
    int i, j, k;
    printf("\nInput number of Vertex & Edge\n");
    fscanf(fp, "%d %d", V, E); /* input no of Vertex & Edge */
    clear_matrix(a, *V);
    for (k = 0; k < *E; k++)
    {
        printf("\n Input two Vertex consist of Edge & Capacity -> ");
        fscanf(fp, "%s %d", vertex, &capacity);
        i = name2int(vertex[0]);
        j = name2int(vertex[1]);
        a[i][j] = capacity; /* asymmetry for directed graph */
    }
}

void print_adjmatrix(int a[][MAX_VERTEX], int V, char *title)
{
    int i, j;

    printf("%s\n   ", title);
    for (i = 0; i < V; i++)
        printf("%3c", int2name(i));

    printf("\n");
    for (i = 0; i < V; i++)
    {
        printf("%3c", int2name(i));
        for (j = 0; j < V; j++)
            printf("%3d", a[i][j]);
        printf("\n");
    }
}

void set_path(void)
{ /* set path[] to simple path */
    int *temp;
    int i, count = 0;
    temp = malloc(sizeof(int) * MAX_VERTEX);
    i = name2int(SINK);
    printf("\n");
    while (i >= 0)
    { /* set temp to revere order of simple path */
        temp[count] = i;
        i = parent[i];
        count++;
    }
    for (i = 0; i < count; i++)
        path[i] = temp[count - i - 1];
    path[i] = -1;
    free(temp);
}

int get_augment_path(int a[][MAX_VERTEX], int V, int S, int T)
{ /* path compression needed */
    int i, j;
    init_queue();
    for (i = 0; i < V; i++)
    {
        check[i] = 0;
        parent[i] = -1;
    }
    i = name2int(S);
    if (check[i] == 0)
    {
        put(i);
        check[i] = 1;
        while (!queue_empty())
        {
            i = get();
            if (i == name2int(T)) /* found augmenting path */
                break;
            for (j = 0; j < V; j++)
                if (a[i][j] != 0)
                    if (check[j] == 0)
                    {
                        put(j);
                        check[j] = 1;
                        parent[j] = i;
                    }
        }
    }
    set_path();
    if (i == name2int(T))
        return 1;
    else
        return 0;
}

void construct_residual(int c[][MAX_VERTEX], int f[][MAX_VERTEX],
                        int r[][MAX_VERTEX], int V)
{
    int i, j;
    for (i = 0; i < V; i++)
        for (j = 0; j < V; j++)
            r[i][j] = c[i][j] - f[i][j];
}

void print_path(void)
{
    int i = 0;
    printf("\n");
    while (path[i] >= 0)
        printf("%c", int2name(path[i++]));
    printf("\n");
}

void network_flow(int c[][MAX_VERTEX], int f[][MAX_VERTEX],
                  int r[][MAX_VERTEX], int V, int S, int T)
{
    int i, min;
    clear_matrix(f, V);
    clear_matrix(r, V);
    construct_residual(c, f, r, V);
    print_adjmatrix(r, V, "Residual");
    getch();
    while (get_augment_path(r, V, S, T))
    /* while to get augment path is possible */
    {
        print_path();
        min = MAXINT;
        for (i = 1; path[i] >= 0; i++)
            if (min > r[path[i - 1]][path[i]])
                min = r[path[i - 1]][path[i]];
        /* set flow to minimum of edge which consist of augment path */
        for (i = 1; path[i] >= 0; i++)
        {
            f[path[i - 1]][path[i]] = f[path[i - 1]][path[i]] + min;
            f[path[i]][path[i - 1]] = -f[path[i - 1]][path[i]];
        }
        print_adjmatrix(f, V, "Flow");
        getch();
        construct_residual(c, f, r, V);
        print_adjmatrix(r, V, "Residual");
        getch();
    }
}

void main(int argc, char *argv[])
{
    int V, E;

    if (argc < 2)
        fp = stdin;
    else if ((fp = fopen(argv[1], "rt")) == NULL)
    {
        printf("\nThat file does not exist!");
        exit(1);
    }
    input_adjmatrix(Capacity, &V, &E);
    printf("\n\nAdjacency Matrix representaion for Capacity");
    print_adjmatrix(Capacity, V, "Capacity");
    print_adjmatrix(Residual, V, "Residual");
    network_flow(Capacity, Flow, Residual, V, SOURCE, SINK);
    print_adjmatrix(Flow, V, "Flow");

    fclose(fp);
}
