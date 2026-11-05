/*
 * Exercise 4-1
 *
 * Write strrindex(s, t), which returns the position of the rightmost
 * occurrence of t in s, or -1 if there is none.
 */
#include <stdio.h>

#define MAXLINE 1000

int getline(char line[], int max);
int strrindex(const char source[], const char searchfor[]);

char pattern[] = "ould";

int main(void)
{
    char line[MAXLINE];
    int found = 0;

    for (; getline(line, MAXLINE) > 0;) {
        if (strrindex(line, pattern) >= 0) {
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

int strrindex(const char s[], const char t[])
{
    int i, j, k, rightmost = -1;

    for (i = 0; s[i] != '\0'; ++i) {
        for (j = i, k = 0; t[k] != '\0' && s[j] == t[k]; ++j, ++k)
            ;
        if (k > 0 && t[k] == '\0')
            rightmost = i;
    }
    return rightmost;
}
