/*  Scicos
*
*  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
* See the file ./license.txt
*/
/*--------------------------------------------------------------------------*/ 
#include <stdio.h> /* printf */
#include <string.h>
#ifdef UNIX
#include <unistd.h>
#include <sys/socket.h>
#endif
#include "scicos_math.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/ 
/* Table of constant values */
static double c_b4 = 1.;
static double c_b5 = 0.;
/*--------------------------------------------------------------------------*/ 
static int dmmulDgemm(char *transa, char *transb, int *m, int *n, int *k, double *alpha, 
                      double *a, int *lda, double *b, int *ldb,double *beta, double *c, 
                      int *ldc);
static long int dmmulLsame(char *ca, char *cb);
static int dmmulXerbla(char *srname, int *info);
/*--------------------------------------------------------------------------*/ 
SCICOS_BLOCKS_IMPEXP int dmmul(double *a, int *na, double *b, int *nb, double *c__, 
                               int *nc, int *l, int *m, int *n)
{
    int a_dim1 = 0, a_offset = 0, b_dim1 = 0, b_offset = 0, c_dim1 = 0, c_offset = 0;

    /*     PURPOSE */
    /*        computes the matrix product C = A * B */
    /*            C   =   A   *   B */
    /*          (l,n)   (l,m) * (m,n) */

    /*     PARAMETERS */
    /*        input */
    /*        ----- */
    /*        A : (double) array (l, m) with leading dim na */

    /*        B : (double) array (m, n) with leading dim nb */

    /*        na, nb, nc, l, m, n : integers */

    /*        output */
    /*        ------ */
    /*        C : (double) array (l, n) with leading dim nc */

    /*     NOTE */
    /*        (original version substituted by a call to the blas dgemm) */
    /* Parameter adjustments */
    a_dim1 = *na;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    c_dim1 = *nc;
    c_offset = 1 + c_dim1 * 1;
    c__ -= c_offset;
    b_dim1 = *nb;
    b_offset = 1 + b_dim1 * 1;
    b -= b_offset;

    /* Function Body */
    dmmulDgemm("n", "n", l, n, m, &c_b4, &a[a_offset], na, &b[b_offset], nb, &
        c_b5, &c__[c_offset], nc);
    return 0;
} /* dmmul */
/*--------------------------------------------------------------------------*/ 
static int dmmulDgemm(char *transa, char *transb, int *m, int *n, int *k, double *alpha, 
                      double *a, int *lda, double *b, int *ldb,double *beta, double *c, 
                      int *ldc)
{
    /* System generated locals */
    int i__1, i__2, i__3;
    /* Local variables */
    static int info = 0;
    static long int nota = 0, notb = 0;
    static double temp = 0.;
    static int i = 0, j = 0, l = 0, ncola = 0;
    static int nrowa = 0, nrowb = 0;  


    /*     .. Scalar Arguments .. */
    /*     .. Array Arguments .. */
    /*     .. */

    /*  Purpose */
    /*  ======= */

    /*  DGEMM  performs one of the matrix-matrix operations */

    /*     C := alpha*op( A )*op( B ) + beta*C, */

    /*  where  op( X ) is one of */

    /*     op( X ) = X   or   op( X ) = X', */

    /*  alpha and beta are scalars, and A, B and C are matrices, with op( A ) */
    /*  an m by k matrix,  op( B )  a  k by n matrix and  C an m by n matrix. */

    /*  Parameters */
    /*  ========== */

    /*  TRANSA - CHARACTER*1. */
    /*           On entry, TRANSA specifies the form of op( A ) to be used in */
    /*           the matrix multiplication as follows: */

    /*              TRANSA = 'N' or 'n',  op( A ) = A. */

    /*              TRANSA = 'T' or 't',  op( A ) = A'. */

    /*              TRANSA = 'C' or 'c',  op( A ) = A'. */

    /*           Unchanged on exit. */

    /*  TRANSB - CHARACTER*1. */
    /*           On entry, TRANSB specifies the form of op( B ) to be used in */
    /*           the matrix multiplication as follows: */

    /*              TRANSB = 'N' or 'n',  op( B ) = B. */

    /*              TRANSB = 'T' or 't',  op( B ) = B'. */

    /*              TRANSB = 'C' or 'c',  op( B ) = B'. */

    /*           Unchanged on exit. */

    /*  M      - INTEGER. */
    /*           On entry,  M  specifies  the number  of rows  of the  matrix */
    /*           op( A )  and of the  matrix  C.  M  must  be at least  zero. */
    /*           Unchanged on exit. */

    /*  N      - INTEGER. */
    /*           On entry,  N  specifies the number  of columns of the matrix */
    /*           op( B ) and the number of columns of the matrix C. N must be */
    /*           at least zero. */
    /*           Unchanged on exit. */

    /*  K      - INTEGER. */
    /*           On entry,  K  specifies  the number of columns of the matrix */
    /*           op( A ) and the number of rows of the matrix op( B ). K must */
    /*           be at least  zero. */
    /*           Unchanged on exit. */

    /*  ALPHA  - DOUBLE PRECISION. */
    /*           On entry, ALPHA specifies the scalar alpha. */
    /*           Unchanged on exit. */

    /*  A      - DOUBLE PRECISION array of DIMENSION ( LDA, ka ), where ka is */
    /*           k  when  TRANSA = 'N' or 'n',  and is  m  otherwise. */
    /*           Before entry with  TRANSA = 'N' or 'n',  the leading  m by k */
    /*           part of the array  A  must contain the matrix  A,  otherwise */
    /*           the leading  k by m  part of the array  A  must contain  the */
    /*           matrix A. */
    /*           Unchanged on exit. */

    /*  LDA    - INTEGER. */
    /*           On entry, LDA specifies the first dimension of A as declared */
    /*           in the calling (sub) program. When  TRANSA = 'N' or 'n' then */
    /*           LDA must be at least  max( 1, m ), otherwise  LDA must be at */
    /*           least  max( 1, k ). */
    /*           Unchanged on exit. */

    /*  B      - DOUBLE PRECISION array of DIMENSION ( LDB, kb ), where kb is */
    /*           n  when  TRANSB = 'N' or 'n',  and is  k  otherwise. */
    /*           Before entry with  TRANSB = 'N' or 'n',  the leading  k by n */
    /*           part of the array  B  must contain the matrix  B,  otherwise */
    /*           the leading  n by k  part of the array  B  must contain  the */
    /*           matrix B. */
    /*           Unchanged on exit. */

    /*  LDB    - INTEGER. */
    /*           On entry, LDB specifies the first dimension of B as declared */
    /*           in the calling (sub) program. When  TRANSB = 'N' or 'n' then */
    /*           LDB must be at least  max( 1, k ), otherwise  LDB must be at */
    /*           least  max( 1, n ). */
    /*           Unchanged on exit. */

    /*  BETA   - DOUBLE PRECISION. */
    /*           On entry,  BETA  specifies the scalar  beta.  When  BETA  is */
    /*           supplied as zero then C need not be set on input. */
    /*           Unchanged on exit. */

    /*  C      - DOUBLE PRECISION array of DIMENSION ( LDC, n ). */
    /*           Before entry, the leading  m by n  part of the array  C must */
    /*           contain the matrix  C,  except when  beta  is zero, in which */
    /*           case C need not be set on entry. */
    /*           On exit, the array  C  is overwritten by the  m by n  matrix */
    /*           ( alpha*op( A )*op( B ) + beta*C ). */

    /*  LDC    - INTEGER. */
    /*           On entry, LDC specifies the first dimension of C as declared */
    /*           in  the  calling  (sub)  program.   LDC  must  be  at  least */
    /*           max( 1, m ). */
    /*           Unchanged on exit. */


    /*  Level 3 Blas routine. */

    /*  -- Written on 8-February-1989. */
    /*     Jack Dongarra, Argonne National Laboratory. */
    /*     Iain Duff, AERE Harwell. */
    /*     Jeremy Du Croz, Numerical Algorithms Group Ltd. */
    /*     Sven Hammarling, Numerical Algorithms Group Ltd. */


    /*     .. External Functions .. */
    /*     .. External Subroutines .. */
    /*     .. Intrinsic Functions .. */
    /*     .. Local Scalars .. */
    /*     .. Parameters .. */
    /*     .. */
    /*     .. Executable Statements .. */

    /*     Set  NOTA  and  NOTB  as  true if  A  and  B  respectively are not */
    /*     transposed and set  NROWA, NCOLA and  NROWB  as the number of rows */
    /*     and  columns of  A  and the  number of  rows  of  B  respectively. */

#define A(I,J) a[(I)-1 + ((J)-1)* ( *lda)]
#define B(I,J) b[(I)-1 + ((J)-1)* ( *ldb)]
#define C(I,J) c[(I)-1 + ((J)-1)* ( *ldc)]

    nota = dmmulLsame(transa, "N");
    notb = dmmulLsame(transb, "N");
    if (nota) {
        nrowa = *m;
        ncola = *k;
    } else {
        nrowa = *k;
        ncola = *m;
    }
    if (notb) {
        nrowb = *k;
    } else {
        nrowb = *n;
    }

    /*     Test the input parameters. */

    info = 0;
    if (! nota && ! dmmulLsame(transa, "C") && ! dmmulLsame(transa, "T")) {
        info = 1;
    } else if (! notb && ! dmmulLsame(transb, "C") && ! dmmulLsame(transb,"T")) {
        info = 2;
    } else if (*m < 0) {
        info = 3;
    } else if (*n < 0) {
        info = 4;
    } else if (*k < 0) {
        info = 5;
    } else if (*lda < max(1,nrowa)) {
        info = 8;
    } else if (*ldb < max(1,nrowb)) {
        info = 10;
    } else if (*ldc < max(1,*m)) {
        info = 13;
    }
    if (info != 0) {
        dmmulXerbla("DGEMM ", &info);
        return 0;
    }

    /*     Quick return if possible. */

    if (*m == 0 || *n == 0 || (*alpha == 0. || *k == 0) && *beta == 1.) {
        return 0;
    }

    /*     And if  alpha.eq.zero. */

    if (*alpha == 0.) {
        if (*beta == 0.) {
            i__1 = *n;
            for (j = 1; j <= *n; ++j) {
                i__2 = *m;
                for (i = 1; i <= *m; ++i) {
                    C(i,j) = 0.;
                    /* L10: */
                }
                /* L20: */
            }
        } else {
            i__1 = *n;
            for (j = 1; j <= *n; ++j) {
                i__2 = *m;
                for (i = 1; i <= *m; ++i) {
                    C(i,j) = *beta * C(i,j);
                    /* L30: */
                }
                /* L40: */
            }
        }
        return 0;
    }

    /*     Start the operations. */

    if (notb) {
        if (nota) {

            /*           Form  C := alpha*A*B + beta*C. */

            i__1 = *n;
            for (j = 1; j <= *n; ++j) {
                if (*beta == 0.) {
                    i__2 = *m;
                    for (i = 1; i <= *m; ++i) {
                        C(i,j) = 0.;
                        /* L50: */
                    }
                } else if (*beta != 1.) {
                    i__2 = *m;
                    for (i = 1; i <= *m; ++i) {
                        C(i,j) = *beta * C(i,j);
                        /* L60: */
                    }
                }
                i__2 = *k;
                for (l = 1; l <= *k; ++l) {
                    if (B(l,j) != 0.) {
                        temp = *alpha * B(l,j);
                        i__3 = *m;
                        for (i = 1; i <= *m; ++i) {
                            C(i,j) += temp * A(i,l);
                            /* L70: */
                        }
                    }
                    /* L80: */
                }
                /* L90: */
            }
        } else {

            /*           Form  C := alpha*A'*B + beta*C */

            i__1 = *n;
            for (j = 1; j <= *n; ++j) {
                i__2 = *m;
                for (i = 1; i <= *m; ++i) {
                    temp = 0.;
                    i__3 = *k;
                    for (l = 1; l <= *k; ++l) {
                        temp += A(l,i) * B(l,j);
                        /* L100: */
                    }
                    if (*beta == 0.) {
                        C(i,j) = *alpha * temp;
                    } else {
                        C(i,j) = *alpha * temp + *beta * C(i,j);
                    }
                    /* L110: */
                }
                /* L120: */
            }
        }
    } else {
        if (nota) {

            /*           Form  C := alpha*A*B' + beta*C */

            i__1 = *n;
            for (j = 1; j <= *n; ++j) {
                if (*beta == 0.) {
                    i__2 = *m;
                    for (i = 1; i <= *m; ++i) {
                        C(i,j) = 0.;
                        /* L130: */
                    }
                } else if (*beta != 1.) {
                    i__2 = *m;
                    for (i = 1; i <= *m; ++i) {
                        C(i,j) = *beta * C(i,j);
                        /* L140: */
                    }
                }
                i__2 = *k;
                for (l = 1; l <= *k; ++l) {
                    if (B(j,l) != 0.) {
                        temp = *alpha * B(j,l);
                        i__3 = *m;
                        for (i = 1; i <= *m; ++i) {
                            C(i,j) += temp * A(i,l);
                            /* L150: */
                        }
                    }
                    /* L160: */
                }
                /* L170: */
            }
        } else {

            /*           Form  C := alpha*A'*B' + beta*C */

            i__1 = *n;
            for (j = 1; j <= *n; ++j) {
                i__2 = *m;
                for (i = 1; i <= *m; ++i) {
                    temp = 0.;
                    i__3 = *k;
                    for (l = 1; l <= *k; ++l) {
                        temp += A(l,i) * B(j,l);
                        /* L180: */
                    }
                    if (*beta == 0.) {
                        C(i,j) = *alpha * temp;
                    } else {
                        C(i,j) = *alpha * temp + *beta * C(i,j);
                    }
                    /* L190: */
                }
                /* L200: */
            }
        }
    }

    return 0;

    /*     End of DGEMM . */

} /* dgemm */
/*--------------------------------------------------------------------------*/ 
static long int dmmulLsame(char *ca, char *cb)
{
    /* System generated locals */
    long int ret_val = 0;

    /* Local variables */
    static int inta = 0, intb = 0, zcode = 0;


    /*  -- LAPACK auxiliary routine (version 2.0) -- */
    /*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd., */
    /*     Courant Institute, Argonne National Lab, and Rice University */
    /*     January 31, 1994 */

    /*     .. Scalar Arguments .. */
    /*     .. */

    /*  Purpose */
    /*  ======= */

    /*  LSAME returns .TRUE. if CA is the same letter as CB regardless of */
    /*  case. */

    /*  Arguments */
    /*  ========= */

    /*  CA      (input) CHARACTER*1 */
    /*  CB      (input) CHARACTER*1 */
    /*          CA and CB specify the single characters to be compared. */

    /* ===================================================================== */

    /*     .. Intrinsic Functions .. */
    /*     .. */
    /*     .. Local Scalars .. */
    /*     .. */
    /*     .. Executable Statements .. */

    /*     Test if the characters are equal */

    ret_val = *(unsigned char *)ca == *(unsigned char *)cb;
    if (ret_val) {
        return ret_val;
    }

    /*     Now test for equivalence if both characters are alphabetic. */

    zcode = 'Z';

    /*     Use 'Z' rather than 'A' so that ASCII can be detected on Prime */
    /*     machines, on which ICHAR returns a value with bit 8 set. */
    /*     ICHAR('A') on Prime machines returns 193 which is the same as */
    /*     ICHAR('A') on an EBCDIC machine. */

    inta = *(unsigned char *)ca;
    intb = *(unsigned char *)cb;

    if (zcode == 90 || zcode == 122) {

        /*        ASCII is assumed - ZCODE is the ASCII code of either lower or */
        /*        upper case 'Z'. */

        if (inta >= 97 && inta <= 122) {
            inta += -32;
        }
        if (intb >= 97 && intb <= 122) {
            intb += -32;
        }

    } else if (zcode == 233 || zcode == 169) {

        /*        EBCDIC is assumed - ZCODE is the EBCDIC code of either lower or */
        /*        upper case 'Z'. */

        if (inta >= 129 && inta <= 137 || inta >= 145 && inta <= 153 || inta 
            >= 162 && inta <= 169) {
                inta += 64;
        }
        if (intb >= 129 && intb <= 137 || intb >= 145 && intb <= 153 || intb 
            >= 162 && intb <= 169) {
                intb += 64;
        }

    } else if (zcode == 218 || zcode == 250) {

        /*        ASCII is assumed, on Prime machines - ZCODE is the ASCII code */
        /*        plus 128 of either lower or upper case 'Z'. */

        if (inta >= 225 && inta <= 250) {
            inta += -32;
        }
        if (intb >= 225 && intb <= 250) {
            intb += -32;
        }
    }
    ret_val = inta == intb;

    /*     RETURN */

    /*     End of LSAME */

    return ret_val;
} /* lsame */
/*--------------------------------------------------------------------------*/ 
static int dmmulXerbla(char *srname, int *info)
{

    /*  -- LAPACK auxiliary routine (version 3.0) -- */
    /*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd., */
    /*     Courant Institute, Argonne National Lab, and Rice University */
    /*     September 30, 1994 */

    /*     .. Scalar Arguments .. */
    /*     .. */

    /*  Purpose */
    /*  ======= */

    /*  XERBLA  is an error handler for the LAPACK routines. */
    /*  It is called by an LAPACK routine if an input parameter has an */
    /*  invalid value.  A message is printed and execution stops. */

    /*  Installers may consider modifying the STOP statement in order to */
    /*  call system-specific exception-handling facilities. */

    /*  Arguments */
    /*  ========= */

    /*  SRNAME  (input) CHARACTER*6 */
    /*          The name of the routine which called XERBLA. */

    /*  INFO    (input) INTEGER */
    /*          The position of the invalid parameter in the parameter list */
    /*          of the calling routine. */

    /* ===================================================================== */

    printf("** On entry to %6s, parameter number %2i had an illegal value\n",
        srname, *info);


    /*     End of XERBLA */
    return 0; 
} /* xerbla */
/*--------------------------------------------------------------------------*/ 
