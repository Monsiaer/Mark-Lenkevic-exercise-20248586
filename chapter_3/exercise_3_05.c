/*
 * Exercise 3-5
 *
 * Write itob(n, s, b) to convert the integer n into a base-b character
 * representation in s. In particular, itob(n, s, 16) produces hexadecimal.
 */
#include <stdio.h>
#include <limits.h>
#include <string.h>

void itob(int n, char s[], int b);
void reverse(char s[]);

int main(void)
{
    int bases[] = { 2, 8, 10, 16 };
    char result[sizeof(int) * CHAR_BIT + 2];
    size_t i;

    for (i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i) {
        itob(255, result, bases[i]);
        printf("itob(255, base %d) = %s\n", bases[i], result);
    }
    itob(-255, result, 16);
    printf("itob(-255, base 16) = %s\n", result);
    itob(INT_MIN, result, 16);
    printf("itob(%d, base 16) = %s\n", INT_MIN, result);
    return 0;
}

void itob(int n, char s[], int b)
{
    const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    unsigned int value = n < 0 ? 0u - (unsigned int)n : (unsigned int)n;
    size_t i = 0;

    if (b < 2 || b > 36) {
        s[0] = '\0';
        return;
    }
    for (;;) {
        s[i++] = digits[value % (unsigned int)b];
        value /= (unsigned int)b;
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
