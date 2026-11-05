/* Check pushback ordering, full buffers, one-slot preservation, and EOF. */
#define _CRT_SECURE_NO_WARNINGS
#define main exercise_demo
#if TEST_EXERCISE == 7
#include "../chapter_4/exercise_4_07.c"
#elif TEST_EXERCISE == 8
#include "../chapter_4/exercise_4_08.c"
#elif TEST_EXERCISE == 9
#include "../chapter_4/exercise_4_09.c"
#else
#error Choose TEST_EXERCISE as 7, 8, or 9
#endif
#undef main

#define CHECK(x) do { if (!(x)) { fprintf(stderr, "Failed at line %d\n", __LINE__); return 1; } } while (0)

int main(void)
{
#if TEST_EXERCISE == 7
    int i;

    CHECK(ungets("abc"));
    CHECK(ungets("XY"));
    CHECK(getch() == 'X' && getch() == 'Y');
    CHECK(getch() == 'a' && getch() == 'b' && getch() == 'c');
    CHECK(ungets(""));
    for (i = 0; i < BUFSIZE; ++i)
        CHECK(ungetch(i));
    CHECK(!ungetch('x'));
    for (i = BUFSIZE - 1; i >= 0; --i)
        CHECK(getch() == i);
#elif TEST_EXERCISE == 8
    CHECK(ungetch('A'));
    CHECK(!ungetch('B'));
    CHECK(getch() == 'A');
    CHECK(ungetch(EOF));
    CHECK(getch() == EOF);
    CHECK(ungetch(255));
    CHECK(getch() == 255);
#elif TEST_EXERCISE == 9
    int i;

    CHECK(ungetch('A'));
    CHECK(ungetch(EOF));
    CHECK(ungetch(255));
    CHECK(getch() == 255);
    CHECK(getch() == EOF);
    CHECK(getch() == 'A');
    for (i = 0; i < BUFSIZE; ++i)
        CHECK(ungetch(i));
    CHECK(!ungetch('x'));
    for (i = BUFSIZE - 1; i >= 0; --i)
        CHECK(getch() == i);
#endif
    puts("Pushback checks passed.");
    return 0;
}
