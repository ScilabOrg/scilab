#include <stdio.h>
#include "machine.h"

void C2F(dscal)(int *n, double *da, double *dx, int *incx)
{
    int i = 0;

    fprintf(stderr, "n=%d, incx=%d\n", *n, *incx);
    if (*n < 0 || * incx < 0)
    {
        return;
    }
    
    for (i = 0 ; i < *n ; i += *incx)
    {
        dx[i] = dx[i] * (*da);
    }
}