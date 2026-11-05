/*
 * Work Examples - Postfix Notation
 *
 * Evaluate postfix expressions using a stack. Read operators and numbers
 * with getop, getch, and ungetch, and print the result on a newline.
 * Example: 1 2 - 4 5 + * produces -9.
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXOP 100
#define NUMBER '0'
#define INVALID '?'
#define BUFSIZE 100
#define MAXVAL 100

int sp = 0;
int bufp = 0;
double val[MAXVAL];
int buf[BUFSIZE];

int getch(void);
void ungetch(int c);
int getop(char s[]);
void push(double f);
double pop(void);

int main(void)
{
    int type;
    double op2;
    char s[MAXOP];

    for (; (type = getop(s)) != EOF;) {
        switch (type) {
        case NUMBER:
            push(atof(s));
            break;
        case '+':
            push(pop() + pop());
            break;
        case '*':
            push(pop() * pop());
            break;
        case '-':
            op2 = pop();
            push(pop() - op2);
            break;
        case '/':
            op2 = pop();
            if (op2 != 0.0)
                push(pop() / op2);
            else
                printf("error: zero divisor\n");
            break;
        case '\n':
            printf("\t%.8g\n", pop());
            break;
        default:
            printf("error: unknown command %s\n", s);
            break;
        }
    }
    return 0;
}

void push(double f)
{
    if (sp < MAXVAL)
        val[sp++] = f;
    else
        printf("error: stack full, cannot push %g\n", f);
}

double pop(void)
{
    if (sp > 0)
        return val[--sp];

    printf("error: stack empty\n");
    return 0.0;
}

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

void ungetch(int c)
{
    if (bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}

int getch(void)
{
    return bufp > 0 ? buf[--bufp] : getchar();
}
