/* Exercise 5-10. Evaluate a reverse Polish expression from argv.
 * Example: exercise_5_10 2 3 4 + "*"  -> 14
 * Each number/operator is a separate argument. Operators: + - * / %.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    double *stack, value, left, right;
    char *end;
    const char *token;
    int i, count = 0;

    if (argc < 2) {
        fprintf(stderr, "Usage: exercise_5_10 number number operator ...\n");
        return 1;
    }
    stack = calloc((size_t)argc, sizeof(*stack));
    if (stack == NULL) {
        fprintf(stderr, "Not enough memory.\n");
        return 1;
    }
    for (i = 1; i < argc; ++i) {
        token = argv[i];
        if (token[0] != '\0' && token[1] == '\0' &&
            strchr("+-*/%", token[0]) != NULL) {
            if (count < 2) {
                fprintf(stderr, "Not enough operands for %s.\n", token);
                free(stack);
                return 1;
            }
            right = stack[--count];
            left = stack[--count];
            if ((token[0] == '/' || token[0] == '%') && right == 0.0) {
                fprintf(stderr, "Division by zero.\n");
                free(stack);
                return 1;
            }
            switch (token[0]) {
            case '+': value = left + right; break;
            case '-': value = left - right; break;
            case '*': value = left * right; break;
            case '/': value = left / right; break;
            default:  value = fmod(left, right); break;
            }
            if (!isfinite(value)) {
                fprintf(stderr, "Arithmetic result is not finite.\n");
                free(stack);
                return 1;
            }
        } else {
            errno = 0;
            value = strtod(token, &end);
            if (end == token || *end != '\0' || errno == ERANGE || !isfinite(value)) {
                fprintf(stderr, "Invalid number or operator: %s\n", token);
                free(stack);
                return 1;
            }
        }
        stack[count++] = value;
    }
    if (count != 1) {
        fprintf(stderr, "Expression leaves %d values; expected one.\n", count);
        free(stack);
        return 1;
    }
    printf("%.10g\n", stack[0]);
    free(stack);
    return 0;
}
