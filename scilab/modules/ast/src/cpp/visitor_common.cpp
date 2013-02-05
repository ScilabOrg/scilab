/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <string>
#include <numeric>
#include "visitor_common.hxx"
#include "exp.hxx"
#include "fieldexp.hxx"
#include "simplevar.hxx"
#include "callexp.hxx"
#include "struct.hxx"
#include "context.hxx"
#include "execvisitor.hxx"

#include "alltypes.hxx"

using namespace types;

static bool isIntTrue(InternalType* _pIT);

bool bConditionState(types::InternalType *_pITResult)
{
    if (_pITResult->isDouble() &&
            _pITResult->getAs<types::Double>()->isComplex() == false)
    {
        types::Double *pR = _pITResult->getAs<types::Double>();
        if (pR->isEmpty())
        {
            //[]
            return false;
        }

        double *pReal = pR->getReal();
        for (int i = 0 ; i < pR->getSize() ; i++)
        {
            if (pReal[i] == 0)
            {
                return false;
            }
        }
    }
    else if (_pITResult->isBool())
    {
        types::Bool *pB		= _pITResult->getAs<types::Bool>();
        int *piData	= pB->get();

        for (int i = 0 ; i < pB->getSize() ; i++)
        {
            if (piData[i] == 0)
            {
                return false;
                break;
            }
        }
    }
    else if (_pITResult->isInt())
    {
        return isIntTrue(_pITResult);
    }
    else
    {
        return false;
    }
    return true;
}

template <class K>
static bool isIntTrue(K* _pI)
{
    for (int i = 0 ; i < _pI->getSize() ; i++)
    {
        if (_pI->get(i) == 0)
        {
            return false;
        }
    }

    return true;
}

static bool isIntTrue(InternalType* _pIT)
{
    switch (_pIT->getType())
    {
        case InternalType::RealInt8 :
        {
            return isIntTrue(_pIT->getAs<Int8>());
        }
        case InternalType::RealUInt8 :
        {
            return isIntTrue(_pIT->getAs<UInt8>());
        }
        case InternalType::RealInt16 :
        {
            return isIntTrue(_pIT->getAs<Int16>());
        }
        case InternalType::RealUInt16 :
        {
            return isIntTrue(_pIT->getAs<UInt16>());
        }
        case InternalType::RealInt32 :
        {
            return isIntTrue(_pIT->getAs<Int32>());
        }
        case InternalType::RealUInt32 :
        {
            return isIntTrue(_pIT->getAs<UInt32>());
        }
        case InternalType::RealInt64 :
        {
            return isIntTrue(_pIT->getAs<Int64>());
        }
        case InternalType::RealUInt64 :
        {
            return isIntTrue(_pIT->getAs<UInt64>());
        }
        default:
            break;
    }

    return false;
}

/*
 * Generate destination variable from _poSource type and size parameters
 */
types::InternalType* allocDest(types::InternalType* _poSource, int _iRows, int _iCols)
{
    types::InternalType* poResult = NULL;
    switch (_poSource->getType())
    {
        case types::GenericType::RealDouble :
            poResult = new types::Double(_iRows, _iCols, false);
            break;
        case types::GenericType::RealBool :
            poResult = new types::Bool(_iRows, _iCols);
            break;
        case types::GenericType::RealInt8 :
            poResult = new types::Int8(_iRows, _iCols);
            break;
        case types::GenericType::RealUInt8 :
            poResult = new types::UInt8(_iRows, _iCols);
            break;
        case types::GenericType::RealInt16 :
            poResult = new types::Int16(_iRows, _iCols);
            break;
        case types::GenericType::RealUInt16 :
            poResult = new types::UInt16(_iRows, _iCols);
            break;
        case types::GenericType::RealInt32 :
            poResult = new types::Int32(_iRows, _iCols);
            break;
        case types::GenericType::RealUInt32 :
            poResult = new types::UInt32(_iRows, _iCols);
            break;
        case types::GenericType::RealInt64 :
            poResult = new types::Int64(_iRows, _iCols);
            break;
        case types::GenericType::RealUInt64 :
            poResult = new types::UInt64(_iRows, _iCols);
            break;
        case types::GenericType::RealString :
            poResult = new types::String(_iRows, _iCols);
            break;
        case types::GenericType::RealPoly :
        {
            int* piRank = new int[_iRows * _iCols];
            for (int i = 0 ; i < _iRows * _iCols ; i++)
            {
                piRank[i] = 1;
            }
            poResult = new types::Polynom(_poSource->getAs<types::Polynom>()->getVariableName(), _iRows, _iCols, piRank);
            break;
        }
        case types::InternalType::RealImplicitList :
            poResult = new types::ImplicitList();
            break;
        default :
            // FIXME : What should we do here ??
            break;
    }
    return poResult;
}

types::InternalType* AddElementToVariableFromCol(types::InternalType* _poDest, types::InternalType* _poSource, int _iRows, int _iCols, int *_piCols)
{
    types::InternalType *poResult	            = NULL;
    types::InternalType::RealType TypeSource	= _poSource->getType();
    types::InternalType::RealType TypeDest		= types::InternalType::RealInternal;
    int iCurRow                                 = _iRows;
    int iCurCol                                 = _iCols;


    if (_poDest == NULL)
    {
        //First call, alloc _poSource
        poResult    = allocDest(_poSource, _iRows, _iCols);
        TypeDest	= TypeSource;
        iCurCol	    = 0;
        iCurRow		= 0;
    }
    else
    {
        TypeDest    = _poDest->getType();
        poResult    = _poDest;
    }

    if (TypeDest != TypeSource)
    {
        //check if source type is compatible with dest type
    }
    else
    {
        switch (TypeDest)
        {
            case types::GenericType::RealDouble :
                if (poResult->getAs<types::Double>()->isComplex() == false && _poSource->getAs<types::Double>()->isComplex() == true)
                {
                    poResult->getAs<types::Double>()->setComplex(true);
                }

                poResult->getAs<types::Double>()->fillFromCol(*_piCols, _poSource->getAs<types::Double>());
                *_piCols += _poSource->getAs<types::Double>()->getCols();

                break;
            default:
                break;
        }
        return poResult;
    }
    return NULL;
}

types::InternalType* AddElementToVariableFromRow(types::InternalType* _poDest, types::InternalType* _poSource, int _iRows, int _iCols, int *_piRows)
{
    types::InternalType *poResult	            = NULL;
    types::InternalType::RealType TypeSource	= _poSource->getType();
    types::InternalType::RealType TypeDest		= types::InternalType::RealInternal;
    int iCurRow                                 = _iRows;
    int iCurCol                                 = _iCols;

    if (_poDest == NULL)
    {
        //First call, alloc _poSource
        poResult    = allocDest(_poSource, _iRows, _iCols);
        iCurCol	    = 0;
        iCurRow		= 0;
        TypeDest	= TypeSource;
    }
    else
    {
        TypeDest	= _poDest->getType();
        poResult    = _poDest;
    }


    if (TypeDest != TypeSource)
    {
        //check if source type is compatible with dest type
    }
    else
    {
        switch (TypeDest)
        {
            case types::GenericType::RealDouble :
                if (poResult->getAs<types::Double>()->isComplex() == false && _poSource->getAs<types::Double>()->isComplex() == true)
                {
                    poResult->getAs<types::Double>()->setComplex(true);
                }

                poResult->getAs<types::Double>()->fillFromRow(*_piRows, _poSource->getAs<types::Double>());
                *_piRows += _poSource->getAs<types::Double>()->getRows();

                break;
            default:
                break;
        }
        return poResult;
    }
    return NULL;
}


/*
_iRows : Position if _poDest allready initialized else size of the matrix
_iCols : Position if _poDest allready initialized else size of the matrix
*/
types::InternalType* AddElementToVariable(types::InternalType* _poDest, types::InternalType* _poSource, int _iRows, int _iCols)
{
    types::InternalType *poResult	= NULL;
    types::InternalType::RealType TypeSource	= _poSource->getType();
    types::InternalType::RealType TypeDest		=	types::InternalType::RealInternal;
    int iCurRow = _iRows;
    int iCurCol = _iCols;

    if (_poDest == NULL)
    {
        switch (TypeSource)
        {
            case types::GenericType::RealDouble :
                poResult = new types::Double(_iRows, _iCols, false);
                break;
            case types::GenericType::RealBool :
                poResult = new types::Bool(_iRows, _iCols);
                break;
            case types::GenericType::RealInt8 :
                poResult = new types::Int8(_iRows, _iCols);
                break;
            case types::GenericType::RealUInt8 :
                poResult = new types::UInt8(_iRows, _iCols);
                break;
            case types::GenericType::RealInt16 :
                poResult = new types::Int16(_iRows, _iCols);
                break;
            case types::GenericType::RealUInt16 :
                poResult = new types::UInt16(_iRows, _iCols);
                break;
            case types::GenericType::RealInt32 :
                poResult = new types::Int32(_iRows, _iCols);
                break;
            case types::GenericType::RealUInt32 :
                poResult = new types::UInt32(_iRows, _iCols);
                break;
            case types::GenericType::RealInt64 :
                poResult = new types::Int64(_iRows, _iCols);
                break;
            case types::GenericType::RealUInt64 :
                poResult = new types::UInt64(_iRows, _iCols);
                break;
            case types::GenericType::RealString :
                poResult = new types::String(_iRows, _iCols);
                break;
            case types::GenericType::RealSparse :
                poResult = new types::Sparse(_iRows, _iCols);
                break;
            case types::GenericType::RealSparseBool :
                poResult = new types::SparseBool(_iRows, _iCols);
                break;
            case types::GenericType::RealPoly :
            {
                int* piRank = new int[_iRows * _iCols];
                for (int i = 0 ; i < _iRows * _iCols ; i++)
                {
                    piRank[i] = 1;
                }
                poResult = new types::Polynom(_poSource->getAs<types::Polynom>()->getVariableName(), _iRows, _iCols, piRank);
                break;
            }
            case types::InternalType::RealImplicitList :
                poResult = new types::ImplicitList();
                break;
            case types::GenericType::RealHandle :
                poResult = new types::GraphicHandle(_iRows, _iCols);
                break;
            default :
                // FIXME What should we do here ...
                break;
        }
        iCurCol = 0;
        iCurRow = 0;
        TypeDest =	TypeSource;
    }
    else
    {
        TypeDest = _poDest->getType();
        poResult = _poDest;
    }


    if (TypeDest != TypeSource)
    {
        //check if source type is compatible with dest type
        switch (TypeDest)
        {
            case types::GenericType::RealDouble :
                if (TypeSource == types::GenericType::RealPoly)
                {
                    types::Double *poDest = _poDest->getAs<types::Double>();
                    //Convert Dest to RealPoly
                    int *piRank = new int[poDest->getSize()];
                    for (int i = 0 ; i < poDest->getSize() ; i++)
                    {
                        piRank[i] = 1;
                    }

                    poResult = new types::Polynom(_poSource->getAs<types::Polynom>()->getVariableName(), poDest->getRows(), poDest->getCols(),  piRank);

                    double *pR = poDest->getReal();
                    double *pI = poDest->getImg();
                    for (int i = 0 ; i < poDest->getSize() ; i++)
                    {
                        types::Double *pdbl = NULL;
                        if (poDest->isComplex())
                        {
                            pdbl = new types::Double(pR[i], pI[i]);
                        }
                        else
                        {
                            pdbl = new types::Double(pR[i]);
                        }

                        poResult->getAs<types::Polynom>()->setCoef(i, pdbl);
                        delete pdbl;
                    }

                    Polynom* pP = _poSource->getAs<types::Polynom>();

                    for (int i = 0 ; i < pP->getRows() ; i++)
                    {
                        for (int j = 0 ; j < pP->getCols() ; j++)
                        {
                            poResult->getAs<types::Polynom>()->setCoef(iCurRow + i, iCurCol + j, _poSource->getAs<types::Polynom>()->get(i, j)->getCoef());
                        }
                    }
                }
                break;
            case types::GenericType::RealPoly :
                if (TypeSource == types::GenericType::RealDouble)
                {
                    //Add Source like coef of the new element
                    Double* pD = _poSource->getAs<Double>();
                    types::Polynom* pPolyOut = poResult->getAs<types::Polynom>();

                    if (pD->isComplex())
                    {
                        pPolyOut->setComplex(true);
                        for (int i = 0 ; i < pD->getRows() ; i++)
                        {
                            for (int j = 0 ; j < pD->getCols() ; j++)
                            {
                                types::SinglePoly* pSPOut = pPolyOut->get(iCurRow + i, iCurCol + j);

                                pSPOut->setRank(1);
                                double pDblR = pD->get(i, j);
                                double pDblI = pD->getImg(i, j);
                                pSPOut->setCoef(&pDblR, &pDblI);
                            }
                        }
                    }
                    else
                    {
                        for (int i = 0 ; i < pD->getRows() ; i++)
                        {
                            for (int j = 0 ; j < pD->getCols() ; j++)
                            {
                                types::SinglePoly* pSPOut = pPolyOut->get(iCurRow + i, iCurCol + j);

                                pSPOut->setRank(1);
                                double pDbl = pD->get(i, j);
                                pSPOut->setCoef(&pDbl, NULL);
                            }
                        }
                    }
                }
                break;
            case types::GenericType::RealSparse :
                if (TypeSource == types::GenericType::RealDouble)
                {
                    types::Double* poSource = _poSource->getAs<types::Double>();
                    types::Sparse* spResult = poResult->getAs<types::Sparse>();

                    // Set complex the result if one of inputs is complex
                    if (poSource->isComplex())
                    {
                        if (spResult->isComplex() == false)
                        {
                            spResult->toComplex();
                        }
                    }

                    // Add poSource at the end of spResult
                    if (spResult->isComplex())
                    {
                        if (poSource->isComplex())
                        {
                            for (int i = 0; i < poSource->getRows(); i++)
                            {
                                for (int j = 0; j < poSource->getCols(); j++)
                                {
                                    double dbl = poSource->get(i, j);
                                    double dblImg = poSource->getImg(i, j);
                                    if (dbl != 0 || dblImg != 0)
                                    {
                                        spResult->set(i + iCurRow, j + iCurCol, std::complex<double>(dbl, dblImg));
                                    }
                                }
                            }
                        }
                        else
                        {
                            for (int i = 0; i < poSource->getRows(); i++)
                            {
                                for (int j = 0; j < poSource->getCols(); j++)
                                {
                                    double dbl = poSource->get(i, j);
                                    if (dbl != 0)
                                    {
                                        spResult->set(i + iCurRow, j + iCurCol, std::complex<double>(dbl, 0));
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        for (int i = 0; i < poSource->getRows(); i++)
                        {
                            for (int j = 0; j < poSource->getCols(); j++)
                            {
                                double dbl = poSource->get(i, j);
                                if (dbl != 0)
                                {
                                    spResult->set(i + iCurRow, j + iCurCol, dbl);
                                }
                            }
                        }
                    }
                }
                break;
            case types::GenericType::RealSparseBool :
                if (TypeSource == types::GenericType::RealBool)
                {
                    types::Bool* poSource = _poSource->getAs<types::Bool>();
                    types::SparseBool* spResult = poResult->getAs<types::SparseBool>();

                    // Add poSource at the end of spResult
                    for (int i = 0; i < poSource->getRows(); i++)
                    {
                        for (int j = 0; j < poSource->getCols(); j++)
                        {
                            bool bValue = poSource->get(i, j) != 0;
                            if (bValue)
                            {
                                spResult->set(i + iCurRow, j + iCurCol, true);
                            }
                        }
                    }
                }
                break;
            default:
                break;
        }
        return poResult;
    }
    else
    {
        //Just add the new value in the current item
        switch (TypeDest)
        {
            case types::GenericType::RealDouble :
                ((Double*)poResult)->append(iCurRow, iCurCol, (Double*)_poSource);
                break;
            case types::GenericType::RealPoly :
                poResult->getAs<types::Polynom>()->append(iCurRow, iCurCol, _poSource->getAs<types::Polynom>());
                break;
            case types::GenericType::RealBool:
                poResult->getAs<types::Bool>()->append(iCurRow, iCurCol, _poSource->getAs<types::Bool>());
                break;
            case types::GenericType::RealInt8 :
                poResult->getAs<types::Int8>()->append(iCurRow, iCurCol, _poSource->getAs<types::Int8>());
                break;
            case types::GenericType::RealUInt8 :
                poResult->getAs<types::UInt8>()->append(iCurRow, iCurCol, _poSource->getAs<types::UInt8>());
                break;
            case types::GenericType::RealInt16 :
                poResult->getAs<types::Int16>()->append(iCurRow, iCurCol, _poSource->getAs<types::Int16>());
                break;
            case types::GenericType::RealUInt16 :
                poResult->getAs<types::UInt16>()->append(iCurRow, iCurCol, _poSource->getAs<types::UInt16>());
                break;
            case types::GenericType::RealInt32 :
                poResult->getAs<types::Int32>()->append(iCurRow, iCurCol, _poSource->getAs<types::Int32>());
                break;
            case types::GenericType::RealUInt32 :
                poResult->getAs<types::UInt32>()->append(iCurRow, iCurCol, _poSource->getAs<types::UInt32>());
                break;
            case types::GenericType::RealInt64 :
                poResult->getAs<types::Int64>()->append(iCurRow, iCurCol, _poSource->getAs<types::Int64>());
                break;
            case types::GenericType::RealUInt64 :
                poResult->getAs<types::UInt64>()->append(iCurRow, iCurCol, _poSource->getAs<types::UInt64>());
                break;
            case types::GenericType::RealSparse :
                poResult->getAs<types::Sparse>()->append(iCurRow, iCurCol, _poSource->getAs<types::Sparse>());
                break;
            case types::GenericType::RealSparseBool :
                poResult->getAs<types::SparseBool>()->append(iCurRow, iCurCol, _poSource->getAs<types::SparseBool>());
                break;
            case types::GenericType::RealString :
            {
                types::String* pSource = _poSource->getAs<types::String>();
                poResult->getAs<types::String>()->append(iCurRow, iCurCol, pSource);
            }
            break;
            case types::GenericType::RealImplicitList :
            {
                if (_poSource->getAs<ImplicitList>()->getStartType() == types::InternalType::RealPoly)
                {
                    poResult->getAs<ImplicitList>()->setStart(_poSource->getAs<ImplicitList>()->getStart());
                }
                else
                {
                    poResult->getAs<ImplicitList>()->setStart(_poSource->getAs<ImplicitList>()->getStart());
                }

                if (_poSource->getAs<ImplicitList>()->getStepType() == types::InternalType::RealPoly)
                {
                    poResult->getAs<ImplicitList>()->setStep(_poSource->getAs<ImplicitList>()->getStep());
                }
                else
                {
                    poResult->getAs<ImplicitList>()->setStep(_poSource->getAs<ImplicitList>()->getStep());
                }

                if (_poSource->getAs<ImplicitList>()->getEndType() == types::InternalType::RealPoly)
                {
                    poResult->getAs<ImplicitList>()->setEnd(_poSource->getAs<ImplicitList>()->getEnd());
                }
                else
                {
                    poResult->getAs<ImplicitList>()->setEnd(_poSource->getAs<ImplicitList>()->getEnd());
                }
                break;
            }
            case types::GenericType::RealHandle :
                poResult->getAs<types::GraphicHandle>()->append(iCurRow, iCurCol, _poSource->getAs<types::GraphicHandle>());
                break;
            default:
                break;
        }
        return poResult;
    }
    return NULL;
}

const std::wstring* getStructNameFromExp(const Exp* _pExp)
{
    const FieldExp* pField =  dynamic_cast<const FieldExp*>(_pExp);
    const SimpleVar* pVar =  dynamic_cast<const SimpleVar*>(_pExp);
    const CallExp* pCall =  dynamic_cast<const CallExp*>(_pExp);

    if (pField)
    {
        return getStructNameFromExp(pField->head_get());
    }
    else if (pVar)
    {
        return &(pVar->name_get().name_get());
    }
    else if (pCall)
    {
        return getStructNameFromExp(&(pCall->name_get()));
    }
    else
    {
        std::wostringstream os;
        os << _W("Unknow expression");
        //os << ((Location)e.right_exp_get().location_get()).location_getString() << std::endl;
        throw ScilabError(os.str(), 999, _pExp->location_get());
    }
    return NULL;
}

static bool callStructOverload(std::wstring& _wstField, typed_list* _pArgs, InternalType* _pValue, InternalType** _pITStruct, InternalType** _pITRet, bool _bComplexForm)
{
    if ((*_pITStruct)->isTList() == false && (*_pITStruct)->isMList() == false && (*_pITStruct)->isStruct() == false && (*_pITStruct)->isHandle() == false)
    {
        //parent is not a complex type, so create it as struct
        *_pITStruct = new Struct(1, 1);
    }

    if (_pValue)
    {
        if ((*_pITStruct)->isTList() || (*_pITStruct)->isMList())
        {
            TList* pTL = (*_pITStruct)->getAs<TList>();
            //insertion
            if (pTL->exists(_wstField))
            {
                //standard insertion
                return pTL->set(_wstField, _pValue);
            }
            else
            {
                //overloaded insertion via %assigntype_i_listtype
                String* pFieldStr = new String(_wstField.c_str());

                typed_list in;
                typed_list out;
                optional_list opt;
                ExecVisitor exec;

                //assign value to field by callin overling insertion function %assigntype_i_listtype
                in.push_back(pFieldStr);
                in.push_back(_pValue);
                in.push_back(pTL);


                //protect data during overload call
                for (int i = 0 ; i < in.size() ; i++)
                {
                    in[i]->IncreaseRef();
                }

                std::wstring stOverload = L"%" + _pValue->getShortTypeStr() +  L"_i_" + pTL->getShortTypeStr();
                Callable::ReturnValue ret = Overload::call(stOverload, in, 1, out, &exec);

                //unprotect inputs
                for (int i = 0 ; i < in.size() ; i++)
                {
                    in[i]->DecreaseRef();
                }

                return ret == Function::OK;
            }
        }
        else if ((*_pITStruct)->isStruct())
        {
            Struct* pS = (*_pITStruct)->getAs<Struct>();
            if (_wstField != L"")
            {
                if (pS->exists(_wstField) == false)
                {
                    //add field
                    if (pS->addField(_wstField) == false)
                    {
                        return false;
                    }
                }

                if (_pArgs != NULL && _pArgs->size() != 0)
                {
                    //extract in new structure
                    Struct *pStr = pS->extractWithoutClone(_pArgs)->getAs<Struct>();
                    pStr->setCloneInCopyValue(false);
                    for (int i = 0 ; i < pStr->getSize() ; i++)
                    {
                        //get sub structure
                        SingleStruct* pSS = pStr->get(i);
                        //set field value
                        pSS->set(_wstField, _pValue);
                    }

                    delete pStr;
                }
                else if (pS->isScalar())
                {
                    pS->get(0)->set(_wstField, _pValue);
                }
                else
                {
                    return false;
                }
            }
            else
            {
                //standard insert
                if (pS->insert(_pArgs, _pValue) == NULL)
                {
                    return false;
                }
            }

            return true;
        }
        else if ((*_pITStruct)->isHandle())
        {
            GraphicHandle* pH = (*_pITStruct)->getAs<GraphicHandle>();
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if ((*_pITStruct)->isTList() || (*_pITStruct)->isMList())
        {
            if (_pArgs != NULL && _pArgs->size() != 0)
            {
                //a(x, y, ...).b
                //must extract a(x, y, ...) before extract b
            }

            TList* pTL = (*_pITStruct)->getAs<TList>();
            //extraction
            if (pTL->exists(_wstField))
            {
                *_pITRet = pTL->getField(_wstField);
                return true;
            }
            else
            {
                types::typed_list in;
                types::optional_list opt;
                types::typed_list out;
                ExecVisitor exec;

                //firt argument: index
                String* pFieldStr = new String(_wstField.c_str());
                in.push_back(pFieldStr);

                //last argument: me ( mlist or tlist )
                in.push_back(pTL);

                //protect data during overload call
                for (int i = 0 ; i < in.size() ; i++)
                {
                    in[i]->IncreaseRef();
                }

                std::wstring wstOverload;
                if (_bComplexForm)
                {
                    wstOverload = L"%" + pTL->getShortTypeStr() + L"_6";
                }
                else
                {
                    wstOverload = L"%" + pTL->getShortTypeStr() + L"_e";
                }
                Callable::ReturnValue Ret = Overload::call(wstOverload, in, 1, out, &exec);

                //unprotect inputs
                for (int i = 0 ; i < in.size() ; i++)
                {
                    in[i]->DecreaseRef();
                }

                delete pFieldStr;
                out[0]->DecreaseRef();

                if (out.size() == 0)
                {
                    return false;
                }

                *_pITRet = out[0];
                return true;
            }
        }
        else if ((*_pITStruct)->isStruct())
        {
            Struct* pS = (*_pITStruct)->getAs<Struct>();

            if (pS->exists(_wstField) == false)
            {
                if (pS->addField(_wstField) == false)
                {
                    return false;
                }
            }

            if (_pArgs != NULL && _pArgs->size() != 0)
            {
                //extract in new structure
                Struct *pStr = pS->extractWithoutClone(_pArgs)->getAs<Struct>();
                pStr->setCloneInCopyValue(false);
                //get sub structure
                SingleStruct* pSS = pStr->get(0);
                //set field value
                *_pITRet = pSS->get(_wstField);
                //release new structure and protect extract data
                (*_pITRet)->IncreaseRef();
                delete pStr;
                (*_pITRet)->DecreaseRef();
            }
            else if (pS->isScalar())
            {
                *_pITRet = pS->get(0)->get(_wstField);
            }
            else //do not allow a.b if a is not scalar
            {
                return false;
            }

            return true;
        }
        else if ((*_pITStruct)->isHandle())
        {
            GraphicHandle* pH = (*_pITStruct)->getAs<GraphicHandle>();
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool updateStructFromHistory(std::list<types::InternalType*>& structHistory, std::list<std::wstring>& fieldHistory, std::list<types::typed_list*>& argHistory)
{
    if (structHistory.size() < 2)
    {
        return true;
    }

    //the goal is to assign "pos" to "pos+1"
    std::list<types::InternalType*>::iterator itWhat = structHistory.begin();
    std::list<types::InternalType*>::iterator itWhere = structHistory.begin();
    std::list<std::wstring>::iterator itField = fieldHistory.begin();
    std::list<types::typed_list*>::iterator itArg = argHistory.begin();
    for (itWhere++; itWhere != structHistory.end(); itWhat++, itWhere++, itField++, itArg++)
    {
        callStructOverload(*itField, *itArg, *itWhat, &(*itWhere), NULL, false);
    }

    return true;
}


bool getStructFromExp(const ast::Exp* _pExp, std::list<types::InternalType*>& structHistory, std::list<std::wstring>& fieldHistory, std::list<types::typed_list*>& argHistory, types::InternalType* _pAssignValue, bool* _pbComplexForm)
{
    const FieldExp* pField      = dynamic_cast<const FieldExp*>(_pExp);
    const SimpleVar* pVar       = dynamic_cast<const SimpleVar*>(_pExp);
    const CallExp* pCall        = dynamic_cast<const CallExp*>(_pExp);
    const CellCallExp* pCell    = dynamic_cast<const CellCallExp*>(_pExp);

    if (pField)
    {
        //y.x
        //evaluate head "y"
        if (getStructFromExp(pField->head_get(), structHistory, fieldHistory, argHistory, NULL, _pbComplexForm))
        {
            pVar    = dynamic_cast<const SimpleVar*>(pField->tail_get());

            InternalType* pHead =  NULL;
            typed_list *pArgs = NULL;

            std::list<types::InternalType*>::iterator str = structHistory.begin();
            std::list<std::wstring>::iterator field = fieldHistory.begin();
            std::list<types::typed_list*>::iterator arg = argHistory.begin();

            while (field != fieldHistory.end() && *field == L"")
            {
                //do not take care of empty field, tips for callexp form
                pArgs = *arg; //last args
                field++;
                str++;
                arg++;
            }
            pHead = *str; //last item

            //if (pHead->isStruct())
            {
                //Struct* pCurStr = pCurrent->getAs<Struct>();
                ////clone _pIT BEFORE addField in case of st.b = st
                //types::InternalType* pIT = _pIT ? _pIT->clone() : NULL;

                ////create field "x"
                //std::wstring var = pVar->name_get().name_get();
                //bool bOK = pCurStr->addField(pVar->name_get().name_get());
                //if (*_pMain == NULL && _pIT != NULL)
                //{
                //    //first stack, assign value to field and return main structure

                //    if (pArgs != NULL && pArgs->size() != 0)
                //    {
                //        //args returned by "parent"
                //        std::wstring var = pVar->name_get().name_get();
                //        //be careful, extract functions copy values

                //        Struct *pStr = pCurStr->extractWithoutClone(pArgs)->getAs<Struct>();
                //        pStr->setCloneInCopyValue(false);
                //        SingleStruct* pSS = pStr->get(0);
                //        pSS->set(pVar->name_get().name_get(), pIT);
                //        pSS->IncreaseRef();
                //        delete pStr;
                //        pSS->DecreaseRef();
                //    }
                //    else if (_pArgs == NULL || *_pArgs == NULL)
                //    {
                //        std::wstring var = pVar->name_get().name_get();
                //        //std::wcout << var << L" <- " << pIT->getTypeStr() << std::endl;
                //        pCurStr->get(0)->set(pVar->name_get().name_get(), pIT);
                //    }
                //    else
                //    {
                //        Struct* pStr = new Struct(1, 1);
                //        std::wstring var = pVar->name_get().name_get();
                //        pStr->addField(pVar->name_get().name_get());
                //        pStr->get(0)->set(pVar->name_get().name_get(), pIT);
                //        pCurStr->insertWithoutClone(*_pArgs, pStr->get(0));
                //        delete pStr;
                //    }
                //}
                //else
                //{
                //    //y.x.w
                //    //in this case, we are in the middle of expression
                //    //we know that "x" is a struct but we can't assign value yet
                //    //so assign empty struct and return new pCurrent
                //    Struct* pStr = NULL;

                //    /*try to extract field*/
                //    if (pArgs == NULL)
                //    {
                //        //without extract argument
                //        pStr = pCurStr->get(0)->get(pVar->name_get().name_get())->getAs<Struct>();
                //    }
                //    else
                //    {
                //        Struct* pStepStr = pCurStr->extractWithoutClone(pArgs)->getAs<Struct>();
                //        pStepStr->setCloneInCopyValue(false);
                //        SingleStruct* pSS = pStepStr->get(0);
                //        pStr = pSS->get(pVar->name_get().name_get())->getAs<Struct>();
                //        //we can delete pStepStr without deleted its fields
                //        pSS->IncreaseRef();
                //        delete pStepStr;
                //        pSS->DecreaseRef();
                //    }

                //    if (pStr == NULL)
                //    {
                //        //new field or not struct field
                //        if (_pArgs == NULL || *_pArgs == NULL)
                //        {
                //            pStr = new Struct(1, 1);
                //        }
                //        else
                //        {
                //            Struct* p = new Struct(1, 1);
                //            pStr = Struct::insertNew(*_pArgs, p)->getAs<Struct>();
                //            delete p;
                //        }

                //        if (pArgs != NULL)
                //        {
                //            std::wstring var = pVar->name_get().name_get();

                //            Struct* pStepStr = pCurStr->extractWithoutClone(pArgs)->getAs<Struct>();
                //            pStepStr->setCloneInCopyValue(false);
                //            SingleStruct* pSS = pStepStr->get(0);
                //            pSS->set(pVar->name_get().name_get(), pStr);
                //            pSS->IncreaseRef();
                //            delete pStepStr;
                //            pSS->DecreaseRef();
                //        }
                //        else
                //        {
                //            std::wstring var = pVar->name_get().name_get();
                //            pCurStr->get(0)->set(pVar->name_get().name_get(), pStr);
                //        }
                //    }

                //    pCurrent = pStr;
                //}

                //_pMain.push_back(pMain);
                //*_pCurrent = pCurrent;
            }
            //else if (pHead->isHandle())
            {
                //String* pTail = new String(pVar->name_get().name_get().c_str());
                //if (_pArgs != NULL && *_pArgs == NULL)
                //{
                //    *_pArgs = new typed_list;
                //    pArgs = *_pArgs;
                //}
                //else if (pArgs == NULL)
                //{
                //    pArgs = new typed_list;
                //}

                //if (pArgs)
                //{
                //    pArgs->push_back(pTail);
                //    if (_pIT == NULL)
                //    {
                //        //let caller work

                //        //try to extract, if extract work, clear args and flag we need set operation after.
                //        GraphicHandle* pCurH = pCurrent->getAs<GraphicHandle>();
                //        typed_list in;
                //        typed_list out;
                //        optional_list opt;
                //        ExecVisitor exec;

                //        if (pArgs->size() == 1)
                //        {
                //            in.push_back((*pArgs)[0]);
                //        }
                //        else
                //        {
                //            List* pList = new List();
                //            for (int i = 0 ; i < pArgs->size() ; i++)
                //            {
                //                pList->append((*pArgs)[i]);
                //            }
                //            in.push_back(pList);
                //        }

                //        in.push_back(pMain);
                //        in.front()->IncreaseRef();
                //        pMain->IncreaseRef();

                //        Function* pCall = (Function*)symbol::Context::getInstance()->get(symbol::Symbol(L"%h_e"));
                //        Callable::ReturnValue ret =  pCall->call(in, opt, 1, out, &exec);

                //        in.front()->DecreaseRef();
                //        pMain->DecreaseRef();

                //        if (in.front()->isList())
                //        {
                //            //delete pList
                //            delete in.front();
                //        }

                //        if (ret != Callable::OK)
                //        {
                //            std::wostringstream os;
                //            os << L"unable to update handle";
                //            throw ScilabError(os.str(), 999, pField->location_get());
                //        }

                //        if (out[0]->isHandle() || out[0]->isStruct())
                //        {
                //            *_pCurrent = out[0];
                //            (*_pCurrent)->IncreaseRef();

                //            //clean *_pArgs to do not extract previons fields
                //            if (_pArgs && *_pArgs)
                //            {
                //                (*_pArgs)->clear();
                //            }

                //        }
                //        else
                //        {
                //            *_pCurrent = pCurrent;
                //        }

                //        *_pMain = pMain;
                //        return true;
                //    }

                //    //call %x_i_h
                //    GraphicHandle* pCurH = pCurrent->getAs<GraphicHandle>();
                //    typed_list in;
                //    typed_list out;
                //    optional_list opt;
                //    ExecVisitor exec;


                //    if (pArgs->size() == 1)
                //    {
                //        in.push_back((*pArgs)[0]);
                //    }
                //    else
                //    {
                //        List* pList = new List();
                //        for (int i = 0 ; i < pArgs->size() ; i++)
                //        {
                //            pList->append((*pArgs)[i]);
                //        }
                //        in.push_back(pList);
                //    }

                //    std::wstring str = L"%" + _pIT->getShortTypeStr() + L"_i_h";

                //    in.push_back(_pIT);
                //    in.push_back(pMain);
                //    in.front()->IncreaseRef();
                //    _pIT->IncreaseRef();
                //    pMain->IncreaseRef();

                //    Function* pCall = (Function*)symbol::Context::getInstance()->get(symbol::Symbol(str));
                //    Callable::ReturnValue ret =  pCall->call(in, opt, 1, out, &exec);
                //    in.front()->DecreaseRef();
                //    //_pIT->DecreaseRef();
                //    pMain->DecreaseRef();

                //    if (in.front()->isList())
                //    {
                //        //delete pList
                //        delete in.front();
                //    }

                //    if (ret != Callable::OK)
                //    {
                //        std::wostringstream os;
                //        os << L"unable to update handle";
                //        throw ScilabError(os.str(), 999, pField->location_get());
                //    }
                //}
                //else
                //{
                //    std::wostringstream os;
                //    os << L"impossible !";
                //    throw ScilabError(os.str(), 999, pField->location_get());
                //}
            }
            //else if (pHead->isTList() || pHead->isMList())
            {
                //a.b
                InternalType* pITRet = NULL;
                wstring wstField = pVar->name_get().name_get();
                if (_pAssignValue == NULL)
                {
                    *_pbComplexForm = true;
                }

                InternalType* pSaveHead = pHead;
                if (callStructOverload(wstField, pArgs, _pAssignValue, &pHead, &pITRet, *_pbComplexForm) == false)
                {
                    std::wostringstream os;
                    if (pHead->isMList())
                    {
                        os << L"unable to update mlist" << std::endl;
                    }
                    else
                    {
                        os << L"unable to update tlist" << std::endl;
                    }
                    throw ScilabError(os.str(), 999, pField->location_get());
                }
                else
                {
                    if (pHead != pSaveHead)
                    {
                        //creation of field
                        structHistory.pop_front();
                        structHistory.push_front(pHead);
                    }

                    if (_pAssignValue == NULL)
                    {
                        structHistory.push_front(pITRet);
                        fieldHistory.push_front(wstField);
                        argHistory.push_front(NULL);
                    }
                }
            }
            //else
            //{
            //    std::wostringstream os;
            //    os << L"Not Yet Managed in Scilab !";
            //    throw ScilabError(os.str(), 999, pField->location_get());
            //}


            //clean pArgs return by getStructFromExp
            //for (int iArg = 0 ; _pArg != NULL && iArg < _pArg->size() ; iArg++)
            //{
            //    if ((*pArgs)[iArg]->isDeletable())
            //    {
            //        delete (*pArgs)[iArg];
            //    }
            //}
            //delete pArgs;
            return true;
        }
        else
        {
            return false;
        }
    }
    else if (pVar)
    {
        //a.x : with x not only a SimpleVar

        types::InternalType *pStr = NULL;
        types::InternalType *pIT = symbol::Context::getInstance()->get(pVar->name_get());
        if (pIT == NULL ||
                (   pIT->isStruct() == false &&
                    pIT->isHandle() == false &&
                    pIT->isMList() == false &&
                    pIT->isTList() == false))
        {
            //"a" doest not exist or it is another type, create it with size 1,1 and return it
            //create new structure variable
            types::typed_list* pArgs = argHistory.size() != 0 ? argHistory.front() : NULL;
            if (pArgs && (*pArgs)[0]->isString() == false)
            {
                //create array of empty struct
                Struct* pSubStruct = new Struct(1, 1);
                pStr = Struct::insertNew(pArgs, pSubStruct);
                //inserNew clone inputs
                delete pSubStruct;
            }
            else
            {
                pStr = new types::Struct(1, 1);
            }
            //Add variable to scope
            symbol::Context::getInstance()->put(pVar->name_get(), *pStr);
        }
        else if (pIT->isHandle() || pIT->isStruct() || pIT->isTList() || pIT->isMList())
        {
            //one of complex types
            pStr = pIT;
        }
        else
        {
            //something else, return failed
            return false;
        }

        structHistory.push_front(pStr);
        return true;
    }
    else if (pCall)
    {
        //a(x,y)
        ExecVisitor execMe;

        typed_list *pCurrentArgs = execMe.GetArgumentList(pCall->args_get());
        typed_list *pReturnedArgs = NULL;

        if (structHistory.empty())
        {
            //a is the new main but can be a complex expression
            if (getStructFromExp(&pCall->name_get(), structHistory, fieldHistory, argHistory, NULL, _pbComplexForm) == false)
            {
                return false;
            }

            //paste of call to resize struct
            //pStr = Struct::insertNew(*_pArgs, p)->getAs<Struct>();

            //get head
            InternalType* pCurrent = structHistory.front();
            if ((*pCurrentArgs)[0]->isString())
            {
                //a("field") <=> a.field so do same thing
                String* pS = (*pCurrentArgs)[0]->getAs<String>();
                if (pCurrentArgs->size() != 1 || pS->isScalar() == false)
                {
                    std::wostringstream os;
                    os << _W("Invalid Index.\n");
                    throw ScilabError(os.str(), 999, (*(pCall->args_get().begin()))->location_get());
                }

                wchar_t* pFieldName = pS->get(0);
                fieldHistory.push_front(pFieldName);

                //try to extract field
                if (pCurrent->isStruct())
                {
                    Struct* pStr = NULL;
                    Struct *pCurStr = pCurrent->getAs<Struct>();
                    if (pReturnedArgs && (*pReturnedArgs)[0]->isString() == false)
                    {
                        pStr = pCurStr->extractWithoutClone(pReturnedArgs)->getAs<Struct>();
                        pStr->setCloneInCopyValue(false);
                    }
                    else
                    {
                        pStr = pCurStr;
                    }

                    SingleStruct* pSS = pStr->get(0);

                    ////check if field already exists
                    //if (pStr->exists(pFieldName))
                    //{
                    //    InternalType* pField = pSS->get(pFieldName);
                    //    if (pField->isStruct())
                    //    {
                    //        pStr = pField->getAs<Struct>();
                    //    }
                    //    else
                    //    {
                    //        //erase previous value by a struct(1,1)
                    //        pSS->set(pFieldName, new Struct(1, 1));
                    //        pStr = pSS->get(pFieldName)->getAs<Struct>();
                    //    }
                    //}
                    //else
                    //{
                    //    //field does not exist
                    //    pCurStr->addField(pFieldName);
                    //    pSS->set(pFieldName, new Struct(1, 1));
                    //    pCurrent = pSS->get(pFieldName);
                    //}

                    //if (pReturnedArgs && (*pReturnedArgs)[0]->isString() == false)
                    //{
                    //    pSS->IncreaseRef();
                    //    delete pStr;
                    //    pSS->DecreaseRef();

                    //    //clean pReturnedArgs return by GetArgumentList
                    //    for (int iArg = 0 ; iArg < pReturnedArgs->size() ; iArg++)
                    //    {
                    //        if ((*pReturnedArgs)[iArg]->isDeletable())
                    //        {
                    //            delete (*pReturnedArgs)[iArg];
                    //        }
                    //    }
                    //    delete pReturnedArgs;
                    //}
                }
                else if (pCurrent->isTList() || pCurrent->isMList())
                {

                }
                else
                {
                    ////handle
                    //GraphicHandle* pCurH = pCurrent->getAs<GraphicHandle>();
                }
            }
            else
            {
                //for extract with indexes, we have to save field and data in upper level.
                //just add data to allow parent to work on it but use empty field to don't assign it after
                argHistory.push_front(execMe.GetArgumentList(pCall->args_get()));
                fieldHistory.push_front(L"");

                if (_pAssignValue)
                {
                    //call standard insert mechanism
                    insertionCall(*_pExp, pCurrentArgs, pCurrent, _pAssignValue);
                }
                else
                {
                    /*try to extract sub struct, if it fails, resize the struct and try again*/
                    if (pCurrent->isStruct())
                    {
                        Struct* pCurStr = pCurrent->getAs<Struct>();
                        InternalType* pIT = pCurStr->extract(pCurrentArgs);
                        if (pIT == NULL)
                        {
                            //fail to extract, pCurrent is not enough big, resize it !
                            Struct* p = new Struct(1, 1);
                            String* pFields = pCurStr->getFieldNames();
                            for (int i = 0 ; i < pFields->getSize() ; i++)
                            {
                                p->addField(pFields->get(0));
                            }

                            pCurStr->insertWithoutClone(pCurrentArgs, p); //insert empty struct, caller will assign the good value
                            pIT = pCurStr->extractWithoutClone(pCurrentArgs);
                        }

                        structHistory.push_front(pIT);
                    }
                    else if (pCurrent->isTList() || pCurrent->isMList())
                    {
                        *_pbComplexForm = true;
                        //extract only
                        types::typed_list in;
                        types::optional_list opt;
                        types::typed_list out;
                        ExecVisitor exec;

                        //firts argument: indexes
                        for (int i = 0 ; i < pCurrentArgs->size() ; i++)
                        {
                            in.push_back((*pCurrentArgs)[i]);
                        }

                        //last argument: me ( mlist or tlist )
                        in.push_back(pCurrent);

                        //protect data during overload call
                        for (int i = 0 ; i < in.size() ; i++)
                        {
                            in[i]->IncreaseRef();
                        }

                        Callable::ReturnValue Ret = Overload::call(L"%" + pCurrent->getShortTypeStr() + L"_6", in, 1, out, &exec);

                        //unprotect inputs
                        for (int i = 0 ; i < in.size() ; i++)
                        {
                            in[i]->DecreaseRef();
                        }

                        if (Ret != Callable::OK)
                        {
                            throw ScilabError(L"", 999, pCall->location_get());
                        }

                        if (out.size() == 0)
                        {
                            //hope never come here
                            pCurrent = NULL;
                        }

                        out[0]->DecreaseRef();

                        structHistory.push_front(out[0]);
                    }
                    else if (pCurrent->isHandle())
                    {
                        //GraphicHandle* pCurH = pCurrent->getAs<GraphicHandle>();
                    }
                    else
                    {
                        return false;
                    }
                }
            }
        }
        else
        {
            //we have a parent, so assign "a" to this parent
            // (*_pMain)->set(0, pStruct->get(0));
        }
        return true;
    }
    else if (pCell)
    {
    }
    else
    {
        std::wostringstream os;
        os << _W("Unknown expression");
        //os << ((Location)e.right_exp_get().location_get()).location_getString() << std::endl;
        throw ScilabError(os.str(), 999, _pExp->location_get());
    }
    return false;
}


/*** overload insertion ***/
//%x_i_x(i1, i2, ..., in, source, dest)
//i1, ..., in : indexes
//dest : variable where to insert data
//source : data to insert
types::InternalType* callOverload(const Exp& e, std::wstring _strType, types::typed_list* _pArgs, types::InternalType* _source, types::InternalType* _dest)
{
    types::InternalType* pITOut = NULL;
    types::typed_list in;
    types::typed_list out;

    for(int i = 0; i < _pArgs->size(); i++)
    {
        (*_pArgs)[i]->IncreaseRef();
        in.push_back((*_pArgs)[i]);
    }

    _source->IncreaseRef();
    _dest->IncreaseRef();

    in.push_back(_source);
    in.push_back(_dest);

    std::wstring function_name;
    function_name = L"%" + _source->getAs<List>()->getShortTypeStr() + L"_" + _strType + L"_" +_dest->getAs<List>()->getShortTypeStr();
    if(symbol::Context::getInstance()->get(symbol::Symbol(function_name)))
    {
        ExecVisitor exec;
        if (Overload::call(function_name, in, 1, out, &exec))
        {
            //manage error
            std::wostringstream os;
            os << _W("Error in overload function: ") << function_name << std::endl;
            throw ScilabError(os.str(), 999, e.location_get());
        }
    }

    for(int i = 0; i < _pArgs->size(); i++)
    {
        (*_pArgs)[i]->DecreaseRef();
    }

    _source->DecreaseRef();
    _dest->DecreaseRef();

    if (out.size() != 0)
    {
        pITOut = out[0];
    }

    return pITOut;
}

InternalType* insertionCall(const Exp& e, typed_list* _pArgs, InternalType* _pVar, InternalType* _pInsert)
{
    InternalType* pOut = NULL;
    //fisrt extract implicit list
    if (_pInsert->isColon())
    {
        //double* pdbl = NULL;
        //_pInsert = new Double(-1, -1, &pdbl);
        //pdbl[0] = 1;
        _pInsert = Double::Identity(-1, -1);
    }
    else if (_pInsert->isImplicitList())
    {
        InternalType *pIL = _pInsert->getAs<ImplicitList>()->extractFullMatrix();
        if (pIL)
        {
            _pInsert = pIL;
        }
    }
    else if (_pInsert->isContainer() && _pInsert->isRef())
    {
        //std::cout << "assign container type during insertion" << std::endl;
        InternalType* pIL = _pInsert->clone();
        _pInsert = pIL;
    }

    if (_pInsert->isDouble() && _pInsert->getAs<Double>()->isEmpty() && _pVar == NULL)
    {
        // l(x) = [] when l is not defined => create l = []
        pOut = Double::Empty();
    }
    else if (_pInsert->isDouble() && _pInsert->getAs<Double>()->isEmpty() && _pVar->isStruct() == false && _pVar->isList() == false)
    {
        //insert [] so deletion except for Struct and List which can insert []
        if (_pVar->isDouble())
        {
            pOut = _pVar->getAs<Double>()->remove(_pArgs);
        }
        else if (_pVar->isString())
        {
            pOut = _pVar->getAs<String>()->remove(_pArgs);
        }
        else if (_pVar->isCell())
        {
            pOut = _pVar->getAs<Cell>()->remove(_pArgs);
        }
        else if (_pVar->isBool())
        {
            pOut = _pVar->getAs<Bool>()->remove(_pArgs);
        }
        else if (_pVar->isPoly())
        {
            pOut = _pVar->getAs<Polynom>()->remove(_pArgs);
        }
        else if (_pVar->isInt8())
        {
            pOut = _pVar->getAs<Int8>()->remove(_pArgs);
        }
        else if (_pVar->isUInt8())
        {
            pOut = _pVar->getAs<UInt8>()->remove(_pArgs);
        }
        else if (_pVar->isInt16())
        {
            pOut = _pVar->getAs<Int16>()->remove(_pArgs);
        }
        else if (_pVar->isUInt16())
        {
            pOut = _pVar->getAs<UInt16>()->remove(_pArgs);
        }
        else if (_pVar->isInt32())
        {
            pOut = _pVar->getAs<Int32>()->remove(_pArgs);
        }
        else if (_pVar->isUInt32())
        {
            pOut = _pVar->getAs<UInt32>()->remove(_pArgs);
        }
        else if (_pVar->isInt64())
        {
            pOut = _pVar->getAs<Int64>()->remove(_pArgs);
        }
        else if (_pVar->isUInt64())
        {
            pOut = _pVar->getAs<UInt64>()->remove(_pArgs);
        }
        else if (_pVar->isStruct())
        {
            // a("b") = [] is not a deletion !!
            Struct* pStr = _pVar->getAs<Struct>();

            pOut = _pVar->getAs<Struct>()->insert(_pArgs, _pInsert);
        }
    }
    else if (_pVar == NULL || (_pVar->isDouble() && _pVar->getAs<Double>()->getSize() == 0))
    {
        //insert in a new variable or []
        //call static insert function
        //if _pVar == NULL and pArg is single string, it's a struct creation
        if ((*_pArgs)[0]->isString())
        {
            String *pS = (*_pArgs)[0]->getAs<types::String>();
            Struct* pStr = new Struct(1, 1);

            if (_pArgs->size() != 1 || pS->isScalar() == false)
            {
                //manage error
                std::wostringstream os;
                os << _W("Invalid Index.\n");
                throw ScilabError(os.str(), 999, e.location_get());
            }

            pStr->addField(pS->get(0));
            pStr->get(0)->set(pS->get(0), _pInsert);
            pOut = pStr;
        }
        else
        {
            switch (_pInsert->getType())
            {
                case InternalType::RealDouble :
                    pOut = Double::insertNew(_pArgs, _pInsert);
                    break;
                case InternalType::RealString :
                    pOut = String::insertNew(_pArgs, _pInsert);
                    break;
                case InternalType::RealCell :
                    pOut = Cell::insertNew(_pArgs, _pInsert);
                    break;
                case InternalType::RealBool :
                    pOut = Bool::insertNew(_pArgs, _pInsert);
                    break;
                case InternalType::RealPoly :
                    pOut = Polynom::insertNew(_pArgs, _pInsert);
                    break;
                case InternalType::RealInt8 :
                    pOut = Int8::insertNew(_pArgs, _pInsert);
                    break;
                case InternalType::RealUInt8 :
                    pOut = UInt8::insertNew(_pArgs, _pInsert);
                    break;
                case InternalType::RealInt16 :
                    pOut = Int16::insertNew(_pArgs, _pInsert);
                    break;
                case InternalType::RealUInt16 :
                    pOut = UInt16::insertNew(_pArgs, _pInsert);
                    break;
                case InternalType::RealInt32 :
                    pOut = Int32::insertNew(_pArgs, _pInsert);
                    break;
                case InternalType::RealUInt32 :
                    pOut = UInt32::insertNew(_pArgs, _pInsert);
                    break;
                case InternalType::RealInt64 :
                    pOut = Int64::insertNew(_pArgs, _pInsert);
                    break;
                case InternalType::RealUInt64 :
                    pOut = UInt64::insertNew(_pArgs, _pInsert);
                    break;
                case InternalType::RealSparse :
                    pOut = Sparse::insertNew(_pArgs, _pInsert);
                    break;
                case InternalType::RealSparseBool :
                    pOut = SparseBool::insertNew(_pArgs, _pInsert);
                    break;
                default :
                {
                    // overload
                    types::Double* pEmpty = types::Double::Empty();
                    pOut = callOverload(e, L"i", _pArgs, _pInsert, pEmpty);
                    delete pEmpty;
                    break;
                }
            }
        }
    }
    else
    {
        //call type insert function
        InternalType* pRet = NULL;

        //check types compatibilties
        if (_pVar->isDouble() && _pInsert->isDouble())
        {
            pRet = _pVar->getAs<Double>()->insert(_pArgs, _pInsert);
        }
        else if (_pVar->isDouble() && _pInsert->isSparse())
        {
            Sparse* pSp = _pInsert->getAs<Sparse>();
            Double* pD = new Double(pSp->getRows(), pSp->getCols(), pSp->isComplex());
            pSp->fill(*pD);
            pRet = _pVar->getAs<Double>()->insert(_pArgs, pD);
            free(pD);
        }
        else if (_pVar->isString() && _pInsert->isString())
        {
            pRet = _pVar->getAs<String>()->insert(_pArgs, _pInsert);
        }
        else if (_pVar->isCell() && _pInsert->isCell())
        {
            pRet = _pVar->getAs<Cell>()->insert(_pArgs, _pInsert);
        }
        else if (_pVar->isBool() && _pInsert->isBool())
        {
            pRet = _pVar->getAs<Bool>()->insert(_pArgs, _pInsert);
        }
        else if (_pVar->isSparse() && _pInsert->isSparse())
        {
            pRet = _pVar->getAs<Sparse>()->insert(_pArgs, _pInsert->getAs<Sparse>());
        }
        else if (_pVar->isSparse() && _pInsert->isDouble())
        {
            pRet = _pVar->getAs<Sparse>()->insert(_pArgs, _pInsert);
        }
        else if (_pVar->isSparseBool() && _pInsert->isSparseBool())
        {
            pRet = _pVar->getAs<SparseBool>()->insert(_pArgs, _pInsert->getAs<SparseBool>());
        }
        else if (_pVar->isSparseBool() && _pInsert->isBool())
        {
            pRet = _pVar->getAs<SparseBool>()->insert(_pArgs, _pInsert);
        }
        else if (_pVar->isPoly() && _pInsert->isDouble())
        {
            Polynom* pDest = _pVar->getAs<Polynom>();
            Double* pIns = _pInsert->getAs<Double>();
            Polynom* pP = new Polynom(pDest->getVariableName(), pIns->getDims(), pIns->getDimsArray());
            pP->setComplex(pIns->isComplex());

            for (int idx = 0 ; idx < pP->getSize() ; idx++)
            {
                double* pR = NULL;
                double* pI = NULL;
                if (pP->isComplex())
                {
                    SinglePoly* pS = new SinglePoly(&pR, &pI, 1);
                    double dblR = pIns->get(idx);
                    double dblI = pIns->getImg(idx);
                    pS->setCoef(&dblR, &dblI);
                    pP->set(idx, pS);
                    delete pS;
                }
                else
                {
                    SinglePoly* pS = new SinglePoly(&pR, 1);
                    double dblR = pIns->get(idx);
                    pS->setCoef(&dblR, NULL);
                    pP->set(idx, pS);
                    delete pS;
                }
            }
            pRet = _pVar->getAs<Polynom>()->insert(_pArgs, pP);
            delete pP;
        }
        else if (_pVar->isPoly() && _pInsert->isDouble())
        {
            Polynom* pDest = _pVar->getAs<Polynom>();
            Double* pIns = _pInsert->getAs<Double>();
            Polynom* pP = new Polynom(pDest->getVariableName(), pIns->getDims(), pIns->getDimsArray());
            pP->setComplex(pIns->isComplex());

            for (int idx = 0 ; idx < pP->getSize() ; idx++)
            {
                double* pR = NULL;
                double* pI = NULL;
                if (pP->isComplex())
                {
                    SinglePoly* pS = new SinglePoly(&pR, &pI, 1);
                    double dblR = pIns->get(idx);
                    double dblI = pIns->getImg(idx);
                    pS->setCoef(&dblR, &dblI);
                    pP->set(idx, pS);
                    delete pS;
                }
                else
                {
                    SinglePoly* pS = new SinglePoly(&pR, 1);
                    double dblR = pIns->get(idx);
                    pS->setCoef(&dblR, NULL);
                    pP->set(idx, pS);
                    delete pS;
                }
            }
            pRet = pDest->insert(_pArgs, pP);
            delete pP;
        }
        else if (_pVar->isPoly() && _pInsert->isPoly())
        {
            pRet = _pVar->getAs<Polynom>()->insert(_pArgs, _pInsert);
        }
        else if (_pVar->isInt8() && _pInsert->isInt8())
        {
            pRet = _pVar->getAs<Int8>()->insert(_pArgs, _pInsert);
        }
        else if (_pVar->isUInt8() && _pInsert->isUInt8())
        {
            pRet = _pVar->getAs<UInt8>()->insert(_pArgs, _pInsert);
        }
        else if (_pVar->isInt16() && _pInsert->isInt16())
        {
            pRet = _pVar->getAs<Int16>()->insert(_pArgs, _pInsert);
        }
        else if (_pVar->isUInt16() && _pInsert->isUInt16())
        {
            pRet = _pVar->getAs<UInt16>()->insert(_pArgs, _pInsert);
        }
        else if (_pVar->isInt32() && _pInsert->isInt32())
        {
            pRet = _pVar->getAs<Int32>()->insert(_pArgs, _pInsert);
        }
        else if (_pVar->isUInt32() && _pInsert->isUInt32())
        {
            pRet = _pVar->getAs<UInt32>()->insert(_pArgs, _pInsert);
        }
        else if (_pVar->isInt64() && _pInsert->isInt64())
        {
            pRet = _pVar->getAs<Int64>()->insert(_pArgs, _pInsert);
        }
        else if (_pVar->isUInt64() && _pInsert->isUInt64())
        {
            pRet = _pVar->getAs<UInt64>()->insert(_pArgs, _pInsert);
        }
        else if (_pVar->isStruct())
        {
            Struct* pStr = _pVar->getAs<Struct>();
            if (_pArgs->size() == 1 && (*_pArgs)[0]->isString())
            {
                //s("x") = y
                String *pS = (*_pArgs)[0]->getAs<types::String>();
                if (pS->isScalar() == false)
                {
                    //manage error
                    std::wostringstream os;
                    os << _W("Invalid Index.\n");
                    throw ScilabError(os.str(), 999, e.location_get());
                }

                pStr->addField(pS->get(0));
                pStr->get(0)->set(pS->get(0), _pInsert);
                pRet = pStr;
            }
            else
            {
                pRet = pStr->insert(_pArgs, _pInsert);
            }
        }
        else if (_pVar->isTList() || _pVar->isMList())
        {
            TList* pTL = _pVar->getAs<TList>();
            if (_pArgs->size() == 1 && (*_pArgs)[0]->isString())
            {
                //s("x") = y
                String *pS = (*_pArgs)[0]->getAs<types::String>();
                if (pS->isScalar() == false)
                {
                    //manage error
                    std::wostringstream os;
                    os << _W("Invalid Index.\n");
                    throw ScilabError(os.str(), 999, e.location_get());
                }

                if (pTL->exists(pS->get(0)))
                {
                    pTL->set(pS->get(0), _pInsert);
                    pRet = pTL;
                }
                else
                {
                    ExecVisitor exec;
                    types::typed_list in;
                    types::typed_list out;
                    std::wstring function_name = L"%l_e";

                    _pInsert->IncreaseRef();
                    in.push_back(_pInsert);

                    Overload::call(function_name, in, 1, out, &exec);
                    _pInsert->DecreaseRef();

                    if (out.size() != 0)
                    {
                        pRet = in[0];
                    }
                }
            }
            else
            {
                if (_pVar->isTList())
                {
                    pRet = pTL->insert(_pArgs, _pInsert);
                }
                else //MList, call overload function
                {
                    pRet = callOverload(e, L"i", _pArgs, _pInsert, pTL);
                }
            }
        }
        else if (_pVar->isList())
        {
            pRet = _pVar->getAs<List>()->insert(_pArgs, _pInsert);
        }
        else if (_pVar->isHandle())
        {
            if (_pArgs->size() == 1 && (*_pArgs)[0]->isString())
            {
                //s(["x"])
                types::GraphicHandle* pH = _pVar->getAs<types::GraphicHandle>();
                types::String *pS = (*_pArgs)[0]->getAs<types::String>();
                typed_list in;
                typed_list out;
                optional_list opt;
                ExecVisitor exec;

                in.push_back(pH);
                in.push_back(pS);
                in.push_back(_pInsert);

                Function* pCall = (Function*)symbol::Context::getInstance()->get(symbol::Symbol(L"set"));
                Callable::ReturnValue ret =  pCall->call(in, opt, 1, out, &exec);
                if (ret == Callable::OK)
                {
                    pRet = _pVar;
                }
            }
            else
            {
                pRet = _pVar->getAs<types::GraphicHandle>()->extract(_pArgs);
            }
        }
        else
        {
            // overload
            pRet = callOverload(e, L"i", _pArgs, _pInsert, _pVar);
        }

        pOut = pRet;
    }
    return pOut;
}

void callOnPrompt(void)
{
    types::InternalType* pOnPrompt = NULL;
    pOnPrompt = symbol::Context::getInstance()->get(symbol::Symbol(L"%onprompt"));
    if (pOnPrompt != NULL && pOnPrompt->isCallable())
    {
        types::typed_list in;
        types::typed_list out;
        types::optional_list opt;
        ExecVisitor execCall;
        pOnPrompt->getAs<types::Callable>()->call(in, opt, 1, out, &execCall);
    }
}

List* getPropertyTree(Exp* e, List* pList)
{

    //a.b
    SimpleVar* pVar = dynamic_cast<SimpleVar*>(e);
    if (pVar)
    {
        pList->append(new String(pVar->name_get().name_get().c_str()));
        return pList;
    }

    //a(x).b
    CallExp* pCall = dynamic_cast<CallExp*>(e);
    if (pCall)
    {
        pList = getPropertyTree(&pCall->name_get(), pList);
        ExecVisitor exec;
        std::list<Exp*> l = pCall->args_get();
        std::list<Exp*>::const_iterator it;
        for (it = l.begin() ; it != l.end() ; it++)
        {
            Exp* pArg = (*it);
            try
            {
                pArg->accept(exec);
                pList->append(exec.result_get());
                exec.result_clear();
            }
            catch (ScilabException e)
            {
                throw e;
            }
        }

        return pList;
    }

    //a.b.c
    FieldExp* pField = dynamic_cast<FieldExp*>(e);
    if (pField)
    {
        pList = getPropertyTree(pField->head_get(), pList);
        pList = getPropertyTree(pField->tail_get(), pList);
        return pList;
    }

    return pList;
}
