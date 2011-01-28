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
#include "ParallelRunner.hxx"
#include "ProcessesRunner.hxx"
#include "ThreadsRunner.hxx"
#include "tlist.hxx"
#include <iostream>
#include <iterator>

#include "CallableCFunctionPtr.hxx"

namespace
{

    std::size_t readFromVar(types::InternalType* i, std::size_t v)
    {
        std::size_t result(v);
        if (i)
        {
            switch (i->getType())
            {
            case types::InternalType::RealDouble :
            {
                result= i->getAsDouble()->real_get(0,0);
                break;
            }
            case types::InternalType::RealInt :
            {
                result= i->getAsInt()->data_get(0,0);
                break;
            }
            default: // result already has the default value // should we error or warn if the key is present wuth bad type ?
            {
            }
            }
        }
        return result;
    }

    Callable* readFromVar(types::InternalType* i, Callable* v)
    {
        Callable* result(v);
        if (i)
        {
            switch (i->getType())
            {
            case types::InternalType::RealString :
            {
                wchar_t const* const funName(i->getAsString()->string_get(0,0));
                result= dynamic_cast<Callable*>(symbol::Context::getInstance()->get(funName));
                if(!result) // not found in macros
                {
                    // TODO search for name in
#ifdef SEARCH_IN_DYNLINKS_IMPLEMENTED
                    typename CallableCFunctionPtr::loadedFunction_t toLoad(0);
                    found=SearchInDynLinks(funName, &toLoad);
                    if (found)
                    {
                        result= new CallableCFunctionPtr(toLoad);
                    }
                    else
                    {
                        // error msg
                    }
#else
                    result= new CallableCFunctionPtr();
#endif
                }
                break;
            }
            case types::InternalType::RealFunction :
            case types::InternalType::RealMacro :
            {
                result= dynamic_cast<Callable*>(i);
                break;
            }
            default: // result already has the default value // should we error or warn if the key is present with bad type ?
            {
            }
            }
        }
        return result;
    }


}
/*
 * TODO make a wrapper to C function ptr with a Callable interface
 */
namespace parallel
{

    ParallelRunner* ParallelRunner::create(typed_list& in, std::size_t nbRes, typed_list& out)
    {
        typedef Callable* CallablePtr;
        std::size_t nbCalls(0), nbArgs(0), chunkSize(1), nbWorkers(0);
        bool dynamic(true), withThreads(false), ok(in.size() >=2);
        CallablePtr function(0), prologue(0), epilogue(0);
        std::vector<std::size_t> resultSizes;

        if(ok)
        {
            for (std::size_t pos(0); (pos != in.size()) && ok; ++pos)
            {
                types::InternalType::RealType const type(in[pos]->getType());
                if (!nbArgs)
                {
                    switch (type)
                    {
                    case types::InternalType::RealDouble :
                    {
                        /* check not complex "%s: This feature has not been implemented.\n" */
                        //ok= true;// !isVarComplex( pvApiCtx, addr); should handle cplx
/*
  In fact, complex are ok for threads because we can let the classes do the copy themselves,
  but not for processes because we have to handle the shared memory buffer (cf. supra on allocators).
*/
                        nbCalls= max(nbCalls, static_cast<std::size_t>(in[pos]->getAsDouble()->cols_get()));
                        break;
                    }
                    case types::InternalType::RealString :
                    {
                        /* check dim = 1x1 */
                        ok= (in[pos]->getAsString()->size_get() == 1);
                    }/* no break */
                    case types::InternalType::RealFunction :
                    case types::InternalType::RealMacro :
                    {
                        nbArgs= pos;
//#define TEST_NATIVE
#ifdef TEST_NATIVE
                            function= (in[pos]->getAsString() && (wcscmp(in[pos]->getAsString()->string_get(0,0), L"test_native")== 0))
                                ? new CallableCFunctionPtr() : readFromVar(in[pos], function);
#else
                            function= readFromVar(in[pos], function);
#endif

                            std::vector<std::size_t> defaultSizes(pos, 1);
                            resultSizes.swap(defaultSizes); // init sizes with default value 1 (= scalar)
                            break;
                    }
                    default : {
                        Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),"parallel_run", pos+1);
                        ok= false;
                    }
                    }
                }
                else
                {
                    switch (type)
                    {
                    case types::InternalType::RealString :
                    {
                        // @TODO-return-type store type info, for now only handles Double /!\ cd  @TODO-return-type in ::newResult()
                        break;
                    }
                    case types::InternalType::RealDouble :
                    {
                        Double &d(*in[pos]->getAsDouble());
                        /* check not complex and ncol <=2 */
                        ok= ! (d.isComplex() || (d.cols_get() > 2));
                        if(ok)
                        {
                            for(std::size_t i(0); i != d.rows_get(); ++i)
                            {
                                resultSizes[i]= (d.cols_get() == 1)
                                    ? d.real_get(i, 0)
                                    : d.real_get(i, 0) *  d.real_get(i, 1);
                            }
                            break;
                        }
                    }
                    case types::InternalType::RealTList :
                    {
                        types::TList CONST& params(*(in[pos]->getAsTList()));
                        chunkSize= readFromVar(params.get(L"chunk_size"), chunkSize);
                        nbWorkers= readFromVar(params.get(L"nb_workers"), nbWorkers);
                        withThreads= static_cast<bool>(readFromVar(params.get(L"shared_memory"), static_cast<std::size_t>(0)));
                        dynamic= static_cast<bool>(readFromVar(params.get(L"dynamic_scheduling"), static_cast<std::size_t>(1)));
                        prologue= readFromVar(params.get(L"prologue"), prologue);
                        epilogue= readFromVar(params.get(L"epilogue"), epilogue);
#ifdef TRACE
                        std::cerr<<"reading a TList param argument\n";
                        std::cerr<<"prologue="<<prologue<<std::endl;
                        std::cerr<<"epilogue="<<epilogue<<std::endl;
#endif
                        break;
                    }
                    default :
                    {
                        ok= false;
                    }
                    }
                }
            }
            ok &= nbCalls && nbArgs;
        }

        if (!ok)
        {
            return 0; // exception with error message
        }
        nbWorkers= nbWorkers ? nbWorkers : omp_get_num_procs() ;// default value of 0 is nb of cores.
#ifdef TRACE
        std::cerr<<"nbArgs="<<nbArgs<<" nbWorkers="<<nbWorkers<<"nbCalls="<< nbCalls<<"chunkSize="<<chunkSize<<"dynamic="<<dynamic<<"withThreads="<<withThreads<<" resultSizes:";
        std::copy(resultSizes.begin(), resultSizes.end(), std::ostream_iterator<std::size_t>(std::cerr, " "));
        std::cerr<<std::endl;
        for(std::size_t i(0); i != nbRes; ++i)
        {
            std::cerr<<"result "<<i<<" is decared of size "<<resultSizes[i]<<std::endl;
        }

#endif
        return withThreads
            ? static_cast<ParallelRunner*>(new ThreadsRunner(in, nbRes, out, nbCalls, nbArgs, chunkSize, nbWorkers, dynamic, function, prologue, epilogue, resultSizes.begin(), resultSizes.end()))
            : static_cast<ParallelRunner*>(new ProcessesRunner(in, nbRes, out, nbCalls, nbArgs, chunkSize, nbWorkers, dynamic, function, prologue, epilogue, resultSizes.begin(), resultSizes.end()));
}


void ParallelRunner::call(ParallelRunner::CallablePtr f, std::size_t id, ast::ExecVisitor& exec) const
{
    if(f)
    {
        Double* arg= new types::Double(1, 1, false);
        arg->val_set(0,0,static_cast<double>(id));
        typed_list i(1,arg), o;
//        arg->IncreaseRef();
        f->call(i, 0, o, &exec);
    }
}

types::InternalType* ParallelRunner::newSlice(types::InternalType* i)
{
    switch(i->getType())
    {
    case types::InternalType::RealDouble :
    {
        Double* d(new Double(i->getAsDouble()->rows_get(), 1, i->getAsDouble()->isComplex()));
        d->IncreaseRef(); // important
        return d;
    }
    default:{}
    }
    return 0;
}
void ParallelRunner::init()
{
    for(std::size_t i(0); i!= nbRes; ++i)
    {
        out.push_back(newResult(i));
    }
    /* upon creation of the CallableCFunctionPtr (if it was such a Callable)
     * ::create() do not have enough info about the expected result variables
     * so if needed, we recreate a new CallableCFunctionPtr with the same ptr
     * (retrieved with cfptr->get()) and slice variables just created to
     * store the results
     */
    CallableCFunctionPtr* cfptr(dynamic_cast<CallableCFunctionPtr*>(function));
    if(cfptr)
    {
        types::typed_list tmp;
        createSlices(out.begin(), out.end(), std::back_inserter(tmp));
        CallableCFunctionPtr* cfptrWithResults(new CallableCFunctionPtr(cfptr->get(), tmp.begin(), tmp.end()));
        delete cfptr;
        cfptr= cfptrWithResults;
    }

}
void ParallelRunner::writeSlice(Double& var, std::size_t i, Double& slice)
{
    i %= var.cols_get();
    slice.real_set(var.real_get()+var.rows_get()*i);
    if(var.isComplex())
    {
        slice.img_set(var.img_get() + var.rows_get()*i);
    }
}

void ParallelRunner::writeSlices(std::size_t nth, typed_list& slices)
{
    for(std::size_t i(0); i != nbArgs; ++i)
    {
        switch(in[i]->getType())
        {
        case types::InternalType::RealDouble :
        {
            writeSlice(*(in[i]->getAsDouble()), nth, *(slices[i]->getAsDouble()));
            break;
        }
        default :
        {
#pragma omp critical
            std::wcerr<<L"TODO: implement "<<(in[i]->getTypeStr())<<L" slicing read in parallel_run \n";
        }
        }
    }
}
void ParallelRunner::readSlice(Double CONST& slice, std::size_t i, Double& var)
{
    var.fillFromCol(i, &slice);
}
void ParallelRunner::deleteSlices(typed_list& slices) const
{
    for(typed_list::iterator i(slices.begin()); i != slices.end(); ++i)
    {
        delete *i;
    }
}


void ParallelRunner::readSlices(std::size_t nth, typed_list const& slices)
{
    for(std::size_t i(0); i != min(nbRes, slices.size()); ++i)
    {
        switch(out[i]->getType())
        {
        case types::InternalType::RealDouble :
        {
            readSlice(*(slices[i]->getAsDouble()), nth, *(out[i]->getAsDouble()));
            break;
        }
        default :
        {
#pragma omp critical
            std::wcerr<<L"TODO: implement "<<(in[i]->getTypeStr())<<L" slicing read in parallel_run \n";
        }
        }
    }
}


types::InternalType* ParallelRunner::newResult(std::size_t i) const
{ // @TODO-return-type handle more result types when given as typeinfo parameter cf @TODO-return-type in ::create()
        return new Double(resultSize[i], n , in[i]->getAsDouble()->isComplex());
}

}
