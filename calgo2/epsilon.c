/*                                                                  */
/*   EPSILON.C  :  Get machine epsilon of float                     */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

void main(void)
{
	float ex, g = 1, eps;
	do
	{
		g /= 2;
		ex = g + 1;
		ex -= 1;
		printf("g = %20.15e   ex = %20.15e\n", g, ex);
		if (ex > 0)
			eps = ex;
	} while (ex > 0);
	printf("\n Machine Epsilon = %20.15e\n\n", eps);
}
