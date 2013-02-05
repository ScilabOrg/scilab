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


bool getStructFromExp(const ast::Exp* _pExp, std::list<types::InternalType*>& structHistory, std::list<const ast::Exp*>& fieldHistory, std::list<types::typed_list*>& argHistory, types::InternalType* _pAssignValue, bool _bFromCallExp)
{
    const FieldExp* pField      = dynamic_cast<const FieldExp*>(_pExp);
    const SimpleVar* pVar       = dynamic_cast<const SimpleVar*>(_pExp);
    const CallExp* pCall        = dynamic_cast<const CallExp*>(_pExp);
    const CellCallExp* pCell    = dynamic_cast<const CellCallExp*>(_pExp);

    InternalType* pITOut = NULL;

    if (pField)
    {
        //y.x
        //evaluate head "y"
        if (getStructFromExp(pField->head_get(), structHistory, fieldHistory, argHistory, NULL, false))
        {
            //evaluate tail "x"
            if (getStructFromExp(pField->tail_get(), structHistory, fieldHistory, argHistory, _pAssignValue, _bFromCallExp))
            {
                return true;
            }
        }

        //error
        return false;
    }
    else if (pVar)
    {
        //a.x : with x not only a SimpleVar
        List* pOut = new List();

        // main variable
        if (fieldHistory.size() == 0)
        {
            InternalType* pIT = symbol::Context::getInstance()->get(pVar->name_get());
            if (pIT == NULL)
            {
                //"a" does not exist or it is another type, create it with size 1,1 and return it
                //create new structure variable
                pIT = new Struct(1, 1);
                symbol::Context::getInstance()->put(pVar->name_get(), *pIT);
            }

            pOut->appendWithoutClone(pIT);
        }
        else
        {
            // fields
            InternalType* pParent = structHistory.front();
            if (pParent->isList())
            {
                List* pL = pParent->getAs<List>();
                // if a is a field of a matrix of struct, loop on all elements extracted.
                for (int i = 0; i < pL->getSize(); i++)
                {
                    InternalType* pCurParent = pL->get(i);
                    if (pCurParent->isStruct())
                    {
                        Struct* pStructParent = pCurParent->getAs<Struct>();
                        std::wstring pwcsFieldname = pVar->name_get().name_get();

                        // create field in parent if it not exist
                        if (pStructParent->exists(pwcsFieldname) == false)
                        {
                            pStructParent->addField(pwcsFieldname);
                        }

                        // get a(args)
                        if (argHistory.front())
                        {
                            InternalType* pIT = pStructParent->extractWithoutClone(argHistory.front());
                            pStructParent = pIT->getAs<Struct>();
                        }

                        // insertion management of last field .x = something
                        if (_pAssignValue)
                        {
                            // .x(arg) = something
                            // insertrion will be done in pCall
                            if (_bFromCallExp)
                            {
                                pOut->appendWithoutClone(pStructParent);
                                continue;
                            }

                            // .x = something
                            for (int iterStruct = 0; iterStruct < pStructParent->getSize(); iterStruct++)
                            {
                                pStructParent->get(iterStruct)->set(pwcsFieldname, _pAssignValue);
                            }

                            continue;
                        }

                        // create a new struct only instead of "undefined" element(s).
                        for (int iterStruct = 0; iterStruct < pStructParent->getSize(); iterStruct++)
                        {
                            SingleStruct* pSS = pStructParent->get(iterStruct);
                            InternalType* pIT = pSS->get(pwcsFieldname);
                            if (pIT->isDouble() && pIT->getAs<Double>()->isEmpty())
                            {
                                pSS->set(pwcsFieldname, new Struct(1, 1));
                            }
                        }

                        // extract field x and append it to elements for next recursion.
                        List* pLOut = pStructParent->extractFieldWithoutClone(pwcsFieldname);
                        for (int iterList = 0; iterList < pLOut->getSize(); iterList++)
                        {
                            pOut->appendWithoutClone(pLOut->get(iterList));
                        }
                    }
                    else if (pCurParent->isTList())
                    {
                        if (_bFromCallExp)
                        {
                            // a(x) managed in pCall case
                            return true;
                        }

                        // a.x
                        TList* pTL = pCurParent->getAs<TList>();
                        std::wstring pwcsFieldname = pVar->name_get().name_get();

                        // check if field exists
                        if (pTL->exists(pwcsFieldname) == false)
                        {
                            //call overload
                            return false;
                        }

                        if (_pAssignValue)
                        {
                            pTL->set(pwcsFieldname, _pAssignValue);
                            continue;
                        }

                        // extract field x and append it to elements for next recursion.
                        InternalType* pITOut = pTL->getField(pwcsFieldname);
                        pOut->appendWithoutClone(pITOut);
                    }
                    else
                    {
                        if (_bFromCallExp)
                        {
                            // a(arg) => error
                            std::wostringstream os;
                            os << _W("Invalid index");
                            throw ScilabError(os.str(), 999, _pExp->location_get());
                        }

                        // replace parent by a struct
                        if (structHistory.size() == 1)
                        {
                            // modify "a" from context
                            Struct* pNewStruct = new Struct(1, 1);
                            symbol::Context::getInstance()->put(((SimpleVar*)fieldHistory.front())->name_get(), *pNewStruct);

                            // reset history
                            structHistory.pop_front();
                            fieldHistory.pop_front();
                            argHistory.pop_front();

                            // reevaluate a
                            if (getStructFromExp(fieldHistory.front(), structHistory, fieldHistory, argHistory, _pAssignValue, _bFromCallExp) == false)
                            {
                                return false;
                            }
                        }
                        else
                        {
                            //std::list<InternalType*>::iterator iterStruct = structHistory.begin();
                            //std::list<const Exp*>::iterator iterField = fieldHistory.begin();
                            //std::list<typed_list*>::iterator iterArg = argHistory.begin();

                            //// get parent
                            //iterStruct++;
                            //iterField++;
                            //iterArg++;

                            //// modify field wich contain pCurParent by a new struct
                            //InternalType* pIT = structHistory.front();
                            //if(pIT->isStruct())
                            //{
                            //    Struct* pStruct = pIT->getAs<Struct>();
                            //    if (*iterArg)
                            //    {
                            //        InternalType* pIT = pStruct->extractWithoutClone(*iterArg);
                            //        pStruct = pIT->getAs<Struct>();
                            //    }

                            //    for (int iterStruct = 0; iterStruct < pStruct->getSize(); iterStruct++)
                            //    {
                            //        pStruct->get(iterStruct)->set(((SimpleVar*)(*iterField))->name_get().name_get(), Double::Empty());
                            //    }

                            //}
                            //else if(pIT->isTList())
                            //{

                            //}
                            //else
                            //{
                            //    std::wostringstream os;
                            //    os << _W("TO DO");
                            //    throw ScilabError(os.str(), 999, _pExp->location_get());
                            //}

                            //
                            //// create new history
                            //std::list<InternalType*> newStructH;
                            //std::list<const Exp*> newFieldH;
                            //std::list<typed_list*> newArgH;

                            //newStructH.push_back(*iterStruct);
                            //newFieldH.push_back(*iterField);
                            //newArgH.push_back(*iterArg);

                            //// reevaluate a
                            //if(getStructFromExp(*iterField, structHistory, fieldHistory, argHistory, _pAssignValue, _bFromCallExp) == false)
                            //{
                            //    return false;
                            //}

                            std::wostringstream os;
                            os << _W("Not yet implemented");
                            throw ScilabError(os.str(), 999, _pExp->location_get());
                        }

                        // reevaluate x
                        if (getStructFromExp(pVar, structHistory, fieldHistory, argHistory, _pAssignValue, _bFromCallExp) == false)
                        {
                            return false;
                        }

                        return true;
                    }
                } // for
            } // isList
        }

        argHistory.push_front(NULL);
        fieldHistory.push_front(_pExp);
        structHistory.push_front(pOut);

        return true;
    }
    else if (pCall)
    {
        //a.b(args)
        bool bFromCallExp = true;
        ExecVisitor execMe;
        typed_list *pCurrentArgs = execMe.GetArgumentList(pCall->args_get());

        // evaluate a.b
        if (getStructFromExp(&pCall->name_get(), structHistory, fieldHistory, argHistory, _pAssignValue, bFromCallExp) == false)
        {
            return false;
        }

        InternalType* pCurrent = structHistory.front();

        // set args in place of NULL inserted when we have evaluated "b"
        argHistory.pop_front();
        argHistory.push_front(pCurrentArgs);

        int* piMaxDim = new int[pCurrentArgs->size()];
        int* piCountDim = new int[pCurrentArgs->size()];
        typed_list pArgOut;

        // a.b(args) = "something"
        if (_pAssignValue)
        {
            if (pCurrent->isList())
            {
                List* pLParent = pCurrent->getAs<List>();
                for (int i = 0; i < pLParent->getSize(); i++)
                {
                    InternalType* pCurParent = pLParent->get(i);
                    if (pCurParent->isStruct())
                    {
                        // extract field b from struct a
                        List* pLChildren = pCurParent->getAs<Struct>()->extractFieldWithoutClone(((SimpleVar*)fieldHistory.front())->name_get().name_get());

                        for (int j = 0; j < pLChildren->getSize(); j++)
                        {
                            // insert "something" in b(x,y)
                            InternalType* pIT = insertionCall(*_pExp, pCurrentArgs, pLChildren->get(j), _pAssignValue);

                            // insert b(x,y) in a
                            pCurParent->getAs<Struct>()->get(j)->set(((SimpleVar*)fieldHistory.front())->name_get().name_get(), pIT);
                        }
                    }
                    else
                    {
                        std::wostringstream os;
                        os << _W("TO DO");
                        //os << ((Location)e.right_exp_get().location_get()).location_getString() << std::endl;
                        throw ScilabError(os.str(), 999, _pExp->location_get());
                    }
                }
            }
        }
        else
        {
            if (pCurrent->isList())
            {
                List* pL = pCurrent->getAs<List>();
                for (int i = 0; i < pL->getSize(); i++)
                {
                    if (pL->get(i)->isStruct())
                    {
                        /*try to extract sub struct, if it fails, resize the struct and try again*/
                        Struct* pCurStr = pL->get(i)->getAs<Struct>();
                        int iSizeStuct = pCurStr->getSize();

                        // compute indexes
                        int iCount = checkIndexesArguments(pCurStr, pCurrentArgs, &pArgOut, piMaxDim, piCountDim);

                        if (iSizeStuct < *piMaxDim)
                        {
                            // resize current struct
                            pCurStr->resize(*piMaxDim, 1);
                        }
                    }
                    else if (pL->get(i)->isTList() /*|| pL->get(i)->isMList()*/)
                    {
                        List* pOut = new List();
                        TList* pTL = pL->get(i)->getAs<TList>();
                        int iSize = pTL->getSize();

                        // compute indexes
                        int iCount = checkIndexesArguments(pTL, pCurrentArgs, &pArgOut, piMaxDim, piCountDim);

                        // resize TList
                        if (iSize < *piMaxDim)
                        {
                            pTL->set((*piMaxDim) - 1, new ListUndefined());
                        }

                        // extract args
                        std::vector<InternalType*> vectpIT = pTL->extract(pCurrentArgs);
                        std::vector<InternalType*>::iterator iterVect;

                        // for all undefined field, create a new struct
                        int iPos = 0;
                        for (iterVect = vectpIT.begin(); iterVect != vectpIT.end(); iterVect++, iPos++)
                        {
                            if ((*iterVect)->isListUndefined())
                            {
                                Struct* pNewStruct = new Struct(1, 1);
                                int iTLPos = pArgOut[iPos]->getAs<Double>()->get(0) - 1;
                                pTL->set(iTLPos, pNewStruct);
                                pOut->appendWithoutClone(pNewStruct);
                            }
                            else
                            {
                                pOut->appendWithoutClone(*iterVect);
                            }
                        }

                        structHistory.pop_front();
                        structHistory.push_front(pOut);

                        // arg managed so removed it from history
                        argHistory.pop_front();
                        argHistory.push_front(NULL);
                    }
                    else if (pL->get(i)->isHandle())
                    {
                        //GraphicHandle* pCurH = pCurrent->getAs<GraphicHandle>();
                    }
                    else
                    {
                        return false;
                    }
                } // for
            } // isList
            else
            {
                return false;
            }
        }

        delete[] piMaxDim;
        delete[] piCountDim;
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
    return NULL;
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

    for (int i = 0; i < _pArgs->size(); i++)
    {
        (*_pArgs)[i]->IncreaseRef();
        in.push_back((*_pArgs)[i]);
    }

    _source->IncreaseRef();
    _dest->IncreaseRef();

    in.push_back(_source);
    in.push_back(_dest);

    std::wstring function_name;
    function_name = L"%" + _source->getAs<List>()->getShortTypeStr() + L"_" + _strType + L"_" + _dest->getAs<List>()->getShortTypeStr();
    if (symbol::Context::getInstance()->get(symbol::Symbol(function_name)))
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

    for (int i = 0; i < _pArgs->size(); i++)
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
        //InternalType* pIL = _pInsert->clone();
        //_pInsert = pIL;
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
        else if (_pVar->isDouble() && _pInsert->isPoly())
        {
            Double* pDest = _pVar->getAs<Double>();
            Polynom* pIns = _pInsert->getAs<Polynom>();
            Polynom* pP = new Polynom(pIns->getVariableName(), pDest->getDims(), pDest->getDimsArray());
            pP->setComplex(pDest->isComplex());

            for (int idx = 0 ; idx < pP->getSize() ; idx++)
            {
                double* pR = NULL;
                double* pI = NULL;
                if (pP->isComplex())
                {
                    SinglePoly* pS = new SinglePoly(&pR, &pI, 1);
                    double dblR = pDest->get(idx);
                    double dblI = pDest->getImg(idx);
                    pS->setCoef(&dblR, &dblI);
                    pP->set(idx, pS);
                    delete pS;
                }
                else
                {
                    SinglePoly* pS = new SinglePoly(&pR, 1);
                    double dblR = pDest->get(idx);
                    pS->setCoef(&dblR, NULL);
                    pP->set(idx, pS);
                    delete pS;
                }
            }

            pRet = pP->insert(_pArgs, pIns);
            pDest->DecreaseRef();
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
            // insert something in a field of a struct
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
                for (int i = 0; i < pStr->getSize(); i++)
                {
                    pStr->get(i)->set(pS->get(0), _pInsert);
                }

                pRet = pStr;
            }
            else // insert something in a struct
            {
                pRet = callOverload(e, L"i", _pArgs, _pInsert, _pVar);
                //pRet = pStr->insert(_pArgs, _pInsert);
            }
        }
        else if (_pVar->isTList() || _pVar->isMList())
        {
            TList* pTL = _pVar->getAs<TList>();
            if (_pArgs->size() == 1)
            {
                if ((*_pArgs)[0]->isString())
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
                    // s(x)
                    if (_pVar->isMList())
                    {
                        pRet = callOverload(e, L"i", _pArgs, _pInsert, _pVar);
                    }
                    else
                    {
                        pRet = pTL->insert(_pArgs, _pInsert);
                    }
                }
            }
            else
            {
                pRet = callOverload(e, L"i", _pArgs, _pInsert, _pVar);
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
