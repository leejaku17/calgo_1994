/*                                                                  */
/*   EXPARSER.C  :  Expression Parser               	            */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <stdio.h>
#include <string.h>
#include <alloc.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define MAXLEN 256
#define MAXVAR 26

char cur_token[MAXLEN];
char *cur_pos;

enum tok_type
{
    Unknown,
    Function,
    Variable,
    Number,
    Operator,
    Rel_Op
};
enum rel_op_type
{
    LT,
    GT,
    LE,
    GE,
    EQ,
    NE
};
enum error
{
    Ok,
    Divide_By_Zero,
    Rparen_Needed,
    Lparen_Needed,
    Syntax_Error
};

char *error_msg[] = {"Ok",
                     "Divide By Zero",
                     "')' is needed",
                     "'(' is needed",
                     "Syntax Error"};

enum error status = Ok;

char *relop_name[] = {"<", ">", "<=", ">=", "==", "!=", NULL};

enum tok_type token_type;

char *func_name[] = {"sin", "cos", "tan", "asin", "acos", "atan",
                     "sinh", "cosh", "tanh", "ceil", "abs",
                     "floor", "log", "log10", "sqrt", NULL};
double (*function[])(double) = {sin, cos, tan, asin, acos, atan,
                                sinh, cosh, tanh, ceil, fabs,
                                floor, log, log10, sqrt, NULL};

double var[MAXVAR];

int get_status(void)
{
    int i;
    i = status;
    status = Ok;
    return i;
}

void print_error(enum error e)
{
    printf("\n%s", error_msg[e]);
}

void init_var(void)
{
    int i;
    for (i = 0; i < MAXVAR; i++)
        var[i] = 0.0;
    var['P' - 'A'] = M_PI;
    var['E' - 'A'] = M_E;
}

int lookup_var(char *s)
{
    if (strlen(s) == 1 && 'A' <= toupper(*s) && toupper(*s) <= 'Z')
        return toupper(*s) - 'A';
    else
        return -1;
}

double get_var(int i)
{
    return var[toupper(i) - 'A'];
}

double put_var(int i, double d)
{
    var[toupper(i) - 'A'] = d;
    return d;
}

int lookup_func(char *s)
{
    int i;
    for (i = 0; func_name[i] && stricmp(func_name[i], s) != 0; i++)
        ;
    if (!func_name[i])
        return -1;
    else
        return i;
}

char *is_white(char c)
{
    return strchr(" \t\n\r", c);
}

char *is_delim(char c)
{
    return strchr("+-*^/%=;(),'"
                  "!<>&~`@#$\\:?/ []{}|",
                  c);
}

char *is_digit(char c)
{
    return strchr("0123456789.Ee-+", c);
}

char *is_alpha(char c)
{
    return strchr("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_", c);
}

void init_token(char *s)
{
    cur_pos = s;
    cur_token[0] = NULL;
}

void put_back(void)
{
    char *t;
    t = cur_token;
    while (*t++)
        cur_pos--;
}

int get_token(void)
{
    int i;
    while (is_white(*cur_pos) && *cur_pos)
        cur_pos++;

    if (*cur_pos == NULL)
    {
        cur_token[0] = NULL;
        token_type = Unknown;
        return 0; /* end of line */
    }

    /* check relation operator */
    if (strchr("!<>=", *cur_pos))
    {
        cur_token[0] = *cur_pos++; /* get first char */
        cur_token[1] = NULL;
        if (*cur_pos == '=') /*  ==, !=, >=, <=  */
        {
            cur_token[1] = *cur_pos++;
            cur_token[2] = NULL;
        }
        if (strcmp(cur_token, "=") == 0)
            token_type = Operator;
        else
            token_type = Rel_Op;
        return 1;
    }
    if (is_delim(*cur_pos))
    {
        cur_token[0] = *cur_pos++;
        cur_token[1] = NULL;
        token_type = Unknown;
        return 1;
    }
    if (is_alpha(*cur_pos))
    {
        i = 0;
        while (!is_delim(*cur_pos))
            cur_token[i++] = *cur_pos++;
        cur_token[i] = NULL;
        if (lookup_var(cur_token) != -1)
            token_type = Variable;
        else if (lookup_func(cur_token) != -1)
            token_type = Function;
        else
            token_type = Unknown;
        return 1;
    }
    if (is_digit(*cur_pos))
    {
        i = 0;
        while (is_digit(*cur_pos))
            cur_token[i++] = *cur_pos++;
        cur_token[i] = NULL;
        token_type = Number;
        return 1;
    }
    return 0;
}

void proc_exp_assign(double *result);
void proc_exp_relop(double *result);
void proc_exp_2plus(double *result);
void proc_exp_2mult(double *result);
void proc_exp_power(double *result);
void proc_exp_unary(double *result);
void proc_exp_paren(double *result);
void proc_exp_atom(double *result);

void eval_exp(double *result)
{
    if (!get_token())
    {
        *result = 0;
        return;
    }
    proc_exp_assign(result);
    put_back();
}

void proc_exp_assign(double *result)
{
    char temp_token[MAXLEN];
    enum tok_type temp_type;
    int i;
    if (token_type == Variable)
    {
        strcpy(temp_token, cur_token);
        temp_type = token_type;
        i = cur_token[0];
        get_token();
        if (strcmp(cur_token, "=") == 0) /* if assignment */
        {
            get_token();
            proc_exp_assign(result);
            put_var(i, *result);
            return;
        }
        put_back();
        strcpy(cur_token, temp_token);
        token_type = temp_type;
    }
    proc_exp_relop(result);
}

enum rel_op_type lookup_relop(char *s)
{
    int i;
    for (i = 0; relop_name[i] && stricmp(relop_name[i], s) != 0; i++)
        ;
    if (!relop_name[i])
        return -1;
    else
        return i;
}

void proc_exp_relop(double *result)
{
    double second;
    enum rel_op_type relop;

    proc_exp_2plus(result);
    if (token_type != Rel_Op)
        return;
    relop = lookup_relop(cur_token);
    get_token();
    proc_exp_2plus(&second);
    if (relop == LT)
        *result = *result < second;
    else if (relop == GT)
        *result = *result > second;
    else if (relop == LE)
        *result = *result <= second;
    else if (relop == GE)
        *result = *result >= second;
    else if (relop == EQ)
        *result = *result == second;
    else if (relop == NE)
        *result = *result != second;
}

void proc_exp_2plus(double *result)
{
    double second;
    int op;
    proc_exp_2mult(result);
    while (cur_token[0] == '+' || cur_token[0] == '-')
    {
        op = cur_token[0];
        get_token();
        proc_exp_2mult(&second);
        if (op == '+')
            *result = *result + second;
        else if (op == '-')
            *result = *result - second;
    }
}

void proc_exp_2mult(double *result)
{
    double second;
    int op;
    proc_exp_power(result);
    while (cur_token[0] == '*' || cur_token[0] == '/')
    {
        op = cur_token[0];
        get_token();
        proc_exp_power(&second);
        if (op == '*')
            *result = *result * second;
        else if (op == '/')
        {
            if (second == 0.0)
            {
                status = Divide_By_Zero;
                return;
            }
            *result = *result / second;
        }
    }
}

void proc_exp_power(double *result)
{
    double second;
    proc_exp_unary(result);
    while (cur_token[0] == '^')
    {
        get_token();
        proc_exp_unary(&second);
        *result = pow(*result, second);
    }
}

void proc_exp_unary(double *result)
{
    int op = 0;
    if (cur_token[0] == '+' || cur_token[0] == '-')
    {
        op = cur_token[0];
        get_token();
    }
    proc_exp_paren(result);
    if (op == '-')
        *result = -(*result);
}

void proc_exp_paren(double *result)
{
    if (cur_token[0] == '(')
    {
        get_token();
        proc_exp_assign(result);
        if (cur_token[0] != ')')
        {
            status = Rparen_Needed;
            return;
        }
        get_token();
    }
    else
        proc_exp_atom(result);
}

void proc_exp_atom(double *result)
{
    double arg;
    char *endptr;
    char *temp_cur_pos;
    int i;
    int p;
    if (token_type == Function)
    {
        i = lookup_func(cur_token);
        if (!get_token())
        {
            status = Lparen_Needed;
            return;
        }
        if (strcmp(cur_token, "(") != 0)
        {
            status = Lparen_Needed;
            return;
        }
        temp_cur_pos = cur_pos;
        for (p = 0; *temp_cur_pos && (*temp_cur_pos != ')' || p != 0);
             temp_cur_pos++)
        {
            if (*temp_cur_pos == '(')
                p++;
            if (*temp_cur_pos == ')')
                p--;
        }
        if (*temp_cur_pos == NULL)
        {
            status = Rparen_Needed;
            return;
        }
        *temp_cur_pos = NULL;
        get_token();
        proc_exp_assign(&arg);
        *temp_cur_pos = ')';
        get_token();
        get_token();
        *result = function[i](arg);
        return;
    }
    if (token_type == Variable)
    {
        *result = get_var(cur_token[0]);
        get_token();
        return;
    }
    if (token_type == Number)
    {
        *result = strtod(cur_token, &endptr);
        get_token();
        return;
    }
    status = Syntax_Error;
}
