/*
 * Exercise 1-9
 *
 * Write a program to copy its input to its output, replacing each string of
 * one or more blanks with a single blank.
 */
#include <stdio.h>

int main(void)
{
    int c;
    int previous_space = 0;

    while ((c = getchar()) != EOF) {
        if (c != ' ' || !previous_space)
            putchar(c);
        previous_space = (c == ' ');
    }
    return 0;
}
