/*                                                                  */
/*   0x13.C  :  Graphic Library for 0x13 mode    	            */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <stdio.h>
#include <bios.h>
#include <dos.h>
#include <stdlib.h>
#include "0x13.h"

unsigned char far *vidseg = (unsigned char far *)0xA0000000UL;

void set_0x13(void)
{
    union REGS r;
    r.x.ax = 0x13;
    int86(0x10, &r, &r);
}

void set_text(void)
{
    union REGS r;
    r.x.ax = 0x03;
    int86(0x10, &r, &r);
}

void write_pixel(int x, int y, int color)
{
    unsigned char far *v;
    v = vidseg + y * 320 + x;
    *v = (unsigned char)color;
}

int read_pixel(int x, int y)
{
    unsigned char far *v;
    v = vidseg + y * 320 + x;
    return (int)*v;
}

void draw_horiz_line(int x1, int x2, int y, int color)
{
    unsigned char far *v1, far *v2;
    if (x1 > x2)
        swap(&x1, &x2);
    v1 = vidseg + y * 320 + x1;
    v2 = v1 + (x2 - x1);
    while (v1 <= v2)
        *v1++ = color;
}

void draw_horiz_pattern_line(int x1, int x2, int y, int color,
                             unsigned char pattern[])
{
    unsigned char far *v1, far *v2;
    unsigned char bit_mask = 128u;
    int x;
    if (x1 > x2)
        swap(&x1, &x2);
    v1 = vidseg + y * 320 + x1;
    v2 = v1 + (x2 - x1);
    x = x1;
    while (v1 <= v2)
    {
        if ((bit_mask >> (x % 8)) & pattern[y % 8])
            *v1++ = color;
        else
            v1++;
        x++;
    }
}

void swap(int *a, int *b)
{
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}
