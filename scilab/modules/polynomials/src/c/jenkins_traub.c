// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2014 - Pierre-Aime Agnel
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
// =============================================================================
#include <stdio.h>
#include <math.h>
#include "jenkins_traub.h"
#include <assert.h>
#include <malloc.h>

// Cos and Sin values for angle 0.5*k
static double cosValues[] = {0.540302305868140, 0.070737201667703, -0.416146836547142, -0.801143615546934, -0.989992496600445};
static double sinValues[] = {0.841470984807897, 0.997494986604054,  0.909297426825682,  0.598472144103957,  0.141120008059867};


//no shift
int noshift(struct JT_POLY* P, struct JT_POLY* K, int M)
{
    int i;
    int j;
    diff(P, K);
    int degP = P->deg;
    int degK = K->deg;
    for (i = 0; i < M; ++i)
    {
        // factor is K(0) / P(0)
        // i.e. last coef of K divided by last coef of P
        double factor = K->coeff[degK] / P->coeff[degP];

        // Calculation of K_l+1 = 1/z * (K_l - factor * P)
        // The last coeff of K_l - factor * P is always 0
        for (j = 1; j < degP; ++j)
        {
            K->coeff[degP - j] = K->coeff[degP - 1 - j] - factor * P->coeff[degP - j];
        }
        K->coeff[0] = -1 * factor * P->coeff[0];
    }
    return 0;
}

int fixedShift(struct JT_POLY* P, struct JT_POLY* K, double* qP_quad, double* qK_quad, struct JT_SIGMA* sigma_res, struct JT_COMPLEX* s_res)
{
    double beta;
    double x_init = 0;
    double tolerance = 1E-12;
    int MAX_STEPS = 15;
    int fixed_shift_iterations = 0;
    int MAX_FIXED_SHIFT_ITERATIONS = 5;
    int trials_for_convergence = 0;
    int MAX_TRIALS_FOR_CONVERGENCE = 15;
    int i;
    struct JT_SIGMA sigma;
    struct JT_POLY P_abs;
    int nb_newton = 0;
    int MAX_NB_NEWTON = 32;
    int t_is_converging = 0;
    int sigma_is_converging = 0;

    struct JT_COMPLEX s_prev;
    struct JT_COMPLEX s_curr;
    struct JT_COMPLEX s_next;

    struct JT_SIGMA sigma_prev;
    struct JT_SIGMA sigma_curr;
    struct JT_SIGMA sigma_next;

    // Compute polynomial P_abs with coeffs
    // all monomial coeffs of P in absolute value
    // except monomial coeff of degree 0, opposite of absolute value
    for (i = 0; i < P->deg; i++)
    {
        P_abs.coeff[i] = fabs(P->coeff[i]);
    }
    P_abs.coeff[P->deg] = -1 * fabs(P->coeff[P->deg]);
    P_abs.deg = P->deg;

    // Find only positive root of P_abs
    do
    {
        beta = newton(&P_abs, x_init, tolerance, MAX_STEPS);
        nb_newton++;
        // Doubling strategy if Newton Rhapson is not converging
        x_init = 2 * x_init + 1.01;
    }
    while ((beta <= 0) && (nb_newton <= MAX_NB_NEWTON));

    // Chose one complex value s1 with norm(s1) = beta
    sigma.s1.real = beta * cosValues[0];
    sigma.s1.img = beta * sinValues[0];

    // u and v are such as (X - s1)(X - s1*) = X^2 + uX + v
    sigma.u = -2 * sigma.s1.real;
    sigma.v = sigma.s1.real * sigma.s1.real + sigma.s1.img * sigma.s1.img;

    s_prev = sigma.s1;
    sigma_prev = sigma;

    // =======================
    // Fixed shift calculation
    // =======================

    // Quadratic Hörner is done once for P with given sigma in fixed shift
    hornerQuad(P, qP_quad, &sigma);

    // Quadratic Hörner is done at every step for new K in fixed shift
    hornerQuad(K, qK_quad, &sigma);
    s_curr = compute_s(s_prev, P, K);
    sigma_curr = compute_sigma(P, K, qP_quad, qK_quad, sigma_prev);

    do
    {
        // K is modified by the fixed shift of s
        trials_for_convergence++;
        printf("Current step in Fixed Shift : %d \n", trials_for_convergence);
        performShift(K, sigma, qP_quad, qK_quad);
        s_next = compute_s(sigma.s1, P, K);

        hornerQuad(K, qK_quad, &sigma);
        sigma_next = compute_sigma(P, K, qP_quad, qK_quad, sigma_curr);

        //On each loop there are two converging conditions to check
        // Condition 1: t = Re(s1 - P(s1)/K(s1)) = Re( s_curr )
        // converging (weakly) if | t_curr - t_prev | <= 1/2 * |t_prev| AND | t_next - t_curr | <= 1/2 * |t_curr|
        t_is_converging = is_value_converging(s_prev.real, s_curr.real, s_next.real);
        printf("\nt_prev = %f, t_curr = %f, t_next = %f \n", s_prev.real, s_curr.real, s_next.real);
        // Condition 2: sigma with the two complex conjugate roots
        // converging (weakly) if | v_curr - v_prev | <= 1/2 * |v_prev| AND | v_next - v_curr | <= 1/2 * |v_curr|
        sigma_is_converging = is_value_converging(sigma_prev.v, sigma_curr.v, sigma_next.v);
        printf("sigma_prev = %f, sigma_curr = %f, sigma_next = %f \n\n", sigma_prev.v, sigma_curr.v, sigma_next.v);

        printf("K : ");
        printPoly(K);
        printf("Sigma_curr : X^2 + %f X + %f \n", sigma_curr.u, sigma_curr.v);  
        printf("u = %f, v = %f \n", sigma.u, sigma.v);  
        printf("t_curr = %f, v_curr = %f \n", s_curr.real, sigma_curr.v);  


        s_prev = s_curr;
        s_curr = s_next;
        sigma_prev = sigma_curr;
        sigma_curr = sigma_next;
        printf("t_is_converging = %d, sigma_is_converging = %d \n\n", t_is_converging, sigma_is_converging);

    }
    /*while (!(t_is_converging || sigma_is_converging || (trials_for_convergence > MAX_TRIALS_FOR_CONVERGENCE)));*/
    while (trials_for_convergence < MAX_TRIALS_FOR_CONVERGENCE);
    *s_res = s_curr;
    *sigma_res = sigma_curr;
}

int variableShiftSigma(struct JT_POLY* P, struct JT_POLY* K, double* qP_quad, double* qK_quad, struct JT_SIGMA* sigma, struct JT_COMPLEX* s)
{
    int i;
    int nb_iterations = 0;
    int NB_ITERATIONS_MAX = 10;
    struct JT_COMPLEX s_curr = *s;
    struct JT_SIGMA sigma_curr = *sigma;
    double* coeffK = K->coeff;
    double* coeffP = P->coeff;
    int degK = K->deg;
    int degP = P->deg;
    
    while (nb_iterations < NB_ITERATIONS_MAX)
    {
        /*computeRoots(&sigma_curr)*/
        printf("\nSigma_curr : X^2 + %f X + %f \n", sigma_curr.u, sigma_curr.v);  
        /*printf("s1 : %f + i%f,   s2 : %f + i%f\n", sigma_curr.s1.real, sigma_curr.s1.img, sigma_curr.s2.real, sigma_curr.s2.img);  */
        hornerQuad(P, qP_quad, &sigma_curr);
        hornerQuad(K, qK_quad, &sigma_curr);
        performShift(K, sigma_curr, qP_quad, qK_quad);
        sigma_curr = compute_sigma(P, K, qP_quad, qK_quad, sigma_curr);
        printf("s_curr.real = %f, s_curr.img = %f \n\n", s_curr.real, s_curr.img);  
        nb_iterations++;
    }
    /**s = s_curr;*/
    *sigma = sigma_curr;
    return 0;
}

int computeRoots(struct JT_SIGMA* sigma)
{
    double u = sigma->u;
    double v = sigma->v;
    double delta = (u/2) * (u/2) - v;
    printf("delta = %f\n", delta);
    if (delta >= 0)
    {
        sigma->s1.real = -1 * u/2 + sqrt(delta);
        sigma->s1.img = 0;
        sigma->s2.real = -1* u/2 - sqrt(delta);
        sigma->s2.img = 0;
    }
    else
    {
        sigma->s1.real = -1 * u/2;
        sigma->s1.img = sqrt(-1*delta);
        sigma->s2.real = -1 * u/2;
        sigma->s2.img = -1 * sqrt(-1*delta);
    }
    return 0;
}

struct JT_COMPLEX compute_s(struct JT_COMPLEX s_in, struct JT_POLY* P, struct JT_POLY* K)
{
    // s is  s1 - P(s1) / K(s1)
    struct JT_COMPLEX c;
    struct JT_COMPLEX evalP_out;
    struct JT_COMPLEX evalK_out;

    evalP_out = polyEval(P, s_in);
    evalK_out = polyEval(K, s_in);

    c = complexDivide(evalP_out, evalK_out);
    if(K->coeff[0] != 1)
    {
        c.real /=  K->coeff[0];
        c.img /=   K->coeff[0];
    }
    c.real = s_in.real - c.real;
    printf("c.real = %f, ss = %f \n", c.real, -P->coeff[P->deg] / K->coeff[K->deg]);
    c.img = s_in.img - c.img;

    return c;
}

struct JT_SIGMA compute_sigma(struct JT_POLY* P, struct JT_POLY* K, double* QP_quad, double* QK_quad, struct JT_SIGMA sigma)
{
    int degP = P->deg;
    int degK = K->deg;
    int i;
    double a, b, c, d;
    double coeffQP, coeffQK, denomCoeff, last_norm_term_K1, last_norm_term_K2, num_v, num_u, denom_uv;
    double u = sigma.u;
    double v = sigma.v;
    struct JT_SIGMA next_sigma;

    // TODO: Should be calculated previously, this is redundant
    a = QP_quad[degP - 1];
    b = QP_quad[degP];
    c = QK_quad[degK - 1];
    d = QK_quad[degK];

    /*printf("a = %f, b = %f, c = %f, d = %f \n", a, b, c, d);*/

    coeffQK = a*a + u*a*b + v*b*b;
    coeffQP = a*c + u*a*d + v*b*d; 
    denomCoeff = b*c - a*d;
    coeffQK /= denomCoeff;
    coeffQP /= denomCoeff;

    /*printf("coeffQK = %f, coeffQP = %f \n", coeffQK, coeffQP);*/

    last_norm_term_K1 = -1 * K->coeff[degK] / P->coeff[degP];
    last_norm_term_K2 = -1 * (K->coeff[degK - 1] + last_norm_term_K1 * P->coeff[degP - 1]) / P->coeff[degP];

    /*printf("last_norm_term_K1 = %f, last_norm_term_K2 = %f \n", last_norm_term_K1, last_norm_term_K2);*/

    num_v = v * last_norm_term_K2 * denomCoeff;
    num_v -= last_norm_term_K1 * coeffQP;
    num_v -= last_norm_term_K1 * last_norm_term_K1 * coeffQK;

    denom_uv = c*c + d*u*c + v*d*d;
    denom_uv += last_norm_term_K1 * ((a + u*b) * c + v * b * d);
    denom_uv -= num_v;

    next_sigma.v = v * (1 + num_v / denom_uv);

    num_u = last_norm_term_K1 * last_norm_term_K1 * coeffQK + last_norm_term_K1 * coeffQP;
    num_u *= u;
    num_u += v * (last_norm_term_K1 * denomCoeff + last_norm_term_K2 * coeffQP);
    num_u /= denom_uv;
    next_sigma.u = u - num_u;
    
    return next_sigma;
}

int is_value_converging(double v_prev, double v_cur, double v_next)
{
    double v_prev_cur = v_prev - v_cur;
    double v_cur_next = v_next - v_cur;
    return (fabs(v_prev_cur) <= fabs(v_prev) / 2) && (fabs(v_cur_next) <= fabs(v_cur) / 2);
}

int performShift(struct JT_POLY* K, struct JT_SIGMA sigma, double* QP_quad, double* QK_quad)
{
    double coeffQK, coeffQP;
    int deg_QP = K->deg - 1;
    int deg_QK = K->deg - 2;
    int i;
    double a, b, c, d;
    double u = sigma.u;
    double v = sigma.v;

    //QP_quad contains on [0 deg_QP] all the coeff values for QP and [degQP+1 degQP+2] the values for horner computation
    a = QP_quad[deg_QP + 1];
    b = QP_quad[deg_QP + 2];
    c = QK_quad[deg_QK + 1];
    d = QK_quad[deg_QK + 2];
    coeffQK = a*a + u*a*b + v*b*b;
    coeffQP = a*c + u*a*d + v*b*d; 
    coeffQK /= b*c - a*d;
    coeffQP /= b*c - a*d;

    K->coeff[0] = QP_quad[0];
    K->coeff[1] = QP_quad[1] - coeffQP * QP_quad[0];
    for (i = 2; i < K->deg; i++) 
    {
        K->coeff[i] = coeffQK * QK_quad[i - 2] + QP_quad[i] - coeffQP * QP_quad[i - 1];
    }
    K->coeff[K->deg] = coeffQK * QK_quad[deg_QK] + coeffQP * QP_quad[deg_QP] + b;

    return 0;
}

int jenkins_traub(double* poly_coeff, int poly_deg, double* res_real, double* res_img, int* nb_roots)
{
    // Computes a jenkins traub algorithm while the roots are not all found
    struct JT_POLY P;
    struct JT_POLY K;

    initialize(&P, poly_coeff, poly_deg);
    noshift(&P, &K, 5);

    double qp_quad[256];
    double qk_quad[256];
    
    struct JT_SIGMA sigma;
    struct JT_COMPLEX s;
    fixedShift(&P, &K, qp_quad, qk_quad, &sigma, &s);
    variableShiftSigma(&P, &K, qp_quad, qk_quad, &sigma, &s);

    res_img[0] = 0;
    res_real[0] = 0;
    (*nb_roots) = 1;

    return 0;
}
