/*                                                                  */
/*   MATDEMO.C  :  Demonstration for Matrix Library                 */
/*                                                                  */
/*                     Programmed By Kim Junmo                      */
/*                       Moidified By Lee Jaekyu                    */

#include <stdio.h>
#include "matrix.h"

float matrix1[MAX][MAX];
float matrix2[MAX][MAX];
float result[MAX][MAX];

void main(int argc, char *argv[])
{
    int i, j;
    int n;
    FILE *fp;
    float f;
    if (argc < 2)
        fp = stdin;
    else if ((fp = fopen(argv[1], "rt")) == NULL)
    {
        printf("\n That file does not exist!");
        exit(1);
    }
    fscanf(fp, "%d", &n);
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
        {
            fscanf(fp, "%f", &f);
            matrix1[i][j] = f;
        }
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
        {
            fscanf(fp, "%f", &f);
            matrix2[i][j] = f;
        }
    fclose(fp);
    clrscr();

    puts("matrix1");
    print_matrix(matrix1, n);

    puts("matrix2");
    print_matrix(matrix2, n);
    getch();

    puts("matrix1 + matrix2");
    add_matrix(matrix1, matrix2, result, n);
    print_matrix(result, n);
    getch();

    puts("matrix1 - matrix2");
    sub_matrix(matrix1, matrix2, result, n);
    print_matrix(result, n);
    getch();

    puts("matrix1 * matrix2");
    mult_matrix(matrix1, matrix2, result, n);
    print_matrix(result, n);
    getch();

    puts("inverse of matrix1");
    if (invrs_matrix(matrix1, result, n))
    {
        print_matrix(result, n);
        puts("matrix1 * (matrix1 ^ -1)");
        mult_matrix(matrix1, result, matrix2, n);
        print_matrix(matrix2, n);
    }
    else
        printf("Does not exist inverse !\n");

    puts("det(matrix1)");
    printf("%10.3g", det(matrix1, n));
}
