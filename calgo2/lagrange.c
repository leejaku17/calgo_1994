/*                                                                  */
/*   LAGRANGE.C  :  Demonstration of Lagrange interpolation         */
/*                                                                  */
/*                     Programmed By Kim Junmo                      */
/*                       Modified By Lee Jaekyu                     */

#include <math.h>
#include <stdio.h>
#include <graphics.h>
#include "polylist.h"

#define XP2R(x) (((x) - cx) * mag + midx)
#define YP2R(y) (midy - ((y) - cy) * mag)
#define PI 3.141592

void draw(node *poly, float cx, float cy,
          float x1, float x2, float mag, float step)
{
    int midx, midy;
    int i;
    float x, y;
    midx = getmaxx() / 2;
    midy = getmaxy() / 2;
    line(0, YP2R(0), getmaxx(), YP2R(0));
    line(XP2R(0), 0, XP2R(0), getmaxy());
    x = x1;
    y = eval_horner_modified(poly, x);
    putpixel(XP2R(x), YP2R(y), getcolor());
    moveto(XP2R(x), YP2R(y));
    for (x = x1 + step; x <= x2; x += step)
    {
        y = eval_horner_modified(poly, x);
        lineto(XP2R(x), YP2R(y));
    }
    y = eval_horner_modified(poly, x);
    lineto(XP2R(x2), YP2R(y));
}

void draw_points(double p[][2], int n, float cx, float cy, float mag)
{
    int midx, midy;
    int i;
    float x, y;
    midx = getmaxx() / 2;
    midy = getmaxy() / 2;
    for (i = 0; i < n; i++)
        circle(XP2R(p[i][0]), YP2R(p[i][1]), 3);
}

void main(void)
{
    int gd = DETECT, gm;
    double points[6][2] = {-3, 2, -2, -1.5, -1, 0, 0, 0, 1, 1, 2, -1.5};
    node *poly;
    if ((poly = lagrange(points, 6)) != NULL)
        print_polynomial(poly, "poly = ");
    else
        printf("Impossible !!!!");
    getch();
    initgraph(&gd, &gm, "c:\\tcpp\\bgi");
    draw_points(points, 6, 0, 0, 100);
    draw(poly, 0, 0, -5, 5, 100, 0.05);
    getch();
    closegraph();
}