/* Chapter 4: demonstrate header guards, macros, conditional compilation,
 * and undef. Define EXAMPLE_DEBUG or EXAMPLE_NATION when compiling.
 */
#include <stdio.h>
#include "preprocessor.h"
#include "preprocessor.h"

#ifndef EXAMPLE_NATION
#define EXAMPLE_NATION 2
#endif

#define TEMPORARY_VALUE 123
#undef TEMPORARY_VALUE

double average(int x, int y)
{
#ifdef EXAMPLE_DEBUG
    printf("debug: x=%d, y=%d\n", x, y);
#endif
    return ((double)x + (double)y) / 2.0;
}

int main(void)
{
    printf("maximum: %d\naverage: %.1f\n", MAXIMUM(3 + 2, 4 + 3), average(3, 8));
#if EXAMPLE_NATION == 1
    puts("nation: 1");
#elif EXAMPLE_NATION == 2
    puts("Hello world");
#else
    puts("other language");
#endif
#ifndef TEMPORARY_VALUE
    puts("temporary macro is undefined");
#endif
    return 0;
}
