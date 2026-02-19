/*                                                                  */
/*   AOE.C  :  Get Critical activity in AOE Network                 */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <stdio.h>
#include <alloc.h>
#include "intstack.c"

/*                                */
/* Implementation of Graph kernel */
/*                                */

#define MAX_VERTEX 100

typedef struct _node /* node for adjacency list*/
{
    struct _node *next;
    int vertex;
    int weight;
    char act[20];
} node;

typedef struct _head /* header for node structure */
{
    int count;
    struct _node *next;
} head;

head AOE[MAX_VERTEX];

FILE *fp;

int earliest[MAX_VERTEX];
int latest[MAX_VERTEX];

int name2int(char c)
{
    return c - 'A';
}

int int2name(int i)
{
    return i + 'A';
}

void set_count_zero(head net[], int V)
{
    int i;
    for (i = 0; i < V; i++)
        net[i].count = 0;
}

void input_adjlist(head net[], int *V, int *E)
{ /* input AOE Network */
    int i, j;
    char edge[3], a[20];
    int w;
    node *t;
    printf("\nInput number of Vertex & Edge\n");
    fscanf(fp, "%d %d", V, E); /* input no of Vertex & Edge */

    set_count_zero(net, *V);

    for (i = 0; i < *V; i++)
        net[i].next = NULL;

    for (j = 0; j < *E; j++)
    {
        printf("\nInput two Vertex consist of Edge  & Weight -> ");
        fscanf(fp, "%s %d %s", edge, &w, a);

        t = (node *)malloc(sizeof(node));
        t->vertex = name2int(edge[1]);
        t->weight = w;
        strcpy(t->act, a);
        t->next = net[name2int(edge[0])].next;
        net[name2int(edge[0])].next = t;
    }
}

void print_adjlist(head net[], int V)
{ /* print AOE */
    int i;
    node *t;
    for (i = 0; i < V; i++)
    {
        printf("\n%c:%d | ", int2name(i), net[i].count);
        for (t = net[i].next; t != NULL; t = t->next)
            printf("-> %c:%d", int2name(t->vertex), t->weight);
    }
}

void set_count_outdegree(head net[], int V)
{ /* set count to outdegree */
    int i;
    int count;
    node *t;
    for (i = 0; i < V; i++)
    {
        count = 0;
        for (t = net[i].next; t; t = t->next)
            count++;
        net[i].count = count;
    }
}

void set_count_indegree(head net[], int V)
{ /* set count to indegree */
    int i, j;
    int count;
    node *t;
    for (i = 0; i < V; i++)
    {
        count = 0;
        for (j = 0; j < V; j++)
        {
            for (t = net[j].next; t; t = t->next)
            {
                if (t->vertex == i)
                    count++;
            }
        }
        net[i].count = count;
    }
}

/* Implementation of critical activity */

void print_earliest(int V)
{ /* print earliest */
    int i;
    printf("\n\nEarliest time \n");
    for (i = 0; i < V; i++)
        printf("%-4c ", int2name(i));
    printf("\n");
    for (i = 0; i < V; i++)
        printf("---- ");
    printf("\n");
    for (i = 0; i < V; i++)
        printf("%-5d", earliest[i]);
}

void print_latest(int V)
{ /* print latest */
    int i;
    printf("\n\nLatest time\n");
    for (i = 0; i < V; i++)
        printf("%-4c ", int2name(i));
    printf("\n");
    for (i = 0; i < V; i++)
        printf("---- ");
    printf("\n");
    for (i = 0; i < V; i++)
        printf("%-5d", latest[i]);
}

void forward_stage(head net[], int V)
{ /* calculate earliest */
    int i, j, k;
    node *ptr;

    init_stack();
    set_count_indegree(net, V);

    for (i = 0; i < V; i++) /* initialize */
        earliest[i] = 0;

    for (i = 0; i < V; i++)
        if (!net[i].count) /* search start position of topological sort */
            push(i);
    for (i = 0; i < V; i++)
    {
        /* if i < V, network has cycle, case of acyclic network
           loop must be exhausted */
        if (stack_empty())
        {
            printf("\nNetwork has a cycle. Sort Terminated ! ");
            exit(1);
        }
        else
        {
            j = pop();
            for (ptr = net[j].next; ptr; ptr = ptr->next)
            {
                k = ptr->vertex;
                net[k].count--;
                if (!net[k].count)
                    push(k);
                if (earliest[k] < earliest[j] + ptr->weight)
                    earliest[k] = earliest[j] + ptr->weight;
            }
        }
    }
}

void backward_stage(head net[], int V)
{ /* calculate lateat */
    int i, j, k, l;
    node *ptr;

    init_stack();
    set_count_outdegree(net, V);

    for (i = 0; i < V; i++) /* initialize */
        latest[i] = earliest[V - 1];

    for (i = 0; i < V; i++)
        if (!net[i].count) /* search start position of topological sort */
            push(i);
    for (i = 0; i < V; i++)
    {
        /* if i < V, network has cycle, case of acyclic network
           loop must be exhausted */
        if (stack_empty())
        {
            printf("\nNetwork has a cycle. Sort Terminated ! ");
            exit(1);
        }
        else
        {
            j = pop();
            for (l = 0; l < V; l++)
            {
                for (ptr = net[l].next; ptr; ptr = ptr->next)
                {
                    if (ptr->vertex == j)
                    {
                        k = l;
                        net[k].count--;
                        if (!net[k].count)
                            push(k);
                        if (latest[k] > latest[j] - ptr->weight)
                            latest[k] = latest[j] - ptr->weight;
                    }
                }
            }
        }
    }
}

void print_critical_activity(head net[], int V)
{
    int i;
    int e, l;
    node *t;
    printf("\n\nCritical Activities");
    printf("\nEdge   Action             Early    Late     Late-Early    Critical?"
           "\n-----  -----------------  -----    -----    ----------    ---------");
    for (i = 0; i < V; i++)
    {
        for (t = net[i].next; t; t = t->next)
        {
            e = earliest[i];
            l = latest[t->vertex] - t->weight;
            printf("\n<%c,%c>  %-20s %-4d  "
                   "   %-4d       %-4d         %s",
                   int2name(i), int2name(t->vertex), t->act, e, l, l - e,
                   (l - e == 0) ? "Yes" : "No");
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
    input_adjlist(AOE, &V, &E);
    printf("\n\nAdjacency List representaion for graph");
    print_adjlist(AOE, V);
    printf("\n\nSet count to indegree");
    set_count_indegree(AOE, V);
    print_adjlist(AOE, V);
    printf("\n\nSet count to outdegree");
    set_count_outdegree(AOE, V);
    print_adjlist(AOE, V);
    forward_stage(AOE, V);
    backward_stage(AOE, V);
    print_earliest(V);
    print_latest(V);
    print_critical_activity(AOE, V);
}
