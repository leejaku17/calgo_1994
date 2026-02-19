/*                                                                  */
/*   COMBI.C  :  Get Sequence of P, C, PI, H    	            */
/*                                                                  */
/*                     Programmed By Kim Junmo                      */
/*                                                                  */

#include <stdio.h>
#include "timer.h"
#define MAX 40

int a[MAX], b[40];
int num;

void print_array(int q)
{
    printf("\n%4d :", ++num);
    while (q)
    {
        printf(" %d", b[--q]);
    }
}

void change(int *x, int *y)
{
    int t;
    t = *y;
    *y = *x;
    *x = t;
}

void C(int n, int r, int q)
{
    if (r == 0)
        print_array(q);
    else if (n < r)
        return;
    else
    {
        b[r - 1] = a[n - 1];
        C(n - 1, r - 1, q);
        C(n - 1, r, q);
    }
}

void H(int n, int r, int q)
{
    if (r == 0)
        print_array(q);
    else if (n == 0)
        return;
    else
    {
        b[r - 1] = a[n - 1];
        H(n, r - 1, q);
        H(n - 1, r, q);
    }
}

void P(int n, int r, int q)
{
    int i;
    if (r == 0)
        print_array(q);
    else
    {
        for (i = n - 1; i >= 0; i--)
        {
            change(&a[i], &a[n - 1]);
            b[r - 1] = a[n - 1];
            P(n - 1, r - 1, q);
            change(&a[i], &a[n - 1]);
        }
    }
}

void PI(int n, int r, int q)
{
    int i;
    if (r == 0)
        print_array(q);
    else
    {
        for (i = n - 1; i >= 0; i--)
        {
            change(&a[i], &a[n - 1]);
            b[r - 1] = a[n - 1];
            PI(n, r - 1, q);
            change(&a[i], &a[n - 1]);
        }
    }
}

void PI_2(int n, int r)
{
    int i;
    for (i = 0; i < r; i++)
        b[i] = 0;
    while (1)
    {
        print_array(r);
        b[0]++;
        for (i = 0; b[i] == n; i++)
        {
            if (i == r - 1)
                return;
            b[i + 1]++;
            b[i] = 0;
        }
    }
}

void C_2(int n, int r)
{
    int i, j;
    unsigned long x, s, s1, l, k;

    x = (unsigned long)(1UL << r) - 1UL;
    while (!(x & ~((unsigned long)(1UL << n) - 1UL)))
    {
        for (i = n - 1, j = 0, k = 1; i >= 0; i--, k <<= 1)
            if (x & k)
                b[j++] = a[i];
        print_array(r);
        s = x & -x;
        l = x + s;
        s1 = l & -l;
        k = ((s1 / s) >> 1) - 1;
        x = l | k;
    }
}

unsigned long combi_1(int n, int r)
{
    if (r == 0 || r == n)
        return 1;
    return combi_1(n - 1, r - 1) + combi_1(n - 1, r);
}

unsigned long c[17];
unsigned long _combi_2(int n, int r)
{
    if (r == 0 || r == n)
        return 1;
    if (!c[n - r - 1])
        c[n - r - 1] = _combi_2(n - 1, r - 1);
    c[n - r - 1] = c[n - r - 1] + _combi_2(n - 1, r);
    return c[n - r - 1];
}

unsigned long combi_2(int n, int r)
{
    int i;
    for (i = 0; i < 33; i++)
        c[i] = 0;
    if (n - r > r)
        r = n - r;
    if (n - r > 17)
        return 0;
    return _combi_2(n, r);
}

unsigned long combi_3(int n, int r)
{
    int i, j;
    unsigned long d[17];
    if (n - r < r)
        r = n - r;
    if (r == 0)
        return 1;
    if (r == 1)
        return n;
    if (r > 17)
        return 0;
    for (i = 1; i < r; i++)
        d[i] = i + 2;
    for (i = 3; i <= n - r + 1; i++)
    {
        d[0] = i;
        for (j = 1; j < r; j++)
            d[j] += d[j - 1];
    }
    return d[r - 1];
}

#define ITER 3000

long time_test(unsigned long (*func)(int, int), int n, int r)
{
    long t1, t2;
    int i;

    t1 = get_tick();
    for (i = 0; i < ITER; i++)
        func(n, r);
    t2 = get_tick();
    return diff_tick(t1, t2);
}

#define item_num 6
int amount = 102;
int table[2][item_num] = {3, 4, 7, 8, 9, 13,
                          4, 5, 10, 11, 13, 19};
void knapsack()
{
    int i, total_size = 0, value, max_val = 0, max_size = 0;
    int max_array[item_num];
    for (i = 0; i < item_num; i++)
        b[i] = 0;
    while (1)
    {
        b[0]++;
        for (i = 0; b[i] >= b[i] + (amount - total_size) / table[0][i] + 2; i++)
        {
            if (i == item_num - 1)
            {
                for (i = 0; i < item_num; i++)
                    printf("%d ", max_array[i]);
                printf("\ntotal size : %d \nvalue : %d", max_size, max_val);
                return;
            }
            b[i + 1]++;
            b[i] = 0;
        }
        total_size = 0;
        value = 0;
        for (i = 0; i < item_num; i++)
        {
            total_size += b[i] * table[0][i];
            value += b[i] * table[1][i];
        }
        if (total_size > amount)
            continue;
        if (max_val < value)
        {
            max_size = total_size;
            max_val = value;
            for (i = 0; i < item_num; i++)
                max_array[i] = b[i];
        }
    }
}

void main(void)
{
    int i;

    for (i = 0; i < MAX; i++)
        a[i] = i;

    clrscr();

    printf("\n");
    num = 0;
    C(5, 3, 3);
    getch();

    printf("\n");
    num = 0;
    C_2(5, 3);
    getch();

    printf("\n");
    num = 0;
    H(3, 3, 3);
    getch();

    printf("\n");
    num = 0;
    P(4, 3, 3);
    getch();

    printf("\n");
    num = 0;
    PI(3, 2, 2);
    getch();

    printf("\n");
    num = 0;
    PI_2(3, 2);
    getch();

    printf("\n\n combi_1 : %lu %ld", combi_1(15, 12),
           time_test(combi_1, 15, 12));
    printf("\n combi_2 : %lu %ld", combi_2(15, 12),
           time_test(combi_2, 15, 12));
    printf("\n combi_3 : %lu %ld", combi_3(15, 12),
           time_test(combi_3, 15, 12));
    getch();

    printf("\n\n");
    knapsack();
    getch();
}
