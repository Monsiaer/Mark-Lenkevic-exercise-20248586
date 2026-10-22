/*
 * Exercise 1-8
 *
 * Write a program to count blanks, tabs, and newlines.
 */
#include <stdio.h>

int main(void)
{
    int c;
    unsigned long blanks = 0, tabs = 0, newlines = 0;

    while ((c = getchar()) != EOF) {
        if (c == ' ')
            ++blanks;
        else if (c == '\t')
            ++tabs;
        else if (c == '\n')
            ++newlines;
    }
    printf("Blanks: %lu\nTabs: %lu\nNewlines: %lu\n",
           blanks, tabs, newlines);
    return 0;
}
