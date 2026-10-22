/*
 * Exercise 1-24
 *
 * Write a program to check a C program for rudimentary syntax errors like
 * unbalanced parentheses, brackets, and braces. Handle single and double
 * quotes, escape sequences, and comments. This program is difficult to write
 * in full generality.
 */
#include <stdio.h>

#define MAX_DEPTH 1000

enum state { CODE, STRING, CHARACTER, BLOCK_COMMENT, LINE_COMMENT };

int source_char(void);
int matches(int opening, int closing);

int main(void)
{
    char stack[MAX_DEPTH];
    int depth = 0, c, next, escaped = 0, previous = 0, errors = 0;
    int pending = EOF;
    enum state state = CODE;

    for (;;) {
        c = pending != EOF ? pending : source_char();
        pending = EOF;
        if (c == EOF)
            break;
        if (state == BLOCK_COMMENT) {
            if (previous == '*' && c == '/') {
                state = CODE;
                previous = 0;
            } else {
                previous = c;
            }
        } else if (state == LINE_COMMENT) {
            if (c == '\n')
                state = CODE;
        } else if (state == STRING || state == CHARACTER) {
            if (c == '\n') {
                fprintf(stderr, "Unterminated quoted literal before newline.\n");
                ++errors;
                escaped = 0;
                state = CODE;
            } else if (escaped) {
                escaped = 0;
            } else if (c == '\\') {
                escaped = 1;
            } else if ((state == STRING && c == '"') ||
                       (state == CHARACTER && c == '\'')) {
                state = CODE;
            }
        } else if (c == '/') {
            next = source_char();
            if (next == '*') {
                state = BLOCK_COMMENT;
                previous = 0;
            } else if (next == '/') {
                state = LINE_COMMENT;
            } else {
                pending = next;
            }
        } else if (c == '"' || c == '\'') {
            state = c == '"' ? STRING : CHARACTER;
        } else if (c == '(' || c == '[' || c == '{') {
            if (depth == MAX_DEPTH) {
                fprintf(stderr, "Nesting exceeds %d levels.\n", MAX_DEPTH);
                return 1;
            }
            stack[depth++] = (char)c;
        } else if (c == ')' || c == ']' || c == '}') {
            if (depth == 0 || !matches(stack[depth - 1], c)) {
                fprintf(stderr, "Unexpected closing delimiter: %c\n", c);
                ++errors;
            } else {
                --depth;
            }
        }
    }
    if (state == BLOCK_COMMENT || state == STRING || state == CHARACTER) {
        fprintf(stderr, "Unterminated comment or quoted literal at EOF.\n");
        ++errors;
    }
    while (depth > 0) {
        fprintf(stderr, "Unclosed delimiter: %c\n", stack[--depth]);
        ++errors;
    }
    if (errors == 0)
        printf("No syntax errors found by this checker.\n");
    return errors != 0;
}

int matches(int opening, int closing)
{
    return (opening == '(' && closing == ')') ||
           (opening == '[' && closing == ']') ||
           (opening == '{' && closing == '}');
}

int source_char(void)
{
    int c, next;

    while ((c = getchar()) == '\\') {
        next = getchar();
        if (next != '\n') {
            if (next != EOF)
                ungetc(next, stdin);
            return c;
        }
    }
    return c;
}
