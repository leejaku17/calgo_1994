/*                                                                  */
/*   ADJLIST.C  :  Representation of Graph by adjacency list        */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include "intstack.c"
#include "intqueue.c"

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

		i = name2int(vertex[0]);
		t = (node *)malloc(sizeof(node));
		t->vertex = name2int(vertex[1]);
		t->next = a[i];
		a[i] = t;

		i = name2int(vertex[1]); /* symmetry for undirected graph */
		t = (node *)malloc(sizeof(node));
		t->vertex = name2int(vertex[0]);
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

void DFS_recur(node *a[], int i)
{
	node *t;
	check[i] = 1;
	visit(i);
	for (t = a[i]; t != NULL; t = t->next)
		if (check[t->vertex] == 0)
			DFS_recur(a, t->vertex);
}

void DFS_adjlist(node *a[], int V)
{
	int i;
	for (i = 0; i < V; i++)
		check[i] = 0;
	for (i = 0; i < V; i++)
		if (check[i] == 0)
			DFS_recur(a, i);
}

void nrDFS_adjlist(node *a[], int V)
{
	int i;
	node *t;
	init_stack();
	for (i = 0; i < V; i++)
		check[i] = 0;
	for (i = 0; i < V; i++)
	{
		if (check[i] == 0)
		{
			push(i);
			check[i] = 1;
			while (!stack_empty())
			{
				i = pop();
				visit(i);
				for (t = a[i]; t != NULL; t = t->next)
					if (check[t->vertex] == 0)
					{
						push(t->vertex);
						check[t->vertex] = 1;
					}
			}
		}
	}
}

void BFS_adjlist(node *a[], int V)
{
	int i;
	node *t;
	init_queue();
	for (i = 0; i < V; i++)
		check[i] = 0;
	for (i = 0; i < V; i++)
	{
		if (check[i] == 0)
		{
			put(i);
			check[i] = 1;
			while (!queue_empty())
			{
				i = get();
				visit(i);
				for (t = a[i]; t != NULL; t = t->next)
					if (check[t->vertex] == 0)
					{
						put(t->vertex);
						check[t->vertex] = 1;
					}
			}
		}
	}
}

int count_components(node *a[], int V)
{
	int count = 0;
	int i;
	node *t;
	init_stack();
	for (i = 0; i < V; i++)
		check[i] = 0;
	for (i = 0; i < V; i++)
	{
		if (check[i] == 0)
		{
			push(i);
			printf("\nConnected Component %d : ", ++count);
			check[i] = 1;
			while (!stack_empty())
			{
				i = pop();
				printf("%c ", int2name(i));
				for (t = a[i]; t != NULL; t = t->next)
					if (check[t->vertex] == 0)
					{
						push(t->vertex);
						check[t->vertex] = 1;
					}
			}
		}
	}
	printf("\nTotally this graph has %d connected components.", count);
	return count;
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
	printf("\n\nDepth-First Search by recursion\n");
	DFS_adjlist(G, V);
	printf("\n\nDepth-First Search by stack\n");
	nrDFS_adjlist(G, V);
	printf("\n\nBreadth-First Search by queue\n");
	BFS_adjlist(G, V);
	printf("\n\nCount connected components by non-recursive DFS");
	count_components(G, V);

	fclose(fp);
}