/*
 * Exercise 4-4 - Operator and number input.
 */
#include <ctype.h>
#include <stdio.h>
#include "calc.h"

int getop(char s[])
{
    int i = 0, c, next, digits = 0, too_long = 0;

    for (c = getch(); c == ' ' || c == '\t' || c == '\r'; c = getch())
        ;
    s[0] = '\0';
    if (c == EOF)
        return EOF;
    s[0] = (char)c;
    s[1] = '\0';

    if (c == '-') {
        next = getch();
        if (!isdigit(next) && next != '.') {
            if (next != EOF)
                ungetch(next);
            return '-';
        }
        s[i++] = '-';
        c = next;
    }
    if (!isdigit(c) && c != '.')
        return c;

    for (; isdigit(c); c = getch()) {
        if (i < MAXOP - 1)
            s[i++] = (char)c;
        else
            too_long = 1;
        ++digits;
    }
    if (c == '.') {
        if (i < MAXOP - 1)
            s[i++] = '.';
        else
            too_long = 1;
        for (c = getch(); isdigit(c); c = getch()) {
            if (i < MAXOP - 1)
                s[i++] = (char)c;
            else
                too_long = 1;
            ++digits;
        }
    }
    s[i] = '\0';
    if (c != EOF)
        ungetch(c);
    if (too_long || digits == 0)
        return INVALID;
    return NUMBER;
}
