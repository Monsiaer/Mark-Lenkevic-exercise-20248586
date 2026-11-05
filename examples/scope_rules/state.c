/* Define shared state once; file-static and function-static names remain local. */
#include "scope.h"

int shared_count = 0;
static int calls;

void increment_count(void)
{
    ++shared_count;
    ++calls;
}

int persistent_count(void)
{
    static int visits;

    ++visits;
    return calls + visits;
}
