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
    T execMeL;
    T execMeR;

    try
    {

        /*getting what to assign*/
        e.left_get().accept(execMeL);
        /*getting what to assign*/
        e.right_get().accept(execMeR);

        InternalType *pITL          = execMeL.result_get();
        InternalType *pITR          = execMeR.result_get();
        GenericType::RealType TypeL = pITL->getType();
        GenericType::RealType TypeR = pITR->getType();

        if(TypeL == GenericType::RealImplicitList)
        {
            ImplicitList* pIL = pITL->getAsImplicitList();
            if(pIL->computable())
            {
                InternalType *pIT = pIL->extract_matrix();
                execMeL.result_set(pIT);
                TypeL = pIT->getType();
            }
        }

        if(TypeR == GenericType::RealImplicitList)
        {
            ImplicitList* pIL = pITR->getAsImplicitList();
            if(pIL->computable())
            {
                InternalType *pIT = pIL->extract_matrix();
                execMeR.result_set(pIT);
                TypeR = pIT->getType();
            }
        }

        InternalType *pResult   = NULL;
        
        switch(e.oper_get())
        {
        case OpExp::plus :
            {
                try
                {
                    pResult = GenericPlus(pITL, pITR);
                }
                catch (ScilabException *pSE)
                {
                    pSE->SetErrorLocation(e.right_get().location_get());
                    throw pSE;
                }

                if (pResult == NULL)
                {
                    // We did not have any algorithm matching, so we try to call OverLoad
                    pResult = callOverload(e.oper_get(), &execMeL, &execMeR);

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
                    pResult = callOverload(e.oper_get(), &execMeL, &execMeR);

                }

                result_set(pResult);
                break;
            }
        case OpExp::times:
              {
                if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
                {
                    Double *pL			= pITL->getAs<Double>();
                    Double *pR			= pITR->getAs<Double>();

                    int iResult = MultiplyDoubleByDouble(pL, pR, (Double**)&pResult);
                    if(iResult)
                    {
                        std::wostringstream os;
                        os << _W("Inconsistent row/column dimensions.\n");
                        //os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                        throw ScilabError(os.str(), 999, e.right_get().location_get());
                    }

                    result_set(pResult);
                }
                else if(TypeL == InternalType::RealDouble && TypeR == InternalType::RealPoly)
                {
                    Double *pL      = pITL->getAs<Double>();
                    MatrixPoly *pR  = pITR->getAsPoly();

                    int iResult = MultiplyDoubleByPoly(pL, pR, (MatrixPoly**)&pResult);

                    if(iResult)
                    {
                        std::wostringstream os;
                        os << _W("Inconsistent row/column dimensions.\n");
                        //os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                        throw ScilabError(os.str(), 999, e.right_get().location_get());
                    }

                    result_set(pResult);
                }
                else if(TypeL == InternalType::RealPoly && TypeR == InternalType::RealDouble)
                {
                    MatrixPoly *pL	    = pITL->getAsPoly();
                    Double *pR			    = pITR->getAs<Double>();

                    int iResult = MultiplyPolyByDouble(pL, pR, (MatrixPoly**)&pResult);

                    if(iResult)
                    {
                        std::wostringstream os;
                        os << _W("Inconsistent row/column dimensions.\n");
                        //os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                        throw ScilabError(os.str(), 999, e.right_get().location_get());
                    }

                    result_set(pResult);
                }
                else if(TypeL == InternalType::RealPoly && TypeR == InternalType::RealPoly)
                {
                    MatrixPoly *pL	    = pITL->getAsPoly();
                    MatrixPoly *pR			= pITR->getAsPoly();

                    int iResult = MultiplyPolyByPoly(pL, pR, (MatrixPoly**)&pResult);

                    if(iResult)
                    {
                        std::wostringstream os;
                        os << _W("Inconsistent row/column dimensions.\n");
                        //os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                        throw ScilabError(os.str(), 999, e.right_get().location_get());
                    }

                    result_set(pResult);
                }
                else
                {
                    result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
                }
                break;
            }
        case OpExp::rdivide:
            {
                if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
                {
                    Double *pL			= pITL->getAs<Double>();
                    Double *pR			= pITR->getAs<Double>();

                    int iResult = DivideDoubleByDouble(pL, pR, (Double**)&pResult);
                    if(iResult)
                    {//manage errors
                        std::wostringstream os;
                        os << _W("Inconsistent row/column dimensions.\n");
                        //os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                        throw ScilabError(os.str(), 999, e.right_get().location_get());
                    }
                    result_set(pResult);
                }
                else if(TypeL == GenericType::RealPoly && TypeR == GenericType::RealDouble)
                {
                    MatrixPoly *pL	= pITL->getAsPoly();
                    Double *pR			= pITR->getAs<Double>();

                    int iResult = DividePolyByDouble(pL, pR, (MatrixPoly**)&pResult);
                    if(iResult)
                    {//manage errors
                        std::wostringstream os;
                        os << _W("Inconsistent row/column dimensions.\n");
                        //os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                        throw ScilabError(os.str(), 999, e.right_get().location_get());
                    }

                    result_set(pResult);
                }
                else if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealPoly)
                {
                    Double *pL			= pITL->getAs<Double>();
                    MatrixPoly *pR	= pITR->getAsPoly();

                    int iResult = DivideDoubleByPoly(pL, pR, (MatrixPoly**)&pResult);
                    if(iResult)
                    {//manage errors
                        std::wostringstream os;
                        os << _W("Inconsistent row/column dimensions.\n");
                        //os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                        throw ScilabError(os.str(), 999, e.right_get().location_get());
                    }
                    result_set(pResult);
                }
                else
                {
                    result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
                }
                break;
            }
        case OpExp::dottimes :
            {
                if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
                {
                    Double *pL			= pITL->getAs<Double>();
                    Double *pR			= pITR->getAs<Double>();


                    int iResult = DotMultiplyDoubleByDouble(pL, pR, (Double**)&pResult);
                    if(iResult)
                    {
                        std::wostringstream os;
                        os << _W("Inconsistent row/column dimensions.\n");
                        //os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                        throw ScilabError(os.str(), 999, e.right_get().location_get());
                    }

                    result_set(pResult);
                }
                else
                {
                    result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
                }
                break;
            }
        case OpExp::eq :
            {
                if(TypeL == GenericType::RealDouble && pITL->getAs<Double>()->size_get() == 0)
                {//[] == xx
                    if(TypeR != InternalType::RealDouble)
                    {
                        result_set(new Bool(false));
                        return;
                    }
                }

                if(TypeR == GenericType::RealDouble && pITR->getAs<Double>()->size_get() == 0)
                {//xx == []
                    if(TypeL != InternalType::RealDouble)
                    {
                        result_set(new Bool(false));
                        return;
                    }
                }

                if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
                {
                    Double *pL			= pITL->getAs<Double>();
                    Double *pR			= pITR->getAs<Double>();

                    if(pR->size_get() == 0 && pL->size_get() == 0)
                    {
                        pResult = new Bool(true);
                    }
                    else if(pL->size_get() == 0  || pR->size_get() == 0)
                    {
                        pResult = new Bool(false);
                    }
                    else if(pR->size_get() == 1)
                    {
                        pResult				= new Bool(pL->rows_get(), pL->cols_get());
                        double dblRef	= pR->get_real(0,0);
                        for(int i = 0 ; i < pL->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pL->cols_get() ; j++)
                            {
                                pResult->getAsBool()->bool_set(i, j, pL->get_real(i, j) == dblRef);
                            }
                        }
                    }
                    else if(pL->size_get() == 1)
                    {
                        pResult				= new Bool(pR->rows_get(), pR->cols_get());
                        double dblRef	= pL->get_real(0,0);
                        for(int i = 0 ; i < pR->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pR->cols_get() ; j++)
                            {
                                pResult->getAsBool()->bool_set(i, j, dblRef == pR->get_real(i, j));
                            }
                        }
                    }
                    else if(pR->rows_get() == pL->rows_get() && pR->cols_get() == pL->cols_get())
                    {
                        pResult				= new Bool(pR->rows_get(), pR->cols_get());
                        for(int i = 0 ; i < pR->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pR->cols_get() ; j++)
                            {
                                pResult->getAsBool()->bool_set(i, j, pL->get_real(i, j) == pR->get_real(i, j));
                            }
                        }
                    }
                    else
                    {
                        pResult = new Bool(false);
                    }

                    result_set(pResult);
                }
                else if(TypeL == GenericType::RealString && TypeR == GenericType::RealString)
                {
                    String *pL			= pITL->getAsString();
                    String *pR			= pITR->getAsString();

                    if(pL->size_get() == 1)
                    {
                        pResult = new Bool(pR->rows_get(), pR->cols_get());

                        wchar_t* pstL = pL->string_get()[0];
                        for(int i = 0 ; i < pR->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pR->cols_get() ; j++)
                            {
                                wchar_t* pstR = pR->string_get(i,j);
                                if(wcscmp(pstL, pstR) == 0)
                                {
                                    pResult->getAsBool()->bool_set(i,j,true);
                                }
                                else
                                {
                                    pResult->getAsBool()->bool_set(i,j,false);
                                }
                            }
                        }
                    }
                    else if(pR->size_get() == 1)
                    {
                        pResult = new Bool(pL->rows_get(), pL->cols_get());

                        wchar_t* pstR = pR->string_get()[0];
                        for(int i = 0 ; i < pL->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pL->cols_get() ; j++)
                            {
                                wchar_t* pstL = pL->string_get(i,j);
                                if(wcscmp(pstL, pstR) == 0)
                                {
                                    pResult->getAsBool()->bool_set(i,j,true);
                                }
                                else
                                {
                                    pResult->getAsBool()->bool_set(i,j,false);
                                }
                            }
                        }
                    }
                    else if(pL->rows_get() == pR->rows_get() && pL->cols_get() == pR->cols_get())
                    {
                        pResult = new Bool(pL->rows_get(), pL->cols_get());

                        for(int i = 0 ; i < pL->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pL->cols_get() ; j++)
                            {
                                wchar_t* pstR = pR->string_get(i,j);
                                wchar_t* pstL = pL->string_get(i,j);
                                if(wcscmp(pstL, pstR) == 0)
                                {
                                    pResult->getAsBool()->bool_set(i,j,true);
                                }
                                else
                                {
                                    pResult->getAsBool()->bool_set(i,j,false);
                                }
                            }
                        }
                    }
                    else
                    {
                        pResult = new Bool(false);
                    }
                    result_set(pResult);
                }
                else if(TypeL == GenericType::RealBool && TypeR == GenericType::RealBool)
                {
                    Bool *pL			= pITL->getAsBool();
                    Bool *pR			= pITR->getAsBool();

                    if(pL->size_get() == 1)
                    {
                        pResult = new Bool(pR->rows_get(), pR->cols_get());

                        int iL = pL->bool_get()[0];
                        for(int i = 0 ; i < pR->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pR->cols_get() ; j++)
                            {
                                if(iL == pR->bool_get(i,j))
                                {
                                    pResult->getAsBool()->bool_set(i,j,true);
                                }
                                else
                                {
                                    pResult->getAsBool()->bool_set(i,j,false);
                                }
                            }
                        }
                    }
                    else if(pR->size_get() == 1)
                    {
                        pResult = new Bool(pL->rows_get(), pL->cols_get());

                        int iR = pR->bool_get()[0];
                        for(int i = 0 ; i < pL->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pL->cols_get() ; j++)
                            {
                                if(iR == pL->bool_get(i,j))
                                {
                                    pResult->getAsBool()->bool_set(i,j,true);
                                }
                                else
                                {
                                    pResult->getAsBool()->bool_set(i,j,false);
                                }
                            }
                        }
                    }
                    else if(pL->rows_get() == pR->rows_get() && pL->cols_get() == pR->cols_get())
                    {
                        pResult = new Bool(pL->rows_get(), pL->cols_get());

                        for(int i = 0 ; i < pL->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pL->cols_get() ; j++)
                            {
                                if(pL->bool_get(i,j) == pR->bool_get(i,j))
                                {
                                    pResult->getAsBool()->bool_set(i,j,true);
                                }
                                else
                                {
                                    pResult->getAsBool()->bool_set(i,j,false);
                                }
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
                    result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
                }
                break;
            }
        case OpExp::ne :
            {
                if(TypeL == GenericType::RealDouble && pITL->getAs<Double>()->size_get() == 0)
                {//[] <> xx
                    if(TypeR != InternalType::RealDouble)
                    {
                        result_set(new Bool(true));
                        return;
                    }
                }

                if(TypeR == GenericType::RealDouble && pITR->getAs<Double>()->size_get() == 0)
                {//xx <> []
                    if(TypeL != InternalType::RealDouble)
                    {
                        result_set(new Bool(true));
                        return;
                    }
                }

                if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
                {
                    Double *pL			= pITL->getAs<Double>();
                    Double *pR			= pITR->getAs<Double>();

                    if(pR->size_get() == 0 && pL->size_get() == 0)
                    {
                        pResult = new Bool(false);
                    }
                    else if(pL->size_get() == 0  || pR->size_get() == 0)
                    {
                        pResult = new Bool(true);
                    }
                    else if(pR->size_get() == 1)
                    {
                        pResult				= new Bool(pL->rows_get(), pL->cols_get());
                        double dblRef	= pR->get_real(0,0);
                        for(int i = 0 ; i < pL->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pL->cols_get() ; j++)
                            {
                                pResult->getAsBool()->bool_set(i, j, pL->get_real(i, j) != dblRef);
                            }
                        }
                    }
                    else if(pL->size_get() == 1)
                    {
                        pResult				= new Bool(pR->rows_get(), pR->cols_get());
                        double dblRef	= pL->get_real(0,0);
                        for(int i = 0 ; i < pR->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pR->cols_get() ; j++)
                            {
                                pResult->getAsBool()->bool_set(i, j, dblRef != pR->get_real(i, j));
                            }
                        }
                    }
                    else if(pR->rows_get() == pL->rows_get() && pR->cols_get() == pL->cols_get())
                    {
                        pResult				= new Bool(pR->rows_get(), pR->cols_get());
                        for(int i = 0 ; i < pR->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pR->cols_get() ; j++)
                            {
                                pResult->getAsBool()->bool_set(i, j, pL->get_real(i, j) != pR->get_real(i, j));
                            }
                        }
                    }
                    else
                    {
                        pResult = new Bool(false);
                    }

                    result_set(pResult);
                }
                else if(TypeL == GenericType::RealString && TypeR == GenericType::RealString)
                {
                    String *pL			= pITL->getAsString();
                    String *pR			= pITR->getAsString();

                    if(pL->size_get() == 1)
                    {
                        pResult = new Bool(pR->rows_get(), pR->cols_get());

                        wchar_t* pstL = pL->string_get()[0];
                        for(int i = 0 ; i < pR->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pR->cols_get() ; j++)
                            {
                                wchar_t* pstR = pR->string_get(i,j);
                                if(wcscmp(pstL, pstR) == 0)
                                {
                                    pResult->getAsBool()->bool_set(i,j,false);
                                }
                                else
                                {
                                    pResult->getAsBool()->bool_set(i,j,true);
                                }
                            }
                        }
                    }
                    else if(pR->size_get() == 1)
                    {
                        pResult = new Bool(pL->rows_get(), pL->cols_get());

                        wchar_t* pstR = pR->string_get()[0];
                        for(int i = 0 ; i < pL->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pL->cols_get() ; j++)
                            {
                                wchar_t* pstL = pL->string_get(i,j);
                                if(wcscmp(pstL, pstR) == 0)
                                {
                                    pResult->getAsBool()->bool_set(i,j,false);
                                }
                                else
                                {
                                    pResult->getAsBool()->bool_set(i,j,true);
                                }
                            }
                        }
                    }
                    else if(pL->rows_get() == pR->rows_get() && pL->cols_get() == pR->cols_get())
                    {
                        pResult = new Bool(pL->rows_get(), pL->cols_get());

                        for(int i = 0 ; i < pL->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pL->cols_get() ; j++)
                            {
                                wchar_t* pstR = pR->string_get(i,j);
                                wchar_t* pstL = pL->string_get(i,j);
                                if(wcscmp(pstL, pstR) == 0)
                                {
                                    pResult->getAsBool()->bool_set(i,j,false);
                                }
                                else
                                {
                                    pResult->getAsBool()->bool_set(i,j,true);
                                }
                            }
                        }
                    }
                    else
                    {
                        pResult = new Bool(true);
                    }
                    result_set(pResult);
                }
                else
                {
                    result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
                }
                break;
            }
        case OpExp::lt :
            {
                if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
                {
                    Double *pL			= pITL->getAs<Double>();
                    Double *pR			= pITR->getAs<Double>();

                    if(pR->size_get() == 1)
                    {
                        pResult				= new Bool(pL->rows_get(), pL->cols_get());
                        double dblRef	= pR->get_real(0,0);
                        for(int i = 0 ; i < pL->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pL->cols_get() ; j++)
                            {
                                pResult->getAsBool()->bool_set(i, j, pL->get_real(i, j) < dblRef);
                            }
                        }
                    }
                    else if(pL->size_get() == 1)
                    {
                        pResult				= new Bool(pR->rows_get(), pR->cols_get());
                        double dblRef	= pL->get_real(0,0);
                        for(int i = 0 ; i < pR->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pR->cols_get() ; j++)
                            {
                                pResult->getAsBool()->bool_set(i, j, dblRef < pR->get_real(i, j));
                            }
                        }
                    }
                    else if(pR->rows_get() == pL->rows_get() && pR->cols_get() == pL->cols_get())
                    {
                        pResult				= new Bool(pR->rows_get(), pR->cols_get());
                        for(int i = 0 ; i < pR->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pR->cols_get() ; j++)
                            {
                                pResult->getAsBool()->bool_set(i, j, pL->get_real(i, j) < pR->get_real(i, j));
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
                    result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
                }
                break;
            }
        case OpExp::le :
            {
                if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
                {
                    Double *pL			= pITL->getAs<Double>();
                    Double *pR			= pITR->getAs<Double>();

                    if(pR->size_get() == 1)
                    {
                        pResult				= new Bool(pL->rows_get(), pL->cols_get());
                        double dblRef	= pR->get_real(0,0);
                        for(int i = 0 ; i < pL->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pL->cols_get() ; j++)
                            {
                                pResult->getAsBool()->bool_set(i, j, pL->get_real(i, j) <= dblRef);
                            }
                        }
                    }
                    else if(pL->size_get() == 1)
                    {
                        pResult				= new Bool(pR->rows_get(), pR->cols_get());
                        double dblRef	= pL->get_real(0,0);
                        for(int i = 0 ; i < pR->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pR->cols_get() ; j++)
                            {
                                pResult->getAsBool()->bool_set(i, j, dblRef <= pR->get_real(i, j));
                            }
                        }
                    }
                    else if(pR->rows_get() == pL->rows_get() && pR->cols_get() == pL->cols_get())
                    {
                        pResult				= new Bool(pR->rows_get(), pR->cols_get());
                        for(int i = 0 ; i < pR->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pR->cols_get() ; j++)
                            {
                                pResult->getAsBool()->bool_set(i, j, pL->get_real(i, j) <= pR->get_real(i, j));
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
                    result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
                }
                break;
            }
        case OpExp::gt :
            {
                if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
                {
                    Double *pL			= pITL->getAs<Double>();
                    Double *pR			= pITR->getAs<Double>();

                    if(pR->size_get() == 1)
                    {
                        pResult				= new Bool(pL->rows_get(), pL->cols_get());
                        double dblRef	= pR->get_real(0,0);
                        for(int i = 0 ; i < pL->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pL->cols_get() ; j++)
                            {
                                pResult->getAsBool()->bool_set(i, j, pL->get_real(i, j) > dblRef);
                            }
                        }
                    }
                    else if(pL->size_get() == 1)
                    {
                        pResult				= new Bool(pR->rows_get(), pR->cols_get());
                        double dblRef	= pL->get_real(0,0);
                        for(int i = 0 ; i < pR->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pR->cols_get() ; j++)
                            {
                                pResult->getAsBool()->bool_set(i, j, dblRef > pR->get_real(i, j));
                            }
                        }
                    }
                    else if(pR->rows_get() == pL->rows_get() && pR->cols_get() == pL->cols_get())
                    {
                        pResult				= new Bool(pR->rows_get(), pR->cols_get());

                        for(int i = 0 ; i < pR->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pR->cols_get() ; j++)
                            {
                                pResult->getAsBool()->bool_set(i, j, pL->get_real(i, j) > pR->get_real(i, j));
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
                    result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
                }
                break;
            }
        case OpExp::ge :
            {
                if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
                {
                    Double *pL			= pITL->getAs<Double>();
                    Double *pR			= pITR->getAs<Double>();

                    if(pR->size_get() == 1)
                    {
                        pResult				= new Bool(pL->rows_get(), pL->cols_get());
                        double dblRef	= pR->get_real(0,0);
                        for(int i = 0 ; i < pL->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pL->cols_get() ; j++)
                            {
                                pResult->getAsBool()->bool_set(i, j, pL->get_real(i, j) >= dblRef);
                            }
                        }
                    }
                    else if(pL->size_get() == 1)
                    {
                        pResult				= new Bool(pR->rows_get(), pR->cols_get());
                        double dblRef	= pL->get_real(0,0);
                        for(int i = 0 ; i < pR->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pR->cols_get() ; j++)
                            {
                                pResult->getAsBool()->bool_set(i, j, dblRef >= pR->get_real(i, j));
                            }
                        }
                    }
                    else if(pR->rows_get() == pL->rows_get() && pR->cols_get() == pL->cols_get())
                    {
                        pResult				= new Bool(pR->rows_get(), pR->cols_get());
                        for(int i = 0 ; i < pR->rows_get() ; i++)
                        {
                            for(int j = 0 ; j < pR->cols_get() ; j++)
                            {
                                pResult->getAsBool()->bool_set(i, j, pL->get_real(i, j) >= pR->get_real(i, j));
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
                    result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
                }
                break;
            }
        case OpExp::power :
            {
                if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
                {
                    Double *pL			= pITL->getAs<Double>();
                    Double *pR			= pITR->getAs<Double>();

                    int iResult = PowerDoubleByDouble(pL, pR, (Double**)&pResult);
                    if(iResult != 0)
                    {
                        std::wostringstream os;
                        os << _W("Inconsistent row/column dimensions.\n");
                        //os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                        throw ScilabError(os.str(), 999, e.right_get().location_get());
                    }
                    result_set(pResult);
                }
                else if(TypeL == GenericType::RealPoly && TypeR == GenericType::RealDouble)
                {
                    MatrixPoly *pL	= pITL->getAsPoly();
                    Double *pR			= pITR->getAs<Double>();

                    int iResult = PowerPolyByDouble(pL, pR, (MatrixPoly**)&pResult);
                    if(iResult != 0)
                    {
                        std::wostringstream os;
                        os << _W("Inconsistent row/column dimensions.\n");
                        //os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                        throw ScilabError(os.str(), 999, e.right_get().location_get());
                    }
                    result_set(pResult);
                }
                else
                {
                    result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
                }
                break;
            }
        default :
            // By default call overload if we do not know this operator ...
            result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
            break;
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
    T execMeL;
    T execMeR;

    e.left_get().accept(execMeL);
    InternalType *pITL          = execMeL.result_get();
    InternalType *pITR          = execMeR.result_get();
    
    GenericType::RealType TypeL = pITL->getType();
    Bool *pL	= NULL;

    if(TypeL == GenericType::RealBool)
    {
        pL	= pITL->getAsBool();
    }
    else
    {
        //TODO YaSp : Overloading %*_oper_*
        e.right_get().accept(execMeR);
        switch(e.oper_get())
        {
        case LogicalOpExp::logicalShortCutOr :
        case LogicalOpExp::logicalOr :
            result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
            break;
        case LogicalOpExp::logicalShortCutAnd :
        case LogicalOpExp::logicalAnd :
            result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
            break;
        default :
            break;
        }
        return;
    }

    InternalType *pResult = NULL;
    switch(e.oper_get())
    {
    case LogicalOpExp::logicalShortCutOr :
        {
            Bool *pB	= pITL->getAsBool();
            int *piL	= pB->bool_get();
            bool iL     = true;
            // Check if all values are true
            // true || <something is always true>
            for(int i = 0 ; i < pB->size_get() ; i++)
            {
                if(piL[i] == false)
                {
                    iL = false;
                    break;
                }
            }

            if(iL)
            {//we don't need to look at ohers exp
                pResult = new Bool(true);
                result_set(pResult);
                return;
            }
            // DO NOT break here, falling into normal Or if this can not be shotcutted.
        }
    case LogicalOpExp::logicalOr :
        {
            e.right_get().accept(execMeR);
            GenericType::RealType TypeR = pITR->getType();

            if(TypeR == GenericType::RealBool)
            {
                Bool *pR	= pITR->getAsBool();
                int* piR = pR->bool_get();
                int* piL = pL->bool_get();
                int* piB	= NULL;

                // M | scalar
                if(pR->size_get() == 1)
                {
                    pResult = new Bool(pL->rows_get(), pL->cols_get(), &piB);
                    for(int i = 0 ; i < pL->size_get(); i++)
                    {
                        piB[i] = (piR[0] == 1) || (piL[i] == 1);
                    }
                    result_set(pResult);
                    return;
                }

                // scalar | M
                if(pL->size_get() == 1)
                {
                    pResult = new Bool(pR->rows_get(), pR->cols_get(), &piB);
                    for(int i = 0 ; i < pR->size_get(); i++)
                    {
                        piB[i] = (piR[i] == 1) || (piL[0] == 1);
                    }
                    result_set(pResult);
                    return;
                }

                // M | N (generic case)
                if(pR->rows_get() == pL->rows_get() && pR->cols_get() == pL->cols_get())
                {
                    pResult = new Bool(pR->rows_get(), pR->cols_get(), &piB);
                    for(int i = 0 ; i < pL->size_get(); i++)
                    {
                        piB[i] = (piR[i] == 1) || (piL[i] == 1);
                    }
                    result_set(pResult);
                }
                else
                {
                    std::wostringstream os;
                    os << _W("Inconsistent row/column dimensions.\n");
                    //os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                    throw ScilabError(os.str(), 999, e.right_get().location_get());
                }
            }
            else
            {
                result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
            }
            break;
        }
    case LogicalOpExp::logicalShortCutAnd :
        {
            Bool *pB	= pITL->getAsBool();
            int *piL	= pB->bool_get();
            // Check if one value is false
            // false && <something> is always false
            for(int i = 0 ; i < pB->size_get() ; i++)
            {
                if(piL[i] == false)
                {
                    pResult = new Bool(false);
                    result_set(pResult);
                    return;
                }
            }
            // DO NOT break here, falling into normal And if this can not be shotcutted.
        }
    case LogicalOpExp::logicalAnd :
        {
            e.right_get().accept(execMeR);
            GenericType::RealType TypeR = pITR->getType();

            if(TypeR == GenericType::RealBool)
            {
                Bool *pR	= pITR->getAsBool();
                int* piR = pR->bool_get();
                int* piL = pL->bool_get();
                int* piB	= NULL;

                // M & scalar
                if(pR->size_get() == 1)
                {
                    pResult = new Bool(pL->rows_get(), pL->cols_get(), &piB);
                    for(int i = 0 ; i < pL->size_get(); i++)
                    {
                        piB[i] = (piR[0] == 1) && (piL[i] == 1);
                    }
                    result_set(pResult);
                    return;
                }

                // scalar & M
                if(pL->size_get() == 1)
                {
                    pResult = new Bool(pR->rows_get(), pR->cols_get(), &piB);
                    for(int i = 0 ; i < pR->size_get(); i++)
                    {
                        piB[i] = (piR[i] == 1) && (piL[0] == 1);
                    }
                    result_set(pResult);
                    return;
                }

                // M & N (generic case)
                if(pR->rows_get() == pL->rows_get() && pR->cols_get() == pL->cols_get())
                {
                    pResult = new Bool(pR->rows_get(), pR->cols_get(), &piB);
                    for(int i = 0 ; i < pL->size_get(); i++)
                    {
                        piB[i] = (piR[i] == 1) && (piL[i] == 1);
                    }
                    result_set(pResult);
                }
                else
                {
                    std::wostringstream os;
                    os << _W("Inconsistent row/column dimensions.\n");
                    //os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                    throw ScilabError(os.str(), 999, e.right_get().location_get());
                }
            }
            else
            {
                result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
            }
            break;
        }
        
    default :
        {
            result_set(callOverload(e.oper_get(), &execMeL, &execMeR));
            break;
        }
    }
}

InternalType* callOverload(OpExp::Oper _oper, T *_paramL, T *_paramR)
{
    T execMe;
    types::typed_list in;
    types::typed_list out;
    _paramL->result_get()->IncreaseRef();
    _paramR->result_get()->IncreaseRef();
    in.push_back(_paramL->result_get());
    in.push_back(_paramR->result_get());

    Overload::generateNameAndCall(Overload::getNameFromOper(_oper), in, 1, out, &execMe);
    if(out[0] != NULL)
    {
        out[0]->IncreaseRef();
    }
    _paramL->result_get()->DecreaseRef();
    _paramR->result_get()->DecreaseRef();
    return out[0];
}
