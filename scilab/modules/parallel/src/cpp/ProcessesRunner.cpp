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


#include <utility>
#include <algorithm>

#ifdef TRACE
#include <iostream>
#endif

#include "ProcessesRunner.hxx"
#include "ConcurrencyState.hxx"

#include "internal.hxx"
#include "parallel_utility.hxx"

extern "C"
{
#include "sci_mode.h"
}

namespace parallel
{

    void ProcessesRunner::run()
    {
        parallel::ConcurrencyState::ScopedUpdater u(parallel::concurrencyState.scopedUpdater(false));
        PortableSignalInhibiter guard; /* disable waiting notification from child death to avoid zombies */
#ifdef TRACE
        std::cerr<<"in processes scheduler for "<<nbWorkers<<" processes\n";
#endif
        std::vector<ProcessesRunner::workshare_t> init_ws(nbWorkers);
        for (std::size_t i(0); i != nbWorkers; ++i)
        {/* we precompute shares so that we don't have to synch */
            init_ws[i]= initialWork(i);
        }
        std::size_t id;
        for (id= 1; id != nbWorkers; ++id)
        {
            if (!fork())
            { /* child process goes to work at once */
                setScilabMode(SCILAB_NWNI);
                break;
            }/* parent process continues to spawn children */
        }
        if (id == nbWorkers)
        {
            id= 0;
        }
        ast::ExecVisitor exec;
        call(prologue, id, exec);
        types::typed_list fIn, fOut;
        createSlices(in.begin(), in.begin()+nbArgs, std::back_inserter(fIn));
        std::vector<bool> processed(n, false);
        for(ProcessesRunner::workshare_t ws(init_ws[id]); ws.first != ws.second; ws= getWork())
        {
            for(std::size_t i(ws.first); i != ws.second; ++i)
            {
#ifdef TRACE
                std::cerr<<" process "<<id<<" processing slice "<<i<<" of "<<n<<std::endl;
#endif
                writeSlices(i, fIn);
                function->call(fIn, nbRes, fOut, &exec);
                readSlices(i, fOut);
                processed[i]= 0xFF;
            }
        }
        call(epilogue, id, exec);
        {
            copyToSharedMemory(processed.begin());
        }
        out_of_work.post();
        if (id)
        {
            exit(EXIT_SUCCESS); // does not call destructor which is good :)
        }
        for (std::size_t i(0); i != nbWorkers; ++i)
        {/* wait for all workers to finish */
            out_of_work.wait();
        }
        copyFromSharedMemory();
    }

    void ProcessesRunner::copyFromSharedMemory()
    {
        for (std::size_t var(0); var != nbRes; ++var)
        {
            sharedBuffer[var]->write(*out[var]);
        }
    }

    ProcessesRunner::~ProcessesRunner()
    {
        std::for_each(sharedBuffer.begin(), sharedBuffer.end(), &callDelete<SharedData>);
        freeSharedMem(todo, sizeof(std::size_t));
    }

    ProcessesRunner::workshare_t ProcessesRunner::initialWork( std::size_t p) const
    {
        std::size_t old_todo(*todo);
        //      std::cerr<<"*todo="<<*todo<<" dynamic="<<dynamic<<" nb_process="<<nb_process<<" p="<<p<<std::endl;
        *todo= min(n, *todo + (dynamic ? chunkSize : (n-*todo)/(nbWorkers-p)));
        //      std::cerr<<"AFTER : *todo="<<*todo<<" dynamic="<<dynamic<<" nb_process="<<nb_process<<" p="<<p<<std::endl;
        return ProcessesRunner::workshare_t(old_todo, *todo);
    }

    ProcessesRunner::workshare_t ProcessesRunner::getWork()
    {
        ProcessesRunner::workshare_t res;
        todo_protect.wait();
        res.first= *todo;
        if (*todo != n)
        {
            res.second= min(n, *todo + chunkSize);// no need to handle static scheduling because the last initialWork then set todo=w.n
            *todo= res.second;
        }
        else
        {
            res.second= n;
        }
        todo_protect.post();
        return res;
    }
}
