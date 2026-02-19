/*                                                                  */
/*   LINE.C  :  Line Test 					    */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <stdio.h>
#include <bios.h>
#include "0x12.h"
#include "timer.h"

void draw_line_equation(int x1, int y1, int x2, int y2, int color)
{
	float m;
	int x, y;
	if (x1 == x2) /* if vertical line */
	{
		if (y1 > y2)
			swap(&y1, &y2);
		for (y = y1; y <= y2; y++)
			write_pixel(x1, y, color);
		return;
	}
	/* if not vertical line */
	m = (float)(y2 - y1) / (float)(x2 - x1);
	if (-1 < m && m < 1) /* x increase */
	{
		if (x1 > x2)
		{
			swap(&x1, &x2);
			swap(&y1, &y2);
		}
		for (x = x1; x <= x2; x++)
		{
			y = m * (x - x1) + y1 + 0.5;
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
		for (y = y1; y <= y2; y++)
		{
			x = (y - y1) / m + x1 + 0.5;
			write_pixel(x, y, color);
		}
	}
}

void draw_line_incremental(int x1, int y1, int x2, int y2, int color)
{
	float m, x, y;
	if (x1 == x2) /* if vertical line */
	{
		if (y1 > y2)
			swap(&y1, &y2);
		for (y = y1; y <= y2; y++)
			write_pixel(x1, y, color);
		return;
	}
	m = (float)(y2 - y1) / (float)(x2 - x1);
	/* if not vertical line */
	if (-1 < m && m < 1) /* x increase */
	{
		if (x1 > x2)
		{
			swap(&x1, &x2);
			swap(&y1, &y2);
		}
		y = y1;
		for (x = x1; x <= x2; x++)
		{
			write_pixel(x, (int)(y + 0.5), color);
			y += m;
		}
	}
	else /* y increase */
	{
		if (y1 > y2)
		{
			swap(&x1, &x2);
			swap(&y1, &y2);
		}
		m = (float)(x2 - x1) / (float)(y2 - y1);
		x = x1;
		for (y = y1; y <= y2; y++)
		{
			write_pixel((int)(x + 0.5), y, color);
			x += m;
		}
	}
}

void draw_line_midpoint1(int x1, int y1, int x2, int y2, int color)
{
	int x, y;
	int delta_x, delta_y, d;

	if (abs(x2 - x1) > abs(y2 - y1)) /* -1 < slope < 1 */
	{
		if (x1 > x2)
		{
			swap(&x1, &x2);
			swap(&y1, &y2);
		}
		delta_x = x2 - x1;
		delta_y = y2 - y1;
		if (delta_y > 0) /* 0 < slope < 1 */
		{
			write_pixel(x = x1, y = y1, color);
			d = 2 * delta_y - delta_x;
			while (x < x2)
			{
				if (d < 0) /* case E */
				{
					d += 2 * delta_y;
					x++;
				}
				else /* case NE */
				{
					d += (delta_y - delta_x) * 2;
					x++;
					y++;
				}
				write_pixel(x, y, color);
			}
		}
		else /* -1 < slope < 0 */
		{
			write_pixel(x = x1, y = y1, color);
			d = 2 * delta_y + delta_x;
			while (x < x2)
			{
				if (d > 0) /* case E */
				{
					d += 2 * delta_y;
					x++;
				}
				else /* case NE */
				{
					d += (delta_y + delta_x) * 2;
					x++;
					y--;
				}
				write_pixel(x, y, color);
			}
		}
	}
	else /* case |slope| > 1 */
	{
		if (y1 > y2)
		{
			swap(&x1, &x2);
			swap(&y1, &y2);
		}
		delta_x = x2 - x1;
		delta_y = y2 - y1;
		if (delta_x > 0) /* slope > 1 */
		{
			write_pixel(x = x1, y = y1, color);
			d = delta_y - 2 * delta_x;
			while (y < y2)
			{
				if (d > 0) /* case E */
				{
					d += -2 * delta_x;
					y++;
				}
				else /* case NE */
				{
					d += 2 * (delta_y - delta_x);
					y++;
					x++;
				}
				write_pixel(x, y, color);
			}
		}
		else /* slope < -1 */
		{
			write_pixel(x = x1, y = y1, color);
			d = -2 * delta_x - delta_y;
			while (y < y2)
			{
				if (d < 0) /* case E */
				{
					d += -2 * delta_x;
					y++;
				}
				else /* case NE */
				{
					d += -2 * (delta_x + delta_y);
					y++;
					x--;
				}
				write_pixel(x, y, color);
			}
		}
	}
}

void draw_line_midpoint2(int x1, int y1, int x2, int y2, int color)
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

void line_test(void (*func)(int, int, int, int, int))
{
	int x, y;
	for (x = 0; x < 640; x += 5)
	{
		func(x, 0, 320, 240, (x % 16) + 1);
		func(x, 479, 320, 240, (x % 16) + 1);
	}
	for (y = 0; y < 480; y += 5)
	{
		func(0, y, 320, 240, (y % 16) + 1);
		func(639, y, 320, 240, (y % 16) + 1);
	}
}

void main(void)
{
	long t1, t2;
	int l1, l2, l3, l4;

	set_0x12();
	t1 = get_tick();
	line_test(draw_line_equation);
	t2 = get_tick();
	l1 = (int)diff_tick(t1, t2);
	bioskey(0);

	set_0x12();
	t1 = get_tick();
	line_test(draw_line_incremental);
	t2 = get_tick();
	l2 = (int)diff_tick(t1, t2);
	bioskey(0);

	set_0x12();
	t1 = get_tick();
	line_test(draw_line_midpoint1);
	t2 = get_tick();
	l3 = (int)diff_tick(t1, t2);
	bioskey(0);

	set_0x12();
	t1 = get_tick();
	line_test(draw_line_midpoint2);
	t2 = get_tick();
	l4 = (int)diff_tick(t1, t2);
	bioskey(0);

	set_text();
	printf("%d %d %d %d", l1, l2, l3, l4);
}