/*                                                                  */
/*   0x12.C  :  Graphic Library for 0x12 mode    	            */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <stdio.h>
#include <bios.h>
#include <dos.h>
#include <stdlib.h>
#include "0x12.h"

unsigned char far *vidseg = (unsigned char far *)0xa0000000UL;

void set_0x12(void)
{
    union REGS r;
    r.x.ax = 0x12;
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
    unsigned char bit_mask = 128;
    v = vidseg + y * 80 + (x >> 3);
    bit_mask >>= x % 8;
    outport(0x3ce, 0x0005);     /* write mode 0 */
    outport(0x3ce, color << 8); /* Set/Reset register to color */
    outport(0x3ce, 0x0f01);     /* Enable Set/Reset register to vertical op */
    outportb(0x03ce, 8);        /* Bit Mask Register to point */
    outportb(0x03cf, bit_mask);
    *v &= 0x80;
}

int read_pixel(int x, int y)
{
    unsigned char far *v;
    unsigned char bit_mask = 128u;
    char bit_plane;
    unsigned int color;
    v = vidseg + y * 80 + (x >> 3);
    bit_mask >>= x % 8;
    for (color = 0, bit_plane = 3; bit_plane >= 0; bit_plane--)
    {
        outportb(0x3ce, 0x0005);
        outportb(0x3ce, 0x04);
        outportb(0x3cf, bit_plane);
        color <<= 1;
        color |= (*v & bit_mask) ? 1 : 0;
    }
    return color;
}

void draw_horiz_line(int x1, int x2, int y, int color)
{
    unsigned char far *v1, far *v2;
    unsigned char bit_mask;
    int a, b;
    if (x1 > x2)
        swap(&x1, &x2);
    v1 = vidseg + y * 80 + (x1 >> 3);
    v2 = vidseg + y * 80 + (x2 >> 3);
    outport(0x3ce, 0x0005);     /* write mode 0 */
    outport(0x3ce, color << 8); /* Set/Reset register to color */
    outport(0x3ce, 0x0f01);     /* Enable Set/Reset register to vertical op */
    if (v1 == v2)
    {
        bit_mask = 0xff << (7 - (x2 - x1));
        bit_mask >>= (x1 % 8);
        outportb(0x3ce, 8);
        outportb(0x3cf, bit_mask);
        *v1 &= 0x80;
        return;
    }
    if (x1 % 8)
    {
        bit_mask = 0xff >> (x1 % 8);
        outportb(0x03ce, 8); /* Bit Mask Register to point */
        outportb(0x03cf, bit_mask);
        *v1 &= 0x80;
        v1++;
    }
    if ((x2 % 8) != 7)
    {
        bit_mask = 0xff << (7 - (x2 % 8));
        outportb(0x03ce, 8); /* Bit Mask Register to point */
        outportb(0x03cf, bit_mask);
        *v2 &= 0x80;
        v2--;
    }
    outport(0x03ce, 0xff08);
    while (v1 <= v2)
    {
        *v1 &= 0x80;
        v1++;
    }
}

void draw_horiz_pattern_line(int x1, int x2, int y,
                             int color, unsigned char pattern[])
{
    unsigned char far *v1, far *v2;
    unsigned char bit_mask;
    unsigned int a, b;
    if (x1 > x2)
        swap(&x1, &x2);
    v1 = vidseg + y * 80 + (x1 >> 3);
    v2 = vidseg + y * 80 + (x2 >> 3);
    outport(0x3ce, 0x0005);     /* write mode 0 */
    outport(0x3ce, color << 8); /* Set/Reset register to color */
    outport(0x3ce, 0x0f01);     /* Enable Set/Reset register to vertical op */
    if (v1 == v2)
    {
        bit_mask = 0xff << (7 - (x2 - x1));
        bit_mask >>= (x1 % 8);
        bit_mask &= pattern[y % 8];
        outportb(0x3ce, 8);
        outportb(0x3cf, bit_mask);
        *v1 &= 0x80;
        return;
    }
    if (x1 % 8)
    {
        bit_mask = 0xff >> (x1 % 8);
        bit_mask &= pattern[y % 8];
        outportb(0x03ce, 8); /* Bit Mask Register to point */
        outportb(0x03cf, bit_mask);
        *v1 &= 0x80;
        v1++;
    }
    if ((x2 % 8) != 7)
    {
        bit_mask = 0xff << (7 - (x2 % 8));
        bit_mask &= pattern[y % 8];
        outportb(0x03ce, 8); /* Bit Mask Register to point */
        outportb(0x03cf, bit_mask);
        *v2 &= 0x80;
        v2--;
    }
    bit_mask = 0xff & pattern[y % 8];
    outport(0x03ce, 8);
    outport(0x03cf, bit_mask);
    while (v1 <= v2)
    {
        *v1 &= 0x80;
        v1++;
    }
}

void swap(int *a, int *b)
{
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}
