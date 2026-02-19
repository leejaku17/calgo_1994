/*                                                                  */
/*   MATRIX.C  :  Matrix Library  			            */
/*                                                                  */
/*                     Programmed By Kim Junmo                      */
/*                       Modified By Lee Jaekyu                     */

#include <math.h>
#include <alloc.h>
#include "matrix.h"

void print_matrix(float m[][MAX], int n)
{
	int i, j;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
			printf(" %10.3g", m[i][j]);
		printf("\n");
	}
	printf("\n");
}

float det(float pm1[][MAX], int n)
{
	int i, j, k;
	float c1, c2;
	float dpm1[MAX][MAX];

	for (i = 0; i < n; i++) /* copy to work area */
		for (j = 0; j < n; j++)
			dpm1[i][j] = pm1[i][j];

	for (i = 0; i < n; i++) /* forbid pivot elem zero */
		if (dpm1[i][i] == 0.)
			for (k = 0; k < n; k++)
			{
				if (k == i)
					continue;
				for (j = 0; j < n; j++)
					dpm1[i][j] += dpm1[k][j];
				if (dpm1[i][i] != 0.)
					break;
			}
	for (i = 0; i < n; i++) /* make upper triangular matrix */
	{
		c1 = dpm1[i][i];
		if (c1 < ERROR && c1 > -ERROR)
			return 0;
		for (k = i + 1; k < n; k++)
		{
			if (dpm1[k][i] == 0)
				continue;
			c2 = dpm1[k][i];
			for (j = 0; j < n; j++)
				dpm1[k][j] = dpm1[k][j] - dpm1[i][j] / c1 * c2;
		}
	}
	k = 1;
	/* product of pivot elem is determiant */
	for (i = 0; i < n; i++)
		k *= dpm1[i][i];
	return k;
}

void add_matrix(float pm1[][MAX], float pm2[][MAX],
				float result[][MAX], int n)
{
	int i, j;

	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			result[i][j] = pm1[i][j] + pm2[i][j];
}

void sub_matrix(float pm1[][MAX], float pm2[][MAX],
				float result[][MAX], int n)
{
	int i, j;

	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			result[i][j] = pm1[i][j] - pm2[i][j];
}

void mult_vector(float vector[], float matrix[][MAX], float result[], int n)
{
	int i, j, k;

	for (i = 0; i < n; i++)
	{
		result[i] = 0;
		for (j = 0; j < n; j++)
			result[i] += vector[j] * matrix[i][j];
	}
}

void mult_matrix(float pm1[][MAX], float pm2[][MAX],
				 float result[][MAX], int n)
{
	int i, j, k;

	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
		{
			result[i][j] = 0;
			for (k = 0; k < n; k++)
				result[i][j] += pm1[i][k] * pm2[k][j];
		}
}

int invrs_matrix(float pm1[][MAX], float result[][MAX], int n)
{
	int i, j, k;
	float constant;
	float dpm1[MAX][MAX];

	for (i = 0; i < n; i++) /* copy to work area */
		for (j = 0; j < n; j++)
			dpm1[i][j] = pm1[i][j];

	for (i = 0; i < n; i++) /* make identity matrix */
		for (j = 0; j < n; j++)
			result[i][j] = (i == j) ? 1 : 0;

	for (i = 0; i < n; i++)
		if (dpm1[i][i] == 0.)
			for (k = 0; k < n; k++)
			{
				if (k == i)
					continue;
				for (j = 0; j < n; j++)
				{
					dpm1[i][j] += dpm1[k][j];
					result[i][j] += result[k][j];
				}
				if (dpm1[i][i] != 0)
					break;
			}

	for (i = 0; i < n; i++)
	{
		if (dpm1[i][i] < ERROR && dpm1[i][i] > -ERROR)
			return 0;
		constant = dpm1[i][i];
		for (j = 0; j < n; j++)
		{
			dpm1[i][j] /= constant;
			result[i][j] /= constant;
		}
		for (k = 0; k < n; k++)
		{
			if (k == i)
				continue;
			if (dpm1[k][i] == 0)
				continue;
			constant = dpm1[k][i];
			for (j = 0; j < n; j++)
			{
				dpm1[k][j] = dpm1[k][j] - dpm1[i][j] * constant;
				result[k][j] = result[k][j] - result[i][j] * constant;
			}
		}
	}
	return 1;
}
