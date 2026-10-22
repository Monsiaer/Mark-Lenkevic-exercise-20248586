/*
 * Exercise 1-15
 *
 * Rewrite the temperature conversion program of Section 1.2 to use a function
 * for conversion.
 */
#include <stdio.h>

double to_celsius(double fahr);

int main(void)
{
    int fahr;

    printf("Fahrenheit  Celsius\n");
    printf("----------  -------\n");
    for (fahr = 0; fahr <= 300; fahr += 20)
        printf("%10d  %7.1f\n", fahr, to_celsius(fahr));
    return 0;
}

double to_celsius(double fahr)
{
    return (5.0 / 9.0) * (fahr - 32);
}
