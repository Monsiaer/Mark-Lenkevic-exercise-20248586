/*
 * Exercise 1-22
 *
 * Write a program to fold long input lines into two or more shorter lines
 * after the last non-blank character that occurs before the n-th column of
 * input. Make sure your program does something intelligent with very long
 * lines, and if there are no blanks or tabs before the specified column.
 */
#include <stdio.h>
#include <string.h>

#define WIDTH 80
#define TAB_WIDTH 8

void append(char line[], int *length, int c, int *folded);

int main(void)
{
    char line[WIDTH + 1];
    int c, length = 0, column = 0, spaces, folded = 0;

    while ((c = getchar()) != EOF) {
        if (c == '\n') {
            if (length > 0 || !folded) {
                fwrite(line, 1, (size_t)length, stdout);
                putchar('\n');
            }
            length = column = folded = 0;
        } else if (c == '\t') {
            spaces = TAB_WIDTH - column;
            while (spaces-- > 0)
                append(line, &length, ' ', &folded);
            column = 0;
        } else {
            append(line, &length, c, &folded);
            column = (column + 1) % TAB_WIDTH;
        }
    }
    fwrite(line, 1, (size_t)length, stdout);
    return 0;
}

void append(char line[], int *length, int c, int *folded)
{
    int split, start;

    line[(*length)++] = (char)c;
    if (*length <= WIDTH)
        return;

    for (split = WIDTH; split > 0 && line[split] != ' '; --split)
        ;
    if (split == 0) {
        split = WIDTH;
        start = WIDTH;
    } else {
        start = split + 1;
        while (split > 0 && line[split - 1] == ' ')
            --split;
    }
    fwrite(line, 1, (size_t)split, stdout);
    putchar('\n');
    *folded = 1;
    while (start < *length && line[start] == ' ')
        ++start;
    memmove(line, line + start, (size_t)(*length - start));
    *length -= start;
}
