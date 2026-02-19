/*                                                                  */
/*   EXPARSER.H  :  Header for Expression Parser 	            */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#ifndef _EXPARSER_H
#define _EXPARSER_H

#define MAXLEN 256
#define MAXVAR 26

void init_var(void);
void init_token(char *s);
int get_status(void);
void print_error(enum error e);
double get_var(int i);
double put_var(int i, double d);
void eval_exp(double *result);

#endif
