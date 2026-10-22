#define main exercise_demo_main
#include "../chapter_2/exercise_2_03.c"
#undef main

int main(void)
{
    const char *valid[] = { "0", "0x0", "0XfF", "abcdef", "000a", "84ada" };
    const int expected[] = { 0, 0, 255, 11259375, 10, 543450 };
    const char *invalid[] = { "", "0x", "0X", "g", "1z", "-1", "+1", " ff", "ff " };
    char boundary[sizeof(int) * CHAR_BIT + 1];
    size_t i;

    for (i = 0; i < sizeof(valid) / sizeof(valid[0]); ++i)
        if (htoi(valid[i]) != expected[i])
            return 1;
    for (i = 0; i < sizeof(invalid) / sizeof(invalid[0]); ++i)
        if (htoi(invalid[i]) != -1)
            return 1;
    if (htoi(NULL) != -1)
        return 1;
    snprintf(boundary, sizeof(boundary), "%x", (unsigned int)INT_MAX);
    if (htoi(boundary) != INT_MAX)
        return 1;
    snprintf(boundary, sizeof(boundary), "%x", (unsigned int)INT_MAX + 1u);
    if (htoi(boundary) != -1)
        return 1;
    puts("18 htoi checks passed.");
    return 0;
}
