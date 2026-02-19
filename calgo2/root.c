/*                                                                  */
/*   ROOT.C  :  Find root by numerical method     	            */
/*                                                                  */
/*                     Programmed By Kim Junmo                      */
/*                                                                  */

#include <math.h>

#define TOLERANCE 1e-10
#define MIN_SLOPE 1e-100
#define MAX_TRIES 2000

int found_flag;
int step;

double bisect(double (*f)(double), double x1, double x2)
{
    double x;
    step = 0;
    if (f(x1) * f(x2) > 0.0)
    {
        found_flag = 0;
        return x1;
    }
    while (fabs(x1 - x2) > TOLERANCE)
    {
        x = (x1 + x2) / 2.0;
        if (f(x1) * f(x) <= 0.0)
            x2 = x;
        else
            x1 = x;
        step++;
    }
    found_flag = 1;
    return (x1 + x2) / 2.;
}

int is_not_zero(double y)
{
    return (y > TOLERANCE || y < -TOLERANCE);
}

int slope_zero(double x)
{
    return (x < MIN_SLOPE && x > -MIN_SLOPE);
}

double falsi(double (*f)(double), double x1, double x2)
{
    double x;
    double devisor;
    step = 0;

    if (f(x1) * f(x2) > 0.0)
    {
        found_flag = 0;
        return x1;
    }
    while (is_not_zero(f(x)) && step < MAX_TRIES)
    {
        if (slope_zero(devisor = f(x2) - f(x1)))
            break;
        x = (x1 * f(x2) - x2 * f(x1)) / devisor;
        if (f(x1) * f(x) <= 0.0)
            x2 = x;
        else
            x1 = x;
        step++;
    }
    found_flag = (step == MAX_TRIES || slope_zero(devisor)) ? 0 : 1;
    return x;
}

double falsi_modified(double (*f)(double), double x1, double x2)
{
    double x, devisor;
    int c = 0, c1 = 1, c2 = 1;
    step = 0;

    if (f(x1) * f(x2) > 0.0)
    {
        found_flag = 0;
        return x1;
    }
    while (is_not_zero(f(x)) && step < MAX_TRIES)
    {
        if (slope_zero(devisor = f(x2) / c2 - f(x1) / c1))
            break;
        x = (x1 * f(x2) / c2 - x2 * f(x1) / c1) / devisor;

        if (f(x1) * f(x) < 0.0)
        {
            if (c < 0)
                c = 0;
            else
                c++;
            x2 = x;
        }
        else if (f(x) == 0.0)
            break;
        else
        {
            if (c > 0)
                c = 0;
            else
                c--;
            x1 = x;
        }
        step++;
        if (c >= 2)
        {
            c1 *= 2;
            c2 = 1;
        }
        else if (c <= -2)
        {
            c1 = 1;
            c2 *= 2;
        }
        else
        {
            c1 = 1;
            c2 = 1;
        }
    }
    found_flag = (step == MAX_TRIES || slope_zero(devisor)) ? 0 : 1;
    return x;
}

double secant(double (*f)(double), double x1, double x2)
{
    double root = x1, devisor;
    step = 0;

    while (fabs(x1 - x2) > TOLERANCE && step < MAX_TRIES)
    {
        if (slope_zero(devisor = f(x2) - f(x1)))
            break;
        root = x1 - f(x1) * (x2 - x1) / devisor;
        x1 = x2;
        x2 = root;
        step++;
    }
    found_flag = (step == MAX_TRIES || slope_zero(devisor)) ? 0 : 1;
    return root;
}

double newton(double (*f)(double), double (*fp)(double), double x0)
{
    double root = x0 + 2 * TOLERANCE, devisor;
    step = 0;

    while (fabs(x0 - root) > TOLERANCE && step < MAX_TRIES)
    {
        x0 = root;
        if (slope_zero(devisor = fp(x0)))
            break;
        root = x0 - f(x0) / devisor;
        step++;
    }
    found_flag = (step == MAX_TRIES || slope_zero(devisor)) ? 0 : 1;
    return root;
}

double f(double x)
{
    return pow(x, 4);
    return pow(x, 3) - 1;
}

double fp(double x)
{
    return 3 * pow(x, 2);
}

void main()
{
    clrscr();
    printf("\n%25.15g", bisect(f, -1, 10));
    printf("% 4d %d", step, found_flag);

    printf("\n%25.15g", falsi(f, -1, 10));
    printf("% 4d %d", step, found_flag);

    printf("\n%25.15g", falsi_modified(f, -1, 10));
    printf("% 4d %d", step, found_flag);

    printf("\n%25.15g", secant(f, -1, 10));
    printf("% 4d %d", step, found_flag);

    printf("\n%25.15g", newton(f, fp, 2));
    printf("% 4d %d", step, found_flag);
    getch();
}
