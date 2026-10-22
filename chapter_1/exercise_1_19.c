/*
 * Exercise 1-19
 *
 * Write a function reverse(s) that reverses the character string s. Use it to
 * write a program that reverses its input a line at a time.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void reverse(char s[]);

int main(void)
{
    char *line = NULL, *larger;
    size_t length = 0, capacity = 0, next;
    int c;

    while ((c = getchar()) != EOF) {
        if (length + 1 >= capacity) {
            if (capacity > SIZE_MAX / 2) {
                free(line);
                fprintf(stderr, "Line is too long.\n");
                return 1;
            }
            next = capacity == 0 ? 128 : capacity * 2;
            larger = realloc(line, next);
            if (larger == NULL) {
                free(line);
                fprintf(stderr, "Not enough memory.\n");
                return 1;
            }
            line = larger;
            capacity = next;
        }
        if (c == '\n') {
            line[length] = '\0';
            reverse(line);
            printf("%s\n", line);
            length = 0;
        } else {
            line[length++] = (char)c;
        }
    }
    if (length > 0) {
        line[length] = '\0';
        reverse(line);
        printf("%s", line);
    }
    free(line);
    return 0;
}

void reverse(char s[])
{
    size_t left = 0, right = strlen(s);
    char temp;

    while (left < right) {
        --right;
        if (left >= right)
            break;
        temp = s[left];
        s[left] = s[right];
        s[right] = temp;
        ++left;
    }
}
