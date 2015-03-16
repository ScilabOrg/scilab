/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2015 - Scilab Enterprises - Sylvain GENIN
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <iostream>
#include "sparse_gw.hxx"
#include "function.hxx"
#include "sparse.hxx"

extern "C"
{
#include "charEncoding.h"
#include "Scierror.h"
#include "localization.h"
}

extern "C" int C2F(sfinit)(int* neqns, int* nnza, int* xadj, int* adjncy,
                           int* perm, int* invp, int* colcnt, int* nnzl, int* nsub,
                           int* nsuper, int* snode, int* xsuper,
                           int* iwsiz, int* iwork, int* iflag);

using namespace types;

Function::ReturnValue sci_sfinit(typed_list &in, int _iRetCount, typed_list &out)
{
    if ((in.size() < 8) || (9 < in.size()))
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d to %d expected.\n"), "sfinit", 8, 9);
        return Function::Error;
    }

    if ((_iRetCount < 8) || (9 < _iRetCount))
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d to %d expected.\n"), "sfinit", 8, 9);
        return Function::Error;
    }


    //get argument #1
    if (in[0]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "sfinit", 1);
        return Function::Error;
    }

    Double* pdbl1 = in[0]->getAs<Double>();
    pdbl1->convertToInteger();
    int* neqns = (int*)pdbl1->get();

    //get argument #2
    if (in[1]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "sfinit", 2);
        return Function::Error;
    }

    Double* pdbl2 = in[1]->getAs<Double>();
    pdbl2->convertToInteger();
    int* nnza = (int*)pdbl2->get();

    //get argument #3
    if (in[2]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "sfinit", 3);
        return Function::Error;
    }

    Double* pdbl3 = in[2]->getAs<Double>();
    pdbl3->convertToInteger();
    int* xadj = (int*)pdbl3->get();

    //get argument #4
    if (in[3]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "sfinit", 4);
        return Function::Error;
    }

    Double* pdbl4 = in[3]->getAs<Double>();
    pdbl4->convertToInteger();
    int* adjncy = (int*)pdbl4->get();

    //get argument #5
    if (in[4]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "sfinit", 5);
        return Function::Error;
    }

    Double* pdbl5 = in[4]->getAs<Double>();
    pdbl5->convertToInteger();
    int* perm = (int*)pdbl5->get();

    //get argument #6
    if (in[5]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "sfinit", 6);
        return Function::Error;
    }

    Double* pdbl6 = in[5]->getAs<Double>();
    pdbl6->convertToInteger();
    int* invp = (int*)pdbl6->get();

    //get argument #7
    if (in[6]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "sfinit", 7);
        return Function::Error;
    }

    Double* pdbl7 = in[6]->getAs<Double>();
    pdbl7->convertToInteger();
    int* iwsiz = (int*)pdbl7->get();

    //get argument #8
    if (in[7]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of integer value expected.\n"), "sfinit", 8);
        return Function::Error;
    }

    Double* pdbl8 = in[7]->getAs<Double>();
    pdbl8->convertToInteger();
    int* iwork = (int*)pdbl8->get();
    /*
    c*******************************************************
    c      1-Get rhs parameters and set their Fortran types
    c*******************************************************
    if(.not.getrhsvar( 1,'i', m1, n1, l1)) return
    if(.not.getrhsvar( 2,'i', m2, n2, l2)) return
    if(.not.getrhsvar( 3,'i', m3, n3, l3)) return
    if(.not.getrhsvar( 4,'i', m4, n4, l4)) return
    if(.not.getrhsvar( 5,'i', m5, n5, l5)) return
    if(.not.getrhsvar( 6,'i', m6, n6, l6)) return
    if(.not.getrhsvar( 7,'i', m7, n7, l7)) return
    if(.not.getrhsvar( 8,'i', m8, n8, l8)) return
    **************************************************
    c      2-If necessary, create additional variables
    c          (working arrays, default values, ...)
    c*****************************************************
    */

    Double* pdbl9 = new Double(neqns[0], 1);
    pdbl9->convertToInteger();
    int* colcnt = (int*)pdbl9->get();

    Double* pdbl10 = new Double(1, 1);
    pdbl10->convertToInteger();
    int* nnzl = (int*)pdbl10->get();

    Double* pdbl11 = new Double(1, 1);
    pdbl11->convertToInteger();
    int* nsub = (int*)pdbl11->get();

    Double* pdbl12 = new Double(1, 1);
    pdbl12->convertToInteger();
    int* nsuper = (int*)pdbl12->get();

    Double* pdbl13 = new Double(neqns[0], 1);
    pdbl13->convertToInteger();
    int* snode = (int*)pdbl13->get();

    Double* pdbl14 = new Double(neqns[0] + 1, 1);
    pdbl14->convertToInteger();
    int* xsuper = (int*)pdbl14->get();

    Double* pdbl15 = new Double(1, 1);
    pdbl15->convertToInteger();
    int* iflag = (int*)pdbl15->get();

    /*
    extern "C" int C2F(sfinit)(int* neqns, int* nnza, int* xadj, int* adjncy,
    int* perm, int* invp, int* colcnt, int* nnzl, int* nsub,
    int* nsuper, int* snode, int* xsuper,
    int* iwsiz, int* iwork, int* iflag);

    k=maxrhs+1
    if(.not.createvar(k  ,'i',istk(l1),1,l9)) return
    if(.not.createvar(k+1,'i',1,1,l10)) return
    if(.not.createvar(k+2,'i',1,1,l11)) return
    if(.not.createvar(k+3,'i',1,1,l12)) return
    if(.not.createvar(k+4,'i',istk(l1),1,l13)) return
    if(.not.createvar(k+5,'i',istk(l1)+1,1,l14)) return
    if(.not.createvar(k+6,'i',1,1,l15)) return

    call sfinit(istk(l1),istk(l2),istk(l3) ,istk(l4),
    $  istk(l5), istk(l6) ,istk(l9) ,istk(l10),istk(l11),
    $  istk(l12),istk(l13),istk(l14),
    $  istk(l7), istk(l8) ,istk(l15))
    */

    C2F(sfinit)((int*)pdbl1->get(), (int*)pdbl2->get(), (int*)pdbl3->get(), (int*)pdbl4->get(), (int*)pdbl5->get(), (int*)pdbl6->get(), (int*)pdbl9->get(), (int*)pdbl10->get(),
                (int*)pdbl11->get(), (int*)pdbl12->get(), (int*)pdbl13->get(), (int*)pdbl14->get(), (int*)pdbl7->get(), (int*)pdbl8->get(), (int*)pdbl15->get());

    if (iflag[0])
    {
        Scierror(999, _("%s: insufficient working storage"), "sfinit");
        return Function::Error;
    }

    pdbl5->convertFromInteger();
    pdbl6->convertFromInteger();
    pdbl9->convertFromInteger();
    pdbl10->convertFromInteger();
    pdbl11->convertFromInteger();
    pdbl12->convertFromInteger();
    pdbl13->convertFromInteger();
    pdbl14->convertFromInteger();
    pdbl15->convertFromInteger();

    out.push_back(pdbl5);
    out.push_back(pdbl6);
    out.push_back(pdbl9);
    out.push_back(pdbl10);
    out.push_back(pdbl11);
    out.push_back(pdbl12);
    out.push_back(pdbl13);
    out.push_back(pdbl14);
    out.push_back(pdbl15);

    return Function::OK;
}

