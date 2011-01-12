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
#include "ProcessesScheduler.hxx"
#include "ThreadsScheduler.hxx"

namespace parallel
{
    ParallelRunner::ParallelRunner(typed_list& args, std::size_t res, typed_list& o)
        : in(args), out(o), nbRes(res), n(0), nbArgs(0), chunkSize(1), nbWorkers(omp_get_num_procs()), dynamic(false), ok(in.size() >=2),
          fun(0), prologue(0), epilogue(0)
    {
        if(ok)
        {
            for( std::size_t pos(0); (pos != in.size()) && ok; ++pos)
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
                        n= max(n, static_cast<std::size_t>(in[pos]->getAsDouble()->cols_get()));
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
                        fun= getFunction(pos);
                        std::vector<std::size_t> defaultSizes(pos, 1);
                        resultSize.swap(defaultSizes); // init sizes with default value 1 (= scalar)
                        break;
                    }
                    default : {
                        Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),"parallel_run", pos+1);
                        ok= false;
                    }
                    }
                }
                else
                { // TODO: get parameters (prologue, epilogue, nb_workers,shared_memory,  dynamic_scheduling, chunk_size
                    switch (type)
                    {
                    case types::InternalType::RealString :
                    {
                        // TODO store type info
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
                                resultSize[i]= (d.cols_get() == 1)
                                    ? d.real_get(i, 0)
                                    : d.real_get(i, 0) *  d.real_get(i, 1);
                            }
                            break;
                        }
                    }
                    case types::InternalType::RealMList :
                    { /* config plist (which is a mlist) */
                        break;
                    }
                    default :
                    {
                        ok= false;
                    }
                    }
                }
            }
            ok &= n && nbArgs;
            if (ok)
            {
                for(std::size_t i(0); i!= nbRes; ++i)
                {
                    out.push_back(newResult(i));
                }
            }
        }
    }

    void ParallelRunner::operator()(bool withThreads)
    {
        if(withThreads)
        {
            ThreadsScheduler runWithThreads(*this);
            runWithThreads();
        }
        else
        {
            ProcessesScheduler runWithProcesses(*this);
            runWithProcesses();
        }
    }

    void ParallelRunner::call(ParallelRunner::CallablePtr f, std::size_t id, ast::ExecVisitor& exec) const
    {
        if(f)
        {
            Double* arg= new types::Double(1, 1, false);
            arg->val_set(0,0,static_cast<double>(id));
            typed_list i(1,arg), o;
            f->call(i, 0, o, &exec);
        }
    }

    bool ParallelRunner::isValid() const
    {
        return ok;
    }

    ParallelRunner::CallablePtr ParallelRunner::getFunction(std::size_t pos)
    {
        nbArgs= pos;
        String* str(in[pos]->getAsString());
        return str
            ? dynamic_cast<ParallelRunner::CallablePtr>(symbol::Context::getInstance()->get(str->string_get(0,0)))
            : dynamic_cast<ParallelRunner::CallablePtr>(in[pos]);
    }

    types::InternalType* ParallelRunner::newSlice(types::InternalType* i) const
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
            switch(in[i]->getType())
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
    {
        return new Double(resultSize[i], n , in[i]->getAsDouble()->isComplex());
    }

}
