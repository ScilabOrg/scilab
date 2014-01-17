/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __COMMOM_STRUCTURE_CACSD_H__
#define __COMMOM_STRUCTURE_CACSD_H__

#include "machine.h"
#include "dynlib_cacsd.h"

typedef struct
{
    int info, ierr;
} STR_ARL2C;

typedef struct
{
    double gnrm;
} STR_NO2F;

//typedef struct
//{
//    int io, info, ll;
//} STR_SORTIE;
//
//typedef struct
//{
//    int nall1;
//} STR_COMALL;

#ifdef _MSC_VER
CACSD_IMPEXP STR_ARL2C C2F(arl2c);
CACSD_IMPEXP STR_NO2F C2F(no2f);
//CACSD_IMPEXP STR_SORTIE C2F(sortie);
//CACSD_IMPEXP STR_COMALL C2F(comall);
#else
#ifdef __CACSD_GW_HXX__
extern STR_ARL2C C2F(arl2c);
extern STR_NO2F C2F(no2f);
//extern STR_SORTIE C2F(sortie);
//extern STR_COMALL C2F(comall);
#else
STR_ARL2C C2F(arl2c);
STR_NO2F C2F(no2f);
//STR_SORTIE C2F(sortie);
//STR_COMALL C2F(comall);
#endif
#endif
#endif /* !__COMMOM_STRUCTURE_CACSD_H__ */
