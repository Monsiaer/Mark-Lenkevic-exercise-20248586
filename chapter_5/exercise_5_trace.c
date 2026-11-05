/* Slide 5. Trace the pointer assignments and print m, *mp, n, and *np.
 * The slide uses np = &np; use np = &n for an int pointer to n.
 */
#include <stdio.h>

int main(void)
{
    int m = 10, n = 5;
    int *mp, *np;

    mp = &m;
    np = &n;
    *mp = *mp + *np;
    *np = *mp - *np;
    printf("%d %d\n%d %d\n", m, *mp, n, *np);
    return 0;
}
