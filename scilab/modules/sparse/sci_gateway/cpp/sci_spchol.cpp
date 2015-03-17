/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*
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

extern "C" int C2F(spcho1)(int* n, double* ia, int* nel, int* iinda, int* ixadjf, int* iperm,
                           int* iinvp, int* icolcnt, int* isnode, int* nnzl, int* nsub, int* nsuper,
                           int* ixsuper, int* iiwork, int* ixadj, int* iadjncy,
                           int* iindlnz, double* ilnz, double* idiag, int* ierr);

extern "C" int C2F(spcho2)(int* n, int* nsuper, int* ixsuper, int* isnode,
                           int* isplit, int* ixlindx, int* ilindx, int* ixlnz,
                           double* ilnz, int* iiwork, int* itmpsiz, double* itmpvec,
                           int* iindlnz, int* nnzl, int* nsub);

extern "C" int C2F(symfct)(int* n, int* idim, int* xadj, int* adjncy,
                           int* perm, int* invp, int* colcnt, int* nsuper, int* xsuper,
                           int* snode, int* nsub, int* xlindx,
                           int* lindx, int* xlnz, int* iwsiz, int* iwork, int* iflag);

extern "C" int C2F(bfinit)(int* n, int* nsuper, int* xsuper,
                           int* snode, int* xlindx,
                           int* lindx, int* cachsz, int* tmpsiz, int* split);

extern "C" int C2F(inpnv)(int* n, int* xadhf, int* colPos, double* ia, int* perm, int* invp,
                          int* nsuper, int* xsuper, int* xlindx, int* lindx,
                          int* xlnz, double* lnz, int* iwork);


using namespace types;

Function::ReturnValue sci_spchol(typed_list &in, int _iRetCount, typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), "spchol", 1);
        return Function::Error;
    }

    if (_iRetCount != 2)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), "spchol", 2);
        return Function::Error;
    }


    //get argument #1
    if (in[0]->isSparse() == false && in[0]->isSparseBool() == false)
    {
        Scierror(999, _("%s: Wrong type for argument %d: Sparse matrix expected.\n"), "spchol", 1);
        return Function::Error;
    }

    Sparse* pspIn = NULL;
    SparseBool* pspbIn = NULL;
    int isize = 0;
    int iNbItems = 0;
    int icols = 0;
    int irows = 0;

    if (in[0]->isSparse())
    {
        pspIn = in[0]->getAs<Sparse>();
        isize = pspIn->getSize();
        iNbItems = (int)pspIn->nonZeros();//nel
        icols = pspIn->getCols();//n
        irows = pspIn->getRows();//m

        if (pspIn->isComplex())
        {
            Scierror(999, _("Wrong type for argument #%d: Real matrix expected.\n"), 1);
            return Function::Error;
        }
    }
    else
    {
        pspbIn = in[0]->getAs<SparseBool>();
        isize = pspbIn->getSize();
        icols = pspbIn->getCols();//n
        irows = pspbIn->getRows();//m
    }

    if (icols != irows)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: Square matrix expected.\n"), "spchol", 1);
        return Function::Error;
    }



    int* piNbItemByRows = new int[irows];//
    int* colPos = NULL;//

    if (in[0]->isSparse())
    {
        colPos = new int[iNbItems];
        pspIn->getNbItemByRow(piNbItemByRows);
        pspIn->getColPos(colPos);
    }
    else
    {
        pspbIn->getNbItemByRow(piNbItemByRows);
        iNbItems = 0;
        for (int i = 0; i < irows; i++)
        {
            iNbItems += piNbItemByRows[i];
        }
        colPos = new int[iNbItems];
        pspbIn->getColPos(colPos);
    }

    //iinda
    //sadr(iinda+nel+n)
    int* iinda = new int[iNbItems + icols];
    int k = 0;
    for (; k < irows; k++)
    {
        iinda[k] = piNbItemByRows[k];
    }
    for (int i = 0; k < irows + iNbItems; i++, k++)
    {
        iinda[k] = colPos[i];
    }

    double* pdblReal = new double[iNbItems];//ia
    int iWriteHead = 0;

    if (in[0]->isSparse())
    {
        for (int j = 0; j < irows; j++)
        {
            for (int i = 0; i < piNbItemByRows[j]; i++)
            {
                pdblReal[iWriteHead] = pspIn->get(j, (colPos[iWriteHead] - 1));
                iWriteHead++;
            }
        }
    }
    else
    {
        for (int j = 0; j < irows; j++)
        {
            for (int i = 0; i < piNbItemByRows[j]; i++)
            {
                pdblReal[iWriteHead] = (double)pspbIn->get(j, (colPos[iWriteHead] - 1));
                iWriteHead++;
            }
        }
    }


    //ixadjf = iadr(ia + nel)
    int* piXadjf = new int[icols + 1];
    //iperm = ixadjf + n + 1
    int* piPerm = new int[icols];
    //iinvp = iperm + n
    int* piInvp = new int[icols];
    //icolcnt = iinvp + n
    int* piColcnt = new int[icols];
    //isnode = icolcnt + n
    int* piIsnode = new int[icols];
    //ixadj = isnode + n
    int* piXadj = new int[icols + 1];
    //iadjncy = ixadj + n + 1
    int* piAdjncy = new int[iNbItems - icols];
    //ixsuper = iadjncy + nel - n
    int* piXsuper = new int[icols + 1];
    ///iiwork = ixsuper + n + 1
    int* piIwork = new int[7 * icols + 3];
    //iindlnz = iiwork + 7 * n + 3
    int* piIndlnz = new int[iNbItems + icols];
    //ilnz = sadr(iindlnz + n + nel)
    double* piLnz = new double[iNbItems];
    //idiag = ilnz + nel
    double* piDiag = new double[icols];
    //err = idiag+n-lstk(bot)

    int iNnzl = 0;
    int iNsub = 0;
    int iNsuper = 0;
    int ierr = 0;

    C2F(spcho1)(&icols, pdblReal, &iNbItems, iinda, piXadjf, piPerm,
                piInvp, piColcnt, piIsnode, &iNnzl, &iNsub, &iNsuper,
                piXsuper, piIwork, piXadj, piAdjncy,
                piIndlnz, piLnz, piDiag, &ierr);

    if (ierr == 1)
    {
        delete[] pdblReal;
        delete[] colPos;
        delete[] piNbItemByRows;
        delete[] iinda;

        delete[] piXadjf;
        delete[] piPerm;
        delete[] piInvp;
        delete[] piColcnt;
        delete[] piIsnode;
        delete[] piXsuper;
        delete[] piIwork;
        delete[] piXadj;
        delete[] piAdjncy;
        delete[] piIndlnz;
        delete[] piLnz;
        delete[] piDiag;
        Scierror(999, _("%s: Matrix is not positive definite.\n"), "spchol");
        return Function::Error;
    }

    int iwsiz = iNsuper + 2 * icols + 1;
    int idim = iNbItems - icols;
    //ixlindx=iiwork+2*nsuper+2*n+1
    int* piXlindx = new int[iNsuper + 1];
    //ilindx=ixlindx+nsuper+1
    int* piLindx = new int[iNsub];
    //ixlnz=ilindx+nsub
    int* piXlnz = new int[icols + 1];
    //ilnz = sadr(ixlnz + n + 1)

    int iflag = 0;

    C2F(symfct)(&icols, &idim, piXadj, piAdjncy,
                piPerm, piInvp, piColcnt, &iNsuper, piXsuper,
                piIsnode, &iNsub, piXlindx,
                piLindx, piXlnz, &iwsiz, piIwork, &iflag);

    //isplit = iadr(ilnz + nnzl)
    int* piSplit = new int[icols];
    //itmpvec = sadr(isplit + n)

    int iCachsz = 16;
    int iTmpsiz = 0;

    C2F(bfinit)(&icols, &iNsuper, piXsuper,
                piIsnode, piXlindx,
                piLindx, &iCachsz, &iTmpsiz, piSplit);

    iTmpsiz = Max(iTmpsiz, icols);
    //ilnz = sadr(ixlnz + n + 1)
    double* pilnz = new double[iNnzl];
    //isplit = iadr(ilnz + nnzl)

    C2F(inpnv)(&icols, piXadjf, colPos, pdblReal, piPerm, piInvp,
               &iNsuper, piXsuper, piXlindx, piLindx,
               piXlnz, pilnz, piIwork);


    //iindlnz=iadr(itmpvec+itmpsiz)
    double* pdblItmpvec = new double[iTmpsiz];

    C2F(spcho2)(&icols, &iNsuper, piXsuper, piIsnode,
                piSplit, piXlindx, piLindx, piXlnz,
                pilnz, piIwork, &iTmpsiz, pdblItmpvec,
                piIndlnz, &iNnzl, &iNsub);

    iNbItems = piXlnz[icols] - 1;
    ierr = 5;

    for (int i = 0; i < iNbItems; i++)
    {
        if (pilnz[i] == (5e63))
        {
            ierr = 1;
            break;
        }
    }

    if (ierr == 1)
    {
        delete[] pdblReal;
        delete[] colPos;
        delete[] piNbItemByRows;
        delete[] iinda;

        delete[] piXadjf;
        delete[] piPerm;
        delete[] piInvp;
        delete[] piColcnt;
        delete[] piIsnode;
        delete[] piXsuper;
        delete[] piIwork;
        delete[] piXadj;
        delete[] piAdjncy;
        delete[] piIndlnz;
        delete[] piLnz;
        delete[] piDiag;
        delete[] piXlindx;

        delete[] piLindx;
        delete[] piXlnz;

        delete[] pilnz;
        delete[] pdblItmpvec;

        Scierror(999, _("WARNING: Possible not positive definite matrix!"));
        return Function::Error;
    }

    Sparse* pOut = new Sparse(icols, icols);
    for (int i = 0, k = 0; i < icols; i++)
    {
        for (int j = 0; j < piIndlnz[i]; j++, k++)
        {
            pOut->set(piIndlnz[icols + k] - 1, i, pilnz[k]);
        }
    }

    InternalType*pOut2 = NULL;

    if (in[0]->isSparse())
    {
        Sparse* pspTempo = new Sparse(icols, icols);
        for (int i = 0; i < icols; i++)
        {
            pspTempo->set(i, piInvp[i] - 1, (double)1);
        }
        pOut2 = pspTempo;
    }
    else
    {
        SparseBool* pspTempo = new SparseBool(icols, icols);
        for (int i = 0; i < icols; i++)
        {
            pspTempo->set(i, piInvp[i] - 1, true);
        }
        pOut2 = pspTempo;
    }

    delete[] pdblReal;
    delete[] colPos;
    delete[] piNbItemByRows;
    delete[] iinda;

    delete[] piXadjf;
    delete[] piPerm;
    delete[] piInvp;
    delete[] piColcnt;
    delete[] piIsnode;
    delete[] piXsuper;
    delete[] piIwork;
    delete[] piXadj;
    delete[] piAdjncy;
    delete[] piIndlnz;
    delete[] piLnz;
    delete[] piDiag;
    delete[] piXlindx;

    delete[] piLindx;
    delete[] piXlnz;

    delete[] pilnz;
    delete[] pdblItmpvec;

    out.push_back(pOut);
    out.push_back(pOut2);

    return Function::OK;
}

