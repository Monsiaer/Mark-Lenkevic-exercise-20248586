/*
 * Exercise 1-5
 *
 * Modify the temperature conversion program to print the table in reverse
 * order, from 300 degrees to 0.
 */
#include <stdio.h>

int main(void)
{
    int fahr;

    printf("Fahrenheit  Celsius\n");
    printf("----------  -------\n");
    for (fahr = 300; fahr >= 0; fahr -= 20)
        printf("%10d  %7.1f\n", fahr, (5.0 / 9.0) * (fahr - 32));
    return 0;
}
