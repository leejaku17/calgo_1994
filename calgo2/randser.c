/*                                                                  */
/*   RANDSER.C  :  Create Random Number in text                     */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                 */

#include <stdio.h>

long seed = 39492l;

double linear_random(void)
{
    seed = (11 * seed + 5) % 17;
    return seed / (double)17.0;
}

void main(void)
{
    int n = 20;
    while (n--)
    {
        printf("\n%lf", linear_random());
    }
}
