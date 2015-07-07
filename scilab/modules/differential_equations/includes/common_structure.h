/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __COMMOM_STRUCTURE_H__
#define __COMMOM_STRUCTURE_H__

#include "machine.h"
#include "dynlib_differential_equations.h"

typedef struct
{
    int iero;
} STR_IERODE;

typedef struct
{
    int mesflg, lunit;
} STR_EH0001;

typedef struct
{
    double tsw; /**< @TODO : add comment */
    double rowns2[20]; /**< @TODO : add comment */
    double pdnorm; /**< @TODO : add comment */
    int insufr; /**< @TODO : add comment */
    int insufi; /**< @TODO : add comment */
    int ixpr; /**< @TODO : add comment */
    int iowns2[2]; /**< @TODO : add comment */
    int jtyp; /**< @TODO : add comment */
    int mused; /**< @TODO : add comment */
    int mxordn; /**< @TODO : add comment */
    int mxords; /**< @TODO : add comment */
} STR_LSA001;

typedef struct
{
    double rownr3[2];
    double t0;
    double tlast;
    double toutc;
    int lg0;
    int lg1;
    int lgx;
    int iownr3[2];
    int irfnd;
    int itaskc;
    int ngc;
    int nge;
} STR_LSR001;

typedef struct
{
    double tret;
    double rowns[209];
    double ccmax;
    double el0;
    double h__;
    double hmin;
    double hmxi;
    double hu;
    double rc;
    double tn;
    double uround;
    int illin;
    int init;
    int lyh;
    int lewt;
    int lacor;
    int lsavf;
    int lwm;
    int liwm;
    int mxstep;
    int mxhnil;

    int nhnil;
    int ntrep;
    int nslast;
    int nyh;
    int iowns[6];
    int icf;
    int ierpj;
    int iersl;
    int jcur;

    int jstart;
    int kflag;
    int l;
    int meth;
    int miter;
    int maxord;
    int maxcor;
    int msbp;
    int mxncf;
    int n;
    int nq;
    int nst;
    int nfe;
    int nje;
    int nqu;
} STR_LS0001;

#ifdef _MSC_VER
DIFFERENTIAL_EQUATIONS_IMPEXP STR_IERODE C2F(ierode);
DIFFERENTIAL_EQUATIONS_IMPEXP STR_EH0001 C2F(eh0001);
DIFFERENTIAL_EQUATIONS_IMPEXP STR_LSA001 C2F(lsa001);
DIFFERENTIAL_EQUATIONS_IMPEXP STR_LSR001 C2F(lsr001);
DIFFERENTIAL_EQUATIONS_IMPEXP STR_LS0001 C2F(ls0001);
#else
#ifdef __DIFFERENTIAL_EQUATIONS_GW_HXX__
extern STR_IERODE C2F(ierode);
extern STR_EH0001 C2F(eh0001);
extern STR_LSA001 C2F(lsa001);
extern STR_LSR001 C2F(lsr001);
extern STR_LS0001 C2F(ls0001);
#else
STR_IERODE C2F(ierode);
STR_EH0001 C2F(eh0001);
STR_LSA001 C2F(lsa001);
STR_LSR001 C2F(lsr001);
STR_LS0001 C2F(ls0001);
#endif
#endif
#endif /* !__COMMOM_STRUCTURE_H__ */
