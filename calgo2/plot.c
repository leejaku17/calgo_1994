/*                                                                  */
/*   PLOT.C  :  Write Pixel Test			            */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <stdio.h>
#include <bios.h>
#include <dos.h>
#include <stdlib.h>

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
    unsigned char bit_mask = 128u;
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
        outport(0x3ce, 0x0005); /* select read mode 0 */
        outportb(0x3ce, 0x04);
        outportb(0x3cf, bit_plane);
        color <<= 1;
        color |= (*v & bit_mask) ? 1 : 0;
    }
    return color;
}

void main(void)
{
    int x, y, c;
    int col[16];
    set_0x12();
    while (!bioskey(1))
    {
        x = random(640);
        y = random(480);
        c = random(16);
        write_pixel(x, y, c);
    }
    bioskey(0);
    set_0x12();
    for (c = 0; c < 16; c++)
        write_pixel(c, 0, c);
    for (c = 0; c < 16; c++)
        col[c] = read_pixel(c, 0);
    bioskey(0);
    set_text();
    for (c = 0; c < 16; c++)
        printf("\n%d", col[c]);
}
