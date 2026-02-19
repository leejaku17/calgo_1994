/*                                                                  */
/*   TAYLOR.C  :  Taylor series and trucation error                 */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

/*  sin(x) near 0, and its first, second, third terms */

#include <stdio.h>
#include <graphics.h>
#include <math.h>
#include <bios.h>

float original(float x)
{
    return sin(x);
}

float first(float x)
{
    return x;
}

float second(float x)
{
    return x - x * x * x / 6.;
}

float third(float x)
{
    return x - x * x * x / 6. + x * x * x * x * x / 120.;
}

float fourth(float x)
{
    return x - x * x * x / 6. + x * x * x * x * x / 120. - x * x * x * x * x * x * x / 5040.;
}

#define XP2R(x) (((x) - cx) * mag + midx)
#define YP2R(y) (midy - ((y) - cy) * mag)
#define PI 3.141592

void draw(float (*func)(float), float cx, float cy,
          float x1, float x2, float mag, float step)
{
    int midx, midy;
    float x, y;
    midx = getmaxx() / 2;
    midy = getmaxy() / 2;
    line(0, YP2R(0), getmaxx(), YP2R(0));
    line(XP2R(0), 0, XP2R(0), getmaxy());
    x = x1;
    y = func(x);
    putpixel(XP2R(x), YP2R(y), getcolor());
    moveto(XP2R(x), YP2R(y));
    for (x = x1 + step; x <= x2; x += step)
    {
        y = func(x);
        lineto(XP2R(x), YP2R(y));
    }
    y = func(x);
    lineto(XP2R(x2), YP2R(y));
}

void main(void)
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "c:\\tcpp\\bgi");
    draw(original, 0, 0, -PI, PI, 100, 0.1);
    bioskey(0);
    draw(first, 0, 0, -PI, PI, 100, 0.1);
    bioskey(0);
    draw(second, 0, 0, -PI, PI, 100, 0.1);
    bioskey(0);
    draw(third, 0, 0, -PI, PI, 100, 0.1);
    bioskey(0);
    draw(fourth, 0, 0, -PI, PI, 100, 0.1);
    bioskey(0);
    closegraph();
}
