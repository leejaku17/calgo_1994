/*                                                                  */
/*   CIRCLE.C  :  Circle & Ellipse Test for 0x12 mode    	    */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <stdio.h>
#include <bios.h>
#include <math.h>
#include "0x12.h"
#include "timer.h"

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

void draw_circle_equation(int xc, int yc, int r, int color)
{
	int x, y;
	x = 0;
	y = r;
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
		x++;
		y = (int)(sqrt((float)r * r - x * x) + 0.5);
	}
}

void draw_circle_midpoint(int xc, int yc, int r, int color)
{
	int h;
	int x, y;
	x = 0;
	y = r;
	h = 1 - r;
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
			h = h + 2 * x + 3;
		else /* case NE */
		{
			h = h + 2 * x - 2 * y + 5;
			y--;
		}
		x++;
	}
}

void draw_circle_2diff(int xc, int yc, int r, int color)
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

void circle_test(void (*func)(int, int, int, int))
{
	int r;
	for (r = 240; r > 0; r -= 3)
		func(320, 240, r, (r % 16) + 1);
}

void draw_ellipse_equation(int xc, int yc, int a, int b, int color)
{
	int x, y;
	long a2 = (long)a * a, b2 = (long)b * b;
	x = 0;
	while (b2 * x <= a2 * y)
	{
		y = (float)b / a * sqrt(a2 - x * x) + 0.5;
		write_pixel(x + xc, y + yc, color);
		write_pixel(x + xc, -y + yc, color);
		write_pixel(-x + xc, -y + yc, color);
		write_pixel(-x + xc, y + yc, color);
		x++;
	}
	y = 0;
	while (b2 * x > a2 * y)
	{
		x = (float)a / b * sqrt(b2 - y * y) + 0.5;
		write_pixel(x + xc, y + yc, color);
		write_pixel(x + xc, -y + yc, color);
		write_pixel(-x + xc, -y + yc, color);
		write_pixel(-x + xc, y + yc, color);
		y++;
	}
}

void draw_ellipse_midpoint(int xc, int yc, int a, int b, int color)
{
	long h;
	int x, y;
	long a2 = (long)a * a;
	long b2 = (long)b * b;
	x = 0;
	y = b;
	h = 4 * b2 + a2 - 4 * a2 * b;
	while (b2 * x <= a2 * y)
	{
		write_pixel(x + xc, y + yc, color);
		write_pixel(x + xc, -y + yc, color);
		write_pixel(-x + xc, -y + yc, color);
		write_pixel(-x + xc, y + yc, color);
		if (h < 0) /* case E */
			h += 8 * b2 * x + 12 * b2;
		else /* case NE */
		{
			h += 8 * b2 * x + 12 * b2 - 8 * a2 * y + 8 * a2;
			y--;
		}
		x++;
	}
	x = a;
	y = 0;
	h = 4 * a2 + b2 - 4 * a * b2;
	while (b2 * x > a2 * y)
	{
		write_pixel(x + xc, y + yc, color);
		write_pixel(x + xc, -y + yc, color);
		write_pixel(-x + xc, -y + yc, color);
		write_pixel(-x + xc, y + yc, color);
		if (h < 0) /* case E */
			h += 8 * a2 * y + 12 * a2;
		else /* case NE */
		{
			h += -8 * b2 * x + 8 * b2 + 8 * a2 * y + 12 * a2;
			x--;
		}
		y++;
	}
}

void draw_ellipse_2diff(int xc, int yc, int a, int b, int color)
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

void ellipse_test(void (*func)(int, int, int, int, int))
{
	int r;
	for (r = 240; r > 20; r -= 3)
		func(320, 240, r, r - 20, (r % 16) + 1);
}

void main(void)
{
	long t1, t2;
	int l1, l2, l3, l4;

	set_0x12();
	t1 = get_tick();
	circle_test(draw_circle_equation);
	t2 = get_tick();
	l1 = (int)diff_tick(t1, t2);
	bioskey(0);

	set_0x12();
	t1 = get_tick();
	circle_test(draw_circle_midpoint);
	t2 = get_tick();
	l2 = (int)diff_tick(t1, t2);
	bioskey(0);

	set_0x12();
	t1 = get_tick();
	circle_test(draw_circle_2diff);
	t2 = get_tick();
	l3 = (int)diff_tick(t1, t2);
	bioskey(0);

	set_0x12();
	circle_test(draw_filled_circle);
	bioskey(0);

	set_0x12();
	draw_arc(320, 240, 200, 0, 90, 13);
	draw_arc(320, 240, 200, 90, 180, 14);
	draw_arc(320, 240, 200, 180, 270, 12);
	draw_arc(320, 240, 200, 270, 360, 11);
	bioskey(0);

	set_0x12();
	t1 = get_tick();
	ellipse_test(draw_ellipse_equation);
	t2 = get_tick();
	l1 = diff_tick(t1, t2);
	bioskey(0);

	set_0x12();
	t1 = get_tick();
	ellipse_test(draw_ellipse_midpoint);
	t2 = get_tick();
	l2 = diff_tick(t1, t2);
	bioskey(0);

	set_0x12();
	t1 = get_tick();
	ellipse_test(draw_ellipse_2diff);
	t2 = get_tick();
	l3 = diff_tick(t1, t2);
	bioskey(0);

	set_text();
	printf("%d %d %d", l1, l2, l3);
}
