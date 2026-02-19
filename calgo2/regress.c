/*                                                                  */
/*   REGRESS.C  :  Regression    	                            */
/*                                                                  */
/*                     Programmed By Kim Junmo                      */
/*                                                                  */

#include <stdio.h>
#include <graphics.h>
#include <stdlib.h>
#include <math.h>
#include "matrix.h"

#define F_NUM 4
#define D_NUM 6

double sumx(int l, double t[][2], double (*f1)(double), double (*f2)(double))
{
    int i;
    double result = 0.;

    for (i = 0; i < l; i++)
        result += f1(t[i][0]) * f2(t[i][0]);
    return result;
}

double sumy(int l, double t[][2], double (*f)(double))
{
    int i;
    double result = 0.;

    for (i = 0; i < l; i++)
        result += t[i][1] * f(t[i][0]);
    return result;
}

int regress(int l, double t[][2], int n, double (*f[])(double), double c[])
{
    int i, j;
    double m[MAX][MAX], invr[MAX][MAX], v[MAX];

    for (i = 0; i < n; i++)
    {
        v[i] = sumy(l, t, f[i]);
        for (j = 0; j < n; j++)
            m[i][j] = sumx(l, t, f[i], f[j]);
    }
    if (!invrs_matrix(m, invr, n))
        return 0;
    mult_vector(v, invr, c, n);
    return 1;
}

int regress_1(int l, double t[][2], double c[])
{
    int i, j;
    double sx = 0, sy = 0, sxx = 0, sxy = 0, d;

    for (i = 0; i < l; i++)
        sx += t[i][0];
    for (i = 0; i < l; i++)
        sy += t[i][1];
    for (i = 0; i < l; i++)
        sxx += t[i][0] * t[i][0];
    for (i = 0; i < l; i++)
        sxy += t[i][0] * t[i][1];

    d = l * sxx - sx * sx;
    c[0] = (sxx * sy - sx * sxy) / d;
    c[1] = (l * sxy - sx * sy) / d;

    return 1;
}

double f0(double x)
{
    return 1;
}

double f1(double x)
{
    return x;
}

double f2(double x)
{
    return sin(x);
}

double f3(double x)
{
    return exp(x);
}

double fd_4(double x, double t[])
{
    return t[0] * f0(x) + t[1] * f1(x) + t[2] * f2(x) + t[3] * f3(x);
}

double fd_1(double x, double t[])
{
    return t[0] * f0(x) + t[1] * f1(x);
}

#define XP2R(x) (((x) - cx) * mag + midx)
#define YP2R(y) (midy - ((y) - cy) * mag)
#define PI 3.141592

void draw(double (*func)(double, double *), double t[], double cx, double cy,
          double x1, double x2, double mag, double step)
{
    int midx, midy;
    double x, y;
    midx = getmaxx() / 2;
    midy = getmaxy() / 2;
    line(0, YP2R(0), getmaxx(), YP2R(0));
    line(XP2R(0), 0, XP2R(0), getmaxy());
    x = x1;
    y = func(x, t);
    putpixel(XP2R(x), YP2R(y), getcolor());
    moveto(XP2R(x), YP2R(y));
    for (x = x1 + step; x <= x2; x += step)
    {
        y = func(x, t);
        lineto(XP2R(x), YP2R(y));
    }
    y = func(x, t);
    lineto(XP2R(x2), YP2R(y));
}

void draw_points(double p[][2], int n, double cx, double cy, double mag)
{
    int midx, midy;
    int i;
    double x, y;
    midx = getmaxx() / 2;
    midy = getmaxy() / 2;
    for (i = 0; i < n; i++)
        circle(XP2R(p[i][0]), YP2R(p[i][1]), 3);
}

void main(void)
{
    int gd = DETECT, gm;
    double c[F_NUM];
    double (*fp_a[F_NUM])(double);
    double table[6][2] = {0.1, 0.61,
                          0.4, 0.92,
                          0.5, 0.99,
                          0.7, 1.52,
                          0.7, 1.47,
                          0.9, 2.03};
    fp_a[0] = f0;
    fp_a[1] = f1;
    fp_a[2] = f2;
    fp_a[3] = f3;

    initgraph(&gd, &gm, "");

    regress_1(D_NUM, table, c);
    printf("%lf %lf", c[0], c[1]);
    draw_points(table, D_NUM, 1, 1, 200);
    draw(fd_1, c, 1, 1, -1, 3, 200, 0.1);
    getch();

    cleardevice();

    regress(D_NUM, table, F_NUM, fp_a, c);
    printf("%lf %lf %lf %lf", c[0], c[1], c[2], c[3]);
    draw_points(table, D_NUM, 1, 1, 200);
    draw(fd_4, c, 1, 1, -1, 3, 200, 0.1);
    getch();

    closegraph();
}
