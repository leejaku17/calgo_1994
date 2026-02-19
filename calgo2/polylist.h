/*                                                                  */
/*   POLYLIST.H  :  Header file for polynomial library              */
/*                                                                  */
/*                     Programmed By Kim Junmo                      */
/*                       Modified By Lee Jaekyu                     */

#ifndef _POLYLIST_H
#define _POLYLIST_H

typedef struct _node
{
    int degree;
    double coef;
    struct _node *next;
} node;

node *make_node(int degree, double coef);
void insert_node(node *head, int degree, double coef);
node *add_polynomial(node *a, node *b);
node *mult_polynomial(node *a, node *b);
node *diff_polynomial(node *a);
node *integ_polynomial(node *a);
double pow_simple(double x, int order);
double pow_modified(double x, int order);
double eval_horner_simple(node *a, double x);
double eval_horner_math(node *a, double x);
double eval_horner_modified(node *a, double x);
double eval_simple(node *a, double x);
double eval_math(node *a, double x);
double eval_modified(node *a, double x);
void print_polynomial(node *a, char *p);
void remove_polynomial(node *a);
node *input_polynomial(FILE *file);
node *lagrange(double points[][2], int n);

#endif
