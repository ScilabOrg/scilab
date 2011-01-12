/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Bernard HUGUENEY
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef PARALLEL_UTILITY_HXX
#define PARALLEL_UTILITY_HXX

#include <cstring>

extern "C"
{
#include <unistd.h>

#include "Scierror.h"
#include "localization.h"
//#include "parameters.h"
#include "Thread_Wrapper.h" /* locks for concurrency access */

#ifdef _MSC_VER

#include "mmapWindows.h"

#else
#include <sys/mman.h>
#ifndef MAP_ANONYMOUS
# define MAP_ANONYMOUS MAP_ANON
#endif
#endif
}
#include <semaphore.h>
#include <pthread.h>
#include <signal.h>


namespace parallel
{
    /*
     * allocates shared memory for s elements of type T. (anonymous, not mapped to a file)
     *
     * @param  T type to alloc
     * @param  s nb of elements to alloc mem, defaults to 1.
     */
    template<typename T> T* allocSharedMem(std::size_t s=1)
    {
        return static_cast<T*>(mmap(0, sizeof(T)*s, PROT_READ | PROT_WRITE,MAP_SHARED |  MAP_ANONYMOUS, -1, 0));
    }
    /*
     * free shared memory for s elements of type T.
     * @param ptr : pointer to the memory zone to deallocate
     * @param  s nb of elements to alloc mem, defaults to 1.
     *
     * Note: no synch is needed amongst writes because the various process do not access the same parts of the data.
     * (of course, one must ensure that all writes are done before reading the result).
     */

    template<typename T> void freeSharedMem(T* ptr, std::size_t s=1)
    {
        munmap(ptr, sizeof(T)*s);
    }

/*
 * A semaphore in shared memory, abstracted to allow windows and Posix implementations
 */
    struct PortableSemaphore
    {
        /*
         * Initialize the semaphore.
         *@param init : initial value
         */
        explicit PortableSemaphore (unsigned int init):ptr(allocSharedMem<sem_t>())
        {
            sem_init(ptr,1, init);
        }

        void post()
        {
            sem_post(ptr);
        }
        void wait()
        {
            sem_wait(ptr);
        }
        ~PortableSemaphore()
        {
            munmap(ptr, sizeof(sem_t));
        }

        sem_t* ptr;
    };
/*
 * Signal inhibiter, used to avoid zombies of dead child processes, abstracted for portability.
 */
    struct PortableSignalInhibiter
    {
        PortableSignalInhibiter()
        {
#ifndef _MSC_VER
            struct sigaction reapchildren;
            std::memset( &reapchildren, 0, sizeof reapchildren );
            reapchildren.sa_flags = SA_NOCLDWAIT;
            sigaction( SIGCHLD, &reapchildren, &backup_sigaction );
#else
#endif
        }
        ~PortableSignalInhibiter()
        {
#ifndef _MSC_VER
            sigaction(SIGCHLD, &backup_sigaction, 0 );/* restore of sigaction */
#else
#endif
        }
#ifndef _MSC_VER
        struct sigaction backup_sigaction;
#else
#endif
    };

    /*
     * function calling delete on a ptr. Used as a callable in algorithms.
     *@param ptr : ptr to delete
     */
    template<typename T> void callDelete(T* ptr) { delete ptr;}

}

#endif
