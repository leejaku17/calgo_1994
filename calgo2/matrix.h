/*                                                                  */
/*   MATRIX.H  :  Matrix Library Header                             */
/*                                                                  */
/*                     Programmed By Kim Junmo                      */
/*                       Modified By Lee Jaekyu                     */

#ifndef _MATRIX_H
#define _MATRIX_H

#define MAX 5
#define ERROR 10e-10

void add_matrix(float pm1[][MAX], float pm2[][MAX],
				float result[][MAX], int rc);
void sub_matrix(float pm1[][MAX], float pm2[][MAX],
				float result[][MAX], int rc);
void mult_matrix(float pm1[][MAX], float pm2[][MAX],
				 float result[][MAX], int rc);
int invrs_matrix(float pm1[][MAX], float result[][MAX], int rc);
float det(float pm1[][MAX], int n);
void mult_vector(float vector[], float matrix[][MAX],
				 float result[], int n);
void print_matrix(float m[][MAX], int n);

#endif
