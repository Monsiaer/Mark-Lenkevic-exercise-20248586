/* Exercise 4-8. Implement getch and ungetch when at most one character
 * of pushback is needed. Return failure if the single slot is occupied.
 */
#include <stdio.h>

static int saved;
static int occupied;

int getch(void);
int ungetch(int c);

int main(void)
{
    int c;

    while ((c = getch()) != EOF) {
        if (!ungetch(c))
            return 1;
        putchar(getch());
    }
    return ferror(stdin) ? 1 : 0;
}

int getch(void)
{
    if (occupied) {
        occupied = 0;
        return saved;
    }
    return getchar();
}

int ungetch(int c)
{
    if (occupied)
        return 0;
    saved = c;
    occupied = 1;
    return 1;
}
