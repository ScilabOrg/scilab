/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include <string.h>
#include <stdio.h>
#include "terme.h"
#include "stack-def.h"
#include "stack-c.h"
#include "basout.h"
#include "do_error_number.h"
#include "parserConstant.h"
/*--------------------------------------------------------------------------*/
extern int C2F(getsym)();
/*--------------------------------------------------------------------------*/
int C2F(terme)(void)
{
#define constnumber 114688

    /* Local variables */
    int op = 0;

    int r = 0;

    /* int equal,less,great,not */
    r = C2F(recu).rstk[(constnumber + (0 + ( (C2F(recu).pt - 1) << 2)) - constnumber) / 4];

    if (C2F(iop).ddt == 4)
    {
        static char tmp[100];
        static int io;
        sprintf(tmp, " terme pt:%d rstk(pt):%d sym:%d", C2F(recu).pt, C2F(recu).rstk[C2F(recu).pt - 1], C2F(com).sym);
        C2F(basout)(&io, &C2F(iop).wte, tmp, (long)strlen(tmp));

    }

    if ( (r / 100) != 2)
    {
        /* first factor */
        ++C2F(recu).pt;

        /* escape to call fact */
        C2F(recu).rstk[C2F(recu).pt - 1] = 201;
        C2F(recu).icall = 3;
        return 0;
    }

    switch (r - 200)
    {
        case 1:
        {
            /* return point after  first factor evaluation*/
            --C2F(recu).pt;
            op = 0;
            if (C2F(com).sym == dot)
            {
                op = dot;
                C2F(getsym)();
            }

            if (C2F(com).sym == star || C2F(com).sym == slash || C2F(com).sym == bchar_slash)
            {
                op += C2F(com).sym;
                C2F(getsym)();
                if (C2F(com).sym == dot)
                {
                    op += C2F(com).sym << 1;
                }

                if (C2F(com).sym == dot)
                {
                    C2F(getsym)();
                }

                ++C2F(recu).pt;
                C2F(recu).pstk[C2F(recu).pt - 1] = op;
                if (C2F(com).sym != not)
                {
                    /* escape to call fact */
                    C2F(recu).rstk[C2F(recu).pt - 1] = 202;
                    C2F(recu).icall = 3;
                    return 0;
                }

                /* escape to call expr */
                C2F(recu).rstk[C2F(recu).pt - 1] = 204;
                C2F(recu).icall = 1;
                return 0;
            }

            if (op != 0)
            {
                SciError(7);
            }
            return 0;
        }
        case 2:
        {
            /* return point after n th factor evaluation*/
            Fin = C2F(recu).pstk[C2F(recu).pt - 1];
            /* evaluation */
            Rhs = 2;

            /* escape to call allops(op) */
            C2F(recu).rstk[C2F(recu).pt - 1] = 203;
            C2F(recu).icall = 4;
            return 0;
        }
        case 3:
        {
            /* return point after operation evaluation (allops(op))*/
            --C2F(recu).pt;
            op = 0;
            if (C2F(com).sym == dot)
            {
                op = dot;
                C2F(getsym)();
            }
            if (C2F(com).sym == star || C2F(com).sym == slash || C2F(com).sym == bchar_slash)
            {
                op += C2F(com).sym;
                C2F(getsym)();
                if (C2F(com).sym == dot)
                {
                    op += C2F(com).sym << 1;
                }

                if (C2F(com).sym == dot)
                {
                    C2F(getsym)();
                }

                ++C2F(recu).pt;
                C2F(recu).pstk[C2F(recu).pt - 1] = op;
                if (C2F(com).sym != not)
                {
                    C2F(recu).rstk[C2F(recu).pt - 1] = 202;
                    /*next line added to handle syntax like a*-b for Matlab compatiblity */
                    if (C2F(com).sym == plus || C2F(com).sym == minus)
                    {
                        // Fix for bug 13168: replace icall=1 by icall=3
                        //  * icall==1 => call expr
                        //  * icall==3 => call fact
                        C2F(recu).icall = 3;    /* escape to call fact */
                    }
                    else
                    {
                        C2F(recu).icall = 3;    /* escape to call fact */
                    }
                    return 0;
                }

                /* escape to evaluate a logical factor (in expr) */
                C2F(recu).rstk[C2F(recu).pt - 1] = 204;
                C2F(recu).icall = 1;
                return 0;
            }

            if (op != 0)
            {
                SciError(7);
            }
            return 0;
        }
        case 4:
        {
            /* return point after logical factor evaluation (in expr)*/
            Fin = C2F(recu).pstk[C2F(recu).pt - 1];
            Rhs = 2;

            /* escape to call allops(op) */
            C2F(recu).rstk[C2F(recu).pt - 1] = 203;
            C2F(recu).icall = 4;
            return 0;
        }
        break;
    }

    SciError(22);
    return 0;
}
/*--------------------------------------------------------------------------*/
