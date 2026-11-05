/* Exercise 4-9. Handle pushed-back EOF correctly. Store int values so
 * EOF remains distinct from every unsigned char. A pushed EOF is returned
 * once; the following getch resumes earlier pushback or underlying input.
 */
#include <stdio.h>

#define BUFSIZE 100

static int buf[BUFSIZE];
static int bufp;

int getch(void);
int ungetch(int c);

int main(void)
{
    int c;

    if (!ungetch(EOF) || getch() != EOF)
        return 1;
    puts("pushed EOF: preserved");
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
