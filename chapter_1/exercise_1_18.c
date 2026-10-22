/*
 * Exercise 1-18
 *
 * Write a program to remove trailing blanks and tabs from each line of input,
 * and to delete entirely blank lines.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(void)
{
    char *line = NULL, *larger;
    size_t length = 0, capacity = 0, next;
    int c;

    do {
        c = getchar();
        if (c == '\n' || c == EOF) {
            while (length > 0 &&
                   (line[length - 1] == ' ' || line[length - 1] == '\t'))
                --length;
            if (length > 0) {
                fwrite(line, 1, length, stdout);
                if (c == '\n')
                    putchar('\n');
            }
            length = 0;
        } else {
            if (length == capacity) {
                if (capacity > SIZE_MAX / 2) {
                    free(line);
                    fprintf(stderr, "Line is too long.\n");
                    return 1;
                }
                next = capacity == 0 ? 128 : capacity * 2;
                larger = realloc(line, next);
                if (larger == NULL) {
                    free(line);
                    fprintf(stderr, "Not enough memory.\n");
                    return 1;
                }
                line = larger;
                capacity = next;
            }
            line[length++] = (char)c;
        }
    } while (c != EOF);

    free(line);
    return 0;
}
