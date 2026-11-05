/* Exercise 5-8. Validate both calendar conversion functions.
 * Valid years are positive Gregorian years. Distinct negative values
 * report invalid arguments. month_day clears available outputs on error.
 */
#include <stdio.h>

enum date_status {
    DATE_OK = 0,
    BAD_YEAR = -1,
    BAD_MONTH = -2,
    BAD_DAY = -3,
    BAD_YEARDAY = -4,
    BAD_POINTER = -5
};

static const int daytab[2][13] = {
    { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
    { 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
};

int is_leap(int year);
int day_of_year(int year, int month, int day);
int month_day(int year, int yearday, int *month, int *day);

int main(void)
{
    int month, day, status;

    printf("day_of_year(1988, 2, 29) = %d\n", day_of_year(1988, 2, 29));
    printf("day_of_year(2020, 2, 31) = %d (invalid day)\n",
           day_of_year(2020, 2, 31));
    status = month_day(1988, 60, &month, &day);
    printf("month_day(1988, 60): status=%d, month=%d, day=%d\n",
           status, month, day);
    status = month_day(1999, 366, &month, &day);
    printf("month_day(1999, 366): status=%d, month=%d, day=%d\n",
           status, month, day);
    return 0;
}

int is_leap(int year)
{
    return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

int day_of_year(int year, int month, int day)
{
    int i, leap;

    if (year < 1)
        return BAD_YEAR;
    if (month < 1 || month > 12)
        return BAD_MONTH;
    leap = is_leap(year);
    if (day < 1 || day > daytab[leap][month])
        return BAD_DAY;
    for (i = 1; i < month; ++i)
        day += daytab[leap][i];
    return day;
}

int month_day(int year, int yearday, int *month, int *day)
{
    int i, leap;

    if (month != NULL)
        *month = 0;
    if (day != NULL)
        *day = 0;
    if (month == NULL || day == NULL || month == day)
        return BAD_POINTER;
    if (year < 1)
        return BAD_YEAR;
    leap = is_leap(year);
    if (yearday < 1 || yearday > 365 + leap)
        return BAD_YEARDAY;
    for (i = 1; yearday > daytab[leap][i]; ++i)
        yearday -= daytab[leap][i];
    *month = i;
    *day = yearday;
    return DATE_OK;
}
