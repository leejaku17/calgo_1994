/*                                                                  */
/*   POLYDEMO.C  :  Demonstration of polynomial library             */
/*                                                                  */
/*                     Programmed By Kim Junmo                      */
/*                  						    */

#include <stdio.h>
#include "polylist.h"
#include "timer.h"

#define LOOP 100000

void main(int argc, char *argv[])
{
    FILE *file;
    node *a, *b, *c, *d, *e, *f;
    int order;
    double coef;
    long t1, t2, i;

    if (argc < 2)
        file = stdin;
    else if ((file = fopen(argv[1], "rt")) == NULL)
    {
        printf("\n That file does not exist!");
        exit(1);
    }
    clrscr();
    a = input_polynomial(file);
    print_polynomial(a, "a(x) = ");
    b = input_polynomial(file);
    print_polynomial(b, "b(x) = ");
    c = add_polynomial(a, b);
    d = mult_polynomial(a, b);
    print_polynomial(c, "a(x) + b(x) = ");
    print_polynomial(d, "a(x) * b(x) = ");
    printf("\n\na(2.5) + b(2.5)");

    printf("\nHorner's rule -- pow_simple Value : %lg",
           eval_horner_simple(c, 2.5));
    t1 = get_tick();
    for (i = 0; i < LOOP; i++)
        eval_horner_simple(c, 2.5);
    t2 = get_tick();
    printf("  Time : %ld", diff_tick(t1, t2));

    printf("\nHorner's rule -- pow<math> Value : %lg",
           eval_horner_math(c, 2.5));
    t1 = get_tick();
    for (i = 0; i < LOOP; i++)
        eval_horner_math(c, 2.5);
    t2 = get_tick();
    printf("  Time : %ld", diff_tick(t1, t2));

    printf("\nHorner's rule -- pow_modified Value : %lg",
           eval_horner_modified(c, 2.5));
    t1 = get_tick();
    for (i = 0; i < LOOP; i++)
        eval_horner_modified(c, 2.5);
    t2 = get_tick();
    printf("  Time : %ld", diff_tick(t1, t2));

    printf("\nbrust-force -- pow_simple  Value : %lg",
           eval_simple(c, 2.5));
    t1 = get_tick();
    for (i = 0; i < LOOP; i++)
        eval_simple(c, 2.5);
    t2 = get_tick();
    printf("  Time : %ld", diff_tick(t1, t2));

    printf("\nbrust-force -- pow<math> Value : %lg",
           eval_math(c, 2.5));
    t1 = get_tick();
    for (i = 0; i < LOOP; i++)
        eval_math(c, 2.5);
    t2 = get_tick();
    printf("  Time : %ld", diff_tick(t1, t2));

    printf("\nbrust-force -- pow_modified Value : %lg",
           eval_modified(c, 2.5));
    t1 = get_tick();
    for (i = 0; i < LOOP; i++)
        eval_modified(c, 2.5);
    t2 = get_tick();
    printf("  Time : %ld", diff_tick(t1, t2));

    e = diff_polynomial(a);
    f = integ_polynomial(a);

    printf("\nDifferentiation -->");
    print_polynomial(e, "e(x)=");
    printf("\nIntegration -->");
    print_polynomial(f, "f(x)=");

    remove_polynomial(a);
    remove_polynomial(b);
    remove_polynomial(c);
    remove_polynomial(d);
    remove_polynomial(e);
    remove_polynomial(f);
    getch();
}
