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
#include "ScilabStream.hxx"
#include "ScilabPythonOStream.hxx"

#include <sstream>
#include <iostream>
#include <fstream>
#include <map>
#include <string>

#define LOG_BUFFER_SIZE 4096

using namespace org_modules_external_objects;

namespace org_modules_external_objects_python
{

class ScilabPythonEnvironment : public ScilabAbstractEnvironment
{
    static const std::string environmentName;
    static int envId;
    static ScilabPythonEnvironment * instance;
    static bool usable;

    PythonVariablesScope & scope;
    bool traceEnabled;
    bool isInit;
    std::ofstream * file;
    ScilabStream & scilabStream;
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

    ScilabPythonGetAttrInvoker getGetAttrInvoker()
    {
        return ScilabPythonGetAttrInvoker(scope);
    }

    ScilabPythonInvokeInvoker getInvokeInvoker()
    {
        return ScilabPythonInvokeInvoker(scope);
    }

    ScilabPythonBuiltinInvoker getBuiltinInvoker(const std::string & name)
    {
        return ScilabPythonBuiltinInvoker(scope, name);
    }

    ScilabPythonModuleInvoker getModuleInvoker(const std::string & name)
    {
        return ScilabPythonModuleInvoker(scope, name);
    }

    void Initialize();

    void Finalize();

    static int start();

    static void finish();

    static ScilabPythonEnvironment & getInstance()
    {
        return *instance;
    }

    PythonOptionsHelper & getOptionsHelper();

    ScilabGatewayOptions & getGatewayOptions();

    ScilabAbstractEnvironmentWrapper & getWrapper();

    const std::string & getEnvironmentName();

    int extract(int id, int * args, int argsSize);

    void insert(int id, int * args, int argsSize);

    void addNamedVariable(int id, const char * varName);

    int getNamedVariable(const char * varName);

    void evalString(const char ** code, int nbLines, ScilabStringStackAllocator * allocator);

    void getEnvironmentInfos(const ScilabStringStackAllocator & allocator);

    void garbagecollect();

    void addtoclasspath(const char * path);

    void getclasspath(const ScilabStringStackAllocator & allocator);

    int createarray(char * className, int * dims, int len);

    int loadclass(char * className, char * currentSciPath, bool isNamedVarCreated, bool allowReload);

    void getrepresentation(int id, const ScilabStringStackAllocator & allocator);

    std::string getrepresentation(int id);

    bool isvalidobject(int id);

    int newinstance(int id, int * args, int argsSize);

    int operation(int idA, int idB, const OperatorsType type);

    int * invoke(int id, const char * methodName, int * args, int argsSize);

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

    inline bool isTraceEnabled() const
    {
        return traceEnabled;
    }

    inline void writeLog(const std::string & fun, const std::string & str, ...) const
    {
        if (traceEnabled)
        {
            char _str[LOG_BUFFER_SIZE];
            va_list args;

            va_start(args, str);
            vsnprintf(_str, LOG_BUFFER_SIZE, str.c_str(), args);
            va_end(args);

            *file << fun << ": " << _str << std::endl;
        }
    }

    static inline std::wstring getWString(PyObject * obj)
    {
        if (PyString_Check(obj))
        {
            int len = PyString_GET_SIZE(obj);
            char * str = PyString_AsString(obj);

            return std::wstring(str, str + len);
        }
        else if (PyUnicode_Check(obj))
        {
            return std::wstring(reinterpret_cast<const wchar_t *>(PyUnicode_AS_DATA(obj)));
        }

        throw ScilabPythonException(__LINE__, __FILE__, gettext("Cannot convert in a wide string"));
    }

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
            if (helper.getShowPrivate() || _field[0] != '_')
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

    static inline std::vector<char *> breakInLines(const std::string & str)
    {
        std::vector<char *> buf;
        std::size_t prev(0);
        std::size_t pos = str.find_first_of("\n");
        while (pos != std::string::npos)
        {
            buf.push_back(strdup(str.substr(prev, pos - prev).c_str()));
            prev = pos + 1;
            pos = str.find_first_of("\n", prev);
        }
        buf.push_back(strdup(str.substr(prev).c_str()));

        std::vector<char *>::iterator last = buf.end();
        for (std::vector<char *>::iterator i = buf.end() - 1; i >= buf.begin(); i--)
        {
            if (strlen(*i) == 0)
            {
                last = i;
                free(*i);
            }
            else
            {
                break;
            }
        }
        buf.erase(last, buf.end());

        return buf;
    }

    static inline const char * getOpNameFromType(const OperatorsType type)
    {
        switch (type)
        {
            case Transp :
                throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid operator: \'"));
            case Add :
                return "add";
            case Sub :
                return "sub";
            case Mul :
                return "mul";
            case Div :
                return "div";
            case Backslash :
                return "truediv";
            case Pow :
                return "pow";
            case DotMul :
                throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid operator: .*"));
            case DotDiv :
                throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid operator: ./"));
            case DotBackslash :
                throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid operator: .\\"));
            case DotMulDot :
                throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid operator: .*."));
            case DotDivDot :
                throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid operator: ./."));
            case DotBackslashDot :
                throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid operator: .\\."));
            case Eq :
                return "eq";
            case Neq :
                return "ne";
            case Or :
                return "or";
            case And :
                return "and";
            case DotPow :
                throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid operator: .^"));
            case Not :
                return "not_";
            case DotTransp :
                throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid operator: .\'"));
            case Lt :
                return "lt";
            case Gt :
                return "gt";
            case Leq :
                return "le";
            case Geq :
                return "ge";
        }
    }
};

}

#endif // __SCILABPYTHONENVIRONMENT_HXX__
