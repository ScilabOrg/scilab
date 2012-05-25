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

#ifndef __SCILABPYTHONENVIRNOMENT_HXX__
#define __SCILABPYTHONENVIRNOMENT_HXX__

#ifdef _MSC_VER
#define PATHSEPARATOR ";"
#else
#define PATHSEPARATOR ":"
#endif

#include <Python.h>

#include "PythonVariablesScope.hxx"
#include "ScilabAbstractEnvironment.hxx"
#include "ScilabEnvironments.hxx"
#include "ScilabPythonException.hxx"
#include "ScilabPythonEnvironmentWrapper.hxx"
#include "ScilabPythonInvokers.hxx"
#include "PythonOptionsHelper.hxx"

#include <sstream>
#include <iostream>
#include <fstream>
#include <map>
#include <string>

using namespace org_modules_external_objects;

namespace org_modules_external_objects_python
{

class ScilabPythonEnvironment : public ScilabAbstractEnvironment
{
    static const std::string environmentName;
    static int envId;
    static ScilabPythonEnvironment * instance;

    PythonVariablesScope & scope;
    bool traceEnabled;
    bool isInit;
    std::ofstream * file;
    PythonOptionsHelper & helper;
    ScilabGatewayOptions & gwOptions;
    ScilabPythonEnvironmentWrapper & wrapper;

public :

    ScilabPythonEnvironment();

    ~ScilabPythonEnvironment();

    ScilabPythonTupleInvoker getTupleInvoker()
    {
        return ScilabPythonTupleInvoker(scope);
    }

    ScilabPythonDictionaryInvoker getDictionaryInvoker()
    {
        return ScilabPythonDictionaryInvoker(scope);
    }

    ScilabPythonListInvoker getListInvoker()
    {
        return ScilabPythonListInvoker(scope);
    }

    ScilabPythonSetInvoker getSetInvoker()
    {
        return ScilabPythonSetInvoker(scope);
    }

    ScilabPythonXRangeInvoker getXRangeInvoker()
    {
        return ScilabPythonXRangeInvoker(scope);
    }

    ScilabPythonSliceInvoker getSliceInvoker()
    {
        return ScilabPythonSliceInvoker(scope);
    }

    void Initialize();

    void Finalize();

    static int start();

    static void finish();

    static ScilabPythonEnvironment & getInstance()
    {
        return *instance;
    }

    OptionsHelper & getOptionsHelper();

    ScilabGatewayOptions & getGatewayOptions();

    ScilabAbstractEnvironmentWrapper & getWrapper();

    const std::string & getEnvironmentName();

    int extract(int id, int * args, int argsSize);

    void insert(int id, int * args, int argsSize);

    void evalString(const char ** code, int nbLines);

    void getEnvironmentInfos(const ScilabStringStackAllocator & allocator);

    void garbagecollect();

    void addtoclasspath(const char * path);

    void getclasspath(const ScilabStringStackAllocator & allocator);

    int createarray(char * className, int * dims, int len);

    int loadclass(char * className, bool allowReload);

    void getrepresentation(int id, const ScilabStringStackAllocator & allocator);

    std::string getrepresentation(int id);

    bool isvalidobject(int id);

    int newinstance(int id, int * args, int argsSize);

    int invoke(int id, const char * methodName, int * args, int argsSize);

    void setfield(int id, const char * fieldName, int idarg);

    int getfield(int id, const char * fieldName);

    int getfieldtype(int id, const char * fieldName);

    int getarrayelement(int id, int * index, int length);

    void setarrayelement(int id, int * index, int length, int idArg);

    int cast(int id, char * className);

    int castwithid(int id, int classId);

    void removeobject(int id);

    void getaccessiblemethods(int id, const ScilabStringStackAllocator & allocator);

    void getaccessiblefields(int id, const ScilabStringStackAllocator & allocator);

    std::string getclassname(int id);

    VariableType isunwrappable(int id);

    int compilecode(char * className, char ** code, int size);

    void enabletrace(const char * filename);

    void disabletrace();


private:

    static void initNumpy();

    inline void getAccessibleFields(int id, const ScilabStringStackAllocator & allocator, const bool isField)
    {
        PyObject * obj = scope.getObject(id);
        if (!obj)
        {
            throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), id);
        }

        PyObject * dir = PyObject_Dir(obj);
        if (!dir || PyList_Size(dir) == 0)
        {
            allocator.allocate(0, 0, static_cast<char **>(0));
            return;
        }

        int size = PyList_Size(dir);
        int j = 0;
        char ** arr = new char*[size];

        for (int i = 0; i < size; i++)
        {
            PyObject * fieldName = PyList_GetItem(dir, i);
            char * _field = PyString_AsString(fieldName);
            if (_field[0] != '_')
            {
                PyObject * field = PyObject_GetAttr(obj, fieldName);
                if (isField && !PyCallable_Check(field))
                {
                    arr[j++] = _field;
                }
                else if (!isField && PyCallable_Check(field))
                {
                    arr[j++] = _field;
                }
            }
        }

        allocator.allocate(j, 1, arr);
        delete[] arr;
    }

    inline void writeLog(const std::string fun, const std::string str)
    {
        if (traceEnabled)
        {
            *file << fun << ": " << str << std::endl;
        }
    }

    static inline PyObject * createMultiList(const int * dims, const int len)
    {
        if (len == 0)
        {
            return PyList_New(0);
        }

        if (len == 1)
        {
            return PyList_New(dims[0]);
        }

        PyObject * list = PyList_New(dims[0]);
        for (int i = 0; i < dims[0]; i++)
        {
            PyList_SetItem(list, i, createMultiList(dims + 1, len - 1));
        }

        return list;
    }
};

}

#endif // __SCILABPYTHONENVIRONMENT_HXX__
