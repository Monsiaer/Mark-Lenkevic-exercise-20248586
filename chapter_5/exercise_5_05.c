/* Exercise 5-5. Write pointer versions of strncpy, strncat, and strncmp
 * that operate on at most the first n characters of their arguments.
 */
#include <stdio.h>
#include <stddef.h>

char *mystrncpy(char *s, const char *t, size_t n);
char *mystrncat(char *s, const char *t, size_t n);
int mystrncmp(const char *s, const char *t, size_t n);

int main(void)
{
    char copy[16] = { 0 }, joined[32] = "hello";

    mystrncpy(copy, "pointer", 5);
    mystrncat(joined, " world!", 6);
    printf("copy: %s\njoined: %s\ncomparison: %d\n",
           copy, joined, mystrncmp("pointer", "pointed", 6));
    return 0;
}

char *mystrncpy(char *s, const char *t, size_t n)
{
    char *start = s;

    while (n != 0 && *t != '\0') {
        *s++ = *t++;
        --n;
    }
    while (n != 0) {
        *s++ = '\0';
        --n;
    }
    return start;
}

char *mystrncat(char *s, const char *t, size_t n)
{
    char *start = s;

    while (*s != '\0')
        ++s;
    while (n != 0 && *t != '\0') {
        *s++ = *t++;
        --n;
    }
    *s = '\0';
    return start;
}

int mystrncmp(const char *s, const char *t, size_t n)
{
    while (n != 0) {
        if ((unsigned char)*s != (unsigned char)*t)
            return (unsigned char)*s - (unsigned char)*t;
        if (*s == '\0')
            return 0;
        ++s;
        ++t;
        --n;
    }
    return 0;
}
