/*                                                                  */
/*   1DRANGE.C  :  1 Dimensional Range Searching                    */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <stdlib.h>
#include <stdio.h>
#include "vga.h"

int inside_x(int x, int x1, int x2)
{
    if (x1 > x2)
        swap(&x1, &x2);
    return (x1 <= x && x <= x2);
}

void found(int x)
{
    draw_circle(x, maxy / 2, 3, YELLOW);
}

int linear_search_1d(int p[], int n, int x1, int x2)
{
    int i;
    int fn = 0;
    for (i = 0; i < n; i++)
    {
        if (inside_x(p[i], x1, x2))
        {
            fn++;
            found(p[i]);
        }
    }
    return fn;
}

void linear_search_1d_test(int n)
{
    int i;
    int rx1, rx2;
    int *p;

    p = (int *)malloc(sizeof(int) * n);

    for (i = 0; i < n; i++)
        p[i] = random(maxx);

    while (!bioskey(1))
    {
        clear_device();

        rx1 = random(maxx);
        rx2 = random(maxx);

        for (i = 0; i < n; i++)
            draw_filled_circle(p[i], maxy / 2, 1, CYAN);

        draw_line(rx1, maxy / 2 - 10, rx1, maxy / 2 + 10, LIGHTRED);
        draw_line(rx2, maxy / 2 - 10, rx2, maxy / 2 + 10, LIGHTRED);

        linear_search_1d(p, n, rx1, rx2);

        delay(1000);
    }
    bioskey(0);
    free(p);
}

void insert_sort(int p[], int n)
{
    int i, j, t;
    for (i = 1; i < n; i++)
    {
        t = p[i];
        j = i;
        while (p[j - 1] > t && j > 0)
        {
            p[j] = p[j - 1];
            j--;
        }
        p[j] = t;
    }
}

int bin_search_1d(int p[], int n, int x1, int x2)
{
    int rx1, rx2;
    int left = 0;
    int right = n - 1;
    int i;
    int fn = 0;

    if (x1 > x2)
        swap(&x1, &x2);
    while (right >= left)
    {
        rx1 = (right + left) / 2;
        if (x1 == p[rx1])
            break;
        if (x1 < p[rx1])
            right = rx1 - 1;
        else
            left = rx1 + 1;
    }
    if (!inside_x(p[rx1], x1, x2))
        rx1++;

    left = 0;
    right = n - 1;
    while (right >= left)
    {
        rx2 = (right + left) / 2;
        if (x2 == p[rx2])
            break;
        if (x2 < p[rx2])
            right = rx2 - 1;
        else
            left = rx2 + 1;
    }
    if (!inside_x(p[rx2], x1, x2))
        rx2--;

    for (i = rx1; i <= rx2; i++)
    {
        fn++;
        found(p[i]);
    }
    return fn;
}

void bin_search_1d_test(int n)
{
    int i;
    int rx1, rx2;
    int *p;

    p = (int *)malloc(sizeof(int) * n);

    for (i = 0; i < n; i++)
        p[i] = random(maxx);

    insert_sort(p, n);

    while (!bioskey(1))
    {
        clear_device();

        rx1 = random(maxx);
        rx2 = random(maxx);

        for (i = 0; i < n; i++)
            draw_filled_circle(p[i], maxy / 2, 1, CYAN);

        draw_line(rx1, maxy / 2 - 10, rx1, maxy / 2 + 10, LIGHTRED);
        draw_line(rx2, maxy / 2 - 10, rx2, maxy / 2 + 10, LIGHTRED);

        bin_search_1d(p, n, rx1, rx2);

        delay(1000);
    }
    bioskey(0);
    free(p);
}

typedef struct _node
{
    struct _node *left;
    struct _node *right;
    int x;
} node;

node *head;

void init_btree(node **p)
{
    *p = (node *)malloc(sizeof(node));
    (*p)->left = NULL;
    (*p)->right = NULL;
}

void btree_search_1d(node *s, int x1, int x2)
{
    if (s == NULL)
        return;
    if (s->x >= x1)
        btree_search_1d(s->left, x1, x2);
    if (inside_x(s->x, x1, x2))
        found(s->x);
    if (s->x <= x2)
        btree_search_1d(s->right, x1, x2);
}

void btree_insert(node *base, int x)
{
    node *p, *s;
    p = base;
    s = base->left;
    while (s != NULL)
    {
        p = s;
        if (x < s->x)
            s = s->left;
        else
            s = s->right;
    }
    if ((s = (node *)malloc(sizeof(node))) == NULL)
        return;
    s->x = x;
    s->left = NULL;
    s->right = NULL;
    if (x < p->x || p == base)
        p->left = s;
    else
        p->right = s;
}

void _deleteall(node *t)
{
    if (t != NULL)
    {
        _deleteall(t->left);
        _deleteall(t->right);
        free(t);
    }
}

void btree_deleteall(node *base)
{
    node *t;
    t = base->left;
    _deleteall(t);
    base->left = NULL;
    base->right = NULL;
}

void btree_list(node *p)
{
    int i;
    static int x = 0;
    if (p != NULL)
    {
        x += 2;
        btree_list(p->right);
        for (i = 2; i < x; i++)
            printf("   ");
        printf("%d\n", p->x);
        btree_list(p->left);
        x -= 2;
    }
}

void draw_points(node *p)
{
    if (p != NULL)
    {
        draw_points(p->left);
        draw_filled_circle(p->x, maxy / 2, 1, CYAN);
        draw_points(p->right);
    }
}

void btree_search_1d_test(int n)
{
    int m;
    int i;
    int rx1, rx2;
    node *p;

    init_btree(&p);

    for (i = 0; i < n; i++)
        btree_insert(p, random(maxx));

    while (!bioskey(1))
    {
        clear_device();

        draw_points(p->left);

        rx1 = random(maxx);
        rx2 = random(maxx);

        if (rx1 > rx2)
            swap(&rx1, &rx2);

        draw_line(rx1, maxy / 2 - 10, rx1, maxy / 2 + 10, LIGHTRED);
        draw_line(rx2, maxy / 2 - 10, rx2, maxy / 2 + 10, LIGHTRED);

        btree_search_1d(p->left, rx1, rx2);

        delay(1000);
    }
    bioskey(0);
    btree_deleteall(p);
    free(p);
}

void main(void)
{
    set_vga(VGA640);
    linear_search_1d_test(50);
    bin_search_1d_test(50);
    btree_search_1d_test(50);
    set_text();
}
