/*
 * Exercise 2-1
 *
 * Write a program to determine the ranges of char, short, int, and long
 * variables, both signed and unsigned, by printing appropriate values from
 * standard headers and by direct computation. Harder if you compute them:
 * determine the ranges of the various floating-point types.
 */
#include <stdio.h>
#include <limits.h>
#include <float.h>

int main(void)
{
    unsigned char uc = 0, next_uc;
    unsigned short us = 0, next_us;
    unsigned int ui = 0, next_ui;
    unsigned long ul = 0, next_ul;
    signed char sc;
    short ss;
    int si;
    long sl;

    printf("RANGES FROM <limits.h>\n");
    printf("char           : %d .. %d\n", CHAR_MIN, CHAR_MAX);
    printf("signed char    : %d .. %d\n", SCHAR_MIN, SCHAR_MAX);
    printf("unsigned char  : 0 .. %u\n", (unsigned int)UCHAR_MAX);
    printf("short          : %d .. %d\n", SHRT_MIN, SHRT_MAX);
    printf("unsigned short : 0 .. %u\n", (unsigned int)USHRT_MAX);
    printf("int            : %d .. %d\n", INT_MIN, INT_MAX);
    printf("unsigned int   : 0 .. %u\n", UINT_MAX);
    printf("long           : %ld .. %ld\n", LONG_MIN, LONG_MAX);
    printf("unsigned long  : 0 .. %lu\n", ULONG_MAX);

    while ((next_uc = (unsigned char)(uc * 2u + 1u)) > uc)
        uc = next_uc;
    while ((next_us = (unsigned short)(us * 2u + 1u)) > us)
        us = next_us;
    while ((next_ui = ui * 2u + 1u) > ui)
        ui = next_ui;
    while ((next_ul = ul * 2ul + 1ul) > ul)
        ul = next_ul;

    sc = (signed char)(uc / 2);
    ss = (short)(us / 2);
    si = (int)(ui / 2);
    sl = (long)(ul / 2);
    printf("\nCOMPUTED INTEGER RANGES (two's complement)\n");
    if ((char)-1 < 0)
        printf("char           : %d .. %d\n", -sc - 1, sc);
    else
        printf("char           : 0 .. %u\n", (unsigned int)uc);
    printf("signed char    : %d .. %d\n", -sc - 1, sc);
    printf("unsigned char  : 0 .. %u\n", (unsigned int)uc);
    printf("short          : %d .. %d\n", -ss - 1, ss);
    printf("unsigned short : 0 .. %u\n", (unsigned int)us);
    printf("int            : %d .. %d\n", -si - 1, si);
    printf("unsigned int   : 0 .. %u\n", ui);
    printf("long           : %ld .. %ld\n", -sl - 1, sl);
    printf("unsigned long  : 0 .. %lu\n", ul);

    printf("\nFLOATING-POINT LIMITS FROM <float.h>\n");
    printf("float      : %e .. %e\n", -FLT_MAX, FLT_MAX);
    printf("double     : %e .. %e\n", -DBL_MAX, DBL_MAX);
    printf("long double: %Le .. %Le\n", -LDBL_MAX, LDBL_MAX);
    printf("Smallest positive normalized values:\n");
    printf("float: %e; double: %e; long double: %Le\n",
           FLT_MIN, DBL_MIN, LDBL_MIN);
    printf("Smallest positive values (including subnormal values):\n");
    printf("float: %e; double: %e; long double: %Le\n",
           FLT_TRUE_MIN, DBL_TRUE_MIN, LDBL_TRUE_MIN);

    printf("\nTYPE SIZES (bytes, %d bits per byte)\n", CHAR_BIT);
    printf("char        : %zu\n", sizeof(char));
    printf("short       : %zu\n", sizeof(short));
    printf("int         : %zu\n", sizeof(int));
    printf("long        : %zu\n", sizeof(long));
    printf("long long   : %zu\n", sizeof(long long));
    printf("float       : %zu\n", sizeof(float));
    printf("double      : %zu\n", sizeof(double));
    printf("long double : %zu\n", sizeof(long double));
    return 0;
}
