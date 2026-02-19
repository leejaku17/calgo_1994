/*                                                                  */
/*   RANDWALK.C  :  Random Walk Problem                             */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <stdlib.h>
#include <bios.h>
#include <graphics.h>
#include <math.h>
#include <stdio.h>

double tc_random(void)
{
    return (double)rand() / (double)RAND_MAX;
}

double random_walk(long int n)
{
    long int i;
    double theta;
    double d;
    double px, py, px0, py0;

    px = px0 = getmaxx() / 2;
    py = py0 = getmaxy() / 2;
    moveto(px + 0.5, py + 0.5);
    setcolor(CYAN);

    for (i = 0; i < n; i++)
    {
        theta = tc_random() * 360;
        d = tc_random() * 3;
        px = px + d * cos(theta / 180. * M_PI);
        py = py + d * sin(theta / 180. * M_PI);
        lineto(px + 0.5, py + 0.5);
    }

    setcolor(YELLOW);
    fillellipse(px0, py0, 2, 2);

    setcolor(LIGHTRED);
    fillellipse(px + 0.5, py + 0.5, 2, 2);
    return sqrt((px - px0) * (px - px0) + (py - py0) * (py - py0));
}

void main(void)
{
    int gd = DETECT, gm;
    long int n;
    double dist;

    randomize();
    printf("\nInput number of try -> ");
    scanf("%ld", &n);

    initgraph(&gd, &gm, "c:\\tcpp\\bgi");
    if (graphresult() != grOk)
    {
        printf("\n Graphics system can't be initialized !");
        exit(1);
    }
    dist = random_walk(n);
    bioskey(0);
    closegraph();
    printf("\nDistance = %lf", dist);
}
