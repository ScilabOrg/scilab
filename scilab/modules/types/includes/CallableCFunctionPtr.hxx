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
#ifndef CALLABLE_C_FUNCTION_PTR_HXX
#define CALLABLE_C_FUNCTION_PTR_HXX

#include <iterator>
#include <vector>
#include "callable.hxx"
//#include "dynamic_link.h"

namespace types
{
    /*
     * This kind of Scilab variable is used to store ptr to foreign compiled function with C (Fortran , C++, ...)
     * linkage.
     * The prototype of the functions must be void(void const* const*, void*const*), taking two array of ptr to data
     * from the Scilab variables passed as arguments and expected as result.
     * The expected result variables must be known beforehand, contrary to other Scilab Callables, so we have
     * constructors taking a sequence of types::InternalType*.
     * On the other hand, the ast::ConstVisitor* argument of the inherited Callable::call() member function
     * is not used, so it is made optional (default to 0).
     */
    struct CallableCFunctionPtr : Callable
    {
        typedef void (*functionToCall_t)(void const*const*, void*const*);
        typedef void (*loadedFunction_t)();

        /* The default constructor gives a default function used to test purposes.
         * The test function expects a scalar as input argument, and will return a scalar as result.
         * http://en.wikipedia.org/wiki/Hofstadter_sequence#Hofstadter_Female_and_Male_sequences
         */
        CallableCFunctionPtr();
        /*
         *@param f ptr to native function to be called. Must not return any result as no result variable is given.
         */
        explicit CallableCFunctionPtr(functionToCall_t f){ function.toCall= f; }
        /* This overload is a convenience for generic ptr to function as probably stored in an ABI bolivious way
         * as a void*()(void).
         *@param f ptr to native function to be called. Must not return any result as no result variable is given.
         */
        explicit CallableCFunctionPtr(loadedFunction_t f){ function.toLoad= f; }
        /*
         * This constructor is used to give the result signature that will be used when call()-ing the CallableCFunctionPtr.
         * @param f ptr to native function to be called. Must not return any result as no result variable is given.
         * @param b Iterator to the first types::InternalType* describing the first result of the sequence.
         * @param e Iterator past the end of the sequence of types::InternalType* describing the results.
         */
        template<typename In>
        CallableCFunctionPtr(functionToCall_t f, In resultB, In resultE) : out(resultB, resultE){ function.toCall= f; }
        /* This overload is a convenience for generic ptr to function as probably stored in an ABI bolivious way
         * as a void*()(void).
         * This constructor is used to give the result signature that will be used when call()-ing the CallableCFunctionPtr.
         * @param f ptr to native function to be called. Must not return any result as no result variable is given.
         * @param b Iterator to the first types::InternalType* describing the first result of the sequence.
         * @param e Iterator past the end of the sequence of types::InternalType* describing the results.
         */
        template<typename In>
        explicit CallableCFunctionPtr(loadedFunction_t f, In resultB, In resultE) : out(resultB, resultE){ function.toLoad= f; }

        virtual Callable::ReturnValue call(types::typed_list &in, int nbResIsUnused, types::typed_list &out, ast::ConstVisitor* execFuncIsUnused=0);

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::wstring  getTypeStr();
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::wstring  getShortTypeStr();
        virtual Callable*     clone(void);
        virtual std::wstring  toString(int _iPrecison, int _iLineLen);
        virtual RealType      getType(void);
        /*
         * @return the ptr to the native c function.
         */
        functionToCall_t get() const { return function.toCall;}
    private:
        typed_list out;
        union
        {
            functionToCall_t toCall;
            loadedFunction_t toLoad;
        } function;

    };
}
#endif
