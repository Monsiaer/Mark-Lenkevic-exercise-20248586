/*
 * Exercise 3-6
 *
 * Write a version of itoa that accepts a minimum field width as its third
 * argument. Pad the converted number with blanks on the left if necessary
 * to make it wide enough.
 */
#include <stdio.h>
#include <limits.h>
#include <string.h>

void itoa(int n, char s[], int width);
void reverse(char s[]);

int main(void)
{
    int values[] = { 0, 123, -123, INT_MAX, INT_MIN };
    char result[sizeof(int) * CHAR_BIT + 32];
    size_t i;

    for (i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        itoa(values[i], result, 14);
        printf("itoa(%d, width 14) = |%s|\n", values[i], result);
    }
    itoa(12345, result, 3);
    printf("itoa(12345, width 3) = |%s|\n", result);
    return 0;
}

void itoa(int n, char s[], int width)
{
    unsigned int value = n < 0 ? 0u - (unsigned int)n : (unsigned int)n;
    size_t i = 0;

    for (;;) {
        s[i++] = (char)('0' + value % 10u);
        value /= 10u;
        if (value == 0)
            break;
    }
    if (n < 0)
        s[i++] = '-';
    for (; width > 0 && i < (size_t)width; ++i)
        s[i] = ' ';
    s[i] = '\0';
    reverse(s);
}

void reverse(char s[])
{
    size_t i, length = strlen(s);
    char temp;

    for (i = 0; i < length / 2; ++i) {
        temp = s[i];
        s[i] = s[length - i - 1];
        s[length - i - 1] = temp;
    }
}
