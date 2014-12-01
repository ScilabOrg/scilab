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

#ifndef __JENKINS_TRAUB__
#define __JENKINS_TRAUB__
#include "jenkins_utils.h"

#define RESULT_IS_REAL = 2;
#define RESULT_IS_IMG = 3;

int noshift(struct JT_POLY* P, struct JT_POLY* K, int M);
struct JT_COMPLEX compute_s(struct JT_COMPLEX s_in, struct JT_POLY* P, struct JT_POLY* K);
struct JT_SIGMA compute_sigma(struct JT_POLY* P, struct JT_POLY* K, double* QP_quad, double* QK_quad, struct JT_SIGMA sigma);
int is_value_converging(double v_prev, double v_cur, double v_next);
int fixedShift(struct JT_POLY* P, struct JT_POLY* K, double* qP_quad, double* qK_quad, struct JT_SIGMA* sigma_res, struct JT_COMPLEX* s_res);
int performShift(struct JT_POLY* K, struct JT_SIGMA sigma, double* QP_quad, double* QK_quad);
int variableShiftSigma(struct JT_POLY* P, struct JT_POLY* K, double* qP_quad, double* qK_quad, struct JT_SIGMA* sigma, struct JT_COMPLEX* s);
int computeRoots(struct JT_SIGMA* sigma);
int jenkins_traub(double* poly_coeff, int poly_deg, double* res_real, double* res_img, int* nb_roots);
#endif // __JENKINS_TRAUB__
