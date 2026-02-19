/*                                                                  */
/*   ECALC.C  :  Simulater of Engineering Calculator 	            */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "exparser.h"

void main(void)
{
    char s[256];
    double result;
    int st;
    init_var();
    while (1)
    {
        printf("\n");
        gets(s);
        if (s[0] == '#')
            exit(0);
        init_token(s);
        eval_exp(&result);
        st = get_status();
        print_error(st);
        if (st == 0)
            printf("\n %lf", result);
    }
}
