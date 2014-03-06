/*  Scicos
   *
   *  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
   *
   * This program is free software; you can redistribute it and/or modify
   * it under the terms of the GNU General Public License as published by
   * the Free Software Foundation; either version 2 of the License, or
   * (at your option) any later version.
   *
   * This program is distributed in the hope that it will be useful,
   * but WITHOUT ANY WARRANTY; without even the implied warranty of
   * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   * GNU General Public License for more details.
   *
   * You should have received a copy of the GNU General Public License
   * along with this program; if not, write to the Free Software
   * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
   *
   * See the file ./license.txt
   */
/*--------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "stack-c.h"
#include "scicos.h"
#include "scicos_print.h"
#include "cvstr.h"
#include "MALLOC.h"
#include "mopen.h"
#include "mget.h"
#include "mclose.h"
#include "expandPathVariable.h"
#include "scicos_block4.h"
#include "scicos_evalhermite.h"
#include "localization.h"
#include "scicos_malloc.h"
#include "scicos_free.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/
#define Fnlength  block->ipar[0]
#define FName     block->ipar[1]
#define Method     block->ipar[1+Fnlength]
#define ZC        block->ipar[2+Fnlength]
#define OutEnd    block->ipar[3+Fnlength]
#define T0        ptr->workt[0]
#define TNm1      ptr->workt[nPoints-1]
#define TP        (TNm1-0)
/*--------------------------------------------------------------------------*/
static int Mytridiagldltsolve(double *dA, double * lA, double * B, int N);
/*--------------------------------------------------------------------------*/
/* function to check and extract data coming from an hypermat */
static int Ishm(int *fd, int *Ytype, int *nPoints, int *my, int *ny, int *YsubType);
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
#ifdef hppa
#undef FILENAME_MAX
#define FILENAME_MAX 4096
#endif
/*--------------------------------------------------------------------------*/
/* work struct for that block */
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
    double *D;
    void *work;
    double *workt;
} fromwork_struct ;
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP void fromws_c(scicos_block *block, int flag)
{
    double t = 0., y1 = 0., y2 = 0., t1 = 0., t2 = 0., r = 0.;
    double *spline = NULL, *A_d = NULL, *A_sd = NULL, *qdy = NULL;
    /* double  a,b,c,*y;*/
    double d1 = 0., d2 = 0., h = 0., dh = 0., ddh = 0., dddh = 0.;
    /* counter and indexes variables */
    int i = 0, inow = 0;
    int j = 0, jfirst = 0;
    int cnt1 = 0, cnt2 = 0, EVindex = 0, PerEVcnt = 0;

    /* variables to handle files of TMPDIR/Workspace */
    int fd = 0;
    char *status = NULL;
    int swap = 1;
    double res = 0.;
    char *filename = NULL;
    char str[100];
    int ierr = 1;

    /* variables for type and dims of data coming from scilab */
    int Ytype = 0, YsubType = 0, mY = 0, nY = 0;
    int nPoints = 0;
    int Ydim[10];

    /* variables for type and dims of data of the output port block */
    int ytype = 0, my = 0, ny = 0;

    /* generic pointer */
    SCSREAL_COP *y_d = NULL, *y_cd = NULL, *ptr_d = NULL, *ptr_T = NULL, *ptr_D = NULL;
    SCSINT8_COP *y_c = NULL, *ptr_c = NULL;
    SCSUINT8_COP *y_uc = NULL, *ptr_uc = NULL;
    SCSINT16_COP *y_s = NULL, *ptr_s = NULL;
    SCSUINT16_COP *y_us = NULL, *ptr_us = NULL;
    SCSINT32_COP *y_l = NULL, *ptr_l = NULL;
    SCSUINT32_COP *y_ul = NULL, *ptr_ul = NULL;

    /* the struct ptr of that block */
    fromwork_struct** work = (fromwork_struct**) block->work;
    fromwork_struct *ptr = NULL;

    /* for path of TMPDIR/workspace */
    char filePrefix[] = "TMPDIR/Workspace/";
    char* env;

    /*retrieve dimensions of output port*/
    my       = GetOutPortRows(block, 1); /* number of rows of Outputs*/
    ny       = GetOutPortCols(block, 1); /* number of cols of Outputs*/
    ytype    = GetOutType(block, 1);    /* output type */

    /* init */
    if (flag == 4)
    {
        /* convert scilab code of the variable name to C string */
        C2F(cvstr)(&(Fnlength), &(FName), str, (j = 1, &j), sizeof(str));
        str[Fnlength] = '\0';

        /* retrieve path of TMPDIR/workspace */
        env = (char*) scicos_malloc(sizeof(filePrefix) + Fnlength);
        if (env == NULL)
        {
            set_block_error(-16);
            return;
        }
        memcpy(env, filePrefix, sizeof(filePrefix));
        memcpy(env + (sizeof(filePrefix) - 1), str, Fnlength + 1);

        /* open tmp file */
        /* "r" : read */
        /* "b" : binary format (required for Windows) */
        status = "rb";

        filename = expandPathVariable(env);
        scicos_free(env);
        if (filename)
        {
            C2F(mopen)(&fd, filename, status, &swap, &res, &ierr);
        }
        FREE(filename);
        filename = NULL;

        if (ierr != 0)
        {
            Coserror(_("The '%s' variable does not exist.\n"), str);
            return;
        }

        /* read x */
        C2F(mgetnc) (&fd, &Ydim[0], (j = nsiz, &j), fmti, &ierr); /* read sci id */
        C2F(mgetnc) (&fd, &Ydim[6], (j = 1, &j), fmti, &ierr);  /* read sci type */
        if (Ydim[6] == 17)
        {
            if (!Ishm(&fd, &Ytype, &nPoints, &mY, &nY, &YsubType))
            {
                Coserror(_("Invalid variable type.\n"));
                /*scicos_print(_("Invalid variable type.\n"));
                set_block_error(-3); */
                C2F(mclose)(&fd, &res);
                return;
            }
            if (!((Ytype == 1) || (Ytype == 8)))
            {
                Coserror(_("Invalid variable type.\n"));
                /*scicos_print(_("Invalid variable type.\n"));
                set_block_error(-3);*/
                C2F(mclose)(&fd, &res);
                return;
            }
        }
        else if ((Ydim[6] == 1) || (Ydim[6] == 8))
        {
            C2F(mgetnc) (&fd, &Ydim[7], (j = 3, &j), fmti, &ierr); /* read sci header */
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
            C2F(mclose)(&fd, &res);
            return;
        }

        /* check dimension for output port and variable */
        if ((mY != my) || (nY != ny))
        {
            Coserror(_("Data dimensions are inconsistent:\n Variable size=[%d,%d] \nBlock output size=[%d,%d].\n"), mY, nY, my, ny);
            /*set_block_error(-3);*/
            C2F(mclose)(&fd, &res);
            return;
        }

        /* check variable data type and output block data type */
        if (Ytype == 1)
        {
            /*real/complex cases*/
            switch (YsubType)
            {
                case 0:
                    if (ytype != 10)
                    {
                        Coserror(_("Output should be of Real type.\n"));
                        /*set_block_error(-3);*/
                        C2F(mclose)(&fd, &res);
                        return;
                    }
                    break;

                case 1:
                    if (ytype != 11)
                    {
                        Coserror(_("Output should be of complex type.\n"));
                        /*set_block_error(-3);*/
                        C2F(mclose)(&fd, &res);
                        return;
                    }
                    break;
            }
        }
        else if (Ytype == 8)
        {
            /*int cases*/
            switch (YsubType)
            {
                case 1:
                    if (ytype != 81)
                    {
                        scicos_print(_("Output should be of int8 type.\n"));
                        set_block_error(-3);
                        C2F(mclose)(&fd, &res);
                        return;
                    }
                    break;

                case 2:
                    if (ytype != 82)
                    {
                        Coserror(_("Output should be of int16 type.\n"));
                        /*set_block_error(-3);*/
                        C2F(mclose)(&fd, &res);
                        return;
                    }
                    break;

                case 4:
                    if (ytype != 84)
                    {
                        Coserror(_("Output should be of int32 type.\n"));
                        /*set_block_error(-3);*/
                        C2F(mclose)(&fd, &res);
                        return;
                    }
                    break;

                case 11:
                    if (ytype != 811)
                    {
                        Coserror(_("Output should be of uint8 type.\n"));
                        /*set_block_error(-3);*/
                        C2F(mclose)(&fd, &res);
                        return;
                    }
                    break;

                case 12:
                    if (ytype != 812)
                    {
                        Coserror(_("Output should be of uint16 type.\n"));
                        /*set_block_error(-3);*/
                        C2F(mclose)(&fd, &res);
                        return;
                    }
                    break;

                case 14:
                    if (ytype != 814)
                    {
                        Coserror(_("Output should be of uint32 type.\n"));
                        /*set_block_error(-3);*/
                        C2F(mclose)(&fd, &res);
                        return;
                    }
                    break;
            }
        }

        /* allocation of the work structure of that block */
        if ((*work = (fromwork_struct*) scicos_malloc(sizeof(fromwork_struct))) == NULL)
        {
            set_block_error(-16);
            C2F(mclose)(&fd, &res);
            return;
        }
        ptr = *work;
        ptr->D = NULL;
        ptr->workt = NULL;
        ptr->work = NULL;

        if (Ytype == 1)
        {
            /*real/complex case*/
            switch (YsubType)
            {
                case 0 : /* Real */
                    if ((ptr->work = (void *) scicos_malloc(nPoints * mY * nY * sizeof(double))) == NULL)
                    {
                        set_block_error(-16);
                        scicos_free(ptr);
                        *(block->work) = NULL;
                        C2F(mclose)(&fd, &res);
                        return;
                    }
                    ptr_d = (SCSREAL_COP *) ptr->work;
                    C2F(mgetnc) (&fd, ptr_d, (j = nPoints * mY * nY, &j), fmtd, &ierr); /* read double data */
                    break;
                case 1:  /* complex */
                    if ((ptr->work = (void *) scicos_malloc(2 * nPoints * mY * nY * sizeof(double))) == NULL)
                    {
                        set_block_error(-16);
                        scicos_free(ptr);
                        *(block->work) = NULL;
                        C2F(mclose)(&fd, &res);
                        return;
                    }
                    ptr_d = (SCSREAL_COP *) ptr->work;
                    C2F(mgetnc) (&fd, ptr_d, (j = 2 * nPoints * mY * nY, &j), fmtd, &ierr); /* read double data */
                    break;
            }
        }
        else if (Ytype == 8)
        {
            /*int case*/
            switch (YsubType)
            {
                case 1 :/* int8 */
                    if ((ptr->work = (void *) scicos_malloc(nPoints * mY * nY * sizeof(char))) == NULL)
                    {
                        set_block_error(-16);
                        scicos_free(ptr);
                        *(block->work) = NULL;
                        C2F(mclose)(&fd, &res);
                        return;
                    }
                    ptr_c = (SCSINT8_COP *) ptr->work;
                    C2F(mgetnc) (&fd, ptr_c, (j = nPoints * mY * nY, &j), fmtc, &ierr); /* read char data */
                    break;
                case 2 :  /* int16 */
                    if ((ptr->work = (void *) scicos_malloc(nPoints * mY * nY * sizeof(short))) == NULL)
                    {
                        set_block_error(-16);
                        scicos_free(ptr);
                        *(block->work) = NULL;
                        C2F(mclose)(&fd, &res);
                        return;
                    }
                    ptr_s = (SCSINT16_COP *) ptr->work;
                    C2F(mgetnc) (&fd, ptr_s, (j = nPoints * mY * nY, &j), fmts, &ierr); /* read short data */
                    break;
                case 4 :   /* int32 */
                    if ((ptr->work = (void *) scicos_malloc(nPoints * mY * nY * sizeof(SCSINT32_COP))) == NULL)
                    {
                        set_block_error(-16);
                        scicos_free(ptr);
                        *(block->work) = NULL;
                        C2F(mclose)(&fd, &res);
                        return;
                    }
                    ptr_l = (SCSINT32_COP *) ptr->work;
                    C2F(mgetnc) (&fd, ptr_l, (j = nPoints * mY * nY, &j), fmtl, &ierr); /* read short data */
                    break;
                case 11 :   /* uint8 */
                    if ((ptr->work = (void *) scicos_malloc(nPoints * mY * nY * sizeof(unsigned char))) == NULL)
                    {
                        set_block_error(-16);
                        scicos_free(ptr);
                        *(block->work) = NULL;
                        C2F(mclose)(&fd, &res);
                        return;
                    }
                    ptr_uc = (SCSUINT8_COP *) ptr->work;
                    C2F(mgetnc) (&fd, ptr_uc, (j = nPoints * mY * nY, &j), fmtuc, &ierr); /* read short data */
                    break;
                case 12 : /* uint16 */
                    if ((ptr->work = (void *) scicos_malloc(nPoints * mY * nY * sizeof(unsigned short))) == NULL)
                    {
                        set_block_error(-16);
                        scicos_free(ptr);
                        *(block->work) = NULL;
                        C2F(mclose)(&fd, &res);
                        return;
                    }
                    ptr_us = (SCSUINT16_COP *) ptr->work;
                    C2F(mgetnc) (&fd, ptr_us, (j = nPoints * mY * nY, &j), fmtus, &ierr); /* read short data */
                    break;
                case 14 :  /* uint32 */
                    if ((ptr->work = (void *) scicos_malloc(nPoints * mY * nY * sizeof(SCSUINT32_COP))) == NULL)
                    {
                        set_block_error(-16);
                        scicos_free(ptr);
                        *(block->work) = NULL;
                        C2F(mclose)(&fd, &res);
                        return;
                    }
                    ptr_ul = (SCSUINT32_COP *) ptr->work;
                    C2F(mgetnc) (&fd, ptr_ul, (j = nPoints * mY * nY, &j), fmtul, &ierr); /* read short data */
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

        /* read t */
        C2F(mgetnc) (&fd, &Ydim[0], (j = nsiz, &j), fmti, &ierr); /* read sci id */
        C2F(mgetnc) (&fd, &Ydim[6], (j = 1, &j), fmti, &ierr); /* read sci type */
        C2F(mgetnc) (&fd, &Ydim[7], (j = 3, &j), fmti, &ierr); /* read sci header */

        if (nPoints != Ydim[7])
        {
            Coserror(_("The Time vector has a wrong size, expecting [%d, %d] and getting [%d, %d].\n"), nPoints, 1, Ydim[7], Ydim[8]);
            /*set_block_error(-3);*/
            *work = NULL;
            scicos_free(ptr->work);
            scicos_free(ptr);
            C2F(mclose)(&fd, &res);
            return;
        }

        if ((Ydim[6] != 1) | (Ydim[9] != 0))
        {
            scicos_print(_("The Time vector type is not ""double"".\n"));
            set_block_error(-3);
            *work = NULL;
            scicos_free(ptr->work);
            scicos_free(ptr);
            C2F(mclose)(&fd, &res);
            return;
        }

        if ((ptr->workt = (double *) scicos_malloc(nPoints * sizeof(double))) == NULL)
        {
            set_block_error(-16);
            *work = NULL;
            scicos_free(ptr->work);
            scicos_free(ptr);
            C2F(mclose)(&fd, &res);
            return;
        }
        ptr_T = (SCSREAL_COP *) ptr->workt;
        C2F(mgetnc) (&fd, ptr_T, (j = nPoints, &j), fmtd, &ierr); /* read data of t */

        /* close the file*/
        C2F(mclose)(&fd, &res);

        /*================================*/
        /* check for an increasing time data */
        for (j = 0; j < nPoints - 1; j++)
        {
            if (ptr_T[j] > ptr_T[j + 1])
            {
                Coserror(_("The time vector should be an increasing vector.\n"));
                /*set_block_error(-3);*/
                *work = NULL;
                scicos_free(ptr->workt);
                scicos_free(ptr->work);
                scicos_free(ptr);
                return;
            }
        }
        /*=================================*/
        if ((Method > 1) && (Ytype == 1) && (!ptr->Hmat))
        {
            /* double or complex */
            if (YsubType == 0) /*real*/
            {
                if ((ptr->D = (double *) scicos_malloc(nPoints * mY * sizeof(double))) == NULL)
                {
                    set_block_error(-16);
                    *work = NULL;
                    scicos_free(ptr->workt);
                    scicos_free(ptr->work);
                    scicos_free(ptr);
                    return;
                }
            }
            else
            {
                /*complex*/
                if ((ptr->D = (double *) scicos_malloc(2 * nPoints * mY * sizeof(double))) == NULL)
                {
                    set_block_error(-16);
                    *work = NULL;
                    scicos_free(ptr->workt);
                    scicos_free(ptr->work);
                    scicos_free(ptr);
                    return;
                }
            }

            if ((spline = (double *) scicos_malloc((3 * nPoints - 2) * sizeof(double))) == NULL)
            {
                Coserror(_("Allocation problem in spline.\n"));
                /*set_block_error(-16);*/
                *work = NULL;
                scicos_free(ptr->D);
                scicos_free(ptr->workt);
                scicos_free(ptr->work);
                scicos_free(ptr);
                return;
            }

            A_d  = spline;
            A_sd = A_d  + nPoints;
            qdy  = A_sd + nPoints - 1;

            for (j = 0; j < mY; j++)
            {
                /* real part */
                for (i = 0; i <= nPoints - 2; i++)
                {
                    A_sd[i] = 1.0 / (ptr_T[i + 1] - ptr_T[i]);
                    qdy[i]  = (ptr_d[i + 1 + j * nPoints] - ptr_d[i + j * nPoints]) * A_sd[i] * A_sd[i];
                }

                for (i = 1; i <= nPoints - 2; i++)
                {
                    A_d[i] = 2.0 * (A_sd[i - 1] + A_sd[i]);
                    ptr->D[i + j * nPoints] = 3.0 * (qdy[i - 1] + qdy[i]);
                }

                if (Method == 2)
                {
                    A_d[0] =  2.0 * A_sd[0];
                    ptr->D[0 + j * nPoints] = 3.0 * qdy[0];
                    A_d[nPoints - 1] =  2.0 * A_sd[nPoints - 2];
                    ptr->D[nPoints - 1 + j * nPoints] =  3.0 * qdy[nPoints - 2];
                    Mytridiagldltsolve(A_d, A_sd, &ptr->D[j * nPoints], nPoints);
                }

                if (Method == 3)
                {
                    /*  s'''(x(2)-) = s'''(x(2)+) */
                    r = A_sd[1] / A_sd[0];
                    A_d[0] = A_sd[0] / (1.0 + r);
                    ptr->D[j * nPoints] = ((3.0 * r + 2.0) * qdy[0] + r * qdy[1]) / ((1.0 + r) * (1.0 + r));
                    /*  s'''(x(n-1)-) = s'''(x(n-1)+) */
                    r = A_sd[nPoints - 3] / A_sd[nPoints - 2];
                    A_d[nPoints - 1] = A_sd[nPoints - 2] / (1.0 + r);
                    ptr->D[nPoints - 1 + j * nPoints] = \
                                                        ((3.0 * r + 2.0) * qdy[nPoints - 2] + r * qdy[nPoints - 3]) / ((1.0 + r) * (1.0 + r));
                    Mytridiagldltsolve(A_d, A_sd, &ptr->D[j * nPoints], nPoints);
                }
            }

            if (YsubType == 1)
            {
                /* imag part */
                for (j = 0; j < mY; j++)
                {
                    for (i = 0; i <= nPoints - 2; i++)
                    {
                        A_sd[i] = 1.0 / (ptr_T[i + 1] - ptr_T[i]);
                        qdy[i]  = (ptr_d[nPoints + i + 1 + j * nPoints] - ptr_d[nPoints + i + j * nPoints]) * A_sd[i] * A_sd[i];
                    }

                    for (i = 1; i <= nPoints - 2; i++)
                    {
                        A_d[i] = 2.0 * (A_sd[i - 1] + A_sd[i]);
                        ptr->D[i + j * nPoints + nPoints] = 3.0 * (qdy[i - 1] + qdy[i]);
                    }

                    if (Method == 2)
                    {
                        A_d[0] =  2.0 * A_sd[0];
                        ptr->D[nPoints + 0 + j * nPoints] = 3.0 * qdy[0];
                        A_d[nPoints - 1] =  2.0 * A_sd[nPoints - 2];
                        ptr->D[nPoints + nPoints - 1 + j * nPoints] =  3.0 * qdy[nPoints - 2];
                        Mytridiagldltsolve(A_d, A_sd, &ptr->D[nPoints + j * nPoints], nPoints);
                    }

                    if (Method == 3)
                    {
                        /*  s'''(x(2)-) = s'''(x(2)+) */
                        r = A_sd[1] / A_sd[0];
                        A_d[0] = A_sd[0] / (1.0 + r);
                        ptr->D[nPoints + j * nPoints] = ((3.0 * r + 2.0) * qdy[0] + r * qdy[1]) / ((1.0 + r) * (1.0 + r));
                        /*  s'''(x(n-1)-) = s'''(x(n-1)+) */
                        r = A_sd[nPoints - 3] / A_sd[nPoints - 2];
                        A_d[nPoints - 1] = A_sd[nPoints - 2] / (1.0 + r);
                        ptr->D[nPoints + nPoints - 1 + j * nPoints] = \
                                ((3.0 * r + 2.0) * qdy[nPoints - 2] + r * qdy[nPoints - 3]) / ((1.0 + r) * (1.0 + r));
                        Mytridiagldltsolve(A_d, A_sd, &ptr->D[nPoints + j * nPoints], nPoints);
                    }
                }
            }

            scicos_free(spline);
        }
        /*===================================*/
        cnt1 = nPoints - 1;
        cnt2 = nPoints;
        for (i = 0; i < nPoints; i++)
        {
            /* finding the first positive time instant */
            if (ptr->workt[i] >= 0 )
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
        return;
        /*******************************************************/
        /*******************************************************/
    }
    else if (flag == 1)
    {
        /* output computation */



        /* retrieve ptr of the structure of that block */
        ptr = *work;
        nPoints = ptr->nPoints;
        cnt1 = ptr->cnt1;
        cnt2 = ptr->cnt2;
        EVindex = ptr->EVindex;
        PerEVcnt = ptr->PerEVcnt;

        /* get current simulation time */
        t = get_scicos_time();
        t1 = t;

        if (ZC == 1)
        {
            /*zero crossing enable*/
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
            for (i = cnt1 ; i < nPoints ; i++)
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
        else   /*zero crossing disable*/
        {
            if (OutEnd == 2)
            {
                if (TP != 0)
                {
                    r = floor((t / TP));
                }
                else
                {
                    r = 0;
                }
                t -= ((int)r) * TP;
                inow = nPoints - 1;
            }
            else
            {
                inow = nPoints + 1; // Arbitrary valuemore than nPoints, will be overwritten if needed.
            }
            // Look in time value table a range to have current time in.
            // take care about exact values.
            for (i = 0 ; i < nPoints ; i++)
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
        /* hypermatrix case */
        if (ptr->Hmat)
        {

            for (j = 0; j < my * ny; j++)
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
                                y_d[j] = 0.0; /* outputs set to zero */
                            }
                            else if (OutEnd == 1)
                            {
                                y_d[j] = ptr_d[(nPoints - 1) * ny * my + j]; /* hold outputs at the end */
                            }
                        }
                        else
                        {
                            if (inow < 0)
                            {
                                y_d[j] = 0.0;
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
                                y_d[j] = 0.0; /* outputs set to zero */
                                y_cd[j] = 0.0; /* outputs set to zero */
                            }
                            else if (OutEnd == 1)
                            {
                                y_d[j] = ptr_d[(nPoints - 1) * ny * my + j]; /* hold outputs at the end */
                                y_cd[j] = ptr_d[nPoints * my * ny + (nPoints - 1) * ny * my + j]; /* hold outputs at the end */
                            }
                        }
                        else
                        {
                            if (inow < 0)
                            {
                                y_d[j] = 0.0; /* outputs set to zero */
                                y_cd[j] = 0.0; /* outputs set to zero */
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
                            /* ---------------------int8 char  ---------------------------- */
                            y_c = Getint8OutPortPtrs(block, 1);
                            ptr_c = (char*) ptr->work;
                            if (inow > nPoints)
                            {
                                if (OutEnd == 0)
                                {
                                    y_c[j] = 0; /* outputs set to zero */
                                }
                                else if (OutEnd == 1)
                                {
                                    y_c[j] = ptr_c[(nPoints - 1) * ny * my + j]; /* hold outputs at the end */
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
                            /* ---------------------int16 short--------------------- */
                            y_s = Getint16OutPortPtrs(block, 1);
                            ptr_s = (short*) ptr->work;
                            if (inow > nPoints)
                            {
                                if (OutEnd == 0)
                                {
                                    y_s[j] = 0; /* outputs set to zero */
                                }
                                else if (OutEnd == 1)
                                {
                                    y_s[j] = ptr_s[(nPoints - 1) * ny * my + j]; /* hold outputs at the end */
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
                            /* ---------------------int32 long--------------------- */
                            y_l = Getint32OutPortPtrs(block, 1);
                            ptr_l = (SCSINT32_COP *) ptr->work;
                            if (inow > nPoints)
                            {
                                if (OutEnd == 0)
                                {
                                    y_l[j] = 0; /* outputs set to zero */
                                }
                                else if (OutEnd == 1)
                                {
                                    y_l[j] = ptr_l[(nPoints - 1) * ny * my + j]; /* hold outputs at the end */
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
                            /*--------------------- uint8 uchar---------------------*/
                            y_uc = Getuint8OutPortPtrs(block, 1);
                            ptr_uc = (unsigned char*) ptr->work;
                            if (inow > nPoints)
                            {
                                if (OutEnd == 0)
                                {
                                    y_uc[j] = 0; /* outputs set to zero */
                                }
                                else if (OutEnd == 1)
                                {
                                    y_uc[j] = ptr_uc[(nPoints - 1) * ny * my + j]; /* hold outputs at the end */
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
                            /* ---------------------uint16 ushort--------------------- */
                            y_us = Getuint16OutPortPtrs(block, 1);
                            ptr_us = (unsigned short*) ptr->work;
                            if (inow > nPoints)
                            {
                                if (OutEnd == 0)
                                {
                                    y_us[j] = 0; /* outputs set to zero */
                                }
                                else if (OutEnd == 1)
                                {
                                    y_us[j] = ptr_us[(nPoints - 1) * ny * my + j]; /* hold outputs at the end */
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
                            /* ---------------------uint32 ulong--------------------- */
                            y_ul = Getuint32OutPortPtrs(block, 1);
                            ptr_ul = (SCSUINT32_COP *) ptr->work;
                            if (inow > nPoints)
                            {
                                if (OutEnd == 0)
                                {
                                    y_ul[j] = 0; /* outputs set to zero */
                                }
                                else if (OutEnd == 1)
                                {
                                    y_ul[j] = ptr_ul[(nPoints - 1) * ny * my + j]; /* hold outputs at the end */
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
        /* scalar of vectorial case */
        else
        {
            for (j = 0; j < my; j++)
            {
                if (ptr->Yt == 1)
                {
                    if ((ptr->Yst == 0) || (ptr->Yst == 1))
                    {
                        /*  if Real or complex*/
                        y_d = GetRealOutPortPtrs(block, 1);
                        ptr_d = (double*) ptr->work;
                        ptr_D = (double*) ptr->D;

                        if (inow > nPoints)
                        {
                            if (OutEnd == 0)
                            {
                                y_d[j] = 0.0; /* outputs set to zero */
                            }
                            else if (OutEnd == 1)
                            {
                                y_d[j] = ptr_d[nPoints - 1 + (j) * nPoints]; /* hold outputs at the end */
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
                        /*  --------------complex----------------------*/
                        y_cd = GetImagOutPortPtrs(block, 1);
                        if (inow > nPoints)
                        {
                            if (OutEnd == 0)
                            {
                                y_cd[j] = 0.0; /* outputs set to zero*/
                            }
                            else if (OutEnd == 1)
                            {
                                y_cd[j] = ptr_d[nPoints * my + nPoints - 1 + (j) * nPoints]; // hold outputs at the end
                            }
                        }
                        else if (Method == 0)
                        {
                            if (inow < 0)
                            {
                                y_cd[j] = 0.0; /* outputs set to zero */
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
                            } /* extrapolation for 0<t<X(0) */
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
                        case 1: /* ---------------------int8 char  ---------------------------- */
                            y_c = Getint8OutPortPtrs(block, 1);
                            ptr_c = (char*) ptr->work;
                            /*y_c[j]=ptr_c[inow+(j)*nPoints];*/
                            if (inow > nPoints)
                            {
                                if (OutEnd == 0)
                                {
                                    y_c[j] = 0; /* outputs set to zero */
                                }
                                else if (OutEnd == 1)
                                {
                                    y_c[j] = ptr_c[nPoints - 1 + (j) * nPoints]; /* hold outputs at the end */
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
                            /* ---------------------int16 short--------------------- */
                            y_s = Getint16OutPortPtrs(block, 1);
                            ptr_s = (short*) ptr->work;
                            /* y_s[j]=ptr_s[inow+(j)*nPoints]; */
                            if (inow > nPoints)
                            {
                                if (OutEnd == 0)
                                {
                                    y_s[j] = 0; /* outputs set to zero */
                                }
                                else if (OutEnd == 1)
                                {
                                    y_s[j] = ptr_s[nPoints - 1 + (j) * nPoints]; // hold outputs at the end
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
                                y_s[j] = (short)((y2 - y1) * (t - t1) / (t2 - t1) + y1);
                            }
                            break;
                        case 4:
                            /* ---------------------int32 long--------------------- */
                            y_l = Getint32OutPortPtrs(block, 1);
                            ptr_l = (SCSINT32_COP *) ptr->work;
                            /*y_l[j]=ptr_l[inow+(j)*nPoints];*/
                            if (inow > nPoints)
                            {
                                if (OutEnd == 0)
                                {
                                    y_l[j] = 0; /* outputs set to zero */
                                }
                                else if (OutEnd == 1)
                                {
                                    y_l[j] = ptr_l[nPoints - 1 + (j) * nPoints]; /* hold outputs at the end */
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
                                y_l[j] = (SCSINT32_COP)((y2 - y1) * (t - t1) / (t2 - t1) + y1);
                            }
                            break;
                        case 11: /*--------------------- uint8 uchar---------------------*/
                            y_uc = Getuint8OutPortPtrs(block, 1);
                            ptr_uc = (unsigned char*) ptr->work;
                            /*y_uc[j]=ptr_uc[inow+(j)*nPoints];*/
                            if (inow > nPoints)
                            {
                                if (OutEnd == 0)
                                {
                                    y_uc[j] = 0; /* outputs set to zero */
                                }
                                else if (OutEnd == 1)
                                {
                                    y_uc[j] = ptr_uc[nPoints - 1 + (j) * nPoints]; /* hold outputs at the end */
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
                            /* ---------------------uint16 ushort--------------------- */
                            y_us = Getuint16OutPortPtrs(block, 1);
                            ptr_us = (unsigned short*) ptr->work;
                            /* y_us[j]=ptr_us[inow+(j)*nPoints]; */
                            if (inow > nPoints)
                            {
                                if (OutEnd == 0)
                                {
                                    y_us[j] = 0; /* outputs set to zero */
                                }
                                else if (OutEnd == 1)
                                {
                                    y_us[j] = ptr_us[nPoints - 1 + (j) * nPoints]; /* hold outputs at the end */
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
                                y_us[j] = (unsigned short)((y2 - y1) * (t - t1) / (t2 - t1) + y1);
                            }
                            break;
                        case 14:
                            /* ---------------------uint32 ulong--------------------- */
                            y_ul = Getuint32OutPortPtrs(block, 1);
                            ptr_ul = (SCSUINT32_COP *) ptr->work;
                            /* y_ul[j]=ptr_ul[inow+(j)*nPoints]; */
                            if (inow > nPoints)
                            {
                                if (OutEnd == 0)
                                {
                                    y_ul[j] = 0; /* outputs set to zero */
                                }
                                else if (OutEnd == 1)
                                {
                                    y_ul[j] = ptr_ul[nPoints - 1 + (j) * nPoints]; /* hold outputs at the end */
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
                                y_ul[j] = (SCSUINT32_COP)((y2 - y1) * (t - t1) / (t2 - t1) + y1);
                            }
                            break;
                    }
                }
            } /* for j loop */
        }
        /********************************************************************/
    }
    else if (flag == 3)
    {
        /* event date computation */
        /* retrieve ptr of the structure of that block */
        ptr = *work;
        nPoints = ptr->nPoints;
        cnt1 = ptr->cnt1;
        cnt2 = ptr->cnt2;
        EVindex = ptr->EVindex;
        PerEVcnt = ptr->PerEVcnt;

        /* get current simulation time */
        t = get_scicos_time();

        if (ZC == 1)
        {
            /* generate Events only if ZC is active */
            if ((Method == 1) || (Method == 0))
            {
                /*-------------------------*/
                if (ptr->firstevent == 1)
                {
                    jfirst = nPoints - 1; /* finding first positive time instant */
                    for (j = 0; j < nPoints; j++)
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
                i = EVindex;
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
                        /*  Periodic*/
                        cnt1 = -1;
                        cnt2 = 0;
                        PerEVcnt++;/* When OutEnd==2 (perodic output)*/
                        jfirst = nPoints - 1; /* finding first positive time instant */
                        for (j = 0; j < nPoints; j++)
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
    }
    else if (flag == 5)
    {
        /* finish */
        ptr = *work;
        if (ptr != NULL)
        {
            if (ptr->D != NULL)
            {
                scicos_free(ptr->D);
            }
            if (ptr->work != NULL)
            {
                scicos_free(ptr->work);
            }
            if (ptr->workt != NULL)
            {
                scicos_free(ptr->workt);
            }
            scicos_free(ptr);
        }
        /***********************************************************************/
    }
    else if (flag == 6)
    {
        /* re-init */

        // finish then init
        fromws_c(block, 5);
        fromws_c(block, 4);
    }
    /*************************************************************************/
}
/*--------------------------------------------------------------------------*/
static int Ishm(int *fd, int *Ytype, int *nPoints, int *my, int *ny, int *YsubType)
{
    int *ptr_i;
    int j, ierr;

    /*work array to store header of hypermat*/
    if ((ptr_i = (int *) scicos_malloc(37 * sizeof(int))) == NULL)
    {
        return 0;
    }

    C2F(mgetnc) (fd, ptr_i, (j = 37, &j), fmti, &ierr); /* read sci id */
    if (ierr != 0)
    {
        return 0;
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
        Coserror(_("Invalid variable type : error in hypermat scilab coding.\n"));
        return 0;
    }

    *my      = ptr_i[30];    /*37*/
    *ny      = ptr_i[31];    /*38*/
    *nPoints = ptr_i[32];    /*39*/
    *Ytype   = ptr_i[33];    /*40*/

    if ((ptr_i[34] != ptr_i[30]*ptr_i[31]*ptr_i[32]) || \
            (ptr_i[35] != 1))
    {
        Coserror(_("Invalid variable type : error in hypermat scilab coding.\n"));
        scicos_free(ptr_i);
        return 0;
    }

    *YsubType = ptr_i[36];   /*43*/

    scicos_free(ptr_i);
    return 1;
}
/*--------------------------------------------------------------------------*/
static int Mytridiagldltsolve(double *dA, double * lA, double * B, int N)
{
    double Temp;
    int j;

    for (j = 1; j <= N - 1; ++j)
    {
        Temp = lA[j - 1];
        lA[j - 1] /= dA[j - 1];
        B[j] -= lA[j - 1] * B[j - 1];
        dA[j] -= Temp * lA[j - 1];
    }

    B[N - 1] /= dA[N - 1];
    for (j = N - 2; j >= 0; --j)
    {
        B[j] = - lA[j] * B[j + 1] + B[j] / dA[j];
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
