/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SCILABPYTHONINVOKERS_HXX__
#define __SCILABPYTHONINVOKERS_HXX__

#include "ScilabAbstractInvoker.hxx"
#include "PythonVariablesScope.hxx"

#include <Python.h>

namespace org_modules_external_objects_python
{

class ScilabPythonGetAttrInvoker : public ScilabAbstractInvoker
{
    PythonVariablesScope & scope;

public:

    ScilabPythonGetAttrInvoker(PythonVariablesScope & _scope) : scope(_scope) { }

    ~ScilabPythonGetAttrInvoker() { }

    int invoke(int * args, int argsSize)
    {
        if (argsSize != 2)
        {
            throw ScilabPythonException(__LINE__, __FILE__, gettext("Wrong number of arguments: 2 expected."));
        }

        PyObject * obj = scope.getObject(args[0]);
        if (!obj)
        {
            throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), args[0]);
        }

        PyObject * attr = scope.getObject(args[1]);
        if (!attr)
        {
            throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), args[1]);
        }

        PyObject * value = PyObject_GetAttr(obj, attr);
        if (!value || PyErr_Occurred())
        {
            PyObject * type, * value, * traceback;
            PyErr_Fetch(&type, &value, &traceback);
            PyErr_NormalizeException(&type, &value, &traceback);
            PyErr_Clear();

            throw ScilabPythonException(__LINE__, __FILE__, type, value, traceback, gettext("Unable to get attribute."));
        }

        return scope.addObject(value);
    }
};

class ScilabPythonDirInvoker : public ScilabAbstractInvoker
{
    PythonVariablesScope & scope;

public:

    ScilabPythonDirInvoker(PythonVariablesScope & _scope) : scope(_scope) { }

    ~ScilabPythonDirInvoker() { }

    int invoke(int * args, int argsSize)
    {
        if (argsSize > 1)
        {
            throw ScilabPythonException(__LINE__, __FILE__, gettext("Wrong number of arguments: 1 expected."));
        }

        PyObject * obj = scope.getObject(*args);
        if (!obj)
        {
            throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), *args);
        }

        PyObject * dir = PyObject_Dir(obj);
        if (!dir)
        {
            if (PyErr_Occurred())
            {
                PyObject * type, * value, * traceback;
                PyErr_Fetch(&type, &value, &traceback);
                PyErr_NormalizeException(&type, &value, &traceback);
                PyErr_Clear();

                throw ScilabPythonException(__LINE__, __FILE__, type, value, traceback, gettext("Unable to get dir."));
            }

            return VOID_OBJECT;
        }

        return scope.addObject(dir);
    }
};

class ScilabPythonTupleInvoker : public ScilabAbstractInvoker
{
    PythonVariablesScope & scope;

public:

    ScilabPythonTupleInvoker(PythonVariablesScope & _scope) : scope(_scope) { }

    ~ScilabPythonTupleInvoker() { }

    int invoke(int * args, int argsSize)
    {
        PyObject * tuple = PyTuple_New(argsSize);
        for (int i = 0; i < argsSize; i++)
        {
            PyObject * obj = scope.getObject(args[i]);
            if (!obj)
            {
                Py_DECREF(tuple);
                throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), args[i]);
            }

            Py_INCREF(obj);
            PyTuple_SetItem(tuple, i, scope.getObject(args[i]));
        }

        return scope.addObject(tuple);
    }
};

class ScilabPythonDictionaryInvoker : public ScilabAbstractInvoker
{
    PythonVariablesScope & scope;

public:

    ScilabPythonDictionaryInvoker(PythonVariablesScope & _scope) : scope(_scope) { }

    ~ScilabPythonDictionaryInvoker() { }

    int invoke(int * args, int argsSize)
    {
        if (argsSize % 2 != 0)
        {
            throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid number of arguments: even number expected"));
        }

        PyObject * dict = PyDict_New();
        for (int i = 0; i < argsSize; i += 2)
        {
            PyObject * key = scope.getObject(args[i]);
            if (!key)
            {
                Py_DECREF(dict);
                throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), args[i]);
            }
            PyObject * value = scope.getObject(args[i + 1]);
            if (!value)
            {
                Py_DECREF(dict);
                throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), args[i + 1]);
            }

            PyDict_SetItem(dict, key, value);
        }

        return scope.addObject(dict);
    }
};

class ScilabPythonListInvoker : public ScilabAbstractInvoker
{
    PythonVariablesScope & scope;

public:

    ScilabPythonListInvoker(PythonVariablesScope & _scope) : scope(_scope) { }

    ~ScilabPythonListInvoker() { }

    int invoke(int * args, int argsSize)
    {
        PyObject * list = PyList_New(argsSize);
        for (int i = 0; i < argsSize; i++)
        {
            PyObject * obj = scope.getObject(args[i]);
            if (!obj)
            {
                Py_DECREF(list);
                throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), args[i]);
            }
            Py_INCREF(obj);
            PyList_SetItem(list, i, scope.getObject(args[i]));
        }

        return scope.addObject(list);
    }
};

class ScilabPythonSetInvoker : public ScilabAbstractInvoker
{
    PythonVariablesScope & scope;

public:

    ScilabPythonSetInvoker(PythonVariablesScope & _scope) : scope(_scope) { }

    ~ScilabPythonSetInvoker() { }

    int invoke(int * args, int argsSize)
    {
        PyObject * set = PySet_New(0);
        for (int i = 0; i < argsSize; i++)
        {
            PyObject * obj = scope.getObject(args[i]);
            if (!obj)
            {
                Py_DECREF(set);
                throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), args[i]);
            }
            PySet_Add(set, obj);
        }

        return scope.addObject(set);
    }
};

class ScilabPythonXRangeInvoker : public ScilabAbstractInvoker
{
    PythonVariablesScope & scope;

public:

    ScilabPythonXRangeInvoker(PythonVariablesScope & _scope) : scope(_scope) { }

    ~ScilabPythonXRangeInvoker() { }

    int invoke(int * args, int argsSize)
    {
        if (argsSize < 1 || argsSize > 3)
        {
            throw ScilabPythonException(__LINE__, __FILE__, gettext("Bad number of argument for function xrange: 1, 2 or 3 expected"));
        }

        PyObject * range = 0;
        PyObject * obj = scope.getObject(args[0]);
        PyObject * obj1 = 0;
        if (!obj)
        {
            throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), args[0]);
        }

        int start = PyInt_AsLong(obj);
        int end, step;
        char * s = const_cast<char *>("lll");

        switch (argsSize)
        {
            case 1:
                range = PyObject_CallFunction((PyObject *) &PyRange_Type, s, 0, start, 1);
                break;
            case 2:
                obj = scope.getObject(args[1]);
                if (!obj)
                {
                    throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), args[1]);
                }
                end = PyInt_AsLong(obj);
                range = PyObject_CallFunction((PyObject *) &PyRange_Type, s, start, end, 1);
                break;
            case 3:
                obj = scope.getObject(args[1]);
                if (!obj)
                {
                    throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), args[1]);
                }
                obj1 = scope.getObject(args[2]);
                if (!obj1)
                {
                    throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), args[2]);
                }
                end = PyInt_AsLong(obj);
                step = PyInt_AsLong(obj1);
                range = PyObject_CallFunction((PyObject *) &PyRange_Type, s, start, end, step);
                break;
        }

        if (!range || PyErr_Occurred())
        {
            PyObject * type, * value, * traceback;
            PyErr_Fetch(&type, &value, &traceback);
            PyErr_NormalizeException(&type, &value, &traceback);
            PyErr_Clear();

            throw ScilabPythonException(__LINE__, __FILE__, type, value, traceback, gettext("Unable to make an xrange"));
        }

        return scope.addObject(range);
    }
};

class ScilabPythonSliceInvoker : public ScilabAbstractInvoker
{
    PythonVariablesScope & scope;

public:

    ScilabPythonSliceInvoker(PythonVariablesScope & _scope) : scope(_scope) { }

    ~ScilabPythonSliceInvoker() { }

    int invoke(int * args, int argsSize)
    {
        if (argsSize < 1 || argsSize > 3)
        {
            throw ScilabPythonException(__LINE__, __FILE__, gettext("Bad number of argument for function slice: 1, 2 or 3 expected"));
        }

        PyObject * start, * stop, * step, * slice;

        switch (argsSize)
        {
            case 1:
                stop = scope.getObject(args[0]);
                if (!stop)
                {
                    throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), args[0]);
                }
                slice = PySlice_New(Py_None, stop, Py_None);
                break;
            case 2:
                start = scope.getObject(args[0]);
                if (!start)
                {
                    throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), args[0]);
                }
                stop = scope.getObject(args[1]);
                if (!stop)
                {
                    throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), args[1]);
                }
                slice = PySlice_New(start, stop, Py_None);
                break;
            case 3:
                start = scope.getObject(args[0]);
                if (!start)
                {
                    throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), args[0]);
                }
                stop = scope.getObject(args[1]);
                if (!stop)
                {
                    throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), args[1]);
                }
                step = scope.getObject(args[2]);
                if (!step)
                {
                    throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), args[2]);
                }
                slice = PySlice_New(start, stop, step);
                break;
        }

        if (!slice || PyErr_Occurred())
        {
            PyObject * type, * value, * traceback;
            PyErr_Fetch(&type, &value, &traceback);
            PyErr_NormalizeException(&type, &value, &traceback);
            PyErr_Clear();

            throw ScilabPythonException(__LINE__, __FILE__, type, value, traceback, gettext("Unable to make a slice"));
        }

        return scope.addObject(slice);
    }
};
}

#endif // __SCILABPYTHONINVOKERS_HXX__
