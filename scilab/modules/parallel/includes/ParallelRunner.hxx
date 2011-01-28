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
#include "CallableCFunctionPtr.hxx"
#include "execvisitor.hxx"

#define CONST
/*
  A ParallelRunner can use either threads (for shared meomry) o processes (for separate memory spaces).
  The ParallelRunner class factors common implementation between ProcessesRunner and ThreadsRunner derived classes.
  It also provides the create() factory method returning a ptr to either a ProcessRunner or a ThreadsRunner.

  Each Runner uses argument slices and result slices. It reads slices from arguments, calls the function and writes slices to results.

  * For shared memory workers, this is as simple as calling the relevent member functions (_set() and fillFromCol() for Double).

  * For separate memory workers (processes), we would need to have slices variables in shared memory. Achieving this would require
  variable classes (such as Double) to store their data in containers templated on an allocator (which could then be a Boost.Interprocess
  shared memory allocator) but we would then need Double to be an interface for the concrete data types.
  For this reason, we resort to a raw (but maximally aligned) shared memory buffer and memcpy data in/out of this buffer.
  (see ProcessesRunner for details)

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
        // factory fonction used by the gateway : takes the gateway fonction arguments and extract parameters
        static ParallelRunner* create(types::typed_list& in, std::size_t nbRes, types::typed_list& out);
        // the function performing the actual parallel computations
        virtual void run()=0;
        virtual ~ParallelRunner() { } // it is a base class

        /*
         * Classic constructor
         */
    protected:
        /*
         * constructor called by derived class contructors, setting all the member variables as per
         * parameters extracted by the factory member function.
         */
        template<typename In>
ParallelRunner(typed_list& in_, std::size_t nbRes_, typed_list& out_
                               , std::size_t n_, std::size_t nbArgs_, std::size_t chunkSize_, std::size_t nbWorkers_
               , bool dynamic_, CallablePtr function_, CallablePtr prologue_, CallablePtr epilogue_, In sizesBegin, In sizesEnd)
        : in(in_), out(out_), nbRes(nbRes_), n(n_), nbArgs(nbArgs_), chunkSize(chunkSize_), nbWorkers(nbWorkers_), dynamic(dynamic_)
        ,function(function_), prologue(prologue_), epilogue(epilogue_), resultSize(sizesBegin, sizesEnd)
        {
            init();
        }
        /* For each scialab variable, creates a slice to pass as argument to the called function.
         * @param typedListBegin  iterator to start of a sequence of InternalType* tp slice
         * @param typedListEnd  iterator to end of a sequence of InternalType* tp slice
         * @param o output iterator to write the InternalType* of the slices
         */
        template<typename In, typename Out> Out createSlices(In typedListBegin, In typedListEnd, Out o) const
        {
            return std::transform(typedListBegin, typedListEnd, o, &ParallelRunner::newSlice);
        }
        /* Write the nth slice of a Double var into a Double slice
         * @param var : scilab Double containing the data to write
         * @param nth : rank of the slice to write
         * @param slice: slice to be written
         */
        static void writeSlice(types::Double& var, std::size_t nth, types::Double& slice);
        /* Write slices Scilab variables (of type InternalType*) from arguments
         * (the first nbArgs in the 'in' member)into a typed_list
         * of already allocated Scilab variables. Performs dynamic dispatching to the relevant
         * writeSlice() overload.
         * @param nth : rank of the slices to write
         * @param slices: slices to be written
         */
        void writeSlices(std::size_t nth, types::typed_list& slices);

        /* Read form a Double slice into a Double var (at the nth column)
         * @param slice : Double to read
         * @param nth : index of the slice in var
         * @param var : Double to fill with the slice data
         */
        static void readSlice(types::Double CONST& slice, std::size_t nth, types::Double& var);

        /* Read slices Scilab variables (of type InternalType*) from a typed_list
         * into the Scilab variables stored in out member.
         * Performs the dinamic dispatching to the relevant readSlice() overlaod.
         * @param nth : rank of the slices to read
         * @param slices: slices to read
         */
        void readSlices(std::size_t nth, typed_list const& slices);

        /* Alloc a Scilab variable for the nth result of parallel_run
         * @param nth : the index of nth expected result in [0, nbRes[
         * @return types::InternalType* ptr to the newly created Scilab variable.
         */
        InternalType* newResult(std::size_t nth) const;

        /* trivial helper function delete-ing all the Scilab variables in a typed_list
         * called by run() to delete the slices created with newSlice
         * @param slices : the typed_list containing the InternalType* ptr of the variables to delete
         */
        void deleteSlices(types::typed_list& slices) const;

        /* Helper function to call a Callable with an std::size_t (cast to types::Double) argument.
         * used to call prologue and epilogue. Does nothing when f is null.
         * @param f Callable* to call or 0 if nothing to do
         * @param n arg to pass as types::Double to f
         * @param exec passed to the Callable::call()
         */
        void call(CallablePtr f, std::size_t n, ast::ExecVisitor& exec) const;

        /*
         * Creates (allocated on the heap, to be deleted as a whole with deleteSlices())
         * slice to hold
         */
        static InternalType* newSlice(types::InternalType* i);

        /* non template part of the constructor that can be put in a separate compilation unit
         * to minimize implementation in this header.
         */
        void init();

        types::typed_list& in; // args given to the gateway function sci_parallel_run
        types::typed_list& out; // or extracted by the create() factory function
        std::size_t nbRes, n, nbArgs, chunkSize, nbWorkers;
        bool dynamic, ok;
        CallablePtr function, prologue, epilogue;
        std::vector<std::size_t> resultSize;
    };

}
#endif
