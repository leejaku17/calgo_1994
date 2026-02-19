/*                                                                  */
/*   DIFFINTG.C  :  Numerical differnce & integration 	            */
/*                                                                  */
/*                     Programmed By Kim Junmo                      */
/*                                                                  */

#include <math.h>

#define TOLERANCE 1e-10
#define MAX_TRIES 1000
#define MIN_SLOPE 1e-100

int found_flag;
int step;

double trpz(double (*f)(double), double a, double b, int n)
{
    int i;
    double h = (b - a) / n, sum = 0;

    for (i = 0; i < n; i++)
        sum += (f(a + i * h) + f(a + (i + 1) * h)) * h / 2;
    return sum;
}

double rect(double (*f)(double), double a, double b, int n)
{
    int i;
    double h = (b - a) / n, sum = 0;

    for (i = 0; i < n; i++)
        sum += f(a + h / 2 + i * h) * h;
    return sum;
}

double smps(double (*f)(double), double a, double b, int n)
{
    int i;
    double h = (b - a) / n, sum = 0;

    for (i = 0; i < n; i++)
        sum += (f(a + i * h) + 4 * f(a + i * h + h / 2) + f(a + (i + 1) * h)) * h / 6;
    return sum;
}

double adapt(double (*f)(double), double a, double b, double tolerance)
{
    double x = smps(f, a, b, 4);
    if (fabs(x - smps(f, a, b, 2)) > tolerance)
        return adapt(f, a, (a + b) / 2, tolerance) + adapt(f, (a + b) / 2, b, tolerance);
    step += 4;
    return x;
}

double diffof1(double (*f)(double), double x, double h)
{
    return (-1 * f(x + 2 * h) + 8 * f(x + h) - 8 * f(x - h) + f(x - 2 * h)) / 12. / h;
}

double diffof1_1(double (*f)(double), double x, double h)
{
    return (f(x + h) - f(x - h)) / 2. / h;
}

double diffof2(double (*f)(double), double x, double h)
{
    return (-1 * f(x + 2 * h) + 16 * f(x + h) - 30 * f(x) + 16 * f(x - h) - f(x - 2 * h)) / 12. / h / h;
}

double diffof2_1(double (*f)(double), double x, double h)
{
    return (f(x + h) - 2 * f(x) + f(x - h)) / h / h;
}

double func(double x)
{
    return pow(x, 5) - pow(x, 4);
}
double fp(double x)
{
    return 5 * pow(x, 4) - 4 * pow(x, 3);
}

int slope_zero(double x)
{
    return (x < MIN_SLOPE && x > -MIN_SLOPE);
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

double newton_2(double (*f)(double), double x0)
{
    double root = x0 + 2 * TOLERANCE, devisor;
    step = 0;

    while (fabs(x0 - root) > TOLERANCE && step < MAX_TRIES)
    {
        x0 = root;
        if (slope_zero(devisor = diffof1(f, x0, 1e-4)))
            break;
        root = x0 - f(x0) / devisor;
        step++;
    }
    found_flag = (step == MAX_TRIES || slope_zero(devisor)) ? 0 : 1;
    return root;
}

void main()
{
    clrscr();
    printf("\nadapt: %25.20lf", adapt(func, 0, 10, 1e-10));
    printf("  step : %d", step);
    printf("\ntrpz : %25.20lf", trpz(func, 0, 10, step * 2));
    printf("\nrect : %25.20lf", rect(func, 0, 10, step * 2));
    printf("\nsmps : %25.20lf", smps(func, 0, 10, step));

    printf("\n%25.20lf", diffof1_1(func, 1, 1e-5));
    printf("\n%25.20lf", diffof1(func, 1, 1e-5));
    printf("\n%25.20lf", diffof2_1(func, 1, 1e-3));
    printf("\n%25.20lf", diffof2(func, 1, 1e-3));

    printf("\n%25.20lf", newton(func, fp, 10));
    printf(" %d", step);
    printf("\n%25.20lf", newton_2(func, 10));
    printf(" %d", step);
    getch();
}
