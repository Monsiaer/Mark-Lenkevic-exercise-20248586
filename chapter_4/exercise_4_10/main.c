/*
 * Exercise 4-10
 *
 * Use whole-line input instead of getch and ungetch. Retain commands to
 * print, duplicate, swap, and clear stack elements.
 * Commands: p = print, d = duplicate, s = swap, c = clear.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "calc.h"

int main(void)
{
    char token[MAXOP];
    int type, failed = 0;
    double left, right;

    for (;;) {
        type = getop(token);
        if (type == '\n' || type == EOF) {
            if (!failed && stack_size() == 1)
                printf("%.8g\n", pop());
            else if (!failed && stack_size() > 1)
                printf("error: too many operands\n");
            clear_stack();
            failed = 0;
            if (type == EOF)
                break;
            continue;
        }
        if (failed)
            continue;

        switch (type) {
        case NUMBER:
            if (!push(atof(token))) {
                printf("error: stack full\n");
                failed = 1;
            }
            break;
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
            if (stack_size() < 2) {
                printf("error: not enough operands\n");
                failed = 1;
                break;
            }
            right = pop();
            left = pop();
            if ((type == '/' || type == '%') && right == 0.0) {
                printf("error: zero divisor\n");
                failed = 1;
                break;
            }
            switch (type) {
            case '+':
                push(left + right);
                break;
            case '-':
                push(left - right);
                break;
            case '*':
                push(left * right);
                break;
            case '/':
                push(left / right);
                break;
            case '%':
                push(fmod(left, right));
                break;
            }
            break;
        case 'p':
            if (stack_size() > 0)
                printf("%.8g\n", peek());
            else {
                printf("error: stack empty\n");
                failed = 1;
            }
            break;
        case 'd':
            if (stack_size() == 0) {
                printf("error: stack empty\n");
                failed = 1;
            } else if (!duplicate()) {
                printf("error: stack full\n");
                failed = 1;
            }
            break;
        case 's':
            if (!swap_top()) {
                printf("error: not enough operands\n");
                failed = 1;
            }
            break;
        case 'c':
            clear_stack();
            break;
        default:
            printf("error: invalid token\n");
            failed = 1;
            break;
        }
    }
    return 0;
}
