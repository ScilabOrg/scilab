/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

//file included in runvisitor.cpp
namespace ast {

template<class T>
void RunVisitorT<T>::visitprivate(const OpExp &e)
{
    InternalType * pITL = NULL, * pITR = NULL, * pResult = NULL;
    try
    {
        /*getting what to assign*/
        e.getLeft().accept(*this);
        if (isSingleResult() == false)
        {
            clearResult();
            std::wostringstream os;
            os << _W("Incompatible output argument.\n");
            //os << ((Location)e.right_get().getLocation()).getLocationString() << std::endl;
            throw ast::ScilabError(os.str(), 999, e.getRight().getLocation());
        }
        pITL = getResult();

        /*getting what to assign*/
        e.getRight().accept(*this);
        if (isSingleResult() == false)
        {
            clearResult();
            std::wostringstream os;
            os << _W("Incompatible output argument.\n");
            //os << ((Location)e.right_get().getLocation()).getLocationString() << std::endl;
            throw ast::ScilabError(os.str(), 999, e.getRight().getLocation());
        }
        pITR = getResult();

        if (pITL->getType() == GenericType::ScilabImplicitList)
        {
            ImplicitList* pIL = pITL->getAs<ImplicitList>();
            if (pIL->isComputable())
            {
                pITL = pIL->extractFullMatrix();
                pIL->killMe();
            }
        }

        if (pITR->getType() == GenericType::ScilabImplicitList)
        {
            ImplicitList* pIR = pITR->getAs<ImplicitList>();
            if (pIR->isComputable())
            {
                pITR = pIR->extractFullMatrix();
                pIR->killMe();
            }
        }

        switch (e.getOper())
        {
            case OpExp::plus :
            {
                pResult = GenericPlus(pITL, pITR);
                break;
            }
            case OpExp::unaryMinus :
            {
                pResult = GenericUnaryMinus(pITR);
                break;
            }
            case OpExp::minus :
            {
                pResult = GenericMinus(pITL, pITR);
                break;
            }
            case OpExp::times:
            {
                pResult = GenericTimes(pITL, pITR);
                break;
            }
            case OpExp::ldivide:
            {
                pResult = GenericLDivide(pITL, pITR);
                break;
            }
            case OpExp::dotldivide :
            {
                pResult = GenericDotLDivide(pITL, pITR);
                break;
            }
            case OpExp::rdivide:
            {
                pResult = GenericRDivide(pITL, pITR);
                break;
            }
            case OpExp::dotrdivide :
            {
                pResult = GenericDotRDivide(pITL, pITR);
                break;
            }
            case OpExp::dottimes :
            {
                pResult = GenericDotTimes(pITL, pITR);
                break;
            }
            case OpExp::dotpower :
            {
                pResult = GenericDotPower(pITL, pITR);
                break;
            }
            case OpExp::eq :
            {
                pResult = GenericComparisonEqual(pITL, pITR);
                break;
            }
            case OpExp::ne :
            {
                pResult = GenericComparisonNonEqual(pITL, pITR);
                break;
            }
            case OpExp::lt :
            {
                pResult = GenericLess(pITL, pITR);
                break;
            }
            case OpExp::le :
            {
                pResult = GenericLessEqual(pITL, pITR);
                break;
            }
            case OpExp::gt :
            {
                pResult = GenericGreater(pITL, pITR);
                break;
            }
            case OpExp::ge :
            {
                pResult = GenericGreaterEqual(pITL, pITR);
                break;
            }
            case OpExp::power :
            {
                pResult = GenericPower(pITL, pITR);
                break;
            }
            case OpExp::krontimes :
            {
                pResult = GenericKrontimes(pITL, pITR);
                break;
            }
            case OpExp::kronrdivide :
            {
                pResult = GenericKronrdivide(pITL, pITR);
                break;
            }
            case OpExp::kronldivide :
            {
                pResult = GenericKronldivide(pITL, pITR);
                break;
            }
            default :
                break;
        }

        //overloading
        if (pResult == NULL)
        {
            // We did not have any algorithm matching, so we try to call OverLoad
            pResult = callOverloadOpExp(e.getOper(), pITL, pITR);
        }

        setResult(pResult);

        //clear left and/or right operands
        if (pResult != pITL)
        {
            pITL->killMe();
        }

        if (pResult != pITR)
        {
            pITR->killMe();
        }
    }
    catch (ast::ScilabError & error)
    {
        setResult(NULL);
        if (pResult)
        {
            pResult->killMe();
        }
        if (pITL && (pITL != pResult))
        {
            pITL->killMe();
        }
        if (pITR && (pITR != pResult))
        {
            pITR->killMe();
        }

        error.SetErrorLocation(e.getLocation());
        throw error;
    }

    if (e.getDecorator().res.isConstant())
    {

    }
}

template<class T>
void RunVisitorT<T>::visitprivate(const LogicalOpExp &e)
{
    try
    {
        InternalType *pITR = NULL; //assign only in non shortcut operations.

        /*getting what to assign*/
        e.getLeft().accept(*this);
        InternalType *pITL = getResult();
        if (isSingleResult() == false)
        {
            std::wostringstream os;
            os << _W("Incompatible output argument.\n");
            //os << ((Location)e.right_get().getLocation()).getLocationString() << std::endl;
            throw ast::ScilabError(os.str(), 999, e.getRight().getLocation());
        }

        setResult(NULL);

        if (pITL->getType() == GenericType::ScilabImplicitList)
        {
            ImplicitList* pIL = pITL->getAs<ImplicitList>();
            if (pIL->isComputable())
            {
                pITL = pIL->extractFullMatrix();
                pIL->killMe();
            }
        }

        InternalType *pResult   = NULL;

        switch (e.getOper())
        {
            case LogicalOpExp::logicalShortCutAnd :
            {
                pResult = GenericShortcutAnd(pITL);
                if (pResult)
                {
                    break;
                }

                //Continue to logicalAnd
            }
            case LogicalOpExp::logicalAnd :
            {
                /*getting what to assign*/
                e.getRight().accept(*this);
                pITR = getResult();
                if (isSingleResult() == false)
                {
                    std::wostringstream os;
                    os << _W("Incompatible output argument.\n");
                    //os << ((Location)e.right_get().getLocation()).getLocationString() << std::endl;
                    throw ast::ScilabError(os.str(), 999, e.getRight().getLocation());
                }

                if (pITR->getType() == GenericType::ScilabImplicitList)
                {
                    ImplicitList* pIR = pITR->getAs<ImplicitList>();
                    if (pIR->isComputable())
                    {
                        pITR = pIR->extractFullMatrix();
                        pIR->killMe();
                    }
                }
                pResult = GenericLogicalAnd(pITL, pITR);
                break;
            }
            case LogicalOpExp::logicalShortCutOr :
            {
                pResult = GenericShortcutOr(pITL);
                if (pResult)
                {
                    break;
                }

                //Continue to logicalAnd
            }
            case LogicalOpExp::logicalOr :
            {
                /*getting what to assign*/
                e.getRight().accept(*this);
                pITR = getResult();
                if (isSingleResult() == false)
                {
                    std::wostringstream os;
                    os << _W("Incompatible output argument.\n");
                    //os << ((Location)e.right_get().getLocation()).getLocationString() << std::endl;
                    throw ast::ScilabError(os.str(), 999, e.getRight().getLocation());
                }

                if (pITR->getType() == GenericType::ScilabImplicitList)
                {
                    ImplicitList* pIR = pITR->getAs<ImplicitList>();
                    if (pIR->isComputable())
                    {
                        pITR = pIR->extractFullMatrix();
                    }
                }
                pResult = GenericLogicalOr(pITL, pITR);
                break;
            }

            default :
                break;
        }
        //overloading
        if (pResult == NULL)
        {
            // We did not have any algorithm matching, so we try to call OverLoad
            pResult = callOverloadOpExp(e.getOper(), pITL, pITR);
        }

        setResult(pResult);

        // protect pResult in case where pITL or pITR equal pResult
        pResult->IncreaseRef();

        //clear left and/or right operands
        pITL->killMe();
        if (pITR)
        {
            pITR->killMe();
        }

        // unprotect pResult
        pResult->DecreaseRef();
    }
    catch (ast::ScilabError error)
    {
        clearResult();
        error.SetErrorLocation(e.getLocation());
        throw error;
    }

}

template<class T>
types::InternalType* RunVisitorT<T>::callOverloadOpExp(OpExp::Oper _oper, types::InternalType* _paramL, types::InternalType* _paramR)
{
    types::typed_list in;
    types::typed_list out;

    /*
    ** Special case for unary minus => will call %{type_s}
    */
    if (_oper == OpExp::unaryMinus)
    {
        _paramR->IncreaseRef();
        in.push_back(_paramR);
        try
        {
            Overload::generateNameAndCall(Overload::getNameFromOper(_oper), in, 1, out, this, true);
        }
        catch (ast::ScilabError e)
        {
            _paramR->DecreaseRef();
            throw e;
        }

        _paramR->DecreaseRef();
        return out[0];
    }

    _paramL->IncreaseRef();
    _paramR->IncreaseRef();
    in.push_back(_paramL);
    in.push_back(_paramR);

    try
    {
        Overload::generateNameAndCall(Overload::getNameFromOper(_oper), in, 1, out, this, true);
    }
    catch (ast::ScilabError e)
    {
        _paramL->DecreaseRef();
        _paramR->DecreaseRef();
        throw e;
    }

    _paramL->DecreaseRef();
    _paramR->DecreaseRef();
    return out[0];
}

} /* namespace ast */
