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
#ifndef PARALLEL_PARALLELRUNNER_HXX
#define PARALLEL_PARALLELRUNNER_HXX

#include <vector>
#include <algorithm>
#include <functional>

#include "callable.hxx"
#include "internal.hxx"
#include "double.hxx" // should be all types when everything is implemented
#include "execvisitor.hxx"

#define CONST
/*
  A ParallelRunner can use either threads (for shared meomry) o processes (for separate memory spaces).
  The ParallelRunner class mainly parses the arguments to extract parameters.
  The operator() performing the parallel computations delegates to either ProcessesScheduler ot ThreadsScheduler.

  The schedulers are taken outside of the ParallelRunenr for clarity reasons, but they make heavy use of ParallelRunner internals
  for the common operations (e.g. managing slices).
  Each scheduler uses argument slices and result slices. It reads slices from arguments, calls the function and writes slices to results.

  * For shared memory workers, this is as simple as calling the relevent member functions (_set() and fillFromCol() for Double).

  * For separate memory workers (processes), we would need to have slices variables in shared memory. Achieving this would require
  variable classes (such as Double) to store their data in containers templated on an allocator (which could then be a Boost.Interprocess
  shared memory allocator) but we would then need Double to be an interface for the concrete data types.
  For this reason, we resort to a type erase (but maximally aligned) shared memory buffer and memcpy data in/out of this buffer.

  Note: For writing to slices from args slices, we can always (shared momry of not) use the simple shared memory model, because
  fork() implements a copy on write, giving each forked process a copy of the arguments.

  Windows specific: However, an hypothetical windows separate meomry model implementation that would spawn a new pristine Scilab
  process would have to use shared memory for both arguments and results. Note that it would not have any environnement available,
  but the master process could store the environement (with hdf5) and the slave processes would load it with a proper prologue
  (as we pay the performance price of dynamic scoping, we might as well use it).

  While the (new in Scilab >= 6.0) ability to run macros in shared memory renders the separate memory model less useful, it
  is still nedeed if the scilab code calls some Fortran/C code writing to a global state (think COMMON blocks).

*/
namespace parallel
{
    /*
     * Main class implementing parallel_run.
     * parses the arguments, allocates data (temporary arguments and returned results) variables
     * and delegates the parallel execution (threads or processes) to a scheduler.
     */
    struct ParallelRunner
    {
        typedef Callable* CallablePtr;

// used by gateway function
        /*
         * Classic constructor, takes the gateway fonction arguments and extract parameters
         */
        ParallelRunner(types::typed_list& in, std::size_t nbRes, types::typed_list& out);
        /*
         * Tells is the arguments allowed to construct a valid ParallelRunner object.
         */
        bool isValid() const;

//used by schedulers
        template<typename Out> Out createSlices(Out o) const
        {
            return std::transform(in.begin(), in.begin()+nbArgs, o, std::bind1st(std::mem_fun(&ParallelRunner::newSlice), this));
        }
        static void writeSlice(types::Double& var, std::size_t i, types::Double& slice);
        void writeSlices(std::size_t nth, types::typed_list& slices);
        static void readSlice(types::Double CONST& slice, std::size_t i, types::Double& var);
        void deleteSlices(types::typed_list& slices) const;
        void readSlices(std::size_t nth, typed_list const& slices);
        InternalType* newResult(std::size_t i) const;

        void operator()(bool withThreads=true);
        void call(CallablePtr f, std::size_t n, ast::ExecVisitor& exec) const;

        /*
         * Creates (allocated on the heap, to be deleted as a whole with deleteSlices())
         * slice to hold
         */
        InternalType* newSlice(types::InternalType* i) const;

        CallablePtr getFunction(std::size_t pos);
        types::typed_list& in;
        types::typed_list& out;
        std::size_t nbRes, n, nbArgs, chunkSize, nbWorkers;
        bool dynamic, ok;
        CallablePtr fun, prologue, epilogue;
        std::vector<std::size_t> resultSize;
    };

}
#endif
