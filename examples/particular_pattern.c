/*
 * Particular Pattern Example
 *
 * Print each input line that contains the pattern "ould".
 * Based on the instructor's Chapter 4.1 example.
 */
#include <stdio.h>

#define MAXLINE 1000

int getline(char line[], int max);
int strindex(const char source[], const char searchfor[]);

char pattern[] = "ould";

int main(void)
{
    char line[MAXLINE];
    int found = 0;

    for (; getline(line, MAXLINE) > 0;) {
        if (strindex(line, pattern) >= 0) {
            printf("%s", line);
            ++found;
        }
    }
    return found;
}

int getline(char s[], int lim)
{
    int c = 0, i;

    for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i)
        s[i] = (char)c;

    if (c == '\n') {
        s[i] = (char)c;
        ++i;
    }
    s[i] = '\0';
    return i;
}

int strindex(const char s[], const char t[])
{
    int i, j, k;

    for (i = 0; s[i] != '\0'; ++i) {
        for (j = i, k = 0; t[k] != '\0' && s[j] == t[k]; ++j, ++k)
            ;
        if (k > 0 && t[k] == '\0')
            return i;
    }
    return -1;
}
