/* Exercise 4-7. Push back an entire string with ungets, implemented
 * through ungetch rather than by accessing its buffer directly.
 */
#include <stdio.h>
#include <string.h>

#define BUFSIZE 1000

static int buf[BUFSIZE];
static size_t bufp;

int getch(void);
int ungetch(int c);
int ungets(const char *s);

int main(void)
{
    char line[BUFSIZE];
    int c;

    if (fgets(line, sizeof(line), stdin) == NULL)
        return ferror(stdin) ? 1 : 0;
    if (!ungets(line)) {
        fputs("Pushback buffer is full.\n", stderr);
        return 1;
    }
    while ((c = getch()) != EOF)
        putchar(c);
    return ferror(stdin) ? 1 : 0;
}

int getch(void)
{
    return bufp > 0 ? buf[--bufp] : getchar();
}

int ungetch(int c)
{
    if (bufp == BUFSIZE)
        return 0;
    buf[bufp++] = c;
    return 1;
}

int ungets(const char *s)
{
    size_t n = strlen(s);

    while (n > 0)
        if (!ungetch((unsigned char)s[--n]))
            return 0;
    return 1;
}
