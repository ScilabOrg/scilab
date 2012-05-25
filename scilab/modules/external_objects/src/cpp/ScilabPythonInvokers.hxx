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

class ScilabPythonBuiltinInvoker : public ScilabAbstractInvoker
{
    PythonVariablesScope & scope;
    std::string name;

public:

    ScilabPythonBuiltinInvoker(PythonVariablesScope & _scope, const std::string & _name) : scope(_scope), name(_name) { }

    ~ScilabPythonBuiltinInvoker() { }

    int invoke(int * args, int argsSize)
    {
        PyObject * _builtin_ = PyImport_AddModule("__builtin__");
        if (!_builtin_)
        {
            throw ScilabPythonException(__LINE__, __FILE__, gettext("Cannot evaluate the function since __builtin__ is not accessible."));
        }

        PyObject * _fun_ = PyObject_GetAttrString(_builtin_, name.c_str());
        if (!_fun_)
        {
            if (PyErr_Occurred())
            {
                PyObject * type, * value, * traceback;
                PyErr_Fetch(&type, &value, &traceback);
                PyErr_NormalizeException(&type, &value, &traceback);
                PyErr_Clear();

                throw ScilabPythonException(__LINE__, __FILE__, type, value, traceback, gettext("Unable to get built-in."));
            }
            throw ScilabPythonException(__LINE__, __FILE__, gettext("Unable to get built-in."));
        }

        PyObject * pArgs = PyTuple_New(argsSize);
        for (int i = 0; i < argsSize; i++)
        {
            PyObject * obj = scope.getObject(args[i]);
            if (!obj)
            {
                Py_DECREF(pArgs);
                Py_DECREF(_fun_);
                throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), args[i]);
            }
            Py_INCREF(obj);
            PyTuple_SetItem(pArgs, i, obj);
        }


        PyObject * result = PyObject_Call(_fun_, pArgs, 0);
        Py_DECREF(pArgs);
        Py_DECREF(_fun_);

        if (!result)
        {
            if (PyErr_Occurred())
            {
                PyObject * type, * value, * traceback;
                PyErr_Fetch(&type, &value, &traceback);
                PyErr_NormalizeException(&type, &value, &traceback);
                PyErr_Clear();

                throw ScilabPythonException(__LINE__, __FILE__, type, value, traceback, gettext("Unable to invoke the function: %s"), name.c_str());
            }
            throw ScilabPythonException(__LINE__, __FILE__, gettext("Unable to invoke the function: %s"), name.c_str());
        }

        return scope.addObject(result);
    }
};

class ScilabPythonInvokeInvoker : public ScilabAbstractInvoker
{
    PythonVariablesScope & scope;

public:

    ScilabPythonInvokeInvoker(PythonVariablesScope & _scope) : scope(_scope) { }

    ~ScilabPythonInvokeInvoker() { }

    int invoke(int * args, int argsSize)
    {
        if (argsSize == 0)
        {
            throw ScilabPythonException(__LINE__, __FILE__, gettext("Wrong number of arguments: at least %d expected."), 1);
        }

        PyObject * fun = scope.getObject(args[0]);
        if (!fun)
        {
            throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), args[0]);
        }

        if (!PyCallable_Check(fun))
        {
            throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid callable object at position %d."), 1);
        }

        PyObject * pArgs = PyTuple_New(argsSize - 1);
        for (int i = 1; i < argsSize; i++)
        {
            PyObject * obj = scope.getObject(args[i]);
            if (!obj)
            {
                Py_DECREF(pArgs);
                throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), args[i]);
            }
            Py_INCREF(obj);
            PyTuple_SetItem(pArgs, i - 1, obj);
        }

        PyObject * result = PyObject_Call(fun, pArgs, 0);
        Py_DECREF(pArgs);

        if (!result)
        {
            if (PyErr_Occurred())
            {
                PyObject * type, * value, * traceback;
                PyErr_Fetch(&type, &value, &traceback);
                PyErr_NormalizeException(&type, &value, &traceback);
                PyErr_Clear();

                throw ScilabPythonException(__LINE__, __FILE__, type, value, traceback, gettext("Unable to invoke the callable"));
            }
            throw ScilabPythonException(__LINE__, __FILE__, gettext("Unable to invoke the callable"));
        }

        return scope.addObject(result);
    }
};

class ScilabPythonModuleInvoker : public ScilabAbstractInvoker
{
    PythonVariablesScope & scope;
    std::string name;

public:

    ScilabPythonModuleInvoker(PythonVariablesScope & _scope, const std::string & _name) : scope(_scope), name(_name) { }

    ~ScilabPythonModuleInvoker() { }

    int invoke(int * args, int argsSize)
    {
        if (argsSize != 0)
        {
            throw ScilabPythonException(__LINE__, __FILE__, gettext("Wrong number of arguments: %d expected."), 0);
        }

        PyObject * _module_ = PyImport_AddModule(name.c_str());
        if (!_module_)
        {
            throw ScilabPythonException(__LINE__, __FILE__, gettext("Cannot evaluate the function since __builtin__ is not accessible."));
        }

        Py_INCREF(_module_);
        return scope.addObject(_module_);
    }
};

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
            throw ScilabPythonException(__LINE__, __FILE__, gettext("Wrong number of arguments: %d expected."), 2);
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
        if (!value)
        {
            if (PyErr_Occurred())
            {
                PyObject * type, * value, * traceback;
                PyErr_Fetch(&type, &value, &traceback);
                PyErr_NormalizeException(&type, &value, &traceback);
                PyErr_Clear();

                throw ScilabPythonException(__LINE__, __FILE__, type, value, traceback, gettext("Unable to get attribute."));
            }
            throw ScilabPythonException(__LINE__, __FILE__, gettext("Unable to get attribute."));
        }

        return scope.addObject(value);
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

            if (PyDict_SetItem(dict, key, value) == -1)
            {
                Py_DECREF(dict);
                if (PyErr_Occurred())
                {
                    PyObject * type, * value, * traceback;
                    PyErr_Fetch(&type, &value, &traceback);
                    PyErr_NormalizeException(&type, &value, &traceback);
                    PyErr_Clear();

                    throw ScilabPythonException(__LINE__, __FILE__, type, value, traceback, gettext("Unable to create a dictionary."));
                }
                throw ScilabPythonException(__LINE__, __FILE__, gettext("Unable to create a dictionary."));
            }
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
            PyList_SET_ITEM(list, i, obj);
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
                throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), args[i]);
            }
            Py_INCREF(obj);
            if (PySet_Add(set, obj) == -1)
            {
                Py_DECREF(set);
                if (PyErr_Occurred())
                {
                    PyObject * type, * value, * traceback;
                    PyErr_Fetch(&type, &value, &traceback);
                    PyErr_NormalizeException(&type, &value, &traceback);
                    PyErr_Clear();

                    throw ScilabPythonException(__LINE__, __FILE__, type, value, traceback, gettext("Unable to create a set."));
                }
                throw ScilabPythonException(__LINE__, __FILE__, gettext("Unable to create a set."));
            }
        }

        return scope.addObject(set);
    }
};
}

#endif // __SCILABPYTHONINVOKERS_HXX__
