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

#include <Python.h>
#define PY_ARRAY_UNIQUE_SYMBOL SciPyEnv_API
#include <numpy/arrayobject.h>

#include "ScilabPythonEnvironment.hxx"

namespace org_modules_external_objects_python
{

const std::string ScilabPythonEnvironment::environmentName = "Python Environment";
int ScilabPythonEnvironment::envId = -1;
ScilabPythonEnvironment * ScilabPythonEnvironment::instance = 0;

ScilabPythonEnvironment::ScilabPythonEnvironment() : scope(*new PythonVariablesScope()),
    helper(*new PythonOptionsHelper()),
    gwOptions(*new ScilabGatewayOptions()),
    wrapper(*new ScilabPythonEnvironmentWrapper(scope, helper)),
    traceEnabled(false),
    isInit(false),
    file(0) { }

ScilabPythonEnvironment::~ScilabPythonEnvironment()
{
    delete &scope;
    delete &helper;
    delete &gwOptions;
    delete &wrapper;

    file->close();
    delete file;
}

int ScilabPythonEnvironment::start()
{
    if (envId == -1)
    {
        instance = new ScilabPythonEnvironment();
        envId = ScilabEnvironments::registerScilabEnvironment(instance);
    }

    return envId;
}

void ScilabPythonEnvironment::finish()
{
    if (envId != -1)
    {
        instance->Finalize();
        ScilabEnvironments::unregisterScilabEnvironment(envId);
        envId = -1;
        delete instance;
        instance = 0;
    }
}

void ScilabPythonEnvironment::Initialize()
{
    if (!isInit)
    {
        isInit = true;
        // TODO: 1 or 0... 1 is to enable to register signal handlers
        Py_InitializeEx(0);
        initNumpy();
    }
}

void ScilabPythonEnvironment::Finalize()
{
    Py_Finalize();
}

void ScilabPythonEnvironment::initNumpy()
{
    import_array();
}

OptionsHelper & ScilabPythonEnvironment::getOptionsHelper()
{
    return helper;
}

ScilabGatewayOptions & ScilabPythonEnvironment::getGatewayOptions()
{
    return gwOptions;
}

ScilabAbstractEnvironmentWrapper & ScilabPythonEnvironment::getWrapper()
{
    return wrapper;
}

const std::string & ScilabPythonEnvironment::getEnvironmentName()
{
    return environmentName;
}

void ScilabPythonEnvironment::getEnvironmentInfos(const ScilabStringStackAllocator & allocator)
{
    const char * version = Py_GetVersion();
    const char * platform = Py_GetPlatform();
    const char * copyright = Py_GetCopyright();
    const char * compiler = Py_GetCompiler();
    const char * buildInfo = Py_GetBuildInfo();

    const char * infos[] = {"Version", "Platform", "Copyright", "Compiler", "Build info", version, platform, copyright, compiler, buildInfo};
    allocator.allocate(5, 2, const_cast<char**>(infos));
}

int ScilabPythonEnvironment::extract(int id, int * args, int argsSize)
{
    PyObject * obj = scope.getObject(id);
    if (!obj)
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), id);
    }

    if (PyDict_Check(obj))
    {
        if (argsSize != 1)
        {
            throw ScilabPythonException(__LINE__, __FILE__, gettext("Cannot extract more than one element from a dictionary"));
        }

        PyObject * key = scope.getObject(*args);
        if (!obj)
        {
            throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid key object"));
        }

        if (!PyDict_Contains(obj, key))
        {
            throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid key"));
        }

        PyObject * value = PyDict_GetItem(obj, key);

        return scope.addObject(value);
    }

    throw ScilabPythonException(__LINE__, __FILE__, gettext("Cannot extract from Python object"));
}

void ScilabPythonEnvironment::insert(int id, int * args, int argsSize)
{
    PyObject * obj = scope.getObject(id);
    if (!obj)
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), id);
    }
    if (PyDict_Check(obj))
    {
        if (argsSize != 2)
        {
            throw ScilabPythonException(__LINE__, __FILE__, gettext("Cannot insert more than one element in a dictionary"));
        }

        PyObject * key = scope.getObject(args[0]);
        if (!obj)
        {
            throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid key object"));
        }

        if (!PyDict_Contains(obj, key))
        {
            throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid key"));
        }

        PyObject * value = scope.getObject(args[1]);
        PyDict_SetItem(obj, key, value);

        return;
    }

    throw ScilabPythonException(__LINE__, __FILE__, gettext("Cannot insert in Python object"));
}

void ScilabPythonEnvironment::garbagecollect() { }

void ScilabPythonEnvironment::addtoclasspath(const char * path)
{
    PyObject * syspath = PySys_GetObject(const_cast<char *>("path"));
    PyObject * _path = PyString_FromString(path);
    PyList_Append(syspath, _path);
    Py_DECREF(_path);
}

void ScilabPythonEnvironment::getclasspath(const ScilabStringStackAllocator & allocator)
{
    PyObject * syspath = PySys_GetObject(const_cast<char *>("path"));
    int size = PyList_Size(syspath);
    char ** arr = new char*[size];

    for (int i = 0; i < size; i++)
    {
        PyObject * item = PyList_GetItem(syspath, i);
        arr[i] = strdup(PyString_AsString(item));
        Py_DECREF(item);
    }

    allocator.allocate(size, 1, arr);
    for (int i = 0; i < size; i++)
    {
        free(arr[i]);
    }
    delete arr;
}

void ScilabPythonEnvironment::evalString(const char ** code, int nbLines)
{
    std::stringstream ss;
    for (int i = 0; i < nbLines; i++)
    {
        ss << code[i] << std::endl;
    }

    if (PyRun_SimpleString(ss.str().c_str()) == -1)
    {
        if (PyErr_Occurred())
        {
            PyObject *type, *value, *traceback;
            PyErr_Fetch(&type, &value, &traceback);
            PyErr_NormalizeException(&type, &value, &traceback);
            PyErr_Clear();

            throw ScilabPythonException(__LINE__, __FILE__, type, value, traceback, gettext("Cannot evaluate the code"));
        }
        else
        {
            throw ScilabPythonException(__LINE__, __FILE__, gettext("Cannot evaluate the code"));
        }
    }
}

int ScilabPythonEnvironment::createarray(char * className, int * dims, int len)
{
    PyObject * obj = 0;
    if (len == 0)
    {
        obj = PyList_New(0);
        return scope.addObject(obj);
    }

    for (int i = 0; i < len; i++)
    {
        if (dims[i] < 0)
        {
            throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid dimension in list creation"));
        }
    }

    obj = createMultiList(dims, len);
    return scope.addObject(obj);
}

int ScilabPythonEnvironment::loadclass(char * className, bool allowReload)
{
    static std::map<std::string, int> map = std::map<std::string, int>();

    std::string _className = std::string(className);
    std::map<std::string, int>::const_iterator it = map.find(_className);
    if (it != map.end())
    {
        int id = it->second;
        PyObject * obj = scope.getObject(id);
        if (allowReload)
        {
            PyObject * pModule = PyImport_ReloadModule(obj);
            if (!pModule && PyErr_Occurred())
            {
                PyObject *type, *value, *traceback;
                PyErr_Fetch(&type, &value, &traceback);
                PyErr_NormalizeException(&type, &value, &traceback);
                PyErr_Clear();

                throw ScilabPythonException(__LINE__, __FILE__, type, value, traceback, gettext("Cannot load the module %s"), className);
            }
            scope.replaceObject(id, pModule);
        }

        return id;
    }
    else
    {
        PyObject * pModule = PyImport_ImportModuleNoBlock(className);
        if (!pModule && PyErr_Occurred())
        {
            PyObject *type, *value, *traceback;
            PyErr_Fetch(&type, &value, &traceback);
            PyErr_NormalizeException(&type, &value, &traceback);
            PyErr_Clear();

            throw ScilabPythonException(__LINE__, __FILE__, type, value, traceback, gettext("Cannot load the module %s"), className);
        }

        int id = scope.addObject(pModule);
        map[_className] = id;

        return id;
    }
}

void ScilabPythonEnvironment::getrepresentation(int id, const ScilabStringStackAllocator & allocator)
{
    PyObject * obj = scope.getObject(id);
    if (!obj)
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), id);
    }

    PyObject * repr = PyObject_Repr(obj);
    char * str = PyString_AsString(repr);
    allocator.allocate(1, 1, &str);
    Py_DECREF(repr);
}

std::string ScilabPythonEnvironment::getrepresentation(int id)
{
    PyObject * obj = scope.getObject(id);
    if (!obj)
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), id);
    }

    PyObject * repr = PyObject_Repr(obj);
    std::string str = std::string(PyString_AsString(repr));
    Py_DECREF(repr);

    return str;
}

bool ScilabPythonEnvironment::isvalidobject(int id)
{
    return scope.getObject(id) != 0;
}

int ScilabPythonEnvironment::newinstance(int id, int * args, int argsSize)
{
    PyObject * module = scope.getObject(id);
    if (!module)
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), id);
    }

    if (!PyModule_Check(module))
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("The object is not a module"));
    }

    char * name = PyModule_GetName(module);
    PyObject * dict = PyModule_GetDict(module);
    if (!dict)
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Cannot find a dictionary"));
    }

    PyObject * constructor = PyDict_GetItemString(dict, name);
    if (!PyCallable_Check(constructor))
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Cannot find a constructor"));
    }

    PyObject * pArgs = PyTuple_New(argsSize);
    for (int i = 0; i < argsSize; i++)
    {
        PyObject * obj = scope.getObject(args[i]);
        if (!obj)
        {
            throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), id);
        }
        Py_INCREF(obj);
        PyTuple_SetItem(pArgs, i, obj);
    }

    PyObject * instance = PyObject_Call(constructor, pArgs, 0);
    Py_DECREF(pArgs);

    if (!instance && PyErr_Occurred())
    {
        PyObject * type, * value, * traceback;
        PyErr_Fetch(&type, &value, &traceback);
        PyErr_NormalizeException(&type, &value, &traceback);
        PyErr_Clear();

        throw ScilabPythonException(__LINE__, __FILE__, type, value, traceback, gettext("Unable to instantiate a new object"));
    }

    return scope.addObject(instance);
}

int ScilabPythonEnvironment::invoke(int id, const char * methodName, int * args, int argsSize)
{
    if (*methodName == '\0')
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid method name"));
    }

    if (*methodName == '_')
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Private method: %s"), methodName);
    }

    PyObject * obj = scope.getObject(id);
    if (!obj)
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), id);
    }

    if (!PyObject_HasAttrString(obj, methodName))
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid method name: %s"), methodName);
    }

    PyObject * method = PyObject_GetAttrString(obj, methodName);
    if (!method)
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid method name: %s"), methodName);
    }

    if (!PyCallable_Check(method))
    {
        Py_DECREF(method);
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid method name: %s"), methodName);
    }

    PyObject * pArgs = PyTuple_New(argsSize);
    for (int i = 0; i < argsSize; i++)
    {
        PyObject * obj = scope.getObject(args[i]);
        if (!obj)
        {
            throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), id);
        }
        Py_INCREF(obj);
        PyTuple_SetItem(pArgs, i, obj);
    }

    PyObject * result = PyObject_Call(method, pArgs, 0);
    Py_DECREF(pArgs);

    if (!result && PyErr_Occurred())
    {
        PyObject * type, * value, * traceback;
        PyErr_Fetch(&type, &value, &traceback);
        PyErr_NormalizeException(&type, &value, &traceback);
        PyErr_Clear();

        throw ScilabPythonException(__LINE__, __FILE__, type, value, traceback, gettext("Unable to invoke the method: %s"), methodName);
    }

    if (result == Py_None)
    {
        return VOID_OBJECT;
    }

    return scope.addObject(result);
}

void ScilabPythonEnvironment::setfield(int id, const char * fieldName, int idarg)
{
    if (*fieldName == '\0')
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid field name"));
    }

    if (*fieldName == '_')
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Private field: %s"), fieldName);
    }

    PyObject * obj = scope.getObject(id);
    if (!obj)
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), id);
    }

    if (!PyObject_HasAttrString(obj, fieldName))
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid field name: %s"), fieldName);
    }

    PyObject * value = scope.getObject(idarg);
    if (!value)
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), id);
    }

    int ret = PyObject_SetAttrString(obj, fieldName, value);
    if (ret == -1 && PyErr_Occurred())
    {
        PyObject * type, * value, * traceback;
        PyErr_Fetch(&type, &value, &traceback);
        PyErr_NormalizeException(&type, &value, &traceback);
        PyErr_Clear();

        throw ScilabPythonException(__LINE__, __FILE__, type, value, traceback, gettext("Unable to set the field: %s"), fieldName);
    }
}

int ScilabPythonEnvironment::getfield(int id, const char * fieldName)
{
    if (*fieldName == '\0')
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid field name"));
    }

    if (*fieldName == '_')
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Private field: %s"), fieldName);
    }

    PyObject * obj = scope.getObject(id);
    if (!obj)
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), id);
    }

    if (!PyObject_HasAttrString(obj, fieldName))
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid field name: %s"), fieldName);
    }

    PyObject * field = PyObject_GetAttrString(obj, fieldName);
    if (!field && PyErr_Occurred())
    {
        PyObject * type, * value, * traceback;
        PyErr_Fetch(&type, &value, &traceback);
        PyErr_NormalizeException(&type, &value, &traceback);
        PyErr_Clear();

        throw ScilabPythonException(__LINE__, __FILE__, type, value, traceback, gettext("Unable to get the field value: %s"), fieldName);
    }

    return scope.addObject(field);
}

int ScilabPythonEnvironment::getfieldtype(int id, const char * fieldName)
{
    if (*fieldName == '_' || *fieldName == '\0')
    {
        return -1;
    }

    PyObject * obj = scope.getObject(id);
    if (!obj)
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), id);
    }

    if (!PyObject_HasAttrString(obj, fieldName))
    {
        return -1;
    }

    PyObject * field = PyObject_GetAttrString(obj, fieldName);
    if (!field)
    {
        return -1;
    }

    if (PyCallable_Check(field))
    {
        Py_DECREF(field);
        return 0;
    }
    else
    {
        Py_DECREF(field);
        return 1;
    }
}

int ScilabPythonEnvironment::getarrayelement(int id, int * index, int length)
{
    PyObject * obj = scope.getObject(id);
    if (!obj)
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), id);
    }

    if (!PyList_Check(obj) && !PyArray_Check(obj))
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Not a list or an array"));
    }

    if (length == 0)
    {
        return 0;
    }

    if (PyList_Check(obj))
    {
        for (int i = 0; i < length; i++)
        {
            if (index[i] < 0 || index[i] >= PyList_Size(obj))
            {
                throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid index"));
            }
            obj = PyList_GetItem(obj, index[i]);
            if (!PyList_Check(obj))
            {
                throw ScilabPythonException(__LINE__, __FILE__, gettext("Not a list"));
            }
        }
    }
    else if (PyArray_Check(obj))
    {
        PyArrayObject * arr = (PyArrayObject *)obj;
        npy_intp * ind = (npy_intp *)index;

        if (length != PyArray_NDIM(arr))
        {
            throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid index dimension"));
        }

        npy_intp * dims = PyArray_DIMS(arr);

        if (sizeof(int) != sizeof(npy_intp))
        {
            ind = new npy_intp[length];
            for (int i = 0; i < length; i++)
            {
                // TODO: ajouter une option pr utiliser l'indexation scilab
                if (index[i] < 0 || index[i] >= dims[i])
                {
                    delete[] ind;
                    throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid index at position %d"), i + 1);
                }

                ind[i] = (npy_intp)index[i];
            }
        }

        obj = PyArray_GETITEM(arr, PyArray_GetPtr(arr, ind));

        if (sizeof(int) != sizeof(npy_intp))
        {
            delete[] ind;
        }
    }
    else
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Must be a list or a Numpy array"));
    }

    return scope.addObject(obj);
}

void ScilabPythonEnvironment::setarrayelement(int id, int * index, int length, int idArg)
{
    PyObject * obj = scope.getObject(id);
    if (!obj)
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), id);
    }

    PyObject * value = scope.getObject(idArg);
    if (!value)
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), id);
    }

    if (PyList_Check(obj))
    {
        if (length == 0)
        {
            return;
        }

        for (int i = 0; i < length - 1; i++)
        {
            if (index[i] < 0 || index[i] >= PyList_Size(obj))
            {
                throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid index at position %d"), i + 1);
            }
            obj = PyList_GetItem(obj, index[i]);
            if (!PyList_Check(obj))
            {
                throw ScilabPythonException(__LINE__, __FILE__, gettext("Not a list at position %d"), index[i]);
            }
        }

        Py_INCREF(value);
        PyList_SetItem(obj, index[length - 1], value);
    }
    else if (PyArray_Check(obj))
    {
        PyArrayObject * arr = (PyArrayObject *)obj;
        npy_intp * ind = (npy_intp *)index;

        if (length != PyArray_NDIM(arr))
        {
            throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid index dimension"));
        }

        npy_intp * dims = PyArray_DIMS(arr);

        if (sizeof(int) != sizeof(npy_intp))
        {
            ind = new npy_intp[length];
            for (int i = 0; i < length; i++)
            {
                // TODO: ajouter une option pr utiliser l'indexation scilab
                if (index[i] < 0 || index[i] >= dims[i])
                {
                    delete[] ind;
                    throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid index at position %d"), i);
                }

                ind[i] = (npy_intp)index[i];
            }
        }

        int ret = PyArray_SETITEM(arr, PyArray_GetPtr(arr, ind), value);

        if (sizeof(int) != sizeof(npy_intp))
        {
            delete[] ind;
        }

        if (ret == -1)
        {
            throw ScilabPythonException(__LINE__, __FILE__, gettext("Cannot set the value in the array"));
        }
    }
}

int ScilabPythonEnvironment::cast(int id, char * className)
{
    throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid operation"));
}

int ScilabPythonEnvironment::castwithid(int id, int classId)
{
    throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid operation"));
}

void ScilabPythonEnvironment::removeobject(int id)
{
    scope.removeObject(id);
}

void ScilabPythonEnvironment::getaccessiblemethods(int id, const ScilabStringStackAllocator & allocator)
{
    getAccessibleFields(id, allocator, false);
}

void ScilabPythonEnvironment::getaccessiblefields(int id, const ScilabStringStackAllocator & allocator)
{
    getAccessibleFields(id, allocator, true);
}

std::string ScilabPythonEnvironment::getclassname(int id)
{
    PyObject * obj = scope.getObject(id);
    if (!obj)
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), id);
    }

    PyObject * __name__ = PyObject_GetAttrString(obj, "__name__");
    char * name = PyString_AsString(__name__);
    std::string str = std::string(name);
    Py_DECREF(__name__);

    return str;
}

VariableType ScilabPythonEnvironment::isunwrappable(int id)
{
    return wrapper.isunwrappable(id);
}

int ScilabPythonEnvironment::compilecode(char * className, char ** code, int size)
{
    std::stringstream ss;
    for (int i = 0; i < size; i++)
    {
        ss << code[i] << std::endl;
    }

    PyObject * obj = Py_CompileString(ss.str().c_str(), className, 0);
    if (!obj && PyErr_Occurred())
    {
        PyObject * type, * value, * traceback;
        PyErr_Fetch(&type, &value, &traceback);
        PyErr_NormalizeException(&type, &value, &traceback);
        PyErr_Clear();

        throw ScilabPythonException(__LINE__, __FILE__, type, value, traceback, gettext("Unable to compile the given code"));
    }

    return scope.addObject(obj);
}

void ScilabPythonEnvironment::enabletrace(const char * filename)
{
    if (!traceEnabled)
    {
        traceEnabled = true;
        file = new std::ofstream(filename, std::ios::out | std::ios::trunc);
        if (!file)
        {
            throw ScilabPythonException(__LINE__, __FILE__, gettext("Cannot open the given file"));
        }
    }
    else
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Trace already enabled"));
    }
}

void ScilabPythonEnvironment::disabletrace()
{
    if (traceEnabled)
    {
        traceEnabled = false;
        file->close();
        delete file;
        file = 0;
    }
}
}
