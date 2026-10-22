/*
 * Exercise 1-4
 *
 * Write a program to print the corresponding Celsius-to-Fahrenheit table.
 */
#include <stdio.h>

int main(void)
{
    int celsius;
    double fahr;

    printf("Celsius  Fahrenheit\n");
    printf("-------  ----------\n");
    for (celsius = 0; celsius <= 300; celsius += 20) {
        fahr = (9.0 / 5.0) * celsius + 32;
        printf("%7d  %10.1f\n", celsius, fahr);
    }
    return 0;
}
