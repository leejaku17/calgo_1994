/*                                                                  */
/*   FUNCDRAW.C  :  Draw implicit function          	            */
/*                                                                  */
/*                     Programmed By Kim Junmo                      */
/*                                                                  */

#include <stdlib.h>
#include <graphics.h>
#include <math.h>

#define TOLERANCE 1e-10
#define MAX_TRIES 1000

int found_flag;
double qq;
double (*f)(double, double);

double example(double x, double y)
{
    return pow(fabs(x), 2. / 3.) + pow(fabs(y), 2. / 3.) - 1;
    return pow(x, 100) + pow(y, 100) - 1;
    return ceil(x) - x - y / 2;
    return tan(x) - y;
}

double f_c(double q, double x0, double y0, double c)
{
    return f(c * cos(q) + x0, c * sin(q) + y0);
}

void bisect(double x1, double x2, double y1, double y2, double *x0, double *y0)
{
    double x, y;
    if (f(x1, y1) * f(x2, y2) > 0.0)
    {
        found_flag = 0;
        return;
    }
    while (fabs(x1 - x2) > TOLERANCE || fabs(y1 - y2) > TOLERANCE)
    {
        x = (x1 + x2) / 2.0;
        y = (y1 + y2) / 2.0;
        if (f(x1, y1) * f(x, y) <= 0.0)
        {
            x2 = x;
            y2 = y;
        }
        else
        {
            x1 = x;
            y1 = y;
        }
    }
    *x0 = x;
    *y0 = y;
    found_flag = 1;
    return;
}

double bisect_c(double x0, double y0, double c, double t)
{
    double q;
    double q1 = qq - M_PI_2 * t;
    double q2 = qq + M_PI_2 * t;

    if (f_c(q1, x0, y0, c) * f_c(q2, x0, y0, c) >= 0.0)
    {
        found_flag = 0;
        return qq;
    }
    while (fabs(q1 - q2) > TOLERANCE)
    {
        q = (q1 + q2) / 2.0;
        if (f_c(q1, x0, y0, c) * f_c(q, x0, y0, c) <= 0.0)
            q2 = q;
        else
            q1 = q;
    }
    found_flag = 1;
    return q;
}

void d_2d(double x, double y, double q, double c, int flag,
          double t0, double k, double x1, double y1)
{
    double x0, y0, t;

    moveto((x - x1) * k + 320 + 0.5, -(y - y1) * k + 240 + 0.5);
    x0 = x;
    y0 = y;
    qq = q;
    t = t0;

    while (1)
    {
        qq = bisect_c(x0, y0, c, t);
        if (found_flag == 0)
        {
            t += 0.01;
            continue;
        }
        t = t0;
        x0 = c * cos(qq) + x0;
        y0 = c * sin(qq) + y0;

        if ((x0 - x) * (x0 - x) + (y0 - y) * (y0 - y) < c * c / 2)
        {
            lineto((x - x1) * k + 320 + 0.5, -(y - y1) * k + 240 + 0.5);
            break;
        }
        if (x0 < -320. / k + x1 || x0 > 320. / k + x1 || y0 < -240. / k + y1 || y0 > 240. / k + y1)
        {
            if (flag)
                d_2d(x, y, q + M_PI, c, 0, t0, k, x1, y1);
            break;
        }
        lineto((x0 - x1) * k + 320 + 0.5, -(y0 - y1) * k + 240 + 0.5);
    }
}

int draw(double (*func)(double, double),
         double c, double t0, double k, int x1, int y1)
{
    double x, y, yl1, yl2, xl1, xl2, q;
    int step = 0;
    randomize();
    f = func;
    c /= k;
    do
    {
        step++;
        yl1 = (double)(random(480) - 240) / k + y1;
        yl2 = (double)(random(480) - 240) / k + y1;
        xl1 = (double)(random(640) - 320) / k + x1;
        xl2 = (double)(random(640) - 320) / k + x1;
        bisect(xl1, xl2, yl1, yl2, &x, &y);
        if (step > MAX_TRIES)
            return -1;
    } while (!found_flag);

    q = atan2(f(x, y + 1e-10) - f(x, y), f(x + 1e-10, y) - f(x, y)) + M_PI_2;
    d_2d(x, y, q, c, 1, t0, k, x1, y1);
    return 1;
}

void main()
{
    int i;
    int gd = DETECT, gm;

    initgraph(&gd, &gm, "c:\\tcpp\\bgi");
    line(320, 0, 320, 480);
    line(0, 240, 640, 240);
    for (i = 0; i < 1; i++)
        printf("%d", draw(example, 1, 0.01, 100, 0, 0));
    getch();
    closegraph();
}
