/*
 * Exercise 4-6 - Stack operations.
 */
#include "calc.h"

static int sp = 0;
static double val[MAXVAL];

int push(double value)
{
    if (sp >= MAXVAL)
        return 0;
    val[sp++] = value;
    return 1;
}

double pop(void)
{
    return sp > 0 ? val[--sp] : 0.0;
}

int stack_size(void)
{
    return sp;
}

void clear_stack(void)
{
    sp = 0;
}

double peek(void)
{
    return sp > 0 ? val[sp - 1] : 0.0;
}

int duplicate(void)
{
    if (sp == 0)
        return 0;
    return push(peek());
}

int swap_top(void)
{
    double temp;

    if (sp < 2)
        return 0;
    temp = val[sp - 1];
    val[sp - 1] = val[sp - 2];
    val[sp - 2] = temp;
    return 1;
}
