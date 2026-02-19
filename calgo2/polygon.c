/*                                                                  */
/*   POLYGON.C  :  Algorithms related to Polygon  	            */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "intstack.c"
#include "vga.h"

#define MAX_ANGLE 2 * M_PI

double angle(int dx, int dy)
{
	if (dx == 0 && dy == 0)
		return 0;
	if (dy == 0)
	{
		if (dx > 0)
			return 0;
		else
			return M_PI;
	}
	if (dx == 0)
	{
		if (dy > 0)
			return M_PI_2;
		else
			return M_PI_2 * 3.;
	}
	if (dx > 0)
	{
		if (dy > 0)
			return atan((double)dy / dx); /* 1 */
		else
			return atan((double)dy / dx) + 2 * M_PI; /* 4 */
	}
	else
	{
		return atan((double)dy / dx) + M_PI;
	}
}

/* this angle function is integer version. but resolution is very low.
   so it has problem. angle is returned 0~20000 range */

/*
int angle(int dx, int dy)
	{
	int q;
	int a;
	if (dx == 0 && dy == 0) return 0;
	if (dy == 0)
	{
	if (dx > 0) return 0;
	else return 10000;
	}
	if (dx == 0)
	{
	if (dy > 0) return 5000;
	else return 15000;
	}
	if (dx > 0)
	{
	if (dy > 0) q = 0;
	else q = 3;
	}
	else
	{
	if (dy > 0) q = 1;
	else q = 2;
	}
	if ((long)dx*dy > 0)
	a = (double)dy*dy*5000.0 / ((double)dx*dx + (double)dy*dy) + 0.5;
	else
	a = 5000 - (double)dy*dy*5000.0 / ((double)dx*dx + (double)dy*dy) + 0.5;
	return 5000*q + a;
	}
*/

void insert_sort(int a[], int n)
{
	int i, j, a0, a1;
	double t;
	for (i = 2; i < n; i++)
	{
		t = angle(a[i * 2] - a[0], a[i * 2 + 1] - a[1]);
		a0 = a[i * 2];
		a1 = a[i * 2 + 1];
		j = i;
		while (j > 1 && angle(a[j * 2 - 2] - a[0], a[j * 2 - 1] - a[1]) > t)
		{
			a[j * 2] = a[j * 2 - 2];
			a[j * 2 + 1] = a[j * 2 - 1];
			j--;
		}
		a[j * 2] = a0;
		a[j * 2 + 1] = a1;
	}
}

void closed_path(int p[], int n, int color)
{
	int i;
	int min;
	int mindex;
	int temp;
	min = p[1];
	mindex = 0;
	for (i = 1; i < n; i++)
	{
		if (p[i * 2 + 1] < min)
		{
			min = p[i * 2 + 1];
			mindex = i;
		}
	}
	temp = p[0];
	p[0] = p[mindex * 2];
	p[mindex * 2] = temp;
	temp = p[1];
	p[1] = p[mindex * 2 + 1];
	p[mindex * 2 + 1] = temp;
	insert_sort(p, n);
	for (i = 0; i < n - 1; i++)
		draw_line(p[i * 2], p[i * 2 + 1], p[i * 2 + 2], p[i * 2 + 3], color);
	draw_line(p[i * 2], p[i * 2 + 1], p[0], p[1], color);
}

void closed_path_test(int n)
{
	int x, y;
	int i, m;
	int *p;
	p = (int *)malloc(n * 2 * sizeof(int));
	while (!bioskey(1))
	{
		set_vga(VGA640);
		m = random(n) + 5;
		for (i = 0; i < m; i++)
		{
			x = random(maxx - 100) + 50;
			y = random(maxy - 100) + 50;
			p[i * 2] = x;
			p[i * 2 + 1] = y;
			draw_filled_circle(x, y, 1, YELLOW);
		}
		delay(1000);
		closed_path(p, m, WHITE);
		delay(1000);
	}
	bioskey(0);
}

int package_wrap(int p[], int n)
{
	int i, j;
	int mindex;
	double min;
	int t;
	double a, a2;
	double r;
	int m = 0; /* number of convex hull point */
	mindex = 0;
	min = p[mindex * 2 + 1];
	for (i = 1; i < n; i++)
	{
		if (min > p[i * 2 + 1])
		{
			min = p[i * 2 + 1];
			mindex = i;
		}
	}
	t = p[0];
	p[0] = p[mindex * 2];
	p[mindex * 2] = t;
	t = p[1];
	p[1] = p[mindex * 2 + 1];
	p[mindex * 2 + 1] = t;
	m++;

	min = angle(p[2] - p[0], p[3] - p[1]);
	mindex = 1;
	for (j = 2; j < n; j++)
	{
		a = angle(p[j * 2] - p[0], p[j * 2 + 1] - p[1]);
		if (min > a)
		{
			min = a;
			mindex = j;
		}
	}
	t = p[2];
	p[2] = p[mindex * 2];
	p[mindex * 2] = t;
	t = p[3];
	p[3] = p[mindex * 2 + 1];
	p[mindex * 2 + 1] = t;
	m++;

	for (i = 2; i < n; i++)
	{
		a2 = angle(p[i * 2 - 2] - p[i * 2 - 4], p[i * 2 - 1] - p[i * 2 - 3]);
		min = angle(p[i * 2] - p[i * 2 - 2], p[i * 2 + 1] - p[i * 2 - 1]) - a2;
		if (min < 0)
			min += MAX_ANGLE;
		mindex = i;
		for (j = i + 1; j < n; j++)
		{
			a = angle(p[j * 2] - p[i * 2 - 2], p[j * 2 + 1] - p[i * 2 - 1]) - a2;
			if (a < 0)
				a += MAX_ANGLE;
			if (min > a)
			{
				min = a;
				mindex = j;
			}
		}
		r = angle(p[0] - p[i * 2 - 2], p[1] - p[i * 2 - 1]) - a2;
		if (r < 0)
			r += MAX_ANGLE;
		if (min > r)
			break;
		m++;

		t = p[i * 2];
		p[i * 2] = p[mindex * 2];
		p[mindex * 2] = t;
		t = p[i * 2 + 1];
		p[i * 2 + 1] = p[mindex * 2 + 1];
		p[mindex * 2 + 1] = t;
	}
	return m;
}

int jarvis_march(int p[], int n)
{
	int i, j;
	int mindex;
	double min;
	int t;
	double a, r;
	int m = 0; /* number of convex hull point */
	mindex = 0;
	min = p[mindex * 2 + 1];
	for (i = 1; i < n; i++)
	{
		if (min > p[i * 2 + 1])
		{
			min = p[i * 2 + 1];
			mindex = i;
		}
	}
	t = p[0];
	p[0] = p[mindex * 2];
	p[mindex * 2] = t;
	t = p[1];
	p[1] = p[mindex * 2 + 1];
	p[mindex * 2 + 1] = t;
	m++;

	for (i = 1; i < n; i++)
	{
		min = angle(p[i * 2] - p[i * 2 - 2], p[i * 2 + 1] - p[i * 2 - 1]);
		mindex = i;
		for (j = i + 1; j < n; j++)
		{
			a = angle(p[j * 2] - p[i * 2 - 2], p[j * 2 + 1] - p[i * 2 - 1]);
			if (min > a)
			{
				min = a;
				mindex = j;
			}
		}
		if (p[mindex * 2 + 1] - p[i * 2 - 1] < 0)
			break;
		t = p[i * 2];
		p[i * 2] = p[mindex * 2];
		p[mindex * 2] = t;
		t = p[i * 2 + 1];
		p[i * 2 + 1] = p[mindex * 2 + 1];
		p[mindex * 2 + 1] = t;
		m++;
	}
	/* down_stage */
	for (; i < n; i++)
	{
		min = angle(p[i * 2 - 2] - p[i * 2], p[i * 2 - 1] - p[i * 2 + 1]);
		mindex = i;
		for (j = i + 1; j < n; j++)
		{
			a = angle(p[i * 2 - 2] - p[j * 2], p[i * 2 - 1] - p[j * 2 + 1]);
			if (min > a)
			{
				min = a;
				mindex = j;
			}
		}
		r = angle(p[i * 2 - 2] - p[0], p[i * 2 - 1] - p[1]);
		if (min > r)
			break;
		m++;

		t = p[i * 2];
		p[i * 2] = p[mindex * 2];
		p[mindex * 2] = t;
		t = p[i * 2 + 1];
		p[i * 2 + 1] = p[mindex * 2 + 1];
		p[mindex * 2 + 1] = t;
	}
	return m;
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

int graham_scan(int p[], int n)
{
	int min;
	int mindex;
	int i;
	int t;

	/* find lower right point */
	mindex = 0;
	min = p[1];
	for (i = 1; i < n; i++)
	{
		if (min > p[i * 2 + 1] || (min == p[i * 2 + 1] && p[i * 2] > p[mindex * 2]))
		{
			mindex = i;
			min = p[i * 2 + 1];
		}
	}
	t = p[mindex * 2];
	p[mindex * 2] = p[0];
	p[0] = t;
	t = p[mindex * 2 + 1];
	p[mindex * 2 + 1] = p[1];
	p[1] = t;

	insert_sort(p, n);

	init_stack();
	push(p[0]);
	push(p[1]);
	push(p[2]);
	push(p[3]);
	push(p[4]);
	push(p[5]);

	for (i = 3; i < n; i++)
	{
		while (ccw(p[i * 2], p[i * 2 + 1], stack[top - 1], stack[top],
				   stack[top - 3], stack[top - 2]) > 0)
		{
			pop();
			pop();
		}
		push(p[i * 2]);
		push(p[i * 2 + 1]);
	}
	return top / 2 + 1;
}

void convex_hull_test(int (*func)(int *, int), int n)
{
	int x, y;
	int i, m, cn;
	int *p;
	p = (int *)malloc(n * 2 * sizeof(int));

	while (!bioskey(1))
	{
		set_vga(VGA640);

		m = random(n) + 5;
		for (i = 0; i < m; i++)
		{
			x = random(maxx - 100) + 50;
			y = random(maxy - 100) + 50;
			p[i * 2] = x;
			p[i * 2 + 1] = y;
			draw_filled_circle(x, y, 1, YELLOW);
		}
		delay(1000);
		cn = func(p, m);
		for (i = 0; i < cn - 1; i++)
			draw_line(p[i * 2], p[i * 2 + 1], p[i * 2 + 2], p[i * 2 + 3], WHITE);
		draw_line(p[i * 2], p[i * 2 + 1], p[0], p[1], WHITE);
		delay(1000);
	}
	bioskey(0);
	free(p);
}

void graham_scan_test(int n)
{
	int x, y;
	int i, m, cn;
	int *p;
	p = (int *)malloc(n * 2 * sizeof(int));

	while (!bioskey(1))
	{
		set_vga(VGA640);

		m = random(n) + 5;
		for (i = 0; i < m; i++)
		{
			x = random(maxx - 100) + 50;
			y = random(maxy - 100) + 50;
			p[i * 2] = x;
			p[i * 2 + 1] = y;
			draw_filled_circle(x, y, 1, YELLOW);
		}
		delay(1000);
		cn = graham_scan(p, m);
		for (i = 0; i < cn - 1; i++)
			draw_line(stack[i * 2], stack[i * 2 + 1],
					  stack[i * 2 + 2], stack[i * 2 + 3], WHITE);
		draw_line(stack[i * 2], stack[i * 2 + 1], stack[0], stack[1], WHITE);
		delay(1000);
	}
	bioskey(0);
	free(p);
}

void main(void)
{
	randomize();

	closed_path_test(100);

	convex_hull_test(package_wrap, 100);
	convex_hull_test(jarvis_march, 100);
	graham_scan_test(100);

	set_text();
}
