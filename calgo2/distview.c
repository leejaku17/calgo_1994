/*                                                                  */
/*   DISTVIEW.C  :  Viewer for Distribution of Random Number        */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <stdio.h>
#include <alloc.h>
#include <bios.h>
#include <stdlib.h>
#include <graphics.h>

#define X_LEFT 20
#define X_WIDTH 600
#define Y_HEIGHT 5000
#define Y_BOTTOM 300
#define BAR_WIDTH 4
#define NO_OF_BAR (600 / 4)

void view_dist(double rand_func(void), char *title, long n)
{
    int gd = DETECT, gm;
    int *count;
    long i;
    int h;
    initgraph(&gd, &gm, "c:\\tcpp\\bgi");
    if (graphresult() != grOk)
    {
        printf("\nGraphic system can't be initialized ! ");
        return -1;
    }
    if ((count = (int *)calloc(NO_OF_BAR, sizeof(int))) == NULL)
    {
        printf("\nMemory can't be allocated ! ");
        return -1;
    }

    setcolor(WHITE);
    line(X_LEFT, Y_BOTTOM, X_LEFT + X_WIDTH, Y_BOTTOM);

    for (i = 0; i < n; i++) /* Distribution Counting */
        count[(int)(rand_func() * NO_OF_BAR)]++;

    for (i = 0; i < NO_OF_BAR; i++)
    {
        h = (double)count[i] / n * Y_HEIGHT;
        line(X_LEFT + i * BAR_WIDTH, Y_BOTTOM,
             X_LEFT + i * BAR_WIDTH, Y_BOTTOM - h);
        line(X_LEFT + i * BAR_WIDTH + 1, Y_BOTTOM,
             X_LEFT + i * BAR_WIDTH + 1, Y_BOTTOM - h);
    }

    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(getmaxx() / 2, Y_BOTTOM + 10, title);

    bioskey(0);
}

double tc_random(void)
{
    return (rand() / (double)RAND_MAX);
}

double gauss2(void)
{
    return (rand() / (double)RAND_MAX + rand() / (double)RAND_MAX) / 2.0;
}

double gauss3(void)
{
    return (rand() / (double)RAND_MAX + rand() / (double)RAND_MAX + rand() / (double)RAND_MAX) / 3.0;
}

double gauss5(void)
{
    return (rand() / (double)RAND_MAX + rand() / (double)RAND_MAX + rand() / (double)RAND_MAX + rand() / (double)RAND_MAX + rand() / (double)RAND_MAX) / 5.0;
}

void main(void)
{
    long n;
    printf("\nDemonstration for Distribution of Random Number");
    printf("\nEnter number of random -> ");
    scanf("%ld", &n);
    view_dist(tc_random, "Turbo C++ random() function", n);
    view_dist(gauss2, "Gaussian Distribution By 2", n);
    view_dist(gauss3, "Gaussian Distribution By 3", n);
    view_dist(gauss5, "Gaussian Distribution By 5", n);
}
