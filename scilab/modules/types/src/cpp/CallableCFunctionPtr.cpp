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
#include "CallableCFunctionPtr.hxx"
#include "double.hxx"
#include <iostream>
#include <algorithm>
#include <sstream>
namespace
{
    /*
     * dummy function used for test only
     */
    int m(int);
    int f(int n) { return n ? (n-m(f(n-1))) : 1 ; }
    int m(int n) { return n ? (n-f(m(n-1))) : 0 ; }
    void test_mf(void const* const* args, void *const* res)
    { *((double*)*res)= (double)m((int)((double*)args[0])[0]); }

    /*
     * functor used to get the ptr to raw data from various
     * Scilab types.
     * It cannot be a simple  functor because a single Scilab Var
     * can have more than one data ptrs (e.g. complex have one real data ptr
     * and one imaginary data ptr.
     * So we define a storage class that stores data ptrs into an underlying
     * output iterator. One call of the storage functor can output any number
     * of pointers.
     */
    template<typename It>
    struct DataPtrStorage
    {
        explicit DataPtrStorage(It i) : it(i)
        {
        }
        It operator()(types::InternalType* i)
        {
        switch(i->getType())
        {
        case types::InternalType::RealDouble :
        {
            types::Double &d(*i->getAsDouble());
            *it= static_cast<void*>(d.real_get());
            if (d.isComplex())
            {
                ++it;
                *it= static_cast<void*>(d.img_get());
            }
            break;
        }
        default:
        {
            *it= 0;
            std::wcerr<<i->getTypeStr()<<L" not implemented yet for foreign compiled function calls in parallel_run\n";
        }
        }
        ++it;
        return it;
        }
    private:
        It it;
    };
    // convenience function
    template<typename It> DataPtrStorage<It> makeDataPtrStorage(It it)
    {
        return DataPtrStorage<It>(it);
    }
}
namespace types
{
    CallableCFunctionPtr::CallableCFunctionPtr(){ function.toCall= &test_mf; out.push_back(new Double(1,1,false));}
    Callable::ReturnValue CallableCFunctionPtr::call(types::typed_list &in, int unused, types::typed_list &out_, ast::ConstVisitor* execFunc)
    {
        out_=out; // we will return the output variables stored in out
        std::vector<void*> inData, outData; // temporary storage for ptr to data in input/ output Scilab variables
        std::for_each(in.begin(), in.end(), makeDataPtrStorage(std::back_inserter(inData)));
        std::for_each(out.begin(), out.end(), makeDataPtrStorage(std::back_inserter(outData)));
        function.toCall(static_cast<void**>(&inData[0]), static_cast<void**>(&outData[0]));//23.2.4 §1 is our friend
    }

    std::wstring CallableCFunctionPtr::getTypeStr() {return L"callable external function";}

    std::wstring CallableCFunctionPtr::getShortTypeStr() { return L"?"; } // TODO

    Callable* CallableCFunctionPtr::clone(void) { return new CallableCFunctionPtr(*this);};

    std::wstring CallableCFunctionPtr::toString(int _iPrecison, int _iLineLen)
    {
        std::wostringstream os;
        os<<L"external function @"<<function.toCall<<std::endl;
        return os.str();
    }

    types::InternalType::RealType CallableCFunctionPtr::getType(void) { return InternalType::RealCallableCFunctionPtr; }
}
