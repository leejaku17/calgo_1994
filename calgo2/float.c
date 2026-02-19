/*                                                                  */
/*   FLOAT.C  :  View float format                                  */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <stdio.h>
#include <math.h>

unsigned long bits(unsigned long x, int k, int j)
{
    unsigned long zero = 0;
    return (x >> k) & ~(~zero << j);
}

unsigned long power(int b, int e)
{
    int i;
    unsigned long r = 1;
    for (i = 0; i < e; i++)
        r *= b;
    return r;
}

void print_binary(unsigned long l, int w)
{
    unsigned long i;
    i = power(2, w - 1);
    while (i > 0)
    {
        printf("%1d", l / i);
        l -= (l / i) * i;
        i /= 2;
    }
    printf(" ");
}

void float_format(float f)
{
    unsigned long l;
    memcpy(&l, &f, sizeof(float));
    printf("%f = ", f);
    print_binary(bits(l, 31, 1), 1);
    print_binary(bits(l, 23, 8), 8);
    printf("1");
    print_binary(bits(l, 0, 23), 23);
    printf(" (exp = %d)\n", bits(l, 23, 8) - 127);
}

void main(void)
{
    float f;
    while (1)
    {
        scanf("%f", &f);
        if (f == 0.0)
            return;
        float_format(f);
    }
}
