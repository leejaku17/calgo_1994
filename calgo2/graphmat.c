/*                                                                  */
/*   GRAPHMAT.C  :  Graphical Application of Matrix Library         */
/*                                                                  */
/*                     Programmed By Kim JunMo                      */
/*                                                                  */

#include <stdio.h>
#include <graphics.h>
#define N 5
#define CENTERX 320
#define CENTERY 240

#define C 0.99619 /* cos 5 */
#define S 0.08716 /* sin 5 */
#define M 5		  /* sin 5 */

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
		time_vector(temp[i], r, p[i], 4);
	draw(temp, 0);
	draw(p, 14);
}

void main()
{
	int graphdrive = DETECT, graphmode;
	int c;

	double p[N][4] = {70.7, 0, 0, 1,
					  0, 50, 50, 1,
					  0, -50, 50, 1,
					  0, -50, -50, 1,
					  0, 50, -50, 1};

	initgraph(&graphdrive, &graphmode, "\\tcpp\\bgi");
	draw(p, 14);
	while (1)
	{
		c = bioskey(0);
		if (c == 0x11b)
			break;
		switch (c)
		{
		case 0x9ff:
			rotate(p, 0);
			break;
		case 0x8ff:
			rotate(p, 1);
			break;
		case 0x6ff:
			rotate(p, 2);
			break;
		case 0x4d00:
			rotate(p, 3);
			break;
		case 0x4800:
			rotate(p, 4);
			break;
		case 0x4b00:
			rotate(p, 5);
			break;
		case 0x5000:
			rotate(p, 6);
			break;
		}
	}
	closegraph();
}
