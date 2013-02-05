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

void visitprivate(const AssignExp  &e)
{
    /*Create local exec visitor*/
    try
    {
        /*get king of left hand*/
        const SimpleVar *pVar = dynamic_cast<const SimpleVar*>(&e.left_exp_get());
        if (pVar)
        {
            // x = ?
            /*getting what to assign*/
            InternalType *pIT = e.right_val_get();
            if (pIT == NULL)
            {
                expected_setSize(1);
                e.right_exp_get().accept(*this);

                if (result_getSize() != 1)
                {
                    std::wostringstream os;
                    os << _W("Can not assign multiple value in a single variable") << std::endl;
                    //os << ((Location)e.right_exp_get().location_get()).location_getString() << std::endl;
                    throw ScilabError(os.str(), 999, e.right_exp_get().location_get());
                }

                pIT = result_get();
                //reset result
                result_set(NULL);
            }

            if (pIT->isImplicitList())
            {
                if (pIT->getAs<ImplicitList>()->isComputable())
                {
                    InternalType *pTemp = pIT->getAs<ImplicitList>()->extractFullMatrix();
                    delete pIT;
                    pIT = pTemp;
                }
            }

            if (pIT->isAssignable() == false)
            {
                result_set(NULL);
                return;
            }

            const ReturnExp *pReturn = dynamic_cast<const ReturnExp*>(&e.right_exp_get());
            if (pReturn)
            {
                //ReturnExp so, put the value in the previous scope
                symbol::Context::getInstance()->putInPreviousScope(pVar->name_get(), *pIT);
                ((AssignExp*)&e)->return_set();
            }
            else
            {
                symbol::Context::getInstance()->put(pVar->name_get(), *pIT);
            }

            if (e.is_verbose() && ConfigVariable::isPromptShow())
            {
                std::wostringstream ostr;
                ostr << pVar->name_get().name_get() << L"  = " << std::endl << std::endl;
                scilabWriteW(ostr.str().c_str());
                VariableToString(pIT, pVar->name_get().name_get().c_str());
            }
            return;
        }

        const CellCallExp *pCell = dynamic_cast<const CellCallExp*>(&e.left_exp_get());
        if (pCell)
        {
            InternalType *pIT;
            bool bNew           = false;

            //retrieve variable
            pVar = dynamic_cast<const SimpleVar*>(&pCell->name_get());
            if (pVar == NULL)
            {
                //manage a.b{1} = x
                pCell->name_get().accept(*this);

                if (result_get() != NULL && result_get()->isCell())
                {
                    pIT = result_get();
                }
                else
                {
                    //never append ?
                    std::wostringstream os;
                    os << _W("Unable to extract left part expression.\n");
                    //os << ((Location)e.left_exp_get().location_get()).location_getString() << std::endl;
                    throw ScilabError(os.str(), 999, e.left_exp_get().location_get());
                }
                //reset result
                result_set(NULL);
            }
            else
            {
                pIT = symbol::Context::getInstance()->get(pVar->name_get());
            }

            /*getting what to assign*/
            InternalType* pITR = e.right_val_get();
            if (pITR == NULL)
            {
                e.right_exp_get().accept(*this);
                pITR = result_get();
                //reset result
                result_set(pIT);
            }

            if (pITR == NULL)
            {
                // if the right hand is NULL.
                std::wostringstream os;
                os << _W("Unable to extract right part expression.\n");
                throw ScilabError(os.str(), 999, e.left_exp_get().location_get());
            }

            if (pIT == NULL)
            {
                //Var doesn't exist, create it with good dimensions
                bNew = true;
            }
            else
            {
                if (pIT->isRef(1) == true)
                {
                    pIT = pIT->clone();
                    bNew = true;
                }
            }

            InternalType *pOut	= NULL;
            typed_list *pArgs = GetArgumentList(pCell->args_get());

            //fisrt extract implicit list
            if (pITR->isImplicitList())
            {
                InternalType *pIL = ((InternalType*)pITR)->getAs<ImplicitList>()->extractFullMatrix();
                if (pITR->isDeletable())
                {
                    delete pITR;
                }
                pITR = pIL;
            }
            else if (pITR->isContainer() && pITR->isDeletable() == false)
            {
                InternalType* pIL = pITR->clone();
                pITR = pIL;
            }


            if (pIT == NULL)
            {
                //call static insert function
                pOut = Cell::insertNewCell(pArgs, pITR);
            }
            else
            {
                //call type insert function
                pOut = pIT->getAs<Cell>()->insertCell(pArgs, pITR);

                if (pOut && pOut != pIT)
                {
                    //variable change
                    pIT->DecreaseRef();
                    if (pIT->isDeletable())
                    {
                        delete pIT;
                    }
                    bNew = true;
                }
            }


            if (pOut != NULL)
            {
                if (bNew)
                {
                    symbol::Context::getInstance()->put(pVar->name_get(), *pOut);
                }

                if (e.is_verbose() && ConfigVariable::isPromptShow())
                {
                    std::wostringstream ostr;
                    const wchar_t* wcsVarName;
                    if (pVar)
                    {
                        ostr << pVar->name_get().name_get() << L"  = " << std::endl;
                        wcsVarName = pVar->name_get().name_get().c_str();
                    }
                    else
                    {
                        ostr << L"???" << L"  = " << std::endl;
                        wcsVarName = L"???";
                    }

                    ostr << std::endl;
                    VariableToString(pOut, wcsVarName);
                }
            }
            else
            {
                //manage error
                std::wostringstream os;
                os << _W("Invalid Index.\n");
                //os << ((Location)e.right_exp_get().location_get()).location_getString() << std::endl;
                throw ScilabError(os.str(), 999, e.right_exp_get().location_get());
            }
            //            delete piMaxDim;
            //            delete[] piDimSize;
            for (int iArg = 0 ; iArg < (int)pArgs->size() ; iArg++)
            {
                if ((*pArgs)[iArg]->isDeletable())
                {
                    delete (*pArgs)[iArg];
                }
            }
            delete pArgs;
            return;
        }

        const CallExp *pCall = dynamic_cast<const CallExp*>(&e.left_exp_get());
        if (pCall)
        {
            //x(?) = ?
            InternalType *pIT = NULL;
            InternalType *pOut	= NULL;
            bool bNew   = false;

            /*getting what to assign*/
            InternalType* pITR = e.right_val_get();
            if (pITR == NULL)
            {
                e.right_exp_get().accept(*this);
                pITR = result_get();
                //reset result
                result_set(NULL);
            }

            if (pITR == NULL)
            {
                // if the right hand is NULL.
                std::wostringstream os;
                os << _W("Unable to extract right part expression.\n");
                throw ScilabError(os.str(), 999, e.left_exp_get().location_get());
            }

            typed_list *pArgs = GetArgumentList(pCall->args_get());

            pVar = dynamic_cast<const SimpleVar*>(&pCall->name_get());
            if (pVar == NULL)
            {
                //manage a.b(1) = x
                //this part is only for complex type ( struct/mlist/tlist/handle )
                std::list<std::vector<std::pair<int, InternalType*> > > reinsertHistory;
                std::list<std::vector<std::pair<int, InternalType*> > > structHistory;
                std::list<const ast::Exp*> fieldHistory;
                std::list<types::typed_list*> argHistory;

                bool bFromCallExp = true;

                const wstring *pstName  = getStructNameFromExp(pCall);
                if (pstName)
                {
                    InternalType* pCurrentStr = symbol::Context::getInstance()->getCurrentLevel(symbol::Symbol(*pstName));
                    InternalType* pHigherStr = symbol::Context::getInstance()->get(symbol::Symbol(*pstName));
                    if (pHigherStr && pHigherStr->isStruct() && pCurrentStr == NULL)
                    {
                        //struct come from higher scope, so we need to clone and put it in current scope
                        InternalType *pITClone = pHigherStr->clone();
                        symbol::Context::getInstance()->put(symbol::Symbol(*pstName), *pITClone);
                    }
                }

                if (getStructFromExp(pCall, structHistory, fieldHistory, argHistory, reinsertHistory, pITR, bFromCallExp))
                {
                    pIT = symbol::Context::getInstance()->get(((SimpleVar*)fieldHistory.back())->name_get());
                    pOut = pIT;

                    while (reinsertHistory.empty() == false)
                    {
                        std::vector<std::pair<int, InternalType*> > vectReinsert = reinsertHistory.front();
                        for (int i = 0; i < vectReinsert.size(); i++)
                        {
                            InternalType* pSources  = structHistory.front()[vectReinsert[i].first].second;
                            InternalType* pDest     = vectReinsert[i].second;
                            InternalType* pInsert   = insertionCall(*(fieldHistory.front()), argHistory.front(), pDest, pSources);

                            updateStructHistory(structHistory, fieldHistory, argHistory, pInsert, vectReinsert[i].first);
                        }

                        structHistory.pop_front();
                        fieldHistory.pop_front();
                        argHistory.pop_front();
                        reinsertHistory.pop_front();
                    }
                }
            }
            else
            {
                pIT = symbol::Context::getInstance()->getCurrentLevel(pVar->name_get());
                if (pIT == NULL)
                {
                    //Var doesn't exist, create it with good dimensions
                    bNew = true;
                }
                else
                {
                    if (pIT->isRef(1) == true)
                    {
                        InternalType* pITTemp = pIT->clone();
                        pIT = pITTemp;
                        bNew = true;
                    }
                }

                pOut = insertionCall(e.right_exp_get(), pArgs, pIT, pITR);
            }

            if (pOut && pOut != pIT)
            {
                bNew = true;
            }

            if (pOut != NULL)
            {
                if (bNew)
                {
                    if (pVar == NULL && pIT->isHandle() == false)
                    {
                        printf("a.b(x) 2 return !\n");
                        ////it is not a(x) = y but something like a.b(x) = y
                        ////so we have to retrieve struct and children to assign new value
                        //std::list<InternalType*> structHistory;
                        //std::list<std::wstring> fieldHistory;
                        //std::list<types::typed_list*> argHistory;
                        //bool bComplexForm = false;
                        //if (getStructFromExp(&pCall->name_get(), structHistory, fieldHistory, argHistory, pOut, &bComplexForm))
                        //{
                        //    //after insertion of right part, we have to insert child in parent.
                        //    updateStructFromHistory(structHistory, fieldHistory, argHistory);
                        //    //change pOut only to toString call
                        //    pOut = structHistory.back();
                        //}
                    }
                    else
                    {
                        symbol::Context::getInstance()->put(pVar->name_get(), *((GenericType*)pOut));
                    }
                }

                if (e.is_verbose() && ConfigVariable::isPromptShow())
                {
                    std::wostringstream ostr;
                    const wchar_t* wcsVarName;

                    if (pVar)
                    {
                        ostr << pVar->name_get().name_get() << L"  = " << std::endl;
                        wcsVarName = pVar->name_get().name_get().c_str();
                    }
                    else
                    {
                        ostr << *getStructNameFromExp(&pCall->name_get()) << L"  = " << std::endl;
                        wcsVarName = getStructNameFromExp(&pCall->name_get())->c_str();
                    }

                    ostr << std::endl;
                    scilabWriteW(ostr.str().c_str());
                    VariableToString(pOut, wcsVarName);
                }
            }
            else
            {
                //manage error
                std::wostringstream os;
                os << _W("Invalid index.\n");
                //os << ((Location)e.right_exp_get().location_get()).location_getString() << std::endl;
                throw ScilabError(os.str(), 999, e.right_exp_get().location_get());
            }

            for (int iArg = 0 ; iArg < pArgs->size() ; iArg++)
            {
                if ((*pArgs)[iArg]->isDeletable())
                {
                    delete (*pArgs)[iArg];
                }
            }

            if (pITR->isDeletable())
            {
                delete pITR;
            }

            result_clear();
            delete pArgs;
            return;
        }

        const AssignListExp *pList = dynamic_cast<const AssignListExp*>(&e.left_exp_get());
        if (pList)
        {
            //[x,y] = ?
            int iLhsCount = (int)pList->exps_get().size();

            /*getting what to assign*/
            T exec;
            exec.expected_setSize(iLhsCount);
            e.right_exp_get().accept(exec);

            if (exec.result_getSize() != iLhsCount)
            {
                std::wostringstream os;
                os << _W("Incompatible assignation: trying to assign ") << exec.result_getSize();
                os << _W(" values in ") << iLhsCount << _W(" variables.") << std::endl;
                throw ScilabError(os.str(), 999, e.right_exp_get().location_get());
            }

            std::list<Exp *>::const_reverse_iterator it;
            int i = (int)iLhsCount - 1;
            for (it = pList->exps_get().rbegin() ; it != pList->exps_get().rend() ; it++, i--)
            {
                //create a new AssignExp and run it
                AssignExp* pAssign = new AssignExp((*it)->location_get(), *(*it), *const_cast<Exp*>(&e.right_exp_get()), exec.result_get(i));
                pAssign->set_verbose(e.is_verbose());
                pAssign->accept(*this);

                //clear result to take care of [n,n]
                exec.result_set(i, NULL);
            }
            exec.result_clear();
            return;
        }

        const FieldExp *pField = dynamic_cast<const FieldExp*>(&e.left_exp_get());
        if (pField)
        {
            //a.b = x
            //a.b can be a struct or a tlist/mlist or a handle
            /*getting what to assign*/
            expected_setSize(1);
            e.right_exp_get().accept(*this);
            InternalType *pIT = result_get();
            if (pIT->isImplicitList())
            {
                if (pIT->getAs<ImplicitList>()->isComputable())
                {
                    InternalType *pTemp = pIT->getAs<ImplicitList>()->extractFullMatrix();
                    delete pIT;
                    result_set(NULL);
                    pIT = pTemp;
                }
            }

            std::list<std::vector<std::pair<int, InternalType*> > > reinsertHistory;
            std::list<std::vector<std::pair<int, InternalType*> > > structHistory;
            std::list<const ast::Exp*> fieldHistory;
            std::list<types::typed_list*> argHistory;
            bool bComplexForm = false;

            if (getStructFromExp(pField, structHistory, fieldHistory, argHistory, reinsertHistory, pIT, bComplexForm))
            {
                while (reinsertHistory.empty() == false)
                {
                    std::vector<std::pair<int, InternalType*> > vectReinsert = reinsertHistory.front();
                    for (int i = 0; i < vectReinsert.size(); i++)
                    {
                        InternalType* pSources  = structHistory.front()[vectReinsert[i].first].second;
                        InternalType* pDest     = vectReinsert[i].second;
                        InternalType* pInsert   = insertionCall(*(fieldHistory.front()), argHistory.front(), pDest, pSources);

                        updateStructHistory(structHistory, fieldHistory, argHistory, pInsert, vectReinsert[i].first);
                    }

                    structHistory.pop_front();
                    fieldHistory.pop_front();
                    argHistory.pop_front();
                    reinsertHistory.pop_front();
                }
            }
            else
            {
                std::wostringstream os;
                os << _W("invalid operation");
                throw ScilabError(os.str(), 999, e.right_exp_get().location_get());
            }

            if (e.is_verbose() && ConfigVariable::isPromptShow())
            {
                const wstring *pstName = getStructNameFromExp(pField);

                types::InternalType* pPrint = symbol::Context::getInstance()->get(symbol::Symbol(*pstName));
                std::wostringstream ostr;
                ostr << *pstName << L"  = " << std::endl << std::endl;
                scilabWriteW(ostr.str().c_str());
                VariableToString(pPrint, pstName->c_str());
            }

            result_clear();
            return;
        }

        std::wostringstream os;
        os << _W("unknow script form");
        //os << ((Location)e.right_exp_get().location_get()).location_getString() << std::endl;
        throw ScilabError(os.str(), 999, e.right_exp_get().location_get());
    }
    catch (ScilabError error)
    {
        throw error;
    }
}
