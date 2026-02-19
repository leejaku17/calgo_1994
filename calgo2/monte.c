/*                                                                  */
/*   MONTE.C    :   Get pi by Montecalro Method                     */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <stdio.h>
#include <bios.h>
#include <stdlib.h>
#include <graphics.h>

#define WIDTH 300

double tc_random(void)
{
    return rand() / (double)RAND_MAX;
}

int main(void)
{
    int gd = DETECT, gm;
    long in = 0, out = 0;
    int x1, y1;
    double x, y;
    long i, n;
    double pi;
    char pi_str[50];

    printf("\nEnter number Random ->");
    scanf("%ld", &n);

    initgraph(&gd, &gm, "d:\\lang\\tcpp\\bgi");
    if (graphresult() != grOk)
    {
        printf("\nGraphic system can't be initialized ! ");
        return -1;
    }

    setcolor(WHITE);
    x1 = (getmaxx() - WIDTH) / 2;
    y1 = (getmaxy() + WIDTH) / 2;
    rectangle(x1, y1, x1 + WIDTH, y1 - WIDTH);
    ellipse(x1, y1, 0, 90, WIDTH, WIDTH);

    for (i = 0; i < n; i++)
    {
        x = tc_random() * WIDTH;
        y = tc_random() * WIDTH;

        if (x * x + y * y > (double)WIDTH * WIDTH)
        {
            putpixel(x1 + x, y1 - y, CYAN);
            out++;
        }
        else
        {
            putpixel(x1 + x, y1 - y, YELLOW);
            in++;
        }
    }

    pi = (double)in / n * 4.0;
    sprintf(pi_str, "pi = %lf", pi);
    setcolor(WHITE);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(getmaxx() / 2, y1 + 30, pi_str);
    bioskey(0);
    return 0;
}
