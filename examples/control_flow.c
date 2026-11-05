/* Chapter 3 examples: safe blocks, nested if, binary search, switch,
 * do-while conversion (exercise 3-4), break, continue, and goto.
 */
#include <stdio.h>
#include <string.h>

int binary_search(int x, const int v[], int n);
int trim(char s[]);
int common_element(const int a[], int na, const int b[], int nb);

int main(void)
{
    char stars[10], text[] = "control flow \t\n";
    const char sample[] = "a0 12\t9\n";
    int values[] = { 1, 2, 5, 6, 7, 9, 11, 13 };
    int other[] = { 4, 6, 8 }, mixed[] = { -3, 2, -1, 5 };
    int digits[10] = { 0 }, whitespace = 0, rest = 0;
    int i, sum = 0, a = 3, b = 8, n = 1, z = 0;

    for (i = 0; i < (int)sizeof(stars) - 1; ++i)
        stars[i] = '*';
    stars[i] = '\0';
    printf("block: %s\n", stars);
    if (n > 0) {
        if (a > b)
            z = a;
        else
            z = b;
    }
    printf("nested if: %d\n", z);
    printf("binary search 9: %d\n", binary_search(9, values, 8));
    for (i = 0; sample[i] != '\0'; ++i) {
        switch (sample[i]) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            ++digits[sample[i] - '0'];
            break;
        case ' ': case '\t': case '\n':
            ++whitespace;
            break;
        default:
            ++rest;
            break;
        }
    }
    printf("switch: digit 1=%d, whitespace=%d, other=%d\n",
           digits[1], whitespace, rest);
    trim(text);
    printf("break: |%s|\n", text);
    for (i = 0; i < 4; ++i) {
        if (mixed[i] < 0)
            continue;
        sum += mixed[i];
    }
    printf("continue: %d\n", sum);
    printf("goto: common=%d\n", common_element(values, 8, other, 3));
    return 0;
}

int binary_search(int x, const int v[], int n)
{
    int low = 0, high = n - 1, mid;

    while (low <= high) {
        mid = low + (high - low) / 2;
        if (x < v[mid])
            high = mid - 1;
        else if (x > v[mid])
            low = mid + 1;
        else
            return mid;
    }
    return -1;
}

int trim(char s[])
{
    size_t n = strlen(s);

    while (n > 0) {
        if (s[n - 1] != ' ' && s[n - 1] != '\t' && s[n - 1] != '\n')
            break;
        --n;
    }
    s[n] = '\0';
    return (int)n;
}

int common_element(const int a[], int na, const int b[], int nb)
{
    int i, j;

    for (i = 0; i < na; ++i)
        for (j = 0; j < nb; ++j)
            if (a[i] == b[j])
                goto found;
    return -1;
found:
    return a[i];
}
