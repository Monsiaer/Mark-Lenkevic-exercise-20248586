/*
 * Exercise 3-2
 *
 * Write a function escape(s, t) that converts newline and tab characters
 * into visible escape sequences as it copies t to s. Use a switch.
 * Write a function for the reverse conversion as well.
 */
#include <stdio.h>

void escape(char s[], const char t[]);
void unescape(char s[], const char t[]);

int main(void)
{
    char input[] = "Hello\tworld\nThis is a test.";
    char escaped[2 * sizeof(input)];
    char restored[sizeof(input)];

    escape(escaped, input);
    unescape(restored, escaped);

    printf("Escaped string: %s\n", escaped);
    printf("Unescaped string: %s\n", restored);

    return 0;
}

void escape(char s[], const char t[])
{
    int i, j = 0;

    for (i = 0; t[i] != '\0'; ++i) {
        switch (t[i]) {
        case '\n':
            s[j++] = '\\';
            s[j++] = 'n';
            break;
        case '\t':
            s[j++] = '\\';
            s[j++] = 't';
            break;
        case '\\':
            s[j++] = '\\';
            s[j++] = '\\';
            break;
        default:
            s[j++] = t[i];
            break;
        }
    }
    s[j] = '\0';
}

void unescape(char s[], const char t[])
{
    int i, j = 0;

    for (i = 0; t[i] != '\0'; ++i) {
        switch (t[i]) {
        case '\\':
            switch (t[i + 1]) {
            case 'n':
                s[j++] = '\n';
                ++i;
                break;
            case 't':
                s[j++] = '\t';
                ++i;
                break;
            case '\\':
                s[j++] = '\\';
                ++i;
                break;
            default:
                s[j++] = '\\';
                break;
            }
            break;
        default:
            s[j++] = t[i];
            break;
        }
    }
    s[j] = '\0';
}