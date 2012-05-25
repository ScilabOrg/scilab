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

#ifndef __SCILABPYTHONOSTREAM_HXX__
#define __SCILABPYTHONOSTREAM_HXX__

#include <Python.h>

#include <sstream>

namespace org_modules_external_objects_python
{

struct _PythonOStreamStruct
{
    PyObject_HEAD
    std::ostream * out;
};

class ScilabPythonOStream
{
    static PyTypeObject _PythonOStreamType;
    static PyMethodDef _methods[];
    static PyObject * out;
    static PyObject * err;

public:

    static std::ostream * setStdOutStream(std::ostream * _out)
    {
        std::ostream * old = reinterpret_cast<_PythonOStreamStruct *>(out)->out;
        reinterpret_cast<_PythonOStreamStruct *>(out)->out = _out;

        return old;
    }

    static std::ostream * setStdErrStream(std::ostream * _err)
    {
        std::ostream * old = reinterpret_cast<_PythonOStreamStruct *>(err)->out;
        reinterpret_cast<_PythonOStreamStruct *>(err)->out = _err;

        return old;
    }

    static void initPythonStreams()
    {
        _PythonOStreamType.tp_new = PyType_GenericNew;
        PyType_Ready(&_PythonOStreamType);
        out = _PyObject_New(&_PythonOStreamType);
        err = _PyObject_New(&_PythonOStreamType);
        reinterpret_cast<_PythonOStreamStruct *>(out)->out = 0;
        reinterpret_cast<_PythonOStreamStruct *>(err)->out = 0;
        PySys_SetObject(const_cast<char *>("stdout"), out);
        PySys_SetObject(const_cast<char *>("stderr"), err);
    }

private:

    static PyObject * write(PyObject * self, PyObject * args)
    {
        std::size_t size(0);
        _PythonOStreamStruct * selfimpl = reinterpret_cast<_PythonOStreamStruct *>(self);
        if (selfimpl->out)
        {
            char * data;
            if (!PyArg_ParseTuple(args, "s", &data))
            {
                return 0;
            }

            std::string str(data);
            *selfimpl->out << str;
            if (*(str.end() - 1) == '\n')
            {
                *selfimpl->out << std::flush;
            }
            size = str.size();
        }
        return PyLong_FromSize_t(size);
    }

    static PyObject * flush(PyObject * self, PyObject * args)
    {
        _PythonOStreamStruct * selfimpl = reinterpret_cast<_PythonOStreamStruct *>(self);
        if (selfimpl->out)
        {
            selfimpl->out->flush();
        }

        return Py_BuildValue("");
    }
};

}

#endif // __SCILABPYTHONOSTREAM_HXX__
