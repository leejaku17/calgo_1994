/*                                                                  */
/*   GM.C  :  Demo of linear transformation                         */
/*                                                                  */
/*                     Programmed By Kim Junmo                      */
/*                                                                  */

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <graphics.h>
#include "matrix.h"
#define N 5
#define CENTERX 320
#define CENTERY 240

#define C 0.999390827 /* cos 5 */
#define S 0.034899496 /* sin 5 */
#define M 1

double XR[4][4] = {1, 0, 0, 0,
				   0, C, -S, 0,
				   0, S, C, 0,
				   0, 0, 0, 1};
double YR[4][4] = {C, 0, -S, 0,
				   0, 1, 0, 0,
				   S, 0, C, 0,
				   0, 0, 0, 1};
double ZR[4][4] = {C, -S, 0, 0,
				   S, C, 0, 0,
				   0, 0, 1, 0,
				   0, 0, 0, 1};
double YM[4][4] = {1, 0, 0, 0,
				   0, 1, 0, M,
				   0, 0, 1, 0,
				   0, 0, 0, 1};
double ZM[4][4] = {1, 0, 0, 0,
				   0, 1, 0, 0,
				   0, 0, 1, M,
				   0, 0, 0, 1};
double Ym[4][4] = {1, 0, 0, 0,
				   0, 1, 0, -M,
				   0, 0, 1, 0,
				   0, 0, 0, 1};
double Zm[4][4] = {1, 0, 0, 0,
				   0, 1, 0, 0,
				   0, 0, 1, -M,
				   0, 0, 0, 1};

void draw(double p[][4], int color)
{
	int i, j;
	setcolor(color);
	for (i = 0; i < N - 1; i++)
		for (j = i + 1; j < N; j++)
		{
			line(CENTERX + p[i][1], CENTERY - p[i][2], CENTERX + p[j][1], CENTERY - p[j][2]);
		}
}

void rotate(double p[][4], char c)
{
	int i, j;
	double (*r)[4];
	double temp[N][4];
	r = (c == 0) ? XR : (c == 1) ? YR
					: (c == 2)	 ? ZR
					: (c == 3)	 ? YM
					: (c == 4)	 ? ZM
					: (c == 5)	 ? Ym
								 : Zm;
	for (i = 0; i < N; i++)
		for (j = 0; j < 4; j++)
			temp[i][j] = p[i][j];
	for (i = 0; i < N; i++)
		mult_vector(temp[i], r, p[i], 4);
	draw(temp, 0);
	draw(p, 15);
}

void main()
{
	int graphdrive = DETECT, graphmode;
	int c;
	int i;

	double p[N][4] = {70.7, 0, 0, 1,
					  0, 50, 50, 1,
					  0, -50, 50, 1,
					  0, -50, -50, 1,
					  0, 50, -50, 1};

	initgraph(&graphdrive, &graphmode, "");
	draw(p, 14);

	while (1)
	{
		if (p[0][1] > 300)
			i = 5;
		else if (p[0][1] < -300)
			i = 3;
		else if (p[0][2] > 200)
			i = 6;
		else if (p[0][2] < -200)
			i = 4;
		else
			i = random(7);
		c = random(100);
		while (c)
		{
			if (bioskey(1))
			{
				closegraph();
				exit(1);
			}
			rotate(p, i);
			delay(10);
			c--;
		}
	}
}
