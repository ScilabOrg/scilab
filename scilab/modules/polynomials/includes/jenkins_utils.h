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

#ifndef __JENKINS_UTILS__
#define __JENKINS_UTILS__
#include <stdio.h>
#include <math.h>

struct JT_COMPLEX
{
    double real;
    double img;
};

struct JT_SIGMA
{
    double u;
    double v;
    struct JT_COMPLEX s1;
    struct JT_COMPLEX s2;
};

struct JT_POLY
{
    double coeff[256];
    int deg;
};

int diff(struct JT_POLY*, struct JT_POLY*);
int printPoly(struct JT_POLY*);
int normalize(struct JT_POLY*);
int initialize(struct JT_POLY *P, double* coeff, int deg);
struct JT_COMPLEX complexDivide(struct JT_COMPLEX num, struct JT_COMPLEX den);
double polyEvalReal(struct JT_POLY* P, double real_in);
struct JT_COMPLEX polyEval(struct JT_POLY* P, struct JT_COMPLEX c_in);
double newton(struct JT_POLY* P, double x_init, double tolerance, int max_steps);
int hornerQuad(struct JT_POLY* P, double* coeff, struct JT_SIGMA *sigma);
int copySigma(struct JT_SIGMA* s_to_copy, struct JT_SIGMA* s_dest);
int polyEvalRealCoeff(struct JT_POLY* P, double real_in, double* q_coeff);
#endif //__JENKINS_UTILS__
