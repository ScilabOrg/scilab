/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

// This code is separated in run_AssignExp.hxx
// but will be inlined in runvisitor.hxx
// using #include with RunVisitorT class declaration.
//
// If you need additionnal headers, please add it in runvisitor.hxx

void visitprivate(const OpExp &e)
{
    try
    {
        /*getting what to assign*/
        e.left_get().accept(*this);
        InternalType *pITL = result_get();
        if(is_single_result() == false)
        {
            std::wostringstream os;
            os << _W("Incompatible output argument.\n");
            //os << ((Location)e.right_get().location_get()).location_getString() << std::endl;
            throw ScilabError(os.str(), 999, e.right_get().location_get());
        }

        /*getting what to assign*/
        e.right_get().accept(*this);
        InternalType *pITR = result_get();
        if(is_single_result() == false)
        {
            std::wostringstream os;
            os << _W("Incompatible output argument.\n");
            //os << ((Location)e.right_get().location_get()).location_getString() << std::endl;
            throw ScilabError(os.str(), 999, e.right_get().location_get());
        }

        GenericType::RealType TypeL = pITL->getType();
        GenericType::RealType TypeR = pITR->getType();

        if(TypeL == GenericType::RealImplicitList)
        {
            ImplicitList* pIL = pITL->getAsImplicitList();
            if(pIL->isComputable())
            {
                pITL = pIL->extractFullMatrix();
                TypeL = pITL->getType();
            }
        }

        if(TypeR == GenericType::RealImplicitList)
        {
            ImplicitList* pIL = pITR->getAsImplicitList();
            if(pIL->isComputable())
            {
                pITR = pIL->extractFullMatrix();
                TypeR = pITR->getType();
            }
        }

        InternalType *pResult   = NULL;

        switch(e.oper_get())
        {
        case OpExp::plus :
            {
                pResult = GenericPlus(pITL, pITR);
                if (pResult == NULL)
                {
                    // We did not have any algorithm matching, so we try to call OverLoad
                    pResult = callOverload(e.oper_get(), pITL, pITR);

                }

                result_set(pResult);
                break;
            }
        case OpExp::minus :
            {
                try
                {
                    pResult = GenericMinus(pITL, pITR);
                }
                catch (ScilabException *pSE)
                {
                    pSE->SetErrorLocation(e.right_get().location_get());
                    throw pSE;
                }

                if (pResult == NULL)
                {
                    // We did not have any algorithm matching, so we try to call OverLoad
                    pResult = callOverload(e.oper_get(), pITL, pITR);

                }

                result_set(pResult);
                break;
            }
        case OpExp::times:
            {
                try
                {
                    pResult = GenericTimes(pITL, pITR);
                }
                catch (ScilabException *pSE)
                {
                    pSE->SetErrorLocation(e.right_get().location_get());
                    throw pSE;
                }

                if (pResult == NULL)
                {
                    // We did not have any algorithm matching, so we try to call OverLoad
                    pResult = callOverload(e.oper_get(), pITL, pITR);

                }
                result_set(pResult);
                break;
            }
        case OpExp::rdivide:
            {
                try
                {
                    pResult = GenericRDivide(pITL, pITR);
                }
                catch (ScilabException *pSE)
                {
                    pSE->SetErrorLocation(e.right_get().location_get());
                    throw pSE;
                }

                if (pResult == NULL)
                {
                    // We did not have any algorithm matching, so we try to call OverLoad
                    pResult = callOverload(e.oper_get(), pITL, pITR);

                }
                result_set(pResult);
                break;
            }
        case OpExp::dotrdivide :
            {
                try
                {
                    pResult = GenericDotRDivide(pITL, pITR);
                }
                catch (ScilabException *pSE)
                {
                    pSE->SetErrorLocation(e.right_get().location_get());
                    throw pSE;
                }

                if (pResult == NULL)
                {
                    // We did not have any algorithm matching, so we try to call OverLoad
                    pResult = callOverload(e.oper_get(), pITL, pITR);
                }
                result_set(pResult);
                break;
            }        
        case OpExp::dottimes :
            {
                try
                {
                    pResult = GenericDotTimes(pITL, pITR);
                }
                catch (ScilabException *pSE)
                {
                    pSE->SetErrorLocation(e.right_get().location_get());
                    throw pSE;
                }

                if (pResult == NULL)
                {
                    // We did not have any algorithm matching, so we try to call OverLoad
                    pResult = callOverload(e.oper_get(), pITL, pITR);

                }
                result_set(pResult);
                break;
            }
        case OpExp::dotpower :
            {
                try
                {
                    pResult = GenericDotPower(pITL, pITR);
                }
                catch (ScilabException *pSE)
                {
                    pSE->SetErrorLocation(e.right_get().location_get());
                    throw pSE;
                }

                if (pResult == NULL)
                {
                    // We did not have any algorithm matching, so we try to call OverLoad
                    pResult = callOverload(e.oper_get(), pITL, pITR);

                }
                result_set(pResult);
                break;
            }
        case OpExp::eq :
            {
              try
                {
                    pResult = GenericComparisonEqual(pITL, pITR);
                }
                catch (ScilabException *pSE)
                {
                    pSE->SetErrorLocation(e.right_get().location_get());
                    throw pSE;
                }

                if (pResult == NULL)
                {
                    // We did not have any algorithm matching, so we try to call OverLoad
                    pResult = callOverload(e.oper_get(), pITL, pITR);
                }
                result_set(pResult);
                break;
            }
        case OpExp::ne :
            {
              try
                {
                    pResult = GenericComparisonNonEqual(pITL, pITR);
                }
                catch (ScilabException *pSE)
                {
                    pSE->SetErrorLocation(e.right_get().location_get());
                    throw pSE;
                }

                if (pResult == NULL)
                {
                    // We did not have any algorithm matching, so we try to call OverLoad
                    pResult = callOverload(e.oper_get(), pITL, pITR);
                }
                result_set(pResult);
                break;
            }
        case OpExp::lt :
            {
                if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
                {
                    Double *pL			= pITL->getAs<Double>();
                    Double *pR			= pITR->getAs<Double>();

                    if(pR->getSize() == 1)
                    {
                        pResult				= new Bool(pL->getRows(), pL->getCols());
                        double dblRef	= pR->getReal(0,0);
                        for(int i = 0 ; i < pL->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pL->getCols() ; j++)
                            {
                                pResult->getAs<types::Bool>()->set(i, j, pL->getReal(i, j) < dblRef);
                            }
                        }
                    }
                    else if(pL->getSize() == 1)
                    {
                        pResult				= new Bool(pR->getRows(), pR->getCols());
                        double dblRef	= pL->getReal(0,0);
                        for(int i = 0 ; i < pR->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pR->getCols() ; j++)
                            {
                                pResult->getAs<types::Bool>()->set(i, j, dblRef < pR->getReal(i, j));
                            }
                        }
                    }
                    else if(pR->getRows() == pL->getRows() && pR->getCols() == pL->getCols())
                    {
                        pResult				= new Bool(pR->getRows(), pR->getCols());
                        for(int i = 0 ; i < pR->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pR->getCols() ; j++)
                            {
                                pResult->getAs<types::Bool>()->set(i, j, pL->getReal(i, j) < pR->getReal(i, j));
                            }
                        }
                    }
                    else
                    {
                        pResult = new Bool(false);
                    }

                    result_set(pResult);
                }
                else
                {
                    result_set(callOverload(e.oper_get(), pITL, pITR));
                }
                break;
            }
        case OpExp::le :
            {
                if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
                {
                    Double *pL			= pITL->getAs<Double>();
                    Double *pR			= pITR->getAs<Double>();

                    if(pR->getSize() == 1)
                    {
                        pResult				= new Bool(pL->getRows(), pL->getCols());
                        double dblRef	= pR->getReal(0,0);
                        for(int i = 0 ; i < pL->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pL->getCols() ; j++)
                            {
                                pResult->getAs<types::Bool>()->set(i, j, pL->getReal(i, j) <= dblRef);
                            }
                        }
                    }
                    else if(pL->getSize() == 1)
                    {
                        pResult				= new Bool(pR->getRows(), pR->getCols());
                        double dblRef	= pL->getReal(0,0);
                        for(int i = 0 ; i < pR->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pR->getCols() ; j++)
                            {
                                pResult->getAs<types::Bool>()->set(i, j, dblRef <= pR->getReal(i, j));
                            }
                        }
                    }
                    else if(pR->getRows() == pL->getRows() && pR->getCols() == pL->getCols())
                    {
                        pResult				= new Bool(pR->getRows(), pR->getCols());
                        for(int i = 0 ; i < pR->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pR->getCols() ; j++)
                            {
                                pResult->getAs<types::Bool>()->set(i, j, pL->getReal(i, j) <= pR->getReal(i, j));
                            }
                        }
                    }
                    else
                    {
                        pResult = new Bool(false);
                    }

                    result_set(pResult);
                }
                else
                {
                    result_set(callOverload(e.oper_get(), pITL, pITR));
                }
                break;
            }
        case OpExp::gt :
            {
                if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
                {
                    Double *pL			= pITL->getAs<Double>();
                    Double *pR			= pITR->getAs<Double>();

                    if(pR->getSize() == 1)
                    {
                        pResult				= new Bool(pL->getRows(), pL->getCols());
                        double dblRef	= pR->getReal(0,0);
                        for(int i = 0 ; i < pL->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pL->getCols() ; j++)
                            {
                                pResult->getAs<types::Bool>()->set(i, j, pL->getReal(i, j) > dblRef);
                            }
                        }
                    }
                    else if(pL->getSize() == 1)
                    {
                        pResult				= new Bool(pR->getRows(), pR->getCols());
                        double dblRef	= pL->getReal(0,0);
                        for(int i = 0 ; i < pR->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pR->getCols() ; j++)
                            {
                                pResult->getAs<types::Bool>()->set(i, j, dblRef > pR->getReal(i, j));
                            }
                        }
                    }
                    else if(pR->getRows() == pL->getRows() && pR->getCols() == pL->getCols())
                    {
                        pResult				= new Bool(pR->getRows(), pR->getCols());

                        for(int i = 0 ; i < pR->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pR->getCols() ; j++)
                            {
                                pResult->getAs<types::Bool>()->set(i, j, pL->getReal(i, j) > pR->getReal(i, j));
                            }
                        }
                    }
                    else
                    {
                        pResult = new Bool(false);
                    }

                    result_set(pResult);
                }
                else
                {
                    result_set(callOverload(e.oper_get(), pITL, pITR));
                }
                break;
            }
        case OpExp::ge :
            {
                if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
                {
                    Double *pL			= pITL->getAs<Double>();
                    Double *pR			= pITR->getAs<Double>();

                    if(pR->getSize() == 1)
                    {
                        pResult				= new Bool(pL->getRows(), pL->getCols());
                        double dblRef	= pR->getReal(0,0);
                        for(int i = 0 ; i < pL->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pL->getCols() ; j++)
                            {
                                pResult->getAs<types::Bool>()->set(i, j, pL->getReal(i, j) >= dblRef);
                            }
                        }
                    }
                    else if(pL->getSize() == 1)
                    {
                        pResult				= new Bool(pR->getRows(), pR->getCols());
                        double dblRef	= pL->getReal(0,0);
                        for(int i = 0 ; i < pR->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pR->getCols() ; j++)
                            {
                                pResult->getAs<types::Bool>()->set(i, j, dblRef >= pR->getReal(i, j));
                            }
                        }
                    }
                    else if(pR->getRows() == pL->getRows() && pR->getCols() == pL->getCols())
                    {
                        pResult				= new Bool(pR->getRows(), pR->getCols());
                        for(int i = 0 ; i < pR->getRows() ; i++)
                        {
                            for(int j = 0 ; j < pR->getCols() ; j++)
                            {
                                pResult->getAs<types::Bool>()->set(i, j, pL->getReal(i, j) >= pR->getReal(i, j));
                            }
                        }
                    }
                    else
                    {
                        pResult = new Bool(false);
                    }

                    result_set(pResult);
                }
                else
                {
                    result_set(callOverload(e.oper_get(), pITL, pITR));
                }
                break;
            }
        case OpExp::power :
            {
                try
                {
                    pResult = GenericPower(pITL, pITR);
                }
                catch (ScilabException *pSE)
                {
                    pSE->SetErrorLocation(e.right_get().location_get());
                    throw pSE;
                }

                if (pResult == NULL)
                {
                    // We did not have any algorithm matching, so we try to call OverLoad
                    pResult = callOverload(e.oper_get(), pITL, pITR);

                }
                result_set(pResult);
                break;
            }
        case OpExp::krontimes :
            {
                try
                {
                    pResult = GenericKrontimes(pITL, pITR);
                }
                catch (ScilabException *pSE)
                {
                    pSE->SetErrorLocation(e.right_get().location_get());
                    throw pSE;
                }

                if (pResult == NULL)
                {
                    // We did not have any algorithm matching, so we try to call OverLoad
                    pResult = callOverload(e.oper_get(), pITL, pITR);

                }
                result_set(pResult);
                break;
            }
        default :
            // By default call overload if we do not know this operator ...
            result_set(callOverload(e.oper_get(), pITL, pITR));
            break;
        }
        //clear left and/or right operands
        if(pITL->isDeletable())
        {
            delete pITL;
        }

        if(pITR->isDeletable())
        {
            delete pITR;
        }

    }
    catch(ScilabError error)
    {
        error.SetErrorLocation(e.location_get());
        throw error;
    }

}

void visitprivate(const LogicalOpExp &e)
{
    e.left_get().accept(*this);
    InternalType *pITL = result_get();
    InternalType *pITR = NULL;

    GenericType::RealType TypeL = pITL->getType();
    
    InternalType *pResult = NULL;

    if(TypeL != GenericType::RealBool)
    {
        //TODO YaSp : Overloading %*_oper_*
        e.right_get().accept(*this);
        pITR = result_get();
        switch(e.oper_get())
        {
        case LogicalOpExp::logicalShortCutOr :
        case LogicalOpExp::logicalOr :
            result_set(callOverload(e.oper_get(), pITL, pITR));
            break;
        case LogicalOpExp::logicalShortCutAnd :
        case LogicalOpExp::logicalAnd :
            result_set(callOverload(e.oper_get(), pITL, pITR));
            break;
        default :
            break;
        }
    }
    else
    {
        Bool *pL = pITL->getAs<types::Bool>();
        switch(e.oper_get())
        {
        case LogicalOpExp::logicalShortCutOr :
            {
                int *piL	= pL->get();
                bool iL     = true;
                // Check if all values are true
                // true || <something is always true>
                for(int i = 0 ; i < pL->getSize() ; i++)
                {
                    if(piL[i] == false)
                    {
                        iL = false;
                        break;
                    }
                }

                if(iL)
                {//we don't need to look at ohers exp
                    result_set(new Bool(true));
                    return;
                }
                // DO NOT break here, falling into normal Or if this can not be shotcutted.
            }
        case LogicalOpExp::logicalOr :
            {
                e.right_get().accept(*this);
                pITR = result_get();
                GenericType::RealType TypeR = pITR->getType();

                if(TypeR == GenericType::RealBool)
                {
                    Bool *pR = pITR->getAs<types::Bool>();
                    int* piR = pR->get();
                    int* piL = pL->get();
                    int* piB = NULL;

                    // M | scalar
                    if(pR->getSize() == 1)
                    {
                        pResult = new Bool(pL->getRows(), pL->getCols(), &piB);
                        for(int i = 0 ; i < pL->getSize(); i++)
                        {
                            piB[i] = (piR[0] == 1) || (piL[i] == 1);
                        }
                    }
                    else if(pL->getSize() == 1)
                    {// scalar | M
                        pResult = new Bool(pR->getRows(), pR->getCols(), &piB);
                        for(int i = 0 ; i < pR->getSize(); i++)
                        {
                            piB[i] = (piR[i] == 1) || (piL[0] == 1);
                        }
                    }
                    else if(pR->getRows() == pL->getRows() && pR->getCols() == pL->getCols())
                    {// M | N (generic case)
                        pResult = new Bool(pR->getRows(), pR->getCols(), &piB);
                        for(int i = 0 ; i < pL->getSize(); i++)
                        {
                            piB[i] = (piR[i] == 1) || (piL[i] == 1);
                        }
                    }
                    else
                    {
                        std::wostringstream os;
                        os << _W("Inconsistent row/column dimensions.\n");
                        //os << ((Location)e.right_get().location_get()).location_getString() << std::endl;
                        throw ScilabError(os.str(), 999, e.right_get().location_get());
                    }
                }
                else
                {
                    pResult = callOverload(e.oper_get(), pITL, pITR);
                }
                break;
            }
        case LogicalOpExp::logicalShortCutAnd :
            {
                int *piL	= pL->get();
                // Check if one value is false
                // false && <something> is always false
                for(int i = 0 ; i < pL->getSize() ; i++)
                {
                    if(piL[i] == false)
                    {
                        result_set(new Bool(false));
                        return;
                    }
                }
                // DO NOT break here, falling into normal And if this can not be shotcutted.
            }
        case LogicalOpExp::logicalAnd :
            {
                e.right_get().accept(*this);
                pITR = result_get();
                GenericType::RealType TypeR = pITR->getType();

                if(TypeR == GenericType::RealBool)
                {
                    Bool *pR = pITR->getAs<types::Bool>();
                    int* piR = pR->get();
                    int* piL = pL->get();
                    int* piB = NULL;

                    // M & scalar
                    if(pR->getSize() == 1)
                    {
                        pResult = new Bool(pL->getRows(), pL->getCols(), &piB);
                        for(int i = 0 ; i < pL->getSize(); i++)
                        {
                            piB[i] = (piR[0] == 1) && (piL[i] == 1);
                        }
                    }
                    else if(pL->getSize() == 1)
                    {// scalar & M
                        pResult = new Bool(pR->getRows(), pR->getCols(), &piB);
                        for(int i = 0 ; i < pR->getSize(); i++)
                        {
                            piB[i] = (piR[i] == 1) && (piL[0] == 1);
                        }
                    }
                    else if(pR->getRows() == pL->getRows() && pR->getCols() == pL->getCols())
                    {// M & N (generic case)
                        pResult = new Bool(pR->getRows(), pR->getCols(), &piB);
                        for(int i = 0 ; i < pL->getSize(); i++)
                        {
                            piB[i] = (piR[i] == 1) && (piL[i] == 1);
                        }
                    }
                    else
                    {
                        std::wostringstream os;
                        os << _W("Inconsistent row/column dimensions.\n");
                        //os << ((Location)e.right_get().location_get()).location_getString() << std::endl;
                        throw ScilabError(os.str(), 999, e.right_get().location_get());
                    }
                }
                else
                {
                    pResult = callOverload(e.oper_get(), pITL, pITR);
                }
                break;
            }

        default :
            {
                pResult = callOverload(e.oper_get(), pITL, pITR);
                break;
            }
        }
    }

    if(pITL && pITL->isDeletable())
    {
        delete pITL;
    }

    if(pITR && pITR->isDeletable())
    {
        delete pITR;
    }

    result_set(pResult);
}

InternalType* callOverload(OpExp::Oper _oper, InternalType* _paramL, InternalType* _paramR)
{
    types::typed_list in;
    types::typed_list out;
    _paramL->IncreaseRef();
    _paramR->IncreaseRef();
    in.push_back(_paramL);
    in.push_back(_paramR);

    Overload::generateNameAndCall(Overload::getNameFromOper(_oper), in, 1, out, this);

    _paramL->DecreaseRef();
    _paramR->DecreaseRef();
    return out[0];
}
