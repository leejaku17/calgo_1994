/*                                                                  */
/*   CLOSEST.C  :  Find Closest Pair of Points                      */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <stdlib.h>
#include <math.h>
#include "vga.h"

typedef struct _point
{
	int x, y;
} point;

typedef struct _pair
{
	int p1, p2;
} pair;

void insert_sort(point p[], int n)
{
	int i, j;
	point t;
	for (i = 1; i < n; i++)
	{
		t = p[i];
		j = i;
		while (p[j - 1].x > t.x && j > 0)
		{
			p[j] = p[j - 1];
			j--;
		}
		p[j] = t;
	}
}

void shell_sort(point p[], int n)
{
	int i, j, k, h;
	point v;
	for (h = n / 2; h > 0; h /= 2)
	{
		for (i = 0; i < h; i++)
		{
			for (j = i + h; j < n; j += h)
			{
				v = p[j];
				k = j;
				while (k > h - 1 && p[k - h].x > v.x)
				{
					p[k] = p[k - h];
					k -= h;
				}
				p[k] = v;
			}
		}
	}
}

float distance(point p1, point p2)
{
	return sqrt((float)(p2.x - p1.x) * (p2.x - p1.x) + (float)(p2.y - p1.y) * (p2.y - p1.y));
}

pair closest_comb(point p[], int n)
{
	int i, j;
	pair closest;
	float d;

	d = 10000;
	for (i = 0; i < n - 1; i++)
		for (j = i + 1; j < n; j++)
			if (d > distance(p[i], p[j]))
			{
				d = distance(p[i], p[j]);
				closest.p1 = i;
				closest.p2 = j;
			}
	return closest;
}

void closest_comb_test(int n)
{
	int x, y;
	int i;
	point *p;
	pair pa;

	p = (point *)malloc(sizeof(point) * n);

	while (!bioskey(1))
	{
		clear_device();
		for (i = 0; i < n; i++)
		{
			p[i].x = random(maxx);
			p[i].y = random(maxy);
			draw_filled_circle(p[i].x, p[i].y, 1, CYAN);
		}
		pa = closest_comb(p, n);
		draw_circle(p[pa.p1].x, p[pa.p1].y, 3, YELLOW);
		draw_circle(p[pa.p2].x, p[pa.p2].y, 3, YELLOW);
		delay(2000);
	}
	bioskey(0);
	free(p);
}

pair closest_points(point p[], int x1, int x2)
{
	int i, j;
	int left, right;
	pair pa1, pa2;
	pair closest;
	float d;
	if (x2 - x1 > 3)
	{
		pa1 = closest_points(p, x1, x1 + (x2 - x1) / 2);
		pa2 = closest_points(p, x1 + (x2 - x1) / 2, x2);

		if (distance(p[pa1.p1], p[pa1.p2]) < distance(p[pa2.p1], p[pa2.p2]))
			closest = pa1;
		else
			closest = pa2;
		d = distance(p[closest.p1], p[closest.p2]);

		for (left = x1; p[left].x < p[x1 + (x2 - x1) / 2].x - (int)d - 1; left++)
			;
		for (right = x2 - 1; p[right].x > p[x1 + (x2 - x1) / 2].x + (int)d + 1; right--)
			;

		for (i = left; i < right; i++)
			for (j = i + 1; j <= right; j++)
				if (d > distance(p[i], p[j]))
				{
					d = distance(p[i], p[j]);
					closest.p1 = i;
					closest.p2 = j;
				}
		return closest;
	}
	else
	{
		d = 10000; /* +inf */
		for (i = x1; i < x2 - 1; i++)
			for (j = i + 1; j < x2; j++)
				if (d > distance(p[i], p[j]))
				{
					d = distance(p[i], p[j]);
					closest.p1 = i;
					closest.p2 = j;
				}
		return closest;
	}
}

void closest_test(int n)
{
	int x, y;
	int i;
	point *p;
	pair pa;

	p = (point *)malloc(sizeof(point) * n);

	while (!bioskey(1))
	{
		clear_device();
		for (i = 0; i < n; i++)
		{
			p[i].x = random(maxx);
			p[i].y = random(maxy);
			draw_filled_circle(p[i].x, p[i].y, 1, CYAN);
		}
		shell_sort(p, n);
		pa = closest_points(p, 0, n);
		draw_circle(p[pa.p1].x, p[pa.p1].y, 3, YELLOW);
		draw_circle(p[pa.p2].x, p[pa.p2].y, 3, YELLOW);
		delay(2000);
	}
	bioskey(0);
	free(p);
}

void main(void)
{
	randomize();
	set_vga(VGA640);
	closest_comb_test(100);
	closest_test(100);
	set_text();
}
