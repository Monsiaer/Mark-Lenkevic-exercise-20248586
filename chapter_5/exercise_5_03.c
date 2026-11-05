/* Exercise 5-3. Append t to s using pointers instead of array indexing.
 * Read two lines without their newlines. Each may contain up to 999 bytes.
 * The caller must provide enough space in s for both strings and '\0'.
 */
#include <stdio.h>

#define MAXLINE 1000

void mystrcat(char *s, const char *t);
int read_line(char *s, int capacity);

int main(void)
{
    char s[2 * MAXLINE], t[MAXLINE];

    if (read_line(s, MAXLINE) != 1 || read_line(t, MAXLINE) != 1) {
        fprintf(stderr, "Enter two lines, each at most %d bytes.\n", MAXLINE - 1);
        return 1;
    }
    mystrcat(s, t);
    printf("%s\n", s);
    return 0;
}

void mystrcat(char *s, const char *t)
{
    while (*s != '\0')
        ++s;
    while ((*s++ = *t++) != '\0')
        ;
}

int read_line(char *s, int capacity)
{
    char *p = s;
    int c;

    while ((c = getchar()) != EOF && c != '\n') {
        if (p - s == capacity - 1)
            return -1;
        *p++ = (char)c;
    }
    *p = '\0';
    if (ferror(stdin))
        return -1;
    return c == '\n' || p != s;
}
