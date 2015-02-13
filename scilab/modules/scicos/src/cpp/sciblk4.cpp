/*  Scicos
*
*  Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
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
#include <cstring>
#include <cstdio>

extern "C"
{
#include "sciblk4.h"
}

//#include "internal.hxx"
//#include "types.hxx"
//#include "double.hxx"

extern "C"
{
#include "machine.h" /*C2F*/
#include "stack-c.h"
#include "sci_malloc.h"
#include "scicos.h"
#include "import.h"
#include "createblklist.h"
#include "Scierror.h"
#include "localization.h"
    /*--------------------------------------------------------------------------*/
    /* Define external functions */
    extern int C2F(scierr)();
    extern void C2F(scifunc)(int&, int&);
    extern int* listentry(int* header, int i);
    extern void C2F(itosci)(int&, int, int);
}
/*--------------------------------------------------------------------------*/
static int sci2var(void* x, void* y, int typ_var);
/*--------------------------------------------------------------------------*/
static void Error(int ierr)
{
    if (ierr < 0) /*flag value error*/
    {
        Scierror(888, _("%s: error %d. Wrong flag value : %d to %d expected.\n"), "var2sci", ierr, 1, 10);
    }
    else if (ierr > 0) /*var2sci or sci2var error*/
    {
        /* Please update me! */
        if (ierr < 1000) /*var2sci error*/
        {
            switch (ierr)
            {
                case 1  :
                    Scierror(888, _("%s: error %d. Stack is full.\n"), "var2sci", ierr);
                    break;

                case 2  :
                    Scierror(888, _("%s: error %d. No more space on the stack for new data.\n"), "var2sci", ierr);
                    break;

                default :
                    Scierror(888, _("%s: error %d. Undefined error.\n"), "var2sci", ierr);
                    break;
            }
        }
        else /*sci2var error*/
        {
            switch (ierr)
            {
                case 1001  :
                    Scierror(888, _("%s: error %d. Only int or double object are accepted.\n"), "sci2var", ierr);
                    break;

                case 1002  :
                    Scierror(888, _("%s: error %d. Bad double object sub_type.\n"), "sci2var", ierr);
                    break;

                case 1003  :
                    Scierror(888, _("%s: error %d. Bad int object sub_type.\n"), "sci2var", ierr);
                    break;

                case 1004  :
                    Scierror(888, _("%s: error %d. A type of a scilab object has changed.\n"), "sci2var", ierr);
                    break;

                default    :
                    Scierror(888, _("%s: error %d. Undefined error.\n"), "sci2var", ierr);
                    break;
            }
        }
    }
    set_block_error(-1);
}
/*--------------------------------------------------------------------------*/
void sciblk4(scicos_block* Blocks, int flag)
{
    int ierr = 0;

    /* Save 'Top' counter */
    int topsave = Top;

    /* Retrieve block number */
    int kfun = get_block_number();

    /* Retrieve 'funtyp' by import structure */
    int* ptr = nullptr;
    int nv = 0, mv = 0;
    char buf[] = "funtyp";
    ierr = getscicosvarsfromimport(buf, (void**)&ptr, &nv, &mv);
    if (ierr == 0)
    {
        Error(ierr);
        return;
    }
    int* funtyp = (int *) ptr;

    /* Retrieve 'funptr' by import structure */
    void* ptr2 = nullptr;
    int nv2 = 0, mv2 = 0;
    char buf2[] = "funptr";
    ierr = getscicosvarsfromimport(buf2, (void**)&ptr2, &nv2, &mv2);
    if (ierr == 0)
    {
        Error(ierr);
        return;
    }
    void* funptr = (void *) ptr2;

    /*****************************
    * Create Scilab tlist Blocks *
    *****************************/
    int i = -1;
    if ((createblklist(&Blocks[0], &ierr, i, funtyp[kfun - 1])) == 0)
    {
        Error(ierr);
        return;
    }

    /* * flag * */
    i = 1;
    int j = 1;
    C2F(itosci)(flag, i, j);
    if (C2F(scierr)() != 0)
    {
        Error(ierr);
        return;
    }

    /* Set number of left and right hand side parameters */
    int mlhs = 1, mrhs = 2;

    /***********************
    * Call Scilab function *
    ***********************/
    C2F(scifunc)(mlhs, mrhs);
    if (C2F(scierr)() != 0)
    {
        Error(ierr);
        return;
    }

    /***************************
    * Update C block structure *
    ***************************/

    /* Get header of output variable Blocks of sciblk4 */
    int* header = (int *) stk(*Lstk(Top));

    /* Switch to appropriate flag */
    switch (flag)
    {
            /**************************
            * Update continuous state *
            **************************/
        case 0  :
        {
            if (Blocks[0].nx != 0)
            {
                /* 14 - xd */
                int* il_xd = (int *) listentry(header, 14);
                ierr = sci2var(il_xd, Blocks[0].xd, SCSREAL_N); /* double */
                if (ierr != 0)
                {
                    Error(ierr);
                    return;
                }

                if ((funtyp[kfun - 1] == 10004) || (funtyp[kfun - 1] == 10005))
                {
                    /* 15 - res */
                    int* il_res = (int *) listentry(header, 15);
                    ierr = sci2var(il_res, Blocks[0].res, SCSREAL_N); /* double */
                    if (ierr != 0)
                    {
                        Error(ierr);
                        return;
                    }
                }
            }
        }
        break;

        default :
            Error(3);
            return;
    }
}
/*--------------------------------------------------------------------------*/
/* sci2var function to convert scilab object
 * to an array of scicos blocks.
 *
 * Input parameters :
 * *x      : void ptr of scilab object.
 * *y      : void ptr of scicos blocks array.
 * typ_var : integer, type of scicos data :
 *           SCSREAL_N    : double real
 *           SCSCOMPLEX_N : double complex
 *           SCSINT_N     : int
 *           SCSINT8_N    : int8
 *           SCSINT16_N   : int16
 *           SCSINT32_N   : int32
 *           SCSUINT _N   : uint
 *           SCSUINT8_N   : uint8
 *           SCSUINT16_N  : uint16
 *           SCSUINT32_N  : uint32
 *           SCSUNKNOW_N  :
 *
 * Output parameters : int (>1000), error flag
 *                     (0 if no error)
 *
 * 15/06/06, Alan   : Initial version.
 *
 */

/* prototype */
static int sci2var(void* x, void* y, int typ_var)
{
    /* Define all types of accepted ptr */
    SCSREAL_COP *ptr_d, *y_d;
    SCSINT8_COP *ptr_c, *y_c;
    SCSUINT8_COP *ptr_uc, *y_uc;
    SCSINT16_COP *ptr_s, *y_s;
    SCSUINT16_COP *ptr_us, *y_us;
    SCSINT_COP *ptr_i, *y_i;
    SCSUINT_COP *ptr_ui, *y_ui;
    SCSINT32_COP *ptr_l, *y_l;
    SCSUINT32_COP *ptr_ul, *y_ul;

    /*****************************************
     * Get header,n,m and typ of scilab object
     *****************************************/
    /* Retrieve header address */
    int* header = (int *) x;

    /* Retieve number of rows and columns */
    int n = header[1];
    int m = header[2];

    /* Retrieve type of scilab object */
    int typ = header[0];

    /* Test if scilab object is a typed scicos accepted data */
    if ((typ != 1) & (typ != 8))
    {
        return 1001;
    }

    /* Retrieve sub_type of scilab object */
    int sub_typ =  header[3];

    /* Cross type of data checking
     * and copy scilab object in
     * scicos blocks array structure
     */
    int i, j;
    switch (typ)
    {
        case 1 :
        {
            /*Check type of double matrix*/
            if ((sub_typ != 0) & (sub_typ != 1))
            {
                return 1002;
            }
            if ((sub_typ == 0) & (typ_var != SCSREAL_N))
            {
                return 1004;
            }
            if ((sub_typ == 1) & (typ_var != SCSCOMPLEX_N))
            {
                return 1004;
            }

            /*Copy double matrix*/
            switch (sub_typ)
            {
                case 0 :
                {
                    int nm = n * m;
                    ptr_d = (SCSREAL_COP *) (header + 4);
                    y_d = (SCSREAL_COP *) y;
                    C2F(unsfdcopy)(&nm, ptr_d, (i = -1, &i), y_d, (j = -1, &j));
                    break;
                }

                case 1 :
                {
                    int nm = 2 * n * m;
                    ptr_d = (SCSCOMPLEX_COP *) (header + 4);
                    y_d = (SCSCOMPLEX_COP *) y;
                    C2F(unsfdcopy)(&nm, ptr_d, (i = -1, &i), y_d, (j = -1, &j));
                    break;
                }
            }
        }
        break;

        case 8 :
        {
            /*Check type of int matrix*/
            if ((sub_typ != 1)  && (sub_typ != 2) && (sub_typ != 4) &&
                    (sub_typ != 11) && (sub_typ != 12) && (sub_typ != 14))
            {
                return 1003;
            }
            if ((sub_typ == 1) & (typ_var != SCSINT8_N))
            {
                return 1004;
            }
            if ((sub_typ == 2) & (typ_var != SCSINT16_N))
            {
                return 1004;
            }
            if ((sub_typ == 4) & (typ_var != SCSINT_N) & (typ_var != SCSINT32_N))
            {
                return 1004;
            }
            if ((sub_typ == 11) & (typ_var != SCSUINT8_N))
            {
                return 1004;
            }
            if ((sub_typ == 12) & (typ_var != SCSUINT16_N))
            {
                return 1004;
            }
            if ((sub_typ == 14) & (typ_var != SCSUINT_N) & (typ_var != SCSUINT32_N))
            {
                return 1004;
            }

            /*Copy int matrix*/
            switch (typ_var)
            {
                case SCSINT_N    :
                    ptr_i = (SCSINT_COP *) (header + 4);
                    y_i = (SCSINT_COP *) y;
                    for (i = 0; i < n * m; i++)
                    {
                        y_i[i] = ptr_i[i];
                    }
                    break;

                case SCSINT8_N   :
                    ptr_c = (SCSINT8_COP *) (header + 4);
                    y_c = (SCSINT8_COP *) y;
                    for (i = 0; i < n * m; i++)
                    {
                        y_c[i] = ptr_c[i];
                    }
                    break;

                case SCSINT16_N  :
                    ptr_s = (SCSINT16_COP *) (header + 4);
                    y_s = (SCSINT16_COP *) y;
                    for (i = 0; i < n * m; i++)
                    {
                        y_s[i] = ptr_s[i];
                    }
                    break;

                case SCSINT32_N  :
                    ptr_l = (SCSINT32_COP *) (header + 4);
                    y_l = (SCSINT32_COP *) y;
                    for (i = 0; i < n * m; i++)
                    {
                        y_l[i] = ptr_l[i];
                    }
                    break;

                case SCSUINT_N   :
                    ptr_ui = (SCSUINT_COP *) (header + 4);
                    y_ui = (SCSUINT_COP *) y;
                    for (i = 0; i < n * m; i++)
                    {
                        y_ui[i] = ptr_ui[i];
                    }
                    break;

                case SCSUINT8_N  :
                    ptr_uc = (SCSUINT8_COP *) (header + 4);
                    y_uc = (SCSUINT8_COP *) y;
                    for (i = 0; i < n * m; i++)
                    {
                        y_uc[i] = ptr_uc[i];
                    }
                    break;

                case SCSUINT16_N :
                    ptr_us = (SCSUINT16_COP *) (header + 4);
                    y_us = (SCSUINT16_COP *) y;
                    for (i = 0; i < n * m; i++)
                    {
                        y_us[i] = ptr_us[i];
                    }
                    break;

                case SCSUINT32_N :
                    ptr_ul = (SCSUINT32_COP *) (header + 4);
                    y_ul = (SCSUINT32_COP *) y;
                    for (i = 0; i < n * m; i++)
                    {
                        y_ul[i] = ptr_ul[i];
                    }
                    break;
            }
        }
        break;
    }

    /* Return error flag = 0 */
    return 0;
}
