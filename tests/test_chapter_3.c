/* Check escaping, integer conversion boundaries, bases, padding,
 * and rightmost substring search from chapters 3 and 4.
 */
#define _CRT_SECURE_NO_WARNINGS
#define main demo_3_02
#include "../chapter_3/exercise_3_02.c"
#undef main
#define main demo_3_04
#define itoa itoa34
#define reverse reverse34
#include "../chapter_3/exercise_3_04.c"
#undef reverse
#undef itoa
#undef main
#define main demo_3_05
#define reverse reverse35
#include "../chapter_3/exercise_3_05.c"
#undef reverse
#undef main
#define main demo_3_06
#define itoa itoa36
#define reverse reverse36
#include "../chapter_3/exercise_3_06.c"
#undef reverse
#undef itoa
#undef main
#define main demo_4_01
#include "../chapter_4/exercise_4_01.c"
#undef main
#include <stdlib.h>

#define CHECK(x) do { if (!(x)) { fprintf(stderr, "Failed at line %d\n", __LINE__); return 1; } } while (0)

int main(void)
{
    char input[256], encoded[512], decoded[256], actual[128], expected[128];
    int values[] = { INT_MIN, INT_MAX, 0, 1, -1, 12345, -98765 };
    size_t i;
    int base, width, n;

    for (n = 1; n <= 255; ++n)
        input[n - 1] = (char)n;
    input[255] = '\0';
    escape(encoded, input);
    unescape(decoded, encoded);
    CHECK(memcmp(input, decoded, sizeof(input)) == 0);
    escape(encoded, "");
    CHECK(encoded[0] == '\0');
    unescape(decoded, "x\\q\\");
    CHECK(strcmp(decoded, "x\\q\\") == 0);
    for (i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        snprintf(expected, sizeof(expected), "%d", values[i]);
        itoa34(values[i], actual);
        CHECK(strcmp(actual, expected) == 0);
        for (base = 2; base <= 36; ++base) {
            itob(values[i], actual, base);
            CHECK(strtol(actual, NULL, base) == values[i]);
        }
        for (width = 0; width <= 64; ++width) {
            snprintf(expected, sizeof(expected), "%*d", width, values[i]);
            itoa36(values[i], actual, width);
            CHECK(strcmp(actual, expected) == 0);
        }
    }
    for (n = -10000; n <= 10000; n += 17) {
        snprintf(expected, sizeof(expected), "%d", n);
        itoa34(n, actual);
        CHECK(strcmp(actual, expected) == 0);
    }
    itob(123, actual, 1);
    CHECK(actual[0] == '\0');
    itob(123, actual, 37);
    CHECK(actual[0] == '\0');
    CHECK(strrindex("abcdedfabcde", "abc") == 7);
    CHECK(strrindex("aaaaa", "aaa") == 2);
    CHECK(strrindex("abc", "z") == -1);
    CHECK(strrindex("", "a") == -1);
    CHECK(strrindex("abc", "") == -1);
    CHECK(strrindex("abc", "abcd") == -1);
    CHECK(strrindex("abc", "c") == 2);
    puts("Chapter 3 and strrindex function checks passed.");
    return 0;
}
