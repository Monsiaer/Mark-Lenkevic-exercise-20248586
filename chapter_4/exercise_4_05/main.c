/* Exercise 4-5. Add sin, exp, and pow to the postfix calculator. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include "calc.h"

int main(void)
{
    char token[MAXOP], *end;
    int type, failed = 0, needed;
    double left, right, result;

    for (;;) {
        type = getop(token);
        if (type == '\n' || type == EOF) {
            if (!failed && stack_size() == 1)
                printf("%.8g\n", pop());
            else if (!failed && stack_size() > 1)
                puts("error: too many operands");
            clear_stack();
            failed = 0;
            if (type == EOF)
                break;
            continue;
        }
        if (failed)
            continue;
        if (type == NUMBER) {
            errno = 0;
            result = strtod(token, &end);
            if (*end != '\0' || errno == ERANGE || !isfinite(result)) {
                puts("error: invalid number");
                failed = 1;
            } else if (!push(result)) {
                puts("error: stack full");
                failed = 1;
            }
        } else if (type == NAME &&
                   (strcmp(token, "sin") == 0 || strcmp(token, "exp") == 0 ||
                    strcmp(token, "pow") == 0)) {
            needed = strcmp(token, "pow") == 0 ? 2 : 1;
            if (stack_size() < needed) {
                puts("error: not enough operands");
                failed = 1;
                continue;
            }
            right = pop();
            errno = 0;
            if (needed == 2) {
                left = pop();
                result = pow(left, right);
            } else
                result = strcmp(token, "sin") == 0 ? sin(right) : exp(right);
            if (errno == EDOM || errno == ERANGE || !isfinite(result)) {
                puts("error: math domain or range");
                failed = 1;
            } else
                push(result);
        } else if (type != EOF && strchr("+-*/%", type) != NULL) {
            if (stack_size() < 2) {
                puts("error: not enough operands");
                failed = 1;
                continue;
            }
            right = pop();
            left = pop();
            if ((type == '/' || type == '%') && right == 0.0) {
                puts("error: zero divisor");
                failed = 1;
                continue;
            }
            switch (type) {
            case '+': result = left + right; break;
            case '-': result = left - right; break;
            case '*': result = left * right; break;
            case '/': result = left / right; break;
            default: result = fmod(left, right); break;
            }
            if (!isfinite(result)) {
                puts("error: math domain or range");
                failed = 1;
            } else
                push(result);
        } else if (type == NAME && strcmp(token, "p") == 0) {
            if (stack_size() > 0)
                printf("%.8g\n", peek());
            else {
                puts("error: stack empty");
                failed = 1;
            }
        } else if (type == NAME && strcmp(token, "d") == 0) {
            if (!duplicate()) {
                puts("error: cannot duplicate");
                failed = 1;
            }
        } else if (type == NAME && strcmp(token, "s") == 0) {
            if (!swap_top()) {
                puts("error: not enough operands");
                failed = 1;
            }
        } else if (type == NAME && strcmp(token, "c") == 0)
            clear_stack();
        else {
            puts("error: invalid token");
            failed = 1;
        }
    }
    return 0;
}
