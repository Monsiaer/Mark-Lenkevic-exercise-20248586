/*
 * Exercise 4-4 - Character input and pushback.
 */
#include <stdio.h>
#include "calc.h"

static int buf[BUFSIZE];
static int bufp = 0;

void ungetch(int c)
{
    if (bufp >= BUFSIZE)
        printf("error: too many pushed-back characters\n");
    else
        buf[bufp++] = c;
}

int getch(void)
{
    return bufp > 0 ? buf[--bufp] : getchar();
}
