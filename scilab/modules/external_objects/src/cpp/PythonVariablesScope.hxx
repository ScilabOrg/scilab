/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Baozeng Ding
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __PYTHONVARIABLESSCOPE_HXX__
#define __PYTHONVARIABLESSCOPE_HXX__

#include <stack>
#include <vector>
#include <Python.h>

#define SCOPE_INITIAL_SIZE 1024

namespace org_modules_external_objects_python
{

class ScilabPythonEnvironment;

class PythonVariablesScope
{
    std::vector<PyObject *> & objects;
    std::stack<int> & freePlaces;
    const ScilabPythonEnvironment & env;

public :

    PythonVariablesScope(const ScilabPythonEnvironment & _env);

    ~PythonVariablesScope();

    int addObject(PyObject * obj) const;

    PyObject * removeObject(int pos) const;

    PyObject * removeObjectWithoutDecref(int pos) const;

    PyObject * getObject(int pos) const;

    void replaceObject(int pos, PyObject * obj) const;

    bool isValid(int pos) const;
};

}

#endif // __PYTHONVARIABLESSCOPE_HXX_
