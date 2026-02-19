/*                                                                  */
/*   TOPOSORT.C  :  Get Topological order of DAG                    */
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

typedef struct _head /* head for node structure */
{
	int count;
	struct _node *next;
} head;

head network[MAX_VERTEX];

FILE *fp;

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
{ /* input network */
	int i, j;
	char edge[3];
	node *t;
	printf("\nInput number of Vertex & Edge\n");
	fscanf(fp, "%d %d", V, E); /* input no of Vertex & Edge */

	set_count_zero(network, *V);

	for (i = 0; i < *V; i++)
		net[i].next = NULL;

	for (j = 0; j < *E; j++)
	{
		printf("\nInput two Vertex consist of Edge -> ");
		fscanf(fp, "%s", edge);

		t = (node *)malloc(sizeof(node));
		t->vertex = name2int(edge[1]);
		t->next = net[name2int(edge[0])].next;
		net[name2int(edge[0])].next = t;
	}
}

void print_adjlist(head net[], int V)
{ /* print network */
	int i;
	node *t;
	for (i = 0; i < V; i++)
	{
		printf("\n%c:%d | ", int2name(i), net[i].count);
		for (t = net[i].next; t != NULL; t = t->next)
			printf("-> %c ", int2name(t->vertex));
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
			for (t = net[j].next; t; t = t->next)
				if (t->vertex == i)
					count++;
		net[i].count = count;
	}
}

void topsort(head net[], int V)
{ /* function for topological sorting */
	int i, j, k;
	node *ptr;

	init_stack();

	set_count_indegree(net, V);

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
			printf("%c, ", int2name(j));
			for (ptr = net[j].next; ptr; ptr = ptr->next)
			{
				k = ptr->vertex;
				net[k].count--;
				if (!net[k].count)
					push(k);
			}
		}
	}
}

void revtopsort(head net[], int V)
{ /* function for reverse topological sorting */
	int i, j, k;
	node *ptr;

	init_stack();

	set_count_outdegree(net, V);

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
			printf("%c, ", int2name(j));
			for (k = 0; k < V; k++)
				for (ptr = net[k].next; ptr; ptr = ptr->next)
					if (ptr->vertex == j)
					{
						net[k].count--;
						if (!net[k].count)
							push(k);
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

	input_adjlist(network, &V, &E);
	printf("\n\nAdjacency List representaion for graph");
	print_adjlist(network, V);
	printf("\n\nSet count to indegree");
	set_count_indegree(network, V);
	print_adjlist(network, V);
	printf("\n\nTopological order\n");
	topsort(network, V);
	getch();
	printf("\n\nSet count to outdegree");
	set_count_outdegree(network, V);
	print_adjlist(network, V);
	printf("\n\nReverse Topological order\n");
	revtopsort(network, V);
}
