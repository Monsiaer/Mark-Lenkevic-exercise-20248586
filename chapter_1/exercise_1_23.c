/*
 * Exercise 1-23
 *
 * Write a program to remove all comments from a C program. Handle quoted
 * strings and character constants properly. C comments do not nest.
 */
#include <stdio.h>

enum state { CODE, STRING, CHARACTER, BLOCK_COMMENT, LINE_COMMENT };

int source_char(void);

int main(void)
{
    int c, next, escaped = 0, previous = 0;
    enum state state = CODE;

    while ((c = source_char()) != EOF) {
        if (state == BLOCK_COMMENT) {
            if (c == '\n')
                putchar('\n');
            if (previous == '*' && c == '/') {
                state = CODE;
                previous = 0;
            } else {
                previous = c;
            }
        } else if (state == LINE_COMMENT) {
            if (c == '\n') {
                putchar(c);
                state = CODE;
            }
        } else if (state == STRING || state == CHARACTER) {
            putchar(c);
            if (escaped)
                escaped = 0;
            else if (c == '\\')
                escaped = 1;
            else if ((state == STRING && c == '"') ||
                     (state == CHARACTER && c == '\''))
                state = CODE;
        } else if (c == '/') {
            next = source_char();
            if (next == '*' || next == '/') {
                putchar(' ');
                state = next == '*' ? BLOCK_COMMENT : LINE_COMMENT;
                previous = 0;
            } else {
                putchar(c);

                if (next == EOF)
                    break;
                putchar(next);
                if (next == '"')
                    state = STRING;
                else if (next == '\'')
                    state = CHARACTER;
            }
        } else {
            putchar(c);
            if (c == '"')
                state = STRING;
            else if (c == '\'')
                state = CHARACTER;
        }
    }
    if (state == BLOCK_COMMENT || state == STRING || state == CHARACTER) {
        fprintf(stderr, "Unterminated comment or quoted literal.\n");
        return 1;
    }
    return 0;
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
