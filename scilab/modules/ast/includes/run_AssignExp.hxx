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
                VariableToString(pIT);
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
                    if (pVar)
                    {
                        ostr << pVar->name_get().name_get() << L"  = " << std::endl;
                    }
                    else
                    {
                        ostr << L"???" << L"  = " << std::endl;
                    }
                    ostr << std::endl;
                    VariableToString(pOut);
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
                std::list<InternalType*> structHistory;
                std::list<std::wstring> fieldHistory;
                std::list<types::typed_list*> argHistory;
                //argHistory.push_front(pArgs);
                bool bComplexForm = false;

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

                if (getStructFromExp(pCall, structHistory, fieldHistory, argHistory, pITR, &bComplexForm))
                {
                    //after insertion of right part, we have to insert child in parent.
                    updateStructFromHistory(structHistory, fieldHistory, argHistory);
                    pIT = structHistory.back();
                    pOut = pIT;
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
                        //it is not a(x) = y but something like a.b(x) = y
                        //so we have to retrieve struct and children to assign new value
                        std::list<InternalType*> structHistory;
                        std::list<std::wstring> fieldHistory;
                        std::list<types::typed_list*> argHistory;
                        bool bComplexForm = false;
                        if (getStructFromExp(&pCall->name_get(), structHistory, fieldHistory, argHistory, pOut, &bComplexForm))
                        {
                            //after insertion of right part, we have to insert child in parent.
                            updateStructFromHistory(structHistory, fieldHistory, argHistory);
                            //change pOut only to toString call
                            pOut = structHistory.back();
                        }
                    }
                    else
                    {
                        symbol::Context::getInstance()->put(pVar->name_get(), *((GenericType*)pOut));
                    }
                }

                if (e.is_verbose() && ConfigVariable::isPromptShow())
                {
                    std::wostringstream ostr;
                    if (pVar)
                    {
                        ostr << pVar->name_get().name_get() << L"  = " << std::endl;
                    }
                    else
                    {
                        ostr << *getStructNameFromExp(&pCall->name_get()) << L"  = " << std::endl;
                    }

                    ostr << std::endl;
                    scilabWriteW(ostr.str().c_str());
                    VariableToString(pOut);
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
            else if (pIT->isContainer())
            {
                //if assigned value is a container, copy it before assign.
                //std::cout << "assign container type to field" << std::endl;
                //pIT = pIT->clone();
            }

            //try to find struct or handle
            {
                const wstring *pstName  = getStructNameFromExp(pField);
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

                std::list<InternalType*> structHistory;
                std::list<std::wstring> fieldHistory;
                std::list<types::typed_list*> argHistory;
                bool bComplexForm = false;
                if (getStructFromExp(pField, structHistory, fieldHistory, argHistory, pIT, &bComplexForm))
                {
                    //after insertion of right part, we have to insert child in parent.
                    updateStructFromHistory(structHistory, fieldHistory, argHistory);
                }
                else
                {
                    //not a struct/handle but it can be a MList ou TList
                    pField->head_get()->accept(*this);
                    InternalType *pHead = result_get();

                    if (pHead->isMList() || pHead->isTList())
                    {
                        //assign result to new field
                        const SimpleVar* pTail =  dynamic_cast<const SimpleVar*>(pField->tail_get());
                        TList* pT = pHead->getAs<TList>();
                        if (pT->exists(pTail->name_get().name_get()))
                        {
                            pT->set(pTail->name_get().name_get(), pIT);
                        }
                        else
                        {
                            SimpleVar *psvRightMember = dynamic_cast<SimpleVar *>(const_cast<Exp *>(pField->tail_get()));
                            std::wstring wstField = L"";
                            if (psvRightMember != NULL)
                            {
                                wstField = psvRightMember->name_get().name_get();
                            }
                            else
                            {
                                wchar_t szError[bsiz];
                                os_swprintf(szError, bsiz, _W("Not implemented in scilab...\n"));
                                throw ScilabError(szError, 999, e.location_get());
                            }

                            //call overload function %typetoassign_i_mlisttype
                            std::wstring stOverload = L"%" + pIT->getShortTypeStr() + L"_i_" +  pT->getShortTypeStr();
                            types::typed_list in;
                            types::typed_list out;

                            //firt argument: index
                            String* pS = new String(wstField.c_str());
                            pS->IncreaseRef();
                            in.push_back(pS);

                            //second argument: who ( var to insert )
                            pIT->IncreaseRef();
                            in.push_back(pIT);

                            //third argument: me ( mlist or tlist )
                            pT->IncreaseRef();
                            in.push_back(pT);

                            Overload::call(stOverload, in, 1, out, this);

                            pIT->DecreaseRef();
                            pT->DecreaseRef();
                            pS->DecreaseRef();
                        }
                    }
                    else
                    {
                        std::wostringstream os;
                        os << _W("invalid operation");
                        throw ScilabError(os.str(), 999, e.right_exp_get().location_get());
                    }
                }
            }

            //if(pHead->isStruct() || pHead == NULL)
            //{
            //    InternalType* pMain     = NULL;
            //    InternalType* pCurrent  = NULL;
            //    const Exp* pCurrentExp  = pField;
            //    const wstring *pstName  = getStructNameFromExp(pField);
            //    if(pstName)
            //    {
            //        InternalType* pCurrentStr = symbol::Context::getInstance()->getCurrentLevel(symbol::Symbol(*pstName));
            //        InternalType* pHigherStr = symbol::Context::getInstance()->get(symbol::Symbol(*pstName));
            //        if(pHigherStr && pCurrentStr == NULL)
            //        {//struct come from higher scope, so we need to clone and put it in current scope
            //            InternalType *pITClone = pHigherStr->clone();
            //            symbol::Context::getInstance()->put(symbol::Symbol(*pstName), *pITClone);
            //        }
            //    }

            //    if(pHead != NULL)
            //    {
            //        pMain = pHead->getAs<Struct>();
            //        pCurrentExp = pField->tail_get();
            //    }

            //    bool bOK = getStructFromExp(pCurrentExp, &pMain, &pCurrent, NULL, pIT);
            //    if (pMain != NULL)
            //    {
            //        pHead = pMain;
            //    }

            //    //if a is already assign, make a copy and replace it
            //    if (pHead->isRef(1) == true)
            //    {
            //        pHead = pHead->clone();
            //        pstName = getStructNameFromExp(pField);
            //        symbol::Context::getInstance()->put(symbol::Symbol(*pstName), *pHead->clone());
            //    }
            //}
            //else if(pHead->isMList())
            //{
            //    //TODO:
            //}
            //else if(pHead->isTList())
            //{
            //    //assign result to new field
            //    const SimpleVar* pTail =  dynamic_cast<const SimpleVar*>(pField->tail_get());
            //    TList* pT = pHead->getAs<TList>();
            //    if (pT->exists(pTail->name_get().name_get()))
            //    {
            //        pT->set(pTail->name_get().name_get(), pIT);
            //    }
            //    else
            //    {
            //        std::wostringstream os;
            //        os << L"Field must be exist";
            //        throw ScilabError(os.str(), 999, pVar->location_get());
            //    }
            //}
            //else if(pHead->isHandle())
            //{
            //    //parse head exp to create a list of "index" to call %x_i_h macro
            //    //List* pList = getPropertyTree((Exp*)pField->tail_get(), new List());
            //    //typed_list arg;
            //    //arg.push_back(new Double(1));
            //    //ListDelete* pDel = new ListDelete();
            //    //pList->insert(&arg, pDel);
            //    //delete pDel;
            //    //delete arg[0];

            //    //call overload %x_i_h
            //    String* pS = new String(((SimpleVar*)pField->tail_get())->name_get().name_get().c_str());
            //    std::wstring str = L"%" + pIT->getShortTypeStr() + L"_i_h";

            //    typed_list in;
            //    typed_list out;
            //    optional_list opt;

            //    in.push_back(pS);
            //    in.push_back(pIT);
            //    in.push_back(pHead);
            //    pS->IncreaseRef();
            //    pIT->IncreaseRef();
            //    pHead->IncreaseRef();

            //    Function* pCall = (Function*)symbol::Context::getInstance()->get(symbol::Symbol(str));
            //    Callable::ReturnValue ret =  pCall->call(in, opt, 1, out, this);
            //    //delete pS;

            //    pS->DecreaseRef();
            //    pIT->DecreaseRef();
            //    pHead->DecreaseRef();
            //    if(ret != Callable::OK)
            //    {
            //        std::wostringstream os;
            //        os << L"unable to update handle";
            //        throw ScilabError(os.str(), 999, e.right_exp_get().location_get());
            //    }
            //}
            //else
            //{
            //    std::wostringstream os;
            //    os << L"invalid operation";
            //    throw ScilabError(os.str(), 999, e.right_exp_get().location_get());
            //}

            if (e.is_verbose() && ConfigVariable::isPromptShow())
            {
                const wstring *pstName = getStructNameFromExp(pField);

                types::InternalType* pPrint = symbol::Context::getInstance()->get(symbol::Symbol(*pstName));
                std::wostringstream ostr;
                ostr << *pstName << L"  = " << std::endl << std::endl;
                scilabWriteW(ostr.str().c_str());
                VariableToString(pPrint);
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
