/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Baozeng DING
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "ScilabPythonEnvironmentWrapper.hxx"

namespace org_modules_external_objects_python
{

VariableType ScilabPythonEnvironmentWrapper::isunwrappable(int id) const
{
    PyObject * obj = scope.getObject(id);

    if (!PyList_Check(obj))
    {
        // This is not a list
        // So this is a single element or a Numpy array
        if (PyFloat_Check(obj))
        {
            return SingleDouble;
        }
        else if (PyComplex_Check(obj))
        {
            return SingleComplex;
        }
        else if (PyBool_Check(obj))
        {
            return SingleBoolean;
        }
        else if (PyInt_Check(obj))
        {
            return SingleInt;
        }
        else if (PyString_Check(obj))
        {
            return SingleString;
        }
        else if (PyArray_Check(obj))
        {
            PyArrayObject * arr = (PyArrayObject *)obj;
            int ndim = PyArray_NDIM(arr);
            if (ndim > 2)
            {
                // TODO: must handle hypermatrices
                return Nothing;
            }

            int type = PyArray_TYPE(arr);
            switch (type)
            {
                case PyArray_DOUBLE:
                    return ndim == 1 ? RowDouble : MatDouble;
                case PyArray_BYTE:
                    return ndim == 1 ? RowChar : MatChar;
                case PyArray_UBYTE:
                    return ndim == 1 ? RowUChar : MatUChar;
                case PyArray_SHORT:
                    return ndim == 1 ? RowShort : MatShort;
                case PyArray_USHORT:
                    return ndim == 1 ? RowUShort : MatUShort;
                case PyArray_INT:
                    return ndim == 1 ? RowInt : MatInt;
                case PyArray_UINT:
                    return ndim == 1 ? RowUInt : MatUInt;

#ifdef __SCILAB_INT64__
                case PyArray_LONG:
                    return ndim == 1 ? RowLong : MatLong;
                case PyArray_ULONG:
                    return ndim == 1 ? RowULong : MatULong;
#endif

                case PyArray_STRING:
                    return ndim == 1 ? RowString : MatString;
                case PyArray_CDOUBLE:
                    return ndim == 1 ? RowComplex : MatComplex;
                default:
                    return Nothing;
            }
        }
    }
    else
    {
        // This is a list
        int size = PyList_Size(obj);
        if (size == 0)
        {
            return Nothing;
        }

        PyObject * first = PyList_GetItem(obj, 0);
        PyTypeObject * type = PyList_GetItem(obj, 0)->ob_type;

        for (int i = 1; i < size; i++)
        {
            if (!PyObject_TypeCheck(PyList_GetItem(obj, i), type))
            {
                return Nothing;
            }
        }

        // Here all the elements have the same type
        if (PyFloat_Check(first))
        {
            return RowDouble;
        }
        else if (PyComplex_Check(obj))
        {
            return RowComplex;
        }
        else if (PyBool_Check(first))
        {
            return RowBoolean;
        }
        else if (PyInt_Check(first))
        {
            return RowInt;
        }
        else if (PyString_Check(first))
        {
            return RowString;
        }
        else if (PyList_Check(first))
        {
            // We probably have a matrix
            int cols = PyList_Size(first);
            if (cols == 0)
            {
                return Nothing;
            }

            for (int i = 1; i < size; i++)
            {
                if (PyList_Size(PyList_GetItem(obj, i)) != cols)
                {
                    return Nothing;
                }
            }

            first = PyList_GetItem(first, 0);
            type = first->ob_type;
            for (int i = 0; i < size; i++)
            {
                PyObject * list = PyList_GetItem(obj, i);
                for (int j = 0; j < cols; j++)
                {
                    if (!PyObject_TypeCheck(PyList_GetItem(list, j), type))
                    {
                        return Nothing;
                    }
                }
            }

            // Here all the elements have thje same type
            if (PyFloat_Check(first))
            {
                return MatDouble;
            }
            else if (PyComplex_Check(obj))
            {
                return MatComplex;
            }
            else if (PyBool_Check(first))
            {
                return MatBoolean;
            }
            else if (PyInt_Check(first))
            {
                return MatInt;
            }
            else if (PyString_Check(first))
            {
                return MatString;
            }
        }
    }

    return Nothing;
}

int ScilabPythonEnvironmentWrapper::wrap(double * x, const bool isRef) const
{
    return wrapData<double>(x, isRef);
}

int ScilabPythonEnvironmentWrapper::wrap(double * x, int xSize, const bool isRef) const
{
    return wrapData<double>(x, xSize, isRef);
}

int ScilabPythonEnvironmentWrapper::wrap(double * x, int xSize, int xSizeCol, const bool isRef) const
{
    return wrapData<double>(x, xSize, xSizeCol, isRef);
}

int ScilabPythonEnvironmentWrapper::wrap(double * re, double * im, const bool isRef) const
{
    if (helper.getWrapSingleWithNumpy())
    {
        return wrapData(re, im, 1, isRef);
    }
    else
    {
        return wrapData(re, im, isRef);
    }
}

int ScilabPythonEnvironmentWrapper::wrap(double * re, double * im, int xSize, const bool isRef) const
{
    return wrapData(re, im, xSize, isRef);
}

int ScilabPythonEnvironmentWrapper::wrap(double * re, double * im, int xSize, int xSizeCol, const bool isRef) const
{
    return wrapData(re, im, xSize, xSizeCol, isRef);
}

int ScilabPythonEnvironmentWrapper::wrap(char * x, const bool isRef) const
{
    return wrapData<char>(x, isRef);
}

int ScilabPythonEnvironmentWrapper::wrap(char * x, int xSize, const bool isRef) const
{
    return wrapData<char>(x, xSize, isRef);
}

int ScilabPythonEnvironmentWrapper::wrap(char * x, int xSize, int xSizeCol, const bool isRef) const
{
    return wrapData<char>(x, xSize, xSizeCol, isRef);
}

int ScilabPythonEnvironmentWrapper::wrap(unsigned char * x, const bool isRef) const
{
    return wrapData<unsigned char>(x, isRef);
}

int ScilabPythonEnvironmentWrapper::wrap(unsigned char * x, int xSize, const bool isRef) const
{
    return wrapData<unsigned char>(x, xSize, isRef);
}

int ScilabPythonEnvironmentWrapper::wrap(unsigned char * x, int xSize, int xSizeCol, const bool isRef) const
{
    return wrapData<unsigned char>(x, xSize, xSizeCol, isRef);
}

int ScilabPythonEnvironmentWrapper::wrap(short * x, const bool isRef) const
{
    return wrapData<short>(x, isRef);
}

int ScilabPythonEnvironmentWrapper::wrap(short * x, int xSize, const bool isRef) const
{
    return wrapData<short>(x, xSize, isRef);
}

int ScilabPythonEnvironmentWrapper::wrap(short * x, int xSize, int xSizeCol, const bool isRef) const
{
    return wrapData<short>(x, xSize, xSizeCol, isRef);
}

int ScilabPythonEnvironmentWrapper::wrap(unsigned short * x, const bool isRef) const
{
    return wrapData<unsigned short>(x, isRef);
}

int ScilabPythonEnvironmentWrapper::wrap(unsigned short * x, int xSize, const bool isRef) const
{
    return wrapData<unsigned short>(x, xSize, isRef);
}

int ScilabPythonEnvironmentWrapper::wrap(unsigned short * x, int xSize, int xSizeCol, const bool isRef) const
{
    return wrapData<unsigned short>(x, xSize, xSizeCol, isRef);
}

int ScilabPythonEnvironmentWrapper::wrap(int * x, const bool isRef) const
{
    return wrapData<int>(x, isRef);
}

int ScilabPythonEnvironmentWrapper::wrap(int * x, int xSize, const bool isRef) const
{
    return wrapData<int>(x, xSize, isRef);
}

int ScilabPythonEnvironmentWrapper::wrap(int * x, int xSize, int xSizeCol, const bool isRef) const
{
    return wrapData<int>(x, xSize, xSizeCol, isRef);
}

int ScilabPythonEnvironmentWrapper::wrap(unsigned int * x, const bool isRef) const
{
    return wrapData<unsigned int>(x, isRef);
}

int ScilabPythonEnvironmentWrapper::wrap(unsigned int * x, int xSize, const bool isRef) const
{
    return wrapData<unsigned int>(x, xSize, isRef);
}

int ScilabPythonEnvironmentWrapper::wrap(unsigned int * x, int xSize, int xSizeCol, const bool isRef) const
{
    return wrapData<unsigned int>(x, xSize, xSizeCol, isRef);
}

#ifdef __SCILAB_INT64__

int ScilabPythonEnvironmentWrapper::wrap(long long * x, const bool isRef) const
{
    return wrapData<long>(x, isRef);
}

int ScilabPythonEnvironmentWrapper::wrap(long long * x, int xSize, const bool isRef) const
{
    return wrapData<long>(x, xSize, isRef);
}

int ScilabPythonEnvironmentWrapper::wrap(long long * x, int xSize, int xSizeCol, const bool isRef) const
{
    return wrapData<long>(x, xSize, xSizeCol, isRef);
}

int ScilabPythonEnvironmentWrapper::wrap(unsigned long long * x, const bool isRef) const
{
    return wrapData<unsigned long>(x, isRef);
}

int ScilabPythonEnvironmentWrapper::wrap(unsigned long long * x, int xSize, const bool isRef) const
{
    return wrapData<unsigned long>(x, xSize, isRef);
}

int ScilabPythonEnvironmentWrapper::wrap(unsigned long long * x, int xSize, int xSizeCol, const bool isRef) const
{
    return wrapData<unsigned long>(x, xSize, xSizeCol, isRef);
}

#endif

int ScilabPythonEnvironmentWrapper::wrap(char ** x, const bool isRef) const
{
    if (helper.getWrapSingleWithNumpy())
    {
        return wrapData(x, 1, isRef);
    }
    else
    {
        return wrapData<char *>(x, isRef);
    }
}

int ScilabPythonEnvironmentWrapper::wrap(char ** x, int xSize, const bool isRef) const
{
    return wrapData(x, xSize, isRef);
}

int ScilabPythonEnvironmentWrapper::wrap(char ** x, int xSize, int xSizeCol, const bool isRef) const
{
    return wrapData(x, xSize, xSizeCol, isRef);
}

int ScilabPythonEnvironmentWrapper::wrapBool(int * x, const bool isRef) const
{
    if (helper.getWrapSingleWithNumpy())
    {
        return wrapData<int, bool, npy_bool>(x, 1, isRef);
    }
    else
    {
        return wrapData<int, bool>(x, isRef);
    }
}

int ScilabPythonEnvironmentWrapper::wrapBool(int * x, int xSize, const bool isRef) const
{
    return wrapData<int, bool, npy_bool>(x, xSize, isRef);
}

int ScilabPythonEnvironmentWrapper::wrapBool(int * x, int xSize, int xSizeCol, const bool isRef) const
{
    return wrapData<int, bool, npy_bool>(x, xSize, xSizeCol, isRef);
}

int ScilabPythonEnvironmentWrapper::wrap(float * x, const bool isRef) const
{
    return wrapData<float>(x, isRef);
}

int ScilabPythonEnvironmentWrapper::wrap(float * x, int xSize, const bool isRef) const
{
    return wrapData<float>(x, xSize, isRef);
}

int ScilabPythonEnvironmentWrapper::wrap(float * x, int xSize, int xSizeCol, const bool isRef) const
{
    return wrapData<float>(x, xSize, xSizeCol, isRef);
}

void ScilabPythonEnvironmentWrapper::unwrapdouble(int id, const ScilabDoubleStackAllocator & allocator) const
{
    pyUnwrapSingle<double>(id, allocator);
}

void ScilabPythonEnvironmentWrapper::unwraprowdouble(int id, const ScilabDoubleStackAllocator & allocator) const
{
    pyUnwrapRow<double>(id, allocator);
}

void ScilabPythonEnvironmentWrapper::unwrapmatdouble(int id, const ScilabDoubleStackAllocator & allocator) const
{
    pyUnwrapMat<double>(id, allocator);
}

void ScilabPythonEnvironmentWrapper::unwrapchar(int id, const ScilabCharStackAllocator & allocator) const
{
    pyUnwrapSingle<char>(id, allocator);
}

void ScilabPythonEnvironmentWrapper::unwraprowchar(int id, const ScilabCharStackAllocator & allocator) const
{
    pyUnwrapRow<char>(id, allocator);
}

void ScilabPythonEnvironmentWrapper::unwrapmatchar(int id, const ScilabCharStackAllocator & allocator) const
{
    pyUnwrapMat<char>(id, allocator);
}

void ScilabPythonEnvironmentWrapper::unwrapuchar(int id, const ScilabUCharStackAllocator & allocator) const
{
    pyUnwrapSingle<unsigned char>(id, allocator);
}

void ScilabPythonEnvironmentWrapper::unwraprowuchar(int id, const ScilabUCharStackAllocator & allocator) const
{
    pyUnwrapRow<unsigned char>(id, allocator);
}

void ScilabPythonEnvironmentWrapper::unwrapmatuchar(int id, const ScilabUCharStackAllocator & allocator) const
{
    pyUnwrapMat<unsigned char>(id, allocator);
}

void ScilabPythonEnvironmentWrapper::unwrapshort(int id, const ScilabShortStackAllocator & allocator) const
{
    pyUnwrapSingle<short>(id, allocator);
}

void ScilabPythonEnvironmentWrapper::unwraprowshort(int id, const ScilabShortStackAllocator & allocator) const
{
    pyUnwrapRow<short>(id, allocator);
}

void ScilabPythonEnvironmentWrapper::unwrapmatshort(int id, const ScilabShortStackAllocator & allocator) const
{
    pyUnwrapMat<short>(id, allocator);
}

void ScilabPythonEnvironmentWrapper::unwrapushort(int id, const ScilabUShortStackAllocator & allocator) const
{
    pyUnwrapSingle<unsigned short>(id, allocator);
}

void ScilabPythonEnvironmentWrapper::unwraprowushort(int id, const ScilabUShortStackAllocator & allocator) const
{
    pyUnwrapRow<unsigned short>(id, allocator);
}

void ScilabPythonEnvironmentWrapper::unwrapmatushort(int id, const ScilabUShortStackAllocator & allocator) const
{
    pyUnwrapMat<unsigned short>(id, allocator);
}

void ScilabPythonEnvironmentWrapper::unwrapint(int id, const ScilabIntStackAllocator & allocator) const
{
    pyUnwrapSingle<int>(id, allocator);
}

void ScilabPythonEnvironmentWrapper::unwraprowint(int id, const ScilabIntStackAllocator & allocator) const
{
    pyUnwrapRow<int>(id, allocator);
}

void ScilabPythonEnvironmentWrapper::unwrapmatint(int id, const ScilabIntStackAllocator & allocator) const
{
    pyUnwrapMat<int>(id, allocator);
}

void ScilabPythonEnvironmentWrapper::unwrapuint(int id, const ScilabUIntStackAllocator & allocator) const
{
    pyUnwrapSingle<unsigned int>(id, allocator);
}

void ScilabPythonEnvironmentWrapper::unwraprowuint(int id, const ScilabUIntStackAllocator & allocator) const
{
    pyUnwrapRow<unsigned int>(id, allocator);
}

void ScilabPythonEnvironmentWrapper::unwrapmatuint(int id, const ScilabUIntStackAllocator & allocator) const
{
    pyUnwrapMat<unsigned int>(id, allocator);
}

void ScilabPythonEnvironmentWrapper::unwraplong(int id, const ScilabLongStackAllocator & allocator) const
{
    pyUnwrapSingle<long long>(id, allocator);
}

void ScilabPythonEnvironmentWrapper::unwraprowlong(int id, const ScilabLongStackAllocator & allocator) const
{
    pyUnwrapRow<long long>(id, allocator);
}

void ScilabPythonEnvironmentWrapper::unwrapmatlong(int id, const ScilabLongStackAllocator & allocator) const
{
    pyUnwrapMat<long long>(id, allocator);
}

void ScilabPythonEnvironmentWrapper::unwrapulong(int id, const ScilabULongStackAllocator & allocator) const
{
    pyUnwrapSingle<unsigned long long>(id, allocator);
}

void ScilabPythonEnvironmentWrapper::unwraprowulong(int id, const ScilabULongStackAllocator & allocator) const
{
    pyUnwrapRow<unsigned long long>(id, allocator);
}

void ScilabPythonEnvironmentWrapper::unwrapmatulong(int id, const ScilabULongStackAllocator & allocator) const
{
    pyUnwrapMat<unsigned long long>(id, allocator);
}

void ScilabPythonEnvironmentWrapper::unwrapboolean(int id, const ScilabBooleanStackAllocator & allocator) const
{
    pyUnwrapSingle<int>(id, allocator);
}

void ScilabPythonEnvironmentWrapper::unwraprowboolean(int id, const ScilabBooleanStackAllocator & allocator) const
{
    pyUnwrapRow<int>(id, allocator);
}

void ScilabPythonEnvironmentWrapper::unwrapmatboolean(int id, const ScilabBooleanStackAllocator & allocator) const
{
    pyUnwrapMat<int >(id, allocator);
}

void ScilabPythonEnvironmentWrapper::unwrapstring(int id, const ScilabStringStackAllocator & allocator) const
{
    PyObject * obj = scope.getObject(id);
    char * data = PyString_AsString(obj);
    allocator.allocate(1, 1, &data);
}

void ScilabPythonEnvironmentWrapper::unwraprowstring(int id, const ScilabStringStackAllocator & allocator) const
{
    PyObject * obj = scope.getObject(id);

    if (PyList_Check(obj))
    {
        // List case
        int col = PyList_Size(obj);
        char ** data = new char*[col];
        for (int i = 0; i < col; i++)
        {
            PyObject * item = PyList_GET_ITEM(obj, i);
            data[i] = PyString_AsString(item);
        }

        allocator.allocate(1, col, data);
        delete[] data;
    }
    else
    {
        // Numpy array case
        PyArrayObject * arr = reinterpret_cast<PyArrayObject *>(obj);
        npy_intp * dims = PyArray_DIMS(arr);
        npy_intp * strides = PyArray_STRIDES(arr);
        char * pyData = reinterpret_cast<char *>(PyArray_DATA(arr));
        char ** data = new char*[dims[0]];

        for (int i = 0; i < dims[0]; i++)
        {
            data[i] = pyData + i * strides[0];
        }

        allocator.allocate(1, dims[0], data);
        delete[] data;
    }
}

void ScilabPythonEnvironmentWrapper::unwrapmatstring(int id, const ScilabStringStackAllocator & allocator) const
{
    PyObject * obj = scope.getObject(id);

    if (PyList_Check(obj))
    {
        // List case
        int row = PyList_Size(obj);
        if (row > 0)
        {
            PyObject * f = PyList_GET_ITEM(obj, 0);
            int col = PyList_Size(f);
            char ** data = new char*[row * col];
            for (int i = 0; i < row; i++)
            {
                PyObject * sublist = PyList_GetItem(obj, i);
                for (int j = 0; j < col; j++)
                {
                    PyObject * item = PyList_GET_ITEM(sublist, j);
                    data[i + row * j] = PyString_AsString(item);
                }
            }

            allocator.allocate(row, col, data);
            delete[] data;
        }
    }
    else
    {
        // Numpy array case
        PyArrayObject * arr = reinterpret_cast<PyArrayObject *>(obj);
        npy_intp * dims = PyArray_DIMS(arr);
        npy_intp * strides = PyArray_STRIDES(arr);
        char * pyData = reinterpret_cast<char *>(PyArray_DATA(arr));
        char ** data = new char*[dims[0] * dims[1]];

        for (int i = 0; i < dims[0]; i++)
        {
            for (int j = 0; j < dims[1]; j++)
            {
                data[i + dims[0] * j] = pyData + i * strides[0] + j * strides[1];
            }
        }

        allocator.allocate(dims[0], dims[1], data);
        delete[] data;
    }
}

void ScilabPythonEnvironmentWrapper::unwrapcomplex(int id, const ScilabComplexStackAllocator & allocator) const
{
    PyObject * obj = scope.getObject(id);
    ComplexDataPointers cdp = allocator.allocate(1, 1, static_cast<double *>(0), static_cast<double *>(0));
    pyUnwrap(*cdp.realPtr, *cdp.imagPtr, obj);
}

void ScilabPythonEnvironmentWrapper::unwraprowcomplex(int id, const ScilabComplexStackAllocator & allocator) const
{
    PyObject * obj = scope.getObject(id);

    if (PyList_Check(obj))
    {
        // List case
        int col = PyList_Size(obj);
        ComplexDataPointers cdp = allocator.allocate(1, col, static_cast<double *>(0), static_cast<double *>(0));
        for (int i = 0; i < col; i++)
        {
            PyObject * item = PyList_GET_ITEM(obj, i);
            pyUnwrap(cdp.realPtr[i], cdp.imagPtr[i], obj);
        }
    }
    else
    {
        // Numpy array case
        PyArrayObject * arr = reinterpret_cast<PyArrayObject *>(obj);
        npy_intp * dims = PyArray_DIMS(arr);
        npy_intp * strides = PyArray_STRIDES(arr);

        ComplexDataPointers cdp = allocator.allocate(1, static_cast<int>(dims[0]), static_cast<double *>(0), static_cast<double *>(0));
        char * pyData = reinterpret_cast<char *>(PyArray_DATA(arr));
        for (int i = 0; i < dims[0]; i++)
        {
            complex c = *reinterpret_cast<complex *>(pyData + i * strides[0]);
            cdp.realPtr[i] = c.re;
            cdp.imagPtr[i] = c.im;
        }
    }
}

void ScilabPythonEnvironmentWrapper::unwrapmatcomplex(int id, const ScilabComplexStackAllocator & allocator) const
{
    PyObject * obj = scope.getObject(id);
    if (PyList_Check(obj))
    {
        // List case
        int row = PyList_Size(obj);
        if (row > 0)
        {
            PyObject * f = PyList_GET_ITEM(obj, 0);
            int col = PyList_Size(f);
            ComplexDataPointers cdp = allocator.allocate(row, col, static_cast<double *>(0), static_cast<double *>(0));
            for (int i = 0; i < row; i++)
            {
                PyObject * sublist = PyList_GET_ITEM(obj, i);
                for (int j = 0; j < col; j++)
                {
                    PyObject * item = PyList_GET_ITEM(sublist, j);
                    pyUnwrap(cdp.realPtr[i + row * j], cdp.imagPtr[i + row * j], item);
                }
            }
        }
    }
    else
    {
        // Numpy array case
        PyArrayObject * arr = reinterpret_cast<PyArrayObject *>(obj);
        npy_intp * dims = PyArray_DIMS(arr);
        npy_intp * strides = PyArray_STRIDES(arr);

        ComplexDataPointers cdp = allocator.allocate(static_cast<int>(dims[0]), static_cast<int>(dims[1]), static_cast<double *>(0), static_cast<double *>(0));
        char * pyData = reinterpret_cast<char*>(PyArray_DATA(arr));

        for (int i = 0; i < dims[0]; i++)
        {
            for (int j = 0; j < dims[1]; j++)
            {
                complex c = *reinterpret_cast<complex *>(pyData + i * strides[0] + j * strides[1]);
                cdp.realPtr[i + dims[0] * j] = c.re;
                cdp.imagPtr[i + dims[0] * j] = c.im;
            }
        }
    }
}

void ScilabPythonEnvironmentWrapper::unwrapfloat(int id, const ScilabFloatStackAllocator & allocator) const { }

void ScilabPythonEnvironmentWrapper::unwraprowfloat(int id, const ScilabFloatStackAllocator & allocator) const { }

void ScilabPythonEnvironmentWrapper::unwrapmatfloat(int id, const ScilabFloatStackAllocator & allocator) const { }
}
