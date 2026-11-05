/* Exercise 5-4. Return 1 if t occurs at the end of s, otherwise 0.
 * Use pointers. The empty string is a suffix of every string.
 */
#include <stdio.h>

#define MAXLINE 1000

int strend(const char *s, const char *t);
int read_line(char *s, int capacity);

int main(void)
{
    char s[MAXLINE], t[MAXLINE];

    if (read_line(s, MAXLINE) != 1 || read_line(t, MAXLINE) != 1) {
        fprintf(stderr, "Enter two lines, each at most %d bytes.\n", MAXLINE - 1);
        return 1;
    }
    printf("%d\n", strend(s, t));
    return 0;
}

int strend(const char *s, const char *t)
{
    const char *s_end = s, *t_end = t;

    while (*s_end != '\0')
        ++s_end;
    while (*t_end != '\0')
        ++t_end;
    while (t_end != t) {
        if (s_end == s || *--s_end != *--t_end)
            return 0;
    }
    return 1;
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
