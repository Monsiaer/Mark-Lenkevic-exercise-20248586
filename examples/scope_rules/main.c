/* Chapter 4.4: demonstrate extern, file scope, block scope, and static lifetime. */
#include <stdio.h>
#include "scope.h"

int main(void)
{
    int local = 10, first, second;

    increment_count();
    increment_count();
    printf("extern shared count: %d\n", shared_count);
    {
        int block_local = 20;

        printf("block: %d, enclosing: %d\n", block_local, local);
    }
    first = persistent_count();
    second = persistent_count();
    printf("static lifetime: %d %d\n", first, second);
    return 0;
}
