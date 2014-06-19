/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string>

#include "runvisitor.hxx"
#include "execvisitor.hxx"
#include "stepvisitor.hxx"
#include "timedvisitor.hxx"
#include "shortcutvisitor.hxx"
#include "printvisitor.hxx"
#include "mutevisitor.hxx"

#include "visitor_common.hxx"

#include "context.hxx"
#include "generic_operations.hxx"
#include "types_or_and.hxx"

extern "C"
{
#include "sciprint.h"
#include "os_swprintf.h"
}

namespace ast
{
template <class T>
void RunVisitorT<T>::visitprivate(const CellExp &e)
{
    std::list<MatrixLineExp *>::const_iterator row;
    std::list<Exp *>::const_iterator col;
    int iColMax = 0;

    //check dimmension
    for (row = e.lines_get().begin() ; row != e.lines_get().end() ; ++row )
    {
        if (iColMax == 0)
        {
            iColMax = static_cast<int>((*row)->columns_get().size());
        }

        if (iColMax != static_cast<int>((*row)->columns_get().size()))
        {
            std::wostringstream os;
            os << _W("inconsistent row/column dimensions\n");
            //os << ((Location)(*row)->location_get()).location_getString() << std::endl;
            throw ScilabError(os.str(), 999, (*row)->location_get());
        }
    }

    //alloc result cell
    types::Cell *pC = new types::Cell(static_cast<int>(e.lines_get().size()), iColMax);

    int i = 0;
    int j = 0;

    //insert items in cell
    for (i = 0, row = e.lines_get().begin() ; row != e.lines_get().end() ; row++, i++ )
    {
        for (j = 0, col = (*row)->columns_get().begin() ; col != (*row)->columns_get().end() ; col++, j++)
        {
            (*col)->accept(*this);
            InternalType *pIT = result_get();
            if (pIT->isImplicitList())
            {
                pIT = pIT->getAs<ImplicitList>()->extractFullMatrix();
            }

            pC->set(i, j, pIT);
            result_set(NULL);
        }
    }

    //return new cell
    result_set(pC);
}

template <class T>
void RunVisitorT<T>::visitprivate(const FieldExp &e)
{
    /*
    a.b
    */
    try
    {
        e.head_get()->accept(*this);
    }
    catch (ScilabError error)
    {
        throw error;
    }

    SimpleVar *psvRightMember = dynamic_cast<SimpleVar *>(const_cast<Exp *>(e.tail_get()));
    std::wstring wstField = L"";
    if (psvRightMember != NULL)
    {
        wstField = psvRightMember->name_get().name_get();
    }
    else
    {
        wchar_t szError[bsiz];
        os_swprintf(szError, bsiz, _W("/!\\ Unmanaged FieldExp.\n"));
        throw ScilabError(szError, 999, e.location_get());
    }

    if (result_get() == NULL)
    {
        wchar_t szError[bsiz];
        os_swprintf(szError, bsiz, _W("Attempt to reference field of non-structure array.\n"));
        throw ScilabError(szError, 999, e.location_get());
    }

    if (result_get()->isStruct())
    {
        InternalType* pTemp = result_get();
        result_set(NULL);
        Struct* psValue = pTemp->getAs<Struct>();
        if (psValue->exists(wstField))
        {
            if (psValue->getSize() != 1)
            {
                std::vector<std::wstring> wstFields;
                wstFields.push_back(wstField);
                std::vector<InternalType*> result;
                result = psValue->extractFields(wstFields);
                result_set(result[0]);
            }
            else
            {
                InternalType* pIT = psValue->get(0)->get(wstField)->clone();
                result_set(pIT);
            }
        }
        else
        {
            wchar_t szError[bsiz];
            os_swprintf(szError, bsiz, _W("Unknown field : %ls.\n"), wstField.c_str());
            throw ScilabError(szError, 999, e.tail_get()->location_get());
        }
    }
    else if (result_get()->isMList() || result_get()->isTList())
    {
        TList* psValue = ((InternalType*)result_get())->getAs<MList>();

        if (psValue->exists(wstField))
        {
            //without overloading function, extract by name
            result_set(psValue->getField(wstField));
        }
        else
        {
            //call %mlisttype_e
            std::wostringstream ostr;
            types::typed_list in;
            types::optional_list opt;
            types::typed_list out;

            //firt argument: index
            String* pS = new String(wstField.c_str());
            pS->IncreaseRef();
            in.push_back(pS);

            //second argument: me ( mlist or tlist )
            psValue->IncreaseRef();
            in.push_back(psValue);

            Callable::ReturnValue Ret = Overload::call(L"%" + psValue->getShortTypeStr() + L"_e", in, 1, out, this);

            if (Ret != Callable::OK)
            {
                throw ScilabError();
            }

            if (out.size() == 0)
            {
                result_set(NULL);
            }
            else if (out.size() == 1)
            {
                out[0]->DecreaseRef();
                result_set(out[0]);
            }
            else
            {
                for (int i = 0 ; i < static_cast<int>(out.size()) ; i++)
                {
                    out[i]->DecreaseRef();
                    result_set(i, out[i]);
                }
            }

            psValue->DecreaseRef();
            pS->DecreaseRef();
        }
    }
    else if (result_get()->isHandle())
    {
        typed_list in;
        typed_list out;
        optional_list opt;

        String* pField = new String(wstField.c_str());
        in.push_back(pField);
        in.push_back(result_get());

        Function* pCall = (Function*)symbol::Context::getInstance()->get(symbol::Symbol(L"%h_e"));
        Callable::ReturnValue ret =  pCall->call(in, opt, 1, out, this);
        if (ret == Callable::OK)
        {
            result_set(out[0]);
        }
    }
    else if (result_get()->isLibrary())
    {
        Library* pLib = ((InternalType*)result_get())->getAs<Library>();

        InternalType* pIT = pLib->get(wstField);
        if (pIT == NULL)
        {
            wchar_t szError[bsiz];
            os_swprintf(szError, bsiz, _W("Unknown macro %ls in library.\n"), wstField.c_str());
            throw ScilabError(szError, 999, e.tail_get()->location_get());
        }

        result_set(pIT);
    }
    else
    {
        wchar_t szError[bsiz];
        os_swprintf(szError, bsiz, _W("Attempt to reference field of non-structure array.\n"));
        throw ScilabError(szError, 999, e.location_get());
    }
}

template <class T>
void RunVisitorT<T>::visitprivate(const IfExp  &e)
{
    //Create local exec visitor
    ShortCutVisitor SCTest;
    bool bTestStatus = false;

    //condition
    e.test_get().accept(SCTest);
    e.test_get().accept(*this);

    bTestStatus = result_get()->isTrue();
    result_clear();
    if (bTestStatus == true)
    {
        //condition == true
        if (e.is_breakable())
        {
            const_cast<IfExp*>(&e)->break_reset();
            const_cast<Exp*>(&e.then_get())->breakable_set();
        }

        if (e.is_continuable())
        {
            const_cast<IfExp*>(&e)->continue_reset();
            const_cast<Exp*>(&e.then_get())->continuable_set();
        }

        if (e.is_returnable())
        {
            const_cast<IfExp*>(&e)->return_reset();
            const_cast<Exp*>(&e.then_get())->returnable_set();
        }

        e.then_get().accept(*this);
    }
    else
    {
        //condition == false

        if (e.has_else())
        {
            if (e.is_breakable())
            {
                const_cast<Exp*>(&e.else_get())->breakable_set();
            }

            if (e.is_continuable())
            {
                const_cast<IfExp*>(&e)->continue_reset();
                const_cast<Exp*>(&e.else_get())->continuable_set();
            }

            if (e.is_returnable())
            {
                const_cast<Exp*>(&e.else_get())->returnable_set();
            }

            e.else_get().accept(*this);
        }
    }

    if (e.is_breakable()
            && ( (&e.else_get())->is_break()
                 || (&e.then_get())->is_break() ))
    {
        const_cast<IfExp*>(&e)->break_set();
        const_cast<Exp*>(&e.else_get())->break_reset();
        const_cast<Exp*>(&e.then_get())->break_reset();
    }

    if (e.is_continuable()
            && ( (&e.else_get())->is_continue()
                 || (&e.then_get())->is_continue() ))
    {
        const_cast<IfExp*>(&e)->continue_set();
        const_cast<Exp*>(&e.else_get())->continue_reset();
        const_cast<Exp*>(&e.then_get())->continue_reset();
    }

    if (e.is_returnable()
            && ( (&e.else_get())->is_return()
                 || (&e.then_get())->is_return() ))
    {
        const_cast<IfExp*>(&e)->return_set();
        const_cast<Exp*>(&e.else_get())->return_reset();
        const_cast<Exp*>(&e.then_get())->return_reset();
    }
}

template <class T>
void RunVisitorT<T>::visitprivate(const WhileExp  &e)
{
    //allow break and continue operations
    const_cast<Exp*>(&e.body_get())->breakable_set();
    const_cast<Exp*>(&e.body_get())->continuable_set();
    //allow return operation
    if (e.is_returnable())
    {
        (&e.body_get())->is_returnable();
    }

    //condition
    e.test_get().accept(*this);
    while (result_get()->isTrue())
    {
        e.body_get().accept(*this);
        if (e.body_get().is_break())
        {
            const_cast<Exp*>(&(e.body_get()))->break_reset();
            break;
        }

        if (e.body_get().is_return())
        {
            const_cast<WhileExp*>(&e)->return_set();
            const_cast<Exp*>(&(e.body_get()))->return_reset();
            break;
        }

        if (e.body_get().is_continue())
        {
            const_cast<WhileExp*>(&e)->continue_set();
            const_cast<Exp*>(&(e.body_get()))->continue_reset();
            e.test_get().accept(*this);
            continue;
        }

        //clear old result value before evaluate new one
        if (result_get() != NULL)
        {
            if (result_get()->isDeletable())
            {
                delete result_get();
            }
        }

        e.test_get().accept(*this);
    }

    //clear result of condition or result of body
    result_clear();
}

template <class T>
void RunVisitorT<T>::visitprivate(const ForExp  &e)
{
    e.vardec_get().accept(*this);
    InternalType* pIT = result_get();
    //allow break and continue operations
    const_cast<Exp*>(&e.body_get())->breakable_set();
    const_cast<Exp*>(&e.body_get())->continuable_set();

    //allow return operation
    if (e.is_returnable())
    {
        (&e.body_get())->is_returnable();
    }

    if (result_get()->isImplicitList())
    {
        ImplicitList* pVar = pIT->getAs<ImplicitList>();

        InternalType *pIL = NULL;
        pIL = pVar->extractValue(0);
        symbol::Symbol varName = e.vardec_get().name_get();

        for (int i = 0 ; i < pVar->getSize() ; i++)
        {
            pIL = pVar->extractValue(i);
            symbol::Context::getInstance()->put(e.vardec_get().stack_get(), pIL);

            e.body_get().accept(*this);
            if (e.body_get().is_break())
            {
                const_cast<Exp*>(&(e.body_get()))->break_reset();
                break;
            }

            if (e.body_get().is_continue())
            {
                const_cast<Exp*>(&(e.body_get()))->continue_reset();
                continue;
            }

            if (e.body_get().is_return())
            {
                const_cast<ForExp*>(&e)->return_set();
                break;
            }
        }
    }
    else if (result_get()->isList())
    {
        List* pL = pIT->getAs<List>();
        for (int i = 0 ; i < pL->getSize() ; i++)
        {
            InternalType* pNew = pL->get(i);
            symbol::Context::getInstance()->put(e.vardec_get().stack_get(), pNew);
            e.body_get().accept(*this);
            if (e.body_get().is_break())
            {
                const_cast<Exp*>(&(e.body_get()))->break_reset();
                break;
            }

            if (e.body_get().is_continue())
            {
                const_cast<Exp*>(&(e.body_get()))->continue_reset();
                continue;
            }

            if (e.body_get().is_return())
            {
                const_cast<ForExp*>(&e)->return_set();
                break;
            }
        }
    }
    else
    {
        //Matrix i = [1,3,2,6] or other type
        GenericType* pVar = pIT->getAs<GenericType>();
        if (pVar->getDims() > 2)
        {
            throw ScilabError(_W("for expression can only manage 1 or 2 dimensions variables\n"), 999, e.vardec_get().location_get());
        }

        for (int i = 0 ; i < pVar->getCols() ; i++)
        {
            GenericType* pNew = pVar->getColumnValues(i);
            symbol::Context::getInstance()->put(e.vardec_get().stack_get(), pNew);
            e.body_get().accept(*this);
            if (e.body_get().is_break())
            {
                const_cast<Exp*>(&(e.body_get()))->break_reset();
                break;
            }

            if (e.body_get().is_continue())
            {
                const_cast<Exp*>(&(e.body_get()))->continue_reset();
                continue;
            }

            if (e.body_get().is_return())
            {
                const_cast<ForExp*>(&e)->return_set();
                break;
            }
        }
    }

    pIT->DecreaseRef();
    if (pIT->isDeletable())
    {
        delete pIT;
    }

    result_set(NULL);
}

template <class T>
void RunVisitorT<T>::visitprivate(const ReturnExp &e)
{
    if (e.is_global())
    {
        //return or resume
        if (ConfigVariable::getPauseLevel() != 0)
        {
            ThreadId* pThreadId = ConfigVariable::getLastPausedThread();
            if (pThreadId == NULL)
            {
                //no paused thread, so just go leave
                return;
            }

            //force exit without prompt of current thread ( via Aborted status )
            ThreadId* pMe = ConfigVariable::getThread(__GetCurrentThreadKey());
            pMe->setStatus(ThreadId::Aborted);

            //resume previous execution thread
            pThreadId->resume();

            return;
        }
        else
        {
            const_cast<ReturnExp*>(&e)->return_set();
        }
    }
    else
    {
        //return(x)

        //in case of CallExp, we can return only one values
        int iSaveExpectedSize = expected_getSize();
        expected_setSize(1);
        e.exp_get().accept(*this);
        expected_setSize(iSaveExpectedSize);

        if (result_getSize() == 1)
        {
            //protect variable
            result_get()->IncreaseRef();
        }
        else
        {
            for (int i = 0 ; i < result_getSize() ; i++)
            {
                //protect variable
                result_get(i)->IncreaseRef();
            }
        }

        if (result_getSize() == 1)
        {
            //unprotect variable
            result_get()->DecreaseRef();
        }
        else
        {
            for (int i = 0 ; i < result_getSize() ; i++)
            {
                //unprotect variable
                result_get(i)->DecreaseRef();
            }
        }

        const_cast<ReturnExp*>(&e)->return_set();
    }
}

template <class T>
void RunVisitorT<T>::visitprivate(const SelectExp &e)
{
    // FIXME : exec select ... case ... else ... end
    e.select_get()->accept(*this);
    bool bCase = false;


    InternalType* pIT = result_get();
    result_set(NULL);
    if (pIT)
    {
        //find good case
        cases_t::iterator it;
        for (it = e.cases_get()->begin(); it != e.cases_get()->end() ; it++)
        {
            CaseExp* pCase = *it;
            pCase->test_get()->accept(*this);
            InternalType *pITCase = result_get();
            result_set(NULL);
            if (pITCase)
            {
                if (pITCase->isContainer()) //WARNING ONLY FOR CELL
                {
                    //check each item
                }
                else if (*pITCase == *pIT)
                {
                    if (e.is_breakable())
                    {
                        const_cast<SelectExp*>(&e)->break_reset();
                        pCase->body_get()->breakable_set();
                    }

                    if (e.is_continuable())
                    {
                        const_cast<SelectExp*>(&e)->continue_reset();
                        pCase->body_get()->continuable_set();
                    }

                    if (e.is_returnable())
                    {
                        const_cast<SelectExp*>(&e)->return_reset();
                        pCase->body_get()->returnable_set();
                    }

                    //the good one
                    pCase->body_get()->accept(*this);

                    if (e.is_breakable() && pCase->body_get()->is_break())
                    {
                        const_cast<SelectExp*>(&e)->break_set();
                        pCase->body_get()->break_reset();
                    }

                    if (e.is_continuable() && pCase->body_get()->is_continue())
                    {
                        const_cast<SelectExp*>(&e)->continue_set();
                        pCase->body_get()->continue_reset();
                    }

                    if (e.is_returnable() && pCase->body_get()->is_return())
                    {
                        const_cast<SelectExp*>(&e)->return_set();
                        pCase->body_get()->return_reset();
                    }

                    bCase = true;
                    break;
                }
            }
        }
    }

    if (bCase == false && e.default_case_get() != NULL)
    {
        if (e.is_breakable())
        {
            const_cast<SelectExp*>(&e)->break_reset();
            e.default_case_get()->breakable_set();
        }

        if (e.is_continuable())
        {
            const_cast<SelectExp*>(&e)->continue_reset();
            e.default_case_get()->continuable_set();
        }

        if (e.is_returnable())
        {
            const_cast<SelectExp*>(&e)->return_reset();
            e.default_case_get()->returnable_set();
        }

        //default case
        e.default_case_get()->accept(*this);

        if (e.is_breakable() && e.default_case_get()->is_break())
        {
            const_cast<SelectExp*>(&e)->break_set();
            e.default_case_get()->break_reset();
        }

        if (e.is_continuable() && e.default_case_get()->is_continue())
        {
            const_cast<SelectExp*>(&e)->continue_set();
            e.default_case_get()->continue_reset();
        }

        if (e.is_returnable() && e.default_case_get()->is_return())
        {
            const_cast<SelectExp*>(&e)->return_set();
            e.default_case_get()->return_reset();
        }
    }

    result_clear();
}

template <class T>
void RunVisitorT<T>::visitprivate(const SeqExp  &e)
{
    //T execMe;
    std::list<Exp *>::const_iterator	itExp;

    for (itExp = e.exps_get().begin (); itExp != e.exps_get().end (); ++itExp)
    {
        if (e.is_breakable())
        {
            (*itExp)->break_reset();
            (*itExp)->breakable_set();
        }

        if (e.is_continuable())
        {
            (*itExp)->continue_reset();
            (*itExp)->continuable_set();
        }

        if (e.is_returnable())
        {
            (*itExp)->returnable_set();
        }

        try
        {
            //reset default values
            result_set(NULL);
            expected_setSize(-1);
            (*itExp)->accept(*this);

            if (result_get() != NULL)
            {
                bool bImplicitCall = false;
                if (result_get()->isCallable()) //to manage call without ()
                {
                    Callable *pCall = ((InternalType*)result_get())->getAs<Callable>();
                    typed_list out;
                    typed_list in;
                    optional_list opt;

                    try
                    {
                        //in this case of calling, we can return only one values
                        int iSaveExpectedSize = expected_getSize();
                        expected_setSize(1);
                        Function::ReturnValue Ret = pCall->call(in, opt, expected_getSize(), out, this);
                        expected_setSize(iSaveExpectedSize);

                        if (Ret == Callable::OK)
                        {
                            if (out.size() == 0)
                            {
                                result_set(NULL);
                            }
                            else
                            {
                                result_set(out[0]);
                            }
                            bImplicitCall = true;
                        }
                        else if (Ret == Callable::Error)
                        {
                            if (ConfigVariable::getLastErrorFunction() == L"")
                            {
                                ConfigVariable::setLastErrorFunction(pCall->getName());
                                ConfigVariable::setLastErrorLine(e.location_get().first_line);
                                throw ScilabError();
                            }

                            if (pCall->isMacro() || pCall->isMacroFile())
                            {
                                wchar_t szError[bsiz];
                                os_swprintf(szError, bsiz, _W("at line % 5d of function %ls called by :\n"), (*itExp)->location_get().first_line, pCall->getName().c_str());
                                throw ScilabMessage(szError);
                            }
                            else
                            {
                                throw ScilabMessage();
                            }
                        }
                    }
                    catch (ScilabMessage sm)
                    {
                        wostringstream os;
                        PrintVisitor printMe(os);
                        (*itExp)->accept(printMe);
                        //os << std::endl << std::endl;
                        if (ConfigVariable::getLastErrorFunction() == L"")
                        {
                            ConfigVariable::setLastErrorFunction(pCall->getName());
                        }

                        if (pCall->isMacro() || pCall->isMacroFile())
                        {
                            wchar_t szError[bsiz];
                            os_swprintf(szError, bsiz, _W("at line % 5d of function %ls called by :\n"), sm.GetErrorLocation().first_line, pCall->getName().c_str());
                            throw ScilabMessage(szError + os.str());
                        }
                        else
                        {
                            sm.SetErrorMessage(sm.GetErrorMessage() + os.str());
                            throw sm;
                        }
                    }
                }

                SimpleVar* pVar = dynamic_cast<SimpleVar*>(*itExp);
                //don't output Simplevar and empty result
                if (result_get() != NULL && (pVar == NULL || bImplicitCall))
                {
                    //symbol::Context::getInstance()->put(symbol::Symbol(L"ans"), *execMe.result_get());
                    InternalType* pITAns = result_get();
                    symbol::Context::getInstance()->put(m_pAns, pITAns);
                    if ((*itExp)->is_verbose() && ConfigVariable::isPromptShow())
                    {
                        //TODO manage multiple returns
                        scilabWriteW(L" ans  =\n\n");
                        VariableToString(pITAns, L"ans");
                    }
                }
            }

            if ((&e)->is_breakable() && (*itExp)->is_break())
            {
                const_cast<SeqExp *>(&e)->break_set();
                break;
            }

            if ((&e)->is_continuable() && (*itExp)->is_continue())
            {
                const_cast<SeqExp *>(&e)->continue_set();
                break;
            }

            if ((&e)->is_returnable() && (*itExp)->is_return())
            {
                const_cast<SeqExp *>(&e)->return_set();
                (*itExp)->return_reset();
                break;
            }
        }
        catch (ScilabMessage sm)
        {
            scilabErrorW(sm.GetErrorMessage().c_str());

            CallExp* pCall = dynamic_cast<CallExp*>(*itExp);
            if (pCall != NULL)
            {
                //to print call expression only of it is a macro
                pCall->name_get().accept(*this);

                if (result_get() != NULL && (result_get()->isMacro() || result_get()->isMacroFile()))
                {
                    wostringstream os;
                    PrintVisitor printMe(os);
                    pCall->accept(printMe);
                    //os << std::endl << std::endl;
                    if (ConfigVariable::getLastErrorFunction() == L"")
                    {
                        ConfigVariable::setLastErrorFunction(((InternalType*)result_get())->getAs<Callable>()->getName());
                    }
                    throw ScilabMessage(os.str(), 0, (*itExp)->location_get());
                }
            }

            throw ScilabMessage((*itExp)->location_get());
        }
        catch (ScilabError se)
        {
            // check on error number because error message can be empty.
            if (ConfigVariable::getLastErrorNumber() == 0)
            {
                ConfigVariable::setLastErrorMessage(se.GetErrorMessage());
                ConfigVariable::setLastErrorNumber(se.GetErrorNumber());
                ConfigVariable::setLastErrorLine(se.GetErrorLocation().first_line);
                ConfigVariable::setLastErrorFunction(wstring(L""));
            }

            CallExp* pCall = dynamic_cast<CallExp*>(*itExp);
            if (pCall != NULL)
            {
                //to print call expression only of it is a macro
                try
                {
                    pCall->name_get().accept(*this);
                    if (ConfigVariable::getWhereDeep() > 0 &&  result_get() != NULL && (result_get()->isMacro() || result_get()->isMacroFile()))
                    {
                        wostringstream os;
                        PrintVisitor printMe(os);
                        pCall->accept(printMe);
                        //os << std::endl << std::endl;
                        ConfigVariable::setLastErrorFunction(((InternalType*)result_get())->getAs<Callable>()->getName());
                        scilabErrorW(se.GetErrorMessage().c_str());
                        throw ScilabMessage(os.str(), 999, (*itExp)->location_get());
                    }
                }
                catch (ScilabError se2)
                {
                    //just to catch exception, do nothing
                }
            }

            scilabErrorW(se.GetErrorMessage().c_str());
            scilabErrorW(L"\n");
            throw ScilabMessage((*itExp)->location_get());
        }
        result_set(NULL);
    }
}

template <class T>
void RunVisitorT<T>::visitprivate(const NotExp &e)
{
    /*
    @ or ~ !
    */
    e.exp_get().accept(*this);

    InternalType * pValue = result_get();
    InternalType * pReturn = NULL;
    if (pValue->neg(pReturn))
    {
        if (pValue->isDeletable())
        {
            delete pValue;
        }

        result_set(pReturn);
    }
    else
    {
        // neg returned false so the negation is not possible so we call the overload (%foo_5)
        types::typed_list in;
        types::typed_list out;

        pValue->IncreaseRef();
        in.push_back(pValue);

        Callable::ReturnValue Ret = Overload::call(L"%" + result_get()->getShortTypeStr() + L"_5", in, 1, out, this);
        if (Ret != Callable::OK)
        {
            throw ScilabError();
        }

        if (out.size() == 0)
        {
            result_set(NULL);
        }
        else if (out.size() == 1)
        {
            out[0]->DecreaseRef();
            result_set(out[0]);
        }
        else
        {
            for (int i = 0 ; i < static_cast<int>(out.size()) ; i++)
            {
                out[i]->DecreaseRef();
                result_set(i, out[i]);
            }
        }

        pValue->DecreaseRef();
    }
}

template <class T>
void RunVisitorT<T>::visitprivate(const TransposeExp &e)
{
    e.exp_get().accept(*this);

    InternalType * pValue = result_get();
    InternalType * pReturn = NULL;
    const bool bConjug = e.conjugate_get() == TransposeExp::_Conjugate_;

    if ((bConjug && pValue->adjoint(pReturn)) || (!bConjug && pValue->transpose(pReturn)))
    {
        if (pValue->isDeletable())
        {
            delete pValue;
        }

        result_set(pReturn);

        return;
    }
    else
    {
        // transpose returned false so the negation is not possible so we call the overload (%foo_t or %foo_0)
        types::typed_list in;
        types::typed_list out;

        pValue->IncreaseRef();
        in.push_back(pValue);

        Callable::ReturnValue Ret;
        if (bConjug)
        {
            Ret = Overload::call(L"%" + result_get()->getShortTypeStr() + L"_t", in, 1, out, this);
        }
        else
        {
            Ret = Overload::call(L"%" + result_get()->getShortTypeStr() + L"_0", in, 1, out, this);
        }

        if (Ret != Callable::OK)
        {
            throw ScilabError();
        }

        if (out.size() == 0)
        {
            result_set(NULL);
        }
        else if (out.size() == 1)
        {
            out[0]->DecreaseRef();
            result_set(out[0]);
        }
        else
        {
            for (int i = 0 ; i < static_cast<int>(out.size()) ; i++)
            {
                out[i]->DecreaseRef();
                result_set(i, out[i]);
            }
        }

        pValue->DecreaseRef();
    }
}

template <class T>
void RunVisitorT<T>::visitprivate(const FunctionDec  &e)
{
    /*
    function foo
    endfunction
    */

    // funcprot(0) : do nothing
    // funcprot(1) && warning(on) : warning
    // funcprot(2) : error
    if (ConfigVariable::getFuncprot() == 1 && ConfigVariable::getWarningMode())
    {
        types::InternalType* pITFunc = symbol::Context::getInstance()->get(((FunctionDec&)e).stack_get());

        if (pITFunc && pITFunc->isCallable())
        {
            wchar_t pwstFuncName[1024];
            os_swprintf(pwstFuncName, 1024, L"%-24ls", e.name_get().name_get().c_str());
            char* pstFuncName = wide_string_to_UTF8(pwstFuncName);

            sciprint(_("Warning : redefining function: %s. Use funcprot(0) to avoid this message"), pstFuncName);
            sciprint("\n");
            FREE(pstFuncName);
        }
    }
    else if (ConfigVariable::getFuncprot() == 2)
    {
        types::InternalType* pITFunc = symbol::Context::getInstance()->get(((FunctionDec&)e).stack_get());

        if (pITFunc && pITFunc->isCallable())
        {
            char pstError[1024];
            char* pstFuncName = wide_string_to_UTF8(e.name_get().name_get().c_str());
            sprintf(pstError, _("It is not possible to redefine the %s primitive this way (see clearfun).\n"), pstFuncName);
            wchar_t* pwstError = to_wide_string(pstError);
            std::wstring wstError(pwstError);
            FREE(pstFuncName);
            FREE(pwstError);
            throw ScilabError(wstError, 999, e.location_get());
        }
    }

    std::list<Var *>::const_iterator	i;

    //get input parameters list
    std::list<symbol::Variable*> *pVarList = new std::list<symbol::Variable*>();
    const ArrayListVar *pListVar = &e.args_get();
    for (i = pListVar->vars_get().begin() ; i != pListVar->vars_get().end() ; i++)
    {
        pVarList->push_back(static_cast<SimpleVar*>(*i)->stack_get());
    }

    //get output parameters list
    std::list<symbol::Variable*> *pRetList = new std::list<symbol::Variable*>();
    const ArrayListVar *pListRet = &e.returns_get();
    for (i = pListRet->vars_get().begin() ; i != pListRet->vars_get().end() ; i++)
    {
        pRetList->push_back(static_cast<SimpleVar*>(*i)->stack_get());
    }

    //            Location* newloc = const_cast<Location*>(&location_get())->clone();
    Exp* exp = const_cast<Exp*>(&e.body_get())->clone();

    //MuteVisitor mute;
    //exp->accept(mute);

    //types::Macro macro(VarList, RetList, (SeqExp&)e.body_get());
    types::Macro *pMacro = new types::Macro(e.name_get().name_get(), *pVarList, *pRetList,
                                            static_cast<SeqExp&>(*exp), L"script");
    pMacro->setFirstLine(e.location_get().first_line);
    symbol::Context::getInstance()->addMacro(pMacro);
}

template <class T>
void RunVisitorT<T>::visitprivate(const ListExp &e)
{
    try
    {
        e.start_get().accept(*this);
        GenericType* pITStart = static_cast<GenericType*>(result_get());
        if (pITStart->getRows() != 1 || pITStart->getCols() != 1)
        {
            throw 1;
        }
        InternalType* piStart = result_get();

        e.step_get().accept(*this);
        GenericType* pITStep = static_cast<GenericType*>(result_get());
        if (pITStep->getRows() != 1 || pITStep->getCols() != 1)
        {
            throw 2;
        }
        InternalType* piStep = result_get();

        e.end_get().accept(*this);
        GenericType* pITEnd = static_cast<GenericType*>(result_get());
        if (pITEnd->getRows() != 1 || pITEnd->getCols() != 1)
        {
            throw 3;
        }
        InternalType* piEnd = result_get();

        //check compatibility

        if (piStart->isInt())
        {
            //if Step or End are Int too, they must have the same precision
            if (piStep->isInt())
            {
                if (piStep->getType() != piStart->getType())
                {
                    throw ScilabError(_W("Undefined operation for the given operands.\n"), 999, e.step_get().location_get());
                }
            }
            else if (piStep->isPoly())
            {
                throw ScilabError(_W("Undefined operation for the given operands.\n"), 999, e.step_get().location_get());
            }


            if (piEnd->isInt())
            {
                if (piEnd->getType() != piStart->getType())
                {
                    throw ScilabError(_W("Undefined operation for the given operands.\n"), 999, e.end_get().location_get());
                }
            }
            else if (piEnd->isPoly())
            {
                throw ScilabError(_W("Undefined operation for the given operands.\n"), 999, e.end_get().location_get());
            }
        }
        else if (piStart->isPoly())
        {
            if (piStep->isInt())
            {
                throw ScilabError(_W("Undefined operation for the given operands.\n"), 999, e.step_get().location_get());
            }

            if (piEnd->isInt())
            {
                throw ScilabError(_W("Undefined operation for the given operands.\n"), 999, e.end_get().location_get());
            }
        }
        else if (piStep->isInt())
        {
            //if End is Int too, they must have the same precision
            if (piEnd->isInt())
            {
                if (piEnd->getType() != piStep->getType())
                {
                    throw ScilabError(_W("Undefined operation for the given operands.\n"), 999, e.end_get().location_get());
                }
            }
        }
        else if (piStep->isPoly())
        {
            if (piEnd->isInt())
            {
                throw ScilabError(_W("Undefined operation for the given operands.\n"), 999, e.step_get().location_get());
            }
        }

        if (piStart->isDouble() && piStart->getAs<Double>()->isComplex())
        {
            throw 1;
        }

        if (piStep->isDouble() && piStep->getAs<Double>()->isComplex())
        {
            throw 2;
        }

        if (piEnd->isDouble() && piEnd->getAs<Double>()->isComplex())
        {
            throw 3;
        }

        ImplicitList *pIL = new ImplicitList(piStart, piStep, piEnd);

        result_set(pIL);

        if (piStart && piStart->isDeletable())
        {
            delete piStart;
        }

        if (piStep && piStep->isDeletable())
        {
            delete piStep;
        }

        if (piEnd && piEnd->isDeletable())
        {
            delete piEnd;
        }
    }
    catch (int iPos)
    {
        wchar_t szError[bsiz];
        os_swprintf(szError, bsiz, _W("%ls: Wrong type for argument %d: Real scalar expected.\n"), L"':'", iPos);
        throw ScilabError(szError, 999, e.location_get());
    }
    catch (ScilabError error)
    {
        //TODO YaSp : Overloading
        throw error;
    }
}

#include "run_CallExp.cpp"
#include "run_MatrixExp.cpp"
#include "run_OpExp.cpp"
#include "run_AssignExp.cpp"
}

template EXTERN_AST class ast::RunVisitorT<ast::ExecVisitor>;
template EXTERN_AST class ast::RunVisitorT<ast::StepVisitor>;
template EXTERN_AST class ast::RunVisitorT<ast::TimedVisitor>;
