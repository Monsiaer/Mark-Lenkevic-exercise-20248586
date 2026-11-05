/* Check exercises 5-5 and 5-6 against standard string functions and limits. */
#define _CRT_SECURE_NO_WARNINGS
#define main demo_5_05
#include "../chapter_5/exercise_5_05.c"
#undef main
#define main demo_5_06
#include "../chapter_5/exercise_5_06.c"
#undef main
#include <string.h>
#include <stdlib.h>

#define CHECK(x) do { if (!(x)) { fprintf(stderr, "Failed at line %d\n", __LINE__); return 1; } } while (0)

int main(void)
{
    const char *strings[] = { "", "a", "abc", "abcdef", "abC", "\xff", "\x80" };
    char a[64], b[64], token[16], text[sizeof(int) * CHAR_BIT + 2];
    int values[] = { 0, 1, -1, INT_MIN, INT_MAX, -12345, 98765 };
    size_t i, j, n;
    int x, y;
    FILE *input;

    for (i = 0; i < sizeof(strings) / sizeof(strings[0]); ++i) {
        for (n = 0; n < 12; ++n) {
            memset(a, '!', sizeof(a));
            memset(b, '!', sizeof(b));
            CHECK(mystrncpy(a, strings[i], n) == a);
            strncpy(b, strings[i], n);
            CHECK(memcmp(a, b, sizeof(a)) == 0);
            strcpy(a, "prefix");
            strcpy(b, "prefix");
            CHECK(mystrncat(a, strings[i], n) == a);
            strncat(b, strings[i], n);
            CHECK(strcmp(a, b) == 0);
            for (j = 0; j < sizeof(strings) / sizeof(strings[0]); ++j) {
                x = mystrncmp(strings[i], strings[j], n);
                y = strncmp(strings[i], strings[j], n);
                CHECK((x > 0) - (x < 0) == (y > 0) - (y < 0));
            }
        }
    }
    for (i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        pointer_itoa(values[i], text);
        CHECK(strtol(text, NULL, 10) == values[i]);
        CHECK(pointer_atoi(text) == values[i]);
        strcpy(a, text);
        pointer_reverse(text);
        pointer_reverse(text);
        CHECK(strcmp(a, text) == 0);
    }
    CHECK(pointer_atoi(" \t+42tail") == 42);
    CHECK(pointer_atoi("bad") == 0);
    CHECK(pointer_atoi("999999999999999999999") == INT_MAX);
    CHECK(pointer_atoi("-999999999999999999999") == INT_MIN);
    a[0] = '\0';
    pointer_reverse(a);
    CHECK(a[0] == '\0');
    CHECK(pointer_strindex("banana", "ana") == 1);
    CHECK(pointer_strindex("abc", "") == 0);
    CHECK(pointer_strindex("", "x") == -1);
    CHECK(pointer_strindex("ab", "abc") == -1);
    CHECK(pointer_strindex("abc", "c") == 2);
    input = tmpfile();
    CHECK(input != NULL);
    CHECK(fputs("abc\nlast", input) >= 0);
    rewind(input);
    CHECK(pointer_getline(a, 1, input) == 0 && a[0] == '\0');
    CHECK(pointer_getline(a, 3, input) == 2 && strcmp(a, "ab") == 0);
    CHECK(pointer_getline(a, sizeof(a), input) == 2 && strcmp(a, "c\n") == 0);
    CHECK(pointer_getline(a, sizeof(a), input) == 4 && strcmp(a, "last") == 0);
    CHECK(pointer_getline(a, sizeof(a), input) == 0);
    fclose(input);
    input = tmpfile();
    CHECK(input != NULL);
    CHECK(fputs("12.5 .25 3 + - * /\n", input) >= 0);
    rewind(input);
    CHECK(pointer_getop(token, sizeof(token), input) == NUMBER && strcmp(token, "12.5") == 0);
    CHECK(pointer_getop(token, sizeof(token), input) == NUMBER && strcmp(token, ".25") == 0);
    CHECK(pointer_getop(token, sizeof(token), input) == NUMBER && strcmp(token, "3") == 0);
    CHECK(pointer_getop(token, sizeof(token), input) == '+');
    CHECK(pointer_getop(token, sizeof(token), input) == '-');
    CHECK(pointer_getop(token, sizeof(token), input) == '*');
    CHECK(pointer_getop(token, sizeof(token), input) == '/');
    CHECK(pointer_getop(token, sizeof(token), input) == EOF);
    fclose(input);
    puts("Exercises 5-5 and 5-6 checks passed.");
    return 0;
}
