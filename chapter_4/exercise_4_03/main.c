/*
 * Exercise 4-3
 *
 * Extend the postfix calculator with the modulus (%) operator and support
 * for negative numbers.
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
        default:
            printf("error: invalid token\n");
            failed = 1;
            break;
        }
    }
    return 0;
}
