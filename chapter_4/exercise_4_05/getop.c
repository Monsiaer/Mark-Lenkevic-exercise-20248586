/* Read numbers, operator characters, and named calculator commands. */
#include <ctype.h>
#include <stdio.h>
#include "calc.h"

int getop(char s[])
{
    int c, next, i = 0, digits = 0, too_long = 0;

    do {
        c = getch();
    } while (c == ' ' || c == '\t' || c == '\r');
    s[0] = '\0';
    if (c == EOF)
        return EOF;
    if (isalpha(c)) {
        do {
            if (i < MAXOP - 1)
                s[i++] = (char)c;
            else
                too_long = 1;
            c = getch();
        } while (isalpha(c));
        if (c != EOF)
            ungetch(c);
        s[i] = '\0';
        return too_long ? INVALID : NAME;
    }
    s[0] = (char)c;
    s[1] = '\0';
    if (c == '-' || c == '+') {
        next = getch();
        if (!isdigit(next) && next != '.') {
            if (next != EOF)
                ungetch(next);
            return c;
        }
        s[i++] = (char)c;
        c = next;
    }
    if (!isdigit(c) && c != '.')
        return c;
    while (isdigit(c)) {
        if (i < MAXOP - 1)
            s[i++] = (char)c;
        else
            too_long = 1;
        ++digits;
        c = getch();
    }
    if (c == '.') {
        if (i < MAXOP - 1)
            s[i++] = '.';
        else
            too_long = 1;
        while (isdigit(c = getch())) {
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
    return too_long || digits == 0 ? INVALID : NUMBER;
}
