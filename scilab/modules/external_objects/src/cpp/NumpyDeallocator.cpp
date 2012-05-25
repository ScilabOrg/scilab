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

#include "NumpyDeallocator.hxx"

namespace org_modules_external_objects_python
{

PyTypeObject NumpyDeallocator::_MyDeallocType =
{
    PyObject_HEAD_INIT(0)
    0,                                          /*ob_size*/
    "mydeallocator",                            /*tp_name*/
    sizeof(struct _MyDeallocStruct),            /*tp_basicsize*/
    0,                                          /*tp_itemsize*/
    _mydealloc_dealloc,                         /*tp_dealloc*/
    0,                                          /*tp_print*/
    0,                                          /*tp_getattr*/
    0,                                          /*tp_setattr*/
    0,                                          /*tp_compare*/
    0,                                          /*tp_repr*/
    0,                                          /*tp_as_number*/
    0,                                          /*tp_as_sequence*/
    0,                                          /*tp_as_mapping*/
    0,                                          /*tp_hash */
    0,                                          /*tp_call*/
    0,                                          /*tp_str*/
    0,                                          /*tp_getattro*/
    0,                                          /*tp_setattro*/
    0,                                          /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT,                         /*tp_flags*/
    "Internal deallocator object",              /* tp_doc */
};

bool NumpyDeallocator::isInit = init();

void NumpyDeallocator::attach_deallocator(PyObject * array, void * mem)
{
    PyObject * newobj = _PyObject_New(&_MyDeallocType);
    ((struct _MyDeallocStruct *)newobj)->memory = mem;

    PyArray_BASE(array) = newobj;
}

void NumpyDeallocator::_mydealloc_dealloc(PyObject * self)
{
    delete[] ((struct _MyDeallocStruct*)self)->memory;
    self->ob_type->tp_free((PyObject *)self);
}
}
