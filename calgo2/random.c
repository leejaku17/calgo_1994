/*                                                                  */
/*   RANDOM.C  :  Random Number Generator Library                   */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <stdlib.h>

long seed = 100001;

double tc_random(void)
{
    return rand() / (double)RAND_MAX;
}

double linear_random(void)
{
    seed = (125 * seed + 5) % 2796203;
    return seed / (double)2796203;
}
