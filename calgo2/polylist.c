/*                                                                  */
/*   POLYLIST.C  :  Polynomial library by linked list               */
/*                                                                  */
/*                     Programmed By Kim Junmo                      */
/*                       Modified By Lee Jaekyu                     */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "polylist.h"

node *make_node(int degree, double coef)
{
	node *new;
	new = (node *)malloc(sizeof(node));
	new->degree = degree;
	new->coef = coef;
	new->next = NULL;
	return new;
}

void insert_node(node *head, int degree, double coef)
{
	node *move, *new;
	if (coef == 0.0)
		return;
	new = make_node(degree, coef);
	move = head;
	while (1)
	{
		if (!move->next) /* append new at end of list */
		{
			move->next = new;
			break;
		}
		else if (move->next->degree == new->degree)
		{ /* if a node of same degree exists, add coef & delete new */
			move->next->coef += new->coef;
			free(new);
			break;
		}
		else if (move->next->degree < new->degree)
		{ /* insert in list */
			new->next = move->next;
			move->next = new;
			break;
		}
		else
			move = move->next;
	}
}

node *add_polynomial(node *a, node *b)
{
	node *result, *move;
	result = make_node(NULL, NULL); /* create head node */
	move = result;
	a = a->next;
	b = b->next;
	while (a && b)
	{
		if (a->degree > b->degree)
		{
			move->next = make_node(a->degree, a->coef);
			move = move->next;
			a = a->next;
		}
		else if (a->degree < b->degree)
		{
			move->next = make_node(b->degree, b->coef);
			move = move->next;
			b = b->next;
		}
		else
		{
			if (a->coef + b->coef)
			{
				move->next = make_node(a->degree, a->coef + b->coef);
				move = move->next;
			}
			a = a->next;
			b = b->next;
		}
	}
	while (a)
	{
		move->next = make_node(a->degree, a->coef);
		a = a->next;
		move = move->next;
	}
	while (b)
	{
		move->next = make_node(b->degree, b->coef);
		b = b->next;
		move = move->next;
	}
	return result;
}

node *mult_polynomial(node *a, node *b)
{
	node *result, *move;
	result = make_node(NULL, NULL); /* create head node */
	a = a->next;
	while (a)
	{
		move = b->next;
		while (move)
		{
			insert_node(result, a->degree + move->degree, a->coef * move->coef);
			move = move->next;
		}
		a = a->next;
	}
	return result;
}

node *diff_polynomial(node *a)
{
	node *result;
	result = make_node(NULL, NULL);
	a = a->next;
	while (a)
	{
		if (a->degree > 0)
			insert_node(result, a->degree - 1, a->coef * a->degree);
		a = a->next;
	}
	return result;
}

node *integ_polynomial(node *a)
{ /* constant term is ignored */
	node *result;
	result = make_node(NULL, NULL);
	a = a->next;
	while (a)
	{
		insert_node(result, a->degree + 1, a->coef / (a->degree + 1));
		a = a->next;
	}
	return result;
}

double pow_simple(double x, int order)
{
	double result = 1;
	while (order)
	{
		result *= x;
		order--;
	}
	return result;
}

double pow_modified(double x, int order)
{
	double result = 1.;
	int bit = 1;
	while (bit <= order)
	{
		if (bit & order)
			result *= x;
		x *= x;
		bit <<= 1;
	}
	return result;
}

double eval_horner_simple(node *a, double x)
{
	double result = 0; /* Horner's rule -- pow_simple */
	int order = a->degree;
	while (a->next)
	{
		a = a->next;
		result = result * pow_simple(x, abs(order - a->degree)) + a->coef;
		order = a->degree;
	}
	return result * pow_simple(x, order);
}

double eval_horner_math(node *a, double x)
{
	double result = 0;
	int order = a->degree; /* Horner's rule -- pow <math> */
	while (a->next)
	{
		a = a->next;
		result = result * pow(x, abs(order - a->degree)) + a->coef;
		order = a->degree;
	}
	return result * pow(x, order);
}

double eval_horner_modified(node *a, double x)
{
	double result = 0.0;
	int order = a->degree; /* Horner's rule -- pow_modified */
	while (a->next)
	{
		a = a->next;
		result = result * pow_modified(x, abs(order - a->degree)) + a->coef;
		order = a->degree;
	}
	return (double)(result * pow_modified(x, order));
}

double eval_simple(node *a, double x)
{
	double result = 0; /* brust force -- pow_node */
	while (a->next)
	{
		a = a->next;
		result += pow_simple(x, a->degree) * a->coef;
	}
	return result;
}

double eval_math(node *a, double x)
{
	double result = 0;
	while (a->next) /* brust force -- pow <math> */
	{
		a = a->next;
		result += pow(x, a->degree) * a->coef;
	}
	return result;
}

double eval_modified(node *a, double x)
{
	double result = 0;
	while (a->next) /* brust force -- pow_modified */
	{
		a = a->next;
		result += pow_modified(x, a->degree) * a->coef;
	}
	return result;
}

double combi(double a[][2], int n, int r)
{
	int i;
	double result = 1;
	if (n <= 0 || r < 0 || n < r)
		return 0;
	if (n == r)
	{
		for (i = 0; i < n; i++)
			result *= a[i][0];
		return result;
	}
	if (r == 0)
		return 1;
	else
		return a[n - 1][0] * combi(a, n - 1, r - 1) + combi(a, n - 1, r);
}

node *lagrange(double a[][2], int n)
{
	int i, j;
	int sign;
	double coef, temp;
	node *result;
	result = make_node(NULL, NULL);
	for (i = 0; i < n; i++)
	{
		if (!a[i][1])
			continue;
		coef = a[i][1];
		for (j = 0; j < n; j++)
		{
			if (i == j)
				continue;
			if (a[i][0] == a[j][0])
			{
				remove_polynomial(result);
				return NULL;
			}
			coef /= a[i][0] - a[j][0];
		}
		temp = a[i][0];
		a[i][0] = 0;
		for (j = 0; j < n; j++)
		{
			if (j % 2)
				sign = -1;
			else
				sign = 1;
			insert_node(result, n - j - 1, sign * coef * combi(a, n, j));
		}
		a[i][0] = temp;
	}
	return result;
}

void print_polynomial(node *a, char *p)
{
	int x;
	printf("\n%s", p);
	while (a->next)
	{
		a = a->next;
		printf("%+lgx^%d ", a->coef, a->degree);
	}
}

void remove_polynomial(node *a)
{
	node *b;
	while (a)
	{
		b = a;
		free(b);
		a = a->next;
	}
}

node *input_polynomial(FILE *file)
{
	int order;
	double coef;
	node *result = make_node(NULL, NULL); /* ������ */
	do
	{
		printf("\ncoeff-> ");
		fscanf(file, "%lf", &coef);
		printf("\norder-> ");
		fscanf(file, "%d", &order);
		if (!coef)
			break;
		insert_node(result, order, coef);
	} while (1);
	return result;
}
