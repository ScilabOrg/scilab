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
bool ScilabPythonEnvironment::usable = true;
std::map<std::string, int> ScilabPythonEnvironment::mapModuleId = std::map<std::string, int>();
std::map<int, std::string> ScilabPythonEnvironment::mapIdModule = std::map<int, std::string>();

ScilabPythonEnvironment::ScilabPythonEnvironment() : scope(*new PythonVariablesScope(*this)),
    helper(*new PythonOptionsHelper()),
    gwOptions(*new ScilabGatewayOptions()),
    wrapper(*new ScilabPythonEnvironmentWrapper(scope, helper)),
    traceEnabled(false),
    isInit(false),
    scilabStream(*new ScilabStream()),
    file(0) { }

ScilabPythonEnvironment::~ScilabPythonEnvironment()
{
    delete &scope;
    delete &helper;
    delete &gwOptions;
    delete &wrapper;
    delete &scilabStream;

    if (file)
    {
        file->flush();
        file->close();
        delete file;
        file = 0;
    }
}

int ScilabPythonEnvironment::start()
{
    if (!usable)
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Due to Python interpreter limitations, Scilab must be restarted"));
    }

    if (envId == -1)
    {
        instance = new ScilabPythonEnvironment();
        envId = ScilabEnvironments::registerScilabEnvironment(instance);
        instance->Initialize();
        ScilabPythonOStream::initPythonStreams();
        ScilabPythonOStream::setStdOutStream(&instance->scilabStream);
        ScilabPythonOStream::setStdErrStream(&instance->scilabStream);
        instance->helper.setUseLastName(false);
        instance->helper.setNewAllowed(false);
    }

    return envId;
}

void ScilabPythonEnvironment::finish()
{
    if (envId != -1)
    {
        ScilabEnvironments::unregisterScilabEnvironment(envId);
        envId = -1;
        delete instance;
        instance = 0;
        instance->Finalize();
        usable = false;
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

PythonOptionsHelper & ScilabPythonEnvironment::getOptionsHelper()
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
    writeLog("getEnvironmentInfos", "Get informations");

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
    if (traceEnabled)
    {
        std::ostringstream os;
        for (int i = 0; i < argsSize - 1; i++)
        {
            os << args[i] << ", ";
        }
        os << args[argsSize - 1];
        os.flush();

        writeLog("extract", "Extraction on object %d with arguments: %s.", id, os.str().c_str());
    }

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

        int ret = scope.addObject(value);
        writeLog("extract", "returned id: %d.", ret);

        return ret;
    }

    throw ScilabPythonException(__LINE__, __FILE__, gettext("Cannot extract from Python object"));
}

void ScilabPythonEnvironment::insert(int id, int * args, int argsSize)
{
    if (traceEnabled)
    {
        std::ostringstream os;
        for (int i = 0; i < argsSize - 1; i++)
        {
            os << args[i] << ", ";
        }
        os << args[argsSize - 1];
        os.flush();

        writeLog("insert", "Insertion on object %d with arguments: %s.", id, os.str().c_str());
    }

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

        writeLog("insert", "success.");

        return;
    }

    throw ScilabPythonException(__LINE__, __FILE__, gettext("Cannot insert in Python object"));
}

void ScilabPythonEnvironment::garbagecollect() { }

void ScilabPythonEnvironment::addtoclasspath(const char * path)
{
    writeLog("addtoclasspath", "Add the path %s to syspath", path);

    PyObject * syspath = PySys_GetObject(const_cast<char *>("path"));
    PyObject * _path = PyString_FromString(path);
    PyList_Append(syspath, _path);
    Py_DECREF(_path);
}

void ScilabPythonEnvironment::getclasspath(const ScilabStringStackAllocator & allocator)
{
    writeLog("getclasspath", "Get syspath");

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

void ScilabPythonEnvironment::addNamedVariable(int id, const char * varName)
{
    writeLog("addNamedVariable", "Associate the variable named %s with object with id %d.", varName, id);

    PyObject * obj = scope.getObject(id);
    if (!obj)
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), id);
    }

    PyObject * _main_ = PyImport_AddModule("__main__");
    if (!_main_)
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Cannot evaluate the code since __main__ is not accessible."));
    }

    if (PyObject_SetAttrString(_main_, varName, obj) == -1)
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Cannot set the variable named %s."), varName);
    }
}

void ScilabPythonEnvironment::evalString(const char ** code, int nbLines, ScilabStringStackAllocator * allocator)
{
    writeLog("evalString", "Evaluate code: %s...(truncated)", *code);

    std::ostringstream os;
    for (int i = 0; i < nbLines; i++)
    {
        os << code[i] << std::endl;
    }
    os.flush();

    PyObject * _main_ = PyImport_AddModule("__main__");
    if (!_main_)
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Cannot evaluate the code since __main__ is not accessible."));
    }

    std::ostream * old;
    std::ostringstream os1;
    if (allocator)
    {
        old = ScilabPythonOStream::setStdOutStream(&os1);
    }

    PyObject * dict = PyModule_GetDict(_main_);
    PyObject * ret = PyRun_StringFlags(os.str().c_str(), Py_file_input, dict, dict, 0);

    if (allocator)
    {
        ScilabPythonOStream::setStdOutStream(old);
    }

    if (!ret && PyErr_Occurred())
    {
        PyObject *type, *value, *traceback;
        PyErr_Fetch(&type, &value, &traceback);
        PyErr_NormalizeException(&type, &value, &traceback);
        PyErr_Clear();

        throw ScilabPythonException(__LINE__, __FILE__, type, value, traceback, gettext("Cannot evaluate the code"));
    }

    if (allocator)
    {
        std::vector<char *> buf = breakInLines(os1.str());
        allocator->allocate(buf.size(), 1, &(buf[0]));
        for (std::vector<char *>::iterator i = buf.begin(); i != buf.end(); i++)
        {
            free(*i);
        }
    }

    Py_DECREF(ret);
}

int ScilabPythonEnvironment::createarray(char * className, int * dims, int len)
{
    if (traceEnabled)
    {
        std::ostringstream os;
        for (int i = 0; i < len - 1; i++)
        {
            os << dims[i] << ", ";
        }
        os << dims[len - 1];
        os.flush();

        writeLog("createarray", "Create a multi-list with dimensions %s.", os.str().c_str());
    }

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
    int ret = scope.addObject(obj);

    writeLog("createarray", "returned id %d.", ret);

    return ret;
}

int ScilabPythonEnvironment::loadclass(char * className, bool isNamedVarCreated, bool allowReload)
{
    writeLog("loadclass", "Load the module %s and allowReload is set to %s", className, allowReload ? "true" : "false");

    std::string _className(className);
    std::string base(_className);
    bool changed = false;

    if (isNamedVarCreated)
    {
        std::size_t pos = _className.find_first_of('.');
        if (pos != std::string::npos)
        {
            base = _className.substr(0, pos);
            changed = true;
        }
    }

    std::map<std::string, int>::const_iterator it = mapModuleId.find(base);
    if (it != mapModuleId.end())
    {
        int id = it->second;
        PyObject * obj = scope.getObject(id);
        if (!obj)
        {
            mapModuleId.erase(base);
        }
        else
        {
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

            writeLog("loadclass", "returned id %d.", id);

            return id;
        }
    }

    PyObject * pModule = PyImport_ImportModuleNoBlock(className);
    if (!pModule && PyErr_Occurred())
    {
        PyObject *type, *value, *traceback;
        PyErr_Fetch(&type, &value, &traceback);
        PyErr_NormalizeException(&type, &value, &traceback);
        PyErr_Clear();

        throw ScilabPythonException(__LINE__, __FILE__, type, value, traceback, gettext("Cannot load the module %s"), className);
    }

    PyObject * baseModule = pModule;

    if (changed)
    {
        baseModule = PyImport_AddModule(base.c_str());

        // We put the name in Python space
        if (helper.getAttachModule())
        {
            PyObject * _main_ = PyImport_AddModule("__main__");
            PyObject_SetAttrString(_main_, base.c_str(), baseModule);
        }
    }

    int id = scope.addObject(baseModule);
    mapModuleId[base] = id;
    mapIdModule[id] = base;

    writeLog("loadclass", "returned id %d.", id);

    return id;
}

void ScilabPythonEnvironment::getrepresentation(int id, const ScilabStringStackAllocator & allocator)
{
    writeLog("getrepresentation", "Get the representation of object %d.", id);

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
    writeLog("getrepresentation", "Get the representation of object %d.", id);

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
    bool ret = scope.isValid(id);
    writeLog("isvalidobject", "Test the validity of object %d which is%s valid.", id, ret ? "" : " not");

    return ret;
}

int ScilabPythonEnvironment::newinstance(int id, int * args, int argsSize)
{
    throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid operation: newinstance."));
}

int ScilabPythonEnvironment::operation(int idA, int idB, const OperatorsType type)
{
    writeLog("operation", "Evalute an operation (%d) with objects with id %d and %d.", (int)type, idA, idB);

    const char * opName = getOpNameFromType(type);

    PyObject * _operator_ = PyImport_AddModule("operator");
    if (!_operator_)
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Cannot get the operator module."));
    }

    PyObject * objA = scope.getObject(idA);
    if (!objA)
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), idA);
    }

    PyObject * objB = 0;
    if (idB != -1)
    {
        // Binary op
        objB = scope.getObject(idB);
        if (!objB)
        {
            throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), idB);
        }
    }

    PyObject * _op_ = PyObject_GetAttrString(_operator_, opName);

    PyObject * pArgs = PyTuple_New(objB ? 2 : 1);
    Py_INCREF(objA);
    PyTuple_SetItem(pArgs, 0, objA);

    if (objB)
    {
        Py_INCREF(objB);
        PyTuple_SetItem(pArgs, 1, objB);
    }

    PyObject * result = PyObject_Call(_op_, pArgs, 0);
    Py_DECREF(pArgs);
    Py_DECREF(_op_);

    if (!result && PyErr_Occurred())
    {
        PyObject * type, * value, * traceback;
        PyErr_Fetch(&type, &value, &traceback);
        PyErr_NormalizeException(&type, &value, &traceback);
        PyErr_Clear();

        throw ScilabPythonException(__LINE__, __FILE__, type, value, traceback, gettext("Unable to make the operation (%s)."), opName);
    }

    int ret = scope.addObject(result);
    writeLog("operation", "returned id %d.", ret);

    return ret;
}

int * ScilabPythonEnvironment::invoke(int id, const char * methodName, int * args, int argsSize)
{
    if (traceEnabled)
    {
        std::ostringstream os;
        for (int i = 0; i < argsSize - 1; i++)
        {
            os << args[i] << ", ";
        }
        os << args[argsSize - 1];
        os.flush();

        writeLog("invoke", "Invoke the method %s on object %d with arguments: %s.", methodName, id, os.str().c_str());
    }

    if (*methodName == '\0')
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid method name"));
    }

    if (!helper.getShowPrivate() && *methodName == '_')
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
    Py_DECREF(method);

    if (!result && PyErr_Occurred())
    {
        PyObject * type, * value, * traceback;
        PyErr_Fetch(&type, &value, &traceback);
        PyErr_NormalizeException(&type, &value, &traceback);
        PyErr_Clear();

        throw ScilabPythonException(__LINE__, __FILE__, type, value, traceback, gettext("Unable to invoke the method: %s"), methodName);
    }

    int * ret = new int[2];
    *ret = 1;

    if (result == Py_None)
    {
        ret[1] = VOID_OBJECT;
        writeLog("invoke", "The method returned void.");

        return ret;
    }

    ret[1] = scope.addObject(result);
    writeLog("invoke", "returned id %d.", ret[1]);

    return ret;
}

void ScilabPythonEnvironment::setfield(int id, const char * fieldName, int idarg)
{
    writeLog("setfield", "Set the field named %s with value id %d on object with id %d.", fieldName, idarg, id);

    if (*fieldName == '\0')
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid field name"));
    }

    if (!helper.getShowPrivate() && *fieldName == '_')
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

    writeLog("setfield", "Value successfully set.");
}

int ScilabPythonEnvironment::getfield(int id, const char * fieldName)
{
    writeLog("getfield", "Get the field named %s on object with id %d.", fieldName, id);

    if (*fieldName == '\0')
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid field name"));
    }

    if (!helper.getShowPrivate() && *fieldName == '_')
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

    int ret = scope.addObject(field);
    writeLog("getfield", "returned id %d.", ret);

    return ret;
}

int ScilabPythonEnvironment::getfieldtype(int id, const char * fieldName)
{
    writeLog("getfieldtype", "Get the type of the field %s on object with id %d.", fieldName, id);

    if ((!helper.getShowPrivate() && *fieldName == '_') || *fieldName == '\0')
    {
        writeLog("getfieldtype", "Return NONE.");
        return -1;
    }

    PyObject * obj = scope.getObject(id);
    if (!obj)
    {
        throw ScilabPythonException(__LINE__, __FILE__, gettext("Invalid object with id %d"), id);
    }

    if (!PyObject_HasAttrString(obj, fieldName))
    {
        writeLog("getfieldtype", "Return NONE.");
        return -1;
    }

    PyObject * field = PyObject_GetAttrString(obj, fieldName);
    if (!field)
    {
        writeLog("getfieldtype", "Return NONE.");
        return -1;
    }

    if (PyCallable_Check(field))
    {
        Py_DECREF(field);
        writeLog("getfieldtype", "Return METHOD.");
        return 0;
    }
    else
    {
        Py_DECREF(field);
        writeLog("getfieldtype", "Return FIELD.");
        return 1;
    }
}

int ScilabPythonEnvironment::getarrayelement(int id, int * index, int length)
{
    if (traceEnabled)
    {
        std::ostringstream os;
        for (int i = 0; i < length - 1; i++)
        {
            os << index[i] << ", ";
        }
        os << index[length - 1];
        os.flush();

        writeLog("getarrayelement", "Get element from array with id %d and with index: %s.", id, os.str().c_str());
    }

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
            if (i != length - 1 && !PyList_Check(obj))
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

    int ret = scope.addObject(obj);
    writeLog("getarrayelement", "returned id %d.", ret);

    return ret;
}

void ScilabPythonEnvironment::setarrayelement(int id, int * index, int length, int idArg)
{
    if (traceEnabled)
    {
        std::ostringstream os;
        for (int i = 0; i < length - 1; i++)
        {
            os << index[i] << ", ";
        }
        os << index[length - 1];
        os.flush();

        writeLog("setarrayelement", "Set element with id %d in array with id %d and with index: %s.", idArg, id, os.str().c_str());
    }

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

    writeLog("setarrayelement", "Successfully set");
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
    writeLog("removeobject", "Remove object with id %d.", id);
    PyObject * obj = scope.removeObjectWithoutDecref(id);
    if (obj)
    {
        if (PyModule_Check(obj))
        {
            std::map<int, std::string>::const_iterator it = mapIdModule.find(id);
            if (it != mapIdModule.end())
            {
                mapModuleId.erase(it->second);
            }
        }
        Py_DECREF(obj);
    }
}

void ScilabPythonEnvironment::getaccessiblemethods(int id, const ScilabStringStackAllocator & allocator)
{
    writeLog("getaccessiblemethods", "Get accessible methods on object with id %d.", id);
    getAccessibleFields(id, allocator, false);
}

void ScilabPythonEnvironment::getaccessiblefields(int id, const ScilabStringStackAllocator & allocator)
{
    writeLog("getaccessiblefields", "Get accessible fields on object with id %d.", id);
    getAccessibleFields(id, allocator, true);
}

std::string ScilabPythonEnvironment::getclassname(int id)
{
    writeLog("getclassname", "Get the class name of object with id %d.", id);

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
    writeLog("isunwrappable", "Test if the object with id %d is unwrappable.");

    return wrapper.isunwrappable(id);
}

int ScilabPythonEnvironment::compilecode(char * className, char ** code, int size)
{
    writeLog("compilecode", "Compile the code %s...", *code);

    std::ostringstream os;
    for (int i = 0; i < size; i++)
    {
        os << code[i] << std::endl;
    }
    os.flush();

    PyObject * obj = Py_CompileString(os.str().c_str(), className, 0);
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
        file = new std::ofstream(filename, std::ios::out | std::ios::trunc);
        if (!file || file->fail())
        {
            if (file)
            {
                file->close();
                delete file;
            }

            throw ScilabPythonException(__LINE__, __FILE__, gettext("Cannot open the given file %s."), filename);
        }
        traceEnabled = true;
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
