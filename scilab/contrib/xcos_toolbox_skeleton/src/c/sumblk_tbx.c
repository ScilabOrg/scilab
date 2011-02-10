#include "scicos_block.h"

void sumblk_tbx(int *flag, int *nevprt, double *t, double xd[],
                              double x[], int *nx, double z[], int *nz,
                              double tvec[], int *ntvec, double rpar[],
                              int *nrpar, int ipar[], int *nipar,
                              double *inptr[], int insz[], int *nin,
                              double *outptr[], int outsz[], int *nout)
{
    int i;      // output index counter
    int n;      // in/out array size
    int k;      // in index counter
    double *y;  // out array
    double *u;  // current in array

    if (flag != 1) {
        return;
    }

    y = (double *)outptr[0];
    n = outsz[0];               /* assert that insz[0]==insz[1] .. ==insz[*nin]== outsz[0] */

    for (i = 0; i < n; i++)
    {
        y[i] = 0.0;     // there is no shift

        for (k = 0; k < *nin; k++)
        {
            u = (double *)inptr[k];
            y[i] = y[i] + u[i];
        }
    }
}

