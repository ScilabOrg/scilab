/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
#include "struct.hxx"
#include "string.hxx"
#include "double.hxx"
#include "list.hxx"
#include "int.hxx"
#include "localization.hxx"

namespace types
{
Struct::Struct()
{
    m_bDisableCloneInCopyValue = false;
    SingleStruct** pIT  = NULL;
    int piDims[2] = {0, 0};
    create(piDims, 2, &pIT, NULL);
#ifndef NDEBUG
    //Inspector::addItem(this);
#endif
}

Struct::Struct(int _iRows, int _iCols)
{
    m_bDisableCloneInCopyValue = false;
    SingleStruct** pIT  = NULL;
    SingleStruct *p = new SingleStruct();
    int piDims[2] = {_iRows, _iCols};
    create(piDims, 2, &pIT, NULL);
    for (int i = 0 ; i < getSize() ; i++)
    {
        set(i, p);
    }
    delete p;
#ifndef NDEBUG
    //Inspector::addItem(this);
#endif
}

Struct::Struct(int _iDims, int* _piDims)
{
    m_bDisableCloneInCopyValue = false;
    SingleStruct** pIT  = NULL;
    create(_piDims, _iDims, &pIT, NULL);
    for (int i = 0 ; i < getSize() ; i++)
    {
        SingleStruct *p = new SingleStruct();
        set(i, p);
        delete p;
    }
#ifndef NDEBUG
    //Inspector::addItem(this);
#endif
}

Struct::~Struct()
{
    if (isDeletable() == true)
    {
        for (int i = 0 ; i < getSize() ; i++)
        {
            SingleStruct *pStr = m_pRealData[i];
            if (pStr)
            {
                pStr->DecreaseRef();
                if (pStr->isDeletable())
                {
                    //std::wcout << L"delete sub struct(" << i << L") : " << pStr << std::endl;
                    delete pStr;
                }
                else
                {
                    //std::wcout << L"!!!!!!!!!!!!!!! pas touche sub struct(" << i << L") : " << pStr->getRef() << L" @" << pStr << std::endl;
                }
            }
        }
    }
#ifndef NDEBUG
    //Inspector::removeItem(this);
#endif
}

Struct::Struct(Struct *_oStructCopyMe)
{
    m_bDisableCloneInCopyValue = false;
    SingleStruct** pIT = NULL;
    create(_oStructCopyMe->getDimsArray(), _oStructCopyMe->getDims(), &pIT, NULL);
    for (int i = 0 ; i < getSize() ; i++)
    {
        m_pRealData[i] = NULL;
    }

    for (int i = 0 ; i < getSize() ; i++)
    {
        pIT[i] = _oStructCopyMe->get(i)->clone();
    }
#ifndef NDEBUG
    //Inspector::addItem(this);
#endif
}

InternalType* Struct::clone()
{
    return new Struct(this);
}

bool Struct::transpose(InternalType *& out)
{
    if (isScalar())
    {
        out = clone();
        return true;
    }

    if (m_iDims == 2)
    {
        Struct * pSt = new Struct();
        out = pSt;
        SingleStruct** pSSt = NULL;
        int piDims[2] = {getCols(), getRows()};
        pSt->create(piDims, 2, &pSSt, NULL);

        Transposition::transpose_clone(getRows(), getCols(), m_pRealData, pSt->m_pRealData);

        return true;
    }

    return false;
}

bool Struct::extract(const std::wstring & name, InternalType *& out)
{
    if (exists(name))
    {
        out = extractField(name);
    }
    else
    {
        wchar_t szError[bsiz];
        os_swprintf(szError, bsiz, _W("Unknown field : %ls.\n").c_str(), name.c_str());
        throw std::wstring(szError);
    }

    return true;
}

bool Struct::set(int _iRows, int _iCols, SingleStruct* _pIT)
{
    if (_iRows < getRows() && _iCols < getCols())
    {
        return set(_iCols * getRows() + _iRows, _pIT);
    }
    return false;
}

bool Struct::set(int _iRows, int _iCols, const SingleStruct* _pIT)
{
    if (_iRows < getRows() && _iCols < getCols())
    {
        return set(_iCols * getRows() + _iRows, _pIT);
    }
    return false;
}

bool Struct::set(int _iIndex, SingleStruct* _pIT)
{
    if (_iIndex < getSize())
    {
        if (m_bDisableCloneInCopyValue && m_pRealData[_iIndex] == _pIT)
        {
            return true;
        }

        InternalType* pOld = m_pRealData[_iIndex];

        m_pRealData[_iIndex] = copyValue(_pIT);
        m_pRealData[_iIndex]->IncreaseRef();

        if (pOld != NULL)
        {
            pOld->DecreaseRef();
            if (pOld->isDeletable())
            {
                delete pOld;
            }
        }

        return true;
    }
    return false;
}

bool Struct::set(int _iIndex, const SingleStruct* _pIT)
{
    if (_iIndex < getSize())
    {
        InternalType* pOld = m_pRealData[_iIndex];

        m_pRealData[_iIndex] = const_cast<SingleStruct*>(_pIT)->clone();

        if (pOld != NULL)
        {
            pOld->DecreaseRef();
            if (pOld->isDeletable())
            {
                delete pOld;
            }
        }

        return true;
    }
    return false;
}

bool Struct::set(SingleStruct** _pIT)
{
    for (int i = 0 ; i < getSize() ; i++)
    {
        if (set(i, _pIT[i]) == false)
        {
            return false;
        }
    }
    return true;
}

String* Struct::getFieldNames()
{
    if (getSize() != 0)
    {
        return get(0)->getFieldNames();
    }
    else
    {
        return NULL;
    }
}

bool Struct::exists(const std::wstring& _sKey)
{
    if (getSize() != 0)
    {
        return get(0)->exists(_sKey);
    }
    else
    {
        return false;
    }
}

bool Struct::operator==(const InternalType& it)
{
    if (const_cast<InternalType &>(it).isStruct() == false)
    {
        return false;
    }

    Struct* pStr = const_cast<InternalType &>(it).getAs<Struct>();

    for (int i = 0 ; i < getDims() ; i++)
    {
        if (pStr->getDimsArray()[i] != getDimsArray()[i])
        {
            return false;
        }
    }

    for (int i = 0 ; i < getSize() ; i++)
    {
        if (*get(i) != *pStr->get(i))
        {
            return false;
        }
    }
    return true;
}

bool Struct::operator!=(const InternalType& it)
{
    return !(*this == it);
}

SingleStruct* Struct::getNullValue()
{
    return new SingleStruct();
}

Struct* Struct::createEmpty(int _iDims, int* _piDims, bool _bComplex)
{
    Struct* pStr = new Struct(_iDims, _piDims);
    pStr->setCloneInCopyValue(!m_bDisableCloneInCopyValue);
    return pStr;
}

SingleStruct* Struct::copyValue(SingleStruct* _pData)
{
    SingleStruct* pStr = NULL;
    if (m_bDisableCloneInCopyValue)
    {
        pStr = _pData;
        //pStr->IncreaseRef();
        //std::wcout << L"copyValueWithoutClone -> " << pStr << L" : " << pStr->getRef() << std::endl;
    }
    else
    {
        pStr = _pData->clone();
    }

    return pStr;
}

void Struct::deleteAll()
{
    for (int i = 0 ; i < getSize() ; i++)
    {
        m_pRealData[i]->DecreaseRef();
        if (m_pRealData[i]->isDeletable())
        {
            delete m_pRealData[i];
        }
    }
    delete[] m_pRealData;
    m_pRealData = NULL;
}

void Struct::deleteImg()
{
    return;
}

SingleStruct** Struct::allocData(int _iSize)
{
    SingleStruct** pData = new SingleStruct*[_iSize];
    for (int i = 0 ; i < _iSize ; i++)
    {
        pData[i] = NULL;
    }
    return pData;
}

bool Struct::subMatrixToString(std::wostringstream& ostr, int* _piDims, int _iDims)
{
    return true;
}

bool Struct::addField(const std::wstring& _sKey)
{
    if (getSize() == 0)
    {
        //change dimension to 1x1 and add field
        resize(1, 1);
    }

    for (int i = 0 ; i < getSize() ; i++)
    {
        /*
                    if(get(i)->isRef(1))
                    {//assign more than once
                        //clone it before add field
                        set(i, get(i)->clone());
                    }
        */
        get(i)->addField(_sKey);
    }
    return true;
}

bool Struct::addFieldFront(const std::wstring& _sKey)
{
    if (getSize() == 0)
    {
        //change dimension to 1x1 and add field
        resize(1, 1);
    }

    for (int i = 0 ; i < getSize() ; i++)
    {
        get(i)->addFieldFront(_sKey);
    }

    return true;
}

bool Struct::removeField(const std::wstring& _sKey)
{
    for (int j = 0; j < getSize(); j++)
    {
        get(j)->removeField(_sKey);
    }

    return true;
}

bool Struct::toString(std::wostringstream& ostr)
{
    if (getSize() == 0)
    {
        ostr << L"0x0 struct array with no field.";
    }
    else if (getSize() == 1)
    {
        SingleStruct* pSS =  get(0);
        String* pwstFields =  pSS->getFieldNames();
        if (pwstFields->getSize() == 0)
        {
            ostr << L"1x1 struct array with no field.";
        }

        for (int i = 0 ; i < pwstFields->getSize() ; i++)
        {
            std::wstring wstField(pwstFields->get(i));
            InternalType* pIT = pSS->get(wstField);

            //                ostr << L"  " << wstField << ": ";
            ostr << L"  " << wstField << L": ";
            ostr << pIT->toStringInLine();
            ostr << std::endl;
        }
        delete pwstFields;
    }
    else
    {
        ostr << L"  ";
        for (int i = 0 ; i < m_iDims ; i++)
        {
            if (i > 0)
            {
                ostr << L"x";
            }
            ostr << m_piDims[i];
        }
        ostr << L" struct array with ";

        String* pwstFields = getFieldNames();
        ostr <<  L"fields:" << std::endl;
        for (int i = 0 ; i < pwstFields->getSize() ; i++)
        {
            ostr << L"    " << pwstFields->get(i) << std::endl;
        }
        delete pwstFields;
    }

    return true;
}

List* Struct::extractFieldWithoutClone(std::wstring _wstField)
{
    List* pL = new List();
    for (int j = 0 ; j < getSize() ; j++)
    {
        pL->set(j, get(j)->get(_wstField));
    }

    return pL;
}

std::vector<InternalType*> Struct::extractFields(std::vector<std::wstring> _wstFields)
{
    std::vector<InternalType*> ResultList;

    for (int i = 0 ; i < _wstFields.size() ; i++)
    {
        ResultList.push_back(extractField(_wstFields[i]));
    }

    return ResultList;
}

InternalType * Struct::extractField(const std::wstring & wstField)
{
    if (wstField == L"dims")
    {
        Int32 * pDims = new Int32(1, getDims());
        for (int j = 0 ; j < getDims() ; j++)
        {
            pDims->set(j, getDimsArray()[j]);
        }

        return pDims;
    }
    else
    {
        if (getSize() == 1)
        {
            return get(0)->get(wstField)->clone();
        }
        else
        {
            List * pL = new List();
            for (int j = 0 ; j < getSize() ; j++)
            {
                pL->append(get(j)->get(wstField)->clone());
            }

            return pL;
        }
    }
}

std::vector<InternalType*> Struct::extractFields(typed_list* _pArgs)
{
    std::vector<InternalType*> ResultList;

    int iDims           = (int)_pArgs->size();
    typed_list pArg;

    int* piMaxDim       = new int[iDims];
    int* piCountDim     = new int[iDims];

    int iSeqCount = checkIndexesArguments(this, _pArgs, &pArg, piMaxDim, piCountDim);
    if (iSeqCount == 0)
    {
        ResultList.push_back(createEmptyDouble());
        return ResultList;
    }

    Double* pIndex = pArg[0]->getAs<Double>();

    for (int i = 0 ; i < iSeqCount ; i++)
    {
        int iIndex = (int)pIndex->get(i);

        if (iIndex == 1)
        {
            //struct fields name
            String* pS = getFieldNames();

            String* pFields = new String(1, pS->getSize() + 2);

            pFields->set(0, L"st");
            pFields->set(1, L"dims");

            for (int j = 0 ; j < pS->getSize() ; j++)
            {
                pFields->set(2 + j, pS->get(j));
            }

            delete pS;
            ResultList.push_back(pFields);
        }
        else if (iIndex == 2)
        {
            //struct dims
            Int32* pDims = new Int32(1, getDims());
            for (int j = 0 ; j < getDims() ; j++)
            {
                pDims->set(j, getDimsArray()[j]);
            }

            ResultList.push_back(pDims);
        }
        else if (getSize() == 0)
        {
            break;
        }
        else if (iIndex > get(0)->getData().size() + 2)
        {
            break;
        }
        else if (getSize() == 1)
        {
            //return elements
            std::list<InternalType*> pData = get(0)->getData();
            std::list<InternalType*>::iterator it = pData.begin();
            std::advance(it, iIndex - 3);
            ResultList.push_back((*it)->clone());
        }
        else
        {
            //return each elements for sub structs in a list
            List* pL = new List();

            for (int j = 0 ; j < getSize() ; j++)
            {
                //-2 for fieldlist and dims, -1 for indexed at 0
                std::list<InternalType*> pData = get(j)->getData();
                std::list<InternalType*>::iterator it = pData.begin();
                std::advance(it, iIndex - 3);
                pL->append((*it)->clone());
            }

            ResultList.push_back(pL);
        }
    }

    //free pArg content
    for (int iArg = 0 ; iArg < pArg.size() ; iArg++)
    {
        if (pArg[iArg] != (*_pArgs)[iArg] && pArg[iArg]->isDeletable())
        {
            delete pArg[iArg];
        }
    }

    return ResultList;
}

bool Struct::resize(int _iNewRows, int _iNewCols)
{
    int piDims[2] = {_iNewRows, _iNewCols};
    return resize(piDims, 2);
}

bool Struct::resize(int* _piDims, int _iDims)
{
    bool bRes = ArrayOf<SingleStruct*>::resize(_piDims, _iDims);
    if (bRes)
    {
        // insert field(s) only in new element(s) of current struct
        String* pFields = getFieldNames();
        for (int iterField = 0; iterField < pFields->getSize(); iterField++)
        {
            for (int iterStruct = 0; iterStruct < getSize(); iterStruct++)
            {
                get(iterStruct)->addField(pFields->get(iterField));
            }
        }
    }

    return bRes;
}

InternalType* Struct::insertWithoutClone(typed_list* _pArgs, InternalType* _pSource)
{
    //std::wcout << L"insertWithoutClone start" << std::endl;
    m_bDisableCloneInCopyValue = true;
    InternalType* pIT = insert(_pArgs, _pSource);
    _pSource->IncreaseRef();
    //std::wcout << L"insertWithoutClone -> " << _pSource << L" : " << _pSource->getRef() << std::endl;
    m_bDisableCloneInCopyValue = false;
    //std::wcout << L"insertWithoutClone end" << std::endl;
    return pIT;
}

InternalType* Struct::extractWithoutClone(typed_list* _pArgs)
{
    //std::wcout << L"extractWithoutClone start" << std::endl;
    m_bDisableCloneInCopyValue = true;
    InternalType* pIT = extract(_pArgs);
    m_bDisableCloneInCopyValue = false;
    //std::wcout << L"extractWithoutClone end" << std::endl;
    return pIT;
}

void Struct::setCloneInCopyValue(bool _val)
{
    m_bDisableCloneInCopyValue = !_val;
}

}
