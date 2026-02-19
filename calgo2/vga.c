/*                                                                  */
/*   VGA.C  :  Graphic Library for 0x12 & 0x13 mode                 */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <bios.h>
#include <dos.h>
#include <math.h>
#include "vga.h"

unsigned char far *vidseg = (unsigned char far *)0xA0000000UL;

void (*write_pixel)(int x, int y, int color);
int (*read_pixel)(int x, int y);
void (*draw_horiz_line)(int x1, int x2, int y, int color);

int maxx, maxy, maxcolor, vgamode;

/* video functions for 0x12 mode (640*480 16color) */

void set_0x12(void)
{
	union REGS r;
	r.x.ax = 0x12;
	int86(0x10, &r, &r);
}

void write_pixel_0x12(int x, int y, int color)
{
	unsigned char far *v;
	unsigned char bit_mask = 128;
	v = vidseg + y * 80 + (x >> 3);
	bit_mask >>= x % 8;
	outport(0x3ce, 0x0005);		/* write mode 0 */
	outport(0x3ce, color << 8); /* Set/Reset register to color */
	outport(0x3ce, 0x0f01);		/* Enable Set/Reset register to vertical op */
	outportb(0x03ce, 8);		/* Bit Mask Register to point */
	outportb(0x03cf, bit_mask);
	*v &= 0x80;
}

int read_pixel_0x12(int x, int y)
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

void draw_horiz_line_0x12(int x1, int x2, int y, int color)
{
	unsigned char far *v1, far *v2;
	unsigned char bit_mask;
	int a, b;
	if (x1 > x2)
		swap(&x1, &x2);
	v1 = vidseg + y * 80 + (x1 >> 3);
	v2 = vidseg + y * 80 + (x2 >> 3);
	outport(0x3ce, 0x0005);		/* write mode 0 */
	outport(0x3ce, color << 8); /* Set/Reset register to color */
	outport(0x3ce, 0x0f01);		/* Enable Set/Reset register to vertical op */
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

/* video functions for 0x13 mode (320*200 256color) */

void set_0x13(void)
{
	union REGS r;
	r.x.ax = 0x13;
	int86(0x10, &r, &r);
}

void write_pixel_0x13(int x, int y, int color)
{
	unsigned char far *v;
	v = vidseg + y * 320 + x;
	*v = (unsigned char)color;
}

int read_pixel_0x13(int x, int y)
{
	unsigned char far *v;
	v = vidseg + y * 320 + x;
	return (int)*v;
}

void draw_horiz_line_0x13(int x1, int x2, int y, int color)
{
	unsigned char far *v1, far *v2;
	if (x1 > x2)
		swap(&x1, &x2);
	v1 = vidseg + y * 320 + x1;
	v2 = v1 + (x2 - x1);
	while (v1 <= v2)
		*v1++ = color;
}

/* common part */

void set_text(void)
{
	union REGS r;
	r.x.ax = 0x03;
	int86(0x10, &r, &r);
}

void set_vga(int mode)
{
	if (mode == VGA640)
	{
		maxx = 640;
		maxy = 480;
		maxcolor = 16;
		vgamode = VGA640;
		set_0x12();
		write_pixel = write_pixel_0x12;
		read_pixel = read_pixel_0x12;
		draw_horiz_line = draw_horiz_line_0x12;
	}
	else if (mode == VGA320)
	{
		maxx = 320;
		maxy = 200;
		maxcolor = 256;
		vgamode = VGA320;
		set_0x13();
		write_pixel = write_pixel_0x13;
		read_pixel = read_pixel_0x13;
		draw_horiz_line = draw_horiz_line_0x13;
	}
}

void clear_device(void)
{
	if (vgamode == VGA640)
		set_0x12();
	else if (vgamode == VGA320)
		set_0x13();
}

void swap(int *a, int *b)
{
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

void draw_line(int x1, int y1, int x2, int y2, int color)
{
	int x, y;
	int delta_x, delta_y, d;
	int inc;
	int Einc, NEinc;

	if (abs(x2 - x1) > abs(y2 - y1)) /* |slope| < 1 */
	{
		if (x1 > x2)
		{
			swap(&x1, &x2);
			swap(&y1, &y2);
		}
		inc = (y2 > y1) ? 1 : -1;
		delta_x = x2 - x1;
		delta_y = abs(y2 - y1);
		d = 2 * delta_y - delta_x;
		Einc = 2 * delta_y;
		NEinc = 2 * (delta_y - delta_x);
		write_pixel(x = x1, y = y1, color);
		while (x < x2)
		{
			x++;
			if (d > 0)
			{
				y += inc;
				d += NEinc;
			}
			else
				d += Einc;
			write_pixel(x, y, color);
		}
	}
	else
	{
		if (y1 > y2)
		{
			swap(&x1, &x2);
			swap(&y1, &y2);
		}
		inc = (x2 > x1) ? 1 : -1;
		delta_x = abs(x2 - x1);
		delta_y = y2 - y1;
		d = 2 * delta_x - delta_y;
		Einc = 2 * delta_x;
		NEinc = 2 * (delta_x - delta_y);
		write_pixel(x = x1, y = y1, color);
		while (y < y2)
		{
			y++;
			if (d > 0)
			{
				x += inc;
				d += NEinc;
			}
			else
				d += Einc;
			write_pixel(x, y, color);
		}
	}
}

void draw_circle(int xc, int yc, int r, int color)
{
	int h;
	int x, y;
	int deltaE, deltaNE;
	x = 0;
	y = r;
	h = 1 - r;
	deltaE = 3;
	deltaNE = 5 - 2 * r;
	while (y >= x)
	{
		write_pixel(x + xc, y + yc, color);
		write_pixel(y + xc, x + yc, color);
		write_pixel(y + xc, -x + yc, color);
		write_pixel(x + xc, -y + yc, color);
		write_pixel(-x + xc, -y + yc, color);
		write_pixel(-y + xc, -x + yc, color);
		write_pixel(-y + xc, x + yc, color);
		write_pixel(-x + xc, y + yc, color);
		if (h < 0) /* case E */
		{
			h += deltaE;
			deltaE += 2;
			deltaNE += 2;
		}
		else /* case NE */
		{
			h += deltaNE;
			deltaE += 2;
			deltaNE += 4;
			y--;
		}
		x++;
	}
}

void draw_filled_circle(int xc, int yc, int r, int color)
{
	int h;
	int x, y;
	int deltaE, deltaNE;
	x = 0;
	y = r;
	h = 1 - r;
	deltaE = 3;
	deltaNE = 5 - 2 * r;
	while (y >= x)
	{
		draw_horiz_line(x + xc, -x + xc, y + yc, color);
		draw_horiz_line(y + xc, -y + xc, x + yc, color);
		draw_horiz_line(y + xc, -y + xc, -x + yc, color);
		draw_horiz_line(x + xc, -x + xc, -y + yc, color);
		if (h < 0) /* case E */
		{
			h += deltaE;
			deltaE += 2;
			deltaNE += 2;
		}
		else /* case NE */
		{
			h += deltaNE;
			deltaE += 2;
			deltaNE += 4;
			y--;
		}
		x++;
	}
}

#define DELTA 5

void draw_arc(int xc, int yc, int r, int start, int end, int color)
{
	int x, y, x_old, y_old, theta;
	theta = start;
	x_old = r * cos(theta * M_PI / 180.) + 0.5;
	y_old = r * sin(theta * M_PI / 180.) + 0.5;
	for (theta += DELTA; theta <= end; theta += DELTA)
	{
		x = r * cos(theta * M_PI / 180.) + 0.5;
		y = r * sin(theta * M_PI / 180.) + 0.5;
		draw_line(x_old + xc, y_old + yc, x + xc, y + yc, color);
		x_old = x;
		y_old = y;
	}
}

void draw_ellipse(int xc, int yc, int a, int b, int color)
{
	long h;
	long a2 = (long)a * a, b2 = (long)b * b;
	long a82 = 8 * a2, b82 = 8 * b2;
	int x, y;
	long deltaE, deltaNE;
	x = 0;
	y = b;
	h = 4 * b2 + a2 - 4 * a2 * b;
	deltaE = 12 * b2;
	deltaNE = 12 * b2 - 8 * a2 * b + 8 * a2;
	while (b2 * x <= a2 * y)
	{
		write_pixel(x + xc, y + yc, color);
		write_pixel(x + xc, -y + yc, color);
		write_pixel(-x + xc, -y + yc, color);
		write_pixel(-x + xc, y + yc, color);
		if (h < 0) /* case E */
		{
			h += deltaE;
			deltaE += b82;
			deltaNE += b82;
		}
		else /* case NE */
		{
			h += deltaNE;
			deltaE += b82;
			deltaNE += b82 + a82;
			y--;
		}
		x++;
	}
	x = a;
	y = 0;
	h = 4 * a2 + a2 - 4 * a * b2;
	deltaE = 12 * a2;
	deltaNE = 12 * a2 - 8 * b2 * a + 8 * b2;
	while (b2 * x > a2 * y)
	{
		write_pixel(x + xc, y + yc, color);
		write_pixel(x + xc, -y + yc, color);
		write_pixel(-x + xc, -y + yc, color);
		write_pixel(-x + xc, y + yc, color);
		if (h < 0) /* case E */
		{
			h += deltaE;
			deltaE += a82;
			deltaNE += a82;
		}
		else /* case NE */
		{
			h += deltaNE;
			deltaE += a82;
			deltaNE += b82 + a82;
			x--;
		}
		y++;
	}
}

void draw_filled_ellipse(int xc, int yc, int a, int b, int color)
{
	long h;
	long a2 = (long)a * a, b2 = (long)b * b;
	long a82 = 8 * a2, b82 = 8 * b2;
	int x, y;
	long deltaE, deltaNE;
	x = 0;
	y = b;
	h = 4 * b2 + a2 - 4 * a2 * b;
	deltaE = 12 * b2;
	deltaNE = 12 * b2 - 8 * a2 * b + 8 * a2;
	while (b2 * x <= a2 * y)
	{
		draw_horiz_line(-x + xc, x + xc, y + yc, color);
		draw_horiz_line(-x + xc, x + xc, -y + yc, color);
		if (h < 0) /* case E */
		{
			h += deltaE;
			deltaE += b82;
			deltaNE += b82;
		}
		else /* case NE */
		{
			h += deltaNE;
			deltaE += b82;
			deltaNE += b82 + a82;
			y--;
		}
		x++;
	}
	x = a;
	y = 0;
	h = 4 * a2 + a2 - 4 * a * b2;
	deltaE = 12 * a2;
	deltaNE = 12 * a2 - 8 * b2 * a + 8 * b2;
	while (b2 * x > a2 * y)
	{
		draw_horiz_line(-x + xc, x + xc, y + yc, color);
		draw_horiz_line(-x + xc, x + xc, -y + yc, color);
		if (h < 0) /* case E */
		{
			h += deltaE;
			deltaE += a82;
			deltaNE += a82;
		}
		else /* case NE */
		{
			h += deltaNE;
			deltaE += a82;
			deltaNE += b82 + a82;
			x--;
		}
		y++;
	}
}

int scan_left(int x, int y, int c, int bc)
{
	int v;
	v = read_pixel(x, y); /* scan left */
	while (v != c && v != bc && x >= 0)
		v = read_pixel(--x, y);
	x++;
	return x;
}

int scan_right(int x, int y, int c, int bc)
{
	int v;
	v = read_pixel(x, y); /* scan right */
	while (v != c && v != bc && x < maxx)
		v = read_pixel(++x, y);
	x--;
	return x;
}

#define NOT_DRAWN 0
#define BELOW_DRAWN 1
#define UPPER_DRAWN 2

int _line_fill(int sx, int sy, int c, int bc, int pl, int pr, int flag)
{
	int x, y;
	int v;
	int xleft, xright;

	xleft = xright = sx;
	y = sy;

	xleft = scan_left(sx, sy, c, bc);
	xright = scan_right(sx, sy, c, bc);

	draw_horiz_line(xleft, xright, y, c);

	if (flag == UPPER_DRAWN)
	{
		for (x = xleft; x < pl; x++)
		{
			v = read_pixel(x, y - 1);
			if (v != c && v != bc)
				x = _line_fill(x, y - 1, c, bc, xleft, xright, BELOW_DRAWN);
		}
		for (x = pr + 1; x <= xright; x++)
		{
			v = read_pixel(x, y - 1);
			if (v != c && v != bc)
				x = _line_fill(x, y - 1, c, bc, xleft, xright, BELOW_DRAWN);
		}
	}
	else
		for (x = xleft; x <= xright; x++)
		{
			v = read_pixel(x, y - 1);
			if (v != c && v != bc)
				x = _line_fill(x, y - 1, c, bc, xleft, xright, BELOW_DRAWN);
		}
	if (flag == BELOW_DRAWN)
	{
		for (x = xleft; x < pl; x++)
		{
			v = read_pixel(x, y + 1);
			if (v != c && v != bc)
				x = _line_fill(x, y + 1, c, bc, xleft, xright, UPPER_DRAWN);
		}
		for (x = pr + 1; x <= xright; x++)
		{
			v = read_pixel(x, y + 1);
			if (v != c && v != bc)
				x = _line_fill(x, y + 1, c, bc, xleft, xright, UPPER_DRAWN);
		}
	}
	else
		for (x = xleft; x <= xright; x++)
		{
			v = read_pixel(x, y + 1);
			if (v != c && v != bc)
				x = _line_fill(x, y + 1, c, bc, xleft, xright, UPPER_DRAWN);
		}
	return xright;
}

void line_fill(int sx, int sy, int c, int bc)
{
	_line_fill(sx, sy, c, bc, 0, 0, NOT_DRAWN);
}

void bezier_spline(float p1x, float p1y, float p2x, float p2y,
				   float p3x, float p3y, float p4x, float p4y, int d, int color)
{
	float m1x, m2x, m3x, m4x, m5x, m6x;
	float m1y, m2y, m3y, m4y, m5y, m6y;

	if (d > 0)
	{
		m1x = (p1x + p2x) / 2;
		m1y = (p1y + p2y) / 2;
		m2x = (p2x + p3x) / 2;
		m2y = (p2y + p3y) / 2;
		m3x = (p3x + p4x) / 2;
		m3y = (p3y + p4y) / 2;

		m4x = (m1x + m2x) / 2;
		m4y = (m1y + m2y) / 2;
		m5x = (m2x + m3x) / 2;
		m5y = (m2y + m3y) / 2;

		m6x = (m4x + m5x) / 2;
		m6y = (m4y + m5y) / 2;

		bezier_spline(p1x, p1y, m1x, m1y, m4x, m4y, m6x, m6y, d - 1, color);
		bezier_spline(m6x, m6y, m5x, m5y, m3x, m3y, p4x, p4y, d - 1, color);
	}
	else
		draw_line(p1x + 0.5, p1y + 0.5, p4x + 0.5, p4y + 0.5, color);
}
