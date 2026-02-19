/*                                                                  */
/*   SPLINE.C  :  Demonstration for splines     	            */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <stdlib.h>
#include <bios.h>
#include <math.h>
#include "vga.h"

float MH[4][4] = {{2, -3, 0, 1},
				  {-2, 3, 0, 0},
				  {1, -2, 1, 0},
				  {1, -1, 0, 0}};

float MB[4][4] = {{-1, 3, -3, 1},
				  {3, -6, 3, 0},
				  {-3, 3, 0, 0},
				  {1, 0, 0, 0}};

void draw_spline(int G[][4], float M[][4], int n, int color)
{
	float t;
	int i;
	int x, y;
	int oldx, oldy;

	oldx = x = G[0][0];
	oldy = y = G[1][0];

	i = 0;
	while (i <= n)
	{
		t = (float)i / n;
		x = (t * t * t * M[0][0] + t * t * M[0][1] + t * M[0][2] + M[0][3]) * G[0][0] +
			(t * t * t * M[1][0] + t * t * M[1][1] + t * M[1][2] + M[1][3]) * G[0][1] +
			(t * t * t * M[2][0] + t * t * M[2][1] + t * M[2][2] + M[2][3]) * G[0][2] +
			(t * t * t * M[3][0] + t * t * M[3][1] + t * M[3][2] + M[3][3]) * G[0][3] + 0.5;
		y = (t * t * t * M[0][0] + t * t * M[0][1] + t * M[0][2] + M[0][3]) * G[1][0] +
			(t * t * t * M[1][0] + t * t * M[1][1] + t * M[1][2] + M[1][3]) * G[1][1] +
			(t * t * t * M[2][0] + t * t * M[2][1] + t * M[2][2] + M[2][3]) * G[1][2] +
			(t * t * t * M[3][0] + t * t * M[3][1] + t * M[3][2] + M[3][3]) * G[1][3] + 0.5;
		draw_line(oldx, oldy, x, y, color);
		oldx = x;
		oldy = y;
		i++;
	}
}

void hermite_spline(int p1x, int p1y, int p4x, int p4y,
					int r1x, int r1y, int r4x, int r4y, int n, int color)
{
	int GH[2][4];
	GH[0][0] = p1x;
	GH[1][0] = p1y;
	GH[0][1] = p4x;
	GH[1][1] = p4y;
	GH[0][2] = r1x;
	GH[1][2] = r1y;
	GH[0][3] = r4x;
	GH[1][3] = r4y;
	draw_spline(GH, MH, n, color);
}

void Bezier_spline(int p1x, int p1y, int p2x, int p2y,
				   int p3x, int p3y, int p4x, int p4y, int n, int color)
{
	int GB[2][4];
	GB[0][0] = p1x;
	GB[1][0] = p1y;
	GB[0][1] = p2x;
	GB[1][1] = p2y;
	GB[0][2] = p3x;
	GB[1][2] = p3y;
	GB[0][3] = p4x;
	GB[1][3] = p4y;
	draw_spline(GB, MB, n, color);
}

void casteljau(float p1x, float p1y, float p2x, float p2y,
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

		casteljau(p1x, p1y, m1x, m1y, m4x, m4y, m6x, m6y, d - 1, color);
		casteljau(m6x, m6y, m5x, m5y, m3x, m3y, p4x, p4y, d - 1, color);
	}
	else
		draw_line(p1x, p1y, p4x, p4y, color);
}

void spline_test(void)
{
	int i;
	int p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y;

	randomize();

	set_vga(VGA640);

	/* hermite spline */
	while (!bioskey(1))
	{
		clear_device();
		p1x = random(maxx);
		p1y = random(maxy);
		p2x = random(maxx);
		p2y = random(maxy);
		p3x = random(1000) - 500;
		p3y = random(1000) - 500;
		p4x = random(1000) - 500;
		p4y = random(1000) - 500;
		draw_line(p1x + p3x / 10, p1y + p3y / 10, p1x, p1y, 3);
		draw_line(p2x + p4x / 10, p2y + p4y / 10, p2x, p2y, 3);
		hermite_spline(p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y, 200, 15);
		delay(1000);
	}
	bioskey(0);

	/* bezier spline */
	while (!bioskey(1))
	{
		clear_device();
		p1x = random(maxx);
		p1y = random(maxy);
		p2x = random(maxx);
		p2y = random(maxy);
		p3x = random(maxx);
		p3y = random(maxy);
		p4x = random(maxx);
		p4y = random(maxy);
		draw_line(p1x, p1y, p2x, p2y, 3);
		draw_line(p2x, p2y, p3x, p3y, 3);
		draw_line(p3x, p3y, p4x, p4y, 3);
		Bezier_spline(p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y, 200, 15);
		delay(1000);
	}
	bioskey(0);
	/* casteljau spline */
	while (!bioskey(1))
	{
		clear_device();
		p1x = random(640);
		p1y = random(480);
		p2x = random(640);
		p2y = random(480);
		p3x = random(640);
		p3y = random(480);
		p4x = random(640);
		p4y = random(480);
		draw_line(p1x, p1y, p2x, p2y, 3);
		draw_line(p2x, p2y, p3x, p3y, 3);
		draw_line(p3x, p3y, p4x, p4y, 3);
		casteljau(p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y, 10, 15);
		delay(1000);
	}
	bioskey(0);
}

void main(void)
{
	spline_test();
}
