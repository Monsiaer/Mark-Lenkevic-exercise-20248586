/* Compare the changed readlines function with the lecture's static alloc.
 * Both versions use the same file, line reader, memcpy, and storage size.
 * File creation, sorting, and console output are outside the timed loops.
 */
#define _CRT_SECURE_NO_WARNINGS
#define main exercise_demo_main
#include "../chapter_5/exercise_5_07.c"
#undef main
#include <time.h>

#define ROUNDS 5
#define REPEATS 2000
#define BENCH_LINES 400

static char allocbuf[STORAGESIZE];
static size_t alloc_used;
static volatile unsigned long checksum;

static char *lecture_alloc(size_t n)
{
    char *p;

    if (n > sizeof(allocbuf) - alloc_used)
        return NULL;
    p = allocbuf + alloc_used;
    alloc_used += n;
    return p;
}

static int original_readlines(FILE *input, char *lineptr[], int maxlines)
{
    char line[MAXLINE], *p;
    int nlines = 0, status;
    size_t length;

    alloc_used = 0;
    while ((status = get_line(input, line, sizeof(line), &length)) > 0) {
        if (nlines == maxlines || (p = lecture_alloc(length + 1)) == NULL)
            return -1;
        memcpy(p, line, length + 1);
        lineptr[nlines++] = p;
    }
    return status < 0 ? -1 : nlines;
}

static double measure(FILE *input, int original)
{
    char *lineptr[MAXLINES], storage[STORAGESIZE];
    clock_t start = clock();
    int i, n;

    for (i = 0; i < REPEATS; ++i) {
        rewind(input);
        n = original ? original_readlines(input, lineptr, MAXLINES)
                     : readlines(input, lineptr, storage, MAXLINES, sizeof(storage));
        if (n != BENCH_LINES)
            return -1.0;
        checksum += (unsigned long)n + (unsigned char)lineptr[n - 1][0];
    }
    return (double)(clock() - start) / CLOCKS_PER_SEC;
}

static double median(double values[])
{
    int i, j;
    double temp;

    for (i = 1; i < ROUNDS; ++i)
        for (j = i; j > 0 && values[j - 1] > values[j]; --j) {
            temp = values[j];
            values[j] = values[j - 1];
            values[j - 1] = temp;
        }
    return values[ROUNDS / 2];
}

int main(void)
{
    FILE *input = tmpfile();
    double old_times[ROUNDS], new_times[ROUNDS], old_median, new_median;
    char *old_lines[MAXLINES], *new_lines[MAXLINES], storage[STORAGESIZE];
    int i;

    if (input == NULL)
        return 1;
    for (i = 0; i < BENCH_LINES; ++i)
        if (fprintf(input, "line-%04d sample\n", i) < 0)
            return 1;
    rewind(input);
    if (original_readlines(input, old_lines, MAXLINES) != BENCH_LINES)
        return 1;
    rewind(input);
    if (readlines(input, new_lines, storage, MAXLINES, sizeof(storage)) != BENCH_LINES)
        return 1;
    for (i = 0; i < BENCH_LINES; ++i)
        if (strcmp(old_lines[i], new_lines[i]) != 0)
            return 1;
    for (i = 0; i < ROUNDS; ++i) {

        if (i % 2 == 0) {
            old_times[i] = measure(input, 1);
            new_times[i] = measure(input, 0);
        } else {
            new_times[i] = measure(input, 0);
            old_times[i] = measure(input, 1);
        }
        if (old_times[i] < 0 || new_times[i] < 0)
            return 1;
    }
    fclose(input);
    old_median = median(old_times);
    new_median = median(new_times);
    printf("%d rounds, %d reads per round, %d lines per read\n", ROUNDS, REPEATS, BENCH_LINES);
    printf("Static alloc median: %.6f s\n", old_median);
    printf("Main storage median: %.6f s\n", new_median);
    if (new_median > 0)
        printf("Ratio (alloc / main storage): %.3f\n", old_median / new_median);
    return 0;
}
