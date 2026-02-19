/*                                                                  */
/*   VGADEMO.C  :  Demonstration for graphic library  	            */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <bios.h>
#include <stdlib.h>
#include <stdio.h>
#include <dos.h>
#include "vga.h"

void pixel_test(void)
{
    int x, y, c;
    clear_device();
    while (!bioskey(1))
    {
        x = random(maxx);
        y = random(maxy);
        c = random(maxcolor);
        write_pixel(x, y, c);
    }
    bioskey(0);
}

void line_test(void)
{
    int x, y;
    clear_device();
    for (x = 0; x < maxx; x += 5)
    {
        draw_line(x, 0, maxx / 2, maxy / 2, (x % maxcolor) + 1);
        draw_line(x, maxy - 1, maxx / 2, maxy / 2, (x % maxcolor) + 1);
    }
    for (y = 0; y < maxy; y += 5)
    {
        draw_line(0, y, maxx / 2, maxy / 2, (y % maxcolor) + 1);
        draw_line(maxx - 1, y, maxx / 2, maxy / 2, (y % maxcolor) + 1);
    }
    bioskey(0);
}

void circle_test(void)
{
    int r;
    clear_device();
    for (r = maxy / 2; r > 0; r -= 3)
        draw_circle(maxx / 2, maxy / 2, r, (r % maxcolor) + 1);
    bioskey(0);
}

void filled_circle_test(void)
{
    int r;
    clear_device();
    for (r = maxy / 2; r > 0; r -= 10)
        draw_filled_circle(maxx / 2, maxy / 2, r, (r % maxcolor) + 1);
    bioskey(0);
}

void ellipse_test(void)
{
    int r;
    clear_device();
    for (r = maxy / 2; r > 20; r -= 3)
        draw_ellipse(maxx / 2, maxy / 2, r, r - 20, (r % maxcolor) + 1);
    bioskey(0);
}

void filled_ellipse_test(void)
{
    int r;
    clear_device();
    for (r = maxy / 2; r > 20; r -= 10)
        draw_filled_ellipse(maxx / 2, maxy / 2, r, r - 20, (r % maxcolor) + 1);
    bioskey(0);
}

void fill_test(void)
{
    clear_device();
    draw_circle(maxx / 2, maxy / 2, maxy / 2 - 2, WHITE);
    draw_circle(maxx / 2, maxy / 2, maxy / 5 - 2, WHITE);
    line_fill(maxx / 2 - maxy / 2 + 10, maxy / 2, GREEN, WHITE);
    bioskey(0);
}

void spline_test(void)
{
    int p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y;
    while (!bioskey(1))
    {
        clear_device();
        p1x = random(maxx);
        p1y = random(maxy);
        p2x = random(maxx);
        p2y = random(maxy);
        p3x = random(maxx);
        p3y = random(maxy);
        p4x = random(maxx);
        p4y = random(maxy);
        draw_line(p1x, p1y, p2x, p2y, CYAN);
        draw_line(p2x, p2y, p3x, p3y, CYAN);
        draw_line(p3x, p3y, p4x, p4y, CYAN);
        bezier_spline(p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y, 5, WHITE);
        delay(1000);
    }
    bioskey(0);
}

void main(void)
{
    int m;
    for (m = 1; m >= 0; m--)
    {
        set_vga(m);
        pixel_test();
        line_test();
        circle_test();
        filled_circle_test();
        ellipse_test();
        filled_ellipse_test();
        fill_test();
        spline_test();
    }
    set_text();
}
