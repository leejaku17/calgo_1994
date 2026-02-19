/*                                                                  */
/*   CROSS.C  :  Detect which do two line segments intersect or not */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <math.h>
#include <bios.h>
#include <stdlib.h>
#include <dos.h>
#include <stdio.h>
#include "vga.h"

int intersect_equation(int p1x, int p1y, int p2x, int p2y,
					   int p3x, int p3y, int p4x, int p4y)
{
	long m;
	float x;
	if (p1x > p2x)
	{
		swap(&p1x, &p2x);
		swap(&p1y, &p2y);
	}
	if (p3x > p4x)
	{
		swap(&p3x, &p4x);
		swap(&p3y, &p4y);
	}
	m = (float)(p4x - p3x) * (p2y - p1y) - (float)(p4y - p3y) * (p2x - p1x);
	if (m == 0)
		return 0;
	x = ((float)(p3y - p1y) * (p2x - p1x) * (p4x - p3x) + (float)p1x * (p2y - p1y) * (p4x - p3x) - (float)p3x * (p4y - p3y) * (p2x - p1x)) / (float)m;
	if (p1x <= x && x <= p2x && p3x <= x && x <= p4x)
		return 1;
	else
		return -1;
}

int ccw(int ax, int ay, int bx, int by, int cx, int cy)
{
	long l;
	l = (long)bx * cy - (long)ay * bx - (long)ax * cy - (long)by * cx + (long)ax * by + (long)ay * cx;
	if (l > 0)
		return 1;
	else if (l < 0)
		return -1;
	else
		return 0;
}

int intersect_ccw(int p1x, int p1y, int p2x, int p2y,
				  int p3x, int p3y, int p4x, int p4y)
{
	int r123, r124, r341, r342;
	if (p1x > p2x)
	{
		swap(&p1x, &p2x);
		swap(&p1y, &p2y);
	}
	if (p3x > p4x)
	{
		swap(&p3x, &p4x);
		swap(&p3y, &p4y);
	}
	r123 = ccw(p1x, p1y, p2x, p2y, p3x, p3y);
	r124 = ccw(p1x, p1y, p2x, p2y, p4x, p4y);
	r341 = ccw(p3x, p3y, p4x, p4y, p1x, p1y);
	r342 = ccw(p3x, p3y, p4x, p4y, p2x, p2y);

	if (r123 * r124 < 0 && r341 * r342 < 0)
		return 1;

	if (r123 == 0 && r124 == 0) /* if two line coincedent */
	{
		if (!(p2x < p3x || p1x > p4x))
			return 0;
		else
			return -1;
	}

	/* now a points lie on line segment or its extend */

	if (r123 == 0)
	{
		if (p1x <= p3x && p3x <= p2x)
			return 0;
		else
			return -1;
	}
	if (r124 == 0)
	{
		if (p1x <= p4x && p4x <= p2x)
			return 0;
		else
			return -1;
	}
	if (r341 == 0)
	{
		if (p3x <= p1x && p1x <= p4x)
			return 0;
		else
			return -1;
	}
	if (r342 == 0)
	{
		if (p3x <= p2x && p2x <= p4x)
			return 0;
		else
			return -1;
	}
	return -1;
}

void ccw_test(void)
{
	int x1, y1, x2, y2, x3, y3;
	int d;
	while (!bioskey(1))
	{
		clear_device();
		x1 = random(maxx);
		y1 = random(maxy);
		x2 = random(maxx);
		y2 = random(maxy);
		x3 = random(maxx);
		y3 = random(maxy);

		draw_line(x1, y1, x2, y2, WHITE);
		draw_line(x2, y2, x3, y3, YELLOW);

		d = ccw(x1, y1, x2, y2, x3, y3);
		if (d > 0)
		{
			draw_line(5, 0, 5, 10, LIGHTRED);
			draw_line(0, 5, 10, 5, LIGHTRED);
		}
		else if (d < 0)
			draw_line(0, 5, 10, 5, LIGHTRED);
		else
			draw_circle(5, 5, 5, LIGHTRED);
		delay(1000);
	}
	bioskey(0);
}

void intersect_test(int (*func)(int, int, int, int, int, int, int, int))
{
	int x1, y1, x2, y2, x3, y3, x4, y4;
	while (!bioskey(1))
	{
		clear_device();
		x1 = random(maxx);
		y1 = random(maxy);
		x2 = random(maxx);
		y2 = random(maxy);
		x3 = random(maxx);
		y3 = random(maxy);
		x4 = random(maxx);
		y4 = random(maxy);

		draw_line(x1, y1, x2, y2, WHITE);
		draw_line(x3, y3, x4, y4, LIGHTCYAN);

		if (func(x1, y1, x2, y2, x3, y3, x4, y4) >= 0)
			draw_circle(5, 5, 5, LIGHTRED);
		else
		{
			draw_line(0, 0, 10, 10, LIGHTRED);
			draw_line(10, 0, 0, 10, LIGHTRED);
		}
		delay(1000);
	}
	bioskey(0);
}

void draw_polygon(int p[], int n, int color)
{
	int i;
	for (i = 0; i < n - 1; i++)
		draw_line(p[i * 2], p[i * 2 + 1], p[i * 2 + 2], p[i * 2 + 3], color);
	draw_line(p[i * 2], p[i * 2 + 1], p[0], p[1], color);
}

int inside(int p[], int n, int x, int y)
{
	int i, j;
	int count = 0;
	int ic;
	for (i = 0; i < n - 1; i++)
	{
		ic = intersect_ccw(x, y, maxx, y, p[i * 2], p[i * 2 + 1], p[i * 2 + 2], p[i * 2 + 3]);
		if (ic > 0)
			count++;
		else if (ic == 0 && y == min(p[i * 2 + 1], p[i * 2 + 3]) && p[i * 2 + 1] != p[i * 2 + 3])
			count++;
	}
	ic = intersect_ccw(x, y, maxx, y, p[i * 2], p[i * 2 + 1], p[0], p[1]);
	if (ic > 0)
		count++;
	else if (ic == 0 && y == min(p[i * 2 + 1], p[1]) && p[i * 2 + 1] != p[1])
		count++;

	return count & 1;
}

void inside_test(void)
{
	int p[] = {50, 50, 110, 100, 150, 60, 160, 100, 200, 100, 230, 50,
			   270, 100, 230, 210, 140, 250, 90, 180, 50, 230};
	int x, y;
	clear_device();
	draw_polygon(p, 11, WHITE);
	while (!bioskey(1))
	{
		x = random(maxx);
		y = random(maxy);
		if (inside(p, 11, x, y))
		{
			write_pixel(x, y, YELLOW);
		}
		else
			write_pixel(x, y, CYAN);
	}
	bioskey(0);
}

void main(void)
{
	set_vga(VGA640);
	intersect_test(intersect_equation);
	intersect_test(intersect_ccw);
	ccw_test();
	inside_test();
	set_text();
}
