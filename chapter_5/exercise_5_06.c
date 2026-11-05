/* Exercise 5-6. Rewrite earlier functions using pointers instead of
 * array indexing: getline, atoi, itoa, reverse, strindex, and getop.
 */
#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include <stddef.h>

#define NUMBER '0'

int pointer_getline(char *s, int limit, FILE *input);
int pointer_atoi(const char *s);
void pointer_itoa(int n, char *s);
void pointer_reverse(char *s);
int pointer_strindex(const char *s, const char *t);
int pointer_getop(char *s, size_t capacity, FILE *input);

int main(void)
{
    char text[sizeof(int) * CHAR_BIT + 2], line[64], token[64];
    FILE *input = tmpfile();
    int type;

    if (input == NULL)
        return 1;
    pointer_itoa(INT_MIN, text);
    printf("itoa: %s\natoi: %d\n", text, pointer_atoi("  -12345"));
    pointer_reverse(text);
    printf("reverse: %s\nstrindex: %d\n", text,
           pointer_strindex("hello pointers", "pointers"));
    if (fputs("sample line\n12.5 3 +\n", input) < 0) {
        fclose(input);
        return 1;
    }
    rewind(input);
    if (pointer_getline(line, (int)sizeof(line), input) > 0)
        printf("getline: %s", line);
    while ((type = pointer_getop(token, sizeof(token), input)) != EOF)
        printf("getop: %s%s\n", type == NUMBER ? "number " : "", token);
    fclose(input);
    return 0;
}

int pointer_getline(char *s, int limit, FILE *input)
{
    char *start = s;
    int c;

    if (limit <= 0)
        return 0;
    while (s - start < limit - 1 && (c = getc(input)) != EOF) {
        *s++ = (char)c;
        if (c == '\n')
            break;
    }
    *s = '\0';
    return (int)(s - start);
}

int pointer_atoi(const char *s)
{
    int negative, digit;
    unsigned int value = 0, limit;

    while (isspace((unsigned char)*s))
        ++s;
    negative = *s == '-';
    if (*s == '+' || *s == '-')
        ++s;
    limit = negative ? 0u - (unsigned int)INT_MIN : (unsigned int)INT_MAX;
    while (isdigit((unsigned char)*s)) {
        digit = *s++ - '0';
        if (value > (limit - (unsigned int)digit) / 10u)
            return negative ? INT_MIN : INT_MAX;
        value = value * 10u + (unsigned int)digit;
    }
    if (!negative)
        return (int)value;
    return value == 0u - (unsigned int)INT_MIN ? INT_MIN : -(int)value;
}

void pointer_itoa(int n, char *s)
{
    char *start = s;
    unsigned int value = n < 0 ? 0u - (unsigned int)n : (unsigned int)n;

    do {
        *s++ = (char)('0' + value % 10u);
        value /= 10u;
    } while (value != 0);
    if (n < 0)
        *s++ = '-';
    *s = '\0';
    pointer_reverse(start);
}

void pointer_reverse(char *s)
{
    char *end = s, temp;

    while (*end != '\0')
        ++end;
    while (s < end) {
        --end;
        if (s >= end)
            break;
        temp = *s;
        *s++ = *end;
        *end = temp;
    }
}

int pointer_strindex(const char *s, const char *t)
{
    const char *start = s, *p, *q;

    if (*t == '\0')
        return 0;
    for (; *s != '\0'; ++s) {
        p = s;
        q = t;
        while (*q != '\0' && *p == *q) {
            ++p;
            ++q;
        }
        if (*q == '\0')
            return (int)(s - start);
    }
    return -1;
}

int pointer_getop(char *s, size_t capacity, FILE *input)
{
    char *start = s;
    size_t used = 0;
    int c;

    if (capacity < 2)
        return EOF;
    do {
        c = getc(input);
    } while (c != EOF && isspace((unsigned char)c));
    if (c == EOF) {
        *s = '\0';
        return EOF;
    }
    *s++ = (char)c;
    used = 1;
    if (!isdigit((unsigned char)c) && c != '.') {
        *s = '\0';
        return c;
    }
    if (isdigit((unsigned char)c)) {
        while ((c = getc(input)) != EOF && isdigit((unsigned char)c)) {
            if (used + 1 < capacity) {
                *s++ = (char)c;
                ++used;
            }
        }
    }
    if (c == '.') {
        if (start != s - 1 || *start != '.') {
            if (used + 1 < capacity) {
                *s++ = '.';
                ++used;
            }
        }
        while ((c = getc(input)) != EOF && isdigit((unsigned char)c)) {
            if (used + 1 < capacity) {
                *s++ = (char)c;
                ++used;
            }
        }
    }
    *s = '\0';
    if (c != EOF)
        ungetc(c, input);
    return NUMBER;
}
