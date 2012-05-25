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

#include "ScilabPythonEnvironment.hxx"
#include "PythonVariablesScope.hxx"

#define MAX_STR_LENGTH 128
#define MAX_STR_LENGTH_STR "128"

namespace org_modules_external_objects_python
{

PythonVariablesScope::PythonVariablesScope(const ScilabPythonEnvironment & _env) : env(_env),
    objects(*new std::vector<PyObject *>()),
    freePlaces(*new std::stack<int>())
{
    objects.reserve(SCOPE_INITIAL_SIZE);
}

PythonVariablesScope::~PythonVariablesScope()
{
    for (std::vector<PyObject *>::iterator i = objects.begin(); i != objects.end(); i++)
    {
        if (*i)
        {
            Py_DECREF(*i);
        }
    }
    delete &objects;
    delete &freePlaces;
}

int PythonVariablesScope::addObject(PyObject * obj) const
{
    int ret;
    if (freePlaces.empty())
    {
        objects.push_back(obj);
        ret = objects.size();
    }
    else
    {
        int pos = freePlaces.top();
        objects[pos] = obj;
        freePlaces.pop();
        ret = pos + 1;
    }

    if (env.isTraceEnabled())
    {
        PyObject * repr = PyObject_Repr(obj);
        const char * _repr = PyString_AsString(repr);
        if (strlen(_repr) > MAX_STR_LENGTH)
        {
            env.writeLog("addObject", "Add object with id %d, repr is %." MAX_STR_LENGTH_STR "s...(truncated) and refcnt=%d.", ret, _repr, obj->ob_refcnt);
        }
        else
        {
            env.writeLog("addObject", "Add object with id %d, repr is %s and refcnt=%d.", ret, _repr, obj->ob_refcnt);
        }

        Py_DECREF(repr);
    }

    return ret;
}

PyObject * PythonVariablesScope::removeObject(int pos) const
{
    PyObject * obj = removeObjectWithoutDecref(pos);
    if (obj)
    {
        Py_DECREF(obj);
    }

    return obj;
}

PyObject * PythonVariablesScope::removeObjectWithoutDecref(int pos) const
{
    pos--;
    if (pos >= 0 && pos < objects.size() && objects[pos])
    {
        PyObject * obj = objects[pos];
        objects[pos] = 0;
        freePlaces.push(pos);

        env.writeLog("removeObject", "Remove object with id %d and refcnt=%d (before DECREF).", pos + 1, obj->ob_refcnt);

        return obj;
    }

    return 0;
}

PyObject * PythonVariablesScope::getObject(int pos) const
{
    pos--;
    if (pos < 0 || pos >= objects.size())
    {
        return 0;
    }

    PyObject * obj = objects[pos];

    if (obj)
    {
        env.writeLog("getObject", "Get object with id %d and refcnt=%d.", pos + 1, obj->ob_refcnt);
    }
    else
    {
        env.writeLog("getObject", "Get object (null pointer) with id %d.", pos + 1);
    }

    return obj;
}

void PythonVariablesScope::replaceObject(int pos, PyObject * obj) const
{
    pos--;
    if (pos >= 0 || pos < objects.size() && obj != objects[pos])
    {
        if (env.isTraceEnabled())
        {
            PyObject * repr = PyObject_Repr(obj);
            const char * _repr = PyString_AsString(repr);
            if (strlen(_repr) > MAX_STR_LENGTH)
            {
                env.writeLog("replaceObject", "Replace object with id %d by object where repr is %." MAX_STR_LENGTH_STR "s...(truncated) and refcnt=%d.", pos + 1, _repr, obj->ob_refcnt);
            }
            else
            {
                env.writeLog("replaceObject", "Replace object with id %d by object where repr is %s and refcnt=%d.", pos + 1, _repr, obj->ob_refcnt);
            }
            Py_DECREF(repr);
        }
        Py_DECREF(objects[pos]);
        objects[pos] = obj;
    }
}

bool PythonVariablesScope::isValid(int pos) const
{
    return pos == 0 || (pos > 0 && pos - 1 < objects.size() && objects[pos - 1] != 0);
}
}
