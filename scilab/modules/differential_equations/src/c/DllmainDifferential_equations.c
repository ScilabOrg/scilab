
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <windows.h>
#include "machine.h"
#include "dynlib_differential_equations.h"
/*--------------------------------------------------------------------------*/
#pragma comment(lib,"../../../../bin/libintl.lib")
/*--------------------------------------------------------------------------*/

/* we force fortran COMMON initialization */

DIFFERENTIAL_EQUATIONS_IMPEXP struct
{
    int iero;
} C2F(iercol);

DIFFERENTIAL_EQUATIONS_IMPEXP struct
{
    double rho[7], coef[49];
} C2F(colloc);

DIFFERENTIAL_EQUATIONS_IMPEXP struct
{
    int itfeval;
} C2F(fevaltyp);

DIFFERENTIAL_EQUATIONS_IMPEXP struct
{
    char ename[25];
} C2F(fevalname);

DIFFERENTIAL_EQUATIONS_IMPEXP struct
{
    char efsub[25], edfsub[25], egsub[25], edgsub[25], eguess[25];
} C2F(colname);

DIFFERENTIAL_EQUATIONS_IMPEXP struct
{
    int iero;
} C2F(ierajf);

DIFFERENTIAL_EQUATIONS_IMPEXP struct
{
    double br[1000], pas, ppr, ppk, ppa, ppb, ppm, pps, ppl, h__[2], l[2], k[2],
           xe[2], f[4]	/* was [2][2] */, g[2], ue;
    int nn;
} C2F(bcompc);

DIFFERENTIAL_EQUATIONS_IMPEXP struct
{
    char namer[25], namej[25], names[25], namep[25], namepj[25];
} C2F(dassln);

DIFFERENTIAL_EQUATIONS_IMPEXP struct
{
    int mshflg, mshnum, mshlmt, mshalt;
} C2F(colmsh);

DIFFERENTIAL_EQUATIONS_IMPEXP union
{
    struct
    {
        double tzeta[40], tleft, tright;
        int izeta, idum;
    } _1;
    struct
    {
        double zeta[40], aleft, aright;
        int izeta, idum;
    } _2;
    struct
    {
        double zeta[40], aleft, aright;
        int izeta, izsave;
    } _3;
} C2F(colsid);

DIFFERENTIAL_EQUATIONS_IMPEXP union
{
    struct
    {
        double ttl[40], wgtmsh[40], wgterr[40], tolin[40], root[40];
        int jtol[40], lttol[40], ntol;
    } _1;
    struct
    {
        double tol[40], wgtmsh[40], wgterr[40], tolin[40], root[40];
        int jtol[40], ltol[40], ntol;
    } _2;
} C2F(colest);

DIFFERENTIAL_EQUATIONS_IMPEXP struct
{
    int n, nold, nmax, nz, ndmz;
} C2F(colapr);


DIFFERENTIAL_EQUATIONS_IMPEXP union
{
    struct
    {
        int k, nc, mstar, kd, mmax, mt[20];
    } _1;
    struct
    {
        int k, ncomp, mstar, kd, mmax, m[20];
    } _2;
    struct
    {
        int k, ncomp, id1, id2, mmax, m[20];
    } _3;
    struct
    {
        int kdum, ncomp, mstar, kd, mmax, m[20];
    } _4;
    struct
    {
        int kdum, ndum, mstar, kd, mmax, m[20];
    } _5;
    struct
    {
        int k, ncdum, mstar, kdum, mmax, m[20];
    } _6;
    struct
    {
        int k, ncomp, mstar, kdum, mmax, m[20];
    } _7;
} C2F(colord);

DIFFERENTIAL_EQUATIONS_IMPEXP struct
{
    int nonlin, iter, limit, icare, iguess;
} C2F(colnln);

DIFFERENTIAL_EQUATIONS_IMPEXP struct
{
    double precis;
    int iout, iprint;
} C2F(colout);

DIFFERENTIAL_EQUATIONS_IMPEXP struct
{
    double b[28], acol[196], asave[112];
} C2F(colbas);

DIFFERENTIAL_EQUATIONS_IMPEXP struct
{
    int iero;
} C2F(ierdcu);


DIFFERENTIAL_EQUATIONS_IMPEXP struct
{
    int iero;
} C2F(iertwo);

DIFFERENTIAL_EQUATIONS_IMPEXP struct
{
    int ncomp, mstar;
} C2F(icolnew);

DIFFERENTIAL_EQUATIONS_IMPEXP struct
{
    int kfsub, kdfsub, kgsub, kdgsub, kguess, kx, ki, kz;
} C2F(coladr);

DIFFERENTIAL_EQUATIONS_IMPEXP struct
{
    int itfsub, itdfsub, itgsub, itdgsub, itguess;
} C2F(coltyp);

DIFFERENTIAL_EQUATIONS_IMPEXP struct
{
    int kfeval, kx1top, kx2top;
} C2F(fevaladr);

DIFFERENTIAL_EQUATIONS_IMPEXP struct
{
    char namef[6];
} C2F(cintg);

DIFFERENTIAL_EQUATIONS_IMPEXP struct
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
} C2F(lsr001);

DIFFERENTIAL_EQUATIONS_IMPEXP struct
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
} C2F(ls0001);

/*--------------------------------------------------------------------------*/
int WINAPI DllMain (HINSTANCE hInstance , DWORD reason, PVOID pvReserved)
{
    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
            break;
        case DLL_PROCESS_DETACH:
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
    }
    return 1;
}
/*--------------------------------------------------------------------------*/

