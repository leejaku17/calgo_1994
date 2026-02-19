/*                                                                  */
/*   2DRANGE.C  :  1 Dimensional Range Searching                    */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <stdlib.h>
#include <stdio.h>
#include "vga.h"

typedef struct _point
{
    int x, y;
} point;

typedef struct _range
{
    int x1, x2;
    int y1, y2;
} range;

int inside_x(int x, int x1, int x2)
{
    if (x1 > x2)
        swap(&x1, &x2);
    return (x1 <= x && x <= x2);
}

int inside_xy(point p, range r)
{
    if (r.x1 > r.x2)
        swap(&r.x1, &r.x2);
    if (r.y1 > r.y2)
        swap(&r.y1, &r.y2);
    return (r.x1 <= p.x && p.x <= r.x2 && r.y1 <= p.y && p.y <= r.y2);
}

void found(point p)
{
    draw_circle(p.x, p.y, 3, YELLOW);
}

int linear_search_2d(point p[], int n, range r)
{
    int i;
    int fn = 0;
    for (i = 0; i < n; i++)
    {
        if (inside_xy(p[i], r))
        {
            fn++;
            found(p[i]);
        }
    }
    return fn;
}

void linear_search_2d_test(int n)
{
    int i;
    range r;
    point *p;

    p = (point *)malloc(sizeof(point) * n);

    for (i = 0; i < n; i++)
    {
        p[i].x = random(maxx);
        p[i].y = random(maxy);
    }

    while (!bioskey(1))
    {
        clear_device();
        r.x1 = random(maxx);
        r.x2 = random(maxx);
        r.y1 = random(maxy);
        r.y2 = random(maxy);

        if (r.x1 > r.x2)
            swap(&r.x1, &r.x2);
        if (r.y1 > r.y2)
            swap(&r.y1, &r.y2);

        for (i = 0; i < n; i++)
            draw_filled_circle(p[i].x, p[i].y, 1, CYAN);

        draw_line(r.x1, r.y1, r.x2, r.y1, LIGHTRED);
        draw_line(r.x2, r.y1, r.x2, r.y2, LIGHTRED);
        draw_line(r.x2, r.y2, r.x1, r.y2, LIGHTRED);
        draw_line(r.x1, r.y2, r.x1, r.y1, LIGHTRED);

        linear_search_2d(p, n, r);

        delay(1000);
    }
    bioskey(0);
    free(p);
}

#define X_KEY 0
#define Y_KEY 1

void insert_sort(point p[], int n, int ip[], int key)
{
    int i, j, t;
    for (i = 1; i < n; i++)
    {
        t = ip[i];
        j = i;
        while ((key == X_KEY ? p[ip[j - 1]].x : p[ip[j - 1]].y) >
                   (key == X_KEY ? p[t].x : p[t].y) &&
               j > 0)
        {
            ip[j] = ip[j - 1];
            j--;
        }
        ip[j] = t;
    }
}

void project_preproc_2d(point p[], int n, int px[], int py[])
{
    int i;
    for (i = 0; i < n; i++)
        px[i] = py[i] = i;
    insert_sort(p, n, px, X_KEY);
    insert_sort(p, n, py, Y_KEY);
}

int projection_2d(point p[], int n, range r, int px[], int py[])
{
    int i1, i2;
    int left;
    int right;
    int i;
    int fn = 0;

    if (r.x2 - r.x1 < r.y2 - r.y1)
    {
        left = 0;
        right = n - 1;
        while (right >= left)
        {
            i1 = (right + left) / 2;
            if (r.x1 == p[px[i1]].x)
                break;
            if (r.x1 < p[px[i1]].x)
                right = i1 - 1;
            else
                left = i1 + 1;
        }
        if (!inside_x(p[px[i1]].x, r.x1, r.x2))
            i1++;

        left = 0;
        right = n - 1;
        while (right >= left)
        {
            i2 = (right + left) / 2;
            if (r.x2 == p[px[i2]].x)
                break;
            if (r.x2 < p[px[i2]].x)
                right = i2 - 1;
            else
                left = i2 + 1;
        }
        if (!inside_x(p[px[i2]].x, r.x1, r.x2))
            i2--;

        for (i = i1; i <= i2; i++)
        {
            fn++;
            if (inside_xy(p[px[i]], r))
                found(p[px[i]]);
        }
        return fn;
    }

    else
    {
        left = 0;
        right = n - 1;
        while (right >= left)
        {
            i1 = (right + left) / 2;
            if (r.y1 == p[py[i1]].y)
                break;
            if (r.y1 < p[py[i1]].y)
                right = i1 - 1;
            else
                left = i1 + 1;
        }
        if (!inside_x(p[py[i1]].y, r.y1, r.y2))
            i1++;

        left = 0;
        right = n - 1;
        while (right >= left)
        {
            i2 = (right + left) / 2;
            if (r.y2 == p[py[i2]].y)
                break;
            if (r.y2 < p[py[i2]].y)
                right = i2 - 1;
            else
                left = i2 + 1;
        }
        if (!inside_x(p[py[i2]].y, r.y1, r.y2))
            i2--;

        for (i = i1; i <= i2; i++)
        {
            fn++;
            if (inside_xy(p[py[i]], r))
                found(p[py[i]]);
        }
        return fn;
    }
}

void projection_2d_test(int n)
{
    int i;
    range r;
    point *p;
    int *px, *py;

    p = (point *)malloc(sizeof(point) * n);
    px = (int *)malloc(sizeof(int) * n);
    py = (int *)malloc(sizeof(int) * n);

    for (i = 0; i < n; i++)
    {
        p[i].x = random(maxx);
        p[i].y = random(maxx);
    }
    project_preproc_2d(p, n, px, py);

    while (!bioskey(1))
    {
        clear_device();

        r.x1 = random(maxx);
        r.x2 = random(maxx);
        r.y1 = random(maxy);
        r.y2 = random(maxy);

        if (r.x1 > r.x2)
            swap(&r.x1, &r.x2);
        if (r.y1 > r.y2)
            swap(&r.y1, &r.y2);

        for (i = 0; i < n; i++)
            draw_filled_circle(p[i].x, p[i].y, 1, CYAN);

        draw_line(r.x1, r.y1, r.x2, r.y1, LIGHTRED);
        draw_line(r.x2, r.y1, r.x2, r.y2, LIGHTRED);
        draw_line(r.x2, r.y2, r.x1, r.y2, LIGHTRED);
        draw_line(r.x1, r.y2, r.x1, r.y1, LIGHTRED);

        projection_2d(p, n, r, px, py);

        delay(1000);
    }
    bioskey(0);
    free(p);
    free(px);
    free(py);
}

typedef struct _gnode
{
    point p;
    struct _gnode *next;
} gnode;

#define MAX_GRID_X 32
#define MAX_GRID_Y 24
#define GRID_SIZE 20

gnode *get_grid(gnode **grid, int i, int j)
{
    return grid[i * MAX_GRID_Y + j];
}

void put_grid(gnode **grid, int i, int j, gnode *t)
{
    grid[i * MAX_GRID_Y + j] = t;
}

void insert_grid(point p, gnode **grid)
{
    gnode *t;
    t = (gnode *)malloc(sizeof(gnode));
    if (t == NULL)
        exit(1);
    t->p = p;
    t->next = get_grid(grid, p.x / GRID_SIZE, p.y / GRID_SIZE);
    put_grid(grid, p.x / GRID_SIZE, p.y / GRID_SIZE, t);
}

void grid_preproc_2d(point p[], int n, gnode **grid)
{
    int i, j;

    for (i = 0; i < MAX_GRID_X; i++)
        for (j = 0; j < MAX_GRID_Y; j++)
            put_grid(grid, i, j, NULL);
    for (i = 0; i < n; i++)
        insert_grid(p[i], grid);
}

int grid_2d(gnode **grid, range r)
{
    gnode *t;
    int fn = 0;
    int i, j;
    for (i = r.x1 / GRID_SIZE; i <= r.x2 / GRID_SIZE; i++)
        for (j = r.y1 / GRID_SIZE; j <= r.y2 / GRID_SIZE; j++)
            for (t = get_grid(grid, i, j); t != NULL; t = t->next)
                if (inside_xy(t->p, r))
                {
                    fn++;
                    found(t->p);
                }
    return fn;
}

void grid_2d_test(int n)
{
    int i;
    range r;
    point *p;
    gnode **grid;

    p = (point *)malloc(sizeof(point) * n);
    grid = (gnode **)malloc(sizeof(gnode *) * MAX_GRID_X * MAX_GRID_Y);

    for (i = 0; i < n; i++)
    {
        p[i].x = random(maxx);
        p[i].y = random(maxx);
    }

    grid_preproc_2d(p, n, grid);

    while (!bioskey(1))
    {
        clear_device();

        r.x1 = random(maxx);
        r.x2 = random(maxx);
        r.y1 = random(maxy);
        r.y2 = random(maxy);

        if (r.x1 > r.x2)
            swap(&r.x1, &r.x2);
        if (r.y1 > r.y2)
            swap(&r.y1, &r.y2);

        for (i = 0; i < n; i++)
            draw_filled_circle(p[i].x, p[i].y, 1, CYAN);

        draw_line(r.x1, r.y1, r.x2, r.y1, LIGHTRED);
        draw_line(r.x2, r.y1, r.x2, r.y2, LIGHTRED);
        draw_line(r.x2, r.y2, r.x1, r.y2, LIGHTRED);
        draw_line(r.x1, r.y2, r.x1, r.y1, LIGHTRED);

        grid_2d(grid, r);

        delay(1000);
    }
    bioskey(0);
    free(p);
    free(grid);
}

typedef struct _node
{
    struct _node *left;
    struct _node *right;
    point p;
} node;

void init_btree(node **p)
{
    *p = (node *)malloc(sizeof(node));
    (*p)->left = NULL;
    (*p)->right = NULL;
}

void btree_search_2d(node *t, range r, int d)
{
    int c1, c2;
    if (t == NULL)
        return;
    c1 = (d ? t->p.x >= r.x1 : t->p.y >= r.y1);
    c2 = (d ? t->p.x <= r.x2 : t->p.y <= r.y2);
    if (c1)
        btree_search_2d(t->left, r, !d);
    if (inside_xy(t->p, r))
        found(t->p);
    if (c2)
        btree_search_2d(t->right, r, !d);
}

void btree_insert_2d(node *base, point po)
{
    node *p, *s;
    int d = 0;
    int c;
    p = base;
    s = base->left;
    while (s != NULL)
    {
        p = s;
        c = d ? po.x < s->p.x : po.y < s->p.y;
        if (c)
            s = s->left;
        else
            s = s->right;
        d = !d;
    }
    if ((s = (node *)malloc(sizeof(node))) == NULL)
        return;
    s->p = po;
    s->left = NULL;
    s->right = NULL;
    if (c || p == base)
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
        printf("%d,%d\n", p->p.x, p->p.y);
        btree_list(p->left);
        x -= 2;
    }
}

void draw_points(node *p)
{
    if (p != NULL)
    {
        draw_points(p->left);
        draw_filled_circle(p->p.x, p->p.y, 1, CYAN);
        draw_points(p->right);
    }
}

void btree_search_2d_test(int n)
{
    int i;
    range r;
    point po;
    node *p;

    init_btree(&p);

    for (i = 0; i < n; i++)
    {
        po.x = random(maxx);
        po.y = random(maxy);
        btree_insert_2d(p, po);
    }

    while (!bioskey(1))
    {
        clear_device();

        draw_points(p->left);

        r.x1 = random(maxx);
        r.x2 = random(maxx);
        r.y1 = random(maxy);
        r.y2 = random(maxy);

        if (r.x1 > r.x2)
            swap(&r.x1, &r.x2);
        if (r.y1 > r.y2)
            swap(&r.y1, &r.y2);

        draw_line(r.x1, r.y1, r.x2, r.y1, LIGHTRED);
        draw_line(r.x2, r.y1, r.x2, r.y2, LIGHTRED);
        draw_line(r.x2, r.y2, r.x1, r.y2, LIGHTRED);
        draw_line(r.x1, r.y2, r.x1, r.y1, LIGHTRED);

        btree_search_2d(p->left, r, 0);

        delay(1000);
    }
    bioskey(0);
    btree_deleteall(p);
    free(p);
}

void main(void)
{
    randomize();
    set_vga(VGA640);

    linear_search_2d_test(1000);
    projection_2d_test(1000);
    grid_2d_test(1000);
    btree_search_2d_test(1000);

    set_text();
}
