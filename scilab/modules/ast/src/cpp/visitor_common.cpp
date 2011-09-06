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

bool bConditionState(types::InternalType *_pITResult)
{
	if(_pITResult->isDouble() &&
        _pITResult->getAs<types::Double>()->isComplex() == false)
	{
		types::Double *pR = _pITResult->getAs<types::Double>();
        if(pR->isEmpty())
        {//[]
            return false;
        }

		double *pReal = pR->getReal();
		for(int i = 0 ; i < pR->getSize() ; i++)
		{
			if(pReal[i] == 0)
			{
				return false;
			}
		}
	}
	else if(_pITResult->isBool())
	{
		types::Bool *pB		= _pITResult->getAs<types::Bool>();
		int *piData	= pB->get();

		for(int i = 0 ; i < pB->getSize() ; i++)
		{
			if(piData[i] == 0)
			{
				return false;
				break;
			}
		}
	}
	else if(_pITResult->isInt())
	{
	}
	else
	{
		return false;
	}
	return true;
}

/*
 * Generate destination variable from _poSource type and size parameters
 */
types::InternalType* allocDest(types::InternalType* _poSource, int _iRows, int _iCols)
{
    types::InternalType* poResult = NULL;
    switch(_poSource->getType())
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
            for(int i = 0 ; i < _iRows * _iCols ; i++)
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


    if(_poDest == NULL)
    {//First call, alloc _poSource
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

    if(TypeDest != TypeSource)
    {//check if source type is compatible with dest type
    }
    else
    {
        switch(TypeDest)
        {
        case types::GenericType::RealDouble :
            if(poResult->getAs<types::Double>()->isComplex() == false && _poSource->getAs<types::Double>()->isComplex() == true)
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

    if(_poDest == NULL)
    {//First call, alloc _poSource
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


    if(TypeDest != TypeSource)
    {//check if source type is compatible with dest type
    }
    else
    {
        switch(TypeDest)
        {
        case types::GenericType::RealDouble :
            if(poResult->getAs<types::Double>()->isComplex() == false && _poSource->getAs<types::Double>()->isComplex() == true)
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
types::InternalType* AddElementToVariable(types::InternalType* _poDest, types::InternalType* _poSource, int _iRows, int _iCols, int *_piRows, int *_piCols)
{
	types::InternalType *poResult	= NULL;
	types::InternalType::RealType TypeSource	= _poSource->getType();
	types::InternalType::RealType TypeDest		=	types::InternalType::RealInternal;
	int iCurRow = _iRows;
	int iCurCol = _iCols;

	if(_poDest == NULL)
	{
		switch(TypeSource)
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
				for(int i = 0 ; i < _iRows * _iCols ; i++)
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
            // FIXME What should we do here ...
            break;
		}
		iCurCol		= 0;
		iCurRow		= 0;
		TypeDest	=	TypeSource;
	}
	else
	{
		TypeDest		= _poDest->getType();
		poResult = _poDest;
	}


	if(TypeDest != TypeSource)
	{//check if source type is compatible with dest type
		switch(TypeDest)
		{
		case types::GenericType::RealDouble :
			if(TypeSource == types::GenericType::RealPoly)
			{
				types::Double *poDest = _poDest->getAs<types::Double>();
				//Convert Dest to RealPoly
				int *piRank = new int[poDest->getSize()];
				for(int i = 0 ; i < poDest->getSize() ; i++)
				{
					piRank[i] = 1;
				}

				poResult = new types::Polynom(_poSource->getAs<types::Polynom>()->getVariableName(), poDest->getRows(), poDest->getCols(),  piRank);

				double *pR = poDest->getReal();
				double *pI = poDest->getImg();
				for(int i = 0 ; i < poDest->getSize() ; i++)
				{
					types::Double *pdbl = NULL;
					if(poDest->isComplex())
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

				poResult->getAs<types::Polynom>()->setCoef(iCurRow, iCurCol, _poSource->getAs<types::Polynom>()->get(0)->getCoef());
			}
			break;
		case types::GenericType::RealPoly :
			if(TypeSource == types::GenericType::RealDouble)
			{
				//Add Source like coef of the new element
				types::SinglePoly* pPolyOut	= poResult->getAs<types::Polynom>()->get(iCurRow, iCurCol);

				pPolyOut->setRank(1);
                pPolyOut->setCoef(_poSource->getAs<types::Double>());
			}
			break;
		default:
			break;
		}
		return poResult;
	}
	else
	{//Just add the new value in the current item
		switch(TypeDest)
		{
		case types::GenericType::RealDouble :
    		{
            types::Double* pDblSource = _poSource->getAs<types::Double>();
			poResult->getAs<types::Double>()->append(iCurRow, iCurCol, pDblSource);
			*_piRows = pDblSource->getRows();
			*_piCols = pDblSource->getCols();
			break;
			}
		case types::GenericType::RealPoly :
			poResult->getAs<types::Polynom>()->append(iCurRow, iCurCol, _poSource->getAs<types::Polynom>());
			*_piRows = _poSource->getAsGenericType()->getRows();
			*_piCols = _poSource->getAsGenericType()->getCols();
			break;
		case types::GenericType::RealBool:
			poResult->getAs<types::Bool>()->append(iCurRow, iCurCol, _poSource->getAs<types::Bool>());
			*_piRows = _poSource->getAsGenericType()->getRows();
			*_piCols = _poSource->getAsGenericType()->getCols();
			break;
		case types::GenericType::RealInt8 :
			poResult->getAs<types::Int8>()->append(iCurRow, iCurCol, _poSource->getAs<types::Int8>());
			*_piRows = _poSource->getAsGenericType()->getRows();
			*_piCols = _poSource->getAsGenericType()->getCols();
			break;
		case types::GenericType::RealUInt8 :
			poResult->getAs<types::UInt8>()->append(iCurRow, iCurCol, _poSource->getAs<types::UInt8>());
			*_piRows = _poSource->getAsGenericType()->getRows();
			*_piCols = _poSource->getAsGenericType()->getCols();
			break;
		case types::GenericType::RealInt16 :
			poResult->getAs<types::Int16>()->append(iCurRow, iCurCol, _poSource->getAs<types::Int16>());
			*_piRows = _poSource->getAsGenericType()->getRows();
			*_piCols = _poSource->getAsGenericType()->getCols();
			break;
		case types::GenericType::RealUInt16 :
			poResult->getAs<types::UInt16>()->append(iCurRow, iCurCol, _poSource->getAs<types::UInt16>());
			*_piRows = _poSource->getAsGenericType()->getRows();
			*_piCols = _poSource->getAsGenericType()->getCols();
			break;
		case types::GenericType::RealInt32 :
			poResult->getAs<types::Int32>()->append(iCurRow, iCurCol, _poSource->getAs<types::Int32>());
			*_piRows = _poSource->getAsGenericType()->getRows();
			*_piCols = _poSource->getAsGenericType()->getCols();
			break;
		case types::GenericType::RealUInt32 :
			poResult->getAs<types::UInt32>()->append(iCurRow, iCurCol, _poSource->getAs<types::UInt32>());
			*_piRows = _poSource->getAsGenericType()->getRows();
			*_piCols = _poSource->getAsGenericType()->getCols();
			break;
		case types::GenericType::RealInt64 :
			poResult->getAs<types::Int64>()->append(iCurRow, iCurCol, _poSource->getAs<types::Int64>());
			*_piRows = _poSource->getAsGenericType()->getRows();
			*_piCols = _poSource->getAsGenericType()->getCols();
			break;
		case types::GenericType::RealUInt64 :
			poResult->getAs<types::UInt64>()->append(iCurRow, iCurCol, _poSource->getAs<types::UInt64>());
			*_piRows = _poSource->getAsGenericType()->getRows();
			*_piCols = _poSource->getAsGenericType()->getCols();
			break;
		case types::GenericType::RealString :
            poResult->getAs<types::String>()->append(iCurRow, iCurCol, _poSource->getAs<types::String>());
			*_piRows = _poSource->getAsGenericType()->getRows();
			*_piCols = _poSource->getAsGenericType()->getCols();
			break;
		case types::GenericType::RealImplicitList :
			{
				if(_poSource->getAsImplicitList()->getStartType() == types::InternalType::RealPoly)
				{
					poResult->getAsImplicitList()->setStart(_poSource->getAsImplicitList()->getStart());
				}
				else
				{
					poResult->getAsImplicitList()->setStart(_poSource->getAsImplicitList()->getStart());
				}

				if(_poSource->getAsImplicitList()->getStepType() == types::InternalType::RealPoly)
				{
					poResult->getAsImplicitList()->setStep(_poSource->getAsImplicitList()->getStep());
				}
				else
				{
					poResult->getAsImplicitList()->setStep(_poSource->getAsImplicitList()->getStep());
				}

				if(_poSource->getAsImplicitList()->getEndType() == types::InternalType::RealPoly)
				{
					poResult->getAsImplicitList()->setEnd(_poSource->getAsImplicitList()->getEnd());
				}
				else
				{
					poResult->getAsImplicitList()->setEnd(_poSource->getAsImplicitList()->getEnd());
				}
				break;
			}
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

    if(pField)
    {
        return getStructNameFromExp(pField->head_get());
    }
    else if(pVar)
    {
        return &(pVar->name_get().name_get());
    }
    else if(pCall)
    {
        return getStructNameFromExp(&(pCall->name_get()));
    }
    else
    {
        std::wostringstream os;
        os << L"Unknow expression";
        //os << ((Location)e.right_exp_get().location_get()).location_getString() << std::endl;
        throw ScilabError(os.str(), 999, _pExp->location_get());
    }
    return NULL;
}

bool fillStructFromExp(const Exp* _pExp, types::Struct* _pStr, int _iIndex, types::InternalType* _pIT)
{
    const SimpleVar* pVar =  dynamic_cast<const SimpleVar*>(_pExp);
    const CallExp* pCall =  dynamic_cast<const CallExp*>(_pExp);
    const FieldExp* pField =  dynamic_cast<const FieldExp*>(_pExp);

    if(pVar)
    {//x.a = y
        _pStr->addField(pVar->name_get().name_get());
        _pStr->get(_iIndex)->set(pVar->name_get().name_get(), _pIT);
    }
    else if(pCall)
    {
    }
    else
    {
        int a = 1;
    }

    return true;
}

bool getStructFromExp(const Exp* _pExp, types::Struct** _pMain, types::Struct** _pCurrent, typed_list** _pArgs, types::InternalType* _pIT)
{
    const FieldExp* pField      = dynamic_cast<const FieldExp*>(_pExp);
    const SimpleVar* pVar       = dynamic_cast<const SimpleVar*>(_pExp);
    const CallExp* pCall        = dynamic_cast<const CallExp*>(_pExp);
    const CellCallExp* pCell    = dynamic_cast<const CellCallExp*>(_pExp);

    if(pField)
    {//y.x

        //evaluate head "y"
        typed_list *pArgs   = NULL;
        Struct* pMain       = *_pMain;
        Struct* pCurrent    = *_pCurrent;

        bool bOK = getStructFromExp(pField->head_get(), &pMain, &pCurrent, &pArgs, NULL);
        if(bOK)
        {
            pVar    = dynamic_cast<const SimpleVar*>(pField->tail_get());

            //clone _pIT BEFORE addField in case of st.b = st
            types::InternalType* pIT = _pIT ? _pIT->clone() : NULL;
            
            //create field "x"
            std::wstring var = pVar->name_get().name_get();
            bool bOK = pCurrent->addField(pVar->name_get().name_get());
            if(*_pMain == NULL && _pIT != NULL)
            {//first stack, assign value to field and return main structure

                if(pArgs != NULL)
                {//args returned by "parent"
                    std::wstring var = pVar->name_get().name_get();
                    //be careful, extract functions copy values

                    Struct *pStr = pCurrent->extractWithoutClone(pArgs)->getAs<Struct>();
                    pStr->setCloneInCopyValue(false);
                    SingleStruct* pSS = pStr->get(0);
                    pSS->set(pVar->name_get().name_get(), pIT);
                    pSS->IncreaseRef();
                    delete pStr;
                    pSS->DecreaseRef();
                }
                else if(_pArgs == NULL || *_pArgs == NULL)
                {
                    std::wstring var = pVar->name_get().name_get();
                    //std::wcout << var << L" <- " << pIT->getTypeStr() << std::endl;
                    pCurrent->get(0)->set(pVar->name_get().name_get(), pIT);
                }
                else
                {
                    Struct* pStr = new Struct(1,1);
                    std::wstring var = pVar->name_get().name_get();
                    pStr->addField(pVar->name_get().name_get());
                    pStr->get(0)->set(pVar->name_get().name_get(), pIT);
                    pCurrent->insertWithoutClone(*_pArgs, pStr->get(0));
                    delete pStr;
                }
            }
            else
            {//y.x.w
                //in this case, we are in the middle of expression
                //we know that "x" is a struct but we can't assign value yet
                //so assign empty struct and return new pCurrent
                Struct* pStr = NULL;

                /*try to extract field*/
                if(pArgs == NULL)
                {//without extract argument
                    pStr = pCurrent->get(0)->get(pVar->name_get().name_get())->getAs<Struct>();
                }
                else
                {
                    Struct* pStepStr = pCurrent->extractWithoutClone(pArgs)->getAs<Struct>();
                    pStepStr->setCloneInCopyValue(false);
                    SingleStruct* pSS = pStepStr->get(0);
                    pStr = pSS->get(pVar->name_get().name_get())->getAs<Struct>();
                    //we can delete pStepStr without deleted its fields
                    pSS->IncreaseRef();
                    delete pStepStr;
                    pSS->DecreaseRef();
                }

                if(pStr == NULL)
                {//new field or not struct field
                    if(_pArgs == NULL || *_pArgs == NULL)
                    {
                        pStr = new Struct(1,1);
                    }
                    else
                    {
                        Struct* p = new Struct(1,1);
                        pStr = Struct::insertNew(*_pArgs, p)->getAs<Struct>();
                        delete p;
                    }

                    if(pArgs != NULL)
                    {
                        std::wstring var = pVar->name_get().name_get();

                        Struct* pStepStr = pCurrent->extractWithoutClone(pArgs)->getAs<Struct>();
                        pStepStr->setCloneInCopyValue(false);
                        SingleStruct* pSS = pStepStr->get(0);
                        pSS->set(pVar->name_get().name_get(), pStr);
                        pSS->IncreaseRef();
                        delete pStepStr;
                        pSS->DecreaseRef();
                    }
                    else
                    {
                        std::wstring var = pVar->name_get().name_get();
                        pCurrent->get(0)->set(pVar->name_get().name_get(), pStr);
                    }
                }

                pCurrent = pStr;
            }

            *_pMain = pMain;
            *_pCurrent = pCurrent;
            return true;
        }
        else
        {
            return false;
        }
    }
    else if(pVar)
    {//a.x : with x not only a SimpleVar
        types::Struct *pStr = NULL;
        types::InternalType *pIT = symbol::Context::getInstance()->get(pVar->name_get());
        if(pIT == NULL || pIT->isStruct() == false)
        {//"a" doest not exist or it is another type, create it with size 1,1 and return it
            //create new structure variable
            if(_pArgs == NULL || *_pArgs == NULL)
            {
                pStr = new types::Struct(1,1);
            }
            else
            {
                //std::wcout << L"create struct" << std::endl;
                Struct* p = new Struct(1,1);
                pStr = Struct::insertNew(*_pArgs, p)->getAs<Struct>();
                delete p;
            }
            //Add variable to scope
            symbol::Context::getInstance()->put(pVar->name_get(), *pStr);
        }
        else if(pIT->isStruct() == false)
        {
            return false;
        }
        else
        {
            pStr = pIT->getAs<Struct>();
        }

        if(*_pMain == NULL)
        {
            *_pMain = pStr;
        }
        *_pCurrent = pStr;
        return true;
    }
    else if(pCall)
    {//a(x,y)
        ExecVisitor execMe;
        Struct* pCurrent = NULL;

        typed_list *pArgs = execMe.GetArgumentList(pCall->args_get());

        //Struct* pStruct = Struct::insertNew(pArgs, new Struct(1,1))->getAs<Struct>();
        if(*_pMain == NULL)
        {//a is the new main but can be a complex expression
            //bool bOK = getStructFromExp(&pCall->name_get(), _pMain, &pCurrent, &pArgs, pStruct);
            bool bOK = getStructFromExp(&pCall->name_get(), _pMain, &pCurrent, &pArgs, NULL);
            if(bOK == false)
            {
                return false;
            }
        
            /*try to extract sub struct, if it fails, resize the struct and try again*/

            InternalType* pIT = pCurrent->extract(pArgs);
            if(pIT == NULL)
            {//fail to extract, pCurrent is not enough big, resize it !
                Struct* p = new Struct(1,1);
                pCurrent->insert(pArgs, p); //insert empty struct, caller will assign the good value
                delete p;
            }
            else
            {
                delete pIT;
            }
//            *_pCurrent = pIT->getAs<Struct>();
            *_pArgs = pArgs;
            //const SimpleVar *pVar = dynamic_cast<const SimpleVar*>(&pCall->name_get());
            //if(pVar)
            //{
            //    //Add variable to scope
            //    symbol::Context::getInstance()->put(pVar->name_get(), **_pMain);
            //}
            //else
            //{
            //    //heu ...
            //}
        }
        else
        {//we have a parent, so assign "a" to this parent
            //(*_pMain)->set(0, pStruct->get(0));
        }
        *_pCurrent = pCurrent;
        return true;
    }
    else if(pCell)
    {
    }
    else
    {
        std::wostringstream os;
        os << L"Unknown expression";
        //os << ((Location)e.right_exp_get().location_get()).location_getString() << std::endl;
        throw ScilabError(os.str(), 999, _pExp->location_get());
    }
    return false;
}

void callOnPrompt(void)
{
    types::InternalType* pOnPrompt = NULL;
    pOnPrompt = symbol::Context::getInstance()->get(symbol::Symbol(L"%onprompt"));
    if(pOnPrompt != NULL && pOnPrompt->isCallable())
    {
        types::typed_list in;
        types::typed_list out;
        ExecVisitor execCall;
        pOnPrompt->getAs<types::Callable>()->call(in, 1, out, &execCall);
    }
}
