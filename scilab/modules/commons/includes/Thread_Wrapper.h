/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *  Copyright (C) 2008-2008 - INRIA - Allan CORNET
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*
** Thread wrapper to have an easy way to manage
** those features on each platform
*/

#ifndef __THREAD_WRAPPER_H__
#define __THREAD_WRAPPER_H__

#include "dynlib_commons.h"

/*
** WINDOWS
*/
#ifdef _MSC_VER
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x500
#endif
#include <Windows.h>
#include <process.h>

typedef DWORD                           __threadKey;
typedef HANDLE                          __threadId;
typedef HANDLE                          __threadLock;
typedef CRITICAL_SECTION                __threadSignalLock;
typedef HANDLE                          __threadSignal;
#define __StaticInitLock                NULL

#define __StaticInitThreadSignal        NULL

#else
/*
** LINUX & Mac
*/
#include <pthread.h>
#include <signal.h>
#include <stdlib.h> // malloc

typedef pthread_t                       __threadKey;
typedef pthread_t                       __threadId;
typedef pthread_mutex_t                 __threadLock;
typedef pthread_mutex_t                 __threadSignalLock;
typedef pthread_cond_t                  __threadSignal;
#define __StaticInitLock                PTHREAD_MUTEX_INITIALIZER

#define __StaticInitThreadSignal        PTHREAD_COND_INITIALIZER
#endif

COMMONS_IMPEXP void __InitLock(__threadLock *lockName);

COMMONS_IMPEXP void __Lock(__threadLock *lockName);

COMMONS_IMPEXP void __UnLock(__threadLock *lockName);

COMMONS_IMPEXP void __InitSignalLock(__threadSignalLock *lockName);

COMMONS_IMPEXP void __LockSignal(__threadSignalLock *lockName);

COMMONS_IMPEXP void __UnLockSignal(__threadSignalLock *lockName);

COMMONS_IMPEXP void __InitSignal(__threadSignal *signalName);

COMMONS_IMPEXP void __Signal(__threadSignal *signalName);

COMMONS_IMPEXP void __Wait(__threadSignal *signalName, __threadSignalLock *lockName);

COMMONS_IMPEXP void __CreateThread(__threadId *threadId, __threadKey *threadKey, void *(*functionName) (void *));

COMMONS_IMPEXP void __CreateThreadWithParams(__threadId *threadId, __threadKey *threadKey, void *(*functionName) (void *), void *params);

COMMONS_IMPEXP void __WaitThreadDie(__threadId threadId);

COMMONS_IMPEXP void __Terminate(__threadId threadId);

COMMONS_IMPEXP __threadId __GetCurrentThreadId();

COMMONS_IMPEXP __threadKey __GetCurrentThreadKey();

COMMONS_IMPEXP void __SuspendThread(__threadId ThreadId);

COMMONS_IMPEXP void __ResumeThread(__threadId ThreadId);

#endif /* !__THREAD_WRAPPER_H__ */

