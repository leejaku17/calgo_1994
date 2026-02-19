/*                                                                  */
/*   INTSTACK.C  :  Stack of Integer                                */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#define MAX_STACK_SIZE 100

int stack[MAX_STACK_SIZE];
int top;

void init_stack(void)
{
    top = -1;
}

int push(int t)
{
    if (top >= MAX_STACK_SIZE - 1)
    {
        printf("\n    Stack overflow.");
        return -1;
    }
    stack[++top] = t;
    return t;
}

int pop(void)
{
    if (top < 0)
    {
        printf("\n    Stack underflow.");
        return -1;
    }
    return stack[top--];
}

int stack_empty(void)
{
    return (top < 0);
}

void print_stack(void)
{
    int i;
    printf("\n  Stack contents : Top ----> Bottom\n");
    for (i = top; i >= 0; i--)
        printf("%-6d", stack[i]);
}
