/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
 *  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

#include <iostream>
#include <cwchar>
#include <cmath>
#include <cstring>

#include "Helpers.hxx"

extern "C"
{
#include "machine.h" /* C2F */
#include "stack-c.h"
#include "charEncoding.h"
#include "dynlib_scicos_blocks.h"
#include "mopen.h"
#include "mget.h"
#include "mclose.h"
#include "expandPathVariable.h"
#include "scicos_block4.h"
#include "scicos_evalhermite.h"
#include "scicos.h"
    //#include "os_string.h"

#include "localization.h"

    SCICOS_BLOCKS_IMPEXP void fromws_c(scicos_block* block, int flag);
}

/*--------------------------------------------------------------------------*/
static char fmtd[3] = {'d', 'l', '\000'};
static char fmti[3] = {'i', 'l', '\000'};
static char fmtl[3] = {'l', 'l', '\000'};
static char fmts[3] = {'s', 'l', '\000'};
static char fmtc[3] = {'c', 'l', '\000'};
static char fmtul[3] = {'u', 'l', '\000'};
static char fmtus[3] = {'u', 's', '\000'};
static char fmtuc[3] = {'u', 'c', '\000'};
/*--------------------------------------------------------------------------*/
/* work struct for a block */
typedef struct
{
    int nPoints;
    int Hmat;
    int Yt;
    int Yst;
    int cnt1;
    int cnt2;
    int EVindex;
    int PerEVcnt;
    int firstevent;
    double* D;
    void* work;
    double* workt;
} fromwork_struct;
/*--------------------------------------------------------------------------*/
static bool Ishm(int* fd, int &Ytype, int &nPoints, int &my, int &ny, int &YsubType)
{
    int j, ierr;
    int ptr_i[37];
    C2F(mgetnc) (fd, ptr_i, (j = 37, &j), fmti, &ierr); /* Read sci id */
    if (ierr != 0)
    {
        return false;
    }

    if ((ptr_i[0] != 3)  || \
            (ptr_i[1] != 1)  || \
            (ptr_i[5] != 10) || \
            (ptr_i[6] != 1)  || \
            (ptr_i[7] != 3)  || \
            (ptr_i[8] != 0)  || \
            (ptr_i[9] != 1)  || \
            (ptr_i[10] != ptr_i[9] + 2)  || \
            (ptr_i[11] != ptr_i[10] + 4) || \
            (ptr_i[12] != ptr_i[11] + 7) || \
            (ptr_i[13] != 17) || \
            (ptr_i[14] != 22) || \
            (ptr_i[15] != 13) || \
            (ptr_i[16] != 18) || \
            (ptr_i[17] != 22) || \
            (ptr_i[18] != 28) || \
            (ptr_i[19] != 14) || \
            (ptr_i[20] != 23) || \
            (ptr_i[21] != 29) || \
            (ptr_i[22] != 27) || \
            (ptr_i[23] != 18) || \
            (ptr_i[24] != 14) || \
            (ptr_i[25] != 28) || \
            (ptr_i[26] != 8)  || \
            (ptr_i[27] != 1)  || \
            (ptr_i[28] != 3)  || \
            (ptr_i[29] != 4))
    {
        Coserror(_("Invalid variable type : error in hypermat Scilab coding.\n"));
        return false;
    }

    my      = ptr_i[30];    /*37*/
    ny      = ptr_i[31];    /*38*/
    nPoints = ptr_i[32];    /*39*/
    Ytype   = ptr_i[33];    /*40*/

    if ((ptr_i[34] != ptr_i[30]*ptr_i[31]*ptr_i[32]) || \
            (ptr_i[35] != 1))
    {
        Coserror(_("Invalid variable type : error in hypermat Scilab coding.\n"));
        return false;
    }

    YsubType = ptr_i[36];   /*43*/

    return true;
}
/*--------------------------------------------------------------------------*/
static int Mytridiagldltsolve(double* &dA, double* &lA, double* &B, int N)
{
    for (int j = 1; j <= N - 1; ++j)
    {
        double Temp = lA[j - 1];
        lA[j - 1] /= dA[j - 1];
        B[j] -= lA[j - 1] * B[j - 1];
        dA[j] -= Temp * lA[j - 1];
    }

    B[N - 1] /= dA[N - 1];
    for (int j = N - 2; j >= 0; --j)
    {
        B[j] = - lA[j] * B[j + 1] + B[j] / dA[j];
    }

    return 0;
}

/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_xcos_block;

/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void fromws_c(scicos_block* block, int flag)
{
    /* Retrieve dimensions of output port */
    int my    = GetOutPortRows(block, 1); /* Number of rows of the output */
    int ny    = GetOutPortCols(block, 1); /* Number of cols of the output */
    int ytype = GetOutType(block, 1);     /* Output type */

    /* Generic pointers */
    double *y_d, *y_cd, *ptr_d, *ptr_T, *ptr_D;
    char *y_c, *ptr_c;
    unsigned char *y_uc, *ptr_uc;
    short int *y_s, *ptr_s;
    unsigned short int *y_us, *ptr_us;
    int *y_l, *ptr_l;
    unsigned int *y_ul, *ptr_ul;

    /* The struct pointer of the block */
    fromwork_struct** work = (fromwork_struct**) block->work;
    fromwork_struct* ptr = nullptr;

    int Fnlength = block->ipar[0];
    int Method   = block->ipar[1 + Fnlength];
    int ZC       = block->ipar[2 + Fnlength];
    int OutEnd   = block->ipar[3 + Fnlength];

    switch (flag)
    {
        case 4 :
        {
            /* Init */

            /* Convert Scilab code of the variable name to C string */

            /* Path to "TMPDIR/Workspace/" */
            const wchar_t* filePrefix = L"TMPDIR/Workspace/";

            char FName[100];
            for (int i = 0; i < Fnlength; ++i)
            {
                FName[i] = static_cast<char>(block->ipar[1] + i);
            }
            FName[Fnlength] = '\0';
            wchar_t* FNameW = to_wide_string(FName);

            wchar_t* env = new wchar_t[wcslen(filePrefix) + Fnlength + 1];
            wcscpy(env, filePrefix);
            wcscpy(env + wcslen(filePrefix), FNameW);
            env[wcslen(filePrefix) + Fnlength] = L'\0';
            delete[] FNameW;

            wchar_t* filename = expandPathVariableW(env);
            delete[] env;
            int fd, swap = 1, ierr = 1;
            if (filename)
            {
                /* Open tmp file */
                /* "r" : read */
                /* "b" : binary format (required for Windows) */
                ierr = mopen(filename, L"rb", swap, &fd);
            }
            delete[] filename;
            filename = nullptr;

            if (ierr != 0)
            {
                Coserror(_("The '%s' variable does not exist.\n"), FName);
                return;
            }

            /* Read the variable contained in the file */
            int Ydim[10], j = 0;
            C2F(mgetnc) (&fd, &Ydim[0], (j = nsiz, &j), fmti, &ierr); /* Read sci id */
            C2F(mgetnc) (&fd, &Ydim[6], (j = 1, &j), fmti, &ierr);    /* Read sci type */
            int Ytype, nPoints, mY, nY, YsubType;
            if (Ydim[6] == 17)
            {
                //FIXME : Ydim[6] == 17 is not a hypermat condition in yasp!
                if (!Ishm(&fd, Ytype, nPoints, mY, nY, YsubType))
                {
                    Coserror(_("Invalid variable type.\n"));
                    /*scicos_print(_("Invalid variable type.\n"));
                    set_block_error(-3); */
                    mclose(fd);
                    return;
                }
                if (!((Ytype == 1) || (Ytype == 8)))
                {
                    Coserror(_("Invalid variable type.\n"));
                    /*scicos_print(_("Invalid variable type.\n"));
                    set_block_error(-3);*/
                    mclose(fd);
                    return;
                }
            }
            else if ((Ydim[6] == 1) || (Ydim[6] == 8))
            {
                C2F(mgetnc) (&fd, &Ydim[7], (j = 3, &j), fmti, &ierr); /* Read sci header */
                Ytype    = Ydim[6]; /* data type        */
                nPoints  = Ydim[7]; /* number of data   */
                mY       = Ydim[8]; /* first dimension  */
                nY       = 1;       /* second dimension */
                YsubType = Ydim[9]; /* subtype          */
            }
            else
            {
                Coserror(_("Invalid variable type.\n"));
                /*scicos_print(_("Invalid variable type.\n"));
                set_block_error(-3);*/
                mclose(fd);
                return;
            }

            /* Check dimension for output port and variable */
            if ((mY != my) || (nY != ny))
            {
                Coserror(_("Data dimensions are inconsistent:\n Variable size=[%d,%d] \nBlock output size=[%d,%d].\n"), mY, nY, my, ny);
                /*set_block_error(-3);*/
                mclose(fd);
                return;
            }

            /* Check variable data type and output block data type */
            if (Ytype == 1)
            {
                /* real/complex cases*/
                switch (YsubType)
                {
                    case 0:
                        if (ytype != 10)
                        {
                            Coserror(_("Output should be of Real type.\n"));
                            /*set_block_error(-3);*/
                            mclose(fd);
                            return;
                        }
                        break;

                    case 1:
                        if (ytype != 11)
                        {
                            Coserror(_("Output should be of complex type.\n"));
                            /*set_block_error(-3);*/
                            mclose(fd);
                            return;
                        }
                        break;
                }
            }
            else if (Ytype == 8)
            {
                /* int cases*/
                switch (YsubType)
                {
                    case 1:
                        if (ytype != 81)
                        {
                            Coserror(_("Output should be of int8 type.\n"));
                            set_block_error(-3);
                            mclose(fd);
                            return;
                        }
                        break;

                    case 2:
                        if (ytype != 82)
                        {
                            Coserror(_("Output should be of int16 type.\n"));
                            /*set_block_error(-3);*/
                            mclose(fd);
                            return;
                        }
                        break;

                    case 4:
                        if (ytype != 84)
                        {
                            Coserror(_("Output should be of int32 type.\n"));
                            /*set_block_error(-3);*/
                            mclose(fd);
                            return;
                        }
                        break;

                    case 11:
                        if (ytype != 811)
                        {
                            Coserror(_("Output should be of uint8 type.\n"));
                            /*set_block_error(-3);*/
                            mclose(fd);
                            return;
                        }
                        break;

                    case 12:
                        if (ytype != 812)
                        {
                            Coserror(_("Output should be of uint16 type.\n"));
                            /*set_block_error(-3);*/
                            mclose(fd);
                            return;
                        }
                        break;

                    case 14:
                        if (ytype != 814)
                        {
                            Coserror(_("Output should be of uint32 type.\n"));
                            /*set_block_error(-3);*/
                            mclose(fd);
                            return;
                        }
                        break;
                }
            }

            /* Allocation of the work structure of that block */

            *work = new fromwork_struct[sizeof(fromwork_struct)];
            ptr = *work;
            ptr->D = nullptr;
            ptr->workt = nullptr;
            ptr->work = nullptr;

            if (Ytype == 1)
            {
                /* real/complex case*/
                switch (YsubType)
                {
                    case 0 : /* Real */
                        ptr->work = new double[nPoints * mY * nY];
                        ptr_d = (double*) ptr->work;
                        C2F(mgetnc) (&fd, ptr_d, (j = nPoints * mY * nY, &j), fmtd, &ierr); /* Read double data */
                        break;
                    case 1:  /* Complex */
                        ptr->work = new double[nPoints * mY * nY];
                        ptr_d = (double*) ptr->work;
                        C2F(mgetnc) (&fd, ptr_d, (j = 2 * nPoints * mY * nY, &j), fmtd, &ierr); /* Read double data */
                        break;
                }
            }
            else if (Ytype == 8)
            {
                /* int case*/
                switch (YsubType)
                {
                    case 1 :/* int8 */
                        ptr->work = new char[nPoints * mY * nY];
                        ptr_c = (char*) ptr->work;
                        C2F(mgetnc) (&fd, ptr_c, (j = nPoints * mY * nY, &j), fmtc, &ierr); /* Read char data */
                        break;
                    case 2 :/* int16 */
                        ptr->work = new short int[nPoints * mY * nY];
                        ptr_s = (short int*) ptr->work;
                        C2F(mgetnc) (&fd, ptr_s, (j = nPoints * mY * nY, &j), fmtc, &ierr); /* Read short int data */
                        break;
                    case 4 :/* int32 */
                        ptr->work = new int[nPoints * mY * nY];
                        ptr_l = (int*) ptr->work;
                        C2F(mgetnc) (&fd, ptr_l, (j = nPoints * mY * nY, &j), fmtc, &ierr); /* Read int data */
                        break;
                    case 11 :/* uint8 */
                        ptr->work = new unsigned char[nPoints * mY * nY];
                        ptr_uc = (unsigned char*) ptr->work;
                        C2F(mgetnc) (&fd, ptr_uc, (j = nPoints * mY * nY, &j), fmtc, &ierr); /* Read unsigned char data */
                        break;
                    case 12 :/* uint16 */
                        ptr->work = new unsigned short int[nPoints * mY * nY];
                        ptr_us = (unsigned short int*) ptr->work;
                        C2F(mgetnc) (&fd, ptr_us, (j = nPoints * mY * nY, &j), fmtc, &ierr); /* Read unsigned short int data */
                        break;
                    case 14 :/* uint32 */
                        ptr->work = new unsigned int[nPoints * mY * nY];
                        ptr_ul = (unsigned int*) ptr->work;
                        C2F(mgetnc) (&fd, ptr_ul, (j = nPoints * mY * nY, &j), fmtc, &ierr); /* Read unsigned int data */
                        break;
                }
            }

            /* check Hmat */
            if (Ydim[6] == 17)
            {
                ptr->Hmat = 1;
            }
            else
            {
                ptr->Hmat = 0;
            }

            /* Read t */
            C2F(mgetnc) (&fd, &Ydim[0], (j = nsiz, &j), fmti, &ierr); /* Read sci id */
            C2F(mgetnc) (&fd, &Ydim[6], (j = 1, &j), fmti, &ierr);    /* Read sci type */
            C2F(mgetnc) (&fd, &Ydim[7], (j = 3, &j), fmti, &ierr);    /* Read sci header */

            if (nPoints != Ydim[7])
            {
                Coserror(_("The Time vector has a wrong size, expecting [%d, %d] and getting [%d, %d].\n"), nPoints, 1, Ydim[7], Ydim[8]);
                /*set_block_error(-3);*/
                *work = nullptr;
                delete[] ptr->work;
                delete[] ptr;
                mclose(fd);
                return;
            }

            if ((Ydim[6] != 1) | (Ydim[9] != 0))
            {
                Coserror(_("The Time vector type is not ""double"".\n"));
                set_block_error(-3);
                *work = nullptr;
                delete[] ptr->work;
                delete[] ptr;
                mclose(fd);
                return;
            }

            ptr->workt = new double[nPoints];
            ptr_T = (double*) ptr->workt;
            C2F(mgetnc) (&fd, ptr_T, (j = nPoints, &j), fmtd, &ierr); /* Read data of t */

            /* Close the file*/
            mclose(fd);

            /*================================*/
            /* Check for an increasing time data */
            for (j = 0; j < nPoints - 1; ++j)
            {
                if (ptr_T[j] > ptr_T[j + 1])
                {
                    Coserror(_("The time vector should be an increasing vector.\n"));
                    /*set_block_error(-3);*/
                    *work = NULL;
                    delete[] ptr->workt;
                    delete[] ptr->work;
                    delete[] ptr;
                    return;
                }
            }
            /*=================================*/
            if ((Method > 1) && (Ytype == 1) && (!ptr->Hmat))
            {
                /* double or complex */
                if (YsubType == 0) /* real */
                {
                    ptr->D = new double[nPoints * mY];
                }
                else /* complex */
                {
                    ptr->D = new double[2 * nPoints * mY];
                }

                double* spline = new double[3 * nPoints - 2];

                double* A_d  = spline;
                double* A_sd = A_d  + nPoints;
                double* qdy  = A_sd + nPoints - 1;

                for (j = 0; j < mY; ++j)
                {
                    /* real part */
                    for (int i = 0; i <= nPoints - 2; ++i)
                    {
                        A_sd[i] = 1 / (ptr_T[i + 1] - ptr_T[i]);
                        qdy[i]  = (ptr_d[i + 1 + j * nPoints] - ptr_d[i + j * nPoints]) * A_sd[i] * A_sd[i];
                    }

                    for (int i = 1; i <= nPoints - 2; ++i)
                    {
                        A_d[i] = 2 * (A_sd[i - 1] + A_sd[i]);
                        ptr->D[i + j * nPoints] = 3 * (qdy[i - 1] + qdy[i]);
                    }

                    if (Method == 2)
                    {
                        A_d[0] =  2 * A_sd[0];
                        ptr->D[0 + j * nPoints] = 3 * qdy[0];
                        A_d[nPoints - 1] =  2 * A_sd[nPoints - 2];
                        ptr->D[nPoints - 1 + j * nPoints] =  3 * qdy[nPoints - 2];
                        double* res = &ptr->D[j * nPoints];
                        Mytridiagldltsolve(A_d, A_sd, res, nPoints);
                    }

                    if (Method == 3)
                    {
                        /*  s'''(x(2)-) = s'''(x(2)+) */
                        double r = A_sd[1] / A_sd[0];
                        A_d[0] = A_sd[0] / (1 + r);
                        ptr->D[j * nPoints] = ((3 * r + 2) * qdy[0] + r * qdy[1]) / ((1 + r) * (1 + r));
                        /*  s'''(x(n-1)-) = s'''(x(n-1)+) */
                        r = A_sd[nPoints - 3] / A_sd[nPoints - 2];
                        A_d[nPoints - 1] = A_sd[nPoints - 2] / (1 + r);
                        ptr->D[nPoints - 1 + j * nPoints] = ((3 * r + 2) * qdy[nPoints - 2] + r * qdy[nPoints - 3]) / ((1 + r) * (1 + r));
                        double* res = &ptr->D[j * nPoints];
                        Mytridiagldltsolve(A_d, A_sd, res, nPoints);
                    }
                }

                if (YsubType == 1)
                {
                    /* imag part */
                    for (j = 0; j < mY; ++j)
                    {
                        for (int i = 0; i <= nPoints - 2; ++i)
                        {
                            A_sd[i] = 1 / (ptr_T[i + 1] - ptr_T[i]);
                            qdy[i]  = (ptr_d[nPoints + i + 1 + j * nPoints] - ptr_d[nPoints + i + j * nPoints]) * A_sd[i] * A_sd[i];
                        }

                        for (int i = 1; i <= nPoints - 2; ++i)
                        {
                            A_d[i] = 2 * (A_sd[i - 1] + A_sd[i]);
                            ptr->D[i + j * nPoints + nPoints] = 3 * (qdy[i - 1] + qdy[i]);
                        }

                        if (Method == 2)
                        {
                            A_d[0] =  2 * A_sd[0];
                            ptr->D[nPoints + 0 + j * nPoints] = 3 * qdy[0];
                            A_d[nPoints - 1] =  2 * A_sd[nPoints - 2];
                            ptr->D[nPoints + nPoints - 1 + j * nPoints] =  3 * qdy[nPoints - 2];
                            double* res = &ptr->D[nPoints + j * nPoints];
                            Mytridiagldltsolve(A_d, A_sd, res, nPoints);
                        }

                        if (Method == 3)
                        {
                            /*  s'''(x(2)-) = s'''(x(2)+) */
                            double r = A_sd[1] / A_sd[0];
                            A_d[0] = A_sd[0] / (1 + r);
                            ptr->D[nPoints + j * nPoints] = ((3 * r + 2) * qdy[0] + r * qdy[1]) / ((1 + r) * (1 + r));
                            /*  s'''(x(n-1)-) = s'''(x(n-1)+) */
                            r = A_sd[nPoints - 3] / A_sd[nPoints - 2];
                            A_d[nPoints - 1] = A_sd[nPoints - 2] / (1 + r);
                            ptr->D[nPoints + nPoints - 1 + j * nPoints] = ((3 * r + 2) * qdy[nPoints - 2] + r * qdy[nPoints - 3]) / ((1 + r) * (1 + r));
                            double* res = &ptr->D[nPoints + j * nPoints];
                            Mytridiagldltsolve(A_d, A_sd, res, nPoints);
                        }
                    }
                }

                delete[] spline;
            }
            /*===================================*/
            int cnt1 = nPoints - 1;
            int cnt2 = nPoints;
            for (int i = 0; i < nPoints; ++i)
            {
                /* finding the first positive time instant */
                if (ptr->workt[i] >= 0)
                {
                    cnt1 = i - 1;
                    cnt2 = i;
                    break;
                }
            }
            ptr->nPoints = nPoints;
            ptr->Yt = Ytype;
            ptr->Yst = YsubType;
            ptr->cnt1 = cnt1;
            ptr->cnt2 = cnt2;
            ptr->EVindex = 0;
            ptr->PerEVcnt = 0;
            ptr->firstevent = 1;
            break;
            /*******************************************************/
            /*******************************************************/
        }
        case 1 :
        {
            /* Output computation */

            /* Retrieve 'ptr' of the structure of the block */
            ptr = *work;
            int nPoints = ptr->nPoints;
            int cnt1 = ptr->cnt1;
            int cnt2 = ptr->cnt2;
            int EVindex = ptr->EVindex;
            int PerEVcnt = ptr->PerEVcnt;

            /* get current simulation time */
            double t = get_scicos_time();
            double t1 = t, t2;

            double TNm1  = ptr->workt[nPoints - 1];
            double TP    = TNm1 - 0;

            int inow;
            if (ZC == 1)
            {
                /* Zero-crossing enabled */
                if (OutEnd == 2)
                {
                    if (PerEVcnt > 0)
                    {
                        // We ran out of value and OutEnd is 2 (Repeat)
                        // Use fake time within our range.
                        t -= (PerEVcnt) * TP;
                    }
                    inow = nPoints - 1;
                }
                else
                {
                    inow = nPoints + 1; // Arbitrary value more than nPoints, will be overwritten if needed.
                }
                for (int i = cnt1 ; i < nPoints ; ++i)
                {
                    if (i == -1)
                    {
                        continue;
                    }
                    if (t <= ptr->workt[i])
                    {
                        if (t < ptr->workt[i])
                        {
                            inow = i - 1;
                        }
                        else
                        {
                            inow = i;
                        }
                        if (inow < cnt2)
                        {
                            cnt2 = inow;
                        }
                        else
                        {
                            cnt1 = cnt2;
                            cnt2 = inow;
                        }
                        break;
                    }
                }
            }
            else   /* Zero-crossing disabled */
            {
                if (OutEnd == 2)
                {
                    double r = 0;
                    if (TP != 0)
                    {
                        r = floor((t / TP));
                    }
                    t -= static_cast<int>(r) * TP;
                    inow = nPoints - 1;
                }
                else
                {
                    inow = nPoints + 1; // Arbitrary value more than nPoints, will be overwritten if needed.
                }
                // Look in time value table a range to have current time in.
                // Beware exact values.
                for (int i = 0 ; i < nPoints ; ++i)
                {
                    if (t <= ptr->workt[i])
                    {
                        if (t < ptr->workt[i])
                        {
                            inow = i - 1;
                        }
                        else
                        {
                            inow = i;
                        }
                        break;
                    }
                }
            }

            ptr->cnt1 = cnt1;
            ptr->cnt2 = cnt2;
            ptr->EVindex = EVindex;
            ptr->PerEVcnt = PerEVcnt;

            /***************************/
            /* Hypermatrix case */
            if (ptr->Hmat)
            {
                for (int j = 0; j < my * ny; ++j)
                {
                    if (ptr->Yt == 1)
                    {
                        if (ptr->Yst == 0)
                        {
                            /* real case */
                            y_d = GetRealOutPortPtrs(block, 1);
                            ptr_d = (double*) ptr->work;

                            if (inow > nPoints)
                            {
                                if (OutEnd == 0)
                                {
                                    y_d[j] = 0; /* Outputs set to zero */
                                }
                                else if (OutEnd == 1)
                                {
                                    y_d[j] = ptr_d[(nPoints - 1) * ny * my + j]; /* Hold outputs at the end */
                                }
                            }
                            else
                            {
                                if (inow < 0)
                                {
                                    y_d[j] = 0;
                                }
                                else
                                {
                                    y_d[j] = ptr_d[inow * ny * my + j];
                                }
                            }
                        }
                        else
                        {
                            /* complexe case */
                            y_d = GetRealOutPortPtrs(block, 1);
                            y_cd = GetImagOutPortPtrs(block, 1);
                            ptr_d = (double*) ptr->work;

                            if (inow > nPoints)
                            {
                                if (OutEnd == 0)
                                {
                                    y_d[j] = 0; /* Outputs set to zero */
                                    y_cd[j] = 0; /* Outputs set to zero */
                                }
                                else if (OutEnd == 1)
                                {
                                    y_d[j] = ptr_d[(nPoints - 1) * ny * my + j]; /* Hold outputs at the end */
                                    y_cd[j] = ptr_d[nPoints * my * ny + (nPoints - 1) * ny * my + j]; /* Hold outputs at the end */
                                }
                            }
                            else
                            {
                                if (inow < 0)
                                {
                                    y_d[j] = 0; /* Outputs set to zero */
                                    y_cd[j] = 0; /* Outputs set to zero */
                                }
                                else
                                {
                                    y_d[j] = ptr_d[inow * ny * my + j];
                                    y_cd[j] = ptr_d[nPoints * my * ny + inow * ny * my + j];
                                }
                            }
                        }
                    }
                    else if (ptr->Yt == 8)
                    {
                        switch (ptr->Yst)
                        {
                            case 1:
                                /* --------------------- int8 char  ----------------------------*/
                                y_c = Getint8OutPortPtrs(block, 1);
                                ptr_c = (char*) ptr->work;
                                if (inow > nPoints)
                                {
                                    if (OutEnd == 0)
                                    {
                                        y_c[j] = 0; /* Outputs set to zero */
                                    }
                                    else if (OutEnd == 1)
                                    {
                                        y_c[j] = ptr_c[(nPoints - 1) * ny * my + j]; /* Hold outputs at the end */
                                    }
                                }
                                else
                                {
                                    if (inow < 0)
                                    {
                                        y_c[j] = 0;
                                    }
                                    else
                                    {
                                        y_c[j] = ptr_c[inow * ny * my + j];
                                    }
                                }
                                break;

                            case 2:
                                /* --------------------- int16 short int ---------------------*/
                                y_s = Getint16OutPortPtrs(block, 1);
                                ptr_s = (short int*) ptr->work;
                                if (inow > nPoints)
                                {
                                    if (OutEnd == 0)
                                    {
                                        y_s[j] = 0; /* Outputs set to zero */
                                    }
                                    else if (OutEnd == 1)
                                    {
                                        y_s[j] = ptr_s[(nPoints - 1) * ny * my + j]; /* Hold outputs at the end */
                                    }
                                }
                                else
                                {
                                    if (inow < 0)
                                    {
                                        y_s[j] = 0;
                                    }
                                    else
                                    {
                                        y_s[j] = ptr_s[inow * ny * my + j];
                                    }
                                }
                                break;

                            case 4:
                                /* --------------------- int32 long ---------------------*/
                                y_l = Getint32OutPortPtrs(block, 1);
                                ptr_l = (int*) ptr->work;
                                if (inow > nPoints)
                                {
                                    if (OutEnd == 0)
                                    {
                                        y_l[j] = 0; /* Outputs set to zero */
                                    }
                                    else if (OutEnd == 1)
                                    {
                                        y_l[j] = ptr_l[(nPoints - 1) * ny * my + j]; /* Hold outputs at the end */
                                    }
                                }
                                else
                                {
                                    if (inow < 0)
                                    {
                                        y_l[j] = 0;
                                    }
                                    else
                                    {
                                        y_l[j] = ptr_l[inow * ny * my + j];
                                    }
                                }
                                break;

                            case 11:
                                /*--------------------- uint8 uchar ---------------------*/
                                y_uc = Getuint8OutPortPtrs(block, 1);
                                ptr_uc = (unsigned char*) ptr->work;
                                if (inow > nPoints)
                                {
                                    if (OutEnd == 0)
                                    {
                                        y_uc[j] = 0; /* Outputs set to zero */
                                    }
                                    else if (OutEnd == 1)
                                    {
                                        y_uc[j] = ptr_uc[(nPoints - 1) * ny * my + j]; /* Hold outputs at the end */
                                    }
                                }
                                else
                                {
                                    if (inow < 0)
                                    {
                                        y_uc[j] = 0;
                                    }
                                    else
                                    {
                                        y_uc[j] = ptr_uc[inow * ny * my + j];
                                    }
                                }
                                break;

                            case 12:
                                /* --------------------- uint16 ushort ---------------------*/
                                y_us = Getuint16OutPortPtrs(block, 1);
                                ptr_us = (unsigned short int*) ptr->work;
                                if (inow > nPoints)
                                {
                                    if (OutEnd == 0)
                                    {
                                        y_us[j] = 0; /* Outputs set to zero */
                                    }
                                    else if (OutEnd == 1)
                                    {
                                        y_us[j] = ptr_us[(nPoints - 1) * ny * my + j]; /* Hold outputs at the end */
                                    }
                                }
                                else
                                {
                                    if (inow < 0)
                                    {
                                        y_us[j] = 0;
                                    }
                                    else
                                    {
                                        y_us[j] = ptr_us[inow * ny * my + j];
                                    }
                                }
                                break;

                            case 14:
                                /* --------------------- uint32 ulong ---------------------*/
                                y_ul = Getuint32OutPortPtrs(block, 1);
                                ptr_ul = (unsigned int*) ptr->work;
                                if (inow > nPoints)
                                {
                                    if (OutEnd == 0)
                                    {
                                        y_ul[j] = 0; /* Outputs set to zero */
                                    }
                                    else if (OutEnd == 1)
                                    {
                                        y_ul[j] = ptr_ul[(nPoints - 1) * ny * my + j]; /* Hold outputs at the end */
                                    }
                                }
                                else
                                {
                                    if (inow < 0)
                                    {
                                        y_ul[j] = 0;
                                    }
                                    else
                                    {
                                        y_ul[j] = ptr_ul[inow * ny * my + j];
                                    }
                                }
                                break;
                        }
                    }
                } /* for j loop */
            }
            /****************************/
            /* Scalar or vectorial case */
            else
            {
                for (int j = 0; j < my; ++j)
                {
                    double y1, y2, d1, d2, h, dh, ddh, dddh;
                    if (ptr->Yt == 1)
                    {
                        if ((ptr->Yst == 0) || (ptr->Yst == 1))
                        {
                            /*  If real or complex*/
                            y_d = GetRealOutPortPtrs(block, 1);
                            ptr_d = (double*) ptr->work;
                            ptr_D = (double*) ptr->D;

                            if (inow > nPoints)
                            {
                                if (OutEnd == 0)
                                {
                                    y_d[j] = 0.0; /* Outputs set to zero */
                                }
                                else if (OutEnd == 1)
                                {
                                    y_d[j] = ptr_d[nPoints - 1 + (j) * nPoints]; /* Hold outputs at the end */
                                }
                            }
                            else if (Method == 0)
                            {
                                if (inow < 0)
                                {
                                    y_d[j] = 0.0;
                                }
                                else
                                {
                                    y_d[j] = ptr_d[inow + (j) * nPoints];
                                }
                            }
                            else if (Method == 1)
                            {
                                if (inow < 0)
                                {
                                    inow = 0;
                                }
                                t1 = ptr->workt[inow];
                                t2 = ptr->workt[inow + 1];
                                y1 = ptr_d[inow + j * nPoints];
                                y2 = ptr_d[inow + 1 + j * nPoints];
                                y_d[j] = (y2 - y1) * (t - t1) / (t2 - t1) + y1;
                            }
                            else if (Method >= 2)
                            {
                                t1 = ptr->workt[inow];
                                t2 = ptr->workt[inow + 1];
                                y1 = ptr_d[inow + j * nPoints];
                                y2 = ptr_d[inow + 1 + j * nPoints];
                                d1 = ptr_D[inow + j * nPoints];
                                d2 = ptr_D[inow + 1 + j * nPoints];
                                scicos_evalhermite(&t, &t1, &t2, &y1, &y2, &d1, &d2, &h, &dh, &ddh, &dddh, &inow);
                                y_d[j] = h;
                            }
                        }
                        if (ptr->Yst == 1)
                        {
                            /*  -------------- complex ----------------------*/
                            y_cd = GetImagOutPortPtrs(block, 1);
                            if (inow > nPoints)
                            {
                                if (OutEnd == 0)
                                {
                                    y_cd[j] = 0.0; /* Outputs set to zero*/
                                }
                                else if (OutEnd == 1)
                                {
                                    y_cd[j] = ptr_d[nPoints * my + nPoints - 1 + (j) * nPoints]; // Hold outputs at the end
                                }
                            }
                            else if (Method == 0)
                            {
                                if (inow < 0)
                                {
                                    y_cd[j] = 0.0; /* Outputs set to zero */
                                }
                                else
                                {
                                    y_cd[j] = ptr_d[nPoints * my + inow + (j) * nPoints];
                                }
                            }
                            else if (Method == 1)
                            {
                                if (inow < 0)
                                {
                                    inow = 0;
                                } /* Extrapolation for 0<t<X(0) */
                                t1 = ptr->workt[inow];
                                t2 = ptr->workt[inow + 1];
                                y1 = ptr_d[nPoints * my + inow + j * nPoints];
                                y2 = ptr_d[nPoints * my + inow + 1 + j * nPoints];
                                y_cd[j] = (y2 - y1) * (t - t1) / (t2 - t1) + y1;
                            }
                            else if (Method >= 2)
                            {
                                t1 = ptr->workt[inow];
                                t2 = ptr->workt[inow + 1];
                                y1 = ptr_d[inow + j * nPoints + nPoints];
                                y2 = ptr_d[inow + 1 + j * nPoints + nPoints];
                                d1 = ptr_D[inow + j * nPoints + nPoints];
                                d2 = ptr_D[inow + 1 + j * nPoints + nPoints];
                                scicos_evalhermite(&t, &t1, &t2, &y1, &y2, &d1, &d2, &h, &dh, &ddh, &dddh, &inow);
                                y_cd[j] = h;
                            }
                        }
                    }
                    else if (ptr->Yt == 8)
                    {
                        switch (ptr->Yst)
                        {
                            case 1: /* --------------------- int8 char  ----------------------------*/
                                y_c = Getint8OutPortPtrs(block, 1);
                                ptr_c = (char*) ptr->work;
                                /* y_c[j]=ptr_c[inow+(j)*nPoints]; */
                                if (inow > nPoints)
                                {
                                    if (OutEnd == 0)
                                    {
                                        y_c[j] = 0; /* Outputs set to zero */
                                    }
                                    else if (OutEnd == 1)
                                    {
                                        y_c[j] = ptr_c[nPoints - 1 + (j) * nPoints]; /* Hold outputs at the end */
                                    }
                                }
                                else if (Method == 0)
                                {
                                    if (inow < 0)
                                    {
                                        y_c[j] = 0;
                                    }
                                    else
                                    {
                                        y_c[j] = ptr_c[inow + (j) * nPoints];
                                    }
                                }
                                else if (Method >= 1)
                                {
                                    if (inow < 0)
                                    {
                                        inow = 0;
                                    }
                                    t1 = ptr->workt[inow];
                                    t2 = ptr->workt[inow + 1];
                                    y1 = (double)ptr_c[inow + j * nPoints];
                                    y2 = (double)ptr_c[inow + 1 + j * nPoints];
                                    y_c[j] = (char)((y2 - y1) * (t - t1) / (t2 - t1) + y1);
                                }
                                break;
                            case 2:
                                /* --------------------- int16 short ---------------------*/
                                y_s = Getint16OutPortPtrs(block, 1);
                                ptr_s = (short int*) ptr->work;
                                /* y_s[j]=ptr_s[inow+(j)*nPoints]; */
                                if (inow > nPoints)
                                {
                                    if (OutEnd == 0)
                                    {
                                        y_s[j] = 0; /* Outputs set to zero */
                                    }
                                    else if (OutEnd == 1)
                                    {
                                        y_s[j] = ptr_s[nPoints - 1 + (j) * nPoints]; // Hold outputs at the end
                                    }
                                }
                                else if (Method == 0)
                                {
                                    if (inow < 0)
                                    {
                                        y_s[j] = 0;
                                    }
                                    else
                                    {
                                        y_s[j] = ptr_s[inow + (j) * nPoints];
                                    }
                                }
                                else if (Method >= 1)
                                {
                                    if (inow < 0)
                                    {
                                        inow = 0;
                                    }
                                    t1 = ptr->workt[inow];
                                    t2 = ptr->workt[inow + 1];
                                    y1 = (double)ptr_s[inow + j * nPoints];
                                    y2 = (double)ptr_s[inow + 1 + j * nPoints];
                                    y_s[j] = (short int)((y2 - y1) * (t - t1) / (t2 - t1) + y1);
                                }
                                break;
                            case 4:
                                /* --------------------- int32 long ---------------------*/
                                y_l = Getint32OutPortPtrs(block, 1);
                                ptr_l = (int*) ptr->work;
                                /* y_l[j]=ptr_l[inow+(j)*nPoints]; */
                                if (inow > nPoints)
                                {
                                    if (OutEnd == 0)
                                    {
                                        y_l[j] = 0; /* Outputs set to zero */
                                    }
                                    else if (OutEnd == 1)
                                    {
                                        y_l[j] = ptr_l[nPoints - 1 + (j) * nPoints]; /* Hold outputs at the end */
                                    }
                                }
                                else if (Method == 0)
                                {
                                    if (inow < 0)
                                    {
                                        y_l[j] = 0;
                                    }
                                    else
                                    {
                                        y_l[j] = ptr_l[inow + (j) * nPoints];
                                    }
                                }
                                else if (Method >= 1)
                                {
                                    t1 = ptr->workt[inow];
                                    t2 = ptr->workt[inow + 1];
                                    y1 = (double)ptr_l[inow + j * nPoints];
                                    y2 = (double)ptr_l[inow + 1 + j * nPoints];
                                    y_l[j] = (int)((y2 - y1) * (t - t1) / (t2 - t1) + y1);
                                }
                                break;
                            case 11: /*--------------------- uint8 uchar ---------------------*/
                                y_uc = Getuint8OutPortPtrs(block, 1);
                                ptr_uc = (unsigned char*) ptr->work;
                                /* y_uc[j]=ptr_uc[inow+(j)*nPoints]; */
                                if (inow > nPoints)
                                {
                                    if (OutEnd == 0)
                                    {
                                        y_uc[j] = 0; /* Outputs set to zero */
                                    }
                                    else if (OutEnd == 1)
                                    {
                                        y_uc[j] = ptr_uc[nPoints - 1 + (j) * nPoints]; /* Hold outputs at the end */
                                    }
                                }
                                else if (Method == 0)
                                {
                                    if (inow < 0)
                                    {
                                        y_uc[j] = 0;
                                    }
                                    else
                                    {
                                        y_uc[j] = ptr_uc[inow + (j) * nPoints];
                                    }
                                }
                                else if (Method >= 1)
                                {
                                    t1 = ptr->workt[inow];
                                    t2 = ptr->workt[inow + 1];
                                    y1 = (double)ptr_uc[inow + j * nPoints];
                                    y2 = (double)ptr_uc[inow + 1 + j * nPoints];
                                    y_uc[j] = (unsigned char)((y2 - y1) * (t - t1) / (t2 - t1) + y1);
                                }
                                break;
                            case 12:
                                /* --------------------- uint16 ushort int ---------------------*/
                                y_us = Getuint16OutPortPtrs(block, 1);
                                ptr_us = (unsigned short int*) ptr->work;
                                /* y_us[j]=ptr_us[inow+(j)*nPoints]; */
                                if (inow > nPoints)
                                {
                                    if (OutEnd == 0)
                                    {
                                        y_us[j] = 0; /* Outputs set to zero */
                                    }
                                    else if (OutEnd == 1)
                                    {
                                        y_us[j] = ptr_us[nPoints - 1 + (j) * nPoints]; /* Hold outputs at the end */
                                    }
                                }
                                else if (Method == 0)
                                {
                                    if (inow < 0)
                                    {
                                        y_us[j] = 0;
                                    }
                                    else
                                    {
                                        y_us[j] = ptr_us[inow + (j) * nPoints];
                                    }
                                }
                                else if (Method >= 1)
                                {
                                    t1 = ptr->workt[inow];
                                    t2 = ptr->workt[inow + 1];
                                    y1 = (double)ptr_us[inow + j * nPoints];
                                    y2 = (double)ptr_us[inow + 1 + j * nPoints];
                                    y_us[j] = (unsigned short int)((y2 - y1) * (t - t1) / (t2 - t1) + y1);
                                }
                                break;
                            case 14:
                                /* --------------------- uint32 ulong ---------------------*/
                                y_ul = Getuint32OutPortPtrs(block, 1);
                                ptr_ul = (unsigned int*) ptr->work;
                                /* y_ul[j]=ptr_ul[inow+(j)*nPoints]; */
                                if (inow > nPoints)
                                {
                                    if (OutEnd == 0)
                                    {
                                        y_ul[j] = 0; /* Outputs set to zero */
                                    }
                                    else if (OutEnd == 1)
                                    {
                                        y_ul[j] = ptr_ul[nPoints - 1 + (j) * nPoints]; /* Hold outputs at the end */
                                    }
                                }
                                else if (Method == 0)
                                {
                                    if (inow < 0)
                                    {
                                        y_ul[j] = 0;
                                    }
                                    else
                                    {
                                        y_ul[j] = ptr_ul[inow + (j) * nPoints];
                                    }
                                }
                                else if (Method >= 1)
                                {
                                    t1 = ptr->workt[inow];
                                    t2 = ptr->workt[inow + 1];
                                    y1 = (double)ptr_ul[inow + j * nPoints];
                                    y2 = (double)ptr_ul[inow + 1 + j * nPoints];
                                    y_ul[j] = (unsigned int)((y2 - y1) * (t - t1) / (t2 - t1) + y1);
                                }
                                break;
                        }
                    }
                } /* for j loop */
            }
            /********************************************************************/
            break;
        }
        case 3 :
        {
            /* Event date computation */
            /* Retrieve 'ptr' of the structure of the block */
            ptr = *work;
            int nPoints = ptr->nPoints;
            int cnt1 = ptr->cnt1;
            int cnt2 = ptr->cnt2;
            int EVindex = ptr->EVindex;
            int PerEVcnt = ptr->PerEVcnt;

            /* Get current simulation time */
            //double t = get_scicos_time();

            double TNm1  = ptr->workt[nPoints - 1];
            double TP    = TNm1 - 0;

            int jfirst;
            if (ZC == 1)
            {
                /* Generate Events only if ZC is active */
                if ((Method == 1) || (Method == 0))
                {
                    /*-------------------------*/
                    if (ptr->firstevent == 1)
                    {
                        jfirst = nPoints - 1; /* Finding first positive time instant */
                        for (int j = 0; j < nPoints; ++j)
                        {
                            if (ptr->workt[j] > 0)
                            {
                                jfirst = j;
                                break;
                            }
                        }
                        block->evout[0] = ptr->workt[jfirst];
                        EVindex = jfirst;
                        ptr->EVindex = EVindex;
                        ptr->firstevent = 0;
                        return;
                    }
                    /*------------------------*/
                    int i = EVindex;
                    /*------------------------*/
                    if (i < nPoints - 1)
                    {
                        block->evout[0] = ptr->workt[i + 1] - ptr->workt[i];
                        EVindex = i + 1;
                    }
                    /*------------------------*/
                    if (i == nPoints - 1)
                    {
                        if (OutEnd == 2)
                        {
                            /*  Periodic */
                            cnt1 = -1;
                            cnt2 = 0;
                            PerEVcnt++; /* When OutEnd==2 (perodic output) */
                            jfirst = nPoints - 1; /* Finding first positive time instant */
                            for (int j = 0; j < nPoints; ++j)
                            {
                                if (ptr->workt[j] >= 0)
                                {
                                    jfirst = j;
                                    break;
                                }
                            }
                            block->evout[0] = ptr->workt[jfirst];
                            EVindex = jfirst;
                        }
                    }
                    /*-------------------------- */
                }
                else if (Method <= 3)
                {
                    if (ptr->firstevent == 1)
                    {
                        block->evout[0] = TP;
                        ptr->firstevent = 0;
                    }
                    else
                    {
                        if (OutEnd == 2)
                        {
                            block->evout[0] = TP;
                        }
                        PerEVcnt++;
                    }
                    cnt1 = -1;
                    cnt2 = 0;
                }
                ptr->cnt1 = cnt1;
                ptr->cnt2 = cnt2;
                ptr->EVindex = EVindex;
                ptr->PerEVcnt = PerEVcnt;
            }
            /***********************************************************************/
            break;
        }
        case 5 :
        {
            /* Finish */
            ptr = *work;
            if (ptr != nullptr)
            {
                if (ptr->D != nullptr)
                {
                    delete[] ptr->D;
                }
                if (ptr->work != nullptr)
                {
                    delete[] ptr->work;
                }
                if (ptr->workt != nullptr)
                {
                    delete[] ptr->workt;
                }
                delete[] ptr;
            }
            break;
            /***********************************************************************/
        }
        case 6 :
        {
            /* Re-init */

            // Finish then init
            fromws_c(block, 5);
            fromws_c(block, 4);
            break;
        }
        /*************************************************************************/
        default :
            return;
    }
}
