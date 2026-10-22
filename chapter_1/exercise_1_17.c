/*
 * Exercise 1-17
 *
 * Write a program to print all input lines that are longer than 80
 * characters.
 */
#include <stdio.h>

#define LIMIT 80

int main(void)
{
    char prefix[LIMIT];
    int c, length = 0, printing = 0;

    while ((c = getchar()) != EOF) {
        if (c == '\n') {
            if (printing)
                putchar(c);
            length = printing = 0;
        } else if (printing) {
            putchar(c);
        } else if (length < LIMIT) {
            prefix[length++] = (char)c;
        } else {
            fwrite(prefix, 1, LIMIT, stdout);
            putchar(c);
            printing = 1;
        }
    }
    return 0;
}
