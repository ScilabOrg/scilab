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

#include "alltypes.hxx"
#include "execvisitor.hxx"
#include "context.hxx"
extern "C"
{
#include "Scierror.h"
#include "localization.h"
}
#include <omp.h>
#include <typeinfo>
using namespace types;


using std::min; // or the VS macros :(
using std::max;

#define CONST

namespace
{
    struct ParallelRunner
    {
        ParallelRunner(typed_list& in, std::size_t nbRes, typed_list& out)
            : in(in), out(out), nbRes(nbRes), funPos(0), ok(in.size() >=2), workers(omp_get_num_procs()), n(0)
        {
            if(ok)
            {
                for( std::size_t pos(0); (pos != in.size()) && ok; ++pos)
                {
                    types::InternalType::RealType const type(in[pos]->getType());
                    if (!funPos)
                    {
                        switch (type)
                        {
                        case types::InternalType::RealDouble :
                        {
                            /* check not complex "%s: This feature has not been implemented.\n" */
                            //ok= true;// !isVarComplex( pvApiCtx, addr); should handle cplx
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
                    {
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
                ok &= n && funPos;
                if (ok)
                {
                    for(std::size_t i(0); i!= nbRes; ++i)
                    {
                        out.push_back(newResult(i));
                    }
                }
            }
        }

        Callable* getFunction(std::size_t pos) {
            funPos= pos;
            String* str(in[pos]->getAsString());
            return str
                ? dynamic_cast<Callable*>(symbol::Context::getInstance()->get(str->string_get(0,0)))
                : dynamic_cast<Callable*>(in[pos]);
        }

        void operator()()
        {
#ifdef SINGLE_THREAD
            workers=1;
#endif
            omp_set_num_threads(workers);
                // TODO call prologue(omp_get_thread_num())

            std::cerr<<" n="<<n<<std::endl;
#pragma omp parallel
            {
                int const id(omp_get_thread_num());
                symbol::Context::initThreadedContext();
#pragma omp barrier // wait for each thread to have the correct init context
                ast::ExecVisitor visit;
                std::size_t i(0);
#pragma omp for private(i, visit) schedule(static, 1)
                for (i =0; i < n; ++i)
                {
#pragma omp critical
                    std::cerr<<"in thread "<<id<<std::endl;
                    typed_list fIn(funPos), fOut;
                    writeSlices( i, fIn);
#define SERIALIZE_THREADS
#ifdef SERIALIZE_THREADS
#pragma omp critical (macro)
#endif
                    fun->call(fIn, nbRes, fOut, &visit);
                    readSlices(i, fOut);
                }
            }

                // TODO call epilogue(omp_get_thread_num())
            }



        std::size_t nCalls() const
        {
            return n;
        }

        std::size_t nbArgs() const
        {
            return funPos;
        }

        template<typename T>
        static void writeSlice(T& var, std::size_t i, T& slice)
        { // TODO perform dynamic dispatching
            writeSlice(*var.getAsDouble(), i, *slice.getAsDouble());
        }
        void writeSlices(std::size_t nth, typed_list& slices)
        {
            for(std::size_t i(0); i != nbArgs(); ++i)
            {
                slices[i]=newSlice(i);
                writeSlice(*in[i], nth, *slices[i]);
            }
        }

        template<typename T>
        static void readSlice(T CONST& slice, std::size_t i, T& var)
        {

//                std::cerr<<"slice is "<<typeid(slice).name()<<" and var is "<<typeid(var).name()<<std::endl;
                readSlice(*slice.getAsDouble(), i, *var.getAsDouble());
         }

        void readSlices(std::size_t nth, typed_list const& slices)
        {
            for(std::size_t i(0); i != min(nbRes, slices.size()); ++i)
            {
                readSlice(*slices[i], nth, *out[i]);
            }
        }


        bool isValid() const
        {
            return ok;
        }

        std::size_t nbWorkers() const
        {
            return workers;
        }

        Callable& function()
        {
            return *fun;
        }

        Double* newSlice(std::size_t i) CONST
        {
            return new Double(in[i]->getAsDouble()->rows_get(), 1, in[i]->getAsDouble()->isComplex());

        }
        Double* newResult(std::size_t i) CONST
        {
            return new Double(resultSize[i], n , in[i]->getAsDouble()->isComplex());
        }

        typed_list& in;
        typed_list& out;

        std::size_t nbRes, funPos;
        int workshare;
        bool ok;
        std::size_t workers, n;
        Callable* fun;
        std::vector<std::size_t> resultSize;
    };
    template<>
    void ParallelRunner::writeSlice(Double& var, std::size_t i, Double& slice)
        {
            i %= var.cols_get();
//            std::cerr<<"writing to slice :"<<i<<" the value "<<*(var.real_get()+var.rows_get()*i)<<std::endl;
            slice.real_set(var.real_get()+var.rows_get()*i);
            if(var.isComplex())
            {
                slice.img_set(var.img_get() + var.rows_get()*i);
            }
        }
    template<>
    void ParallelRunner::readSlice(Double CONST& slice, std::size_t i, Double& var)
    {
        //      std::cerr<<"reading from slice :"<<i<<" the value "<<slice.real_get(0,0)<<std::endl;
        var.fillFromCol(i, &slice);
    }

}
Function::ReturnValue sci_parallel_run(typed_list &in, int nbRes, typed_list &out)
{
    std::cerr<<"in parallel_run\n";
#if 0
    typed_list fIn;
    ast::ExecVisitor v;
    fIn.push_back(in[0]);
    String* str(in[1]->getAsString());
    (str
     ? dynamic_cast<Callable*>(symbol::Context::getInstance()->get(str->string_get(0,0)))
     : dynamic_cast<Callable*>(in[1]))->call(fIn, nbRes, out, &v);

    return Function::OK;
#endif
    ParallelRunner runner(in, nbRes, out);
    if (!runner.isValid())
    {
        std::cerr<<"illegal Args!\n";
        return Function::Error;
    }
    runner();
    return Function::OK;
}
