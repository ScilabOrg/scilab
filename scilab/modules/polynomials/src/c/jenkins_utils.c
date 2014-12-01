/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Pierre-Aime Agnel 
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <stdlib.h>
#include "jenkins_utils.h"

int diff(struct JT_POLY* P, struct JT_POLY* P_prime)
{
    int i;
    if (P->deg == 0)
    {
        P_prime->coeff[0] = 0;
        P_prime->deg = 0;
        return 0;
    }
    else if(P->deg < 0)
    {
        return 1;
    }
    else
    {
        P_prime->deg = P->deg - 1;
        for (i = 0; i < P->deg; i++)
        {
            P_prime->coeff[i] = (P->deg - i) * P->coeff[i];
        }

        return 0;
    }
}

int printPoly(struct JT_POLY* P)
{
    int i;
    printf("poly value = \n");
    for (i = 0; i <= P->deg; ++i)
    {
        printf("%f ", P->coeff[i]);
    }
    printf("\n");

    return 0;
}

int normalize(struct JT_POLY* P)
{
    int i;
    double max_coeff = P->coeff[0];
    if (P->deg < 0)
    {
        return 1;
    }
    else
    {
        if (max_coeff == 0)
        {
            return 1;
        }
        for (i = 0; i < P->deg + 1; i++)
        {
            P->coeff[i] /= max_coeff;
        }
    }
    return 0;
}

int initialize(struct JT_POLY *P, double* coeff, int deg)
{
    int i;
    int non_zero_suffix = 0;
    int zero_prefix = 0;


    for (i = 0; i < deg + 1; i++)
    {
        if ((!non_zero_suffix) && (coeff[i] == 0))
        {
            zero_prefix++;
            continue;
        }
        else
        {
            non_zero_suffix++;
            P->coeff[i - zero_prefix] = coeff[i];
        }
        
    }
    if (deg + 1 - zero_prefix == 0)
    {
        P->deg = 0;
        P->coeff[0] = coeff[0];
        return 0;
    }
    else
    {
        P->deg = deg - zero_prefix;
    }
    return 0;
}

struct JT_COMPLEX complexDivide(struct JT_COMPLEX num, struct JT_COMPLEX den)
{
   struct JT_COMPLEX res;
   double norm = den.real * den.real + den.img * den.img;
   res.real = (num.real * den.real + num.img * den.img) / norm;
   res.img = (-1 * num.real * den.img + num.img * den.real) / norm;
   return res;
}

int polyEvalRealCoeff(struct JT_POLY* P, double real_in, double* q_coeff)
{
    // Horner evaluation of a Polynomial P
    // P(z) = (z-s)Q(z) + P(s)
    // Q coefficients are evaluated with Horner's method
    // P(s) is stored in q[degP]

    int i;
    int deg = P->deg;
    q_coeff[0] = P->coeff[0];

    for (i = 1; i < deg + 1; i++)
    {
        q_coeff[i] = P->coeff[i] + q_coeff[i - 1] * real_in;
    }
    return 0;
}

double polyEvalReal(struct JT_POLY* P, double real_in)
{
    double ret = 0;
    int deg = P->deg;
    double* real_out = (double*)malloc((deg + 1) * sizeof(double));
    polyEvalRealCoeff(P, real_in, real_out);
    
    ret = real_out[deg];
    free(real_out);
    return ret;
}

struct JT_COMPLEX polyEval(struct JT_POLY* P, struct JT_COMPLEX c_in)
{
    // Horner evaluation of polynomials
    int i;
    struct JT_COMPLEX c_out;
    double temp;
    c_out.real = P->coeff[0];
    c_out.img = 0;

    if (P->deg == 0)
    {
        return c_out;
    }
    else
    {
        for (i = 0; i < P->deg; ++i)
        {
            temp = P->coeff[i + 1] + c_out.real * c_in.real - c_out.img * c_in.img;
            c_out.img = c_out.img * c_in.real + c_out.real * c_in.img;
            c_out.real = temp;
        }
        return c_out;
    }
}

double newton(struct JT_POLY* P, double x_init, double tolerance, int max_steps)
{
    struct JT_POLY P_prime;
    double x = 0;
    double result = 0;
    double result_prime = 0;
    int i = 0;

    diff(P, &P_prime);
    printPoly(P);
    printPoly(&P_prime);
    x = x_init;
    result = polyEvalReal(P, x);
    result_prime = polyEvalReal(&P_prime, x);

    while ((fabs(result) > tolerance) && (i <= max_steps))
    {
        // TODO: strategy for result_prime = 0
        x -= result/result_prime;
        result = polyEvalReal(P, x);
        result_prime = polyEvalReal(&P_prime, x);
        ++i;
    }
    return x;
}


int hornerQuad(struct JT_POLY* P, double* coeff, struct JT_SIGMA *sigma)
{
    // Gives the quadratic horner coefficient
    // for poly evaluated with Sigma(X) = X^2 + u*X + v
    int i;
    coeff[0] = P->coeff[0];
    coeff[1] = P->coeff[1] - P->coeff[0] * sigma->u;
    for (i = 0; i <= P->deg - 2; ++i)
    {
        coeff[i + 2] = P->coeff[i + 2] - coeff[i + 1] * sigma->u - coeff[i] * sigma->v;
    }
    return 0;
}
