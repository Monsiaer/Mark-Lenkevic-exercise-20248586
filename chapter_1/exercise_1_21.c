/*
 * Exercise 1-21
 *
 * Write a program entab that replaces strings of blanks with the minimum
 * number of tabs and blanks to achieve the same spacing. Use the same tab
 * stops as for detab. When either a tab or a single blank would suffice to
 * reach a tab stop, which should be given preference?
 */
#include <stdio.h>

#define TAB_WIDTH 8

int main(void)
{
    int c, column = 0, spaces = 0;

    while ((c = getchar()) != EOF) {
        if (c == ' ') {
            ++spaces;
            column = (column + 1) % TAB_WIDTH;
            if (column == 0) {
                if (spaces > 1)
                    putchar('\t');
                else
                    putchar(' ');
                spaces = 0;
            }
        } else {

            if (c == '\t')
                spaces = 0;
            while (spaces > 0) {
                putchar(' ');
                --spaces;
            }
            putchar(c);
            if (c == '\n' || c == '\r' || c == '\t')
                column = 0;
            else
                column = (column + 1) % TAB_WIDTH;
        }
    }
    while (spaces > 0) {
        putchar(' ');
        --spaces;
    }
    return 0;
}
