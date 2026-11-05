/*
 * Exercise 3-4
 *
 * Explain why the original itoa cannot handle the most negative integer
 * in two's complement, and modify it to convert that value correctly.
 *
 * Negating INT_MIN would require a positive value greater than INT_MAX.
 * Unsigned arithmetic can represent its magnitude without signed overflow.
 */
#include <stdio.h>
#include <limits.h>
#include <string.h>

void itoa(int n, char s[]);
void reverse(char s[]);

int main(void)
{
    int values[] = { 0, 12345, -12345, INT_MAX, INT_MIN };
    char result[sizeof(int) * CHAR_BIT + 2];
    size_t i;

    for (i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        itoa(values[i], result);
        printf("itoa(%d) = %s\n", values[i], result);
    }
    return 0;
}

void itoa(int n, char s[])
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
