/*
 * Exercise 1-2
 *
 * Experiment to find out what happens when the argument string of printf
 * contains \c, where c is a character that is not a recognized escape
 * sequence.
 */
#include <stdio.h>

int main(void)
{
#ifdef TEST_UNKNOWN_ESCAPE
    printf("Unknown escape: \q\n");
#else
    printf("Newline: first\nsecond\n");
    printf("Tab: left\tright\n");
    printf("Quote: \"  Backslash: \\\n");
    printf("Enable TEST_UNKNOWN_ESCAPE to experiment with \\q.\n");
#endif
    return 0;
}
