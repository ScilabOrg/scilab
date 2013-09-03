/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __SCILABABSTRACTMEMORYALLOCATOR_H__
#define __SCILABABSTRACTMEMORYALLOCATOR_H__

extern "C"
{
#include "api_scilab.h"
}
#include "ScilabAbstractEnvironmentWrapper.hxx"
#include "ScilabAbstractEnvironmentException.hxx"

#include <iostream>

namespace org_modules_external_objects
{

class ComplexDataPointers
{
public:

    ComplexDataPointers(double * _realPtr, double * _imagPtr) : realPtr(_realPtr), imagPtr(_imagPtr) { }
    ComplexDataPointers() : realPtr(0), imagPtr(0) { }
    ~ComplexDataPointers() { }

    double * const realPtr;
    double * const imagPtr;
};

class ScilabStackAllocator
{

public:

    ScilabStackAllocator(void * pvCtx, int _position) : position(_position), pvCtx(pvCtx) { }

    ~ScilabStackAllocator() { }

protected:

    int position;
    void * pvCtx;

    inline static void create(void * pvCtx, const int position, const int rows, const int cols, double * ptr)
    {
        SciErr err = createMatrixOfDouble(pvCtx, position, rows, cols, ptr);
        checkError(err);
    }

    inline static double * alloc(void * pvCtx, const int position, const int rows, const int cols, double * ptr)
    {
        double * _ptr = 0;
        SciErr err = allocMatrixOfDouble(pvCtx, position, rows, cols, &_ptr);
        checkError(err);

        return _ptr;
    }

    inline static void create(void * pvCtx, const int position, const int rows, const int cols, float * ptr)
    {
        double * _ptr = alloc(pvCtx, position, rows, cols, (double *)0);
        for (int i = 0; i < rows * cols; i++)
        {
            _ptr[i] = static_cast<double>(ptr[i]);
        }
    }

    inline static float * alloc(void * pvCtx, const int position, const int rows, const int cols, float * ptr)
    {
        return (float *)alloc(pvCtx, position, rows, cols, (double *)0);
    }

    inline static void create(void * pvCtx, const int position, const int rows, const int cols, double * re, double * im)
    {
        SciErr err = createComplexMatrixOfDouble(pvCtx, position, rows, cols, re, im);
        checkError(err);
    }

    inline static ComplexDataPointers alloc(void * pvCtx, const int position, const int rows, const int cols, double * re, double * im)
    {
        double * _re = 0, * _im = 0;
        SciErr err = allocComplexMatrixOfDouble(pvCtx, position, rows, cols, &_re, &_im);
        checkError(err);

        return ComplexDataPointers(_re, _im);
    }

    inline static void create(void * pvCtx, const int position, const int rows, const int cols, char * ptr)
    {
        SciErr err = createMatrixOfInteger8(pvCtx, position, rows, cols, ptr);
        checkError(err);
    }

    inline static char * alloc(void * pvCtx, const int position, const int rows, const int cols, char * ptr)
    {
        char * _ptr = 0;
        SciErr err = allocMatrixOfInteger8(pvCtx, position, rows, cols, &_ptr);
        checkError(err);

        return _ptr;
    }

    inline static void create(void * pvCtx, const int position, const int rows, const int cols, unsigned char * ptr)
    {
        SciErr err = createMatrixOfUnsignedInteger8(pvCtx, position, rows, cols, ptr);
        checkError(err);
    }

    inline static unsigned char * alloc(void * pvCtx, const int position, const int rows, const int cols, unsigned char * ptr)
    {
        unsigned char * _ptr = 0;
        SciErr err = allocMatrixOfUnsignedInteger8(pvCtx, position, rows, cols, &_ptr);
        checkError(err);

        return _ptr;
    }

    inline static void create(void * pvCtx, const int position, const int rows, const int cols, short * ptr)
    {
        SciErr err = createMatrixOfInteger16(pvCtx, position, rows, cols, ptr);
        checkError(err);
    }

    inline static short * alloc(void * pvCtx, const int position, const int rows, const int cols, short * ptr)
    {
        short * _ptr = 0;
        SciErr err = allocMatrixOfInteger16(pvCtx, position, rows, cols, &_ptr);
        checkError(err);

        return _ptr;
    }

    inline static void create(void * pvCtx, const int position, const int rows, const int cols, unsigned short * ptr)
    {
        SciErr err = createMatrixOfUnsignedInteger16(pvCtx, position, rows, cols, ptr);
        checkError(err);
    }

    inline static unsigned short * alloc(void * pvCtx, const int position, const int rows, const int cols, unsigned short * ptr)
    {
        unsigned short * _ptr = 0;
        SciErr err = allocMatrixOfUnsignedInteger16(pvCtx, position, rows, cols, &_ptr);
        checkError(err);

        return _ptr;
    }

    inline static void create(void * pvCtx, const int position, const int rows, const int cols, int * ptr)
    {
        SciErr err = createMatrixOfInteger32(pvCtx, position, rows, cols, ptr);
        checkError(err);
    }

    inline static int * alloc(void * pvCtx, const int position, const int rows, const int cols, int * ptr)
    {
        int * _ptr = 0;
        SciErr err = allocMatrixOfInteger32(pvCtx, position, rows, cols, &_ptr);
        checkError(err);

        return _ptr;
    }

    inline static void create(void * pvCtx, const int position, const int rows, const int cols, unsigned int * ptr)
    {
        SciErr err = createMatrixOfUnsignedInteger32(pvCtx, position, rows, cols, ptr);
        checkError(err);
    }

    inline static unsigned int * alloc(void * pvCtx, const int position, const int rows, const int cols, unsigned int * ptr)
    {
        unsigned int * _ptr = 0;
        SciErr err = allocMatrixOfUnsignedInteger32(pvCtx, position, rows, cols, &_ptr);
        checkError(err);

        return _ptr;
    }

#ifdef __SCILAB_INT64__

    inline static void create(void * pvCtx, const int position, const int rows, const int cols, long long * ptr)
    {
        SciErr err = createMatrixOfInteger64(pvCtx, position, rows, cols, ptr);
        checkError(err);
    }

    inline static long long * alloc(void * pvCtx, const int position, const int rows, const int cols, long long * ptr)
    {
        long long * _ptr = 0;
        SciErr err = allocMatrixOfInteger64(pvCtx, position, rows, cols, &_ptr);
        checkError(err);

        return _ptr;
    }

    inline static void create(void * pvCtx, const int position, const int rows, const int cols, unsigned long long * ptr)
    {
        SciErr err = createMatrixOfUnsignedIntege64(pvCtx, position, rows, cols, ptr);
        checkError(err);
    }

    inline static unsigned long long * alloc(void * pvCtx, const int position, const int rows, const int cols, unsigned long long * ptr)
    {
        unsigned long long * _ptr = 0;
        SciErr err = allocMatrixOfUnsignedInteger64(pvCtx, position, rows, cols, &_ptr);
        checkError(err);

        return_ ptr;
    }

#else

    inline static void create(void * pvCtx, const int position, const int rows, const int cols, long long * ptr)
    {
        int * dataPtr = alloc(pvCtx, position, rows, cols, (int *)0);
        for (int i = 0; i < rows * cols; i++)
        {
            dataPtr[i] = static_cast<int>(ptr[i]);
        }
    }

    inline static long long * alloc(void * pvCtx, const int position, const int rows, const int cols, long long * ptr)
    {
        return (long long *)alloc(pvCtx, position, rows, cols, (int *)0);
    }

    inline static void create(void * pvCtx, const int position, const int rows, const int cols, unsigned long long * ptr)
    {
        unsigned int * dataPtr = alloc(pvCtx, position, rows, cols, (unsigned int *)0);
        for (int i = 0; i < rows * cols; i++)
        {
            dataPtr[i] = static_cast<unsigned int>(ptr[i]);
        }
    }

    inline static unsigned long long * alloc(void * pvCtx, const int position, const int rows, const int cols, unsigned long long * ptr)
    {
        return (unsigned long long *)alloc(pvCtx, position, rows, cols, (unsigned int *)0);
    }

#endif

    inline static void create(void * pvCtx, const int position, const int rows, const int cols, char ** ptr)
    {
        SciErr err = createMatrixOfString(pvCtx, position, rows, cols, const_cast<const char * const *>(ptr));
        checkError(err);
    }

    inline static char ** alloc(void * pvCtx, const int position, const int rows, const int cols, char ** ptr)
    {
        throw ScilabAbstractEnvironmentException("Invalid operation: cannot allocate a matrix of String");
    }

    inline static void createBool(void * pvCtx, const int position, const int rows, const int cols, int * ptr)
    {
        SciErr err = createMatrixOfBoolean(pvCtx, position, rows, cols, ptr);
        checkError(err);
    }

    inline static int * allocBool(void * pvCtx, const int position, const int rows, const int cols, int * ptr)
    {
        int * _ptr = 0;
        SciErr err = allocMatrixOfBoolean(pvCtx, position, rows, cols, &_ptr);
        checkError(err);

        return _ptr;
    }


private:

    inline static void checkError(const SciErr & err)
    {
        if (err.iErr)
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, "Cannot allocate memory");
        }
    }

};

template <typename T>
class ScilabSingleTypeStackAllocator : public ScilabStackAllocator
{

public:

    ScilabSingleTypeStackAllocator(void * _pvCtx, int _position) : ScilabStackAllocator(_pvCtx, _position) { }

    ~ScilabSingleTypeStackAllocator() { }

    virtual T * allocate(const int rows, const int cols, T * dataPtr) const
    {
        if (!rows || !cols)
        {
            createEmptyMatrix(pvCtx, position);
            return 0;
        }

        if (dataPtr)
        {
            create(pvCtx, position, rows, cols, dataPtr);
            return 0;
        }
        else
        {
            return alloc(pvCtx, position, rows, cols, dataPtr);
        }
    }
};

typedef ScilabSingleTypeStackAllocator<double> ScilabDoubleStackAllocator;
typedef ScilabSingleTypeStackAllocator<char *> ScilabStringStackAllocator;
typedef ScilabSingleTypeStackAllocator<char> ScilabCharStackAllocator;
typedef ScilabSingleTypeStackAllocator<unsigned char> ScilabUCharStackAllocator;
typedef ScilabSingleTypeStackAllocator<short> ScilabShortStackAllocator;
typedef ScilabSingleTypeStackAllocator<unsigned short> ScilabUShortStackAllocator;
typedef ScilabSingleTypeStackAllocator<int> ScilabIntStackAllocator;
typedef ScilabSingleTypeStackAllocator<unsigned int> ScilabUIntStackAllocator;
typedef ScilabSingleTypeStackAllocator<long long> ScilabLongStackAllocator;
typedef ScilabSingleTypeStackAllocator<unsigned long long> ScilabULongStackAllocator;
typedef ScilabSingleTypeStackAllocator<float> ScilabFloatStackAllocator;

class ScilabComplexStackAllocator : public ScilabStackAllocator
{

public:

    ScilabComplexStackAllocator(void * _pvCtx, int _position) : ScilabStackAllocator(_pvCtx, _position) { }

    ~ScilabComplexStackAllocator() { }

    ComplexDataPointers allocate(const int rows, const int cols, double * realPtr, double * imagPtr) const
    {
        if (!rows || !cols)
        {
            createEmptyMatrix(pvCtx, position);
            return ComplexDataPointers();
        }

        if (realPtr && imagPtr)
        {
            create(pvCtx, position, rows, cols, realPtr, imagPtr);
            return ComplexDataPointers();
        }
        else
        {
            return alloc(pvCtx, position, rows, cols, realPtr, imagPtr);
        }
    }
};

class ScilabBooleanStackAllocator : public ScilabSingleTypeStackAllocator<int>
{

public:

    ScilabBooleanStackAllocator(void * _pvCtx, int _position) : ScilabSingleTypeStackAllocator<int>(_pvCtx, _position) { }

    ~ScilabBooleanStackAllocator() { }

    int * allocate(const int rows, const int cols, int * dataPtr) const
    {
        if (!rows || !cols)
        {
            createEmptyMatrix(pvCtx, position);
            return 0;
        }

        if (dataPtr)
        {
            createBool(pvCtx, position, rows, cols, dataPtr);
            return 0;
        }
        else
        {
            return allocBool(pvCtx, position, rows, cols, dataPtr);
        }
    }

    template <typename T>
    int * allocate(const int rows, const int cols, T * dataPtr) const
    {
        if (!rows || !cols)
        {
            createEmptyMatrix(pvCtx, position);
            return 0;
        }

        if (dataPtr)
        {
            int * ptr = 0;
            allocBool(pvCtx, position, rows, cols, ptr);
            for (int i = 0; i < rows * cols; i++)
            {
                ptr[i] = static_cast<int>(dataPtr[i]);
            }

            return 0;
        }
        else
        {
            throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, "Invalid operation: cannot allocate a matrix of Boolean");
        }
    }
};

}

#endif // __SCILABABSTRACTMEMORYALLOCATOR_H__
