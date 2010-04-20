#ifndef PARALLEL_RUN_HXX
#define PARALLEL_RUN_HXX

/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Bernard HUGUENEY
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <algorithm>
#include <vector>
#include <iterator>
#include <iostream>
#include <sys/mman.h>
#include <semaphore.h>
#include <pthread.h>
#include <signal.h>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <omp.h>

/*
  implementation notes: 
  due to alignment issues, we have to use lhs differents shared memory buffers
  we avoif busywaiting (bad for cpu time) or sleeping (bad for wallclock time) thanks to semaphores in shared memopry
*/


namespace {
    /*
     * allocates shared memory for s elements of type T. (anonymous, not mapped to a file)
     *
     * @param  T type to alloc
     * @param  s nb of elements to alloc mem, defaults to 1.
     */
    template<typename T> T* alloc_shared_mem(std::size_t s=1) {
        return static_cast<T*>(mmap(0, sizeof(T)*s, PROT_READ | PROT_WRITE,MAP_SHARED |  MAP_ANONYMOUS, -1, 0));
    }

    /*
     * Handles scheduling. Could be done in parallel_wrapper, but it would a a very long apply_with_processes() member function
     * breaking it would involve adding many member variables in the wrapper, so I chose an utility class with friend access to a parallel_wrapper
     * taken by ref.
     *
     * The trick is to exchange the res[] ptr with ptrs to some shared memory so that call_f() from each process fills the same shared result buffer.
     * When all is done, we copy the result to the original (not shared) result buffer. Apart from the result buffer, we also share a std::size_t *todo
     * poiting to the next index to compute.
     *
     * We use two cross process synch semaphores :
     * 1�) todo_protect to protect access to the *todo shared value
     * 2�) out_of_work to count how many workers have finished their work. Master process waits until nb_process have done their work.
     *
     */
    template< typename ParallelWrapper> struct scheduler
    {
        typedef std::pair<std::size_t, std::size_t> workshare_t;

        /*
         * constructs the scheduler, allocating the ressources (shared memory) and semaphores
         * @param wrapper the parallel wrapper launching the computations
         * @param nb_proc number of processes to use (spawns nb_proc-1 processes)
         * @param dyn if scheduling is dynamic or static
         * @param chunk_s chunk size. Only useful for dynamic sheduling as static is always faster with max chunk size for processes.
         * /!\ changes w.res[] to point to shared memory buffer
         */
        scheduler( ParallelWrapper& wrapper,std::size_t nb_proc,  bool dyn, std::size_t chunk_s)
            : w(wrapper), nb_process(nb_proc), dynamic(dyn), chunk_size(chunk_s), backup_res(wrapper.lhs) {
            for(std::size_t i(0); i != w.lhs; ++i) {
                backup_res[i]= w.res[i];
                const_cast<void**>(w.res)[i]=  alloc_shared_mem<char>(w.n * w.res_size[i]);
            }
            todo= alloc_shared_mem<std::size_t>();
            *todo= 0;
            out_of_work= alloc_shared_mem<sem_t>();
            sem_init(out_of_work, 1, 0); 
            todo_protect= alloc_shared_mem<sem_t>();
            sem_init(todo_protect, 1, 1); // available at start
        }

        /*
         * performs concurrent calls from w. (with w.f()) and copy results to the original w.res[] locations
         * but does not restore w.res[} (this is done in destructor.
         */
        void operator()() {
            struct sigaction backup_sigaction;
            {/* disable waiting notification from child death to avoid zombies */
                struct sigaction reapchildren;
                std::memset( &reapchildren, 0, sizeof reapchildren );
                reapchildren.sa_flags = SA_NOCLDWAIT;
                sigaction( SIGCHLD, &reapchildren, &backup_sigaction );
            }
            std::vector<workshare_t> init_ws(nb_process);
            for (std::size_t i(0); i != nb_process; ++i) {/* we precompute shares so that we don't have to synch */
		init_ws[i]= initial_work(i);
	    }
            std::size_t p;
            for (p= 1; p != nb_process; ++p) {
		if (!fork()) { /* child process goes to work at once */
		    break;
		}/* parent process continues to spawn children */
	    }
            if (p == nb_process) {
		p= 0;
	    }
            w.prologue(p);
            for(workshare_t ws(init_ws[p]); ws.first != ws.second; ws= get_work()) {
		for(std::size_t i(ws.first); i != ws.second; ++i) {
		    w.call_f(i);/* call_f() is performed on our shared mem as res[] */
		}
	    }
            sem_post(out_of_work);
            w.epilogue(p);
            if (p) {
		exit(EXIT_SUCCESS); // does not call destructor which is good :)
	    }
            for (std::size_t i(0); i != nb_process; ++i)  {/* wait for all workers to finish */
		sem_wait(out_of_work);
	    }
            for (std::size_t i(0); i != w.lhs; ++i) {/* copy results into the original res[] memory */
		std::memcpy(backup_res[i], w.res[i], w.res_size[i]*w.n);
	    }
            sigaction(SIGCHLD, &backup_sigaction, 0 );/* restore of sigaction */
        }
        /* destroy/ free semaphores/shared memory, and restores original w.res[] values. */
        ~scheduler() {
            for(std::size_t j(0); j!= w.lhs; ++j) {
                munmap(w.res[j], w.n * w.res_size[j]);
                const_cast<void**>(w.res)[j]= backup_res[j];
            }
            munmap(todo, sizeof(std::size_t));
            sem_destroy(out_of_work);
            munmap(out_of_work, sizeof(sem_t));
            sem_destroy(todo_protect);
            munmap(todo_protect, sizeof(sem_t));
        }
    private:
        /* compute initial workshares. no need to synch because we did not fork() yet 
         * @param p process id from 0(parent) -> nb_process-1 
         */
        workshare_t initial_work( std::size_t p) const {
            std::size_t old_todo(*todo);
            //      std::cerr<<"*todo="<<*todo<<" dynamic="<<dynamic<<" nb_process="<<nb_process<<" p="<<p<<std::endl;
            *todo= std::min(w.n, *todo + (dynamic ? chunk_size : (w.n-*todo)/(nb_process-p)));
            //      std::cerr<<"AFTER : *todo="<<*todo<<" dynamic="<<dynamic<<" nb_process="<<nb_process<<" p="<<p<<std::endl;
            return workshare_t(old_todo, *todo);
        }
        /* computes next workshare with protected access to shared *todo value  */
        workshare_t get_work() {
            workshare_t res;
            sem_wait(todo_protect);
            res.first= *todo;
            if (*todo != w.n) {
		res.second= std::min(w.n, *todo + chunk_size);// no need to handle static scheduling because the last initial_work then set todo=w.n
		*todo= res.second;
	    }
            else {
		res.second= w.n;
	    }
            sem_post(todo_protect);
            return res;
        }
        ParallelWrapper& w;
        std::size_t nb_process;
        bool dynamic;
        std::size_t chunk_size;
        sem_t* out_of_work;
        sem_t* todo_protect;
        std::size_t* todo;
        std::vector<void*> backup_res;
    };

    template<typename F, typename G>
    struct parallel_wrapper {
        parallel_wrapper(void const* const* const a, std::size_t const* a_s, std::size_t const* a_n, std::size_t const the_rhs, std::size_t nb_tasks, void * const* const r, std::size_t const* r_s, std::size_t const the_lhs, F the_f, G p, G e)
            :args(a), args_size(a_s), args_nb(a_n), rhs(the_rhs), n(nb_tasks),res(r), res_size(r_s), lhs(the_lhs), f(the_f), prologue(p), epilogue(e){}
 
        /* we define a functor. Calling it lanches the parallel processing of args, either with threads of processes(default).
           the nb of wokers (threads or processes) can also be specified (default is implementation defined usually nb of cores).
           TODO : enable specification of near / far / all (must, must not, can share L2 cache), at least for threads.
           so first arge might not stay boolean (we can add an overload)*/
        F operator()( bool with_threads=false, std::size_t nb_workers=0, bool dynamic_scheduling=true, int chunk_size=1){
            return with_threads 
                ? apply_with_threads(nb_workers, dynamic_scheduling, chunk_size) 
                : apply_with_processes(nb_workers, dynamic_scheduling, chunk_size);
        }
    private:
        friend struct scheduler<parallel_wrapper>;

        /* Launch concurrent calls to f using OpenMP threads. OpenMP semantics for the parameters.
         * @param nb_threads number of threads to use 
         * @param dynamic_scheduling if scheduling is dynamic or static
         * @param chunk_size chunk size.
         */
        F apply_with_threads(std::size_t nb_threads, bool dynamic_scheduling, int chunk_size) {
            std::size_t i;
	    nb_threads= std::min(nb_threads, n);
            if (nb_threads) {
                omp_set_num_threads(nb_threads);
            }
            if (dynamic_scheduling){
#pragma omp parallel for private(i) schedule(dynamic, chunk_size)
                for(i=0; i < n; ++i)
                    {
                        call_f(i);
                    }
            } else {
#pragma omp parallel for private(i)  schedule(static, chunk_size)
                for(i=0; i < n; ++i)
                    {
                        call_f(i);
                    }
      
            }
            return f;
        }

        /* Launch concurrent calls to f using fork()ed processes. See scheduler<> fr details
         * @param nb_processes number of processes to use 
         * @param dynamic_scheduling if scheduling is dynamic or static
         * @param chunk_size chunk size.
         */
        F apply_with_processes(std::size_t nb_process, bool dynamic_scheduling, std::size_t chunk_size) {
            nb_process = std::min( (nb_process ? nb_process : omp_get_num_procs()), n);
            scheduler<parallel_wrapper> s(*this, nb_process, dynamic_scheduling, chunk_size );
            s();
            return f;
        }
        /* Perform i^th call to f, ajusting arguments and results ptrs from args[] and res[] 
         * @param i args and reults index.
         */
        void call_f(std::size_t const i) {
            void const * local_args[rhs];
            for (std::size_t j(0); j!= rhs; ++j) {
                local_args[j]= args[j]+ (i%(*(args_nb+j))) *(*(args_size+j));/*(i%(*(args_nb+j))) because all args are not required to have same nb of elts*/
            }
            void * local_res[lhs];
            for (std::size_t j(0); j!= lhs; ++j) {
                local_res[j]= res[j]+ i*(*(res_size+j));// all res are required to have n elts
            }
            f(local_args, local_res);
            //      std::cerr<<"local_args "<<*(double*)*local_args<<" local_res "<<*(double*)*local_res<<std::endl;
      
        }
        void const* const* const args; /* ptrs to the rhs args */
        std::size_t const* args_size; /* sizeof each rhs */
        std::size_t const* args_nb;/* nb of given values for each arg, in 1...n */
        std::size_t const rhs;  /* nb of rhs for f */
        std::size_t n; /* nb of calls to perform */
        void* const* const res; /* ptr to the lhs results */
        std::size_t const* res_size; /* sizeof each lhs */
        std::size_t const lhs;/* nb of lhs for f */
        /* no res_nb because we must have room for n vaules for each result */
        F& f;/* function to call */
        G prologue, epilogue; /* function to call when launching/ terminating a worker process */
    };

}


/*
 * make a parallel wrapper, just calls the constructor : is used mainly not to have to type the complete templated name of the wrapper type.
 * We can then directly call the parallel_wrapper operator() to lanch the parallel processing.
 * @param 
 */
template<typename ArgsItIt, typename ArgsSizeIt, typename ArgsNbIt, typename Sz1, typename Sz2, typename ResItIt, typename ResSizeIt, typename Sz3, typename F, typename G>   parallel_wrapper<F, G>
make_parallel_wrapper(ArgsItIt args, ArgsSizeIt args_size, ArgsNbIt args_nb, Sz1 rhs, Sz2 n, ResItIt res, ResSizeIt res_size, Sz3 lhs, F f, G prologue, G epilogue)
{
    return parallel_wrapper<F, G>(args, args_size, args_nb, rhs, n, res, res_size, lhs, f, prologue, epilogue); 
}

#endif
