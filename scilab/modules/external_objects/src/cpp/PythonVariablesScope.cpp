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

#include "PythonVariablesScope.hxx"

namespace org_modules_external_objects_python
{

PythonVariablesScope::PythonVariablesScope()
{
    objects = new std::vector<PyObject *>(SCOPE_INITIAL_SIZE, static_cast<PyObject *>(0));
    freePlaces = new std::stack<int>();
}

PythonVariablesScope::~PythonVariablesScope()
{
    for (std::vector<PyObject *>::iterator i = objects->begin(); i != objects->end(); i++)
    {
        if (*i)
        {
            Py_DECREF(*i);
        }
    }
    delete objects;
    delete freePlaces;
}

int PythonVariablesScope::addObject(PyObject * obj) const
{
    //printf("add obj=%p refcount=%d\n", obj, obj->ob_refcnt);

    if (freePlaces->empty())
    {
        objects->push_back(obj);

        return objects->size();
    }
    else
    {
        int pos = freePlaces->top();
        (*objects)[pos] = obj;
        freePlaces->pop();

        return pos + 1;
    }
}

PyObject* PythonVariablesScope::removeObject(int pos) const
{
    pos--;
    if ((pos >= 0 || pos < objects->size()) && (*objects)[pos])
    {
        PyObject * obj = (*objects)[pos];
        //printf("remove obj=%p refcount=%d\n", obj, obj->ob_refcnt);
        Py_DECREF(obj);
        (*objects)[pos] = 0;
        freePlaces->push(pos);

        return obj;
    }

    return 0;
}

PyObject * PythonVariablesScope::getObject(int pos) const
{
    pos--;
    if (pos < 0 || pos >= objects->size())
    {
        return 0;
    }

    return (*objects)[pos];
}

void PythonVariablesScope::replaceObject(int pos, PyObject * obj) const
{
    pos--;
    if (pos >= 0 || pos < objects->size() && obj != (*objects)[pos])
    {
        //printf("remove obj=%p (*objects)[pos]=%p refcount=%d %d\n", obj, obj->ob_refcnt, (*objects)[pos]->ob_refcnt);
        Py_DECREF((*objects)[pos]);
        (*objects)[pos] = obj;
    }
}
}
