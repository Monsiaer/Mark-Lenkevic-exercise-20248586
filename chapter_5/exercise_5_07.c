/* Exercise 5-7. Store input lines in an array supplied by main.
 * readlines uses a position in linestor instead of calling alloc.
 * Sort the pointers, not the stored strings. See the separate benchmark
 * for a measured comparison with the lecture's static alloc version.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINES 5000
#define MAXLINE 1000
#define STORAGESIZE 10000

int get_line(FILE *input, char line[], size_t capacity, size_t *length);
int readlines(FILE *input, char *lineptr[], char linestor[],
              int maxlines, size_t capacity);
int compare_lines(const void *left, const void *right);
void writelines(char *lineptr[], int nlines);

int main(void)
{
    char *lineptr[MAXLINES];
    char linestor[STORAGESIZE];
    int nlines;

    nlines = readlines(stdin, lineptr, linestor, MAXLINES, sizeof(linestor));
    if (nlines < 0) {
        fprintf(stderr, "Input exceeds the line/storage limits, or a read failed.\n");
        return 1;
    }
    qsort(lineptr, (size_t)nlines, sizeof(lineptr[0]), compare_lines);
    writelines(lineptr, nlines);
    return 0;
}

int readlines(FILE *input, char *lineptr[], char linestor[],
              int maxlines, size_t capacity)
{
    char line[MAXLINE];
    size_t length, used = 0;
    int nlines = 0, status;

    while ((status = get_line(input, line, sizeof(line), &length)) > 0) {
        if (nlines == maxlines || length + 1 > capacity - used)
            return -1;
        lineptr[nlines++] = linestor + used;
        memcpy(linestor + used, line, length + 1);
        used += length + 1;
    }
    return status < 0 ? -1 : nlines;
}

int get_line(FILE *input, char line[], size_t capacity, size_t *length)
{
    int c;
    size_t n = 0;

    while ((c = getc(input)) != EOF && c != '\n') {
        if (n + 1 == capacity)
            return -1;
        line[n++] = (char)c;
    }
    if (ferror(input))
        return -1;
    line[n] = '\0';
    *length = n;
    return c == '\n' || n != 0;
}

int compare_lines(const void *left, const void *right)
{
    const char *const *a = left;
    const char *const *b = right;

    return strcmp(*a, *b);
}

void writelines(char *lineptr[], int nlines)
{
    while (nlines-- > 0)
        printf("%s\n", *lineptr++);
}
