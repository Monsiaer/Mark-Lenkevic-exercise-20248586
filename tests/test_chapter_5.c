/* Function-level checks for the new lecture exercises. */
#define _CRT_SECURE_NO_WARNINGS
#define main demo_5_03
#define read_line read_line_03
#include "../chapter_5/exercise_5_03.c"
#undef read_line
#undef main
#define main demo_5_04
#define read_line read_line_04
#include "../chapter_5/exercise_5_04.c"
#undef read_line
#undef main
#define main demo_5_07
#include "../chapter_5/exercise_5_07.c"
#undef main
#define main demo_5_08
#include "../chapter_5/exercise_5_08.c"
#undef main

#define CHECK(condition) do { \
    if (!(condition)) { \
        fprintf(stderr, "Check failed at line %d: %s\n", __LINE__, #condition); \
        return 1; \
    } \
} while (0)

int main(void)
{
    char text[16] = "hello", store[32], *lines[8];
    char exact[7], protected_store[4] = { '!', '!', '!', '!' };
    int year, ordinal, month, day, status;
    unsigned long dates = 0;
    FILE *input;

    mystrcat(text, " world");
    CHECK(strcmp(text, "hello world") == 0);
    mystrcat(text, "");
    CHECK(strcmp(text, "hello world") == 0);
    text[0] = '\0';
    mystrcat(text, "abc");
    CHECK(strcmp(text, "abc") == 0);
    memcpy(exact, "abc\0!!!", sizeof(exact));
    mystrcat(exact, "def");
    CHECK(memcmp(exact, "abcdef\0", sizeof(exact)) == 0);
    CHECK(strend("hello world", "world") == 1);
    CHECK(strend("hello world", "hello") == 0);
    CHECK(strend("same", "same") == 1);
    CHECK(strend("abc", "abcd") == 0);
    CHECK(strend("", "") == 1);
    CHECK(strend("abc", "") == 1);
    CHECK(strend("", "a") == 0);
    CHECK(strend("abcabc", "abc") == 1);
    CHECK(strend("abc", "ABC") == 0);

    input = tmpfile();
    CHECK(input != NULL);
    CHECK(fputs("zebra\n\napple\nzebra", input) >= 0);
    rewind(input);
    CHECK(readlines(input, lines, store, 8, sizeof(store)) == 4);
    CHECK(strcmp(lines[3], "zebra") == 0);
    qsort(lines, 4, sizeof(lines[0]), compare_lines);
    CHECK(strcmp(lines[0], "") == 0);
    CHECK(strcmp(lines[1], "apple") == 0);
    CHECK(strcmp(lines[2], "zebra") == 0);
    CHECK(strcmp(lines[3], "zebra") == 0);
    rewind(input);
    CHECK(readlines(input, lines, store, 3, sizeof(store)) == -1);
    rewind(input);
    CHECK(readlines(input, lines, store, 8, 0) == -1);
    fclose(input);

    input = tmpfile();
    CHECK(input != NULL);
    CHECK(fputs("abc", input) >= 0);
    rewind(input);
    CHECK(readlines(input, lines, protected_store, 8, 3) == -1);
    CHECK(memcmp(protected_store, "!!!!", 4) == 0);
    rewind(input);
    CHECK(readlines(input, lines, protected_store, 8, 4) == 1);
    CHECK(strcmp(lines[0], "abc") == 0);
    fclose(input);

    CHECK(day_of_year(1988, 2, 29) == 60);
    CHECK(day_of_year(2020, 2, 31) == BAD_DAY);
    CHECK(day_of_year(1900, 2, 29) == BAD_DAY);
    CHECK(day_of_year(2000, 2, 29) == 60);
    CHECK(day_of_year(2024, 0, 1) == BAD_MONTH);
    CHECK(day_of_year(2024, 13, 1) == BAD_MONTH);
    CHECK(day_of_year(2024, 1, 0) == BAD_DAY);
    CHECK(day_of_year(0, 1, 1) == BAD_YEAR);
    CHECK(month_day(1999, 366, &month, &day) == BAD_YEARDAY);
    CHECK(month == 0 && day == 0);
    CHECK(month_day(2000, 0, &month, &day) == BAD_YEARDAY);
    CHECK(month_day(2000, 367, &month, &day) == BAD_YEARDAY);
    CHECK(month_day(-1, 1, &month, &day) == BAD_YEAR);
    CHECK(month_day(2000, 1, NULL, &day) == BAD_POINTER);
    CHECK(day == 0);
    CHECK(month_day(2000, 1, &month, NULL) == BAD_POINTER);
    CHECK(month_day(2000, 1, &month, &month) == BAD_POINTER);

    for (year = 1600; year < 2000; ++year) {
        for (ordinal = 1; ordinal <= 365 + is_leap(year); ++ordinal) {
            status = month_day(year, ordinal, &month, &day);
            CHECK(status == DATE_OK);
            CHECK(month >= 1 && month <= 12);
            CHECK(day_of_year(year, month, day) == ordinal);
            ++dates;
        }
    }
    printf("Chapter 5 function checks passed; %lu calendar dates checked.\n", dates);
    return 0;
}
