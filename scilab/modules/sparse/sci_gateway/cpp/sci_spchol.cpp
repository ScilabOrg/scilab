/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "sparse_gw.hxx"
#include "function.hxx"
#include "sparse.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "sci_warning.h"
#include "sciprint.h"
}

extern int C2F(bfinit)(int*, int*, int*, int*, int*, int*, int*, int*, int*);
extern int C2F(symfct)(int*, int*, int*, int*, int*, int*, int*, int*, int*,
                       int*, int*, int*, int*, int*, int*, int*, int*);
extern int C2F(inpnv)(int*, int*, int*, double*, int*, int*, int*,
                      int*, int*, int*, int*, double*, int*);
extern int C2F(spcho1)(int*, double*, int*, int*, int*, int*, int*, int*,
                       int*, int*, int*, int*, int*, int*, int*, int*,
                       int*, double*, double*, int*);
extern int C2F(spcho2)(int*, int*, int*, int*, int*, int*, int*, int*, double*,
                       int*, int*, double*, int*, int*, int*);


types::Function::ReturnValue sci_spchol(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Sparse* pSpIn        = NULL;
    types::Double* pDblPermut   = NULL;
    types::Double* pDblFact     = NULL;

    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), "spchol", 1);
        return types::Function::Error;
    }

    if (_iRetCount != 2)
    {
        Scierror(999, _("%s: Wrong number of output argument(s): %d expected.\n"), "spchol", 2);
        return types::Function::Error;
    }

    if (in[0]->isSparse() == false && in[0]->isSparseBool() == false)
    {
        Scierror(999, _("%s: Wrong type for argument %d: Sparse matrix expected.\n"), "spchol", 1);
        return types::Function::Error;
    }

    pSpIn = in[0]->getAs<types::Sparse>();

    if (pSpIn->isComplex())
    {
        Scierror(999, _("%s: Wrong type for argument %d: Real sparse matrix expected.\n"), "spchol", 1);
        return types::Function::Error;
    }

    if (pSpIn->getRows() != pSpIn->getCols())
    {
        Scierror(999, _("%s: Wrong size for argument %d: Square sparse matrix expected.\n"), "spchol", 1);
        return types::Function::Error;
    }

    int ierr      = 0;
    int nnzl      = 0;
    int nsub      = 0;
    int nsuper    = 0;
    int n         = pSpIn->getRows(); // square sparse
    int nel       = pSpIn->nonZeros();
    int* iinda    = (int*)malloc(n * sizeof(int));
    int* iColPos  = (int*)malloc(nel * sizeof(int));

    int iSizeWork = 4 * nel + 16 * n + 6;
    int* piWork   = (int*)malloc(iSizeWork * sizeof(double)); // ixadjf

    int* iperm    = piWork + n + 1;
    int* iinvp    = iperm + n;
    int* icolcnt  = iinvp + n;
    int* isnode   = icolcnt + n;
    int* ixadj    = isnode + n;
    int* iadjncy  = ixadj + n + 1;
    int* ixsuper  = iadjncy + nel - n;
    int* iiwork   = ixsuper + n + 1;
    int* iindlnz  = iiwork + 7 * n + 3;
    double* ilnz  = (double*)(iindlnz + n + nel);
    double* idiag = ilnz + nel; // size of idiag is n

    pSpIn->getNbItemByRow(iinda);
    pSpIn->getColPos(iColPos);

    double* pdblData = (double*)malloc(nel * sizeof(double));
    pSpIn->outputValues(pdblData, NULL);

    C2F(spcho1)(&n, pdblData, &nel, iinda, piWork, iperm, iinvp, icolcnt, isnode, &nnzl, &nsub, &nsuper,
                ixsuper, iiwork, ixadj, iadjncy, iindlnz, ilnz, idiag, &ierr);

    if (ierr)
    {
        Scierror(999, _("%s: Wrong type for argument %d: Positive definite sparse matrix expected.\n"), "spchol", 1);
        return types::Function::Error;
    }

    printf("nnzl %d\n", nnzl);
    printf("nsub %d\n", nsub);
    printf("nsuper %d\n", nsuper);

    int itmpsiz  = 0;
    int iSize    = nsuper + 2 * n + 1;
    int inelmn   = nel - n;
    int iflag    = 0;
    int isixteen = 16;

    iSizeWork = 4 * nsuper + 4 * n + nsub + 2 * nnzl + 4;
    int* piWork2  = (int*)malloc(iSizeWork * sizeof(int)); // ixsuper

    iiwork       = piWork2 + nsuper + 1;
    int* ixlindx = iiwork + 2 * nsuper + 2 * n + 1;
    int* ilindx  = ixlindx + nsuper + 1;
    int* ixlnz   = ilindx + nsub;
    ilnz         = (double*)(ixlnz + n + 1);
    int* isplit  = (int*)(ilnz + nnzl); // size of isplit is n

    C2F(symfct)(&n, &inelmn, ixadj, iadjncy, iperm, iinvp, icolcnt, &nsuper, ixsuper, isnode,
                &nsub , ixlindx, ilindx, ixlnz, &iSize, iiwork, &iflag);

    C2F(bfinit)(&n, &nsuper, ixsuper, isnode, ixlindx, ilindx, &isixteen, &itmpsiz, isplit);

    printf("itmpsiz %d\n", itmpsiz);

    double* itmpvec  = (double*)malloc(Max(itmpsiz, n) * sizeof(double));
    iindlnz  = (int*)malloc((n + nnzl) * sizeof(int));

    C2F(inpnv)(&n, piWork, iColPos , pdblData, iperm, iinvp , &nsuper, piWork2, ixlindx, ilindx,
               ixlnz, ilnz, iiwork);

    C2F(spcho2)(&n, &nsuper, piWork2, isnode, isplit, ixlindx, ilindx, ixlnz,
                ilnz, iiwork, &itmpsiz, itmpvec, iindlnz, &nnzl, &nsub);

    nel = ixlnz[n] - 1;
    int* ia = iinda + nel + n; // data of sparse
    ierr = 5;

    for (int i = 0; i < nel; i++)
    {
        if (ilnz[i] > 5.e+63)
        {
            ierr = 1;
            if (getWarningMode())
            {
                sciprint(_("warning: possible not positive definite matrix!\n"));
            }
            break;
        }
    }

    memcpy(iindlnz - n, iinvp, n * sizeof(int));

    types::Sparse* pSpR = new types::Sparse(n, n);
    types::Sparse* pSpP = new types::Sparse(n, n);

    out.push_back(pSpR);
    if (_iRetCount == 2)
    {
        out.push_back(pSpP);
    }

    // DO NOT FORGET TO FREE MEMORY !

    return types::Function::OK;
}
