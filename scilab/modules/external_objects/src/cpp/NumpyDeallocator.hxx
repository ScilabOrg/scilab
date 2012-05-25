/*
  This file is part of Sciscipy and has been updated for Epfs purposes.

  Sciscipy is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Sciscipy is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Epfs. If not, see <http://www.gnu.org/licenses/>.

  Copyright (c) 2009, Vincent Guffens, Baozeng Ding.
*/

#ifndef __NUMPYDEALLOCATOR_HXX__
#define __NUMPYDEALLOCATOR_HXX__

#include <Python.h>

#ifndef PY_ARRAY_UNIQUE_SYMBOL
#define PY_ARRAY_UNIQUE_SYMBOL SciPyEnv_API
#define NO_IMPORT_ARRAY
#include <numpy/arrayobject.h>
#endif

namespace org_modules_external_objects_python
{

struct _MyDeallocStruct
{
    PyObject_HEAD
    void * memory;
};

class NumpyDeallocator
{

    static PyTypeObject _MyDeallocType;
    static bool isInit;

public:

    static void attach_deallocator(PyObject * array, void * mem);

private:

    static void _mydealloc_dealloc(PyObject * self);

    static bool init()
    {
        _MyDeallocType.tp_new = PyType_GenericNew;
        PyType_Ready(&_MyDeallocType);

        return true;
    }
};
}

#endif // __NUMPYDEALLOCATOR_HXX__
