/*                                                                  */
/*   ATAN.C  :  Show angle() function's operation                   */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <math.h>
#include <stdio.h>

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

void main(void)
{
	double theta;
	double at;
	int x;
	int y;
	int r = 200;
	double t;
	for (theta = 0; theta <= 2 * M_PI; theta += M_PI / 6.)
	{
		x = r * cos(theta);
		y = r * sin(theta);
		at = angle(x, y);
		if (x == 0)
			t = 0;
		else
			t = (double)y / x;
		printf("\natan(%lf) : %d, %d = %lf", t, x, y, at);
	}
}
