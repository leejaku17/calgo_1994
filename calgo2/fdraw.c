/*                                                                  */
/*   FDRAW.C  :  Draw Function y = f(x) using Expression Parser     */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <stdio.h>
#include <graphics.h>
#include <math.h>
#include <graphics.h>
#include "exparser.h"

#define XP2R(x) (((x) - cx) * mag + midx)
#define YP2R(y) (midy - ((y) - cy) * mag)
#define PI 3.141592

void draw(char *func, double cx, double cy,
          double x1, double x2, double mag, double step)
{
    int midx, midy;
    double x, y;
    midx = getmaxx() / 2;
    midy = getmaxy() / 2;
    line(0, YP2R(0), getmaxx(), YP2R(0));
    line(XP2R(0), 0, XP2R(0), getmaxy());
    x = x1;
    init_token(func);
    put_var('x', x);
    eval_exp(&y);
    putpixel(XP2R(x), YP2R(y), getcolor());
    moveto(XP2R(x), YP2R(y));
    for (x = x1 + step; x <= x2; x += step)
    {
        init_token(func);
        put_var('x', x);
        eval_exp(&y);
        lineto(XP2R(x), YP2R(y));
    }
    init_token(func);
    put_var('x', x);
    eval_exp(&y);
    lineto(XP2R(x2), YP2R(y));
}

void main(void)
{
    int gd = DETECT, gm;
    char s[MAXLEN];
    while (1)
    {
        gets(s);
        if (s[0] == '#')
            exit(0);
        initgraph(&gd, &gm, "c:\\tcpp\\bgi");
        init_var();
        draw(s, 0, 0, -16, 16, 20, 0.01);
        bioskey(0);
        closegraph();
    }
}
