/* Exercise 4-10. Read an entire input line before scanning tokens;
 * getch and ungetch are unnecessary. Reject lines beyond 4095 bytes.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "calc.h"

static char line[4096];
static char *position;
static int pending_newline;

int getop(char s[])
{
    char *end;
    size_t length;
    int c;

    s[0] = '\0';
    if (pending_newline && (position == NULL || *position == '\0')) {
        pending_newline = 0;
        position = NULL;
        return '\n';
    }
    if (position == NULL || *position == '\0') {
        if (fgets(line, sizeof(line), stdin) == NULL)
            return EOF;
        if (strchr(line, '\n') == NULL) {
            c = getchar();
            if (c != EOF && c != '\n') {
                while ((c = getchar()) != EOF && c != '\n')
                    ;
                position = NULL;
                pending_newline = 1;
                return INVALID;
            }
            if (c == '\n')
                pending_newline = 1;
        }
        position = line;
    }
    while (*position == ' ' || *position == '\t' || *position == '\r')
        ++position;
    if (*position == '\0') {
        position = NULL;
        pending_newline = 0;
        return '\n';
    }
    if (isdigit((unsigned char)*position) || *position == '.' ||
        ((*position == '-' || *position == '+') &&
         (isdigit((unsigned char)position[1]) || position[1] == '.'))) {
        (void)strtod(position, &end);
        if (end == position) {
            ++position;
            return INVALID;
        }
        length = (size_t)(end - position);
        if (length >= MAXOP) {
            position = end;
            return INVALID;
        }
        memcpy(s, position, length);
        s[length] = '\0';
        position = end;
        return NUMBER;
    }
    c = (unsigned char)*position++;
    s[0] = (char)c;
    s[1] = '\0';
    return c;
}
