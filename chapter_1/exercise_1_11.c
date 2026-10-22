/*
 * Exercise 1-11
 *
 * How would you test the word count program? What kinds of input are most
 * likely to uncover bugs if there are any?
 */
#include <stdio.h>

#define IN 1
#define OUT 0

int main(void)
{
    int c, state = OUT;
    unsigned long lines = 0, words = 0, characters = 0;

    while ((c = getchar()) != EOF) {
        ++characters;
        if (c == '\n')
            ++lines;
        if (c == ' ' || c == '\t' || c == '\n')
            state = OUT;
        else if (state == OUT) {
            state = IN;
            ++words;
        }
    }
    printf("Lines: %lu\nWords: %lu\nCharacters: %lu\n",
           lines, words, characters);
    return 0;
}
