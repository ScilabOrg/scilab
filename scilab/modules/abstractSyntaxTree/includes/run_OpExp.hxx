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

// This code is separated in run_AssignExp.hxx
// but will be inlined in runvisitor.hxx
// using #include with RunVisitorT class declaration.
//
// If you need additionnal headers, please add it in runvisitor.hxx

void visitprivate(const OpExp &e)
{
    T execMeL;
    T execMeR;

    /*getting what to assign*/
    e.left_get().accept(execMeL);
    /*getting what to assign*/
    e.right_get().accept(execMeR);
    GenericType::RealType TypeL = execMeL.result_get()->getType();
    GenericType::RealType TypeR = execMeR.result_get()->getType();

    if(TypeL == GenericType::RealImplicitList)
    {
        ImplicitList* pIL = execMeL.result_get()->getAsImplicitList();
        if(pIL->computable())
        {
            InternalType *pIT = pIL->extract_matrix();
            execMeL.result_set(pIT);
            TypeL = pIT->getType();
        }
    }

    if(TypeR == GenericType::RealImplicitList)
    {
        ImplicitList* pIL = execMeR.result_get()->getAsImplicitList();
        if(pIL->computable())
        {
            InternalType *pIT = pIL->extract_matrix();
            execMeR.result_set(pIT);
            TypeR = pIT->getType();
        }
    }

    InternalType *pResult = NULL;
    switch(e.oper_get())
    {
    case OpExp::plus :
    {
        if(TypeR == GenericType::RealDouble && TypeL == GenericType::RealDouble)
        {
            Double *pL = execMeL.result_get()->getAsDouble();
            Double *pR = execMeR.result_get()->getAsDouble();

            int iResult = AddDoubleToDouble(pL, pR, (Double**)&pResult);
            if(iResult != 0)
            {
                std::ostringstream os;
                os << "inconsistent row/column dimensions";
                os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                throw os.str();
            }
            result_set(pResult);
        }
        else if(TypeL == GenericType::RealBool && TypeR == GenericType::RealBool)
        {
            //nothing to do, all in macro : %b_+_b
        }
        else if(TypeL == GenericType::RealString && TypeR == GenericType::RealString)
        {
            String *pL = execMeL.result_get()->getAsString();
            String *pR = execMeR.result_get()->getAsString();

            int iResult = AddStringToString(pL, pR, (String**)&pResult);

            if(iResult != 0)
            {
                std::ostringstream os;
                os << "inconsistent row/column dimensions";
                os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                throw os.str();
            }
            result_set(pResult);
        }
        else if(TypeL == GenericType::RealInt && TypeR == GenericType::RealInt)
        {
        }
        else if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealPoly)
        {
            Double *pL				= execMeL.result_get()->getAsDouble();
            MatrixPoly *pR		= execMeR.result_get()->getAsPoly();

            int iResult = AddDoubleToPoly(pR, pL, (MatrixPoly**)&pResult);
            if(iResult != 0)
            {
                std::ostringstream os;
                os << "inconsistent row/column dimensions";
                os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                throw os.str();
            }
            result_set(pResult);
        }
        else if(TypeL == GenericType::RealPoly && TypeR == GenericType::RealDouble)
        {
            Double *pR				= execMeR.result_get()->getAsDouble();
            MatrixPoly *pL		= execMeL.result_get()->getAsPoly();

            int iResult = AddDoubleToPoly(pL, pR, (MatrixPoly**)&pResult);
            if(iResult != 0)
            {
                std::ostringstream os;
                os << "inconsistent row/column dimensions";
                os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                throw os.str();
            }
            result_set(pResult);
        }
        else if(TypeL == GenericType::RealPoly && TypeR == GenericType::RealPoly)
        {
            MatrixPoly *pL	= execMeL.result_get()->getAsPoly();
            MatrixPoly *pR	= execMeR.result_get()->getAsPoly();

            int iResult = AddPolyToPoly(pL, pR, (MatrixPoly**)&pResult);
            if(iResult != 0)
            {
                if(iResult == 1)
                {
                    std::ostringstream os;
                    os << "inconsistent row/column dimensions";
                    os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                    throw os.str();
                }
                else if(iResult == 2)
                {
                    std::ostringstream os;
                    os << "variables don't have the same formal variable";
                    os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                    throw os.str();
                }
            }
            result_set(pResult);
        }
        break;
    }
    case OpExp::minus :
    {
        if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
        {
            Double *pL = execMeL.result_get()->getAsDouble();
            Double *pR = execMeR.result_get()->getAsDouble();

            int iResult = SubstractDoubleToDouble(pL, pR, (Double**)&pResult);
            if(iResult != 0)
            {
                std::ostringstream os;
                os << _("Inconsistent row/column dimensions.\n");
                os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                throw os.str();
            }
            result_set(pResult);
        }
        else if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealPoly)
        {
            Double *pL					= execMeL.result_get()->getAsDouble();
            MatrixPoly *pR			= execMeR.result_get()->getAsPoly();

            int iResult = SubstractPolyToDouble(pL, pR, (MatrixPoly**)&pResult);
            if(iResult != 0)
            {
                std::ostringstream os;
                os << _("Inconsistent row/column dimensions.\n");
                os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                throw os.str();
            }
            result_set(pResult);
        }
        else if(TypeL == GenericType::RealPoly && TypeR == GenericType::RealDouble)
        {
            MatrixPoly *pL			= execMeL.result_get()->getAsPoly();
            Double *pR					= execMeR.result_get()->getAsDouble();

            int iResult = SubstractDoubleToPoly(pL, pR, (MatrixPoly**)&pResult);
            if(iResult != 0)
            {
                std::ostringstream os;
                os << _("Inconsistent row/column dimensions.\n");
                os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                throw os.str();
            }
            result_set(pResult);
        }
        else if(TypeL == GenericType::RealPoly && TypeR == GenericType::RealPoly)
        {
            MatrixPoly *pL			= execMeL.result_get()->getAsPoly();
            MatrixPoly *pR			= execMeR.result_get()->getAsPoly();

            int iResult = SubstractPolyToPoly(pL, pR, (MatrixPoly**)&pResult);
            if(iResult != 0)
            {
                std::ostringstream os;
                os << _("Inconsistent row/column dimensions.\n");
                os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                throw os.str();
            }
            result_set(pResult);
        }
        else if(TypeL == GenericType::RealBool && TypeR == GenericType::RealBool)
        {
        }
        else if(TypeL == GenericType::RealString && TypeR == GenericType::RealString)
        {
        }
        else if(TypeL == GenericType::RealInt && TypeR == GenericType::RealInt)
        {
        }

        break;
    }
    case OpExp::times:
    {
        if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
        {
            Double *pL			= execMeL.result_get()->getAsDouble();
            Double *pR			= execMeR.result_get()->getAsDouble();

            int iResult = MultiplyDoubleByDouble(pL, pR, (Double**)&pResult);
            if(iResult)
            {
                std::ostringstream os;
                os << _("Inconsistent row/column dimensions.\n");
                os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                throw os.str();
            }

            result_set(pResult);
        }
        else if(TypeL == InternalType::RealDouble && TypeR == InternalType::RealPoly)
        {
            Double *pL			    = execMeL.result_get()->getAsDouble();
            MatrixPoly *pR	    = execMeR.result_get()->getAsPoly();

            int iResult = MultiplyDoubleByPoly(pL, pR, (MatrixPoly**)&pResult);

            if(iResult)
            {
                std::ostringstream os;
                os << _("Inconsistent row/column dimensions.\n");
                os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                throw os.str();
            }

            result_set(pResult);
        }
        else if(TypeL == InternalType::RealPoly && TypeR == InternalType::RealDouble)
        {
            MatrixPoly *pL	    = execMeL.result_get()->getAsPoly();
            Double *pR			    = execMeR.result_get()->getAsDouble();

            int iResult = MultiplyPolyByDouble(pL, pR, (MatrixPoly**)&pResult);

            if(iResult)
            {
                std::ostringstream os;
                os << _("Inconsistent row/column dimensions.\n");
                os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                throw os.str();
            }

            result_set(pResult);
        }
        else if(TypeL == InternalType::RealPoly && TypeR == InternalType::RealPoly)
        {
            MatrixPoly *pL	    = execMeL.result_get()->getAsPoly();
            MatrixPoly *pR			= execMeR.result_get()->getAsPoly();

            int iResult = MultiplyPolyByPoly(pL, pR, (MatrixPoly**)&pResult);

            if(iResult)
            {
                std::ostringstream os;
                os << _("Inconsistent row/column dimensions.\n");
                os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                throw os.str();
            }

            result_set(pResult);
        }
        break;
    }
    case OpExp::divide:
    {
        if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
        {
            Double *pL			= execMeL.result_get()->getAsDouble();
            Double *pR			= execMeR.result_get()->getAsDouble();

            int iResult = DivideDoubleByDouble(pL, pR, (Double**)&pResult);
            if(iResult)
            {//manage errors
                std::ostringstream os;
                os << _("Inconsistent row/column dimensions.\n");
                os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                throw os.str();
            }
            result_set(pResult);
        }
        else if(TypeL == GenericType::RealPoly && TypeR == GenericType::RealDouble)
        {
            MatrixPoly *pL	= execMeL.result_get()->getAsPoly();
            Double *pR			= execMeR.result_get()->getAsDouble();

            int iResult = DividePolyByDouble(pL, pR, (MatrixPoly**)&pResult);
            if(iResult)
            {//manage errors
                std::ostringstream os;
                os << _("Inconsistent row/column dimensions.\n");
                os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                throw os.str();
            }

            result_set(pResult);
        }
        else if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealPoly)
        {
            Double *pL			= execMeL.result_get()->getAsDouble();
            MatrixPoly *pR	= execMeR.result_get()->getAsPoly();

            int iResult = DivideDoubleByPoly(pL, pR, (MatrixPoly**)&pResult);
            if(iResult)
            {//manage errors
                std::ostringstream os;
                os << _("Inconsistent row/column dimensions.\n");
                os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                throw os.str();
            }
            result_set(pResult);
        }

        break;
    }
    case OpExp::dottimes :
    {
        if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
        {
            Double *pL			= execMeL.result_get()->getAsDouble();
            Double *pR			= execMeR.result_get()->getAsDouble();


            int iResult = DotMultiplyDoubleByDouble(pL, pR, (Double**)&pResult);
            if(iResult)
            {
                std::ostringstream os;
                os << _("Inconsistent row/column dimensions.\n");
                os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                throw os.str();
            }

            result_set(pResult);
        }
        break;
    }
    case OpExp::eq :
    {
        if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
        {
            Double *pL			= execMeL.result_get()->getAsDouble();
            Double *pR			= execMeR.result_get()->getAsDouble();

            if(pR->size_get() == 1)
            {
                pResult				= new Bool(pL->rows_get(), pL->cols_get());
                double dblRef	= pR->real_get(0,0);
                for(int i = 0 ; i < pL->rows_get() ; i++)
                {
                    for(int j = 0 ; j < pL->cols_get() ; j++)
                    {
                        pResult->getAsBool()->bool_set(i, j, pL->real_get(i, j) == dblRef);
                    }
                }
            }
            else if(pL->size_get() == 1)
            {
                pResult				= new Bool(pR->rows_get(), pR->cols_get());
                double dblRef	= pL->real_get(0,0);
                for(int i = 0 ; i < pR->rows_get() ; i++)
                {
                    for(int j = 0 ; j < pR->cols_get() ; j++)
                    {
                        pResult->getAsBool()->bool_set(i, j, dblRef == pR->real_get(i, j));
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
                        pResult->getAsBool()->bool_set(i, j, pL->real_get(i, j) == pR->real_get(i, j));
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
            String *pL			= execMeL.result_get()->getAsString();
            String *pR			= execMeR.result_get()->getAsString();

            if(pL->size_get() == 1)
            {
                pResult = new Bool(pR->rows_get(), pR->cols_get());

                char* pstL = pL->string_get()[0];
                for(int i = 0 ; i < pR->rows_get() ; i++)
                {
                    for(int j = 0 ; j < pR->cols_get() ; j++)
                    {
                        char* pstR = pR->string_get(i,j);
                        if(strcmp(pstL, pstR) == 0)
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

                char* pstR = pR->string_get()[0];
                for(int i = 0 ; i < pL->rows_get() ; i++)
                {
                    for(int j = 0 ; j < pL->cols_get() ; j++)
                    {
                        char* pstL = pL->string_get(i,j);
                        if(strcmp(pstL, pstR) == 0)
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
                        char* pstR = pR->string_get(i,j);
                        char* pstL = pL->string_get(i,j);
                        if(strcmp(pstL, pstR) == 0)
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
        break;
    }
    case OpExp::ne :
    {
        if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
        {
            Double *pL			= execMeL.result_get()->getAsDouble();
            Double *pR			= execMeR.result_get()->getAsDouble();

            if(pR->size_get() == 1)
            {
                pResult				= new Bool(pL->rows_get(), pL->cols_get());
                double dblRef	= pR->real_get(0,0);
                for(int i = 0 ; i < pL->rows_get() ; i++)
                {
                    for(int j = 0 ; j < pL->cols_get() ; j++)
                    {
                        pResult->getAsBool()->bool_set(i, j, pL->real_get(i, j) != dblRef);
                    }
                }
            }
            else if(pL->size_get() == 1)
            {
                pResult				= new Bool(pR->rows_get(), pR->cols_get());
                double dblRef	= pL->real_get(0,0);
                for(int i = 0 ; i < pR->rows_get() ; i++)
                {
                    for(int j = 0 ; j < pR->cols_get() ; j++)
                    {
                        pResult->getAsBool()->bool_set(i, j, dblRef != pR->real_get(i, j));
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
                        pResult->getAsBool()->bool_set(i, j, pL->real_get(i, j) != pR->real_get(i, j));
                    }
                }
            }
            else
            {
                pResult = new Bool(false);
            }

            result_set(pResult);
        }
        break;
    }
    case OpExp::lt :
    {
        if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
        {
            Double *pL			= execMeL.result_get()->getAsDouble();
            Double *pR			= execMeR.result_get()->getAsDouble();

            if(pR->size_get() == 1)
            {
                pResult				= new Bool(pL->rows_get(), pL->cols_get());
                double dblRef	= pR->real_get(0,0);
                for(int i = 0 ; i < pL->rows_get() ; i++)
                {
                    for(int j = 0 ; j < pL->cols_get() ; j++)
                    {
                        pResult->getAsBool()->bool_set(i, j, pL->real_get(i, j) < dblRef);
                    }
                }
            }
            else if(pL->size_get() == 1)
            {
                pResult				= new Bool(pR->rows_get(), pR->cols_get());
                double dblRef	= pL->real_get(0,0);
                for(int i = 0 ; i < pR->rows_get() ; i++)
                {
                    for(int j = 0 ; j < pR->cols_get() ; j++)
                    {
                        pResult->getAsBool()->bool_set(i, j, dblRef < pR->real_get(i, j));
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
                        pResult->getAsBool()->bool_set(i, j, pL->real_get(i, j) < pR->real_get(i, j));
                    }
                }
            }
            else
            {
                pResult = new Bool(false);
            }

            result_set(pResult);
        }
        break;
    }
    case OpExp::le :
    {
        if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
        {
            Double *pL			= execMeL.result_get()->getAsDouble();
            Double *pR			= execMeR.result_get()->getAsDouble();

            if(pR->size_get() == 1)
            {
                pResult				= new Bool(pL->rows_get(), pL->cols_get());
                double dblRef	= pR->real_get(0,0);
                for(int i = 0 ; i < pL->rows_get() ; i++)
                {
                    for(int j = 0 ; j < pL->cols_get() ; j++)
                    {
                        pResult->getAsBool()->bool_set(i, j, pL->real_get(i, j) <= dblRef);
                    }
                }
            }
            else if(pL->size_get() == 1)
            {
                pResult				= new Bool(pR->rows_get(), pR->cols_get());
                double dblRef	= pL->real_get(0,0);
                for(int i = 0 ; i < pR->rows_get() ; i++)
                {
                    for(int j = 0 ; j < pR->cols_get() ; j++)
                    {
                        pResult->getAsBool()->bool_set(i, j, dblRef <= pR->real_get(i, j));
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
                        pResult->getAsBool()->bool_set(i, j, pL->real_get(i, j) <= pR->real_get(i, j));
                    }
                }
            }
            else
            {
                pResult = new Bool(false);
            }

            result_set(pResult);
        }
        break;
    }
    case OpExp::gt :
    {
        if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
        {
            Double *pL			= execMeL.result_get()->getAsDouble();
            Double *pR			= execMeR.result_get()->getAsDouble();

            if(pR->size_get() == 1)
            {
                pResult				= new Bool(pL->rows_get(), pL->cols_get());
                double dblRef	= pR->real_get(0,0);
                for(int i = 0 ; i < pL->rows_get() ; i++)
                {
                    for(int j = 0 ; j < pL->cols_get() ; j++)
                    {
                        pResult->getAsBool()->bool_set(i, j, pL->real_get(i, j) > dblRef);
                    }
                }
            }
            else if(pL->size_get() == 1)
            {
                pResult				= new Bool(pR->rows_get(), pR->cols_get());
                double dblRef	= pL->real_get(0,0);
                for(int i = 0 ; i < pR->rows_get() ; i++)
                {
                    for(int j = 0 ; j < pR->cols_get() ; j++)
                    {
                        pResult->getAsBool()->bool_set(i, j, dblRef > pR->real_get(i, j));
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
                        pResult->getAsBool()->bool_set(i, j, pL->real_get(i, j) > pR->real_get(i, j));
                    }
                }
            }
            else
            {
                pResult = new Bool(false);
            }

            result_set(pResult);
        }
        break;
    }
    case OpExp::ge :
    {
        if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
        {
            Double *pL			= execMeL.result_get()->getAsDouble();
            Double *pR			= execMeR.result_get()->getAsDouble();

            if(pR->size_get() == 1)
            {
                pResult				= new Bool(pL->rows_get(), pL->cols_get());
                double dblRef	= pR->real_get(0,0);
                for(int i = 0 ; i < pL->rows_get() ; i++)
                {
                    for(int j = 0 ; j < pL->cols_get() ; j++)
                    {
                        pResult->getAsBool()->bool_set(i, j, pL->real_get(i, j) >= dblRef);
                    }
                }
            }
            else if(pL->size_get() == 1)
            {
                pResult				= new Bool(pR->rows_get(), pR->cols_get());
                double dblRef	= pL->real_get(0,0);
                for(int i = 0 ; i < pR->rows_get() ; i++)
                {
                    for(int j = 0 ; j < pR->cols_get() ; j++)
                    {
                        pResult->getAsBool()->bool_set(i, j, dblRef >= pR->real_get(i, j));
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
                        pResult->getAsBool()->bool_set(i, j, pL->real_get(i, j) >= pR->real_get(i, j));
                    }
                }
            }
            else
            {
                pResult = new Bool(false);
            }

            result_set(pResult);
        }
        break;
    }
    case OpExp::power :
    {
        if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
        {
            Double *pL			= execMeL.result_get()->getAsDouble();
            Double *pR			= execMeR.result_get()->getAsDouble();

            int iResult = PowerDoubleByDouble(pL, pR, (Double**)&pResult);
            if(iResult != 0)
            {
                std::ostringstream os;
                os << _("Inconsistent row/column dimensions.\n");
                os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                throw os.str();
            }
            result_set(pResult);
        }
        else if(TypeL == GenericType::RealPoly && TypeR == GenericType::RealDouble)
        {
            MatrixPoly *pL	= execMeL.result_get()->getAsPoly();
            Double *pR			= execMeR.result_get()->getAsDouble();

            int iResult = PowerPolyByDouble(pL, pR, (MatrixPoly**)&pResult);
            if(iResult != 0)
            {
                std::ostringstream os;
                os << _("Inconsistent row/column dimensions.\n");
                os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                throw os.str();
            }
            result_set(pResult);
        }
        else
        {
            throw _("Invalid input");
        }
        break;
    }
    default :
        break;
    }
}

void visitprivate(const LogicalOpExp &e)
{
    T execMeL;
    T execMeR;

    e.left_get().accept(execMeL);
    GenericType::RealType TypeL = execMeL.result_get()->getType();
    Bool *pL	= NULL;

    if(TypeL == GenericType::RealBool)
    {
        pL	= execMeL.result_get()->getAsBool();
    }
    else
    {
        //TODO YaSp : Overloading %*_oper_*
        e.right_get().accept(execMeR);
        types::typed_list in;
        types::typed_list out;
        T execOverload;
        in.push_back(execMeL.result_get());
        in.push_back(execMeR.result_get());
        switch(e.oper_get())
        {
        case LogicalOpExp::logicalShortCutOr :
        case LogicalOpExp::logicalOr :
            Overload::generateNameAndCall(std::string("g"), in, 1, out, &execOverload);
            if(out.size() == 1)
            {
                result_set(out[0]);
            }
            else
            {
                result_set(new types::Double(0,0,false));
            }
            break;
        case LogicalOpExp::logicalShortCutAnd :
        case LogicalOpExp::logicalAnd :
            Overload::generateNameAndCall(std::string("h"), in, 1, out, &execOverload);
            if(out.size() == 1)
            {
                result_set(out[0]);
            }
            else
            {
                result_set(new types::Double(0,0,false));
            }
            break;
        }
        return;
    }

    InternalType *pResult = NULL;
    switch(e.oper_get())
    {
    case LogicalOpExp::logicalShortCutOr :
    {
        Bool *pL	= execMeL.result_get()->getAsBool();
        int *piL	= pL->bool_get();
        bool iL     = true;
        // Check if all values are true
        // true || <something is always true>
        for(int i = 0 ; i < pL->size_get() ; i++)
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
        GenericType::RealType TypeR = execMeR.result_get()->getType();

        if(TypeR == GenericType::RealBool)
        {
            Bool *pR	= execMeR.result_get()->getAsBool();
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
                int* piB	= NULL;
                int* piR	= pR->bool_get();
                int* piL	= pL->bool_get();

                pResult = new Bool(pR->rows_get(), pR->cols_get(), &piB);
                for(int i = 0 ; i < pL->size_get(); i++)
                {
                    piB[i] = (piR[i] == 1) || (piL[i] == 1);
                }
                result_set(pResult);
            }
            else
            {
                std::ostringstream os;
                os << _("Inconsistent row/column dimensions.\n");
                os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                throw os.str();
            }
        }
        else
        {
            T execMe;
            types::typed_list in;
            types::typed_list out;

            Overload::generateNameAndCall(std::string("g"), in, 1, out, &execMe);
        }
        break;
    }
    case LogicalOpExp::logicalShortCutAnd :
    {
        Bool *pL	= execMeL.result_get()->getAsBool();
        int *piL	= pL->bool_get();
        bool iL     = true;
        // Check if all values are false
        // false && <something> is always false
        for(int i = 0 ; i < pL->size_get() ; i++)
        {
            if(piL[i] == true)
            {
                iL = false;
                break;
            }
        }
        if(iL)
        {//we don't need to look at ohers exp
            pResult = new Bool(false);
            result_set(pResult);
            return;
        }
        // DO NOT break here, falling into normal And if this can not be shotcutted.
    }
    case LogicalOpExp::logicalAnd :
    {
        e.right_get().accept(execMeR);
        GenericType::RealType TypeR = execMeR.result_get()->getType();

        if(TypeR == GenericType::RealBool)
        {
            Bool *pR	= execMeR.result_get()->getAsBool();
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
                int* piB	= NULL;
                int* piR	= pR->bool_get();
                int* piL	= pL->bool_get();

                pResult = new Bool(pR->rows_get(), pR->cols_get(), &piB);
                for(int i = 0 ; i < pL->size_get(); i++)
                {
                    piB[i] = (piR[i] == 1) && (piL[i] == 1);
                }
                result_set(pResult);
            }
            else
            {
                std::ostringstream os;
                os << _("Inconsistent row/column dimensions.\n");
                os << ((Location)e.right_get().location_get()).location_string_get() << std::endl;
                throw os.str();
            }
        }
        else
        {
            //TODO YaSp : Overloading %*_h_*
            YaspWrite("Calling overload : %*_h_*\n");
        }
        break;
    }
    }
}

