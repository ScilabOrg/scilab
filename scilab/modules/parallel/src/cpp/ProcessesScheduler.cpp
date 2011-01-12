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

#include "ProcessesScheduler.hxx"
#include "internal.hxx"
#include "parallel_utility.hxx"
#include "ParallelRunner.hxx"

extern "C"
{
#include "sci_mode.h"
}

namespace parallel
{

    ProcessesScheduler::ProcessesScheduler(ParallelRunner& p)
        : pr(p), todo(allocSharedMem<std::size_t>()), out_of_work(0)
        , todo_protect(1), sharedBuffer(pr.nbRes)
    {
        std::transform(pr.out.begin(), pr.out.end(), sharedBuffer.begin(), &SharedData::create);
        *todo= 0;
    }

    void ProcessesScheduler::operator()()
    {
        PortableSignalInhibiter guard; /* disable waiting notification from child death to avoid zombies */
#ifdef TRACE
        std::cerr<<"in processes scheduler for "<<pr.nbWorkers<<" processes\n";
#endif
        std::vector<ProcessesScheduler::workshare_t> init_ws(pr.nbWorkers);
        for (std::size_t i(0); i != pr.nbWorkers; ++i)
        {/* we precompute shares so that we don't have to synch */
            init_ws[i]= initialWork(i);
        }
        std::size_t id;
        for (id= 1; id != pr.nbWorkers; ++id)
        {
            if (!fork())
            { /* child process goes to work at once */
                setScilabMode(SCILAB_NWNI);
                break;
            }/* parent process continues to spawn children */
        }
        if (id == pr.nbWorkers)
        {
            id= 0;
        }
        ast::ExecVisitor exec;
        pr.call(pr.prologue, id, exec);
        types::typed_list fIn, fOut;
        pr.createSlices(std::back_inserter(fIn));
        std::vector<char> processed(pr.n, 0);// not <boll> because we need pointers (check that the specialization oddities do not hurt us)
        for(ProcessesScheduler::workshare_t ws(init_ws[id]); ws.first != ws.second; ws= getWork())
        {
            for(std::size_t i(ws.first); i != ws.second; ++i)
            {
#ifdef TRACE
                std::cerr<<" process "<<id<<" processing slice "<<i<<" of "<<pr.n<<std::endl;
#endif
                pr.writeSlices(i, fIn);
                pr.fun->call(fIn, pr.nbRes, fOut, &exec);
                pr.readSlices(i, fOut);
                processed[i]= 0xFF;
            }
        }
        pr.call(pr.epilogue, id, exec);
        {
            char const& start(*processed.begin());
            copyToSharedMemory(&start);
        }
        out_of_work.post();
        if (id)
        {
            exit(EXIT_SUCCESS); // does not call destructor which is good :)
        }
        for (std::size_t i(0); i != pr.nbWorkers; ++i)
        {/* wait for all workers to finish */
            out_of_work.wait();
        }
        copyFromSharedMemory();
    }

    void ProcessesScheduler::copyFromSharedMemory()
    {
        for (std::size_t var(0); var != pr.nbRes; ++var)
        {
            sharedBuffer[var]->write(*pr.out[var]);
        }
    }

    ProcessesScheduler::~ProcessesScheduler()
    {
        std::for_each(sharedBuffer.begin(), sharedBuffer.end(), &callDelete<SharedData>);
        freeSharedMem(todo, sizeof(std::size_t));
    }

    ProcessesScheduler::workshare_t ProcessesScheduler::initialWork( std::size_t p) const
    {
        std::size_t old_todo(*todo);
        //      std::cerr<<"*todo="<<*todo<<" dynamic="<<dynamic<<" nb_process="<<nb_process<<" p="<<p<<std::endl;
        *todo= min(pr.n, *todo + (pr.dynamic ? pr.chunkSize : (pr.n-*todo)/(pr.nbWorkers-p)));
        //      std::cerr<<"AFTER : *todo="<<*todo<<" dynamic="<<dynamic<<" nb_process="<<nb_process<<" p="<<p<<std::endl;
        return ProcessesScheduler::workshare_t(old_todo, *todo);
    }

    ProcessesScheduler::workshare_t ProcessesScheduler::getWork()
    {
        ProcessesScheduler::workshare_t res;
        todo_protect.wait();
        res.first= *todo;
        if (*todo != pr.n)
        {
            res.second= min(pr.n, *todo + pr.chunkSize);// no need to handle static scheduling because the last initialWork then set todo=w.n
            *todo= res.second;
        }
        else
        {
            res.second= pr.n;
        }
        todo_protect.post();
        return res;
    }
}
