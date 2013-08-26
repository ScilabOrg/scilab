/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* Copyright (C) DIGITEO - 2009-2010 - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include <time.h>

#ifndef _MSC_VER
#include <sys/time.h>
#include <sys/resource.h> /* getrusage */
#else
#include <windows.h>
#endif
#include "timer.h"
/*
http://msdn.microsoft.com/en-us/library/ms683223(VS.85).aspx
http://www.opengroup.org/onlinepubs/000095399/functions/getrusage.html
*/
/*--------------------------------------------------------------------------*/
static double previousTimerValue = 0.0;
/*--------------------------------------------------------------------------*/
static int init_clock = 1;
/*--------------------------------------------------------------------------*/
int C2F(timer)(double *etime)
{
    *etime = scilab_timer();
    return(0);
}
/*--------------------------------------------------------------------------*/
double scilab_timer(void)
{
    double etime = 0.0;
    double now = 0.0;
    double usertime = 0.0, systime = 0.0;

#ifdef _MSC_VER
    FILETIME creation_time, exit_time, kernel_time, user_time;
    unsigned long long utime, stime;

    GetProcessTimes (GetCurrentProcess (), &creation_time, &exit_time,
                     &kernel_time, &user_time);

    utime = ((unsigned long long) user_time.dwHighDateTime << 32)
            + (unsigned) user_time.dwLowDateTime;

    stime = ((unsigned long long) kernel_time.dwHighDateTime << 32)
            + (unsigned) kernel_time.dwLowDateTime;

    usertime = (double)(utime / 1.0e7);
    systime = (double)(stime / 1.0e7);

    now = (double)(usertime + systime);

    if (init_clock == 1)
    {
        init_clock = 0;
        previousTimerValue = now ;
        // initialization forced here 0 - 0 > -0 or +0
        etime = 0.0;
    }
#else
    struct rusage rbuff;
    getrusage (RUSAGE_SELF, &rbuff);

    usertime = (double)((float) (rbuff.ru_utime).tv_sec +
                        (float) (rbuff.ru_utime).tv_usec / 1000000.0);

    systime = (double)((float) (rbuff.ru_stime).tv_sec +
                       (float) (rbuff.ru_stime).tv_usec / 1000000.0);

    now = (double)(usertime + systime);

    if (init_clock == 1)
    {
        init_clock = 0;
        previousTimerValue = now ;
        // initialization forced here 0 - 0 > -0 or +0
        etime = 0.0;
    }
#endif
    else
    {
        etime =  (double)(now - previousTimerValue);
        if (etime < 0.0)
        {
            previousTimerValue = 0.0;
            etime = 0.0;
        }
    }
    previousTimerValue = now ;
    return etime;
}
/*--------------------------------------------------------------------------*/
