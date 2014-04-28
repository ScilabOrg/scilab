/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*/


#ifndef __API_VARIABLE_HXX__
#define __API_VARIABLE_HXX__

#include "dynlib_api_scilab.h"
#include "internal.hxx"

namespace api_scilab
{
class Variable
{
private :
protected :
    bool created;
public :
    Variable() {};
    virtual ~Variable() {};

    virtual types::InternalType::ScilabType getType() = 0;
    void setReturnVariable()
    {
        created = false;
    }

    virtual void* getReturnVariable() = 0;
    virtual int getSize() = 0;
};

class Matrix : public Variable
{
public :
    Matrix() {};
    virtual ~Matrix() {};

    virtual int getDims() = 0;
    virtual int* getDimsArray() = 0;
    virtual int getRows() = 0;
    virtual int getCols() = 0;
    virtual int getSize() = 0;
    virtual bool isScalar() = 0;
    virtual bool isVector() = 0;
    virtual bool isRowVector() = 0;
    virtual bool isColVector() = 0;
    virtual bool isHypermatrix() = 0;
};

template<class XType> //XType is types::Double for example
class VariableT : public Matrix
{
protected :
    XType* data;

public :
    VariableT() {};
    virtual ~VariableT() {};

    virtual void* getReturnVariable()
    {
        return (void*)data;
    }

};

template<class Type, class XType> //Type is double and XType is Double
class MatrixT : public VariableT<XType>
{
public :
    virtual Type* getReal()
    {
        return get();
    }
    virtual Type* get()
    {
        if (data)
        {
            return data->get();
        }

        return NULL;
    }

    virtual Type* getImg()
    {
        if (data)
        {
            return data->getImg();
        }

        return NULL;
    }

    virtual Type getReal(int _iIndex)
    {
        return get(_iIndex);
    }
    virtual Type get(int _iIndex)
    {
        if (data)
        {
            return data->get(_iIndex);
        }

        return 0;
    }


    virtual Type getImg(int _iIndex)
    {
        if (data)
        {
            return data->getImg(_iIndex);
        }

        return 0;
    }

    virtual Type getReal(int* _piCoordinates)
    {
        return get(_piCoordinates);
    }
    virtual Type get(int* _piCoordinates)
    {
        if (data)
        {
            int index = data->getIndex(_piCoordinates);
            return data->get(index);
        }

        return 0;
    }

    virtual Type getImg(int* _piCoordinates)
    {
        if (data)
        {
            int index = data->getIndex(_piCoordinates);
            return data->getImg(index);
        }

        return 0;
    }

    virtual void setReal(int _iIndex, Type _dbl)
    {
        set(_iIndex, _dbl);
    }
    virtual void set(int _iIndex, Type _dbl)
    {
        if (data)
        {
            data->set(_iIndex, _dbl);
        }
    }

    virtual void setImg(int _iIndex, Type _dbl)
    {
        if (data)
        {
            data->setImg(_iIndex, _dbl);
        }
    }

    virtual void setReal(int* _piCoordinates, Type _dbl)
    {
        set(_piCoordinates, _dbl);
    }
    virtual void set(int* _piCoordinates, Type _dbl)
    {
        if (data)
        {
            int index = data->getIndex(_piCoordinates);
            data->set(index, _dbl);
        }
    }

    virtual void setImg(int* _piCoordinates, Type _dbl)
    {
        if (data)
        {
            int index = data->getIndex(_piCoordinates);
            data->setImg(index, _dbl);
        }
    }

    virtual void setReal(Type* _pdblCopy)
    {
        set(_pdblCopy);
    }
    virtual void set(Type* _pdblCopy)
    {
        if (data)
        {
            data->set(_pdblCopy);
        }
    }

    virtual void setImg(Type* _pdblCopy)
    {
        if (data)
        {
            data->setImg(_pdblCopy);
        }
    }

    virtual int getDims()
    {
        if (data)
        {
            return data->getDims();
        }

        return 0;
    }

    virtual int* getDimsArray()
    {
        if (data)
        {
            return data->getDimsArray();
        }

        return NULL;
    }

    virtual int getRows()
    {
        if (data)
        {
            return data->getRows();
        }

        return 0;
    }

    virtual int getCols()
    {
        if (data)
        {
            return data->getCols();
        }

        return 0;
    }

    virtual int getSize()
    {
        if (data)
        {
            return data->getSize();
        }

        return 0;
    }

    virtual bool isScalar()
    {
        if (data)
        {
            return data->getSize() == 1;
        }

        return false;
    }

    virtual bool isVector()
    {
        if (data)
        {
            return data->isVector();
        }

        return false;
    }

    virtual bool isRowVector()
    {
        if (data)
        {
            if (data->isVector() && data->getDimsArray()[0] != 1)
            {
                return true;
            }
        }

        return false;
    }

    virtual bool isColVector()
    {
        if (data)
        {
            if (data->isVector() && data->getDimsArray()[1] != 1)
            {
                return true;
            }
        }

        return false;
    }

    virtual bool isHypermatrix()
    {
        if (data)
        {
            return data->getDims() > 2;
        }

        return false;
    }

    virtual bool isComplex()
    {
        if (data)
        {
            return data->isComplex();
        }

        return false;
    }

    virtual void setComplex(bool _bComplex)
    {
        if (data)
        {
            return data->setComplex(_bComplex);
        }
    }

};

template <class XType>
class Container : public VariableT<XType>
{

};
}
#endif /* ! __API_VARIABLE_HXX__ */

