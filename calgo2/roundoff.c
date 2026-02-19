/*                                                                  */
/*   ROUNDOFF.C  :  Demo for round-off error                        */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <stdio.h>
#include <math.h>
#include <float.h>

void sum_bad(void)
{
    int i;
    float sum = 1000.0;
    printf("\nBad summation for 1000.0 + 10000*0.0001");
    for (i = 0; i < 10000; i++)
        sum = sum + 0.0001f;
    printf("\nSum is = %f", sum);
}

void sum_group(void)
{
    int i;
    float sum = 1000.0, group = 0.0;
    printf("\nGrouping summation for 1000.0 + 10000*0.0001");
    for (i = 1; i <= 10000; i++)
        group = group + 0.0001f;
    sum = sum + group;
    printf("\nSum is = %f", sum);
}

void sum_double(void)
{
    int i;
    double sum = 1000.0;
    printf("\nDouble precision summation for 1000.0 + 10000*0.0001");
    for (i = 0; i < 10000; i++)
        sum = sum + 0.0001f;
    printf("\nSum is = %lf", sum);
}

void main(void)
{
    sum_bad();
    sum_group();
    sum_double();
}
