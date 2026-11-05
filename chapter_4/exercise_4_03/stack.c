/*
 * Exercise 4-3 - Stack operations.
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
