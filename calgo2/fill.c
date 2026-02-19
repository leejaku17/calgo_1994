/*                                                                  */
/*   FILL.C  :  Line Fill Test                    	            */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <dos.h>
#include "0x12.h"
#include "timer.h"
#include "intstack.c"

int maxx = 640, maxy = 480;
int WHITE = 15, GREEN = 10;

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

void line_fill(int sx, int sy, int c, int bc)
{
	int x, y;
	int v;
	int xleft, xright;

	xleft = xright = sx;
	y = sy;

	xleft = scan_left(sx, sy, c, bc);
	xright = scan_right(sx, sy, c, bc);

	draw_horiz_line(xleft, xright, y, c);

	for (x = xleft; x <= xright; x++)
	{
		v = read_pixel(x, y - 1); /* scan up */
		if (v != c && v != bc)
			line_fill(x, y - 1, c, bc);
	}

	for (x = xleft; x <= xright; x++)
	{
		v = read_pixel(x, y + 1); /* scan down */
		if (v != c && v != bc)
			line_fill(x, y + 1, c, bc);
	}
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

void line_fill2(int sx, int sy, int c, int bc)
{
	_line_fill(sx, sy, c, bc, 0, 0, 0);
}

void line_fill3(int sx, int sy, int c, int bc)
{
	int x, y;
	int v;
	int xleft, xright;

	init_stack();

	push(sx);
	push(sy);

	while (!stack_empty())
	{
		sy = pop();
		sx = pop();
		xleft = xright = sx;
		y = sy;

		xleft = scan_left(sx, sy, c, bc);
		xright = scan_right(sx, sy, c, bc);

		draw_horiz_line(xleft, xright, y, c);

		for (x = xleft; x <= xright; x++)
		{
			v = read_pixel(x, y - 1); /* scan up */
			if (v != c && v != bc)
			{
				push(x);
				push(y - 1);
				x = scan_right(x, y - 1, c, bc); /**/
			}
		}

		for (x = xleft; x <= xright; x++)
		{
			v = read_pixel(x, y + 1); /* scan down */
			if (v != c && v != bc)
			{
				push(x);
				push(y + 1);
				x = scan_right(x, y + 1, c, bc);
			}
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

unsigned char pattern[] = {0x3e, 0x63, 0x63, 0x60,
						   0x67, 0x63, 0x3e, 0x00};

void fill_test(void)
{
	long t1, t2;
	long l1, l2, l3;

	set_0x12();
	draw_circle(maxx / 2, maxy / 2, maxy / 2 - 2, WHITE);
	draw_circle(maxx / 2, maxy / 2, maxy / 5 - 2, WHITE);
	t1 = get_tick();
	line_fill(maxx / 2 - maxy / 2 + 10, maxy / 2, GREEN, WHITE);
	t2 = get_tick();
	l1 = diff_tick(t1, t2);
	bioskey(0);

	set_0x12();
	draw_circle(maxx / 2, maxy / 2, maxy / 2 - 2, WHITE);
	draw_circle(maxx / 2, maxy / 2, maxy / 5 - 2, WHITE);
	t1 = get_tick();
	line_fill2(maxx / 2 - maxy / 2 + 10, maxy / 2, GREEN, WHITE);
	t2 = get_tick();
	l2 = diff_tick(t1, t2);
	bioskey(0);

	set_0x12();
	draw_circle(maxx / 2, maxy / 2, maxy / 2 - 2, WHITE);
	draw_circle(maxx / 2, maxy / 2, maxy / 5 - 2, WHITE);
	t1 = get_tick();
	line_fill3(maxx / 2 - maxy / 2 + 10, maxy / 2, GREEN, WHITE);
	t2 = get_tick();
	l3 = diff_tick(t1, t2);
	bioskey(0);

	set_text();
	printf("%ld %ld %ld ", l1, l2, l3);
	bioskey(0);
}

void main(void)
{
	fill_test();
}