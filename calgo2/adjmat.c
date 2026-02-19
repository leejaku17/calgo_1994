/*                                                                  */
/*   ADJMAT.C  :  Representation of Graph by adjacency matrix       */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include "intstack.c"
#include "intqueue.c"

#define MAX_VERTEX 100

int G[MAX_VERTEX][MAX_VERTEX];
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

void input_adjmatrix(int a[][MAX_VERTEX], int *V, int *E)
{
	char vertex[3];
	int i, j, k;
	printf("\nInput number of Vertex & Edge\n");
	fscanf(fp, "%d %d", V, E); /* input no of Vertex & Edge */
	for (i = 0; i < *V; i++)
		for (j = 0; j < *E; j++)
			a[i][j] = 0;
	for (i = 0; i < *V; i++)
		a[i][i] = 1; /* assume reflexive */
	for (k = 0; k < *E; k++)
	{
		printf("\n Input two Vertex consist of Edge  -> ");
		fscanf(fp, "%s", vertex);
		i = name2int(vertex[0]);
		j = name2int(vertex[1]);
		a[i][j] = 1; /* symmetry for undirected graph */
		a[j][i] = 1;
	}
}

void print_adjmatrix(int a[][MAX_VERTEX], int V)
{
	int i, j;

	printf("\n   ");
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

void visit(int vertex)
{
	printf("%c ", int2name(vertex));
}

void DFS_recur(int a[][MAX_VERTEX], int V, int i)
{
	int j;
	check[i] = 1;
	visit(i);
	for (j = 0; j < V; j++)
		if (a[i][j] != 0)
			if (check[j] == 0)
				DFS_recur(a, V, j);
}

void DFS_adjmatrix(int a[][MAX_VERTEX], int V)
{
	int i;
	for (i = 0; i < V; i++)
		check[i] = 0;
	for (i = 0; i < V; i++)
		if (check[i] == 0)
			DFS_recur(a, V, i);
}

void nrDFS_adjmatrix(int a[][MAX_VERTEX], int V)
{
	int i, j;
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
				for (j = 0; j < V; j++)
					if (a[i][j] != 0)
						if (check[j] == 0)
						{
							push(j);
							check[j] = 1;
						}
			}
		}
	}
}

void BFS_adjmatrix(int a[][MAX_VERTEX], int V)
{
	int i, j;
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
				for (j = 0; j < V; j++)
					if (a[i][j] != 0)
						if (check[j] == 0)
						{
							put(j);
							check[j] = 1;
						}
			}
		}
	}
}

int count_components(int a[][MAX_VERTEX], int V)
{
	int count = 0;
	int i, j;
	init_stack();
	for (i = 0; i < V; i++)
		check[i] = 0;
	for (i = 0; i < V; i++)
	{
		if (check[i] == 0)
		{
			printf("\nConnected Component %d : ", ++count);
			push(i);
			check[i] = 1;
			while (!stack_empty())
			{
				i = pop();
				printf("%c ", int2name(i));
				for (j = 0; j < V; j++)
					if (a[i][j] != 0)
						if (check[j] == 0)
						{
							push(j);
							check[j] = 1;
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
		printf("\nThat file does not exist!");
		exit(1);
	}

	input_adjmatrix(G, &V, &E);
	printf("\n\nAdjacency Matrix representaion for graph");
	print_adjmatrix(G, V);
	printf("\n\nDepth-First Search by recursion\n");
	DFS_adjmatrix(G, V);
	printf("\n\nDepth-First Search by stack\n");
	nrDFS_adjmatrix(G, V);
	printf("\n\nBreadth-First Search by queue\n");
	BFS_adjmatrix(G, V);
	printf("\n\nCount connected components by non-recursive DFS");
	count_components(G, V);

	fclose(fp);
}
